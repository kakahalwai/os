#include <stdio.h>
#include <string.h>

int main()
{


	char str[100];
	printf("Enter to check for pali: ");
	scanf("%99s", str);
	int len = strlen(str); // to avoid buffer overflow

	for(int i = 0; i < len / 2; i++)
	{
	
		if(str[i] != str[len - i - 1])
		{
			printf("Not a plai");
			return 0;
				
		}
	}

	printf("Is a pali");
	return 0;
}
