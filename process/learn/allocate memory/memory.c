/*
 * Program to calculate sum of n number entered by user
 * example use malloc() and free()
 */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int n, i, sum = 0;
	int *ptr = NULL;

	printf("Enter number of elements: ");
	scanf("%d", &n);

	ptr = (int *) malloc(n * sizeof(int));

	if (ptr == NULL) {
		printf("Error! memory not allocated.\n");
		exit(0);
	}

	printf("Enter elements: ");
	for (i = 0; i < n; i++) {
		scanf("%d", ptr + i);
		sum += *(ptr + i);
	}

	printf("Sum = %d\n", sum);

	free(ptr);

	return 0;
}
