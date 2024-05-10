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

chmod 777 "$path"

nr_of_words=$(wc -w < "$path")
nr_of_lines=$(wc -l < "$path")
nr_of_char=$(wc -m < "$path")


if [ $nr_of_lines -ge 3 ] || [ $nr_of_words -le 1000 ] || [ $nr_of_char -le 2000 ]; then
    echo "SAFE"
    chmod 000 "$path"
    exit 0
fi


if grep -qP "[^\x80-\xFF]" "$1"; then
    echo "$path"
    exit 0
fi


if grep -q -E '\b(malicious|risk|attack)\b' "$path"; then
    echo "$path"
    exit 0
fi

chmod 000 "$path"

echo "SAFE"
exit 0