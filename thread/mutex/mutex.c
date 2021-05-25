#include <stdio.h>
#include <pthread.h>

int count = 0;
pthread_mutex_t count_lock;

void *thread_count1(void *arg)
{
	int i = 0;

	for (i = 0; i < 100000; i++) {
		pthread_mutex_lock(&count_lock);
		count++;
		pthread_mutex_unlock(&count_lock);
	}

	return NULL;
}

void *thread_count2(void *arg)
{
	int i = 0;

	for (i = 0; i < 100000; i++) {
		pthread_mutex_lock(&count_lock);
		count++;
		pthread_mutex_unlock(&count_lock);
	}

	return NULL;
}

void main(void)
{
	pthread_t thread1;
	pthread_t thread2;

	pthread_mutex_init(&count_lock, NULL);

	pthread_create(&thread1, NULL, thread_count1, NULL);
	pthread_create(&thread2, NULL, thread_count2, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	pthread_mutex_destroy(&count_lock);

	printf("\ncount = %d\n", count);
}
