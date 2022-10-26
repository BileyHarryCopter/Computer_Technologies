#include <fcntl.h>    // open
#include <unistd.h>   // read, write, fork
#include <sys/stat.h> // umask, mknod
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <string.h>   // strlen

enum
{
    LEN_PATH = 100,
    MAX_NUMB = 1000
};

int main(char argc, char *argv[])
{
    if (argc != 2)
    {
        printf("The program should take an argument\n");
        return 0;
    }

    char *fifo_file = argv[1];

    int fd = 0;
    if ((fd = open(fifo_file, O_RDONLY)) < 0)
    {
        printf("Can't open output stream\n");
        exit(-1);
    }

    pid_t process = fork();

    if (process < 0)
    {
        printf("Can't create fork process\n");
        exit(-1);
    }
    else if (process == 0)
    {
        char delcom[LEN_PATH] = "rm -rf ";
        strcat(delcom, argv[1]);

        if ((execl("/bin/bash", "/bin/bash", "-c", delcom) < 0))
        {
            printf("Unsuccessful execution of the program\n");
            exit(-1);
        }
    }
    else
    {
        char *info_dir = (char *)calloc(MAX_NUMB, sizeof(char));
        int numb_read = read(fd, info_dir, MAX_NUMB);
        if (numb_read < 0)
        {
            printf("Can't read file\n");
            exit(-1);
        }

        if (close(fd) < 0)
        {
            printf("Can't close output stream\n");
            exit(-1);
        }

        printf("%s\n", info_dir);
        free(info_dir);
    }

    return 0;
}
