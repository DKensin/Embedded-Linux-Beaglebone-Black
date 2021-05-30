#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE *fp;

	fp = fopen ("data.txt", "w");
	if (fp == NULL)
	{
		printf("Error!\n");
		exit(1);
	}

	fputs("Hello World\n", fp);

	fclose(fp);

	return 0;
}
