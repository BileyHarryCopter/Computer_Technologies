#include "useless.h"

//  Execution entity
typedef struct
{
    int sec;
    char *com;
    char **flags;
} execent;

typedef struct
{
    execent *execent_i;
    int size;
} execent_ar;

int file_opening(const char *file_name)
{
    int file_des = open(file_name, O_RDONLY);
    if (file_des == OPEN_ER)
    {
        printf("Opening of %s is failed\n", file_name);
        exit(-1);
    }
    return file_des;
}

int file_closing(const int des)
{
    int res = close(des);
    if (res == -1)
    {
        printf("Error with closing a file\n");
        exit(-1);
    }
    return res;
}

size_t file_size(const int des)
{
    size_t file_sz = 0;

    struct stat file_stat;
    if (fstat(des, &file_stat) == -1)
    {
        printf("Error with file stat\n");
        exit(-1);
    }
    file_sz = file_stat.st_size;
    if (file_sz == EMPTY_FILE)
    {
        printf("File with commands is empty\n");
        exit(-1);
    }

    return file_sz;
}

int get_sec(char *buff, int *pos)
{
    int seconds = 0;
    for (; isdigit(buff[*pos]); ++(*pos))
    {
        seconds += buff[*pos] - '0';
        seconds *= 10;
    }
    seconds /= 10;
    return seconds;
}

void passspace(char *buff, int *pos)
{
    for (; (buff[*pos] != '\0') && (buff[*pos] != '\n') && isspace(buff[*pos]); ++(*pos))
    {
        ;
    }
}

char *command_init(char *buff, int *pos)
{
    char *istr = (char *)calloc(MAX_LENGTH, sizeof(char));
    int ipos = 0;
    for (; (buff[*pos] != '\0') && (!isspace(buff[*pos])); ++(*pos))
    {
        if (ipos >= MAX_LENGTH - 1)
        {
            printf("Command size of file is bigger than recomended. \
                    From developer with love)\n");
            exit(-1);
        }
        istr[ipos] = buff[*pos];
        ++ipos;
    }
    return istr;
}

char **flags_init(char *buff, int *pos, char *command)
{
    char **flags = 0;
    passspace(buff, pos);
    if ((*buff == '\n') || (*buff == '\0'))
        return flags;

    flags = (char **)calloc(FLAGS_NUMB, sizeof(char *));
    flags[0] = command;
    int ipos = 1;
    for (; ipos < FLAGS_NUMB;)
    {
        if (isspace(buff[*pos]) && buff[*pos] != '\n' && buff[*pos] != '\0')
        {
            ++(*pos);
            continue;
        }
        else if (buff[*pos] == '\n' || buff[*pos] == '\0')
            break;
        else
        {
            flags[ipos] = command_init(buff, pos);
            ++ipos;
        }
    }
    if (ipos == FLAGS_NUMB)
    {
        for (; buff[*pos] != '\0' && buff[*pos] != '\n';)
            ++(*pos);
        if (buff[*pos] == '\0' || buff[*pos] == '\n')
        {
            printf("Number of flags for executable program is more than recomended. \
                    From developer with love\n");
            exit(-1);
        }
    }
    return flags;
}

void free_flags(char **flags)
{
    for (int pos = 0; flags[pos] != NULL; ++pos)
    {
        free(flags[pos]);
    }
}

char *get_command(char *flags[])
{
    char *command = (char *)calloc(MAX_LENGTH, sizeof(char));
    for (int pos = 0; flags[pos] != NULL; pos++)
    {
        strcat(command, flags[pos]);
        strcat(command, " ");
    }
    return command;
}

void execute_func(const int sec, char *command, char *flags[])
{
    printf("\nTIME: %d\tPROGRAM: %s\n\n", sec, command);

    pid_t process = fork();

    if (process == -1)
    {
        printf("Error with create of execution process\n");
        exit(-1);
    }
    else if (process == 0)
    {
        if ((command[0] == '.') && (execv(command, flags) < 0))
        {
            printf("ГОЙДА!\n");
            printf("Unsuccessful execution of the program\n");
            exit(-1);
        }
        else
        {
            char *sys_com = get_command(flags);
            execl("/bin/bash", "/bin/bash", "-c", sys_com, (char *)NULL);
        }
    }
    else
    {
        fflush(stdin);
        int wstatus;
        wait(&wstatus);
    }
}

int execent_cmp(const void *_first, const void *_second)
{
    execent *first = (execent *)_first;
    execent *second = (execent *)_second;
    return first->sec - second->sec;
}

void execent_dump(execent_ar *array)
{
    for (int i = 0; i < array->size; i++)
    {
        printf("TIME: %d\tCOMMAND: %s\n", array->execent_i[i].sec, array->execent_i[i].com);
    }
}

execent_ar *lexer_primitive(const char *file_name)
{
    int file_des = file_opening(file_name);

    size_t file_sz = file_size(file_des);
    char *buffer = (char *)calloc(file_sz, sizeof(char));

    if (read(file_des, buffer, file_sz) != file_sz)
    {
        printf("File size mismatch\n");
        exit(-1);
    }

    file_closing(file_des);

    execent_ar *exe_array = (execent_ar *)calloc(1, sizeof(execent_ar));
    exe_array->execent_i = (execent *)calloc(COM_NUMB, sizeof(execent));
    int ipos = 0;

    for (int pos = 0; buffer[pos] != '\0'; ++pos)
    {
        if (isspace(buffer[pos]))
            continue;
        if (isdigit(buffer[pos]))
        {
            int sec = get_sec(buffer, &pos);
            exe_array->execent_i[ipos].sec = sec;

            passspace(buffer, &pos);
            if (buffer[pos] != '\n')
            {
                exe_array->execent_i[ipos].com = command_init(buffer, &pos);
                exe_array->execent_i[ipos].flags = flags_init(buffer, &pos, exe_array->execent_i[ipos].com);
                ++ipos;
            }
            else
            {
                printf("No execution file name\n");
                exit(-1);
            }
        }
    }

    exe_array->size = ipos;
    qsort(exe_array->execent_i, exe_array->size, sizeof(execent), execent_cmp);
    execent_dump(exe_array);

    return exe_array;
}

int lexer_delete(execent_ar *array)
{
    int max = array->size;
    for (int i = 0; i < max; ++i)
    {
        free_flags(array->execent_i[i].flags);
    }
    free(array);
}

int execution(const char *file_name)
{
    execent_ar *array = lexer_primitive(file_name);

    clock_t current;
    current = clock() / CLOCKS_PER_SEC;
    int high_time = array->execent_i[array->size - 1].sec + 1;

    for (int pos = 0; (int)current < high_time;)
    {
        current = (int)(clock() / CLOCKS_PER_SEC);
        if (array->execent_i[pos].sec == (int)current)
        {
            execute_func(array->execent_i[pos].sec,
                         array->execent_i[pos].com,
                         array->execent_i[pos].flags);
            ++pos;
        }
    }

    lexer_delete(array);
}