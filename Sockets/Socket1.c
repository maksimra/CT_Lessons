#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>

int main (const int argc, const char** argv)
/* Полюбоваться на созданный сокет man 2 socket man 2 bind*/
{
	int sock;
	struct sockaddr_un saddr;

	if (argc < 2)
	{
		fprintf (stderr, "Too few arguments\n");
		return 1;
	}

    /* Создаём сокет */
	sock = socket (AF_UNIX, SOCK_STREAM, 0);
	if (sock == -1)
	{
		fprintf (stderr, "socket() error\n");
		return EXIT_FAILURE;
	}

	saddr.sun_family = AF_UNIX;

	/* Присвоить массив в структуре нельзя, но можно скопировать содержимое.*/
    strcpy (saddr.sun_path, argv[1]);

	if (bind (sock, (struct sockaddr *) &saddr,	sizeof saddr) == -1)
	{
		fprintf (stderr, "bind() error\n");
		return 1;
	}

	fprintf (stderr, "Press <Enter> to continue...");
	fgetc (stdin);

	int cl = close (sock);

    /* Удаление файла сокета */
	int un = unlink (argv[1]);
	return 0;
}
