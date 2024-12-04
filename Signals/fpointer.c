#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUM_SYMBOLS 80

enum Mode
{
    FULL_CMP   = 0,
    LENGTH_CMP = 1
};

struct ModeInfo
{
    enum Mode   mode_enum;
    const char* flag_name;
};

const struct ModeInfo MODES[] = {{FULL_CMP,   "-f"},
                                 {LENGTH_CMP, "-n"}};

void check (char *a, char *b,
            int (*cmp)(const char *, const char *))
{
    printf("Проверка на совпадение.\n");
    if(!(*cmp)(a, b))
        printf("Равны\n");
    else
        printf("Не равны\n");
}

int n_symb_cmp (const char* a, const char* b)
{
    return strlen (a) - strlen (b);
}

int main (const int argc, const char* argv[])
{
    char s1[MAX_NUM_SYMBOLS] = {};
    char s2[MAX_NUM_SYMBOLS] = {};

    int (*p)(const char *, const char *);

    if (argc > 1)
    {
        const char* mode_name = argv [1];
        if (strcmp (mode_name, MODES[0].flag_name) == 0)
            p = strcmp;
        else if (strcmp (mode_name, MODES[1].flag_name) == 0)
            p = n_symb_cmp;
    }
    else
    {
        fprintf (stderr, "Требуется флаг:\n"
                         "\"-f\" - полное сравнение строк\n"
                         "\"-n\" - сравнение на длину\n");
        return EXIT_FAILURE;
    }

    printf("Введите две строки.\n");
    fgets(s1, MAX_NUM_SYMBOLS, stdin);
    fgets(s2, MAX_NUM_SYMBOLS, stdin);

    check(s1, s2, p);

    return 0;
}
