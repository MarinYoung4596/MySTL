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
	vector::~vector()
	{
		free();
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
	void vector::push_back(const T &s)
	{
		chk_n_alloc();
		alloc.construct(first_free++, s);
	}


	std::pair<std::string*, std::string*>
		vector::alloc_n_copy(const string* b, const string *e)
	{
			auto data = alloc.allocate(e - b);
			return{ data, unintialized_copy(b, e, data) };
		}


	template <typename T>
	void vector::free()
	{
		if (elements)
		{
			// destory the object p in vector one by one,
			for (auto p = first_free; p != elements;)
				alloc.destory(--p);
			// and free the space.
			alloc.deallocate(elements, cap - elements);
		}
	}


	template <typename T>
	void vector::clear()
	{
		if (elements)
		{
			for (auto p = first_free; p != elements;)
				alloc.destory(--p);
		}
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
