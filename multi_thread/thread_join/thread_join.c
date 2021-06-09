#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

void *roll_dice()
{
	int value = (rand() % 6) + 1;
	int *result = malloc(sizeof(int));

	*result = value;
	printf("In main thread function:\n");
	printf("pointer: %p\n", result);
	printf("value = %d\n", value);

	return (void*)result;
}

int main(void)
{
	int *res;
	srand(time(NULL));
	pthread_t th;

	if (pthread_create(&th, NULL, &roll_dice, NULL) != 0) {
		return 1;
	}

	if (pthread_join(th, (void**)(&res)) != 0) {
		return 2;
	}

	printf("\n\nIn main function:\n");
	printf("pointer: %p\n", res);
	printf("result = %d\n", *res);
	/* because we malloc in main thread function, we must de-allocate
	 * but in thread function, we return this pointer was allocate
	 * so we must de-allocate in here
	 */
	free(res);

	return 0;
}
