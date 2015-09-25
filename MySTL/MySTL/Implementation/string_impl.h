#ifndef _STRING_IMPL_H_
#define _STRING_IMPL_H_

#include <utility>
#include <iterator>

#include "../Declaration/string.h"

#define  MIN(A, B) (A) < (B) ? (A) : (B)


namespace MySTL
{
	//////////////////// private functions ////////////////////
	void string::chk_n_alloc()
	{
		if (_end == _cap)
			reallocate();
	}

	void string::reallocate()
	{
		auto new_cap = size() ? 2 * size() : 1;
		auto data = alloc.allocate(new_cap);
		auto _elem = _begin;
		auto _dest = data;
		for (auto i = 0; i < size(); ++i)
			alloc.construct(_dest++, std::move(*_elem++));
		free();
		_begin = data;
		_end = _dest;
		_cap = _begin + new_cap;
	}

	void string::free()
	{
		if (_begin)
		{
			for (auto p = _end; p != _begin;)
				alloc.destroy(--p);
			alloc.deallocate(_begin, _cap - _begin);
		}
	}

	using const_iterator = MySTL::string::const_iterator;

	void string::alloc_n_copy(const_iterator first, const_iterator second)
	{
		auto start = alloc.allocate(second - first);
		auto finish = std::uninitialized_copy(first, second, _begin);
		
		_begin = start;
		_end = _cap = finish;
	}


	void string::alloc_n_fill_n(const char &c, std::size_t n)
	{
		auto start = alloc.allocate(n);
		auto finish = std::uninitialized_fill_n(_begin, n, c);
		
		_begin = start;
		_end = _cap = finish;
	}

	//////////////////// constructor ////////////////////
	string::string(const string &str)
	{
		alloc_n_copy(str.begin(), str.end());
	}

	string::string(const string &str, std::size_t pos, std::size_t len = npos)
	{
		if (pos > str.size() || len > str.size())
			throw std::out_of_range("out of range");
		
		const_iterator finish;
		if (len == npos)
			finish = _end;
		else
			finish = (str.begin() + pos + len) < str.end() ? (str.begin() + pos + len) : str.end();

		alloc_n_copy(str.begin() + pos, finish);
	}

	string::string(const char* s)
	{
		char *p = const_cast<char *>(s);
		for (; p != '\0'; ++p);

		alloc_n_copy(s, p);
	}

	string::string(const char* s, std::size_t n)
	{
		alloc_n_copy(s, s + n);
	}

	string::string(std::size_t n, char c)
	{
		alloc_n_fill_n(c, n);
	}

	template <typename InputIterator>
	string::string(InputIterator first, InputIterator last)
	{
		 alloc_n_copy(first, last);
	}

	string::string(std::initializer_list<char> il)
	{
		alloc_n_copy(il.begin(), il.end());
	}

	string::string(string &&str) //noexcept
		: _begin(str._begin), _end(str._end), _cap(str._cap)
	{
		str._begin = str._end = str._cap = nullptr;
	}


	string::~string()
	{
		free();
	}


	//////////////////// string assignment ////////////////////
	string& string::operator= (const string& str)
	{
		if (this != &str)
		{
			free();
			alloc_n_copy(str.begin(), str.end());
		}
		return *this;
	}

	string& string::operator= (const char* s)
	{
		auto *p = const_cast<char *>(s);
		for (; p != '\0'; ++p);
		
		free();
		alloc_n_copy(s, p);
		return *this;
	}

	string& string::operator= (char c)
	{
		free();
		alloc_n_fill_n(c, 1);
		return *this;
	}

	string& string::operator= (std::initializer_list<char> il)
	{
		free();
		alloc_n_copy(il.begin(), il.end());
		return *this;
	}

	string& string::operator= (string&& str) //noexcept
	{
		if (this != &str)
		{
			free();
			_begin = str._begin;
			_end = str._end;
			_cap = str._cap;

			str._begin = str._end = str._cap = nullptr;
		}
		return *this;
	}


	//////////////////// capacity ////////////////////
	void string::resize(std::size_t n)
	{
		resize(n, value_type());
	}

	void string::resize(std::size_t n, char c)
	{
		if (n < size())
		{
			for (; _end > _begin + n;)
				alloc.destroy(--_end);
		}
		else if (n >= size() && n < capacity())
		{
			auto len_insert = n - size();
			_end = std::uninitialized_fill_n(_end, len_insert, c);
		}
		else
		{
			auto len_insert = n - size();
			iterator start = alloc.allocate(n);
			iterator finish = std::uninitialized_copy(std::make_move_iterator(_begin), std::make_move_iterator(_end), start);
			finish = std::uninitialized_fill_n(finish, len_insert, c);
			free();
			_begin = start;
			_end = _cap = finish;
		}
	}

	void string::reserve(std::size_t n = 0)
	{
		if (n <= capacity())
			return;
		iterator start = alloc.allocate(n);
		iterator finish = std::uninitialized_copy(std::make_move_iterator(_begin), std::make_move_iterator(_end), start);
		free();
		_begin = start;
		_end = finish;
		_cap = start + n;
	}

	void string::clear() // noexcept
	{
		if (_begin)
		{
			while (_end != _begin)
				alloc.destroy(--_end);
		}
	}

	void string::shrink_to_fit()
	{
		alloc.deallocate(_end, _cap - _end);
		_cap = _end;
	}


	//////////////////// modifiers ////////////////////
	string::iterator string::insert_help(iterator p, std::size_t n, value_type c)
	{
		auto newcap = capacity() > n ? 2 * capacity() : capacity() + n;
		iterator start = alloc.allocate(newcap);
		iterator finish = std::uninitialized_copy(std::make_move_iterator(_begin), std::make_move_iterator(p), start);
		finish = std::uninitialized_fill_n(finish, n, c);
		
		iterator result = finish;

		finish = std::uninitialized_copy(std::make_move_iterator(finish), std::make_move_iterator(_end), finish);
		free();
		_begin = start;
		_end = finish;
		_cap = _begin + newcap;
		return result;
	}


	string& string::operator+= (const string& str)
	{
		insert(size(), str);
		return *this;
	}

	string& string::operator+= (const char* s)
	{
		insert(size(), s);
		return *this;
	}

	string& string::operator+= (char c)
	{
		insert(size(), 1, c);
		return *this;
	}

	string& string::operator+= (std::initializer_list<char> il)
	{
		insert(_end, il);
		return *this;
	}

	// insert
	string& string::insert(std::size_t pos, const string& str)
	{
		//if (pos > size())
		//	throw std::out_of_range("out of range");
		//if (str.empty())
		//	return *this;

		insert(_begin + pos, str.begin(), str.end());
		return *this;
	}

	string& string::insert(std::size_t pos, const string& str, std::size_t subpos, std::size_t sublen)
	{

	}

	string& string::insert(std::size_t pos, const char* s)
	{

	}

	string& string::insert(std::size_t pos, const char* s, std::size_t n)
	{

	}

	string& string::insert(std::size_t pos, std::size_t n, char c)
	{
		insert(_begin + pos, n, c);
		return *this;
	}

	// insert auxiliary: fill n
	string::iterator string::insert(const_iterator p, std::size_t n, char c)
	{
		auto newcap = capacity() > n ? 2 * capacity() : capacity() + n;
		iterator start = alloc.allocate(newcap);
		iterator finish = std::uninitialized_copy(
			std::make_move_iterator(_begin), 
			std::make_move_iterator(const_cast<iterator>(p)), 
			start);
		finish = std::uninitialized_fill_n(finish, n, c);

		iterator result = finish;

		finish = std::uninitialized_copy(std::make_move_iterator(finish), std::make_move_iterator(_end), finish);
		free();
		_begin = start;
		_end = finish;
		_cap = _begin + newcap;
		return result;
	}

	string::iterator string::insert(const_iterator p, char c)
	{
		return insert(p, 1, c);
	}

	// insert auxiliary: range
	template <typename InputIterator>
	string::iterator string::insert(iterator p, InputIterator first, InputIterator last)
	{
		auto newcap = capacity() > n ? 2 * capacity() : capacity() + n;
		iterator start = alloc.allocate(newcap);
		iterator finish = std::uninitialized_copy(
			std::make_move_iterator(_begin), std::make_move_iterator(p), start);
		finish = std::uninitialized_copy(std::make_move_iterator(first), std::make_move_iterator(last), finish);

		iterator result = finish;

		finish = std::uninitialized_copy(std::make_move_iterator(p), std::make_move_iterator(_end), finish);
		free();
		_begin = start;
		_end = finish;
		_cap = start + newcap;
		return result;
	}

	string& string::insert(const_iterator p, std::initializer_list<char> il)
	{
		insert(const_cast<iterator>(p), const_cast<iterator>(il.begin()), const_cast<iterator>(il.end()));
		return *this;
	}
}

#endif