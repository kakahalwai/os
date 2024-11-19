#!/usr/bin/awk -f

# Define the vowel pattern
BEGIN {
    vowel_pattern = /[aeiouAEIOU]/
    count = 0
}

# Process each line
{
    if ($0 !~ vowel_pattern) {
        count++
    }
}

# Print the result after processing all lines
END {
    print count
}
