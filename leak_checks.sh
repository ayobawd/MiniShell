#!/bin/sh

if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <executable> [args...]"
    exit 1
fi

EXECUTABLE="$1"

shift

valgrind --leak-check=full --leak-resolution=high -s --track-origins=yes \
         --num-callers=500 --show-mismatched-frees=yes --show-leak-kinds=all \
         --track-fds=yes --trace-children=yes --gen-suppressions=no \
         --error-limit=no --undef-value-errors=yes --expensive-definedness-checks=yes \
         --read-var-info=yes --keep-debuginfo=yes \
         --suppressions=bin.supp --suppressions=readline.supp \
         "$EXECUTABLE" "$@"