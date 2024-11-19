#!/bin/bash


count_file_stats() {
    local filename="$1"


    echo "Hello, this is a sample text file.
    It contains multiple lines of text.
    Let's count the number of lines, words, and characters." > "$filename"


    local lines=$(wc -l < "$filename")
    local words=$(wc -w < "$filename")
    local characters=$(wc -m < "$filename")


    echo "File: $filename"
    echo "Number of lines: $lines"
    echo "Number of words: $words"
    echo "Number of characters: $characters"
}


filename="sample.txt"


count_file_stats "$filename"

