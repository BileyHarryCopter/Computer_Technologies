#include <stdio.h>
#include <pthread.h>
#include <errno.h>

int global = 0;

void* start_routine(void*)
{
    pthread_t thd_extrn = pthread_self();
    global++;
    printf("Global: %d\tId of extrn_thd: %ld\n", global, thd_extrn);
}

int main (int argc, char** argv)
{
    pthread_t main_thd = pthread_self();

    pthread_t thd;
    int res;
    if ((res = pthread_create(&thd, (pthread_attr_t *)NULL, start_routine, NULL)) != 0)
        printf("Error here!\n");

    global++;
    printf("Global: %d\tId of main_thd: %ld\n", global, main_thd);

    pthread_join(thd, (void **)NULL);

    return 0;
}