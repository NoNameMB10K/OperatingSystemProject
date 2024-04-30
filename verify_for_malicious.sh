#!/bin/bash

# Concatenate all arguments into a single line
args_line=""
for arg in "$@"; do
    args_line+="$arg "
done

# Print the concatenated arguments line
echo "All arguments in a single line: $args_line"
