#include <unistd.h>

void main(void)
{
	char *bin_path = "/bin/ls";
	char *arg1 = "-a";
	char *arg2 = "-s";
	
	execl(bin_path, bin_path, arg1, arg2, NULL);
}
