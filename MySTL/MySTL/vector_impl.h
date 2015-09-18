#include "vector.h"

namespace MySTL
{
	template <typename T>
	vector::vector(const vector &s)
	{
		auto newdata = alloc_n_copy(s.begin(), s.end());
		elements = newdata.first;
		first_free = cap = newdata.second;
	}


	template <typename T>
	vector::vector(vector &&s) noexcept : elements(s.elements), first_free(s.first_free), cap(s.cap)
	{
		s.elements = s.first_free = s.cap = nullptr;
	}


	template <typename T>
	explicit vector::vector(const::size_t n)
	{
		auto newdata = alloc.allocate(n);
		elements = newdata;
		first_free = cap = elements + n;
	}


	template <typename T>
	vector::vector(const std::size_t n, const T &val)
	{
		auto newdata = alloc.allocate(n);
		for (std::size_t i = 0; i < n;)
			alloc.construct(i++, val);
		elements = newdata;
		first_free = cap = elements + n;
	}


	template <typename T>
	vector::vector(initializer_list<T> il)
	{

	}


	template <typename InputIterator>
	vector::vector(InputIterator first, InputIterator second)
	{

	}


	template <typename T>
	vector& vector::operator=(const vector &rhs)
	{
		auto data = alloc_n_copy(rhs.begin(), rhs.end());
		free();
		elements = data.first;
		first_free = cap = data.second;
		return *this;
	}


	template <typename T>
	vector& vector::operator=(const vector &&rhs) noexcept
	{
		if (this != &rhs)
		{
			free();
			elements = rhs.elements;
			first_free = rhs.first_free;
			cap = rhs.cap;
			rhs.elements = rhs.first_free = rhs.cap = nullptr;
		}
		return *this;
	}


	template <typename T>
	vector& vector::operator=(initializer_list<T> il)
	{

	}


	template <typename T>
	vector::~vector()
	{
		free();
	}


	// Modifiers
	template <typename T>
	void vector::push_back(const T &s)
	{
		chk_n_alloc();
		alloc.construct(first_free++, s);
	}


	template <typename T>
	void vector::pop_back()
	{
		if (elements)
			alloc.destroy(--first_free);
	}


	template <typename T>
	void vector::clear()
	{
		if (elements)
		{
			for (auto p = first_free; p != elements;)
				alloc.destroy(--p);
		}
	}



	template <typename T>
	void vector::free()
	{
		if (elements)
		{
			// destory the object p in vector one by one,
			for (auto p = first_free; p != elements;)
				alloc.destroy(--p);
			// and free the space.
			alloc.deallocate(elements, cap - elements);
		}
	}


	template <typename T>
	std::pair<T*, T*>
		vector::alloc_n_copy(const T *first, const T *second)
	{
		auto data = alloc.allocate(second - first);
		return{ data, uninitialized_copy(first, second, data) };
	}


	template <typename T>
	void vector::reallocate()
	{
		auto newcapacity = size() ? 2 * size() : 1;
		auto newdata = alloc.allocate(mewcapacity);
		auto dest = newdata;
		auto elem = elements;
		for (std::size_t i = 0; i != size(); ++i)
		{
			alloc.construct(dest++, std::move(*elem++));
		}
		free();
		elements = newdata;
		first_free = dest;
		cap = elements + newcapacity;
	}
}