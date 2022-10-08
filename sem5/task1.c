#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>


int main (char argc, char** argv)
{
    int is_created = 0; //  if file has existed already

    key_t key_1;
    if ((key_1 = ftok("task1.c", 0)) < 0)
    {
        printf("Failure with generation key\n");
        exit(1);
    }

    int id_1;
    size_t size_1 = 3 * sizeof(int);
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

    int* ptr_by_id1;
    if ((ptr_by_id1 = (int*)shmat(id_1, NULL, 0)) == (int*)(-1))
    {
        printf("Failure with attempt address of address space\n");
        exit(1);
    }

    if (is_created)
    {
        ptr_by_id1[0]++;
        ptr_by_id1[2]++;
    }
    else 
    {
        ptr_by_id1[0] = 1;
        ptr_by_id1[1] = 0;
        ptr_by_id1[2] = 1;
    }

    printf("Program 1: %d; Program 2: %d, sum: %d\n", ptr_by_id1[0], ptr_by_id1[1], ptr_by_id1[2]);

    if (shmdt(ptr_by_id1) < 0)
    {
        printf("Failure with detach the shmem\n");
        exit(1);
    }

    return 0;
}