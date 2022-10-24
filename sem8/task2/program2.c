#include <fcntl.h>    // open
#include <unistd.h>   // read, write, fork
#include <sys/stat.h> // umask, mknod
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <string.h>   // strlen

enum
{
    LEN_PATH = 100,
};

int main(char argc, char *argv[])
{
    if (argc != 2)
    {
        printf("The program should take an argument\n");
        return 0;
    }

    pid_t process = fork();

    if (process < 0)
    {
        printf("Fork can't create\n");
        exit(-1);
    }
    else if (process == 0)
    {
        char delcom[LEN_PATH] = "rm -rf ";
        strcat(delcom, argv[1]);

        printf("%s\n", delcom);

        if ((execl("/bin/bash", "/bin/bash", "-c", delcom) < 0))
        {
            printf("Unsuccessful execution of the program\n");
            exit(-1);
        }
    }
    else
    {
        char command[LEN_PATH] = "cat ";
        strcat(command, argv[1]);

        printf("%s\n", command);

        if ((execl("/bin/bash", "/bin/bash", "-c", command) < 0))
        {
            printf("Unsuccessful execution of the program\n");
            exit(-1);
        }
    }

    return 0;
}