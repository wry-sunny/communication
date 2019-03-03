#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>

void sig_Fun(int sigNO)
{
    pid_t pid = waitpid(0,NULL,WNOHANG);
    if(pid < 0)
    {
        return;
    }
}

int main()
{

    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    assert(sockfd != -1);

    struct sockaddr_in saddr,caddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6000);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int res = bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    assert(res != -1);

    listen(sockfd,5);

    signal(SIGCHLD, sig_Fun);
    while(1)
    {
        int len = sizeof(caddr);
        int c = accept(sockfd,(struct sockaddr*)&caddr,&len);
        if(c < 0)
        {
            continue;
        }

        pid_t pid = fork();

        if(pid == 0)
        {
            while(1)
            {
                char buff[128] = {0};
                recv(c,buff,127,0);
                printf("buff=%s\n",buff);
                send(c,"ok",2,0);
            }
            close(c);
            exit(0);
        }

        close(c);
    }
    close(sockfd);
}
