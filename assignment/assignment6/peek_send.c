#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in send_addr;
	
	if(argc!=3)
	{
		printf("usage: %s<IP><port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&send_addr, 0, sizeof(send_addr));
	send_addr.sin_family=AF_INET;
	send_addr.sin_addr.s_addr=inet_addr(argv[1]);
	send_addr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&send_addr, sizeof(send_addr))==-1)
		error_handling("connect() error!");
	
	write(sock, "123", strlen("123"));
	close(sock);
	
	return 0;
}