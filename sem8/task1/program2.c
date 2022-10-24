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

    int maxlen = 100;

    for (; 1;)
    {
        if (msgrcv(msqid, (struct msgbuf *)&mybuf, maxlen, 0, 0) < 0)
        {
            printf("Error with recieving a message\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
            exit(-1);
        }

        printf("Message: type %ld, text: %s\n", mybuf.mtype, mybuf.mtext);

        if (mybuf.mtype == 255)
        {
            msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
            exit(0);
        }
    }

    return 0;
}