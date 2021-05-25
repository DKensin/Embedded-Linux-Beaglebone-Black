#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for sleep() */
#include <pthread.h>

void *mythread_func(void *arg)
{
	sleep(1);
	printf("Printing Hello from Thread\n");
	return NULL;
}

void main(void)
{
	pthread_t thread_id;

	printf("Before Thread\n");
	pthread_create(&thread_id, NULL, mythread_func, NULL);
	pthread_join(thread_id, NULL);
	printf("After Thread\n");
}
