#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

/* Дочерний процесс, анализирующий год и возвращающий различные сигналы,
 * в зависимости от его високосности.
 */
int main (const int argc, const char* argv[])
{
	if (argc < 2)
	{
		fprintf (stderr, "child: too few arguments\n");
		return 2;
	}
    printf ("I am here\n");

    /* Получаем int, соответствующий второму аргументу */
	int year = atoi (argv[1]);

	if (year <= 0)
		return 2;

    /* Если год високосный, бросаем родительскому процессу SIGUSR1,
       если нет - SIGUSR2 */
	if (((year%4 == 0) && (year%100 != 0)) ||
		(year%400 == 0))
		kill (getppid (), SIGUSR1);
	else
		kill (getppid (), SIGUSR2);

	return 0;
}
