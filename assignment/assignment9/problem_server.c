#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutex;

void error_handling(char *msg);
void *handle_clnt(void *arg);
int checkAnswer(int answer, int input);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;

	srand(time(NULL));

	if(argc!=2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	pthread_mutex_init(&mutex, NULL);

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	
	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	while(1)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

		pthread_mutex_lock(&mutex);
		clnt_socks[clnt_cnt++] = clnt_sock;
		pthread_mutex_unlock(&mutex);

		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		pthread_detach(t_id);
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void *handle_clnt(void *arg)
{
	int clnt_sock = *((int*)arg);
	int str_len = 0, i;
	int input;
	int answer = rand()%100+1;
	char buf[BUF_SIZE] = {0,};
	int cnt = 0;
	
	while((str_len = read(clnt_sock, buf, sizeof(buf))) != 0){
		input = atoi(buf);
		if(checkAnswer(answer, input))
		{
			sprintf(buf, "Correct!!! Answer is %d count : %d\n\n", answer, cnt);
			write(clnt_sock, buf, strlen(buf));
			memset(buf, 0, sizeof(buf));
		}
		else{
			if(answer > input)
			{
				sprintf(buf, "Wrong... Answer is bigger than %d\n", input);
				write(clnt_sock, buf, strlen(buf));
				memset(buf, 0, sizeof(buf));
			}
			else
			{
				sprintf(buf, "Wrong... Answer is smaller than %d\n", input);
				write(clnt_sock, buf, strlen(buf));
				memset(buf, 0, sizeof(buf));
			}
			cnt++;
		}
	}
	
	pthread_mutex_lock(&mutex);
	for(i=0;i<clnt_cnt;i++)
	{
		if(clnt_sock == clnt_socks[i])
		{
			while(i++<clnt_cnt-1)
				clnt_socks[i] = clnt_socks[i+1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutex);
	close(clnt_sock);
	return NULL;
}

int checkAnswer(int answer, int input)
{
	if(answer == input)
		return 1;
	else
		return 0;
}
