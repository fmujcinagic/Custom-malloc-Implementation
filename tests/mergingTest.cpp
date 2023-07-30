#include "stdio.h"
#include "../memory.h"
#include "stdlib.h"
#include <unistd.h>

int main()
{
    snp::Memory mem;
    void *request10 = malloc(10ULL);
    void *request20 = malloc(20ULL);
    void *request30 = malloc(30ULL);
    void *request40 = malloc(40ULL);

    /*Here we are going to merge 4 blocks from the above,
      including their metadata sizes.*/

    free(request10);
    free(request20);
    free(request30);
    free(request40);
    void *first_free_block = (void *)mem.free_block_info(0);
    void *request100 = malloc(100ULL);
    /*We can check if the first free block and our new request of 100 have the same address*/
    /*Also 10+20+30+40 = 100, which would suite one block of 100*/
    /*Also 'request10' has the same address as 'request100', which means
    'request100' has successfully taken place in the free block that we got after merging*/
    printf("%p %p %p\n", first_free_block, request100, request10);
    return 0;
}
