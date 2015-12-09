#include <cstdlib>

#include "../Declaration/iterator.h"
#include "../Declaration/alloc.h"

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
		else // if there's at least one node available in free_list
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
			alloc::obj *q = (alloc::obj *)p;
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


	// assume that @bytes is the multiple of 8.
	void* alloc::refill(size_type bytes)
	{
		int nobjs = 20; // the number of nodes increased every time.
		char* chunk = chunk_alloc(bytes, nobjs);
		if (1 == nobjs) // only one nodes available, return this block
			return chunk;

		alloc::obj* volatile *list;
		list = free_list + FREE_LIST_INDEX(bytes);
		alloc::obj *result, *next, *curr;
		result = (alloc::obj*)chunk;	// refill free_list in chunk

		*list = next = (alloc::obj*)(chunk + bytes); // let @list point to the new configured memory
		for (int i = 1; i < nobjs; ++i)	// loop start from 1, because the zeroth node would be returned to the user
		{
			curr = next;
			next = (alloc::obj*)((char*)next + bytes);
			curr->next = ((i == nobjs - 1) ? nullptr : next);
		}
		return result;
	}


	// allocate a space that contains @nOBJs blocks with the size of @bytes
	//    @nOBJs might be reduced in different situations.
	// @bytes: the bytes of a block��(assume that bytes is the multiple of 8)
	// @nOBJs: number of blocks
	char* alloc::chunk_alloc(size_type bytes, int &nOBJs)
	{
		char *result;
		size_type required_bytes = bytes * nOBJs;		// the required bytes in total
		size_type bytes_left = end_free - start_free;	// the left space in the memory pool

		if (bytes_left >= required_bytes)	// @bytes_left fulfills the requirement
		{
			result = start_free;
			start_free += required_bytes;
			return result;
		}
		else if (bytes_left >= bytes)	// @bytes_left can't entirely satisfy the requirement, but is sufficient for at least one blocks
		{
			nOBJs = bytes_left / bytes;
			required_bytes = bytes * nOBJs;
			result = start_free;
			start_free += required_bytes;
			return result;
		}
		else	// @bytes_left can't even provide free space for one block
		{
			size_type bytes_to_get = 2 * required_bytes + ROUND_UP(heap_size >> 4);
			if (bytes_left > 0)
			{
				alloc::obj* volatile *list = free_list + FREE_LIST_INDEX(bytes_left);
				((alloc::obj*)start_free)->next = *list;
				*list = (alloc::obj*)start_free;
			}

			start_free = (char*)malloc(bytes_to_get);
			if (nullptr == start_free)
			{
				alloc::obj* volatile *list, *p;
				for (int i = bytes; i <= MAX_BYTES; i += ALIGN)
				{
					list = free_list + FREE_LIST_INDEX(i);
					p = *list;
					if (p != nullptr)
					{
						*list = p->next;
						start_free = (char*)p;
						end_free = start_free + i;
						return chunk_alloc(bytes, nOBJs); // invoke itself recursively to correct @nOBJs
					}
				}
				end_free = nullptr;
				start_free = (char*)malloc(bytes_to_get);
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return chunk_alloc(bytes, nOBJs);  // invoke itself recursively to correct @nOBJs
		}
	} // end chunk_alloc

} // end namespace
