#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define MESSAGE_LENGTH 255

struct mymsgbuf
{
    long mtype;
    char mtext[MESSAGE_LENGTH];
};

int msqid; // Глобальная переменная для идентификатора очереди

// Функция очистки очереди сообщений при завершении программы
void cleanup()
{
    if (msgctl (msqid, IPC_RMID, NULL) < 0)
    {
        fprintf (stderr, "Ошибка при удалении очереди сообщений!\n");
    }
    exit (EXIT_SUCCESS);
}

int main (const int argc, const char* argv[])
{
    const int necessary_num_args = 3;

    if (argc < necessary_num_args)
    {
        fprintf (stderr, "Программа должна получить на вход два номера!\n");
        return EXIT_FAILURE;
    }

    int rcv_number = atoi (argv[1]);
    int snd_number = atoi (argv[2]);

    char pathname[] = "Q2.c";
    key_t key;

    // Генерация ключа
    if ((key = ftok (pathname, 0)) == -1)
    {
        fprintf (stderr, "Ошибка при генерации ключа!\n");
        return EXIT_FAILURE;
    }

    // Создание очереди сообщений
    if ((msqid = msgget (key, 0666 | IPC_CREAT)) < 0)
    {
        fprintf (stderr, "Ошибка при создании или получении очереди сообщений!\n");
        return EXIT_FAILURE;
    }

    // Обработка сигнала для корректного завершения
    signal (SIGINT, cleanup);

    pid_t pid = fork ();
    if (pid < 0)
    {
        fprintf (stderr, "Ошибка при создании процесса!\n");
        return EXIT_FAILURE;
    }

    if (pid == 0) /* Дочерний процесс — получение сообщений */
    {
        struct mymsgbuf mybuf;
        while (1)
        {
            // Ожидание сообщения
            if (msgrcv (msqid, &mybuf, sizeof (mybuf.mtext), rcv_number, 0) < 0)
            {
                fprintf (stderr, "Ошибка при получении сообщения!\n");
                exit (EXIT_FAILURE);
            }
            printf ("Получено сообщение: %s\n", mybuf.mtext);
        }
    }
    else /* Родительский процесс — отправка сообщений */
    {
        struct mymsgbuf mybuf;
        mybuf.mtype = snd_number; // Установка типа сообщения

        while (1)
        {
            printf ("Введите сообщение: ");
            if (fgets (mybuf.mtext, MESSAGE_LENGTH, stdin) == NULL)
            {
                fprintf (stderr, "Ошибка при вводе сообщения!\n");
                cleanup ();
            }

            // Удаление символа новой строки из сообщения
            size_t len = strlen (mybuf.mtext);
            if (len > 0 && mybuf.mtext[len - 1] == '\n')
            {
                mybuf.mtext[len - 1] = '\0';
            }

            // Отправка сообщения
            if (msgsnd (msqid, &mybuf, strlen (mybuf.mtext) + 1, 0) < 0)
            {
                fprintf (stderr, "Ошибка при отправке сообщения!\n");
                cleanup ();
            }
        }
    }

    return 0;
}
