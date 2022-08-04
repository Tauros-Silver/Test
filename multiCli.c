#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

//Why is there Segmentation Fault core dumped?

char msg[500];

void *recvmg(void *my_sock)
{
	int sock = *((int *)my_sock);
	int len;
	//Client thread always ready to receive messages
	while((len = recv(sock, msg, 500, 0)) > 0) {
		msg[len] = ' ';
		fputs(msg, stdout);
	}
}

int main(int argc, char *argv[])
{
	pthread_t recvt;
	int sock = 0, len = 0;
	char send_msg[500];

	struct sockaddr_in ServerIp;	
		
	char client_name[100];
	strcpy(client_name, argv[1]);
	sock = socket(AF_INET, SOCK_STREAM, 0);

	ServerIp.sin_port = htons(1234);
	ServerIp.sin_family = AF_INET;
	ServerIp.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(connect(sock, (struct sockaddr *)&ServerIp, sizeof(ServerIp)) == -1)	{	
		printf("Connection to Socket FAILED!\n");
	}

	//Creatinf a client thread which is always waiting for a message
	pthread_create(&recvt, NULL, (void*)recvmg, &sock);

	while(fgets(msg, 500, stdin) > 0)
	{
		strcpy(send_msg, client_name);
		strcat(send_msg, ":");
		strcat(send_msg, msg);
		len = write(sock, send_msg, strlen(send_msg));
		if(len < 0)
			printf("\n Message not sent \n");
	}

	//Thread is closed
	pthread_join(recvt, NULL);
	close(sock);
	return 0;
}
