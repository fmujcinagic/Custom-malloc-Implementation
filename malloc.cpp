#include <pthread.h>
#include "memory.h"
#include <new>
#include "string.h"

namespace snp
{
  void *start_global = nullptr;
  size_t malloc_count = 0;
  size_t free_count = 0;
  size_t size_of_the_extended_heap = 0;
  pthread_mutex_t malloc_lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t global_var_lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t calloc_lock = PTHREAD_MUTEX_INITIALIZER;

  size_t Memory::getBlockSize() { return sizeof(struct MetaData); }

  Memory::t_block Memory::findBlock(t_block *last, size_t size)
  {
    t_block block = (MetaData *)(start_global);
    while (block)
    {
      if (!(block->free && block->size >= size))
      {
        *last = block;
        block = block->next_chunk;
      }
      else
        break;
    }
    return block;
  }

  Memory::t_block Memory::extendHeap(t_block last, size_t size)
  {
    t_block current_program_break = (MetaData *)(snp::sbrk(0));
    long int request = (long int)snp::sbrk(size + getBlockSize());
    if (request < 0)
      return nullptr;

    size_of_the_extended_heap += (size + getBlockSize());

    current_program_break->previous_chunk = last;

    current_program_break->size = size;
    current_program_break->next_chunk = nullptr;
    current_program_break->free = false;
    current_program_break->key = 12127517;

    if (last != nullptr)
    {
      last->next_chunk = current_program_break;
    }
    return current_program_break;
  }

  Memory::t_block Memory::getBlockForSpecificPtr(void *given_ptr)
  {
    return (MetaData *)(given_ptr)-1;
  }

  void *Memory::malloc(size_t size)
  {
    pthread_mutex_lock(&malloc_lock);
    t_block block;
    if ((int)size < 0) // there was a <= condition, but its now set to return null only for non-positive numbers
    {
      pthread_mutex_unlock(&malloc_lock);
      return nullptr;
    }
    if (start_global == nullptr) // this applies only to the first iteration
    {
      block = extendHeap(nullptr, size);
      if (block == nullptr)
      {
        pthread_mutex_unlock(&malloc_lock);
        return nullptr;
      }
      start_global = block;
    }
    else
    {
      struct MetaData *last = (MetaData *)(start_global);
      block = findBlock(&last, size);
      if (!block)
      {
        block = extendHeap(last, size);
        if (block == nullptr)
        {
          pthread_mutex_unlock(&malloc_lock);
          return nullptr;
        }
      }
      else
      {
        long int try_this_size_for_split = (block->size - size - getBlockSize());
        if (try_this_size_for_split >= (long int)(getBlockSize()))
        {
          splitTheBlock(block, size);
        }
        block->free = false;
        block->key = 12127517;
      }
    }
    incrementMallocCount(); // increment of the counter that counts malloc() calls
    pthread_mutex_unlock(&malloc_lock);
    return (block + 1);
  }

  void Memory::splitTheBlock(t_block block, size_t user_requested_size)
  {
    t_block second_part = (t_block)((char *)block + user_requested_size + getBlockSize());

    second_part->size = block->size - user_requested_size - getBlockSize();
    second_part->next_chunk = block->next_chunk;
    block->size = user_requested_size;
    second_part->free = true;
    second_part->previous_chunk = block;
    second_part->key = 0;

    block->next_chunk = second_part;

    if (second_part->next_chunk != nullptr)
      second_part->next_chunk->previous_chunk = second_part;
  }

  Memory::t_block Memory::freeChunksMerging(t_block block_to_be_merged)
  {
    block_to_be_merged->size += block_to_be_merged->next_chunk->size + getBlockSize();
    block_to_be_merged->next_chunk = block_to_be_merged->next_chunk->next_chunk;
    if (block_to_be_merged->next_chunk != nullptr)
      block_to_be_merged->next_chunk->previous_chunk = block_to_be_merged;

    return block_to_be_merged;
  }

  void Memory::free(void *ptr)
  {
    pthread_mutex_lock(&malloc_lock);

    if (!ptr)
    {
      pthread_mutex_unlock(&malloc_lock);
      return;
    }

    t_block block_to_be_freed = getBlockForSpecificPtr(ptr);
    if (block_to_be_freed->free == false)
    {
      if (block_to_be_freed->key == 12127517)
      {
        block_to_be_freed->free = true;
        block_to_be_freed->key = 0;
        incrementFreeCount();

        if (block_to_be_freed->previous_chunk != nullptr)
          if (block_to_be_freed->previous_chunk->free == true)
            if (block_to_be_freed->previous_chunk->next_chunk != nullptr)
              if (block_to_be_freed->previous_chunk->next_chunk->free == true)
                block_to_be_freed = freeChunksMerging(block_to_be_freed->previous_chunk);

        if (block_to_be_freed->next_chunk != nullptr)
          if (block_to_be_freed->next_chunk->free == true)
            freeChunksMerging(block_to_be_freed);

        if (getMallocCount() == free_called_count())
        {
          sbrk(-getSizeOfTheExtendedHeap());
          start_global = nullptr;
          size_of_the_extended_heap = 0;
        }
        pthread_mutex_unlock(&malloc_lock);
      }
      else
      {
        pthread_mutex_unlock(&malloc_lock);
        exit(-1);
      }
    }
    else
    {
      pthread_mutex_unlock(&malloc_lock);
      exit(-1);
    }
  }

  void *Memory::calloc(size_t num_memb, size_t size_each)
  {
    pthread_mutex_lock(&calloc_lock);
    void *mem_block = malloc(num_memb * size_each);
    if (mem_block != NULL)
    {
      size_t block_sum = num_memb * size_each;
      char *char_mem_block = (char *)mem_block; // because we are using pointer arithmetic in c++
      for (size_t i = 0; i < block_sum; i++)
      {
        char_mem_block[i] = 0;
      }
      pthread_mutex_unlock(&calloc_lock);
      return mem_block;
    }
    else
    {
      pthread_mutex_unlock(&calloc_lock);
      return NULL;
    }
  }

  void Memory::incrementFreeCount() { free_count++; }
  void Memory::incrementMallocCount() { malloc_count++; }

  size_t Memory::getMallocCount()
  {
    return malloc_count;
  }

  size_t Memory::getSizeOfTheExtendedHeap() { return size_of_the_extended_heap; }

  size_t Memory::free_called_count() noexcept
  {
    return free_count;
  }

  size_t Memory::free_block_info(int type) noexcept
  {
    pthread_mutex_lock(&global_var_lock); // added locks to secure 'start_global', shared resource
    t_block block = (MetaData *)(start_global);
    while (block != nullptr)
    {
      if (block->free == true)
      {
        if (type == 0)
        {
          pthread_mutex_unlock(&global_var_lock);
          return (size_t)(block + 1);
        }
        else if (type != 0)
        {
          pthread_mutex_unlock(&global_var_lock);
          return block->size;
        }
      }
      block = block->next_chunk;
    }
    pthread_mutex_unlock(&global_var_lock);
    return 0;
  }

}
