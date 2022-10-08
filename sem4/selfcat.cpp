#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
	int sample = 0;
	if (sample = open("selfcat.cpp", O_RDONLY) < 0)
	{
		printf("Failure with opening selfcat.cpp, LOL\n");
		exit(1);
	}

	char* buff =(char *) calloc(1000, sizeof(char));
	read(sample, buff, 1000);
	close(sample);
	printf("%s\n", buff);

	return 0;
}
