#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SIZE_FILE 10000

struct arg_t
{
    char *file_name;
    char *content;
};

void * interaction(void * func_arg)
{
    int file_dr = open(((struct arg_t *)func_arg)->file_name, O_RDONLY);
    if (file_dr < 0)
    {
        printf("Failure with opening a file\n");
        exit(-1);
    }

    char *buff_check = (char *)calloc(SIZE_FILE, sizeof(char));
    if (read(file_dr, buff_check, SIZE_FILE) < 0)
    {
        printf("Failure with reading a file\n");
        free(buff_check);
        exit(-1);
    }

    close(file_dr);
    ((struct arg_t *)func_arg)->content = buff_check;
}

void * reading (void* func_arg)
{
    FILE *file = fopen(((struct arg_t *)func_arg)->file_name, "r");
    fseek(file, 0L, SEEK_END);
    size_t size_file = ftell(file);
    rewind(file);

    char *buff_check = (char *)calloc(size_file + 1, sizeof(char));
    if (fread(buff_check, sizeof(char), size_file, file) != size_file)
    {
        printf("Failure with reading a file\n");
        free(buff_check);
        return (void *)NULL;
    }
    fclose(file);
    ((struct arg_t *)func_arg)->content = buff_check;
}

int main (int argc, char** argv)
{
    pthread_t main_thd = pthread_self();
    struct arg_t arg = {argv[1]};

    pthread_t thd;
    if (pthread_create(&thd, (pthread_attr_t *)NULL, interaction, &arg) != 0)
        printf("Error here!\n");

    pthread_join(thd, (void **)NULL);

    if (arg.content == NULL)
        printf("Content is not exist\n");

    printf("%s", arg.content);
    free(arg.content);
    return 0;
}
