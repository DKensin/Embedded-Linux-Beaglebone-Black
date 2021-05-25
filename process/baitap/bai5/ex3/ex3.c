#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void main(void)
{
	int var = 1;
	pid_t pid;
	
	printf("Before fork, variable value = %d\n", var);
	pid = fork();
	
	printf("After fork: \n");
	if (pid == 0) /* child process */
		printf("Result of variable in child process: %d\n", ++var);
	else	/* parent process */
		printf("Result of variable in parent process: %d\n", --var);
}
