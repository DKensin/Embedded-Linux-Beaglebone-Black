#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main(void)
{
	char *user = NULL; /* use for store name of user */
	char *path_home = NULL; /* store path of home folder */
	char *pwd = NULL; /* path of current program */
	
	/* get path of home folder */
	path_home = getenv("HOME");
	if (path_home != NULL) {
		printf("Path of home folder = %s\n", path_home);
	}

	/* get value of USER environment variable */
	user = getenv("USER");	
	if (user != NULL) {
		printf("User = %s\n", user);
	}

	/* get pwd of current program */
	pwd = getenv("PWD");
	if (pwd != 0) {
		printf("PWD = %s\n", pwd); 
	}
}
