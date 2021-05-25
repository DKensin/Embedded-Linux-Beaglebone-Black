#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void main()
{
	if (fork() == 0) {
		printf("Hello World\n");
	}
	else {
		execl("/bin/ls", "/bin/ls", "-l", (char *) NULL);
	}
}
