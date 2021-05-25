#include <stdio.h>

int sum()
{
	int n = 0, i = 0;

	for (i = 0; i <= 101; i++)
	{
		n += i;
	}

	return n;
}

void main()
{
	printf("Sum = %d\n", sum() );
}
