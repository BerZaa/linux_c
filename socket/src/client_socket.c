/*
 * @Author: BerZaa 
 * @Date: 2021-10-25 23:01:32 
 * @Last Modified by:   BerZaa 
 * @Last Modified time: 2021-10-25 23:01:32 
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"

#define MAX_LENGTH 		4096
#define MAX_CLIENT		100
#define SOCKET_ERRNO 	-1

char message_buffer[MAX_LENGTH];

int main(int argc, char *argv[]) {
    int         socket_des, 
                connection_des,
                pton_des,
                send_des;
    char        send_message[MAX_LENGTH];
    struct sockaddr_in	server_adder;

    if(argc != 3) {
        printf("please input ip address and port\n");
        exit(1);
    }

    while(1) {
        socket_des = socket(AF_INET, SOCK_STREAM, 0);
        if(socket_des == SOCKET_ERRNO) {
            printf("create socket error\n");
            printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);
            exit(2); 
        }

        memset(&server_adder, 0, sizeof(server_adder));
        server_adder.sin_family   = AF_INET;
        server_adder.sin_port     = atoi(argv[2]);

        pton_des = inet_pton(AF_INET, argv[1], &server_adder.sin_addr);
        if(pton_des < 0) {
            printf("convert ip address error\n");
            printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);
            exit(3);        
        }

        connection_des = connect(socket_des, (struct sockaddr*)&server_adder, sizeof(server_adder));
        if(connection_des == SOCKET_ERRNO) {
            printf("connect socket error\n");
            printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);
            exit(4);         
        }

        printf("input the message send to server: ");
        fgets(send_message, MAX_LENGTH, stdin);
        send_des = send(socket_des, send_message, strlen(send_message), 0);
        if(send_des == SOCKET_ERRNO) {
            printf("send message error\n");
            printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);
            exit(5);        
        }   

        if(strncmp(send_message, "end", 3) == 0) {
			break;
		}

        close(socket_des);
    }

    return 0;
}