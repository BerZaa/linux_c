/*
 * @Author: BerZaa 
 * @Date: 2021-10-24 21:32:28 
 * @Last Modified by: BerZaa
 * @Last Modified time: 2021-10-26 18:18:59
 */

#include "stdio.h"                                                                          //头文件引入
#include "stdlib.h"
#include "errno.h"
#include "string.h"
#include "unistd.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "sys/sem.h"


#define SEMAPHORE_ERRNO -1                                                                  //系统函数一般返回错误代码为1，使用宏定义增强代码可读性
#define PER_VALUE       1                                                                   //使用信号量读写共享的资源时，采用二值信号量，宏定义可以使用资源时信号量值为1                                                             
#define UNPER_VALUE     0                                                                   //定义不可使用资源时信号量值为0
#define MAX_BUFFER      1024                                                                //定义共享资源的大小
#define EXCEPTNO        10                                                                  //自定义函数的错误返回值

/*------------------------------from linux/sem.h------------------------------*/            //在使用共用体初始化信号量值的时候，sys/sem.h已经注释掉了共用体的定义，如果直接包含linux/sem.h又会出现其他数据类型的重复定义问题，所以在linux/sem.h中提取该共用体的原始定义来初始化信号量值
union semun {
	int val;			    /* value for SETVAL */
	struct semid_ds *buf;	/* buffer for IPC_STAT & IPC_SET */
	unsigned short *array;	/* array for GETALL & SETALL */
	struct seminfo *__buf;	/* buffer for IPC_INFO */
	void *__pad;
};
/*------------------------------from linux/sem.h------------------------------*/

typedef struct sembuf sem_optr;                                                             //对信号量编程时需要使用的结构体数据进行重命名
typedef union  semun  sem_init;

char           shraed_data[MAX_BUFFER];                                                     //定义一个数组作为两个进程的共享资源，使用信号量来对两个进程进行调度，对该块内存进行读写

int            sem_operate_p(int sem_id);                                                   //定义函数对信号量进行P操作，即-1操作
int            sem_operate_v(int sem_id);                                                   //定义函数对信号量进行v操作，即+1操作
int            sem_getvalue (int sem_id);                                                   //获取当前信号量的值

sem_optr       p_operation = {0, -1, SEM_UNDO};                                             //使用信号量控制函数进行操作时需要用用到的结构体参量
sem_optr       v_operation = {0, +1, SEM_UNDO};

int main(char argc, char *argv[]) {
    int      status,                                                                        //在使用一些系统API函数时，该变量用来保存函数的返回值，通过对返回值的判断来确定API函数的执行情况
             sem_id;                                                                        //信号量的id
    key_t    sem_key;                                                                       //在创建信号量的时候所给予函数参数列表的键
    sem_init sem_union;                                                                     //用来初始化信号量的共同体变量
    pid_t    pid;                                                                           //使用fork函数分裂进程时，fork函数的返回值，以此来判断当前执行的进程是子进程还是父进程

    sprintf(shraed_data, "init value");                                                     //给共享资源一个初始值，为字符串

    if(argc != 2) {                                                                         //程序通过主函数的参数，即命令行的参数来提供信号量的键值，所以如果参数不为2，证明只执行但是没有输入参数
        printf("please input sem's key\n");

        exit(1);
    }

    sem_key = atoi(argv[1]);                                                                //如果输入了参数，将输入的参数转换为整数保存在键值变量中
    sem_id = semget(sem_key, 1, 0666 | IPC_CREAT);                                          //使用系统API函数创建一个信号量
    if(status == SEMAPHORE_ERRNO) {                                                         //如果创建失败则打印提示信息并退出程序
        printf("create semaphore error\n");
        printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);

        exit(2); 
    }

    sem_union.val = PER_VALUE;
    status = semctl(sem_id, 0, SETVAL, sem_union);                                          //使用semctl函数初始化信号量的值
    if(status == SEMAPHORE_ERRNO) {
        printf("init semaphore error\n");
        printf("ERROR MESSAGE: %s\nERROR CODE: %d\n", strerror(errno), errno);

        exit(4);         
    }

    printf("shared buffer init value: %s\n", shraed_data);                                  //将共享资源的初始化值打印出来

    pid = fork();                                                                           //使用fork函数分裂进程，并将返回值给予pid
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