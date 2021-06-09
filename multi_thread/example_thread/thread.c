#include <stdio.h>
#include <pthread.h>
#include <string.h>

void *my_thread(void *arg)
{
	printf("Hello world %s\n", (char *)arg);

	return arg;
}

void main(void)
{
	pthread_t thread1;
	pthread_t thread2;
	char a[100] = { 0 };

	memset(a, 0, sizeof(a));

	pthread_create(&thread1, NULL, my_thread, "Phu");
	pthread_create(&thread2, NULL, my_thread, "Toan");

	pthread_join(thread1, (void *)&a);
	printf("thread 1 return %s\n", a);
	pthread_join(thread2, (void *)&a);
	printf("thread 2 return %s\n", a);
}
