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
    MAX_NUMB = 1000
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

    int fd = 0, fd_aux = 0;
    pid_t process = fork();

    if (process < 0)
    {
        printf("Failure with fork\n");
        exit(-1);
    }
    else if (process == 0)
    {
        sleep(1);

        if ((fd_aux = open("jep.txt", O_RDONLY)) < 0)
        {
            printf("Can't open file jep\n");
            exit(-1);
        }

        char *buff = (char *)calloc(MAX_NUMB, sizeof(char));
        int read_res = read(fd_aux, buff, MAX_NUMB);
        if (read_res < 0)
        {
            printf("Failure with reading from jep\n");
            exit(-1);
        }

        if (close(fd_aux) < 0)
        {
            printf("Can't close file JEP\n");
            exit(-1);
        }

        if ((fd = open(file_name, O_WRONLY, 0666)) < 0)
        {
            printf("Failure with openning fifo_file\n");
            exit(-1);
        }

        int size_buff = strlen(buff) + 1;
        int write_res = write(fd, buff, size_buff);
        if (write_res != size_buff)
        {
            printf("Failure with writing to FIFO\n");
            exit(-1);
        }

        if (close(fd) < 0)
        {
            printf("Can't close fifo_file\n");
            exit(-1);
        }

        free(buff);
    }
    else
    {
        if ((fd_aux = open("jep.txt", O_CREAT, 0666)) < 0)
        {
            printf("Can't open file jep\n");
            exit(-1);
        }
        close(fd_aux);

        char command[LEN_PATH] = "ls ";
        if (dir_name == NULL)
            strcat(command, ". ");
        else
        {
            strcat(command, dir_name);
            strcat(command, " ");
        }

        strcat(command, "> jep.txt");
        printf("%s\n", command);

        if (execl("/bin/bash", "/bin/bash", "-c", command) < 0)
        {
            printf("Unsuccessful execution of the program\n");
            exit(-1);
        }
    }

    return 0;
}
