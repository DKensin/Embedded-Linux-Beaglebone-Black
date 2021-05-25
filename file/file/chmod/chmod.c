#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void main(void)
{
	int fd = -1;

	/* creat new file name data with permission: -rw-r--r-- */
	fd = creat("data", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	/* want change permission of data is -rwxr--r--
	 * mean user (owner) can execute */
	chmod("data", S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH);
}
