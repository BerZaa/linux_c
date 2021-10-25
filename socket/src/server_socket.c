#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"

#define MAX_LENGTH 		4096
#define MAX_CLIENT		100
#define SOCKET_ERRNO 	-1

char message_buffer[MAX_LENGTH];

int main(int argc, char *argv[]) {
	int 				socket_des, 
						listen_des,
						connection_des, 
						bind_des,
						recv_message_length;
	struct sockaddr_in	server_adder;

	if(argc != 2) {
		printf("please input port parameter\n");
	}

	socket_des = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_des == SOCKET_ERRNO) {
		printf("create socket error\n");
		printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);
		exit(1);
	}

	memset(&server_adder, 0, sizeof(server_adder));
	server_adder.sin_family 		= AF_INET;
	server_adder.sin_port 			= atoi(argv[1]);
	server_adder.sin_addr.s_addr 	= htonl(INADDR_ANY);

	bind_des = bind(socket_des, (struct sockaddr*)&server_adder, sizeof(server_adder));
	if(bind_des == SOCKET_ERRNO) {
		printf("bind socket error\n");
		printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);
		exit(2);
	}	

	listen_des = listen(socket_des, MAX_CLIENT);
	if(listen_des == SOCKET_ERRNO) {
		printf("listen socket error\n");
		printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);
		exit(3);		
	}

	printf("------------------------------------------\n");
	printf("-      waiting for client's message      -\n");
	printf("------------------------------------------\n");

	while(1) {
		connection_des = accept(socket_des, (struct sockaddr*)NULL, NULL);
		if(connection_des == SOCKET_ERRNO) {
			printf("accept socket error\n");
			printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);
			continue;			
		}

		recv_message_length = recv(connection_des, message_buffer, MAX_LENGTH, 0);
		message_buffer[recv_message_length] = '\0';
		printf("client's message: %slength: %d\n", message_buffer, recv_message_length - 1);
		close(connection_des);
		
        if(strncmp(message_buffer, "end", 3) == 0) {
			break;
		}
	}

	close(socket_des);

	return 0;
}