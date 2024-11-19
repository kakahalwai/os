#!/usr/bin/awk -f

# Initialize counters
BEGIN {
    char_count = 0
    word_count = 0
    line_count = 0
}

# Process each line
{
    line_count++
    char_count += length($0)  # Count characters in the current line
    word_count += NF          # Count words in the current line
}

# Print the results after processing all lines
END {
    print "Lines:", line_count
    print "Words:", word_count
    print "Characters:", char_count
}
