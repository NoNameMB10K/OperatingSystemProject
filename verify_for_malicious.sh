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

# echo "Number of words: $nr_of_words"
# echo "Number of lines: $nr_of_lines"
# echo "Number of characters: $nr_of_char"

while read -n1 char; do
    if [ "$(printf '%d' "'$char")" -gt 127 ]; then
        echo "$path"
        exit 0
    fi
done < "$path"

while IFS= read -r word; do
    if [[ "$word" == *"malicious"* || "$word" == *"risk"* || "$word" == *"attack"* ]]; then
        echo "$path"
        exit 0
    fi
done < <(tr -cs '[:alnum:]' '\n' < "$path")

echo "SAFE"
exit 0