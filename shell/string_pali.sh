#!/bin/bash

pali() {
	local input=$1
	local len=${#input}
	local reversed=""


	for (( i = len - 1; i >= 0; i--)); do
		reversed="$reversed${input:$i:1}"
	done

	if [ "$input" = "$reversed" ]; then
		echo "Yes"
	else
		echo "No"
	fi

}

read n

pali "$n"
