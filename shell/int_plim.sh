read n

original=$n
reverse=0

while [ $n -gt 0 ]; do
	remainder=$(( n % 10))
	reverse=$(( reverse * 10 + remainder ))
	n=$(( n / 10))
done

if [ $original -eq $reverse ]; then
	echo "Plaindrome"
else
       	echo "No palindrome"

fi

