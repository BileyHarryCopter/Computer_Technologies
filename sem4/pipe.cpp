#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
	int fp[2];

	if (pipe(fp) < 0)
	{
		printf("Failure with pipe\n");
		exit(-1);
	}

	char str[] = "Hello, World!\n";
	size_t nbytes = sizeof(str);

	if (write(fp[1], str, nbytes) != nbytes)
	{
		printf("Failure with writing to pipe[1]\n");
		exit(-1);
	}

	close(fp[1]);

	char *request = (char *) calloc (nbytes + 1, sizeof(char));
	if (read(fp[0], request, nbytes) != nbytes)
	{
		printf("Failure with reading from pipe[0]");
		exit(-1);
	}

	close (fp[0]);

	printf("%s\n", request);
	free(request);

	return 0;
}

