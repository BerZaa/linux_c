/*
 * @Author: BerZaa 
 * @Date: 2021-10-24 21:32:28 
 * @Last Modified by:   BerZaa 
 * @Last Modified time: 2021-10-24 21:32:28 
 */
#include "stdio.h"
#include "stdlib.h"
#include "errno.h"
#include "string.h"
#include "unistd.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "sys/sem.h"

#define SEMAPHORE_ERRNO -1
#define PER_VALUE       1
#define UNPER_VALUE     0
#define MAX_BUFFER      1024
#define EXCEPTNO        10

/*------------------------------from linux/sem.h------------------------------*/
union semun {
	int val;			    /* value for SETVAL */
	struct semid_ds *buf;	/* buffer for IPC_STAT & IPC_SET */
	unsigned short *array;	/* array for GETALL & SETALL */
	struct seminfo *__buf;	/* buffer for IPC_INFO */
	void *__pad;
};
/*------------------------------from linux/sem.h------------------------------*/

typedef struct sembuf sem_optr;
typedef union  semun  sem_init;

char           shraed_data[MAX_BUFFER];

int sem_operate_p(int sem_id);
int sem_operate_v(int sem_id);
int sem_getvalue (int sem_id);

sem_optr p_operation = {0, -1, SEM_UNDO};
sem_optr v_operation = {0, +1, SEM_UNDO};

int main(char argc, char *argv[]) {
    int      status, 
             sem_id;
    key_t    sem_key;
    sem_init sem_union;
    pid_t    pid;

    sprintf(shraed_data, "init value");

    if(argc != 2) {
        printf("please input sem's key\n");

        exit(1);
    }

    sem_key = atoi(argv[1]);
    sem_id = semget(sem_key, 1, 0666 | IPC_CREAT);
    if(status == SEMAPHORE_ERRNO) {
        printf("create semaphore error\n");
        printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);

        exit(2); 
    }

    sem_union.val = PER_VALUE;
    status = semctl(sem_id, 0, SETVAL, sem_union);
    if(status == SEMAPHORE_ERRNO) {
        printf("init semaphore error\n");
        printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);

        exit(4);         
    }

    printf("shared buffer init value: %s\n", shraed_data);

    pid = fork();
    if(pid != 0) {
        if(pid < 0) {
            printf("create process error\n");
            printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);

            exit(5);               
        }
        else {
            printf("father process running\n");
            sleep(1);
            
            while(1) {
                status = sem_getvalue(sem_id);
                if(status == EXCEPTNO) exit(6);

                if(status == PER_VALUE) {
                    status = sem_operate_p(sem_id);
                    if(status == EXCEPTNO) exit(7);

                    sprintf(shraed_data, "parent process has changed data");
                    printf("current data: %s\n", shraed_data);

                    status = sem_operate_v(sem_id);  
                    if(status == EXCEPTNO) exit(7);   
                }
                else {
                    printf("wait son's v operater\n");
                }

                sleep(rand() % 10);
            }

            wait(NULL);
        }
    }
    else {
        printf("son process running\n");
        sleep(1);

        while(1) {
            status = sem_getvalue(sem_id);
            if(status == EXCEPTNO) exit(6);

            if(status == PER_VALUE) {
                status = sem_operate_p(sem_id);
                if(status == EXCEPTNO) exit(7);

                sprintf(shraed_data, "son process has changed data");
                printf("current data: %s\n", shraed_data);

                status = sem_operate_v(sem_id);  
                if(status == EXCEPTNO) exit(7);   
            }
            else {
                printf("wait father's v operater\n");
            }

            sleep(rand() % 10);            
        }
    }

    return 0;
}

int sem_operate_p(int sem_id) {
    int status;

    status = semop(sem_id, &p_operation, 1);
    if(status == SEMAPHORE_ERRNO) {
        printf("operate p error\n");
        printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);    

        return EXCEPTNO;
    }

    return 0;
}

int sem_operate_v(int sem_id) {
    int status;

    status = semop(sem_id, &v_operation, 1);
    if(status == SEMAPHORE_ERRNO) {
        printf("operate v error\n");
        printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);    

        return EXCEPTNO;
    }

    return 0;
}

int sem_getvalue(int sem_id) {
    int status_value;

    status_value = semctl(sem_id, 0, GETVAL);
    if(status_value == SEMAPHORE_ERRNO) {
        printf("get semaphore error\n");
        printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);    

        return EXCEPTNO;
    }

    return status_value;
}