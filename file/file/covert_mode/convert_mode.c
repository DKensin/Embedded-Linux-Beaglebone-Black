#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void main(void)
{
	int modeNumber = 0;

	/* pointer to stat struct */
	struct stat sfile;

	/* stat system call */
	stat("data", &sfile);
	
	/* get mode of file */
	modeNumber = sfile.st_mode;

	printf("File Permission: \t");
	printf((S_ISDIR(modeNumber)) ? "d" : "-");
	printf((modeNumber & S_IRUSR) ? "r" : "-");
	printf((modeNumber & S_IWUSR) ? "w" : "-");
	
	printf((modeNumber & S_IXUSR) ? "x" : "-");
	printf((modeNumber & S_IRGRP) ? "r" : "-");
	printf((modeNumber & S_IWGRP) ? "w": "-");

	printf((modeNumber & S_IXGRP) ? "x" : "-");
	printf((modeNumber & S_IROTH) ? "r" : "-");
	printf((modeNumber & S_IWOTH) ? "w" : "-");
	printf((modeNumber & S_IXOTH) ? "x" : "-");
	printf("\n");

	printf("The file %s a symbolic link\n", (S_ISLNK(modeNumber)) ? "is" : "is not");
}
