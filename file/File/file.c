#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void main(void)
{
	int fd = -1;
	char buf[128] = {0};
	int ret = 0;

	memset(buf, 0, 128);
	fd = open("data.txt", O_RDONLY);
	if (fd < 0) {
		printf("Can't open the file\n");
		return;
	}
	
	lseek(fd, strlen("Hello "), SEEK_SET);
	ret = read(fd, buf, sizeof(buf));
	if (ret > 0)
		printf("left data = %s", buf);

	close(fd);
}
