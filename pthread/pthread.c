#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t mutex;
//sem_t sem;

void *fun(void *arg)
{
    int i=0;
    for( ;i<5;i++)
    {
       // sem_wait(&sem);
        pthread_mutex_lock(&mutex);
        printf("A");
        fflush(stdout);
        int n = rand() % 3;
        sleep(n);
        printf("A");
        fflush(stdout);
        pthread_mutex_unlock(&mutex);
        //sem_post(&sem);
        n=rand() % 3;
        sleep(n);
    }
}

int main()
{
    pthread_t id;
    //sem_init(&sem,0,1);
    pthread_mutex_init(&mutex,NULL);
    pthread_create(&id,NULL,fun,NULL);


    int i=0;
    for( ;i<5;i++)
    {
        pthread_mutex_lock(&mutex);
        //sem_wait(&sem);
        printf("B");
        fflush(stdout);
        int n = rand() % 3;
        sleep(n);
        printf("B");
        fflush(stdout);
        pthread_mutex_unlock(&mutex);
        //sem_post(&sem);
        n=rand() % 3;
        sleep(n);
    }

    pthread_join(id,NULL);
    //sem_destroy(&sem);
    pthread_mutex_destroy(&mutex);
    exit(0);
}
