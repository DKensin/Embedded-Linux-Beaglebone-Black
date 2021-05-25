#include <stdio.h>
#include <pthread.h>

void *nhap_a(void *arg)
{
	int a = 0;
	printf("\nNhap a: ");
	scanf("%d", &a);
	printf("\na = %d\n", a);

	return NULL;
}

void *print_hello(void *arg)
{
	printf("Hello world\n");

	return NULL;
}

void main(void)
{
	pthread_t thread1;
	pthread_t thread2;

	pthread_create(&thread1, NULL, nhap_a, NULL);
	pthread_create(&thread2, NULL, print_hello, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
}
