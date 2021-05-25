#include <stdio.h>

void main(void)
{
	FILE *fp;
	char a[] = "World ";

	fp = fopen("data.txt", "r+");

	fseek(fp, 6, SEEK_SET);
	fputs("World ", fp);

	fclose(fp);
}
