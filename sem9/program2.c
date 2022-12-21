#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(char argc, char **argv)
{
    int msgid;
    char pathname[] = "program1.c";
    key_t key;

    if ((key = ftok(pathname, 0)) < 0)
    {
        printf("Can't generate a key\n");
        exit(-1);
    }

    if ((msgid = msgget(key, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can\'t get msgid\n");
        exit(-1);
    }

    struct mymsgbuf
    {
        long mtype;
        char mtext[100];
    } mybuf;

    mybuf.mtype = 1;
    strcat(mybuf.mtext, "Hello! I'm program 1! Vanya Loh");
    int len = strlen(mybuf.mtext) + 1;

    if (msgsnd(msgid, (struct msgbuf *)&mybuf, len, 0) < 0)
    {
        printf("Can't send the last message to the queue\n");
        if (msgctl(msgid, IPC_RMID, (struct msgid_ds *)NULL) < 0)
        {
            printf("Can't delete the queue from the system\n");
            exit(-1);
        }
        exit(-1);
    }

    while (1)
    {
        int maxlen = 100;
        if ((len = msgrcv(msgid, (struct msgbuf *)&mybuf,
                          maxlen, 0, 0)) < 0)
        {
            printf("Can't receive a message from the queue\n");
            exit(-1);
        }

        printf("Message: type = %ld, text = %s\n",
               mybuf.mtype, mybuf.mtext);

        if (mybuf.mtype == 2)
        {
            if (msgctl(msgid, IPC_RMID, (struct msqid_ds *)NULL) < 0)
            {
                printf("Can't delete the queue from the system\n");
                exit(-1);
            }
            exit(0);
        }
    }

    return 0;
}