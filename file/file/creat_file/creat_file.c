#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void main(void)
{
	int fd = -1;

	fd = creat("data", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}
