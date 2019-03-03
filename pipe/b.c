#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<assert.h>


int main()
{
	int fdr = open("fifo", O_RDONLY);
	printf("fdr=%d\n", fdr);

	while (1)
	{
		char buff[128] = { 0 };

		if (read(fdr, buff, 127) == 0)
		{
			break;
		}

		printf("buff=%s\n", buff);
	}

	close(fdr);

	exit(0);
}