#include <stdio.h>
#include "stdlib.h"
#include <unistd.h>
#include <string.h>

void print_memory_contents(void *ptr, size_t size)
{
    printf("Memory contents:");
    for (size_t i = 0; i < size; i++)
    {
        printf(" %02X", *((unsigned char *)ptr + i));
    }
    printf("\n");
}

int main()
{
    // Test Case 1: Allocate and free memory
    printf("Test Case 1: Allocate and free memory\n");
    void *ptr1 = malloc(10);

    if (ptr1 == NULL)
    {
        printf("Error: malloc failed to allocate memory.\n");
        return 1;
    }

    printf("Allocated 10 bytes of memory.\n");
    print_memory_contents(ptr1, 10);

    free(ptr1);
    printf("Freed the allocated memory.\n");

    // Test Case 2: Allocate and access memory
    printf("\nTest Case 2: Allocate and access memory\n");
    void *ptr2 = malloc(20);

    if (ptr2 == NULL)
    {
        printf("Error: malloc failed to allocate memory.\n");
        return 1;
    }

    printf("Allocated 20 bytes of memory.\n");
    print_memory_contents(ptr2, 20);

    strcpy((char *)ptr2, "Hello, World!");
    printf("Copied 'Hello, World!' to the allocated memory.\n");
    printf("Contents of allocated memory: %s\n", (char *)ptr2);

    free(ptr2);
    printf("Freed the allocated memory.\n");

    // Test Case 3: Allocate more memory than available
    printf("\nTest Case 3: Allocate more memory than available\n");
    void *ptr3 = malloc(1000000000); // Allocating 1 billion bytes

    if (ptr3 != NULL)
    {
        printf("Error: malloc allocated more memory than available.\n");
        return 1;
    }

    printf("Test passed: malloc did not allocate more memory than available.\n");

    return 0;
}
