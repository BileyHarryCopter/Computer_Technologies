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

    pid_t pid = fork();

    if (pid < 0)
    {
        printf("Can't fork the process\n");
        exit(-1);
    }
    else if (pid == 0)
    {
        int maxlen = 100;
        if (msgrcv(msgid, (struct msgbuf *)&mybuf,
                   maxlen, 1, 0) < 0)
        {
            printf("Can't receive a message from the queue\n");
            exit(-1);
        }

        printf("Program 2 message: type = %ld, text = %s\n",
               mybuf.mtype, mybuf.mtext);

        mybuf.mtype = 2;
        strcpy(mybuf.mtext, "Hello, I am program 2!");
        int len = strlen(mybuf.mtext) + 1;

        if (msgsnd(msgid, (struct msgbuf *)&mybuf, len, 0) < 0)
        {
            printf("Can't send a message to the queue\n");
            if (msgctl(msgid, IPC_RMID, (struct msgid_ds *)NULL) < 0)
            {
                printf("Can't delete the queue from the system\n");
                exit(-1);
            }
            exit(-1);
        }
    }
    else
    {
        mybuf.mtype = 1;
        strcpy(mybuf.mtext, "Hello, I am program 1!");
        int len = strlen(mybuf.mtext) + 1;

        if (msgsnd(msgid, (struct msgbuf *)&mybuf, len, 0) < 0)
        {
            printf("Can't send a message to the queue\n");
            if (msgctl(msgid, IPC_RMID, (struct msgid_ds *)NULL) < 0)
            {
                printf("Can't delete the queue from the system\n");
                exit(-1);
            }
            exit(-1);
        }

        int maxlen = 100;
        if ((len = msgrcv(msgid, (struct msgbuf *)&mybuf,
                          maxlen, 2, 0)) < 0)
        {
            printf("Can't receive a message from the queue\n");
            exit(-1);
        }

        printf("Program 1 message: type = %ld, text = %s\n",
               mybuf.mtype, mybuf.mtext);
    }

    return 0;
}