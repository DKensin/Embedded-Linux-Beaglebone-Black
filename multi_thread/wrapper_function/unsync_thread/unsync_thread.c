#include <stdio.h>
#include <pthread.h>
#include <string.h>

int count;

void *my_thread(void *arg)
{
	int i = 0;

	for (i = 0; i < 50000; i++)
		count++;

	return NULL;
}

void main(void)
{
	pthread_t thread1;
	pthread_t thread2;

	pthread_create(&thread1, NULL, my_thread, NULL);
	pthread_create(&thread2, NULL, my_thread, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("count = %d\n", count);
}
