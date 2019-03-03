#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>
#include<pthread.h>

void *fun(void *arg)
{
    char arr[] = "1 2 3 4 5 6 7 8 9";
    char *ptr = NULL;
    char *p=strtok_r(arr," ",&ptr);
    while(p!=NULL)
    {
        printf("p=%s\n",p);
        sleep(1);
        p=strtok_r(NULL," ",&ptr);
    }

}

int main()
{
    pthread_t id;
    pthread_create(&id,NULL,fun,NULL);

    char str[]="a b c d e f g h";

    char *ptr = NULL;

    char*s=strtok_r(str," ",&ptr);
    while(s!=NULL)
    {
        printf("s=%s\n",s);
        sleep(1);
        s=strtok_r(NULL," ",&ptr);
    }

    exit(0);
}
