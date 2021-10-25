/*
 * @Author: BerZaa 
 * @Date: 2021-10-24 21:32:02 
 * @Last Modified by:   BerZaa 
 * @Last Modified time: 2021-10-24 21:32:02 
 */
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/msg.h"
#include "errno.h"

#define STRING          1
#define BUFFSIZE        BUFSIZ
#define RUN             1
#define STOP            0
#define RW_PER          0666
#define MESSAGE_ERRNO   -1

typedef struct MESSAGE_CONTEXT {
    long int message_type;
    char     content[BUFFSIZE];
}message;

int main(int argc, char *argv[]) {
    int     run_status = RUN;
    key_t   message_id;
    message message_inst;
    long int recv_type = 0;
    
    message_id = msgget((key_t)atoi(argv[1]), IPC_CREAT | RW_PER);
    if(message_id == MESSAGE_ERRNO) {
        printf("create message queue error\n");
        printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);

        exit(1); 
    }

    while(run_status) {
        
        if((msgrcv(message_id, (void *)&message_inst, BUFFSIZE, recv_type, 0)) == MESSAGE_ERRNO) {
            printf("recv message error\n");
            printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);

            if(msgctl(message_id, IPC_RMID,0) == MESSAGE_ERRNO) {
                printf("delete message queue error\n");
                printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);
                exit(2);
            }
            
            exit(3);    
        }

        printf("recv message: %s", message_inst.content);
        printf("recv message type: %ld\n", message_inst.message_type);

        if(strncmp(message_inst.content, "end", 3) == 0) {
            if(msgctl(message_id, IPC_RMID,0) == MESSAGE_ERRNO) {
                printf("delete message queue error\n");
                printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);
                exit(2);
            }
            run_status = STOP;
        }
    }

    return 0;
}