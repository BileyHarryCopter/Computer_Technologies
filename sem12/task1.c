#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#define length 10000
#define iters 100

int main()
{
    int fd = 0;
    if ((fd = open("mapped_file", O_RDWR | O_CREAT, 0666)) == 0)
    {
        printf("Can't open the mapped file\n");
        return 0;
    }

    ftruncate(fd, length);

    char *ptr = mmap(NULL, length, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

    close(fd);

    char *tmpptr = ptr;
    char text[] = "TEXT\n";
    int ilength = strlen(text) + 1;

    for (int i = 0; i < iters; ++i)
    {
        strcpy(tmpptr, text);
        tmpptr += ilength;
    }

    munmap(ptr, length);

    return 0;
}