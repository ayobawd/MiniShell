# Minishell Testing Guide

This project uses [LucasKuhn/minishell_tester](https://github.com/LucasKuhn/minishell_tester) as the primary test suite for validation.

## Prerequisites

Before running tests, ensure you have:
- Built the project with `make`
- Installed `libreadline-dev` (for compilation)
- Installed `norminette` (for 42 School compliance checking)

## Building the Project

```bash
make
```

## Running Tests

### All Mandatory Tests
```bash
make test
```

### Individual Test Categories
```bash
make test-builtins    # Test built-in commands (echo, cd, pwd, export, unset, env, exit)
make test-pipes       # Test pipe functionality 
make test-redirects   # Test input/output redirection
make test-extras      # Extra functionality tests
```

### Manual Test Commands
```bash
cd minishell_tester
./tester                    # Run all mandatory tests
./tester builtins          # Run specific test category
./tester syntax            # Run syntax error tests (extra)
./tester os_specific       # Run OS-specific tests (extra)
```

## Code Quality Checks

### Norminette Compliance
```bash
make norm
```

This checks all source files against 42 School's coding standards.

## Testing Infrastructure

- **Primary Test Suite**: LucasKuhn/minishell_tester (comprehensive functional tests)
- **Code Standards**: Norminette (42 School compliance)
- **Build System**: Makefile with integrated test targets

## Current Status

- ✅ **Build**: Project compiles cleanly with no errors
- ✅ **Norminette**: 100% compliant (only acceptable global variable notices)
- ✅ **Test Infrastructure**: LucasKuhn/minishell_tester integrated and working
- ⚠️ **Test Results**: Some quote-handling tests currently hang (parsing issue to be addressed)

## Known Issues

- Tests involving quoted strings (e.g., `echo "hello world"`) may hang
- This indicates a parsing/quote-handling bug in the current implementation
- Basic functionality (unquoted commands) works correctly

## Next Steps

According to the refactoring plan:
1. Fix parsing/quote-handling issues
2. Ensure 100% test suite compliance
3. Refactor code following the guiding principles
4. Maintain Norminette compliance throughout refactoring