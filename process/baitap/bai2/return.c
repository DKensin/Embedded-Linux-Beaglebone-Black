#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void)
{
	int fd = -1;

	fd = open("data", O_RDONLY);

	if (fd < 0) {
		return -1;
		exit(0);
	}
	else
		return 0;
}
