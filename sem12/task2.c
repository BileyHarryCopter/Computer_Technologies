#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#define length 10000
#define iters 100

struct leng_sq
{
    int number;
    int square;
};

int main()
{
    int fd = 0;
    if ((fd = open("mapped_file_number", O_RDWR | O_CREAT, 0666)) == 0)
    {
        printf("Can't open the mapped file\n");
        return 0;
    }

    ftruncate(fd, length);

    struct leng_sq *ptr = (struct leng_sq *)mmap(NULL, length, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

    close(fd);

    struct leng_sq *tmpptr = ptr;
    int ileng = sizeof(struct leng_sq);

    for (int i = 0; i < iters; ++i)
    {
        tmpptr->number = i;
        tmpptr->square = i * i;
        tmpptr += 1;
    }

    munmap(ptr, length);

    return 0;
}