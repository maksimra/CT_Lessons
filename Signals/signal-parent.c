#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <signal.h>
#include <stdlib.h>

/* Программа запускает дочерний процесс, который анализирует чиcло (год),
 * введённое как параметр и возвращает различные сигналы,
 * в зависимости от его високосности.*/
/* 0 - no signal, 1 - SIGUSR1, 2 - SIGUSR2 */
sig_atomic_t sig_status = 0;

void handle_usr1 (int s_num)
{
	sig_status = 1;
}

void handle_usr2 (int s_num)
{
	sig_status = 2;
}

int main (int argc, char ** argv)
{
	struct sigaction act_usr1 = {}, act_usr2 = {};

    /* Инициализируем signal sa_mask пустым набором сигналов */
	sigemptyset (&act_usr1.sa_mask);
	sigemptyset (&act_usr2.sa_mask);

    /* Устанавливаем флаги для act_usr1 и act_usr2 как 0 (нет дополнительных флагов) */
	act_usr1.sa_flags = 0;
	act_usr2.sa_flags = 0;

    /* Устанавливаем действия, ассоциированные с signum */
	act_usr1.sa_handler = &handle_usr1;
	act_usr2.sa_handler = &handle_usr2;

    /* Изменяем реакцию на сигнал SIGUSR1 */
	if (sigaction (SIGUSR1, &act_usr1, NULL) == -1)
	{
		fprintf (stderr, "sigaction (act_usr1) error\n");
		return 1;
	}

    /* Изменяем реакцию на сигнал SIGUSR2 */
	if (sigaction (SIGUSR2, &act_usr2, NULL) == -1)
	{
		fprintf (stderr, "sigaction (act_usr2) error\n");
		return 1;
	}

    /* Проверяем число аргументов */
	if (argc < 2)
	{
		fprintf (stderr, "Too few arguments\n");
		return 1;
	}

    printf ("A AM HERER\n");
    /* Заходим в дочернем процессе */
	if (!fork())
	{
        /* Заменяем текущий процесс на signal-child */
		execl ("./child", "child", argv[1], (char*) NULL);
		fprintf (stderr, "execl() error\n");
		return 1;
	}

	while (1)
	{
		if (sig_status == 1)
		{
			printf ("%s: leap year\n", argv[1]);
			return 0;
		}

		if (sig_status == 2)
		{
			printf ("%s: not leap year\n", argv[1]);
			return 0;
		}
	}

	return 0;
}
