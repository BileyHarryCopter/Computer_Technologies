/* Программа получает доступ к очереди сообщений
   и читает из нее сообщения с любым типом в порядке FIFO
   до тех пор, пока не получит сообщение с типом 255,
   которое будет служить сигналом прекращения работы */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Тип сообщения для прекращения работы второй программы
#define LAST_MESSAGE 255

int main()
{
    int msqid; // IPC-дескриптор для очереди сообщений
    /* Имя файла, использующееся для генерации ключа.
       Файл с таким именем должен существовать в
       текущей директории */
    char pathname[] = "msg1.c";
    key_t key; // IPC ключ

    /* Генерируем IPC-ключ
       из имени файла msq1.c в текущей директории
       и номера экземпляра очереди сообщений 0 */
    if ((key = ftok(pathname, 0)) < 0)
    {
        printf("Can't generate a key\n");
        exit(-1);
    }

    /* Пытаемся получить доступ по ключу
       к очереди сообщений, если она существует,
       или создать ее
       с правами доступа read & write для всех пользователей */
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    // Пользовательская структура для сообщения
    struct mymsgbuf
    {
        long mtype;
        char mtext[100];
    } mybuf;

    /* Реальная длина и
       максимальная длина информативной части сообщения */
    int len, maxlen = 0;

    while (1)
    {
        /* В бесконечном цикле принимаем сообщения любого типа
           в порядке FIFO с максимальной длиной
           информативной части 100 символ до тех пор, пока
           не поступит сообщение с типом LAST_MESSAGE */
        maxlen = 100;
        if ((len = msgrcv(msqid, (struct msgbuf *)&mybuf,
                          maxlen, 0, 0)) < 0)
        {
            printf("Can't receive a message from the queue\n");
            exit(-1);
        }

        printf("Message: type = %ld, text = %s\n",
               mybuf.mtype, mybuf.mtext);

        /* Если принятое сообщение имеет тип LAST_MESSAGE,
           прекращаем работу и удаляем очередь сообщений из системы.
           В противном случае печатаем текст принятого сообщения */
        if (mybuf.mtype == LAST_MESSAGE)
        {
            if (msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL) < 0)
            {
                printf("Can't delete the queue from the system\n");
                exit(-1);
            }
            exit(0);
        }
    }

    // Исключительно для отсутствия warning'ов при компиляции
    return 0;
}