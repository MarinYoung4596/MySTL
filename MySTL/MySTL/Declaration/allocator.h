#ifndef INCLUDED_ALLOCATOR
#define INCLUDED_ALLOCATOR


#include <cstddef>
#include <new>	// placement new
#include "alloc.h"

namespace MySTL
{
	template <typename T>
	class allocator
	{
	public:
		typedef T				value_type;
		typedef T*				pointer;
		typedef const T*		const_pointer;
		typedef T*				iterator;
		typedef const T*		const_iterator;
		typedef T&				reference;
		typedef const T&		const_reference;
		typedef std::size_t		size_type;
		typedef std::ptrdiff_t	difference_type;


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


		static void construct(iterator p)
		{
			new (p) T();
		}

		static void construct(iterator p, const_reference v)
		{
			new (p) T(v);
		}

		static void destory(iterator p)
		{
			p->~T();
		}

		static void destory(iterator first, iterator last)
		{
			for (; first != last; ++first)
				first->~T();
		}
	};
}
#endif