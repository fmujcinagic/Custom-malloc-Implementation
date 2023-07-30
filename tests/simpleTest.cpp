#include <stdio.h>
#include "stdlib.h"
#include <unistd.h>
#include "../memory.h"
int main()
{

       snp::Memory mem;

       for (int i = 0; i < 20; i++)
       {
              int *ptr1 = (int *)malloc(1);
              free(ptr1);

              int *ptr2 = (int *)malloc(1);
              free(ptr2);

              int *ptr3 = (int *)malloc(1);
              free(ptr3);
              // if (i == 19)
              // {
              //     printf("First address %p\n", ptr1);
              //     printf("First address %p\n", ptr2);
              //     printf("First address %p\n", ptr3);
              // }
       }
       printf("%lu\n", mem.free_called_count());
}