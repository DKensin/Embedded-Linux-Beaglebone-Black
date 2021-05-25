#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#define MAGIC_NO	500
#define ON_OFF_CMD	_IOW(MAGIC_NO, 1, char*)

int main(int argc, char* argv[])
{
	char config = 0;
	int fd = -1;

	config = argv[1][0];
	fd = open("/dev/simple_misc", O_RDWR);
	if (fd < 0) {
		printf("Can't open file\n");
		return -1;
	}

	if (0 != ioctl(fd, ON_OFF_CMD, &config))
		printf("ioctl error\n");

	return 0;
}
