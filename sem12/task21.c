#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

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
    if ((fd = open("mapped_file_number", O_RDONLY, 0666)) == 0)
    {
        printf("Can't open the mapped file\n");
        return 0;
    }

    // int leng = iters * sizeof(struct leng_sq);
    struct leng_sq *buff = (struct leng_sq *)calloc(iters + 1, sizeof(struct leng_sq));
    read(fd, buff, sizeof(buff) * iters);

    close(fd);

    struct leng_sq *tmpptr = buff;

    for (int i = 0; i < iters; ++i)
    {
        printf("Struct %d: %d %d\n", i, tmpptr->number, tmpptr->square);
        tmpptr++;
    }

    return 0;
}