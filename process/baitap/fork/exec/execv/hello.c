#include <stdio.h>
#include <unistd.h>

void main(void)
{
	printf("I am in hello.c\n");
	printf("PID of hello.c is %d\n", getpid());
}
