/*
 * @Author: BerZaa 
 * @Date: 2021-10-24 21:32:47 
 * @Last Modified by:   BerZaa 
 * @Last Modified time: 2021-10-24 21:32:47 
 */
#include "pthread.h"                                                                        //头文件引入
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"

void *thread_print(void *argument) {                                                        //输出信息子线程
    while(1) {                                                                              //该线程中循环打印传入的参数
        printf("thread print string: %s\n", (char *)argument);
        sleep(1);
    }
}

void *create_thread(void *argument) {                                                       //在主线程中使用该线程来创建thread_print线程
    pthread_t thread_id;

    pthread_create(&thread_id, NULL, thread_print, "-----son thread running-----");         //使用pthread_create函数创建线程，传入参数
    printf("son thread create successfully\n");                                             //输出提示信息线程创建成功
    pthread_exit(NULL);                                                                     //创建完成以后直接退出该线程
}

int main(int argc, char *argv[]) {
    pthread_t create_thread_id;                                                             //线程指针

    pthread_create(&create_thread_id, NULL, create_thread, NULL);                           //使用线程创建函数来创建创建线程的线程
    pthread_join(create_thread_id, NULL);                                                   //等待该线程退出
    while(1) {                                                                              //在主线程中循环输出制定的内容
        printf("-----main thread running-----\n");
        sleep(2);
    }

    return 0;
}