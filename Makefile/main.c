#include <stdio.h>
#include "header.h"

void main(void)
{
	printf("\nI am in main.c\n");
	printf("\nCalling hello function.\n");
	hello();
	printf("\nCalling add function.\n");
	add(10, 15);
	printf("\nBack in main.\n");
}
