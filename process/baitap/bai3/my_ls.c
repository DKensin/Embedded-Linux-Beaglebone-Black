#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Invalid command line argument.\n");
		exit(1);
	}
	if (strcmp(argv[1], "-l") == 0)
		printf("full property\n");
	else if (strcmp(argv[1], "-a") == 0)
		printf("display hidden file\n");
	else if (strcmp(argv[1], "-la") == 0) {
		printf("full property\n");
		printf("display hidden file\n");
	}
	else
		printf("incorrect flag\n");

	return 0;
}
