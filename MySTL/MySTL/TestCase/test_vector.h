#ifndef INCLUDED_TEST_VECTOR_H
#define INCLUDED_TEST_VECTOR_H

#include "../Declaration/vector.h"

namespace MySTL
{
	namespace TestVector
	{
		// function declaration
		void tc_constructor_1();
		void tc_assignment();
		void tc_size();
		void tc_empty();
		void tc_resize();
		void tc_reserve();
		void tc_shrink_to_fit();
		void tc_elementAccess();
		void tc_at();
		void tc_front_back();
		void tc_data();
		void tc_assign();
		void tc_push_back();
		void tc_pop_back();
		void tc_insert();
		void tc_erase();
		void tc_swap();
		void tc_clear();
		void tc_emplace();
		void tc_emplace_back();
		//void tc_get_allocator();
		//void tc_relationalOperators();
		
		void test_all();
	}
}
#endif
