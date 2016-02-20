#MySTL
MySTL is a Tiny C++ STL library.

#TO-DO LIST
 1. type_traits
 2. iterator, reverse_iterator
 3. allocater
 4. uninitialized functions, construct / destroy
 4. vector
 5. string
 6. dequeue, stack, queue
 7. algorithm
 8. ...

#TIPS
 * traits
 * `const T*` means `(const T)*`,
 
   `using pointer = T*;`
   `using const_pointer = const pointer;` // `const_pointer` means `const (T*)`
   
   use `T const *`
 * `const *` // **pointer to const**; Cann't change the object's value it pointed (by this pointer), while its object's value can be changed in another way, **we need `pointer to const` in STL**.
   
   `* const` // **const pointer**;    Once initialized, its storaged value (the address of the object) can not be changed. 
	


#BUGS & SOLUTIONS

 - **Use customized reverse_iterator**
 - **In conjunction with customized uninitialized functions**
 - **vector**
	 - [compile error] emplace & emplace_back
	 - [logical error] shrink_to_fit
	 - [compile error] get_allocator
	 - [compile error] rbegin, rend
	 - [logical error] insert_aux
	 - [link error] relational operations	[RESOLVED]
		- Solutions: friend functions declaration error
 - **ERROR C2664:** 'MySTL::allocator<_Newfirst>::allocator(MySTL::allocator<_Newfirst> &&)': cannot convert argument 1 from 'std::_Wrap_alloc<MySTL::allocator<int>>' to 'const MySTL::allocator<_Newfirst> &'      xmemory0    874
	 - Solution:  **use vs2013**
 - **WARINING C4996:** 'std::_Uninitialized_copy0': Function call with parameters that may be unsafe...
	 - Solution: Create project without Security Development Lifecycle (SDL) checks
 - **ERROR C2672, C2893, C2780, C2665 ...**
	 - Solutions: functions overload failed ==> use traits
 - **ERROR C2572:** 'MySTL::string::string': redefinition of default argument: parameter 1
	 - Solution: The defaults only belong in the declaration in the header file, not the implementation in the cpp file.
