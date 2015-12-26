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
			_reallocate();
	}

	void string::_reallocate()
	{
		auto new_cap = size() ? 2 * size() : 1;
		auto data = alloc.allocate(new_cap);
		auto _elem = _begin;
		auto _dest = data;
		for (size_type i = 0; i < size(); ++i)
			alloc.construct(_dest++, std::move(*_elem++));
		_free();
		_begin = data;
		_end = _dest;
		_cap = _begin + new_cap;
	}


	string::size_type string::strlen(const char* s)
	{
		size_type len = 0;
		for (auto p = s; p != nullptr; ++p, ++len);
		return len;
	}


	void string::_free()
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

	void string::alloc_n_fill_n(const char &c, size_type n)
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

	string::string(const string &str, size_type pos, size_type len = npos)
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

	string::string(const char* s, size_type n)
	{
		alloc_n_copy(s, s + n);
	}

	string::string(size_type n, char c)
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

	string::string(string &&str) /*noexcept*/
		: _begin(str._begin), _end(str._end), _cap(str._cap)
	{
		str._begin = str._end = str._cap = nullptr;
	}


	string::~string()
	{
		_free();
	}


	//////////////////// string assignment ////////////////////
	// operator =
	string& string::operator= (const string& str)
	{
		if (this != &str)
		{
			_free();
			alloc_n_copy(str.begin(), str.end());
		}
		return *this;
	}

	string& string::operator= (const char* s)
	{
		_free();
		alloc_n_copy(s, s + strlen(s));
		return *this;
	}

	string& string::operator= (char c)
	{
		_free();
		alloc_n_fill_n(c, 1);
		return *this;
	}

	string& string::operator= (std::initializer_list<char> il)
	{
		_free();
		alloc_n_copy(il.begin(), il.end());
		return *this;
	}

	string& string::operator= (string&& str) /*noexcept*/
	{
		if (this != &str)
		{
			_free();
			_begin = str._begin;
			_end = str._end;
			_cap = str._cap;

			str._begin = str._end = str._cap = nullptr;
		}
		return *this;
	}


	//////////////////// capacity ////////////////////
	void string::resize(size_type n)
	{
		resize(n, value_type());  // value_type() ??
	}

	void string::resize(size_type n, char c)
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
			_free();
			_begin = start;
			_end = _cap = finish;
		}
	}

	void string::reserve(size_type n = 0)
	{
		if (n <= capacity())
			return;
		iterator start = alloc.allocate(n);
		iterator finish = std::uninitialized_copy(std::make_move_iterator(_begin), std::make_move_iterator(_end), start);
		_free();
		_begin = start;
		_end = finish;
		_cap = start + n;
	}

	void string::clear()  /*noexcept*/
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
	char& string::at(size_type pos)
	{
		if (pos > size())
			throw std::out_of_range("out of range!");
		return *(_begin + pos);
	}

	const char& string::at(size_type pos) const
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

	string& string::append(const string& str, size_type subpos, size_type sublen)
	{
		return insert(size(), str, subpos, sublen);
	}

	string& string::append(const char* s)
	{
		return insert(size(), s);
	}

	string& string::append(const char* s, size_type n)
	{
		return insert(size(), s, n);
	}

	string& string::append(size_type n, char c)
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

	string& string::assign(const string& str, size_type subpos, size_type sublen)
	{
		return assign(str.begin() + subpos, str.begin() + subpos + sublen);
	}

	string& string::assign(const char* s)
	{
		return assign(s, s + strlen(s));
	}

	string& string::assign(const char* s, size_type n)
	{
		return assign(s, s + n);
	}

	string& string::assign(size_type n, char c)
	{
		if (n <= capacity())
		{
			iterator finish = std::uninitialized_fill_n(_begin, n, c);
			_end = n <= size() ? _end : finish;
		}
		else
		{
			_free();
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
			_free();
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

	string& string::assign(string&& str) /*noexcept*/
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
	string& string::insert(size_type pos, const string& str)
	{
		insert(_begin + pos, str.begin(), str.end());
		return *this;
	}

	string& string::insert(size_type pos, const string& str, size_type subpos, size_type sublen)
	{
		insert(_begin + pos, str.begin() + subpos, str.begin() + subpos + sublen);
		return *this;
	}

	string& string::insert(size_type pos, const char* s)
	{
		insert(_begin + pos, s, s + strlen(s));
		return *this;
	}

	string& string::insert(size_type pos, const char* s, size_type n)
	{
		insert(_begin + pos, s, s + n);
		return *this;
	}

	string& string::insert(size_type pos, size_type n, char c)
	{
		insert(_begin + pos, n, c);
		return *this;
	}

	// insert auxiliary: fill n
	string::iterator string::insert(const_iterator p, size_type n, char c)
	{
		if (p < _begin || p > _end)
			throw std::out_of_range("out of range");
		if (!n)
			return const_cast<iterator>(p);

		size_type space_left = _cap - _end;

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
			_free();
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
			auto newcap = capacity() > space_required ? 2 * capacity() : capacity() + space_required;
			iterator start = alloc.allocate(newcap);
			iterator finish = std::uninitialized_copy(
				std::make_move_iterator(_begin), std::make_move_iterator(p), start);
			res = finish = std::uninitialized_copy(std::make_move_iterator(first), std::make_move_iterator(last), finish);
			finish = std::uninitialized_copy(std::make_move_iterator(p), std::make_move_iterator(_end), finish);
			_free();
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
	string& string::erase(size_type pos = 0, size_type len = npos)
	{
		return erase(begin() + pos, begin() + pos + len);
	}

	string::iterator string::erase(const_iterator p)
	{
		return erase(p, p + 1);
	}

	string::iterator string::erase(const_iterator first, const_iterator last)
	{
		/*if (first < _begin || first > _end ||
			last < _begin || last > _end )
			throw std::out_of_range("out of range");
		if (first > last)
			throw std::invalid_argument("invalid argument");
		if (first == last)
			return const_cast<iterator>(first);*/

		iterator p = begin() + static_cast<size_type>(last - begin());
		std::uninitialized_copy(p, _end, first);

		difference_type len = last - first;
		const_iterator stop = _end - len;
		for (; _end != stop; --_end)
			alloc.destroy(_end);

		return const_cast<iterator>(first);
	}


	// replace
	string& string::replace(size_type pos, size_type len, const string& str)
	{
		return replace(_begin + pos, _begin + pos + len, str.begin(), str.end());
	}

	string& string::replace(const_iterator i1, const_iterator i2, const string& str)
	{
		return replace(i1, i2, str.begin(), str.end());
	}

	string& string::replace(size_type pos, size_type len, const string& str, size_type subpos, size_type sublen)
	{
		return replace(_begin + pos, _begin + pos + len, str.begin() + subpos, str.begin() + subpos + sublen);
	}

	string&	string::replace(size_type pos, size_type len, const char* s)
	{
		return replace(_begin + pos, _begin + pos + len, s, s + strlen(s));
	}

	string& string::replace(const_iterator i1, const_iterator i2, const char* s)
	{
		return replace(i1, i2, s, s + strlen(s));
	}

	string& string::replace(size_type pos, size_type len, const char* s, size_type n)
	{
		return replace(_begin + pos, _begin + pos + len, s, s + n);
	}

	string& string::replace(const_iterator i1, const_iterator i2, const char* s, size_type n)
	{
		return replace(i1, i2, s, s + n);
	}

	string& string::replace(size_type pos, size_type len, size_type n, char c)
	{
		return replace(_begin + pos, _begin + pos + len, n, c);
	}

	// replace auxiliary: fill
	string& string::replace(const_iterator i1, const_iterator i2, size_type n, char c)
	{
		iterator finish = erase(const_cast<iterator>(i1), const_cast<iterator>(i2));
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
		{
			using std::swap;
			swap(_begin, str._begin);
			swap(_end, str._end);
			swap(_cap, str._end);
		}
	}


	// pop_back
	void string::pop_back()
	{
		if (_begin)
			alloc.destroy(--_end);
	}



	//////////////////// string operations ////////////////////
	const char* string::c_str() const /*noexcept*/
	{
		char *res = new char(size() + 1);
		char *finish = std::uninitialized_copy(_begin, _end, res);
		*finish = '\0';
		return res;
	}

	const char* string::data() const /*noexcept*/
	{
		return c_str();
	}


	string::size_type string::copy(char* s, size_type len, size_type pos = 0) const
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
	string::size_type string::find(const string& str, size_type pos = 0) const /*noexcept*/
	{
		return find(str.begin(), pos, str.size());
	}

	string::size_type string::find(const char* s, size_type pos = 0) const
	{
		return find(s, pos, strlen(s));
	}

	// find auxiliary: buffer
	string::size_type string::find(const char* s, size_type pos, size_type n) const
	{

	}

	string::size_type string::find(char c, size_type pos = 0) const /*noexcept*/
	{
		for (auto cit = cbegin() + pos; cit != cend(); ++cit)
		{
			if (*cit == c)
				return cit - cbegin();
		}
		return npos; // -1
	}


	// rfind
	string::size_type string::rfind(const string& str, size_type pos = npos) const /*noexcept*/
	{

	}

	string::size_type string::rfind(const char* s, size_type pos = npos) const
	{
		return rfind(s, pos, strlen(s));
	}

	// rfind auxiliary: buffer
	string::size_type string::rfind(const char* s, size_type pos, size_type n) const
	{

	}

	string::size_type string::rfind(char c, size_type pos = npos) const /*noexcept*/
	{
		for (auto cit = cbegin() + pos; cit >= cbegin(); --cit)
		{
			if (*cit == c)
				return cit - cbegin();
		}
		return npos;
	}


	// find_first_of
	string::size_type string::find_first_of(const string& str, size_type pos = 0) const /*noexcept*/
	{

	}

	string::size_type string::find_first_of(const char* s, size_type pos = 0) const
	{
		return find_first_of(s, pos, strlen(s));
	}

	string::size_type string::find_first_of(const char* s, size_type pos, size_type n) const
	{

	}

	string::size_type string::find_first_of(char c, size_type pos = 0) const /*noexcept*/
	{
		return find(c, pos);
	}


	// find_last_of
	string::size_type string::find_last_of(const string& str, size_type pos = npos) const /*noexcept*/
	{

	}

	string::size_type string::find_last_of(const char* s, size_type pos = npos) const
	{
		return find_last_of(s, pos, strlen(s));
	}

	string::size_type string::find_last_of(const char* s, size_type pos, size_type n) const
	{

	}

	string::size_type string::find_last_of(char c, size_type pos = npos) const /*noexcept*/
	{
		return rfind(c, pos);
	}


	// find_first_not_of
	string::size_type string::find_first_not_of(const string& str, size_type pos = 0) const /*noexcept*/
	{

	}

	string::size_type string::find_first_not_of(const char* s, size_type pos = 0) const
	{
		return find_first_not_of(s, pos, strlen(s));
	}

	string::size_type string::find_first_not_of(const char* s, size_type pos, size_type n) const
	{
		
	}

	string::size_type string::find_first_not_of(char c, size_type pos = 0) const /*noexcept*/
	{
		for (auto i = pos; i != size(); ++i)
		{
			if (*(cbegin() + i) != c)
				return i;
		}
		return npos;
	}


	// find_last_not_of
	string::size_type string::find_last_not_of(const string& str, size_type pos = npos) const /*noexcept*/
	{

	}

	string::size_type string::find_last_not_of(const char* s, size_type pos = npos) const
	{
		return find_last_not_of(s, pos, strlen(s));
	}

	string::size_type string::find_last_not_of(const char* s, size_type pos, size_type n) const
	{

	}

	string::size_type string::find_last_not_of(char c, size_type pos = npos) const /*noexcept*/
	{
		for (auto i = pos; i >= 0; --i)
		{
			if (*(cbegin() + i) != c)
				return i;
		}
		return npos;
	}


	string string::substr(size_type pos = 0, size_type len = npos) const
	{
		string res(*this, pos, len);
		return res;
	}


	/* compare
	 * PARAMETERS
	 *	@str		another string object, used entirely (or partially) as the \comparing string
	 *	@s			pointer to an array of characters
	 *				(if argument @n is specified, the first n characters in the array are used as the \comparing string)
	 *	@pos		position of the first character in the \compared string
	 *	 @len		length of \compared string (if the string is shorter, as many characters as possible)
	 *	@subpos, sublen
	 *				same as @pos and @len above, but for \comparing string
	 *	@n			number of characters to compare
	 * RETURN VALUES
	 *	
	*/
	int string::compare(const string& str) const /*noexcept*/
	{
		return compare(0, size(), str, 0, str.size());
	}

	int string::compare(size_type pos, size_type len, const string& str) const
	{
		return compare(pos, len, str, 0, str.size());
	}

	// compare auxiliary: string object
	int string::compare(size_type pos, size_type len, const string &str, size_type subpos, size_type sublen) const
	{
		return _compare(pos, len, str.begin() + subpos, str.begin() + subpos + sublen);
	}

	int string::compare(const char* s) const
	{
		return compare(0, size(), s, strlen(s));
	}

	int string::compare(size_type pos, size_type len, const char* s) const
	{
		return compare(pos, len, s, strlen(s));
	}

	// compare auxiliary: buffer
	int string::compare(size_type pos, size_type len, const char* s, size_type n) const
	{
		return _compare(pos, len, s, s + n);
	}

	// compare auxiliary
	template <typename InputIterator>
	int string::_compare(size_type pos, size_type len, InputIterator first, InputIterator last)
	{
		InputIterator it = first;
		for (auto i = pos; i < pos + len && it != last; ++i, ++it)
		{
			if (*(begin() + pos) > *it)
				return 1;
			if (*(begin() + pos) < *it)
				return -1;
		}
		return 0;
	}




	//////////////////// non-member functions overloads ////////////////////
	// operator +
	// can be implemented by insert, append, +=
	string operator+ (const string& lhs, const string& rhs)
	{
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
		if (lhs.size() != rhs.size())
			return false;
		for (auto i = 0; i < lhs.size(); ++i)
		{
			if (lhs[i] != rhs[i])
				return false;
		}
		return true;
	}

	bool operator==(const char* lhs, const string& rhs)
	{
		const size_t len = strlen(lhs);
		if (len != rhs.size())
			return false;
		for (auto i = 0; i < len; ++i )
		{
			if (*(lhs + i) != rhs[i])
				return false;
		}
		return true;
	}

	bool operator==(const string& lhs, const char* rhs)
	{
		return rhs == lhs;
	}

	bool operator!=(const string& lhs, const string& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator!=(const char* lhs, const string& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator!=(const string& lhs, const char* rhs)
	{
		return !(lhs == rhs);
	}

	bool operator<(const string& lhs, const string& rhs)
	{
		for (auto i = 0; i < lhs.size() && i < rhs.size(); ++i)
		{
			if (!(lhs[i] < rhs[i]))
				return false;
		}
		return true;
	}

	bool operator<(const char* lhs, const string& rhs)
	{
		const size_t len = strlen(lhs);
		for (auto i = 0; i < len && i < rhs.size(); ++i)
		{
			if (!(*(lhs + i) < rhs[i]))
				return false;
		}
		return true;
	}

	bool operator<(const string& lhs, const char* rhs)
	{
		return !(rhs < lhs);
	}

	bool operator<=(const string& lhs, const string& rhs)
	{
		return lhs < rhs || lhs == rhs;
	}

	bool operator<=(const char* lhs, const string& rhs)
	{
		return lhs < rhs || lhs == rhs;
	}

	bool operator<=(const string& lhs, const char* rhs)
	{
		return lhs < rhs || lhs == rhs;
	}

	bool operator>(const string& lhs, const string& rhs)
	{
		return !(lhs <= rhs);
	}

	bool operator>(const char* lhs, const string& rhs)
	{
		return !(lhs <= rhs);
	}

	bool operator>(const string& lhs, const char* rhs)
	{
		return !(lhs <= rhs);
	}

	bool operator>=(const string& lhs, const string& rhs)
	{
		return !(lhs < rhs);
	}

	bool operator>=(const char* lhs, const string& rhs)
	{
		return !(lhs < rhs);
	}

	bool operator>=(const string& lhs, const char* rhs)
	{
		return !(lhs < rhs);
	}

	void swap(string& x, string& y)
	{
		x.swap(y);
	}
}
