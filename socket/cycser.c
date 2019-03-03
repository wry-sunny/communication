#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>

void* fun(void *arg)
{
    int c = (int)arg;

        while(1)
        {
            char buff[128] = {0};
            int n = recv(c,buff,127,0);
            if(n <= 0)
            {
                break;
            }
            printf("buff=%s\n",buff);
            send(c,"ok",2,0);
        }
        printf("one client over\n");
        close(c);
}


int main()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    assert(sockfd != -1);

    struct sockaddr_in saddr,caddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6000);//管理员可以使用 端口值 1024以内知名端口，1024-4096 保留端口，4096以上 临时端口，应用程序使用的
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int res = bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    assert(res != -1);

    listen(sockfd,5);//不会阻塞

    while(1)
    {
        int len = sizeof(caddr);
        int c = accept(sockfd,(struct sockaddr*)&caddr,&len);//若以完成三次握手为空，则阻塞
        if(c < 0)
        {
            continue;
        }

        printf("accept c=%d\n",c);
        pthread_t id;
        pthread_create(&id,NULL,fun,(void *)c);
    }

}
