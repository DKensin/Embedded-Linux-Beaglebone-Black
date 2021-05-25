#include <stdio.h>
#include <stdlib.h>

extern char **environ;

void main(void)
{
	char **s = environ;
	const char *name = "OLDPWD";
	char *value;

	for (; *s; s++)
	{
		printf("%s\n", *s);
	}

	value = getenv(name);
	printf("OLDPWD: %s\n", value);


}
