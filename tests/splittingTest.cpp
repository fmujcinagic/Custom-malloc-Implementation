#include "stdio.h"
#include "../memory.h"
#include "stdlib.h"
#include <unistd.h>

int main()
{
    snp::Memory mem;

    // lets allocate a large chunk of memory e.g. 200ULL
    void *large = malloc(200ULL);
    free(large);
    printf("This is first free block after deallocation, so : %lu\n", mem.free_block_info(1));

    // smaller allocation
    void *request_50 = malloc(50ULL);
    printf("This is first free block after 50ULL request, so : %lu\n", mem.free_block_info(1));
    printf("Result is 118, because 200 - 50(request) - 32(size of metadata(struct))\n");
    printf("---------------------------------------\n\n");

    void *request_20 = malloc(20ULL);
    printf("This is first free block after 20ULL request, so : %lu\n", mem.free_block_info(1));
    printf("Result is 66, because 118 - 20(request) - 32(size of metadata(struct))\n");
    printf("---------------------------------------\n\n");

    void *request_1 = calloc(1ULL, sizeof(char));
    printf("This is first free block after 1ULL request, so : %lu\n", mem.free_block_info(1));
    printf("Result is 33, because 66 - 1(request) - 32(size of metadata(struct))\n");
    printf("---------------------------------------\n\n");

    free(request_50);
    free(request_20);
    free(request_1);

    printf("After freeing all blocks: %lu\n", mem.free_block_info(1));
    return 0;
}
