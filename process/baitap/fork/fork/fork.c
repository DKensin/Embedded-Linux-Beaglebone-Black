#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void main(void)
{	
	int n,a,b;
	pid_t pid;

	pid = fork();

	if (pid < 0) {
		printf("Error when fork\n");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) {
		/* child process */
		printf("\n**** Child process ****\n");
		printf("The process id is %d and parent id is %d\n", getpid(), 
								getppid());
		printf("Enter a number to check even or odd\n");
		scanf("%d", &n);
		if (n % 2 == 0)
			printf("Number %d is Even\n", n);
		else
			printf("Number %d is Odd\n", n);
		//sleep(20);
	}
	else {
		/*  parent process */
		wait();
		printf("\n\n*** Parent Process ****\n");
		printf("The process id is %d\n", getpid());
		printf("Enter 2 number to check maximum or minimum\n");
		scanf("%d%d", &a, &b);
		if (a > b) 
			printf("%d is greater than %d\n", a, b);
		else
			printf("%d is greater than %d\n", b, a);
		//sleep(30);
	}
}
