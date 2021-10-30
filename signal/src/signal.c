/*
 * @Author: BerZaa 
 * @Date: 2021-10-26 21:15:08 
 * @Last Modified by: BerZaa
 * @Last Modified time: 2021-10-30 21:36:55
 */
#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"

void signal_handler(int sig_num) {
    if(sig_num == SIGCHLD) {
        printf("action : ctrl + z\n");
    }
    else if(sig_num == SIGINT) {
        printf("action : ctrl + c\n");
    }
    else if(sig_num == SIGQUIT) {
        printf("action : ctrl + \\n");
    }
    else if(sig_num == SIGUSR2) {
        exit(0);
    }
    else {
        printf("other action\n");
    }
}

int main(int argc, char *argv[]) {
    pid_t current_pid;
    current_pid = getpid();
    int count = 0;
    for(int index = 0; index < 32; ++index) {
        signal(index, signal_handler);
    }
    while(1) {
        printf("waiting for signal...\n");
        sleep(1);
        count++;
        if(count == 10) {
            kill(current_pid, SIGUSR2);
        }
    }
    return 0;
}