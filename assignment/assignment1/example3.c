#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

void error_handling(char* message);


int main(int argc, char *argv[])
{
	struct sockaddr_in addr_inet;
	if(argc !=2)
	{
		printf("Usage: %s <IP>\n", argv[0]);
		exit(1);
	}
	if(!inet_aton(argv[1], &addr_inet.sin_addr))
		error_handling("Conversion error");
	else
		printf("Network ordered integer addr: %#x\n", addr_inet.sin_addr.s_addr);
	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n', stderr);
	exit(1);
}
