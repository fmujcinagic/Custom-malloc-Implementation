#include <pthread.h>
#include "stdio.h"
#include "../memory.h"
#include "stdlib.h"
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *routine(void *args)
{
    pthread_mutex_lock(&lock);
    void *ptr = malloc(10ULL);
    printf("%p\n", ptr);
    // free(ptr);
    pthread_mutex_unlock(&lock);

    return args;
}

int main()
{
    size_t size = 10;
    pthread_t *tids = (pthread_t *)malloc(sizeof(pthread_t) * size);

    for (size_t i = 0; i < size; i++)
    {
        pthread_create(&tids[i], 0, (void *(*)(void *)) & routine, 0);
    }
    for (size_t i = 0; i < size; i++)
    {
        pthread_join(tids[i], 0);
    }
    free(tids);
    return 0;
}
