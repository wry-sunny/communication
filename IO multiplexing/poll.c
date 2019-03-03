#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<string.h>
#include<netinet/in.h>
#include<poll.h>
#include<sys/time.h>
#define _GNU_SOURCE
#define MAXFD 10

void fds_init(struct pollfd fds[])
{
    int i = 0;
    for(; i<MAXFD;i++)
    {
        fds[i].fd = -1;
        fds[i].events = 0;
        fds[i].revents = 0;
    }
}

void fds_add(struct pollfd fds[],int fd)
{
    int i = 0;
    for(;i<MAXFD;i++)
    {
        if(fds[i].fd == -1)
        {
            fds[i].fd = fd;
            fds[i].events = POLLIN | POLLRDHUP;
            break;
        }
    }
}

void fds_del(struct pollfd fds[],int fd)
{
    int i = 0;
    for(;i<MAXFD;i++)
    {
        if(fds[i].fd == fd)
        {
            fds[i].fd = -1;
            fds[i].events = 0;
            break;
        }
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

    struct pollfd fds[MAXFD];
    fds_init(fds);

    fds_add(fds,sockfd);

    while(1)
    {
        int n = poll(fds,MAXFD,5000);
        if(n == -1)
        {
            printf("poll error\n");
        }
        else if(n == 0)
        {
            printf("time out\n");
        }
        else
        {
            int i = 0;
            for(;i<MAXFD;i++)
            {
                if(fds[i].fd == -1)
                {
                    continue;
                }
                if(fds[i].revents & POLLRDHUP)
                {
                    close(fds[i].fd);
                    printf("one client hup\n");
                    fds_del(fds,fds[i].fd);
                }
                if(fds[i].revents & POLLIN)
                {
                    if(fds[i].fd == sockfd)
                    {
                        int len = sizeof(caddr);
                        int c =accept(fds[i].fd,(struct sockaddr*)&caddr,&len);
                        if(c<0)
                        {
                            continue;
                        }
                        printf("accept c=%d\n",c);
                        fds_add(fds,c);
                    }
                    else
                    {
                        char buff[128] = {0};
                        int num = recv(fds[i].fd,buff,127,0);
                        if(num <= 0)
                        {
                            close(fds[i].fd);
                            printf("one client close\n");
                            fds_del(fds,fds[i].fd);
                        }
                        else
                        {
                            printf("recv(%d)=%s\n",fds[i].fd,buff);
                            send(fds[i].fd,"ok",2,0);
                        }
                    }
                }
            }
        }
    }
}
