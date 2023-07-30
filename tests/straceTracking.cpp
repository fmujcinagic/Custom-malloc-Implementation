#include "stdlib.h"

int main()
{
    void *ptr1 = malloc(50 * 1024ULL);
    void *ptr2 = calloc((50 * 1024ULL), sizeof(char));
    free(ptr1);
    free(ptr2);
}
