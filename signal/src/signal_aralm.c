/*
 * @Author: BerZaa 
 * @Date: 2021-10-27 20:35:27 
 * @Last Modified by: BerZaa
 * @Last Modified time: 2021-10-30 21:44:36
 */
#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"

void alarm_handler() {
    exit(0);
}

int main(int argc, char *argv[]) {
    int count = 0;
    alarm(10);
    signal(SIGALRM, alarm_handler);
    while(1) {
        printf("after alarm %d seconds\n", ++count);
        sleep(1);
    }
}