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
    dt.type = 1;
    strcpy(dt.buff,"hello");

    msgsnd(msgid,(void*)&dt,32,1);

    exit(0);

}
