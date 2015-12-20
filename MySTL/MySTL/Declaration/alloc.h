#ifndef INCLUDED_ALLOC_H
#define INCLUDED_ALLOC_H

namespace MySTL
{
    class alloc
    {
    private:
        typedef size_t     size_type;

		// block increasing step length
        enum { ALIGN = 8 };
		// upper bound of block
        enum { MAX_BYTES = 128 };
		// number of free lists
        enum { N_FREE_LISTS = MAX_BYTES / ALIGN };
	
	public:
		// definition of node in the  free lists
        union obj
        {
            union obj *next;
			// see:
			// [1] http://blog.csdn.net/ce123/article/details/8973073
			// [2] http://blog.csdn.net/yby4769250/article/details/7294696
            char client[1];	// pointer 
		};

	private:
		// round up @bytes to a multiple of @ALIGN
        static size_t ROUND_UP(size_type bytes)
        { return ((bytes + ALIGN - 1) & ~(ALIGN - 1)); }

		// choose an appropriate node according to the value of @bytes
        static size_t FREE_LIST_INDEX(size_type bytes)
        { return ((bytes + ALIGN - 1) / ALIGN - 1); }

        static void* refill(size_type bytes);
        static char* chunk_alloc(size_type bytes, int &nOBJs);

	private:
		static char *start_free; // start position of memory pool
		static char *end_free;	// end position of memory pool
		static size_type heap_size;

		static obj* free_list[N_FREE_LISTS]; /* obj* */
        
    public:
        static void* allocate(size_type bytes);
        static void  deallocate(void *p, size_type n);
        static void* reallocate(void *p, size_type old_size, size_type new_size);
    };
}

#endif
