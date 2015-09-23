#ifndef _STRING_IMPL_H_
#define _STRING_IMPL_H_

#include <utility>

#include "../Declaration/string.h"


namespace MySTL
{
	//////////////////// private functions ////////////////////
	void string::chk_n_alloc()
	{
		if (_first_free == _cap)
			reallocate();
	}

	void string::reallocate()
	{
		auto new_cap = size() ? 2 * size() : 1;
		auto data = alloc.allocate(new_cap);
		auto _elem = _start;
		auto _dest = data;
		for (auto i : size())
			alloc.construct(_dest++, std::move(*_elem++));
		free();
		_start = data;
		_first_free = _dest;
		_cap = _start + new_cap;
	}

	void string::free()
	{
		if (_start)
		{
			for (auto p = _first_free; p != _start;)
				alloc.destroy(--p);
			alloc.deallocate(_start, _cap - _start);
		}
	}

	using iterator = MySTL::string::iterator;

	std::pair<iterator, iterator> string::alloc_n_copy(iterator first, iterator second)
	{
		auto data = alloc.allocate(second - first);
		return{ data, std::uninitialized_copy(first, second, data) };
	}


	//////////////////// constructor ////////////////////
	string::string(const string &str)
	{

	}


}

#endif