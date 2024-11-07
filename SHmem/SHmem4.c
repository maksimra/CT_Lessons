#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char ** argv)
{
    /* IPC дескриптор для области разделяемой памяти */
    int shm_id;

    /* Указатель на разделяемую память */
    char * shm_buf;

    if (argc < 2)
    {
    	fprintf (stderr, "Too few arguments\n");
    	return 1;
    }

    /* Передали дескриптор из предыдущей программы */
    shm_id = atoi (argv[1]);

    /* Пытемся отобразить разделяемую память в
       адресное пространство текущего процесса */
    shm_buf = (char *) shmat (shm_id, 0, 0);
    if (shm_buf == (char *) (-1))
    {
    	fprintf (stderr, "shmat() error\n");
    	return 1;
    }

    printf ("Message: %s\n", shm_buf);

    /* Отсоединяем разделяемую память и завершаем работу */
    shmdt (shm_buf);

    /* Удаляем разделяемую память из системы */
    shmctl(shm_id,
           IPC_RMID,
           NULL);

    return 0;
}
