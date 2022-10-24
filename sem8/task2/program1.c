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
    LEN_PATH = 100,
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
    char *path = get_path(file_name, dir_name);

    umask(0);
    if (mkfifo(file_name, 0666) < 0)
    {
        printf("Error with creation fifo label\n");
        exit(-1);
    }

    char command[LEN_PATH] = "ls ";
    if (dir_name == NULL)
        strcat(command, ". ");
    else
    {
        strcat(command, dir_name);
        strcat(command, " ");
    }
    strcat(command, "> ");
    strcat(command, file_name);

    printf("%s\n", command);

    if (execl("/bin/bash", "/bin/bash", "-c", command) < 0)
    {
        printf("Unsuccessful execution of the program\n");
        exit(-1);
    }

    return 0;
}