#include <stdio.h>
#include "stdlib.h"
#include <unistd.h>

extern size_t free_called_count();
extern size_t free_block_info(int type);

int main()
{
    void *program_break_before = sbrk(0);
    void *large = malloc(100ULL);
    void *large2 = malloc(100ULL);
    void *large3 = malloc(100ULL);
    free(large);
    free(large2);
    free(large3);
    void *program_break_after = sbrk(0);
    // printf("First free block: %lu\n", free_block_info(1));
    printf("Program Break before %p and, after malloc: %p\n", program_break_before, program_break_after);
    return 0;
}
