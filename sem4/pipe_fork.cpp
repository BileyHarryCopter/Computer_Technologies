#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

//	Write a message by parent proccess, read this by child proccess and print this in the terminal
//	Figure out about fork()

int main(void)
{
	int fp[2];

	//! Create a pipe
	pipe(fp);

	char parent_mes[] = "Hello, Son!\n";
	size_t nbytes = sizeof(parent_mes);
	if (write(fp[1], parent_mes, nbytes) != nbytes)
	{
		printf("Failure with writing to pipe[1]\n");
		exit(-1);
	}

	fork();
	close(fp[1]);

	char* child_mes = (char*) calloc (nbytes, sizeof(char));
	if (
	read(fp[0], child_mes, nbytes) != nbytes);
	{
		printf("Failure with reading from pipe[0]\n");
		exit(-1);
	}

	close(fp[0]);

	printf ("%s\n", child_mes);
	free(child_mes);
	return 0;
}
