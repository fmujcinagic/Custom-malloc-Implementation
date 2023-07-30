#include <stdio.h>
#include "stdlib.h"
#include <unistd.h>

extern size_t free_block_info(int type);

int main()
{
    /*SPLITTING: lets try to split a large block into 7 pieces*/
    void *start = sbrk(0);
    void *large = malloc(300ULL);
    free(large);

    void *allocate100 = malloc(100);
    void *allocate50v1 = malloc(50);
    void *allocate50v2 = malloc(50);
    void *allocate40 = malloc(40);
    void *allocate30 = malloc(30);
    void *allocate20 = malloc(20);
    void *allocate10 = malloc(10);
    free(allocate100);
    size_t first_free = free_block_info(1);
    free(allocate50v1);
    size_t second_free = free_block_info(1);
    free(allocate50v2);
    size_t third_free = free_block_info(1);
    free(allocate40);
    size_t fourth_free = free_block_info(1);
    free(allocate30);
    size_t fifth_free = free_block_info(1);
    free(allocate20);
    size_t sixth_free = free_block_info(1);
    free(allocate10);
    size_t seventh_free = free_block_info(1);
    void *end = sbrk(0);

    printf("First free block after 100 ULL free: %lu\n", first_free);
    printf("First free block after 50 ULL free(first time): %lu\n", second_free);
    printf("First free block after 50 ULL free(second time): %lu\n", third_free);
    printf("First free block after 40 ULL free: %lu\n", fourth_free);
    printf("First free block after 30 ULL free: %lu\n", fifth_free);
    printf("First free block after 20 ULL free: %lu\n", sixth_free);
    printf("First free block after 10 ULL free: %lu\n", seventh_free);

    printf("start: %p --- end: %p\n", start, end);
    return 0;
}
