#!/bin/bash


fact(){
	local num=$1	
	if [ $num -lt 0 ]; then
		echo "enter number greator than zero!"
		
	fi
	
	if [ $num -le 1 ]; then
		echo 1
	else
		local result=$((num * $(fact $((num - 1))) ))
		echo $result
	fi		

}

echo "Enter Number: "

read n

result=$(fact $n)

echo $result
