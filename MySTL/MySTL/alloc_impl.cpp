#include <cstdlib>

#include "./Declaration/iterator.h"
#include "./Declaration/alloc.h"

namespace MySTL
{
	char* alloc::start_free = 0;
	char* alloc::end_free = 0;
	alloc::size_type alloc::heap_size = 0;
	alloc::obj* alloc::free_list[N_FREE_LISTS] = { 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0 }; // N_FREE_LISTS = 16


	// no need to specify static feature
	void* alloc::allocate(alloc::size_type bytes)
	{
		if (bytes > (size_t)alloc::MAX_BYTES) // if block size > 128 bytes
			return malloc(bytes);
		
		size_t index = FREE_LIST_INDEX(bytes);
		alloc::obj *list = free_list[index]; // choose an appropriate node from the free_list
		if (list == nullptr)	// if we didn't find any available node
		{
			void *r = refill(ROUND_UP(bytes)); // refill free_list
			return r;
		}
		else // if there's available node in free_list
		{
			free_list[index] = list->next; // remove this block (list) from the free_list
			return list;	// and return to construct object.
		}
	}


	void alloc::deallocate(void *p, alloc::size_type bytes)
	{
		if (bytes > (size_t)MAX_BYTES)
			free(p);
		else
		{
			/*
			                               index
										    |
			free_list[16]: | #0 | #1 | #2 | #3 | ... | #13 | #14 | #15 |
			               | 8  | 16 | 24 | 32 | ... | 112 | 120 | 128 |
						                    |
										   bytes
			*/
			size_t index = FREE_LIST_INDEX(bytes);
			// insert @q in the head of suitable node list in the @free_list,
			// same with the insertion operation of single link list
			obj *q = (obj *)p;
			q->next = free_list[index];
			free_list[index] = q;
		}
	}


	void* alloc::reallocate(void *p, size_type old_size, size_type new_size)
	{
		alloc::deallocate(p, old_size);
		p = alloc::allocate(new_size);
		return p;
	}


	// 
	void* alloc::refill(size_type bytes)
	{

	}


	// allocate a space that contains @nOBJs blocks with the size of @bytes
	//    @nOBJs might be reduced in different situations.
	// @bytes: the bytes of a block
	// @nOBJs: number of blocks
	char* alloc::chunk_alloc(size_type bytes, int &nOBJs)
	{

	}
}

