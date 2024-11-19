#!/usr/bin/awk -f

BEGIN{
	FS = ","; #field seperator is a , now
}

NR == 1{
	print $0 ",Grade";
	next;
}

{
	grade = "Pass";

	for(i = 2; i <= NF; i++)
	{
		if($i < 40)
		{
			grade = "Fail";
			break;
		}		
	}

	print $0 "," grade;
}
