#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<sys/msg.h>
#include<unistd.h>

struct mess
{
    long int type;
    char buff[32];
};

int main()
{
    int msgid = msgget((key_t)1234,IPC_CREAT|0600);
    assert(msgid != -1);

    struct mess dt;

    msgrcv(msgid,(void*)&dt,32,0,0);

    printf("buff=%s\n",dt.buff);

    exit(0);

}
