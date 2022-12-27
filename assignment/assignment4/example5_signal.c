#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void timeout(int sig)
{
	if(sig ==SIGALRM)
		puts("Time out!");
	alarm(2);
}

void keycontrol(int sig)
{
	if(sig==SIGINT)
		puts("CTRL+C pressed");
}

int main(int arg, char* argv[])
{
	int i;
	signal(SIGALRM, timeout);
	signal(SIGINT, keycontrol);
	alarm(2);
	
	for(int i=0;i<3;i++)
	{
		puts("wait..");
		sleep(100);
	}
	
	return 0;
}