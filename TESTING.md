# Minishell Testing and Compliance Guide

This document outlines the testing and compliance workflow for the Minishell project, integrating [LucasKuhn/minishell_tester](https://github.com/LucasKuhn/minishell_tester) as the primary test suite.

## Prerequisites

1. **Build the project**: Always run `make` first before testing
2. **Install norminette** (for compliance checking): `pip3 install norminette`

## Testing Commands

### Run All Tests
```bash
make test
```
This runs the complete mandatory test suite including:
- builtins
- pipes  
- redirects
- extras

### Run Specific Test Categories
```bash
make test-builtins    # Test built-in commands
make test-pipes       # Test pipe functionality
make test-redirects   # Test redirection functionality  
make test-extras      # Test additional features
```

### Run Norminette Compliance Check
```bash
make norminette
```
Checks all source files for 42 School coding standard compliance.

### Using the Test Runner Script Directly
```bash
./run_tests.sh [category]
```
Available categories: `all`, `builtins`, `pipes`, `redirects`, `extras`, `norminette`

## Compliance Requirements

According to the project requirements, each commit must:

1. ✅ **Pass Norminette 100%** - No coding standard violations
2. ✅ **Pass all mandatory Minishell tests 100%** - All functional tests must pass

## Development Workflow

1. **Before making changes**: Run `make test` to establish baseline
2. **After code changes**: 
   - Run `make` to rebuild
   - Run `make norminette` to check compliance
   - Run `make test` or specific test categories
3. **Before committing**: Ensure both norminette and tests pass 100%

## Test Suite Details

The LucasKuhn/minishell_tester includes:
- **Mandatory tests**: Core functionality required for the project
- **Bonus tests**: Additional features like wildcards (`./tester wildcards`)
- **Manual tests**: Suggestions in `minishell_tester/manual_tests/`
- **Extra tests**: Syntax errors and OS-specific behavior

## Notes

- The tester compares minishell output with bash output
- Tests check exit codes, stdout, stderr, and file outputs
- Global variables detected by norminette are acceptable for this project
- The test runner automatically clones the tester if not present

## Troubleshooting

If tests hang or timeout:
1. Check if minishell builds correctly: `make`
2. Test basic functionality manually: `echo "echo hello" | ./minishell`
3. Run individual test categories to isolate issues
4. Check the manual tests directory for additional validation

## Integration

The testing infrastructure is integrated into the Makefile and can be easily used in CI/CD pipelines or development workflows.