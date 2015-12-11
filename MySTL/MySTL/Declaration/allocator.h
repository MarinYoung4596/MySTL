#ifndef INCLUDED_ALLOCATOR
#define INCLUDED_ALLOCATOR

#define NOMINMAX

#include <cstddef>	// ptrdiff_t
#include <new>	// placement new
#include <algorithm>	// std::max
#include "alloc.h"

namespace MySTL
{
	template <typename T>
	class allocator
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;


	public:
		static pointer allocate()
		{
			return static_cast<pointer>(alloc::allocate(sizeof(value_type)));
		}

		static pointer allocate(size_type n)
		{
			return  static_cast<pointer>(alloc::allocate(n * sizeof(value_type)));
		}
		 
		static void deallocate(pointer p)
		{
			alloc::deallocate(static_cast<void *>(p), sizeof(value_type));
		}

		static void deallocate(pointer p, size_type n)
		{
			if (!n) return;
			alloc::deallocate(static_cast<void*>(p), n * sizeof(value_type) );
		}


		static void construct(pointer p) { new (p) T(); }
		static void construct(pointer p, const_reference v) { new (p) T(v); }

		static void destroy(pointer p) { p->~T(); }
		static void destroy(pointer first, pointer last)
		{
			for (; first != last; ++first)
				first->~T();
		}

		
		pointer address(reference x) { return static_cast<pointer>(&x); }
		const_pointer address(const_reference x) { return static_cast<const_pointer>(&x); }

		size_type max_size() const 
		{
			using std::max;
			return max(size_type(1), size_type(UINT_MAX / sizeof(value_type))); 
		}

	};
}
#endif