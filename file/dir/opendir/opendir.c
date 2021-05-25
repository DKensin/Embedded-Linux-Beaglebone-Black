#include <stdio.h>
#include <dirent.h>

int main(void)
{
	DIR *folder;

	folder = opendir(".");
	if (folder == NULL) {
		puts("Unable to read directory.\n");
		return 1;
	}
	else {
		puts("Directory is opened.");
	}

	closedir(folder);

	return 0;
}
