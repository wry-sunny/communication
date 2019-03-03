#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<errno.h>

#define MAXFD 10

void setnonblock(int fd)
{
    int oldfl = fcntl(fd,F_GETFL);
    int newfl = oldfl | O_NONBLOCK;
    if(fcntl(fd,F_SETFL,newfl) == -1)
    {
        perror("fcntl error");
    }
}

void epoll_add(int epfd,int fd)
{
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = fd;
    
    if(epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev) == -1)
    {
        perror("epoll ctl error");
    }
    setnonblock(fd);
}

void epoll_del(int epfd,int fd)
{
    if(epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL) == -1)
    {
        perror("epoll ctl del error");
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

    int epfd = epoll_create(MAXFD);

    epoll_add(epfd,sockfd);
    struct epoll_event events[MAXFD];

    while(1)
    {
        int n = epoll_wait(epfd,events,MAXFD,5000);
        if(n == -1)
        {
            printf("epoll_wait error\n");
            continue;
        }
        else if(n == 0)
        {
            printf("time out\n");
            continue;
        }
        else
        {
            int i = 0;
            for( ; i < n; i++)
            {
                int fd = events[i].data.fd;
                if(events[i].events & EPOLLIN)
                {
                    if(fd == sockfd)
                    {
                        int len = sizeof(caddr);
                        int c =accept(fd,(struct sockaddr*)&caddr,&len);
                        if(c<0)
                        {
                            continue;
                        }
                        printf("accept c=%d\n",c);
                        epoll_add(epfd,c);
                    }
                    else
                    {
                        while(1)
                        {
                            char buff[128] = {0};
                            int num = recv(fd,buff,127,0);
                            if(num == -1)
                            {
                                if(errno == EAGAIN || errno == EWOULDBLOCK)
                                {
                                    send(fd,"ok",2,0);
                                }
                                break;
                            }
                            else if(num == 0)
                            {
                                epoll_del(epfd,fd);
                                close(fd);
                                printf("one client close\n");
                                break;
                            }
                            printf("recv(%d)=%s\n",fd,buff);
                        }
                    }
                }
            }
        }
    }
}
