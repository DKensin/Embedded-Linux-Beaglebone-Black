#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void main(void)
{
	/* pointer to stat struct */
	struct stat sfile;

	/* stat system call */
	stat("data", &sfile);

	/* access st_mode (data member  of stat struct ) */
	printf("st_mode = %d\n", sfile.st_mode);
}
