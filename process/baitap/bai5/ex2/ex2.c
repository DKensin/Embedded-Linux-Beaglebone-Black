#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

void main(void)
{
	int fd = -1;
	pid_t pid;
	char buf_parent[] = "Write from parent process\n";
	char buf_child[] = "Write from child process\n";	

	fd = open("data", O_WRONLY);
	if (fd < 0) {
		printf("Error while open file\n");
		exit(EXIT_FAILURE);
	}

	pid = fork();
	
	if (pid == 0) /* child process */
		write(fd, buf_child, strlen(buf_child));
	else	/* parent process */
		write(fd, buf_parent, strlen(buf_parent));

	close(fd);
}
