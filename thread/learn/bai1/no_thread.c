#include <stdio.h>

void main(void)
{
	int i, n;

	for (i = 1; i <= 100000000000; i+=2)
		n += i;

	printf("Tong = %d\n", n);
}
