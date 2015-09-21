#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <initializer_list>
#include <cstddef>

/*
Reference:
C++ Primer (5th Edition)
http://www.cplusplus.com/reference/vector/vector/

| 0 | 1 | 2 | 3 |   |   |   |   |   |   |
^               ^                       ^
elements        first_free              cap
*/


template <typename T>
class vector
{
public:
	typedef std::size_t         size_type;
	typedef T                   value_type;
	typedef T&                  reference;
	typedef const T&            const_reference;
	typedef T*                  iterator;
	typedef const T*            const_iterator;
	typedef std::ptrdiff_t		difference_type;

public:
	vector() : elements(nullptr), first_free(nullptr), cap(nullptr) {}//constructor: default
	vector(const vector &);										    // constructor: copy
	vector(vector &&); //noexcept;									// constructor: move 
	explicit vector(const std::size_t n);							// constructor: fill
	vector(const std::size_t n, const T &val);						// constructor: fill
	vector(std::initializer_list<T> il);							// constructor: initializer_list
	template<typename InputIterator>
	vector(InputIterator first, InputIterator second);				// constructor: range

	vector& operator=(const vector &rhs);							// assign content: copy
	vector& operator=(std::initializer_list<T> il);					// assign content: initializer list
	vector& operator=(vector &&rhs); //noexcept;					// assign content: move
	~vector();                                                      // destructor

	// Element Access
	T& operator[](std::size_t n) { return *(elements + n); }        // access element
	T& front() { return *(begin()); }                               // access first element
	T& back() { return *(end() - 1); }                              // access last element
	T& at(std::size_t n);                                           // access element
	T* data()  { return elements; }							// access data

	// Modifiers
	void assign(std::size_t n, const T&);							// assign vector content: fill
	void assign(std::initializer_list<T> il);							// assign vector content: initializer list
	void push_back(const T&);										// add element at the end
	void pop_back();												// delete last element

	T* insert(const T* position, const T &val);					// insert elements: single element
	T* insert(const T* position, std::size_t n, const T &val);	// insert elements: fill
	template<typename InputIterator>
	T* insert(const T* position, InputIterator first, InputIterator second);// insert elements: range
	T* insert(const T* position, T &&val);						// insert elements: move
	T* insert(const T* position, std::initializer_list<T> il);		// insert elements: initializer list
	void clear(); //noexcept;										// clear content
	void swap(vector &x);										// swap content
	T* erase(const T* position);                                // erase elements: single element
	T* erase(const T* first, const T* second);					// erase elements: range
	template<typename... Args>
	void emplace(const T* position, Args&&... args);            // construct and insert element
	template<typename... Args>
	void emplace_back(Args&&... args);							// construct and insert element at the end

	// Capacity
	std::size_t size() const { return first_free - elements; }	// return size
	//std::size_t max_size() const noexcept;					// return maximum size
	void resize(std::size_t n, const T &val);					// change size
	std::size_t capacity() const { return cap - elements; }		// return size of allocated storage capacity
	bool empty() const { return elements == first_free; }		// test whether vector is empty
	void reverse(std::size_t n);								// request a change in capacity
	void shrink_to_fit();										// shrink to fit

	// Iterators
	T* begin() const { return elements; }       // return iterator to begining
	T* end() const { return first_free; }       // return iterator to end
	T* rbegin() const;                          // return reverse iterator to reverse begining
	T* rend() const;                            // return reverse iterator to reverse end
	const T* cbegin() const { return elements; }// return const_iterator to begining
	const T* cend() const { return first_free; }// return const_iterator to end	
	const T* crbegin() const;                   // return const_reverse_iterator to reverse beigining
	const T* crend() const;                     // return const_reverse_iterator to reverse end


private:
	void chk_n_alloc()
	{
		if (size() == capacity())
			reallocate();
	}


	std::pair<T*, T*> alloc_n_copy(const T*, const T*);
	void free();
	void reallocate();

	static std::allocator<T> alloc;
	T *elements;      // head pointer
	T *first_free;    // the pointer that point to the first free element in the array
	T *cap;           // tail pointer, end of storage
};


#endif // _VECTOR_H_