#ifndef INCLUDED_TEST_ALLOCATOR
#define INCLUDED_TEST_ALLOCATOR


#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>

#include "../Declaration/allocator.h"

using namespace MySTL;

namespace MySTL
{
	namespace TestAllocator
	{
		inline void tc_allocator()
		{
			std::cout << "----------test allocator----------" << std::endl;
			const unsigned int num = 10000;
			std::vector<int, MySTL::allocator<int> > vv;

			srand(static_cast<unsigned>(time(nullptr)));
			for (auto i = 0; i < num; ++i)
				vv.push_back(rand());
			std::cout << vv.size() << std::endl;

			for (auto i = 0; i < num / 3; ++i)
				vv.pop_back();
			std::cout << vv.size() << std::endl;

			vv.resize(10 * num);
			std::cout << vv.size() << std::endl;
			std::cout << vv.capacity() << std::endl;

			vv.clear();
			std::cout << vv.size() << std::endl;
			std::cout << "----------test allocator success----------\n" << std::endl;
		}
	}
}
#endif