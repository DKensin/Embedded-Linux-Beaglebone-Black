#include <stdio.h>
#include <unistd.h>

void main()
{
	printf("I am in execv.c\n");
	printf("PID of execv.c is %d\n", getpid());

	char *args[] = {"./hello", NULL};

	execv(args[0], args);
	printf("Coming back to execv.c\n"); /* This will not be executed */
}
