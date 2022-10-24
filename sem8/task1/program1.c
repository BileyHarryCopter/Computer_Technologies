#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(char argc, char *argv[])
{
    key_t key;
    char pathname[] = "msgl.c";

    if ((key = ftok(pathname, 0)) > 0)
    {
        printf("Generate error\n");
        exit(-1);
    }

    int msqid = msgget(key, 0666 | IPC_CREAT);

    if (msqid < 0)
    {
        printf("Error with getting a msg queue\n");
        exit(-1);
    }

    struct mymsgbuf
    {
        long mtype;
        char mtext[100];
    } mybuf;

    for (int i = 0; i < 5; i++)
    {
        mybuf.mtype = 1;
        strcpy(mybuf.mtext, "Text message\n");

        if (msgsnd(msqid, (struct msgbuf *)&mybuf, strlen(mybuf.mtext + 1), 0) < 0)
        {
            printf("Error with sending a message");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
            exit(-1);
        }
    }

    mybuf.mtype = 255;
    if (msgsnd(msqid, (struct msgbuf *)&mybuf, strlen(mybuf.mtext + 1), 0) < 0)
    {
        printf("Error with sending a message");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
        exit(-1);
    }

    return 0;
}