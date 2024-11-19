#!/bin/bash

read char

case "$char" in
	[aeiouAEIOU])
		echo "Vowel" ;;
	[[:alpha:]])
		echo "Not a Vowel" ;;

	*)
		echo "Invalid char"

esac

