#!/bin/bash

# Minishell Test Runner Script
# Integrates LucasKuhn/minishell_tester as the primary test suite

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# Check if minishell is built
if [ ! -f "./minishell" ]; then
    print_status "Building minishell..."
    make
    if [ $? -ne 0 ]; then
        print_error "Failed to build minishell"
        exit 1
    fi
    print_success "Minishell built successfully"
fi

# Check if tester exists
if [ ! -d "./minishell_tester" ]; then
    print_status "Cloning minishell_tester..."
    git clone https://github.com/LucasKuhn/minishell_tester.git
    if [ $? -ne 0 ]; then
        print_error "Failed to clone minishell_tester"
        exit 1
    fi
    print_success "Minishell tester cloned successfully"
fi

# Make tester executable
chmod +x ./minishell_tester/tester

# Run tests based on argument
cd minishell_tester

if [ "$1" = "all" ] || [ -z "$1" ]; then
    print_status "Running all mandatory tests..."
    ./tester
elif [ "$1" = "norminette" ]; then
    print_status "Running Norminette compliance check..."
    cd ..
    if command -v norminette >/dev/null 2>&1; then
        norminette *.h src/ || true
    else
        print_warning "Norminette not found. Install with: pip3 install norminette"
    fi
else
    print_status "Running $1 tests..."
    ./tester "$1"
fi

exit 0