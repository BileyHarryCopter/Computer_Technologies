#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>

const enum {
    MAX_LENGTH = 100
};

DIR *dir_opening(const char *dir_path)
{
    DIR *dir = opendir(dir_path);
    if (dir == NULL)
    {
        printf("Error with opening curent directory\n");
        exit(-1);
    }
    return dir;
}

char *getpath(const char *parent_dir_name, const char *dir_name)
{
    char *path = (char *)calloc(MAX_LENGTH, sizeof(char));
    strcat(path, parent_dir_name);
    strcat(path, dir_name);
    strcat(path, "/");
    return path;
}

void dumpfiles(const char *dir_name)
{
    DIR *dir = dir_opening(dir_name);
    struct dirent *entity_dir = readdir(dir);
    if (entity_dir == NULL)
        return;

    while (entity_dir != NULL)
    {
        printf("%s%s\n", dir_name, entity_dir->d_name);
        if ((entity_dir->d_type == DT_DIR) && strcmp(entity_dir->d_name, ".") && strcmp(entity_dir->d_name, ".."))
        {
            char *path = getpath(dir_name, entity_dir->d_name);
            dumpfiles(path);
            free(path);
        }
        entity_dir = readdir(dir);
    }

    closedir(dir);
}

void reserve_copy(const char *source_name, const char *dest_name)
{
    DIR *source = dir_opening(source_name);
    DIR *dest = dir_opening(dest_name);

    for (struct dirent *current_source = readdir(source); current_source != NULL; current_source = readdir(source))
    {
    }
}

int main(int argc, char *argv[])
{
    dumpfiles(argv[1]);
    return 0;
}