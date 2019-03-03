#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<pthread.h>
#include<unistd.h>

void* thread_fun(void* arg)
{
    int index = *(int*)arg;
    int i = 0;
    for( ;i<4;i++)
    {
        printf("index=%d\n",index);
        sleep(1);
    }
}

int main()
{
    pthread_t id[5];
    
    int i = 0;
    for( ;i<5;i++)
    {
        pthread_create(&id[i],NULL,thread_fun,(void*)&i);
    }

    for(i=0;i<5;i++)
    {
        pthread_join(id[i],NULL);
    }

    exit(0);
}
