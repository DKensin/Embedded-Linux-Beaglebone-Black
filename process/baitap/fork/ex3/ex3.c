#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 

int main(void)
{
	if (fork() == 0) 
		printf("Hello from Child\n");
	else
		printf("Hello from parent\n");

	return 0;
}
