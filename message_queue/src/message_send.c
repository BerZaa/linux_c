/*
 * @Author: BerZaa 
 * @Date: 2021-10-24 21:32:14 
 * @Last Modified by:   BerZaa 
 * @Last Modified time: 2021-10-24 21:32:14 
 */
#include "stdio.h"                                                                          //头文件引入
#include "string.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/msg.h"
#include "errno.h"

#define STRING          1                                                                   //宏定义数据类型
#define BUFFSIZE        BUFSIZ                                                              //重新命令系统定义的BUFSIZ
#define RUN             1                                                                   //进程状态，1为执行中
#define STOP            0                                                                   //进程状态，0为执结束
#define RW_PER          0666                                                                //创建消息队列时给予的权限，0666为可读可写
#define MESSAGE_ERRNO   -1                                                                  //消息队列的系统函数如果发生执行错误一般返回-1，所以对错误代码进行宏定义

typedef struct MESSAGE_CONTEXT {                                                            //定义消息的结构体，包含数据类型，数据内容
    long int message_type;
    char     content[BUFFSIZE];
}message;

int main(int argc, char *argv[]) {
    int     run_status = RUN;                                                               //初始化进程状态为运行状态
    key_t   message_id;                                                                     //消息队列的id
    message message_inst;                                                                   //要发送的消息buffer

    message_id = msgget((key_t)atoi(argv[1]), IPC_CREAT | RW_PER);                          //调用系统API函数，使用命令行传入的参数进行消息队列的创建
    if(message_id == MESSAGE_ERRNO) {                                                       //在包含了errno.h以后，系统函数会对一个全局变量进行修改，再通过strerror函数可以输出错误信息，以下所有报错均以这种形式输出
        printf("create message queue error\n");
        printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);

        exit(1); 
    }

    while(run_status) {                                                                     //根据运行状态变量判断是否执行
        printf("please input message you want to input: ");                                 //输出提示语，提示用户输入要发送的消息
        fgets(message_inst.content, BUFFSIZE, stdin);                                       //从标准输入缓冲区将内容存入消息的内容
        message_inst.message_type = STRING;                                                 //定义数据类型

        if(msgsnd(message_id, (void *)&message_inst, BUFFSIZE, 0) == MESSAGE_ERRNO) {       //调用系统API函数，msgsnd通过message_id发送出去
            printf("send message error\n");
            printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);

            exit(2);             
        }

        if(strncmp(message_inst.content, "end", 3) == 0) {                                  //如果用户输入end，则修改运行状态为stop，并结束程序的运行
            run_status = STOP;
        }
    }

    return 0;
}