#include <stdio.h>
#include <pthread.h>
#include <string.h>

int count;
pthread_mutex_t count_lock;

void *my_thread(void *arg)
{
	int i = 0;

	pthread_mutex_lock(&count_lock);
	for (i = 0; i < 50000; i++)
		count++;
	pthread_mutex_unlock(&count_lock);

	return NULL;
}

void main(void)
{
	pthread_t thread1;
	pthread_t thread2;

	pthread_mutex_init(&count_lock, NULL);

	pthread_create(&thread1, NULL, my_thread, NULL);
	pthread_create(&thread2, NULL, my_thread, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_mutex_destroy(&count_lock);

	printf("count = %d\n", count);
}
