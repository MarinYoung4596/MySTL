#ifndef _STRING_IMPL_H_
#define _STRING_IMPL_H_

#include <utility>

#include "../Declaration/string.h"

#define  MIN(A, B) (A) < (B) ? (A) : (B)


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

	using const_iterator = MySTL::string::const_iterator;
	using iterator = MySTL::string::iterator;

	std::pair<iterator, iterator> string::alloc_n_copy(const_iterator first, const_iterator second)
	{
		auto _begin = alloc.allocate(second - first);
		auto _end = std::uninitialized_copy(first, second, _begin);
		return std::make_pair(_begin, _end);
	}


	std::pair<iterator, iterator> string::alloc_n_fill_n(const char &c, std::size_t n)
	{
		auto _begin = alloc.allocate(n);
		auto _end = std::uninitialized_fill_n(_begin, n, c);
		return std::make_pair(_begin, _end);
	}

	//////////////////// constructor ////////////////////
	string::string(const string &str)
	{
		auto new_str = alloc_n_copy(str.begin(), str.end());
		_start = new_str.first;
		_first_free = _cap = new_str.second;
	}

	string::string(const string &str, std::size_t pos, std::size_t len = npos)
	{
		iterator _end;
		if (len == npos)
			_end = str.end();
		else
			_end = MIN(str.begin() + pos + len, str.end());

		auto new_str = alloc_n_copy(str.begin() + pos, _end);
		_start = new_str.first;
		_first_free = _cap = new_str.second;
	}

	string::string(const char* s)
	{
		char *p = const_cast<char *>(s);
		for (; p != '\0'; ++p);

		auto new_str = alloc_n_copy(s, p);
		_start = new_str.first;
		_first_free = _cap = new_str.second;
	}

	string::string(const char* s, std::size_t n)
	{
		auto new_str = alloc_n_copy(s, s + n);
		_start = new_str.first;
		_first_free = _cap = new_str.second;
	}

	string::string(std::size_t n, char c)
	{
		auto new_str = alloc_n_fill_n(c, n);
		_start = new_str.first;
		_first_free = _cap = new_str.second;
	}

	template <typename InputIterator>
	string::string(InputIterator first, InputIterator last)
	{
		auto new_str = alloc_n_copy(first, last);
		_start = new_str.first;
		_first_free = _cap = new_str.second;
	}

	string::string(std::initializer_list<char> il)
	{
		auto new_str = alloc_n_copy(il.begin(), il.end());
		_start = new_str.first;
		_first_free = _cap = new_str.second;
	}

	string::string(string &&str) //noexcept
		: _start(str._start), _first_free(str._first_free), _cap(str._cap)
	{
		str._start = str._first_free = str._cap = nullptr;
	}


	//////////////////// string assignment ////////////////////
	string& string::operator= (const string& str)
	{

	}

}

#endif