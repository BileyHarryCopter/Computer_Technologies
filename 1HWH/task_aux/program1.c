#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>

enum
{
    DEFAULT = 1,
    LEN_PATH = 100
};

char *get_path(const char *file, const char *dir)
{
    char *path = (char *)calloc(LEN_PATH, sizeof(char));
    if (dir != NULL)
    {
        strcat(path, dir);
        strcat(path, "/");
    }
    strcat(path, file);
    return path;
}

int main(char argc, char *argv[])
{

    if (argc != 2 && argc != 3)
    {
        printf("Not enough arguments\n");
        return 0;
    }

    char *file_name = argv[1];
    char *dir_name = argv[2];

    umask(0);
    if (mkfifo(file_name, 0666) < 0)
    {
        printf("Error with creation fifo label\n");
        exit(-1);
    }

    int fd = 0;
    if ((fd = open(file_name, O_WRONLY)) < 0)
    {
        printf("Failure with openning fifo_file\n");
        exit(-1);
    }

    pid_t process = fork();

    if (process < 0)
    {
        printf("Failure with fork\n");
        exit(-1);
    }
    else if (process == 0)
    {
        if (close(fd) < 0)
        {
            printf("Can't close fifo_file\n");
            exit(-1);
        }
    }
    else
    {
        char command[LEN_PATH] = "ls ";
        if (dir_name == NULL)
            strcat(command, ". ");
        else
        {
            strcat(command, dir_name);
            strcat(command, " ");
        }
        if (execl("/bin/bash", "/bin/bash", "-c", command) < 0)
        {
            printf("Unsuccessful execution of the program\n");
            exit(-1);
        }
    }

    return 0;
}
