#include <stdio.h>
#include <signal.h>

void main(void)
{
	kill(1045, SIGCONT);
}
