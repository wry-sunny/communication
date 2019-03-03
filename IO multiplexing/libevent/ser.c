#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<string.h>
#include<event.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define MAXFD 20

struct event* fds_map[MAXFD];

void fds_map_init(struct event* fds[])
{
    int i = 0;
    for(;i<MAXFD;i++)
    {
        fds[i] = NULL;
    }
}

void fds_map_add(struct event* fds[],int fd,struct event* ev)
{
    if(fd >= MAXFD || ev == NULL)
    {
        return;
    }

    fds[fd] = ev;
}

struct event* fds_map_find(struct event* fds[],int fd)
{
    return fds[fd];
}

void fds_map_del(struct event* fds[],int fd)
{
    if(fd >= MAXFD)
    {
        return;
    }
    fds[ds] = NULL;
}

void c_cb(int fd,short ev,void*arg)
{
    if(ev & EV_READ)
    {
        char buff[128] = {0};
        if(recv(fd,buff,127,0) <= 0)
        {
            event_free(fds_map_find(fds_map,fd));
            fds_map_del(fds_map,fd);
            close(fd);
            printf("one client over\n");
            return;_
        }
        printf("buff=%s\n",buff);
        send(fd,"ok",2,0);
    }
}

void accept_cb(int fd,short ev,void*arg)
{
    struct event_base = (struct event_base)arg;
    struct sockaddr_in caddr;
    int len = sizeof(caddr);
    if(ev & EV_READ)
    {
        int c = accept(fd,(struct sockaddr*)&caddr,&len);
        if(c<0)
        {
            return;
        }

        struct event*ev_c = event_new(base,c,EV_READ|EV_PERSIST,c_cb,NULL);
        event_add(ev_c,NULL);
        fds_map_add(fds_map,c,ev_c);
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

    fds_map_init(fds_map);

    struct event_base *base = event_init();
    assert(base != NULL);

    struct event* ev_sock = event_new(base,sockfd,EV_READ|PERSIST,accept_cb,base);
    event_add(ev_sock,NULL);
    fds_map_add(fds_map,sockfd,ev_sock);

    event_base_dispatch(base);

    event_free(ev_sock);
    event_base_free(base);
}
