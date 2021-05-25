#include <unistd.h>

void main(void)
{
	char *file_name = "ls";
	char *arg1 = "-a";
	char *arg2 = "-s";

	execlp(file_name, file_name, arg1, arg2, NULL);
}
