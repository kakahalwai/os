#!/bin/bash

bubble_sort() {
    local arr=("$@")
    local len=${#arr[@]}
    local swapped

    for ((i = 0; i < len - 1; i++)); do
        swapped=false
        for ((j = 0; j < len - i - 1; j++)); do
            if [ "${arr[j]}" -gt "${arr[j+1]}" ]; then
                # Swap elements
                temp=${arr[j]}
                arr[j]=${arr[j+1]}
                arr[j+1]=$temp
                swapped=true
            fi
        done

        
        if [ "$swapped" = false ]; then
            break
        fi
    done

    echo "${arr[@]}"
}


read -p "Enter the number of elements in the array: " n


declare -a elements


echo "Enter $n elements:"
for ((i = 0; i < n; i++)); do
    read element
    elements+=("$element")
done


sorted_array=$(bubble_sort "${elements[@]}")


echo "Sorted array:"
echo "$sorted_array"



