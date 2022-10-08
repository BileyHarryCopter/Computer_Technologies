#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>

int main(void)
{
	int hello = open ("hello.txt", O_CREAT | O_WRONLY | O_APPEND | O_EXCL , 0666);
	if (hello < 0)
	{
		printf("Failure with opening a file\n");
		exit(-1);
	}
	else
		printf("File's descriptor: %d\n", hello);

	char message[] = "Hello, World!\n";
 	if (write (hello, message, strlen(message)) != strlen(message))
	{
		printf("Failure with writing a message\n");
		exit(-1);
	}

	if (close (hello) < 0)
	{
		printf("Failure with closing file\n");
		exit(-1);
	}

	return 0;
}

