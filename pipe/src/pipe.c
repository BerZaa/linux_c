/*
 * @Author: BerZaa 
 * @Date: 2021-10-24 21:32:23 
 * @Last Modified by:   BerZaa 
 * @Last Modified time: 2021-10-24 21:32:23 
 */
#include "unistd.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "wait.h"
#include "errno.h"

#define PIPE_ERRNO -1
#define MAX_BUFFER 1024

int main(int argc, char *argv[]) {
    int     pipe_inst[2],
            status,
            length;
    pid_t   pid;
    char    buffer[MAX_BUFFER],
            pipe_data[MAX_BUFFER];

    status = pipe(pipe_inst);
    if(status == PIPE_ERRNO) {
        printf("create pipe error\n");
        printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);

        exit(1);
    }

    pid = fork();
    if(pid != 0) {
        if(pid < 0) {
            printf("create process error\n");
            printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);

            exit(2);                
        }
        else {
            printf("input data you want to send: ");
            fgets(pipe_data, MAX_BUFFER, stdin);

            status = write(pipe_inst[1], pipe_data, strlen(pipe_data));
            if(status == PIPE_ERRNO) {
            printf("send data error\n");
            printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);

            wait(NULL);
            close(pipe_inst[1]);

            exit(3);               
        }
        }
    }
    else {
        length = read(pipe_inst[0], buffer, MAX_BUFFER);
        if(length == PIPE_ERRNO) {
            printf("recv data error\n");
            printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);

            exit(4);               
        }
        buffer[length - 1] = '\0';
        printf("son process recv data: %s\n", buffer);

        close(pipe_inst[0]);
    }
}