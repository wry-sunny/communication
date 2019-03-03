#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>


int main()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    assert(sockfd != -1);

    struct sockaddr_in saddr,caddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(80);
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

        char buff[1024] = {0};
        recv(c,buff,1000,0);
        printf("recv:\n%s\n",buff);

        int fd = open("index.html",O_RDONLY);
        if(fd == -1)
        {
            close(c);
            continue;
        }

        int size = lseek(fd,0,SEEK_END);
        lseek(fd,0,SEEK_SET);
        //char *s = "你好，秋天!";
        char sendbuff[256] = {0};
        strcpy(sendbuff,"http/1.1 200 OK\r\n");
        strcat(sendbuff,"Server: myhttp\r\n");
        //strcat(sendbuff,"Content-Length: 5"\r\n);
        sprintf(sendbuff+strlen(sendbuff),"Content-length: %d\r\n",size);
        strcat(sendbuff,"\r\n");
        //strcat(sendbuff,"hello");

        printf("send:\n%s\n",sendbuff);
        send(c,sendbuff,strlen(sendbuff),0);

        char readbuff[512] = {0};
        int n = 0;
        while((n=read(fd,readbuff,512))>0)
        {
            send(c,readbuff,n,0);
        }
        close(c);
    
    close(fd);
    }
}
