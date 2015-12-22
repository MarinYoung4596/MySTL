#MySTL
MySTL is a Tiny C++ STL library.

#TO-DO LIST
 1. type_traits .............................................................................. [ DONE ]
 2. iterator, reverse_iterator ................................................. [ DONE ]
 3. allocater ................................................................................. [ DONE ]
 4. uninitialized functions, construct / destroy .............. [ DONE ]
 4. vector ...................................................................................... [ DONE ]
 5. string ....................................................................................... [ ...        ]
 6. dequeue, stack, queue ...................................................... [ ...        ]
 7. ...

#TIPS

#Bugs & Solutions

 - Use customized reverse_iterator
 - In conjunction with customized uninitialized functions
 - vector
	 - [compile error] emplace & emplace_back
	 - [logical error] shrink_to_fit
	 - [compile error] get_allocator
	 - [compile error] rbegin, rend
	 - [logical error] insert_aux
	 - [link error] operational functions
 - ERROR C2664: 'MySTL::allocator<_Newfirst>::allocator(MySTL::allocator<_Newfirst> &&)': cannot convert argument 1 from 'std::_Wrap_alloc<MySTL::allocator<int>>' to 'const MySTL::allocator<_Newfirst> &'      xmemory0    874
	 - Solution:  use vs2013
 - ERROR C4996: 'std::_Uninitialized_copy0': Function call with parameters that may be unsafe...
	 - Solution: Create project without Security Development Lifecycle (SDL) checks
 - ERROR C2672, 'std::Iter_cat': no matching overloaded function found
 - ERROR C2893, C2780, C2665 ...
	 - Solutions: 
    vector(size_type n, const_reference val) {}
    template<typename InputIterator>
    vector(InputIterator first, InputIterator second) {}

	==> use traits

    vector(size_type n, const_reference val)
    {
    using IS_INTEGER = typename std::is_integral<size_type>::type;
    _vector_aux(n, val, IS_INTEGER());
    }




