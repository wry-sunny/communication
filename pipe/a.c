#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<assert.h>
#include<signal.h>

void Fun(int sig)
{
	printf("sig=%d\n", sig);
}

int main()
{
	signal(SIGPIPE, Fun);
	int fdw = open("fifo", O_WRONLY);
	printf("fdw=%d\n", fdw);
	printf("input:\n");

	while (1)
	{
		char buff[128] = { 0 };
		fgets(buff, 128, stdin);

		if (strncmp(buff, "end", 3) == 0)
		{
			break;
		}

		write(fdw, buff, strlen(buff));
	}

	close(fdw);

	exit(0);
}