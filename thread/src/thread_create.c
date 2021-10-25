/*
 * @Author: BerZaa 
 * @Date: 2021-10-24 21:32:47 
 * @Last Modified by:   BerZaa 
 * @Last Modified time: 2021-10-24 21:32:47 
 */
#include "pthread.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"

void *thread_print(void *argument) {
    while(1) {
        printf("thread print string: %s\n", (char *)argument);
        sleep(1);
    }
}

void *create_thread(void *argument) {
    pthread_t thread_id;

    pthread_create(&thread_id, NULL, thread_print, "----son thread running----");
    printf("son thread create successfully\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t create_thread_id;

    pthread_create(&create_thread_id, NULL, create_thread, NULL);
    pthread_join(create_thread_id, NULL);
    while(1) {
        printf("----main thread running----\n");
        sleep(2);
    }

    return 0;
}