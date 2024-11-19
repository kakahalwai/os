#!/bin/bash

n=(5 3 9 10 9 2 1 4 6 8)


sorted_number=($(echo "${n[@]} " | tr ' ' '\n' | sort -rn))

second=${sorted_number[1]}

echo $second
