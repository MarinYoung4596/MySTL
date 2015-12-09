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


		static void construct(pointer p) { new (p) T(); }
		static void construct(pointer p, const_reference v) { new (p) T(v); }

		static void destory(pointer p) { p->~T(); }

		static void destory(pointer first, iterator last)
		{
			for (; first != last; ++first)
				first->~T();
		}

		
		pointer address(reference x) { return static_cast<pointer>(&x); }
		const_pointer address(const_reference x) { return static_cast<const_pointer>(&x); }

		size_type max_size() const { return max(size_type(1), size_type(UINT_MAX / sizeof(value_type))); }

	};
}
#endif