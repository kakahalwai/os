#!/bin/bash

search() {


	local arr=("$@")
	local search_ele=$1
	local found=false

	for item in "${arr[@]}"; do
		if [ "$item" -eq "$search_ele" ]; then
			found=true
			break
		fi
	done

	if [ "$found" = true ]; then
		echo "Found"
	else
		echo "Not Found"
	fi
}

read -p "Enter elements: " n

declare -a elements


echo "Enter $n elements: "
for ((i = 0; i < n; i++)); do
	read element
	elements+=("$element")
done

read -p "Element to search? " search

search "${elements[@]}"
