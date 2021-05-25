#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int globvar = 6; /* external variable in initialized data */

char buf[] = "a write to stdout\n";

int main(void)
{
	int var; /* automatic variable on stack */
	pid_t pid;

	var = 88;

	if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
		puts("write error");
	printf("before fork\n"); /* we don't flush stdout */

	if ((pid = fork()) < 0)
		puts("fork error\n");
	else if (pid == 0) {  /* child */
		globvar++;
		var++;
	}
	else
		sleep(5); /* parent */
	printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);

	exit(0);
}
