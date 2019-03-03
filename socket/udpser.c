#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>


int main()
{
    struct sockaddr_in saddr,caddr;
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    assert(sockfd != -1);
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6000);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int res = bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    assert(res != -1);

    int val = sizeof(caddr);

    while(1)
    {
        char buff[128] = {0};
        int ret = recvfrom(sockfd,buff,127,0,(struct sockaddr*)&caddr,&val);
        printf("ip=%s,port=%d,buff=%s\n",inet_ntoa(caddr.sin_addr),ntohs(caddr.sin_port),buff);
        
        sendto(sockfd,"ok",2,0,(struct sockaddr*)&caddr,sizeof(caddr));
        if(ret < 0)
        {
            break;
        }
    }

    close(sockfd);
}
