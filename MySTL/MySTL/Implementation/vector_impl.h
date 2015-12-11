#ifndef INCLUDED_VECTOR_IMPL_H
#define INCLUDED_VECTOR_IMPL_H


#include <utility>	// std::move & std::forward & std::pair
#include <memory>	// std::uninitialized_copy & std::uninitialized_fill
#include <algorithm>	// std::swap, max
#include <stdexcept>

#include "../Declaration/vector.h"
#include "../Declaration/uninitialized_functions.h"
#include "../Declaration/construct.h"

using namespace MySTL;
namespace MySTL
{
	/*
	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(const vector &s)
	{
		auto newdata = alloc_n_copy(s.begin(), s.end());
		elements_start = newdata.first;
		first_free = end_of_storage = newdata.second;
	}


	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(vector &&s) noexcept
		: elements_start(s.elements_start), first_free(s.first_free), end_of_storage(s.end_of_storage)
	{
		s.elements_start = s.first_free = s.end_of_storage = nullptr;
	}


	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(const size_type n)
	{
		auto newdata = data_allocator::allocate(n);
		std::uninitialized_fill_n(newdata, n, value_type());
		elements_start = static_cast<iterator>(newdata);
		first_free = end_of_storage = elements_start + n;
	}


	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(const size_type n, const T &val)
	{
		auto newdata = data_allocator::allocate(n);
		std::uninitialized_fill_n(newdata, n, val);
		elements_start = newdata;
		first_free = end_of_storage = elements_start + n;
	}


	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(std::initializer_list<T> il)
	{
		auto data = alloc_n_copy(il.begin(), il.end());
		elements_start = data.first;
		first_free = end_of_storage = data.second;
	}


	template <typename T, typename Alloc>
	template <typename InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator second)
	{
		auto data = alloc_n_copy(first, second);
		elements_start = data.first;
		first_free = end_of_storage = data.second;
	}


	template <typename T, typename Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector<T, Alloc> &rhs)
	{
		auto data = alloc_n_copy(rhs.begin(), rhs.end());
		free();
		elements_start = data.first;
		first_free = end_of_storage = data.second;
		return *this;
	}


	template <typename T, typename Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(vector<T, Alloc> &&rhs) noexcept
	{
		if (this != &rhs)
		{
			free();
			elements_start = rhs.elements_start;
			first_free = rhs.first_free;
			end_of_storage = rhs.end_of_storage;
			rhs.elements_start = rhs.first_free = rhs.elements_start = nullptr;
		}
		return *this;
	}


	template <typename T, typename Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(std::initializer_list<T> il)
	{
		auto data = alloc_n_copy(il.begin(), il.end());
		free();
		elements_start = data.first;
		first_free = end_of_storage = data.second;
		return *this;
	}
	

	*/
	template <typename T, typename Alloc>
	vector<T, Alloc>::~vector()
	{
	
		std::free(elements_start);
		std::free(first_free);
		std::free(end_of_storage);
	}
	/*


	// Elements Access
	template <typename T, typename Alloc>
	typename vector<T, Alloc>::reference vector<T, Alloc>::at(size_type n)
	{
		if (n >= size())
		{
			throw std::out_of_range("out of range!");
		}
		return *(elements_start + n);
	}



	// Modifiers
	template <typename T, typename Alloc>
	void vector<T, Alloc>::push_back(const value_type &s)
	{
		chk_n_alloc();
		data_allocator::construct(first_free++, s);
	}


	template <typename T, typename Alloc>
	void vector<T, Alloc>::pop_back()
	{
		if (elements_start)
			data_allocator::destroy(--first_free);
	}


	template <typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type &val)
	{
		return vector<T, Alloc>::insert(position, 1, val);
	}


	template <typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, size_type n, const value_type &val)
	{
		auto space_left = end_of_storage - first_free;
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
			elements_start = _start;
			first_free = _end;
			end_of_storage = elements_start + len;
		}
		return const_cast<T *>(position);
	}


	template <typename T, typename Alloc>
	template <typename InputIterator>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator second)
	{
		difference_type space_left = end_of_storage - first_free;
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
			const size_type len = size() + max(size(), static_cast<size_type>(space_required));
			iterator _start = static_cast<iterator>(data_allocator::allocate(len));
			iterator _end = std::uninitialized_copy(begin(), position, _start);
			_end = std::uninitialized_copy(first, second, _end); // insert
			_end = std::uninitialized_copy(position, end(), _end);
			free();
			elements_start = _start;
			first_free = _end;
			end_of_storage = elements_start + len;
		}
		return const_cast<iterator>(position);
	}


	template <typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, std::initializer_list<T> il)
	{
		return insert<T>(position, il.begin(), il.end());
	}


	template <typename T, typename Alloc>
	void vector<T, Alloc>::swap(vector &x)
	{
		if (this != &x)
		{
			using std::swap;
			swap(elements_start, x.elements_start);
			swap(first_free, x.first_free);
			swap(end_of_storage, x.end_of_storage);
		}
	}


	template <typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position)
	{
		return vector<T, Alloc>::erase(position, position + 1);
	}


	template <typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator second)
	{
		if (first == second)
			return first;

		difference_type len = second - first;
		for (auto i = 0; i < len && (second + i) != first_free; ++i)
			*(first + i) = *(second + i);
		// destory the objects from second to end() one by one
		for (auto ptr = second; ptr != end(); ++ptr)
			data_allocator::destroy(ptr);

		first_free -= len;
		return first;
	}


	//template <typename T, typename Alloc>
	//template <typename... Args>
	//void vector<T, Alloc>::emplace(const_iterator position, Args&&... args)
	//{
	//	chk_n_alloc();
	//	for (auto curr = end(); curr != position; --curr)
	//		*curr = *(curr - 1); // move forward
	//	data_allocator::construct(position, std::forward<Args>(args)...);
	//	++first_free;
	//}


	//template <typename T, typename Alloc>
	//template <typename... Args>
	//void vector<T, Alloc>::emplace_back(Args&&... args)
	//{
	//	chk_n_alloc();
	//	data_allocator::construct(first_free++, std::forward<Args>(args)...);
	//}


	// capacity
	template <typename T, typename Alloc>
	void vector<T, Alloc>::resize(size_type n, value_type val = value_type())
	{
		if (n < size())
		{
			for (; first_free > elements_start + n;)
				data_allocator::destroy(--first_free);
		}
		else if (n >= size() && n < capacity())
		{
			auto len_insert = n - size();
			first_free = std::uninitialized_fill_n(first_free, len_insert, val);
		}
		else
		{
			T *_start = static_cast<T*>(Alloc::allocate(n));
			auto len_insert = n - size();
			T *_end = std::uninitialized_copy(begin(), end(), _start);
			_end = std::uninitialized_fill_n(_end, len_insert, val);
			free();
			elements_start = _start;
			first_free = end_of_storage = _end;
		}
	}


	template <typename T, typename Alloc>
	void vector<T, Alloc>::reserve(size_type n)
	{
		if (n <= capacity())
			return;
		iterator _start = static_cast<iterator>(data_allocator::allocate(n));
		iterator _end = std::uninitialized_copy(begin(), end(), _start);
		free();
		elements_start = _start;
		first_free = _end;
		end_of_storage = elements_start + n;
	}


	template <typename T, typename Alloc>
	void vector<T, Alloc>::shrink_to_fit()
	{
		data_allocator::deallocate(first_free, end_of_storage - first_free);
		end_of_storage = first_free;
	}

	
	// private functions
	template <typename T, typename Alloc>
	void vector<T, Alloc>::free()
	{
		if (elements_start)
		{
			// destroy the object p in vector one by one
			for (auto p = first_free; p != elements_start; --p)
				data_allocator::destroy(p);
			// and free the space.
			data_allocator::deallocate(elements_start, end_of_storage - elements_start);
		}
	}
	


	template <typename T, typename Alloc>
	void vector<T, Alloc>::chk_n_alloc()
	{
		if (size() == capacity())
			reallocate();
	}


	template <typename T, typename Alloc>
	template <typename InputIterator>
	std::pair<typename vector<T, Alloc>::iterator, typename vector<T, Alloc>::iterator>
		vector<T, Alloc>::alloc_n_copy(InputIterator first, InputIterator last)
	{
		auto _begin = data_allocator::allocate(last - first);
		auto _end = std::uninitialized_copy(first, last, _begin);
		return std::make_pair(_begin, _end);
	}


	template <typename T, typename Alloc>
	void vector<T, Alloc>::reallocate()
	{
		auto newcapacity = size() ? 2 * size() : 1;
		auto newdata = data_allocator::allocate(newcapacity);
		
		auto dest = newdata;
		auto elem = elements_start;
		for (size_type i = 0; i != size(); ++i)
		{
			data_allocator::construct(dest++, std::move(*elem++));
		}
		// or
		// dest = std::uninitialized_copy(std::make_move_iterator(begin()), std::make_move_iterator(end()), newdata);

		free();
		elements_start = newdata;
		first_free = dest;
		end_of_storage = elements_start + newcapacity;
	}
	*/
}
#endif
