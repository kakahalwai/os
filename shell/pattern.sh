#!/bin/bash

read -p "Enter the filename: " filename

if [ ! -f "$filename" ]; then
    echo "File not found!"
    exit 1
fi

read -p "Enter the pattern to search: " pattern

grep -n "$pattern" "$filename"

if [ $? -eq 0 ]; then
    echo "Pattern found in the file."
else
    echo "Pattern not found in the file."
fi

