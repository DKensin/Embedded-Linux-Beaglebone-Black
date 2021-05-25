#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

void main(void)
{
	int count = 0;
	char *val = NULL;

	while (environ[count] != NULL) {
		printf("[%s]\n", environ[count]);
		count++;
	}

	val = getenv("OLDPWD");	

	if (val != NULL)
	{
		printf("%s\n", val);
	}
}
