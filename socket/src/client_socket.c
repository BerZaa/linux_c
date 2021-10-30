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
#define FILE_TYPE       1
#define MESSAGE_TYPE    2
#define TYPE            1
#define DATA            2

char message_buffer[MAX_LENGTH];

int main(int argc, char *argv[]) {
    int         socket_des, 
                connection_des,
                pton_des,
                send_des,
                data_type,
                send_state = TYPE;
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
        if(send_state == TYPE) {
            send_state = DATA;
            printf("which type data you want to send(1:file 2:message): ");
            scanf("%d", &data_type);
            if(data_type == MESSAGE_TYPE) {
                send_des = send(socket_des, "msg", strlen("msg"), 0);
                if(send_des == SOCKET_ERRNO) {
                    printf("send type error\n");
                    printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);
                    exit(5);        
                }  

            }
            else {
                send_des = send(socket_des, "fil", strlen("fil"), 0);
                if(send_des == SOCKET_ERRNO) {
                    printf("send type error\n");
                    printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);
                    exit(5);        
                } 
            }
        } 
        else {
            send_state = TYPE;
            if(data_type == MESSAGE_TYPE) {
                printf("input the message send to server: ");
                getchar();
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
            }
            else {
                char    file_name[100], 
                        file_content[1000];
                FILE    *file_read;
                memset(file_content, 0, sizeof(file_content));
                printf("input the file name: ");
                getchar();
                fgets(file_name, 100, stdin);
                file_name[strlen(file_name) - 1] = '\0';
                file_read = fopen(file_name, "r");
                if(file_read == NULL) {
                    printf("open file error\n");
                    printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);  
                    exit(6);                  
                }
                while(!feof(file_read)) {
                    char file_char = (char)fgetc(file_read);
                    strncat(file_content, &file_char, 1);
                }
                file_content[strlen(file_content) - 1] = '\0';
                printf("file content: \n%s\n", file_content);
                printf("content length: %d\n", strlen(file_content));
                fclose(file_read);                
                send_des = send(socket_des, file_content, strlen(file_content), 0);
                if(send_des == SOCKET_ERRNO) {
                    printf("send message error\n");
                    printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);
                    exit(5);        
                }     
                memset(file_content, 0, sizeof(file_content));            
            }
        }
        close(socket_des);
    }
    return 0;
}
