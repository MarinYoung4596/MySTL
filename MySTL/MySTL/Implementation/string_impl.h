#ifndef _STRING_IMPL_H_
#define _STRING_IMPL_H_

#include <utility>     // for std::move()
#include <iterator>    // for make_move_iterator
#include <stdexcept>   // for std::out_of_range, std::invalid_argument

#include "../Declaration/string.h"



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


	std::size_t string::strlen(const char *s) const
	{
		std::size_t len = 0;
		for (auto p = s; p != '\0'; ++p, ++len);
		return len;
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
		alloc_n_copy(s, s + strlen(s));
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
	// operator =
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
		free();
		alloc_n_copy(s, s + strlen(s));
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
		resize(n, value_type());  // value_type() ??
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


	//////////////////// element access ////////////////////
	char& string::at(std::size_t pos)
	{
		if (pos > size())
			throw std::out_of_range("out of range!");
		return *(_begin + pos);
	}

	const char& string::at(std::size_t pos) const
	{
		if (pos > size())
			throw std::out_of_range("out of range!");
		return *(_begin + pos);
	}


	//////////////////// modifiers ////////////////////
	// operator +=
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


	// append
	string& string::append(const string& str)
	{
		return insert(size(), str);
	}

	string& string::append(const string& str, std::size_t subpos, std::size_t sublen)
	{
		return insert(size(), str, subpos, sublen);
	}

	string& string::append(const char* s)
	{
		return insert(size(), s);
	}

	string& string::append(const char* s, std::size_t n)
	{
		return insert(size(), s, n);
	}

	string& string::append(std::size_t n, char c)
	{
		return insert(size(), n, c);
	}

	template <typename InputIterator>
	string& string::append(InputIterator first, InputIterator last)
	{
		insert(_end, first, last);
		return *this;
	}

	string& string::append(std::initializer_list<char> il)
	{
		return insert(_end, il);
	}

	void string::push_back(char c)
	{
		chk_n_alloc();
		alloc.construct(_end++, c);
	}

	// assign
	string& string::assign(const string& str)
	{
		return assign(str.begin(), str.end());
	}

	string& string::assign(const string& str, std::size_t subpos, std::size_t sublen)
	{
		return assign(str.begin() + subpos, str.begin() + subpos + sublen);
	}

	string& string::assign(const char* s)
	{
		return assign(s, s + strlen(s));
	}

	string& string::assign(const char* s, std::size_t n)
	{
		return assign(s, s + n);
	}

	string& string::assign(std::size_t n, char c)
	{
		if (n <= capacity())
		{
			iterator finish = std::uninitialized_fill_n(_begin, n, c);
			_end = n <= size() ? _end : finish;
		}
		else
		{
			free();
			auto newcap = 2 * capacity();
			iterator data = alloc.allocate(newcap);
			_end = std::uninitialized_fill_n(data, n, c);
			_begin = data;
			_cap = _begin + newcap;
		}
		return *this;
	}

	template <typename InputIterator>
	string& string::assign(InputIterator first, InputIterator last)
	{
		difference_type space_required = last - first;
		if (space_required <= capacity())
		{
			iterator finish = std::uninitialized_copy(first, last, _begin);
			_end = space_required <= size() ? _end : finish;
		}
		else
		{
			free();
			auto newcap = 2 * capacity();
			iterator data = alloc.allocate(newcap);
			_end = std::uninitialized_copy(first, last, data);
			_begin = data;
			_cap = _begin + newcap;
		}
		return *this;
	}

	string& string::assign(std::initializer_list<char> il)
	{
		return assign(il.begin(), il.end());
	}

	string& string::assign(string&& str) //noexcept
	{
		if (this != &str)
		{
			_begin = str._begin;
			_end = str._end;
			_cap = str._cap;
			str._begin = str._end = str._cap = nullptr;
		}
		return *this;
	}


	// insert
	string& string::insert(std::size_t pos, const string& str)
	{
		insert(_begin + pos, str.begin(), str.end());
		return *this;
	}

	string& string::insert(std::size_t pos, const string& str, std::size_t subpos, std::size_t sublen)
	{
		insert(_begin + pos, str.begin() + subpos, str.begin() + subpos + sublen);
		return *this;
	}

	string& string::insert(std::size_t pos, const char* s)
	{
		insert(_begin + pos, s, s + strlen(s));
		return *this;
	}

	string& string::insert(std::size_t pos, const char* s, std::size_t n)
	{
		insert(_begin + pos, s, s + n);
		return *this;
	}

	string& string::insert(std::size_t pos, std::size_t n, char c)
	{
		insert(_begin + pos, n, c);
		return *this;
	}

	// insert auxiliary: fill n
	string::iterator string::insert(const_iterator p, std::size_t n, char c)
	{
		if (p < _begin || p > _end)
			throw std::out_of_range("out of range");
		if (!n)
			return const_cast<iterator>(p);

		auto space_left = _cap - _end;

		iterator res = nullptr;
		if (n <= space_left)
		{
			for (iterator curr = _end - 1; curr != p; --curr)
				*(curr + n) = *curr;
			res = std::uninitialized_fill_n(const_cast<iterator>(p), n, c);
			_end += n;
		}
		else
		{
			auto newcap = capacity() > n ? 2 * capacity() : capacity() + n;
			iterator start = alloc.allocate(newcap);
			iterator finish = std::uninitialized_copy(
				std::make_move_iterator(_begin),
				std::make_move_iterator(const_cast<iterator>(p)),
				start);
			res = finish = std::uninitialized_fill_n(finish, n, c);
			finish = std::uninitialized_copy(std::make_move_iterator(finish), std::make_move_iterator(_end), finish);
			free();
			_begin = start;
			_end = finish;
			_cap = _begin + newcap;
		}
		return res;
	}

	string::iterator string::insert(const_iterator p, char c)
	{
		return insert(p, 1, c);
	}

	// insert auxiliary: range
	template <typename InputIterator>
	string::iterator string::insert(iterator p, InputIterator first, InputIterator last)
	{
		if (p < _begin || p > _end)
			throw std::out_of_range("out of range");

		difference_type space_required = last - first;
		if (!space_required)
			return p;

		difference_type space_left = _cap - _end;
		iterator res = nullptr;

		if (space_required <= space_left)
		{
			for (iterator curr = _end - 1; curr != p; --curr)
				*(curr + space_required) = *curr;
			res = std::uninitialized_copy(first, last, p);
			_end += space_required;
		}
		else
		{
			auto newcap = capacity() > n ? 2 * capacity() : capacity() + n;
			iterator start = alloc.allocate(newcap);
			iterator finish = std::uninitialized_copy(
				std::make_move_iterator(_begin), std::make_move_iterator(p), start);
			res = finish = std::uninitialized_copy(std::make_move_iterator(first), std::make_move_iterator(last), finish);
			finish = std::uninitialized_copy(std::make_move_iterator(p), std::make_move_iterator(_end), finish);
			free();
			_begin = start;
			_end = finish;
			_cap = start + newcap;
		}
		return res;
	}

	string& string::insert(const_iterator p, std::initializer_list<char> il)
	{
		insert(const_cast<iterator>(p), const_cast<iterator>(il.begin()), const_cast<iterator>(il.end()));
		return *this;
	}


	// erase
	string& string::erase(std::size_t pos = 0, std::size_t len = npos)
	{
		return erase(cbegin() + pos, cbegin() + pos + len);
	}

	string::iterator string::erase(const_iterator p)
	{
		return erase(p, p + 1);
	}

	string::iterator string::erase(const_iterator first, const_iterator last)
	{
		if (first < _begin || first > _end ||
			last < _begin || last > _end)
			throw std::out_of_range("out of range");
		if (first > last)
			throw std::invalid_argument("invalid argument");
		if (first == last)
			return const_cast<iterator>(first);

		difference_type len = last - first;
		for (iterator i = const_cast<iterator>(first); i != last && i != _end; ++i)
			*i = *(i + len);  // right?
		
		const_iterator stop = _end - len;
		for (; _end != stop; )
			alloc.destroy(--_end);
		
		return const_cast<iterator>(first);
	}


	// replace
	string& string::replace(std::size_t pos, std::size_t len, const string& str)
	{
		return replace(_begin + pos, _begin + pos + len, str.begin(), str.end());
	}

	string& string::replace(const_iterator i1, const_iterator i2, const string& str)
	{
		return replace(i1, i2, str.begin(), str.end());
	}

	string& string::replace(std::size_t pos, std::size_t len, const string& str, std::size_t subpos, std::size_t sublen)
	{
		return replace(_begin + pos, _begin + pos + len, str.begin() + subpos, str.begin() + subpos + sublen);
	}

	string&	string::replace(std::size_t pos, std::size_t len, const char* s)
	{
		return replace(_begin + pos, _begin + pos + len, s, s + strlen(s));
	}

	string& string::replace(const_iterator i1, const_iterator i2, const char* s)
	{
		return replace(i1, i2, s, s + strlen(s));
	}

	string& string::replace(std::size_t pos, std::size_t len, const char* s, std::size_t n)
	{
		return replace(_begin + pos, _begin + pos + len, s, s + n);
	}

	string& string::replace(const_iterator i1, const_iterator i2, const char* s, std::size_t n)
	{
		return replace(i1, i2, s, s + n);
	}

	string& string::replace(std::size_t pos, std::size_t len, std::size_t n, char c)
	{
		return replace(_begin + pos, _begin + pos + len, n, c);
	}

	// replace auxiliary: fill
	string& string::replace(const_iterator i1, const_iterator i2, std::size_t n, char c)
	{
		iterator finish = erase(i1, i2);
		insert(finish, n, c);
		return *this;
	}

	// replace auxiliary: range
	template <typename InputIterator>
	string& string::replace(const_iterator i1, const_iterator i2, InputIterator first, InputIterator last)
	{
		iterator finish = erase(i1, i2);
		insert(finish, first, last);
		return *this;
	}

	string& string::replace(const_iterator i1, const_iterator i2, std::initializer_list<char> il)
	{
		return replace(i1, i2, il.begin(), il.end());
	}


	// swap
	void string::swap(string& str)
	{
		if (this != &str)
			MySTL::swap(*this, str);		// friend function
	}


	// pop_back
	void string::pop_back()
	{
		if (_begin)
			alloc.destroy(--_end);
	}



	//////////////////// string operations ////////////////////
	const char* string::c_str() const //noexcept
	{
		char *res = new char(size() + 1);
		char *finish = std::uninitialized_copy(_begin, _end, res);
		*finish = '\0';
		return res;
	}

	const char* string::data() const //noexcept
	{
		return c_str();
	}

	string::allocator_type string::get_allocator() const //noexcept
	{
		return alloc;
	}

	std::size_t string::copy(char* s, std::size_t len, std::size_t pos = 0) const
	{
		auto end_copy = (pos + len) < length() ? (_begin + pos + len) : _end;
		auto finish =  std::uninitialized_copy(_begin + pos, end_copy, s);
		return end_copy == _end ? length() - pos : len;
	}


	// find
	// @str		another string with the subject to search for
	// @s		pointer to an array of characters 
	//			(if argument @n is specified, the sequence to match are the first n characters in the array)
	// @c		individual character to be searched for
	// @pos		position of the first character in the string to be considered in the search
	// @n		length of sequence of characters to match
	std::size_t string::find(const string& str, std::size_t pos = 0) const //noexcept
	{
		// KMP ?
	}

	std::size_t string::find(const char* s, std::size_t pos = 0) const
	{
		return find(s, pos, strlen(s));
	}

	// find auxiliary: buffer
	std::size_t string::find(const char* s, std::size_t pos, std::size_t n) const
	{

	}

	std::size_t string::find(char c, std::size_t pos = 0) const //noexcept
	{
		for (auto cit = cbegin() + pos; cit != cend(); ++cit)
		{
			if (*cit == c)
				return cit - cbegin();
		}
		return npos; // -1
	}


	// rfind
	std::size_t string::rfind(const string& str, std::size_t pos = npos) const //noexcept
	{

	}
	
	std::size_t string::rfind(const char* s, std::size_t pos = npos) const
	{
		return rfind(s, pos, strlen(s));
	}

	// rfind auxiliary: buffer
	std::size_t string::rfind(const char* s, std::size_t pos, std::size_t n) const
	{

	}

	std::size_t string::rfind(char c, std::size_t pos = npos) const // noexcept
	{
		for (auto cit = cbegin() + pos; cit >= cbegin(); --cit)
		{
			if (*cit == c)
				return cit - cbegin();
		}
		return npos;
	}


	// find_first_of
	std::size_t string::find_first_of(const string& str, std::size_t pos = 0) const //noexcept
	{

	}

	std::size_t string::find_first_of(const char* s, std::size_t pos = 0) const
	{
		return find_first_of(s, pos, strlen(s));
	}

	std::size_t string::find_first_of(const char* s, std::size_t pos, std::size_t n) const
	{

	}

	std::size_t string::find_first_of(char c, std::size_t pos = 0) const //noexcept
	{
		return find(c, pos);
	}


	// find_last_of
	std::size_t string::find_last_of(const string& str, std::size_t pos = npos) const //noexcept
	{
		
	}

	std::size_t string::find_last_of(const char* s, std::size_t pos = npos) const
	{
		return find_last_of(s, pos, strlen(s));
	}

	std::size_t string::find_last_of(const char* s, std::size_t pos, std::size_t n) const
	{

	}

	std::size_t string::find_last_of(char c, std::size_t pos = npos) const //noexcept
	{
		return rfind(c, pos);
	}


	// find_first_not_of
	std::size_t string::find_first_not_of(const string& str, std::size_t pos = 0) const //noexcept
	{

	}

	std::size_t string::find_first_not_of(const char* s, std::size_t pos = 0) const
	{
		return find_first_not_of(s, pos, strlen(s));
	}

	std::size_t string::find_first_not_of(const char* s, std::size_t pos, std::size_t n) const
	{

	}

	std::size_t string::find_first_not_of(char c, std::size_t pos = 0) const //noexcept
	{
		for (auto i = pos; i != size(); ++i)
		{
			if (*(cbegin() + i) != c)
				return i;
		}
		return npos;
	}


	// find_last_not_of
	std::size_t string::find_last_not_of(const string& str, std::size_t pos = npos) const //noexcept
	{

	}

	std::size_t string::find_last_not_of(const char* s, std::size_t pos = npos) const
	{
		return find_last_not_of(s, pos, strlen(s));
	}

	std::size_t string::find_last_not_of(const char* s, std::size_t pos, std::size_t n) const
	{

	}

	std::size_t string::find_last_not_of(char c, std::size_t pos = npos) const //noexcept
	{
		for (auto i = pos; i >= 0; --i)
		{
			if (*(cbegin() + i) != c)
				return i;
		}
		return npos;
	}


	string string::substr(std::size_t pos = 0, std::size_t len = npos) const
	{
		string res(*this, pos, len);
		return res;
	}


	// compare
	// @str		another string object, used entirely (or partially) as the \comparing string
	// @s		pointer to an array of characters 
	//			(if argument @n is specified, the first n characters in the array are used as the \comparing string)
	// @pos		position of the first character in the \compared string 
	// @len		length of \compared string (if the string is shorter, as many characters as possible)
	// @subpos, sublen
	//			same as @pos and @len above, but for /comparing string
	// @n		number of characters to compare
	int string::compare(const string& str) const //noexcept
	{
		return compare(0, size(), str, 0, str.size());
	}

	int string::compare(std::size_t pos, std::size_t len, const string& str) const
	{
		return compare(pos, len, str, 0, str.size());
	}

	// compare auxiliary: string object
	int string::compare(std::size_t pos, std::size_t len, const string& str, std::size_t subpos, std::size_t sublen) const
	{
		return compare_aux(pos, len, str.begin() + subpos, str.begin() + subpos + sublen);
	}

	int string::compare(const char* s) const
	{
		return compare(0, size(), s, strlen(s));
	}

	int string::compare(std::size_t pos, std::size_t len, const char* s) const
	{
		return compare(pos, len, s, strlen(s));
	}

	// compare auxiliary: buffer
	int string::compare(std::size_t pos, std::size_t len, const char* s, std::size_t n) const
	{
		return compare_aux(pos, len, s, s + n);
	}

	// compare auxiliary
	template <typename InputIterator>
	int string::compare_aux(std::size_t pos, std::size_t len, InputIterator first, InputIterator last)
	{

	}




	//////////////////// non-member functions overloads ////////////////////
	// operator +
	string operator+ (const string& lhs, const string& rhs)
	{
		// can be implemented by insert, append, +=
		string res(lhs);
		res += rhs;
		return res;
	}

	string operator+ (string&&      lhs, string&&      rhs)
	{
		string res(lhs);
		res += rhs;
		return res;
	}

	string operator+ (string&&      lhs, const string& rhs)
	{
		string res(lhs);
		res += rhs;
		return res;
	}

	string operator+ (const string& lhs, string&&      rhs)
	{
		string res(lhs);
		res += rhs;
		return res;
	}

	string operator+ (const string& lhs, const char*   rhs)
	{
		string res(lhs);
		res += rhs;
		return res;
	}

	string operator+ (string&&      lhs, const char*   rhs)
	{
		string res(lhs);
		res += rhs;
		return res;
	}

	string operator+ (const char*   lhs, const string& rhs)
	{
		string res(lhs);
		res += rhs;
		return res;
	}

	string operator+ (const char*   lhs, string&&      rhs)
	{
		string res(lhs);
		res += rhs;
		return res;
	}

	string operator+ (const string& lhs, char          rhs)
	{
		string res(lhs);
		res += rhs;
		return res;
	}

	string operator+ (string&&      lhs, char          rhs)
	{
		string res(lhs);
		res += rhs;
		return res;
	}

	string operator+ (char          lhs, const string& rhs)
	{
		string res(1, lhs);
		res += rhs;
		return res;
	}

	string operator+ (char          lhs, string&&      rhs)
	{
		string res(1, lhs);
		res += rhs;
		return res;
	}


	// comparing operator
	bool operator== (const string& lhs, const string& rhs)
	{

	}

	bool operator== (const char*   lhs, const string& rhs){}
	bool operator== (const string& lhs, const char*   rhs){}
	bool operator!= (const string& lhs, const string& rhs){}
	bool operator!= (const char*   lhs, const string& rhs){}
	bool operator!= (const string& lhs, const char*   rhs){}
	bool operator<  (const string& lhs, const string& rhs){}
	bool operator<  (const char*   lhs, const string& rhs){}
	bool operator<  (const string& lhs, const char*   rhs){}
	bool operator<= (const string& lhs, const string& rhs){}
	bool operator<= (const char*   lhs, const string& rhs){}
	bool operator<= (const string& lhs, const char*   rhs){}
	bool operator>  (const string& lhs, const string& rhs){}
	bool operator>  (const char*   lhs, const string& rhs){}
	bool operator>  (const string& lhs, const char*   rhs){}
	bool operator>= (const string& lhs, const string& rhs){}
	bool operator>= (const char*   lhs, const string& rhs){}
	bool operator>= (const string& lhs, const char*   rhs){}


	void swap(string& x, string& y)
	{
		std::swap(x._begin, y._begin);
		std::swap(x._end, y._end);
		std::swap(x._cap, y._cap);
	}
}

#endif