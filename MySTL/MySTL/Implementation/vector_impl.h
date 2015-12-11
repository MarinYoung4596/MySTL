#ifndef INCLUDED_VECTOR_IMPL_H
#define INCLUDED_VECTOR_IMPL_H


#include <utility>	// std::move & std::forward & std::pair
#include <memory>	// std::uninitialized_copy & std::uninitialized_fill
#include <algorithm>	// std::swap, max
#include <stdexcept>

#include "../Declaration/vector.h"
#include "../Declaration/uninitialized_functions.h"


namespace MySTL
{

	template <typename T, typename Alloc>
	vector<T>::vector(const vector &s)
	{
		auto newdata = alloc_n_copy(s.begin(), s.end());
		elements = newdata.first;
		first_free = cap = newdata.second;
	}


	template <typename T, typename Alloc>
	vector<T>::vector(vector &&s) //noexcept
		: elements(s.elements), first_free(s.first_free), cap(s.cap)
	{
		s.elements = s.first_free = s.cap = nullptr;
	}


	template <typename T, typename Alloc>
	explicit vector<T>::vector(const size_type n)
	{
		auto newdata = data_allocator::allocate(n);
		std::uninitialized_fill_n(newdata, n, value_type());
		elements = newdata;
		first_free = cap = elements + n;
	}


	template <typename T, typename Alloc>
	vector<T>::vector(const size_type n, const T &val)
	{
		auto newdata = data_allocator::allocate(n);
		std::uninitialized_fill_n(newdata, n, val);
		elements = newdata;
		first_free = cap = elements + n;
	}


	template <typename T, typename Alloc>
	vector<T>::vector(std::initializer_list<T> il)
	{
		auto data = alloc_n_copy(il.begin(), il.end());
		elements = data.first;
		first_free = cap = data.second;
	}


	template <typename T, typename Alloc>
	template <typename InputIterator>
	vector<T>::vector(InputIterator first, InputIterator second)
	{
		auto data = alloc_n_copy(first, second);
		elements = data.first;
		first_free = cap = data.second;
	}


	template <typename T, typename Alloc>
	vector<T>& vector<T>::operator=(const vector &rhs)
	{
		auto data = alloc_n_copy(rhs.begin(), rhs.end());
		free();
		elements = data.first;
		first_free = cap = data.second;
		return *this;
	}


	template <typename T, typename Alloc>
	vector<T>& vector<T>::operator=(vector &&rhs) //noexcept
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


	template <typename T, typename Alloc>
	vector<T>& vector<T>::operator=(std::initializer_list<T> il)
	{
		auto data = alloc_n_copy(il.begin(), il.end());
		free();
		elements = data.first;
		first_free = cap = data.second;
		return *this;
	}


	template <typename T, typename Alloc>
	vector<T>::~vector()
	{
		free();
	}



	// Elements Access
	template <typename T, typename Alloc>
	typename vector<T>::reference vector<T>::at(size_type n)
	{
		if (n >= size())
		{
			throw std::out_of_range("out of range!");
		}
		return *(elements + n);
	}



	// Modifiers
	template <typename T, typename Alloc>
	void vector<T>::push_back(const value_type &s)
	{
		chk_n_alloc();
		alloc.construct(first_free++, s);
	}


	template <typename T, typename Alloc>
	void vector<T>::pop_back()
	{
		if (elements)
			alloc.destroy(--first_free);
	}


	template <typename T, typename Alloc>
	typename vector<T>::iterator vector<T>::insert(iterator position, const value_type &val)
	{
		return vector<T>::insert(position, 1, val);
	}


	template <typename T, typename Alloc>
	typename vector<T>::iterator vector<T>::insert(iterator position, size_type n, const value_type &val)
	{
		auto space_left = cap - first_free;
		if (space_left >= n)
		{
			for (auto curr = end() - 1; curr != position; --curr)
				*(curr + n) = *curr;
			std::uninitialized_fill_n(position, n, val);
			first_free += n;
		}
		else
		{
			// bug fix: 2015/10/6
			using std::max;
			const size_type len = size() + max(size(), n);
			iterator _start = data_allocator::allocate(static_cast<size_type>(len));
			iterator _end = std::uninitialized_copy(begin(), position, _start);
			_end = std::uninitialized_fill_n(_end, n, val); // _end point to next free space
			_end = std::uninitialized_copy(position, end(), _end);
			free();
			elements = _start;
			first_free = _end;
			cap = elements + len;
		}
		return const_cast<T *>(position);
	}


	template <typename T, typename Alloc>
	template <typename InputIterator>
	typename vector<T>::iterator vector<T>::insert(iterator position, InputIterator first, InputIterator second)
	{
		difference_type space_left = cap - first_free;
		difference_type space_required = second - first;
		if (space_left >= space_required)
		{
			std::uninitialized_copy(position, end(), end() + space_required);
			std::uninitialized_copy(first, second, position);// insert
			first_free += space_required;
		}
		else
		{
			using std::max;
			const size_type len = size() + max(size(), space_required);
			iterator _start = data_allocator::allocate(len);
			iterator _end = std::uninitialized_copy(begin(), position, _start);
			_end = std::uninitialized_copy(first, second, _end); // insert
			_end = std::uninitialized_copy(position, end(), _end);
			free();
			elements = _start;
			first_free = _end;
			cap = elements + len;
		}
		return const_cast<iterator>(position);
	}


	template <typename T, typename Alloc>
	typename vector<T>::iterator vector<T>::insert(iterator position, std::initializer_list<T> il)
	{
		return insert<T>(position, il.begin(), il.end());
	}


	template <typename T, typename Alloc>
	void vector<T>::swap(vector &x)
	{
		if (this != &x)
		{
			using std::swap;
			swap(elements, x.elements);
			swap(first_free, x.first_free);
			swap(cap, x.cap);
		}
	}


	template <typename T, typename Alloc>
	typename vector<T>::iterator vector<T>::erase(const_iterator position)
	{
		return vector<T>::erase(position, position + 1);
	}


	template <typename T, typename Alloc>
	typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator second)
	{
		if (first == second)
			return const_cast<iterator>(first);

		difference_type len = second - first;
		iterator _first = const_cast<iterator>(first);
		iterator _second = const_cast<iterator>(second);
		for (auto i = 0; i < len && (_second + i) != first_free; ++i)
			*(_first + i) = *(_second + i);
		// destory the objects from second to end() one by one
		for (auto ptr = second; ptr != end(); ++ptr)
			data_allocator::destroy(ptr);

		first_free -= len;
		return const_cast<iterator>(first);
	}


	template <typename T, typename Alloc>
	template <typename... Args>
	void vector<T>::emplace(const_iterator position, Args&&... args)
	{
		chk_n_alloc();
		for (auto curr = end(); curr != position; --curr)
			*curr = *(curr - 1); // move forward
		data_allocator::construct(position, std::forward<Args>(args)...);
		++first_free;
	}


	template <typename T, typename Alloc>
	template <typename... Args>
	void vector<T>::emplace_back(Args&&... args)
	{
		chk_n_alloc();
		data_allocator::construct(first_free++, std::forward<Args>(args)...);
	}


	// capacity
	template <typename T, typename Alloc>
	void vector<T>::resize(size_type n, value_type val = value_type())
	{
		if (n < size())
		{
			for (; first_free > elements + n;)
				data_allocator::destroy(--first_free);
		}
		else if (n >= size() && n < capacity())
		{
			auto len_insert = n - size();
			first_free = std::uninitialized_fill_n(first_free, len_insert, val);
		}
		else
		{
			T *_start = alloc.allocate(n);
			auto len_insert = n - size();
			T *_end = std::uninitialized_copy(begin(), end(), _start);
			_end = std::uninitialized_fill_n(_end, len_insert, val);
			free();
			elements = _start;
			first_free = cap = _end;
		}
	}


	template <typename T, typename Alloc>
	void vector<T>::reserve(size_type n)
	{
		if (n <= capacity())
			return;
		iterator _start = data_allocator::allocate(n);
		iterator _end = std::uninitialized_copy(begin(), end(), _start);
		free();
		elements = _start;
		first_free = _end;
		cap = elements + n;
	}


	template <typename T, typename Alloc>
	void vector<T>::shrink_to_fit()
	{
		data_allocator::deallocate(first_free, cap - first_free);
		cap = first_free;
	}


	// private functions
	template <typename T, typename Alloc>
	void vector<T>::free()
	{
		if (elements)
		{
			// destroy the object p in vector one by one
			for (auto p = first_free; p != elements; --p)
				data_allocator::destroy(p);
			// and free the space.
			data_allocator::deallocate(elements, cap - elements);
		}
	}



	template <typename T, typename Alloc>
	void vector<T>::chk_n_alloc()
	{
		if (size() == capacity())
			reallocate();
	}


	template <typename T, typename Alloc>
	template <typename InputIterator>
	std::pair<typename vector<T>::iterator, typename vector<T>::iterator>
		vector<T>::alloc_n_copy(InputIterator first, InputIterator last)
	{
		auto _begin = data_allocator::allocate(last - first);
		auto _end = std::uninitialized_copy(first, last, _begin);
		return std::make_pair(_begin, _end);
	}


	template <typename T, typename Alloc>
	void vector<T>::reallocate()
	{
		auto newcapacity = size() ? 2 * size() : 1;
		auto newdata = data_allocator::allocate(newcapacity);
		
		auto dest = newdata;
		auto elem = elements;
		for (size_type i = 0; i != size(); ++i)
		{
			data_allocator::construct(dest++, std::move(*elem++));
		}
		// or
		// dest = std::uninitialized_copy(std::make_move_iterator(begin()), std::make_move_iterator(end()), newdata);

		free();
		elements = newdata;
		first_free = dest;
		cap = elements + newcapacity;
	}

}
#endif
