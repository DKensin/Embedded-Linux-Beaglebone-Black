#include <stdio.h>
#include <signal.h>

void main()
{
	kill(7380, SIGUSR2);
}
