#include <stdio.h>
#include <signal.h>

sigset_t newset, oldset;

void my_handler(int signo)
{
	
	sigemptyset(&newset);
	sigemptyset(&oldset);

	sigaddset(&newset, SIGINT);

	sigprocmask(SIG_UNBLOCK, &newset, &oldset);
}

void main()
{
	signal(SIGUSR2, my_handler);

	sigemptyset(&newset);
	sigemptyset(&oldset);

	sigaddset(&newset, SIGINT);

	sigprocmask(SIG_BLOCK, &newset, &oldset);

	while (1);
}
