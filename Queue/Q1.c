#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#define LAST_MESSAGE 255

int main (void)
{
    int  msqid = 0;
    char pathname[] = "Q1.c";
    key_t  key;
    int i = 0, len = 0;
    struct mymsgbuf
    {
        long mtype;
        char mtext[81];
    } mybuf;

    /* Create or attach message queue  */
    key = ftok(pathname, 0);
    if (key == -1)
    {
        fprintf (stderr, "Error ftok\n");
        exit (-1);
    }

    /* Создаём новую очередь сообщений */
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    /* Send information */
    for (i = 1; i <= 5; i++)
    {
        mybuf.mtype = 1;

        /* Копируем сообщение в буффер структуры */
        strcpy(mybuf.mtext, "This is text message");
        len = strlen(mybuf.mtext)+1;

        /* Пытаетмся записать сообщение в очередь */
        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0)
        {
            printf("Can\'t send message to queue\n");
            /* Удаляем очередь */
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
    }

    /* Send the last message */
    mybuf.mtype = LAST_MESSAGE;
    len = 0;

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0)
    {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    return 0;
}
