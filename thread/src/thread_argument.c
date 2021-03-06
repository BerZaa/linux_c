/*
 * @Author: BerZaa 
 * @Date: 2021-10-24 21:32:49 
 * @Last Modified by: BerZaa
 * @Last Modified time: 2021-10-30 21:47:43
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *son_thread(void *argument) {
    printf("-       son thread index: %02d     -\n", *((char *)argument));
    pthread_exit(NULL);
}

void *create_thread(void *argument) {
    pthread_t thread_id;
    for(char index = 0; index < 100; ++index) {
        printf("------------son thread------------\n");
        pthread_create(&thread_id, NULL, son_thread, (void *)&index);
        pthread_join(thread_id, NULL);
        printf("----------------------------------\n");
        usleep(5000);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t create_thread_id;
    pthread_create(&create_thread_id, NULL, create_thread, NULL);
    pthread_join(create_thread_id, NULL);
    printf("------------main thread------------\n");
    printf("-            test over            -\n");
    printf("-----------------------------------\n");
    return 0;
}