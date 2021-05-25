#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

void main(void)
{
	DIR *dir_ptr = NULL;
	struct dirent *dp = NULL;

	dir_ptr = opendir("folder");
	if (NULL ==dir_ptr) {
		printf("can't open directory\n");
		return;
	}

	while ((dp = readdir(dir_ptr)) != NULL) {
		printf("name = %s\n", dp->d_name);
	}

	closedir(dir_ptr);
}
