#ifndef INCLUDED_VECTOR_H
#define INCLUDED_VECTOR_H

#include <initializer_list>
#include <cstddef>		// ptrdiff_t
#include <memory>		// allocator, uninitialized_copy, uninitialized_fill

#include "iterator.h"
#include "type_traits.h"
#include "alloc.h"
#include "allocator.h"


namespace MySTL
{

	template <typename T, typename Alloc = allocator<T>>
	class vector
	{
	public:
		using size_type = std::size_t;
		using value_type = T;
		using reference = value_type&;
		using const_reference = const value_type&;
		using iterator = value_type*;
		using const_iterator = const value_type*;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using reverse_iterator = std::reverse_iterator<T*>;
		using const_reverse_iterator = std::reverse_iterator<const T*>;
		using difference_type = std::ptrdiff_t;
		using allocator_type = allocator<T>;

	protected:
		using data_allocator = Alloc;

	public:
		vector() : elements_start(nullptr), first_free(nullptr), end_of_storage(nullptr) {}//constructor: default
		/*
		vector(const vector &);											// constructor: copy
		vector(vector &&) noexcept;										// constructor: move
		explicit vector(const size_type n);								// constructor: fill
		vector(const size_type n, const value_type &val);				// constructor: fill
		vector(std::initializer_list<value_type> il);					// constructor: initializer_list
		template<typename InputIterator>
		vector(InputIterator first, InputIterator second);				// constructor: range

		vector& operator=(const vector &rhs);							// assign content: copy
		vector& operator=(std::initializer_list<value_type> il);		// assign content: initializer list
		vector& operator=(vector &&rhs) noexcept;						// assign content: move
		*/
		~vector();														// destructor
		/*
		// Element Access
		reference operator[](size_type n) { return *(elements_start + n); }	// access element
		reference front() { return *(begin()); }						// access first element
		reference back() { return *(end() - 1); }						// access last element
		iterator  data() { return elements_start; }							// access data
		reference at(size_type n);										// access element

		// Modifiers
		void assign(size_type n, const value_type&);					// assign vector content: fill
		void assign(std::initializer_list<value_type> il);				// assign vector content: initializer list
		template <typename InputIterator>
		void assign(InputIterator first, InputIterator last);

		void push_back(const value_type&);								// add element at the end
		void pop_back();												// delete last element

		iterator insert(iterator position, const value_type &val);				// insert elements: single element
		iterator insert(iterator position, size_type n, const value_type &val);	// insert elements: fill
		template<typename InputIterator>
		iterator insert(iterator position, InputIterator first, InputIterator second);// insert elements: range
		iterator insert(iterator position, value_type &&val);					// insert elements: move
		iterator insert(iterator position, std::initializer_list<value_type> il);// insert elements: initializer list

		void clear() noexcept { erase(begin(), end()); } 				// clear content
		void swap(vector &x);											// swap content

		iterator erase(iterator position);								// erase elements: single element
		iterator erase(iterator first, iterator second);				// erase elements: range

		template<typename... Args>
		void emplace(const_iterator position, Args&&... args);			// construct and insert element
		template<typename... Args>
		void emplace_back(Args&&... args);								// construct and insert element at the end

		// Capacity
		size_type size() const { return first_free - elements_start; }		// return size
		static size_type max_size() noexcept { return size_type(UINT_MAX / sizeof(value_type)); }  // return maximum size
		void resize(size_type n, value_type val = value_type());		// change size
		size_type capacity() const { return end_of_storage - elements_start; }			// return size of allocated storage capacity
		bool empty() const { return elements_start == first_free; }			// test whether vector is empty
		void reserve(size_type n);										// request a change in capacity
		void shrink_to_fit();											// shrink to fit

		// Iterators
		iterator begin() const { return elements_start; }						// return iterator to begining
		iterator end() const { return first_free; }						// return iterator to end
		iterator rbegin() const { return reverse_iterator(elements_start); }	// return reverse iterator to reverse begining
		iterator rend() const { return reverse_iterator(first_free); }	// return reverse iterator to reverse end
		const_iterator cbegin() const { return elements_start; }				// return const_iterator to begining
		const_iterator cend() const { return first_free; }				// return const_iterator to end
		const_iterator crbegin() const { return const_reverse_iterator(elements_start); } // return const_reverse_iterator to reverse beigining
		const_iterator crend() const { return const_reverse_iterator(first_free); } // return const_reverse_iterator to reverse end

		// Allocator
		allocator_type get_allocator() const { return data_allocator; }
		*/

	private:
		void chk_n_alloc();

		template <typename InputIterator>
		std::pair<iterator, iterator> alloc_n_copy(InputIterator, InputIterator);

		void free();

		void reallocate();

		iterator elements_start;				// head pointer
		iterator first_free;		// the pointer that point to the first free element in the array
		iterator end_of_storage;	// tail pointer, end of storage

	public:
		// non-member functions overloads
		template <typename T, typename Alloc> // why?
		friend bool operator== (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);
	
		template <typename T, typename Alloc>
		friend bool operator!= (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);
	
		template <typename T, typename Alloc>
		friend bool operator<  (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);
	
		template <typename T, typename Alloc>
		friend bool operator<= (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);
	
		template <typename T, typename Alloc>
		friend bool operator>  (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);
	
		template <typename T, typename Alloc>
		friend bool operator>= (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

		template <typename T, typename Alloc>
		friend void swap(vector<T, Alloc>& x, vector<T, Alloc>& y);
	};
}

#include "../Implementation/vector_impl.h"

#endif // INCLUDED_VECTOR_H
