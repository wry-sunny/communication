#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<unistd.h>
#include<assert.h>
#include"sem.h"

int main()
{
    sem_init();
    int shmid = shmget((key_t)1234,256,IPC_CREAT|0600);
    assert(shmid != -1);

    char *s = (char*)shmat(shmid,NULL,0);

    while(1)
    {
        char buff[128] = {0};
        printf("input\n");
        fgets(buff,128,stdin);
        strcpy(s,buff);
        if(strncmp(buff,"end",3)==0)
        {
            break;
        }
        sem_v();
    }
    shmdt(s);
    exit(0);
}
