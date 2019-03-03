#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>


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

    while(1)
    {
        int len = sizeof(caddr);
        int c = accept(sockfd,(struct sockaddr*)&caddr,&len);
        if(c < 0)
        {
            continue;
        }

        char buff[128] = {0};
        recv(c,buff,127,0);
        printf("buff=%s\n",buff);
        send(c,"ok",2,0);

        close(c);
    }

}
