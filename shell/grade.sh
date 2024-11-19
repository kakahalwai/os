read marks

if [ $marks -lt 0 ] || [ $marks -gt 100 ]; then
	echo "between 0 and 100"
	exit 1
fi

if [ $marks -ge 90 ]; then
	grade="O"
elif [ $marks -ge 80 ]; then
	grade="A"
elif [ $marks -ge 70 ]; then
	grade="B"
elif [ $marks -ge 60 ]; then
	grade="C"
else 
	grade="F"
fi

echo $grade
