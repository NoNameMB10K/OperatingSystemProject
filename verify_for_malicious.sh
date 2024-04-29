#!/bin/bash

if [ $# -lt 2 ]; then
    echo "Usage: $0 <path>"
    exit 1
fi

path="$1"
safe_dir="$2"

if [ ! -f "$path" ]; then
    echo "Error: File '$path' does not exist."
    exit 1
fi

while read -n1 char; do
    if [ "$(printf '%d' "'$char")" -gt 127 ]; then
        echo "Characters with ASCII code > 127 found in the file '$path'."
        mv "$path" "$safe_dir/$path"
        exit 0
    fi
done < "$path"

echo "No characters with ASCII code > 127 found in the file '$path'."


