#include <stdio.h>
#include <unistd.h>
#include <string.h>

void main(void)
{
	pid_t pid;

	pid = fork();

	if (0 == pid) {
		while (1) {
			strcpy("a", NULL);
			printf("I am parent\n");
		}
	}

	else {
		printf("I am child\n");
	}
}
