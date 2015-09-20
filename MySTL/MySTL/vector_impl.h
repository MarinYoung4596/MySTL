#ifndef _VECTOR_IMPL_H_
#define _VECTOR_IMPL_H_


#include <utility>	// for std::move & std::forward & std::pair
#include <memory>	// uninitialized_copy & uninitialized_fill

#include "vector.h"

namespace MySTL
{
	template <typename T>
	vector<T>::vector(const vector &s)
	{
		auto newdata = alloc_n_copy(s.begin(), s.end());
		elements = newdata.first;
		first_free = cap = newdata.second;
	}


	template <typename T>
	vector<T>::vector(vector &&s) //noexcept 
		: elements(s.elements), first_free(s.first_free), cap(s.cap)
	{
		s.elements = s.first_free = s.cap = nullptr;
	}


	template <typename T>
	explicit vector<T>::vector(const::size_t n)
	{
		auto newdata = alloc.allocate(n);
		elements = newdata;
		first_free = cap = elements + n;
	}


	template <typename T>
	vector<T>::vector(const std::size_t n, const T &val)
	{
		auto newdata = alloc.allocate(n);
		for (std::size_t i = 0; i < n;)
			alloc.construct(i++, val);
		elements = newdata;
		first_free = cap = elements + n;
	}


	template <typename T>
	vector<T>::vector(initializer_list<T> il)
	{
		auto data = alloc_n_copy(il.begin(), il.end());
		elements = data.first;
		first_free = cap = data.second;
	}


	template <typename InputIterator>
	vector<T>::vector(InputIterator first, InputIterator second)
	{

	}


	template <typename T>
	vector& vector<T>::operator=(const vector &rhs)
	{
		auto data = alloc_n_copy(rhs.begin(), rhs.end());
		free();
		elements = data.first;
		first_free = cap = data.second;
		return *this;
	}


	template <typename T>
	vector& vector<T>::operator=(const vector &&rhs) //noexcept
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
	vector& vector<T>::operator=(initializer_list<T> il)
	{
		auto data = alloc_n_copy(il.begin(), il.end());
		free();
		elements = data.first;
		first_free = cap = data.second;
		return *this;
	}


	template <typename T>
	vector<T>::~vector()
	{
		free();
	}


	// Elements Access
	template <typename T>
	T& vector<T>::at(std::size_t n)
	{
		if (n >= size())
		{
			throw std::out_of_range("out of range!");
		}
		return *(elements + n);
	}



	// Modifiers
	template <typename T>
	void vector<T>::push_back(const T &s)
	{
		chk_n_alloc();
		alloc.construct(first_free++, s);
	}


	template <typename T>
	void vector<T>::pop_back()
	{
		if (elements)
			alloc.destroy(--first_free);
	}


	template <typename T>
	T* vector<T>::insert(const T* position, const T &val)
	{
		MySTL::insert(position, 1, val);
	}


	template <typename T>
	T* vector<T>::insert(const T* position, std::size_t n, const T &val)
	{
		if (position > end())
			throw out_of_range("position out of range!");
		auto space_left = cap - first_free;
		if (space_left >= n)
		{
			for (auto curr = end() - 1 ; curr != position; --curr)
				*(curr + space_left) = *curr;
			uninitialized_fill_n(position, n, val);
			first_free += n;
		}
		else
		{
			T *_start = alloc.allocate(static_cast<std::size_t>(capacity()) + n);
			T *_end = uninitialized_copy(begin(), position, _start);
			_end = uninitialized_fill_n(_end, n, val); // _end point to next free space
			_end = uninitialized_copy(position, end(), _end);
			free();
			elements = _start;
			first_free = cap = _end;
		}
	}


	template <typename T, typename InputIterator>
	T* vector<T>::insert(const T* position, InputIterator first, InputIterator second)
	{

	}


	template <typename T>
	void vector<T>::clear() //noexcept
	{
		if (elements)
		{
			for (auto p = first_free; p != elements;)
				alloc.destroy(--p);
		}
	}


	template <typename T>
	void vector<T>::swap(vector &x)
	{
		if (this != &x)
		{
			std::swap(elements, x.elements);
			std::swap(first_free, x.first_free);
			std::swap(cap, x.cap);
		}
	}


	template <typename T>
	T* vector<T>::erase(const T* position)
	{

	}


	template <typename... Args>
	T* vector<T>::emplace(const T* position, Args&&... args)
	{

	}


	template <typename... Args>
	void vector<T>::emplace_back(Args&&... args)
	{
		chk_n_alloc();
		alloc.construct(first_free++, std::forward<Args>(args)...);
	}


	// capacity
	template <typename T>
	void vector<T>::resize(std::size_t n, const T &val)
	{
		if (n < size())
		{
			for (; first_free > elements + n; )
				alloc.destroy(--first_free);
		}
		if (n >= size() && n < capacity())
		{
			auto len_insert = n - size();
			first_free = uninitialized_fill_n(first_free, len_insert, val);
		}
		else
		{
			T *_start = alloc.allocate(n);
			//auto elem = elements;
			//auto _end = _start;
			//for (std::size_t i = 0; i < size(); ++i)
			//	alloc.construct(_end++, std::move(*elem++));
			//for (std::size_t i = size(); i < n; ++i)
			//	alloc.construct(_end++, val);

			auto len_insert = n - size();
			T *_end = uninitialized_copy(begin(), end(), _start);
			_end = uninitialized_fill_n(_end, len_insert, val);
			free();
			elements = _start;
			first_free = cap = _end;
		}
	}


	template <typename T>
	void vector<T>::reverse(std::size_t n)
	{
		if (n <= capacity())
			return;
		T* _start = alloc.allocate(n);
		T* _end = uninitialized_copy(elements, first_free, _start);
		free();
		elements = _start;
		first_free = _end;
		cap = elements + n;
	}


	template <typename T>
	void vector<T>::shrink_to_fit()
	{
		alloc.deallocate(first_free, cap - first_free);
		cap = first_free;
	}


	// 
	template <typename T>
	void vector<T>::free()
	{
		if (elements)
		{
			// destroy the object p in vector one by one
			for (auto p = first_free; p != elements;)
				alloc.destroy(--p);
			// and free the space.
			alloc.deallocate(elements, cap - elements);
		}
	}


	template <typename T>
	std::pair<T*, T*> vector<T>::alloc_n_copy(const T *first, const T *last)
	{
		auto data = alloc.allocate(last - first);
		return { data, uninitialized_copy(first, last, data) };
	}


	template <typename T>
	void vector<T>::reallocate()
	{
		auto newcapacity = size() ? 2 * size() : 1;
		auto newdata = alloc.allocate(newcapacity);
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

#endif