#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int flag = 0;
int *pf = &flag;

void my_handler(int signo)
{
	if (signo == SIGCONT) 
		*pf = 1;
}

void main(void)
{
	signal(SIGCONT, my_handler);
	while (1) {
		pause();
		if (flag == 1)
		{
			break;
		}
	}

	printf("Hello World.\n");
}
