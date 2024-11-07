/* Программа для записи текста исходного файла в разделяемую память */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

 /* Предполагаем, что размер исходного файла < SIZE */
#define SIZE 65535

int main()
{
  /* Указатель на разделяемую память */
  char *memory;

  /* IPC дескриптор для области разделяемой памяти */
  int shmid;

  /* Переменные для файлового дескриптора и количества прочитанных байт */
  int fd, nread;

   /* Имя файла, использующееся для генерации ключа и чтения исходного текста.
   Файл с таким именем должен существовать в текущей директории */
  char pathname[] = "SHmem.txt";

  /* IPC ключ */
  key_t key;

  /* Генерируем IPC ключ из имени файла и номера экземпляра области разделяемой памяти 0 */
  if((key = ftok(pathname, 0)) < 0)
  {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  /* Пытаемся создать разделяемую память для сгенерированного ключа */
  if((shmid = shmget(key, SIZE, 0666|IPC_CREAT)) < 0)
  {
    printf("Can\'t create shared memory\n");
    exit(-1);
  }

  /* Пытаемся отобразить разделяемую память в адресное пространство текущего процесса */
  if((memory = (char *)shmat(shmid, NULL, 0)) == (char *)(-1))
  {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

  /* Открываем файл только на чтение*/
  if((fd = open(pathname,O_RDONLY)) < 0)
  {
    printf("Can't open source file\n");
    (void)shmdt(memory);
    exit(-1);
  }

  /* Читаем файл порциями по 1kb до тех пор, пока не достигнем конца файла
  или не возникнет ошибка */
  char* start = memory;
  while((nread = read(fd, memory, 1024)) > 0)
  {
    printf ("nread = %d\n", nread);
    memory += nread;
  }

  /* Закрываем файл */
  (void)close(fd);

  /* Если возникла ошибка - завершаем работу */
  if(nread < 0)
  {
    printf("Error reading source file\n");
    (void)shmdt(memory);
    exit(-1);
  }

  /* После всего считанного текста вставляем признак конца строки,
  чтобы впоследствии распечатать все одним printf'ом */
  *memory = '\0';

  /* Печатаем содержимое буфера.*/
  printf ("%s\n", memory);

  /* Отсоединяем разделяемую память и завершаем работу */
  if(shmdt(start) < 0)
  {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  return 0;
}
