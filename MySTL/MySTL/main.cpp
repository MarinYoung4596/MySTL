#define  _SCL_SECURE_NO_WARNINGS
#define D_SCL_SECURE_NO_WARNINGS
//#define _ITERATOR_DEBUG_LEVEL 0
#pragma warning(disable:4996)


#include "TestCase/test_allocator.h"
#include "TestCase/test_vector.h"

using namespace MySTL;

int main()
{
	MySTL::TestAllocator::tc_allocator();
	MySTL::TestVector::test_all();


	system("pause");
	return 0;
}
