# Memory Leak Fixes for MiniShell

## Overview
This document describes the memory leaks that were identified and fixed in the MiniShell project using Valgrind.

## Issues Identified

### 1. File Descriptor Leaks in Child Processes
**Location**: `src/execution/pipelines_exec.c`

**Problem**: When executing commands, the `execute_single_command_with_redirections` function would duplicate stdin/stdout file descriptors for restoration later. However, when forking to execute commands, the child process would inherit these duplicated file descriptors but never close them before exiting.

**Fix**: Modified `fork_and_execute_command` to:
- Accept the saved file descriptors as parameters
- Close them in the child process immediately after forking
- This prevents file descriptor leaks in child processes

**Code Changes**:
```c
// Before: Child process would inherit saved FDs and never close them
pid = fork();
if (pid == 0)
{
    // setup and execute command
    exit(...);
}

// After: Child process closes saved FDs before executing
pid = fork();
if (pid == 0)
{
    close(saved_stdin);
    close(saved_stdout);
    // setup and execute command
    exit(...);
}
```

### 2. Environment Variables Not Freed on Exit
**Location**: `minishell.c`, `src/execution/builtins_exit.c`

**Problem**: When the shell exits (either via Ctrl+D or the `exit` command), the environment variables stored in a linked list were never freed, causing a memory leak.

**Fix**: 
- Added `free_environment()` calls in both exit paths:
  - When user presses Ctrl+D (readline returns NULL)
  - When user types `exit` command (via `builtin_exit`)
- Modified `builtin_exit` to accept the shell structure so it can clean up resources

**Code Changes**:
```c
// In minishell.c - shell_loop()
if (!input)
{
    printf("exit\n");
    free_environment(s);      // Added
    rl_clear_history();       // Added
    return ;
}

// In builtins_exit.c - cleanup_before_exit()
static void cleanup_before_exit(t_shell *shell)
{
    // ... close file descriptors ...
    free_environment(shell);   // Added
    rl_clear_history();        // Added
}
```

### 3. Readline History Not Cleared
**Location**: `minishell.c`, `src/execution/builtins_exit.c`

**Problem**: The readline library maintains a history of commands, which was not cleared on exit.

**Fix**: Added `rl_clear_history()` calls in both exit paths (same locations as environment cleanup).

## Results

### Before Fixes
```
HEAP SUMMARY:
    in use at exit: 236,058 bytes in 549 blocks
  total heap usage: 762 allocs, 213 frees

LEAK SUMMARY:
   definitely lost: 0 bytes in 0 blocks
   still reachable: 29,212 bytes in 332 blocks

FILE DESCRIPTORS: 6 open (3 std) at exit (when executing commands)
```

### After Fixes
```
HEAP SUMMARY:
    in use at exit: 220,729 bytes in 229 blocks
  total heap usage: 762 allocs, 533 frees

LEAK SUMMARY:
   definitely lost: 0 bytes in 0 blocks
   still reachable: 13,883 bytes in 12 blocks

FILE DESCRIPTORS: 4 open (3 std) at exit
```

### Improvements
- **320 more blocks freed** (from 332 remaining to 12 remaining)
- **52% reduction** in still-reachable memory (from 29,212 to 13,883 bytes)
- **File descriptor leaks eliminated** (from 6 to 4 FDs, with extra 2 being leaked saved FDs)
- **0 definitely lost bytes** (perfect score for critical leaks)
- **0 errors** from Valgrind

## Remaining "Still Reachable" Memory

The remaining ~13,883 bytes are primarily from:
1. **Library allocations** (~13,808 bytes): libtinfo and libreadline internal allocations that cannot be freed
2. **Last command parsing** (~75 bytes): Memory allocated while parsing the "exit" command, which is freed by the OS on process termination

These remaining allocations are acceptable because:
- They only occur at program exit
- The OS reclaims all memory when the process terminates
- Fixing them would require significant refactoring for minimal benefit

## Testing

To verify the fixes, run:
```bash
./sh.sh
```

Or manually:
```bash
valgrind --suppressions=valgrind_readline_leaks_ignore.txt \
    --leak-check=full --show-leak-kinds=all \
    --track-origins=yes --track-fds=yes \
    --show-reachable=no ./minishell
```

## Files Modified

1. `src/execution/pipelines_exec.c` - Fixed file descriptor leaks in child processes
2. `minishell.c` - Added cleanup on Ctrl+D exit
3. `src/execution/builtins_exit.c` - Added cleanup in exit builtin
4. `minishell.h` - Updated function signatures
5. `src/execution/builtins.c` - Updated function call
6. `.gitignore` - Added to exclude build artifacts
