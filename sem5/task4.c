#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SIZE_OF_SRC 10000

int main (char argc, char** argv)
{
    int is_created = 0; //  if file has existed already

    key_t key_1;
    if ((key_1 = ftok("task3.c", 0)) < 0)
    {
        printf("Failure with generation key\n");
        exit(1);
    }

    int id_1;
    size_t size_1 = SIZE_OF_SRC;
    if ((id_1 = shmget(key_1, size_1, 0666 | IPC_CREAT | IPC_EXCL)) < 0)
    {
        if (errno != EEXIST)
        {
            printf("Failure with receiving id_1\n");
            exit(1);
        }
        else if ((id_1 = shmget(key_1, size_1, 0)) < 0)
        {
            printf("Failure with finding shmem\n");
            exit(1);
        }
        is_created = 1;
    }

    char* buff_by_id1;
    if ((buff_by_id1 = shmat(id_1, NULL, 0)) == (char*)(-1))
    {
        printf("Failure with attempt address of address space\n");
        exit(1);
    }

    printf("%s", buff_by_id1);

    if (shmdt(buff_by_id1) < 0)
    {
        printf("Failure with detach the shmem\n");
        exit(1);
    }

    shmctl(id_1, IPC_RMID, NULL);

    return 0;
}
