#!/bin/bash

# Check if correct number of arguments are provided
if [ $# -ne 2 ]; then
    echo "Usage: $0 <main_string> <substring>"
    exit 1
fi

# Assign arguments to variables
main_string="$1"
substring="$2"

# Initialize occurrence count
count=0

# Find all occurrences and their positions
echo "Searching for '$substring' in '$main_string'"

# Use grep to find all occurrences with their positions
positions=($(echo "$main_string" | grep -o -b "$substring" | cut -d: -f1))

# Calculate number of occurrences
count=${#positions[@]}

# Print results
if [ $count -eq 0 ]; then
    echo "Substring not found."
else
    echo "Number of occurrences: $count"
    echo "Positions (0-indexed):"
    for pos in "${positions[@]}"; do
        echo "- Position: $pos"
    done
fi
