#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#define LAST_MESSAGE 255

int main(void)
{
    int msqid;
    char pathname[] = "Q1.c";
    key_t  key;
    int len, maxlen;
    struct mymsgbuf
    {
        long mtype;
        char mtext[81];
    } mybuf;

    /* Генерируем ключ */
    key = ftok(pathname, 0);

    /* Получаем идентификатор очереди сообщений */
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    while (1)
    {
        maxlen = 81;

        /* Получаем сообщения любого типа */
        if ((len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 0, 0)) < 0)
        {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }

        if (mybuf.mtype == LAST_MESSAGE)
        {
            /* Удаляем очередь */
            msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
            exit(0);
        }

        /* Печатаем сообщения, посланные прошлой программой */
        printf("message type = %ld, info = %s\n", mybuf.mtype, mybuf.mtext);
    }
    return 0;
}
