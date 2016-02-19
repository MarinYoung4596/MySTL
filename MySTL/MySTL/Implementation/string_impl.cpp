#include <utility>     // for std::move()
#include <iterator>    // for make_move_iterator
#include <stdexcept>   // for std::out_of_range, std::invalid_argument

#include "../Declaration/string.h"


namespace MySTL
{
	//////////////////// private functions ////////////////////
	void string::chk_n_alloc()
	{
		if (first_free == end_of_storage)
			_reallocate();
	}

	void string::_reallocate()
	{
		auto new_cap = size() ? 2 * size() : 1;
		auto data = alloc.allocate(new_cap);
		auto _elem = elements_start;
		auto _dest = data;
		for (size_type i = 0; i < size(); ++i)
			alloc.construct(_dest++, std::move(*_elem++));
		_free();
		elements_start = data;
		first_free = _dest;
		end_of_storage = elements_start + new_cap;
	}


	string::size_type string::strlen(const char* s)
	{
		size_type len = 0;
		for (auto p = s; p != nullptr; ++p, ++len);
		return len;
	}


	void string::_free() const
	{
		if (elements_start)
		{
			for (auto p = first_free; p != elements_start;)
				alloc.destroy(--p);
			alloc.deallocate(elements_start, end_of_storage - elements_start);
		}
	}

	using const_iterator = MySTL::string::const_iterator;

	void string::alloc_n_copy(const_iterator first, const_iterator second)
	{
		auto start = alloc.allocate(second - first);
		auto finish = std::uninitialized_copy(first, second, elements_start);

		elements_start = start;
		first_free = end_of_storage = finish;
	}

	void string::alloc_n_fill_n(const char &c, size_type n)
	{
		auto start = alloc.allocate(n);
		auto finish = std::uninitialized_fill_n(elements_start, n, c);

		elements_start = start;
		first_free = end_of_storage = finish;
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
			finish = first_free;
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
		typedef typename std::is_integral<size_type>::type IS_INTEGER;
		_string(n, c, IS_INTEGER());
	}

	void string::_string(size_type n, char c, std::true_type)
	{
		alloc_n_fill_n(c, n);
	}

	template <typename InputIterator>
	string::string(InputIterator first, InputIterator last)
	{
		typedef typename std::is_integral<InputIterator>::type IS_INTEGER;
		_string(first, last, IS_INTEGER());
	}

	template<typename InputIterator>
	void string::_string(InputIterator first, InputIterator last, std::false_type)
	{
		alloc_n_copy(first, last);
	}

	string::string(std::initializer_list<char> il)
	{
		alloc_n_copy(il.begin(), il.end());
	}

	string::string(string &&str) /*noexcept*/
		: elements_start(str.elements_start), first_free(str.first_free), end_of_storage(str.end_of_storage)
	{
		str.elements_start = str.first_free = str.end_of_storage = nullptr;
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
			elements_start = str.elements_start;
			first_free = str.first_free;
			end_of_storage = str.end_of_storage;

			str.elements_start = str.first_free = str.end_of_storage = nullptr;
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
			for (; first_free > elements_start + n;)
				alloc.destroy(--first_free);
		}
		else if (n >= size() && n < capacity())
		{
			auto len_insert = n - size();
			first_free = std::uninitialized_fill_n(first_free, len_insert, c);
		}
		else
		{
			auto len_insert = n - size();
			iterator start = alloc.allocate(n);
			iterator finish = std::uninitialized_copy(std::make_move_iterator(elements_start), std::make_move_iterator(first_free), start);
			finish = std::uninitialized_fill_n(finish, len_insert, c);
			_free();
			elements_start = start;
			first_free = end_of_storage = finish;
		}
	}

	void string::reserve(size_type n = 0)
	{
		if (n <= capacity())
			return;
		iterator start = alloc.allocate(n);
		iterator finish = std::uninitialized_copy(std::make_move_iterator(elements_start), std::make_move_iterator(first_free), start);
		_free();
		elements_start = start;
		first_free = finish;
		end_of_storage = start + n;
	}

	void string::clear()  /*noexcept*/
	{
		if (elements_start)
		{
			while (first_free != elements_start)
				alloc.destroy(--first_free);
		}
	}

	void string::shrink_to_fit()
	{
		alloc.deallocate(first_free, end_of_storage - first_free);
		end_of_storage = first_free;
	}


	//////////////////// element access ////////////////////
	char& string::at(size_type pos)
	{
		if (pos > size())
			throw std::out_of_range("out of range!");
		return *(elements_start + pos);
	}

	const char& string::at(size_type pos) const
	{
		if (pos > size())
			throw std::out_of_range("out of range!");
		return *(elements_start + pos);
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
		insert(first_free, il);
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
		insert(first_free, first, last);
		return *this;
	}

	string& string::append(std::initializer_list<char> il)
	{
		return insert(first_free, il);
	}

	void string::push_back(char c)
	{
		chk_n_alloc();
		alloc.construct(first_free++, c);
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
			iterator finish = std::uninitialized_fill_n(elements_start, n, c);
			first_free = n <= size() ? first_free : finish;
		}
		else
		{
			_free();
			auto newcap = 2 * capacity();
			iterator data = alloc.allocate(newcap);
			first_free = std::uninitialized_fill_n(data, n, c);
			elements_start = data;
			end_of_storage = elements_start + newcap;
		}
		return *this;
	}

	template <typename InputIterator>
	string& string::assign(InputIterator first, InputIterator last)
	{
		difference_type space_required = last - first;
		if (space_required <= capacity())
		{
			iterator finish = std::uninitialized_copy(first, last, elements_start);
			first_free = space_required <= size() ? first_free : finish;
		}
		else
		{
			_free();
			auto newcap = 2 * capacity();
			iterator data = alloc.allocate(newcap);
			first_free = std::uninitialized_copy(first, last, data);
			elements_start = data;
			end_of_storage = elements_start + newcap;
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
			elements_start = str.elements_start;
			first_free = str.first_free;
			end_of_storage = str.end_of_storage;
			str.elements_start = str.first_free = str.end_of_storage = nullptr;
		}
		return *this;
	}


	// insert
	string& string::insert(size_type pos, const string& str)
	{
		insert(elements_start + pos, str.begin(), str.end());
		return *this;
	}

	string& string::insert(size_type pos, const string& str, size_type subpos, size_type sublen)
	{
		insert(elements_start + pos, str.begin() + subpos, str.begin() + subpos + sublen);
		return *this;
	}

	string& string::insert(size_type pos, const char* s)
	{
		insert(elements_start + pos, s, s + strlen(s));
		return *this;
	}

	string& string::insert(size_type pos, const char* s, size_type n)
	{
		insert(elements_start + pos, s, s + n);
		return *this;
	}

	string& string::insert(size_type pos, size_type n, char c)
	{
		insert(elements_start + pos, n, c);
		return *this;
	}

	// insert auxiliary: fill n
	string::iterator string::insert(const_iterator p, size_type n, char c)
	{
		if (p < elements_start || p > first_free)
			throw std::out_of_range("out of range");
		if (!n)
			return const_cast<iterator>(p);

		size_type space_left = end_of_storage - first_free;

		iterator res = nullptr;
		if (n <= space_left)
		{
			for (iterator curr = first_free - 1; curr != p; --curr)
				*(curr + n) = *curr;
			res = std::uninitialized_fill_n(const_cast<iterator>(p), n, c);
			first_free += n;
		}
		else
		{
			auto newcap = capacity() > n ? 2 * capacity() : capacity() + n;
			iterator start = alloc.allocate(newcap);
			iterator finish = std::uninitialized_copy(
				std::make_move_iterator(elements_start),
				std::make_move_iterator(const_cast<iterator>(p)),
				start);
			res = finish = std::uninitialized_fill_n(finish, n, c);
			finish = std::uninitialized_copy(std::make_move_iterator(finish), std::make_move_iterator(first_free), finish);
			_free();
			elements_start = start;
			first_free = finish;
			end_of_storage = elements_start + newcap;
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
		if (p < elements_start || p > first_free)
			throw std::out_of_range("out of range");

		difference_type space_required = last - first;
		if (!space_required)
			return p;

		difference_type space_left = end_of_storage - first_free;
		iterator res = nullptr;

		if (space_required <= space_left)
		{
			for (iterator curr = first_free - 1; curr != p; --curr)
				*(curr + space_required) = *curr;
			res = std::uninitialized_copy(first, last, p);
			first_free += space_required;
		}
		else
		{
			auto newcap = capacity() > space_required ? 2 * capacity() : capacity() + space_required;
			iterator start = alloc.allocate(newcap);
			iterator finish = std::uninitialized_copy(
				std::make_move_iterator(elements_start), std::make_move_iterator(p), start);
			res = finish = std::uninitialized_copy(std::make_move_iterator(first), std::make_move_iterator(last), finish);
			finish = std::uninitialized_copy(std::make_move_iterator(p), std::make_move_iterator(first_free), finish);
			_free();
			elements_start = start;
			first_free = finish;
			end_of_storage = start + newcap;
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
		iterator p = begin() + static_cast<size_type>(last - begin());
		std::uninitialized_copy(p, first_free, first);

		difference_type len = last - first;
		const_iterator stop = first_free - len;
		for (; first_free != stop; --first_free)
			alloc.destroy(first_free);

		return const_cast<iterator>(first);
	}


	// replace
	string& string::replace(size_type pos, size_type len, const string& str)
	{
		return replace(elements_start + pos, elements_start + pos + len, str.begin(), str.end());
	}

	string& string::replace(const_iterator i1, const_iterator i2, const string& str)
	{
		return replace(i1, i2, str.begin(), str.end());
	}

	string& string::replace(size_type pos, size_type len, const string& str, size_type subpos, size_type sublen)
	{
		return replace(elements_start + pos, elements_start + pos + len, str.begin() + subpos, str.begin() + subpos + sublen);
	}

	string&	string::replace(size_type pos, size_type len, const char* s)
	{
		return replace(elements_start + pos, elements_start + pos + len, s, s + strlen(s));
	}

	string& string::replace(const_iterator i1, const_iterator i2, const char* s)
	{
		return replace(i1, i2, s, s + strlen(s));
	}

	string& string::replace(size_type pos, size_type len, const char* s, size_type n)
	{
		return replace(elements_start + pos, elements_start + pos + len, s, s + n);
	}

	string& string::replace(const_iterator i1, const_iterator i2, const char* s, size_type n)
	{
		return replace(i1, i2, s, s + n);
	}

	string& string::replace(size_type pos, size_type len, size_type n, char c)
	{
		return replace(elements_start + pos, elements_start + pos + len, n, c);
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
			swap(elements_start, str.elements_start);
			swap(first_free, str.first_free);
			swap(end_of_storage, str.first_free);
		}
	}


	// pop_back
	void string::pop_back()
	{
		if (elements_start)
			alloc.destroy(--first_free);
	}



	//////////////////// string operations ////////////////////
	const char* string::c_str() const /*noexcept*/
	{
		char *res = new char(size() + 1);
		char *finish = std::uninitialized_copy(elements_start, first_free, res);
		*finish = '\0';
		return res;
	}

	const char* string::data() const /*noexcept*/
	{
		return c_str();
	}


	string::size_type string::copy(char* s, size_type len, size_type pos = 0) const
	{
		auto end_copy = (pos + len) < length() ? (elements_start + pos + len) : first_free;
		auto finish =  std::uninitialized_copy(elements_start + pos, end_copy, s);
		return end_copy == first_free ? length() - pos : len;
	}


	// find
	// @str		another string with the subject to be searched for
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
		auto m = size();
		if (m == 0) return 0;
		if (n == 0 && m == 0) return 0;
		int *next = static_cast<int*>(malloc(sizeof(int) * m));
		int i = 0, j = 0;

		getNextVal(s, pos, n, next);
		for (; i < m && j < n; )
		{
			if (j == -1 || *(begin()+i) == *(s+pos+j))
			{
				++i;
				++j;
			}
			else // j != -1 or match failed
				j = next[j];
		}
		free(next);
		next = nullptr;
		if (j == n) return i - j;
		return -1;
	}

	void string::getNextVal(const char* s, size_type pos, size_type n, int next[])
	{
		next[0] = -1;
		int k = -1, j = 0;
		for (; j < n - 1;)
		{
			if (k == -1 || *(s + pos + j) == *(s + pos + k))
			{
				++j;
				++k;
				next[j] = (*(s+pos+j) != *(s+pos+k)) ? k : next[k];
			}
			else
				k = next[k];
		}
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
		return _rfind(str.begin(), pos, str.size());
	}

	string::size_type string::rfind(const char* s, size_type pos = npos) const
	{
		return rfind(s, pos, strlen(s));
	}

	// rfind auxiliary: buffer
	string::size_type string::rfind(const char* s, size_type pos, size_type n) const
	{
		return _rfind(s, pos, n);
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

	string::size_type string::_rfind(const_iterator cit, size_type pos, size_type n)
	{
		
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
	 *	@len		length of \compared string (if the string is shorter, as many characters as possible)
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
		auto basePos = begin();
		InputIterator it = first;
		for (auto i = pos; i < pos + len && it != last; ++i, ++it)
		{
			if (*(basePos + i) > *it)
				return 1;
			if (*(basePos + i) < *it)
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

	std::istream& operator>> (std::istream& is, string& str)
	{

	}

	std::ostream& operator<< (std::ostream& os, const string& str)
	{
		for (auto it = str.begin(); it != str.end(); ++it)
			os << *it;
		return os;
	}

	std::istream& getline(std::istream&  is, string& str, char delim)
	{
		str.clear();
		char ch;
		while(is.get(ch))
		{
			if (ch == delim) break;
			str.push_back(ch);
		}
		return is;
	}

	//std::istream& getline(std::istream&& is, string& str, char delim){}
	
	std::istream& getline(std::istream&  is, string& str)
	{
		return getline(is, str, '\n');
	}
	
	//std::istream& getline(std::istream&& is, string& str){}
}
