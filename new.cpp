#include <new>
#include <stdlib.h>
#include "memory.h"

namespace snp
{

  // TODO Student
  void *Memory::_new(size_t size)
  {
    void *result = malloc(size);
    if (result == nullptr)
    {
      throw std::bad_alloc();
    }
    return result;
  }

  // TODO Student
  void Memory::_delete(void *p)
  {
    try
    {
      t_block block_to_be_freed = getBlockForSpecificPtr(p);
      if (block_to_be_freed->key == 12127517)
      {
        if (block_to_be_freed->free == false)
        {
          free(p);
        }
        else
        {
          throw("free(): double free detected in tcache 2");
        }
      }
      else
      {
        throw("free(): invalid pointer");
      }
    }
    catch (const std::exception &e)
    {
      std::exit(-1);
    }
  }

}
