#include "stdlib.h"
#include <unistd.h>
#include <iostream>

using namespace std;

class ComplexObject
{
public:
  int data;

  ComplexObject(int value) : data(value)
  {
    cout << "ComplexObject constructed. Data: " << data << endl;
  }

  ~ComplexObject()
  {
    cout << "ComplexObject destructed. Data: " << data << endl;
  }
};

void testNewDeleteOperands()
{
  // Test 1: Single object allocation and deallocation
  ComplexObject *obj1 = new ComplexObject(1);
  delete obj1;

  // Test 2: Array allocation and deallocation
  ComplexObject *objArr = new ComplexObject[3]{ComplexObject(2), ComplexObject(3), ComplexObject(4)};
  delete[] objArr;

  // Test 3: Allocation and deallocation of objects within loops
  for (int i = 0; i < 2; i++)
  {
    ComplexObject *obj = new ComplexObject(i);
    delete obj;
  }

  // Test 4: Allocations and deallocations in a nested loop
  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      ComplexObject *obj = new ComplexObject(i + j);
      delete obj;
    }
  }

  // Test 5: Allocation and deallocation of objects in different scopes
  {
    ComplexObject *obj1 = new ComplexObject(5);
    {
      ComplexObject *obj2 = new ComplexObject(6);
      delete obj2;
    }
    ComplexObject *obj3 = new ComplexObject(7);
    delete obj1;
    delete obj3;
  }
}

int main()
{
  // void *start = sbrk(0);
  // testNewDeleteOperands();
  // void *end = sbrk(0);
  // std::cout << start << " " << end << std::endl;
  int *ptr = new int(20);
  delete ptr;
  delete ptr;
  return 0;
}
