#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100

void *send_answer(void *arg);
void *recv_answer(void *arg);
void error_handling(char *msg);

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void *thread_return;

	if(argc!=3)
	{
		printf("Usgae : %s <IP> <port> <name>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");

	pthread_create(&snd_thread, NULL, send_answer, (void*)&sock);
	pthread_create(&rcv_thread, NULL, recv_answer, (void*)&sock);
	pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);
	close(sock);
	return 0;
}

void *send_answer(void *arg)
{
	int sock = *((int*)arg);
	int input;
	char inputStr[BUF_SIZE];
	
	while(1)
	{
		scanf("%d", &input);
		if(input == 0){
			close(sock);
			exit(0);
		}
		sprintf(inputStr, "%d", input);
		write(sock, inputStr, strlen(inputStr));
	}
	return NULL;
}

void *recv_answer(void *arg)
{
	int sock = *((int*)arg);
	int str_len;
	char buf[BUF_SIZE];

	while(1)
	{
		str_len = read(sock, buf, BUF_SIZE-1);
		if(str_len == -1)
			return (void*)-1;
		buf[str_len] = 0;
		fputs(buf, stdout);
	}
	return NULL;
}

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
