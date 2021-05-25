#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void main(void)
{
	int pid;

	pid = fork();

	if (pid > 0){
		printf("\nI am parent\n");
		printf("pid = %d\n", getpid());
	}
	if (pid == 0) {
		printf("\nI am child\n");
		printf("pid = %d\n", getpid());
	}
}
