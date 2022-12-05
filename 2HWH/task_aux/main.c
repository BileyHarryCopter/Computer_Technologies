#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#define FILE_SIZE 10000

int main(char argc, char *argv[])
{
    if (argc > 2)
    {
        printf("Too many arguments\n");
        return 0;
    }
    char *file_name = argv[argc - 1];

    struct stat buf;
    int status = stat(file_name, &buf);
    if (status == 0)
    {
        printf("File should be unexisting for mapping\n");
        return 0;
    }

    int fd = open(file_name, O_RDWR | O_CREAT, 0666);
    ftruncate(fd, FILE_SIZE);

    //  mapping the file to the virtual memory
    char *file_ptr = (char *)mmap(NULL, FILE_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

    close(fd);

    if (file_ptr == MAP_FAILED)
    {
        printf("Error with mapping the file\n");
        exit(-1);
    }

    pid_t res = fork();

    if (res < 0)
    {
        printf("Can't fork the process\n");
        exit(-1);
    }
    else if (res == 0) //  inside the child
    {
        char *tmp_ptr = file_ptr;
        int source_fd = open("main.c", O_RDONLY, 0666);

        read(source_fd, tmp_ptr, FILE_SIZE);

        close(source_fd);
    }
    else //  inside the parent
    {
        //  waiting the end of the work of child process
        int wstatus;
        wait(&wstatus);

        printf("%s\n", file_ptr);

        if (munmap(file_ptr, FILE_SIZE) == -1)
        {
            printf("Error with munmapping the file\n");
            exit(-1);
        }

        //  remoting the file
        execlp("rm", "rm", "-rf", file_name, (char *)NULL);
    }

    return 0;
}