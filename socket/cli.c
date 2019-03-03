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

    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6000);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int res = connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    assert(res != -1);

    char buff[128] = {0};

    printf("input:\n");
    fgets(buff,128,stdin);

    send(sockfd,buff,strlen(buff),0);
    memset(buff,0,128);
    recv(sockfd,buff,127,0);
    printf("buff=%s\n",buff);

    close(sockfd);

}
