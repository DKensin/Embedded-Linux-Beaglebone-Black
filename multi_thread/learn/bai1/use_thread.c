#include <stdio.h>
#include <pthread.h>

void *s1(void *arg)
{
	int i = 0;
	int s1 = 0;

	for (i = 1; i <= 250000000; i+= 2)
		s1 += i;
}

void *s2(void *arg)
{
	int i = 0;
	int s2 = 0;

	for (i = 250000001; i <= 500000000; i+= 2)
		s2 += i;

}

void *s3(void *arg)
{
	int i = 0;
	int s3 = 0;

	for (i = 500000001; i <= 750000000; i+=2)
		s3 += i;

}

void *s4(void *arg)
{
	int i = 0;
	int s4 = 0;
	for (i = 750000001; i <= 100000000000; i+=2)
	       s4 += i;	
}

void main(void)
{
	pthread_t thread0;
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;

	pthread_create(&thread0, NULL, s1, NULL);
	pthread_create(&thread1, NULL, s2, NULL);
	pthread_create(&thread2, NULL, s3, NULL);
	pthread_create(&thread3, NULL, s4, NULL);

	pthread_join(thread0, NULL);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
}
