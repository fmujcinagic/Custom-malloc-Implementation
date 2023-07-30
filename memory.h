#ifndef SNP_MEMORY_H_
#define SNP_MEMORY_H_

#include "unistd.h"
#include "stdlib.h"

namespace snp
{
  class Memory
  {
  public:
    // DO NOT MODIFY
    static void *calloc(size_t num_memb, size_t size_each);
    static void *malloc(size_t size);
    static void free(void *ptr);

    // DO NOT MODIFY
    static void *_new(size_t size);
    static void _delete(void *ptr);
    static size_t free_called_count() noexcept;
    static size_t free_block_info(int type) noexcept;

    // Feel free to add new members to this class.
    // primary functions
    struct MetaData
    {
      size_t size;
      struct MetaData *next_chunk;
      struct MetaData *previous_chunk;
      bool free;
      int key;
    };
    typedef struct MetaData *t_block;
    static size_t getBlockSize();
    static t_block findBlock(t_block *last, size_t size);
    static t_block extendHeap(t_block last, size_t size);
    static t_block getBlockForSpecificPtr(void *given_ptr);

    // counters and info
    static void incrementMallocCount();
    static void incrementFreeCount();
    static size_t getMallocCount();
    static size_t getSizeOfTheExtendedHeap();

    // advancing malloc()
    static void splitTheBlock(t_block block, size_t user_requested_size);
    static t_block freeChunksMerging(t_block block_to_be_merged);
  };

  // DO NOT MODIFY
  void *sbrk(intptr_t __delta);
  int brk(void *__addr);
}

// DO NOT CHANGE ANYTHING BELOW THIS LINE
void *malloc(size_t size) noexcept;
void *calloc(size_t num_memb, size_t size_each) noexcept;
void free(void *ptr) noexcept;
void *operator new(size_t size);
void operator delete(void *address) noexcept;
void *operator new[](size_t size);
void operator delete[](void *address) noexcept;

#endif /* SNP_MEMORY_H_ */