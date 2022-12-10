#include "runsim.h"

int getnumb(const char *str)
{
    int numb = 0;
    for (int i = 0, strsize = strlen(str); i < strsize; i++)
    {
        numb += str[i] - '0';
        numb *= 10;
    }
    return numb / 10;
}

int getcmd(char **cmd)
{
    *cmd = (char *)calloc(MAXLEN, sizeof(char));
    for (char i = 0, symb = getc(stdin); (symb != '\n') && (i < MAXLEN); i++)
    {
        (*cmd)[i] = symb;
        symb = getc(stdin);
    }
}

int delcmd(char *cmd)
{
    free(cmd);
}