#include <stdio.h>
#include <signal.h>

void my_handler(int signo)
{
	if (signo == SIGINT) {
		printf("Hello World\n");
	}
}

void main()
{
	signal(SIGINT, my_handler);
	while (1);
}
