#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<string.h>

int main()
{
    pid_t pid = fork();
    if(pid == 0)
    {
        exit(0);
    }

    setsid();

    pid = fork();
    if(pid != 0)
    {
        exit(0);
    }

    chdir("/");

    umask(0);

    int maxfd = getdtablesize();//获得打开到描述符的最大值
    int i = 0;
    for(;i<maxfd;i++)
    {
        close(i);
    }

    while(1)
    {
        FILE*fp = fopen("/tmp/cytest.log","a");
        if(fp == NULL)
        {
            break;
        }

        time_t t;
        time(&t);//获取日期

        fprintf(fp,"Current time:%s",asctime(localtime(&t)));
        fclose(fp);

        sleep(5);
    }
    exit(0);
}
