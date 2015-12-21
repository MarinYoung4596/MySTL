#ifndef INCLUDED_STRING_H
#define INCLUDED_STRING_H

// reference:
// http://www.cplusplus.com/reference/string/string/?kw=string

#include <memory>		// allocator, uninitialized_copy, uninitialized_fill_n
#include <initializer_list>
#include <iterator>		// reverse_iterator
#include <cstddef>		// size_t, ptrdiff_t
#include <climits>      // UINT_MAX

#include "uninitialized_functions.h"

namespace MySTL
{

	class string
	{
	public:
		typedef char								value_type;
		typedef std::char_traits<char>				traits_type;
		typedef std::allocator<char>				allocator_type;
		typedef char &								reference;
		typedef const char &						const_reference;
		typedef char *								iterator;
		typedef const char *						const_iterator;
		typedef std::reverse_iterator<char *>		reverse_iterator;
		typedef std::reverse_iterator<const char *>	const_reverse_iterator;
		typedef std::ptrdiff_t						difference_type;
		typedef std::size_t							size_type;

	public:
		//////////////////// constructor ////////////////////
		string() : _begin(nullptr), _end(nullptr), _cap(nullptr) {}			// (1) default constructor
		string(const string &str);											// (2) copy constructor
		string(const string &str, size_type pos, size_type len = npos);		// (3) substring constructor
		string(const char* s);												// (4) from c-string
		string(const char* s, size_type n);									// (5) from buffer
		string(size_type n, char c);										// (6) fill constructor
		template <typename InputIterator>
		string(InputIterator first, InputIterator last);					// (7) range constructor
		string(std::initializer_list<char> il);								// (8) initializer list
		string(string &&str) /*noexcept*/;									// (9) move constructor

		~string();



		//////////////////// string assignment ////////////////////
		// Assigns a new value to the string, replacing its current contents.
		string& operator= (const string& str);				// string
		string& operator= (const char* s);					// c-string
		string& operator= (char c);							// character
		string& operator= (std::initializer_list<char> il);	// initializer list
		string& operator= (string&& str) /*noexcept*/;		// move



		//////////////////// iterators ////////////////////
		iterator begin() const/*noexcept*/ { return _begin; }
		iterator end() const/*noexcept*/ { return _end; }
		reverse_iterator rbegin() const/*noexcept*/ { return reverse_iterator(_begin); }
		reverse_iterator rend() const/*noexcept*/ { return reverse_iterator(_begin); }
		const_iterator cbegin() const /*noexcept*/ { return _begin; }
		const_iterator cend() const /*noexcept*/ { return _end; }
		const_reverse_iterator crbegin() const /*noexcept*/ { return const_reverse_iterator(_begin); }
		const_reverse_iterator crend() const /*noexcept*/ { return const_reverse_iterator(_end); }



		//////////////////// capacity ////////////////////
		size_type size() const /*noexcept*/ { return _end - _begin; }
		size_type length() const /*noexcept*/ { return _end - _begin; }
		static size_type max_size() /*noexcept*/ { return size_type(UINT_MAX / sizeof(char)); }

		void resize(size_type n);
		void resize(size_type n, char c);

		size_type capacity() const /*noexcept*/ { return _cap - _begin; }
		void reserve(size_type n = 0);
		void clear() /*noexcept*/;
		bool empty() const /*noexcept*/ { return _begin == _end; }
		void shrink_to_fit();



		//////////////////// element access ////////////////////

		char& operator[] (size_type pos) { return *(_begin + pos); }
		const char& operator[] (size_type pos) const { return *(_begin + pos); }

		char& at(size_type pos);
		const char& at(size_type pos) const;

		char& back() { return *(_end - 1); }
		const char& back() const { return *(_end - 1); }

		char& front() { return *_begin; }
		const char& front() const { return *_begin; }



		//////////////////// modifiers ////////////////////

		// Append to string
		// Extends the string by appending additional characters at the end of its current value :
		string& operator+= (const string& str);					// string(1)
		string& operator+= (const char* s);						// c - string(2)
		string& operator+= (char c);							// character(3)
		string& operator+= (std::initializer_list<char> il);	// initializer list(4)

		//Append to string
		//	Extends the string by appending additional characters at the end of its current value :
		string& append(const string& str);										// (1) string
		string& append(const string& str, size_type subpos, size_type sublen);	// (2) substring
		string& append(const char* s);											// (3) c - string
		string& append(const char* s, size_type n);								// (4) buffer
		string& append(size_type n, char c);									// (5) fill
		template <typename InputIterator>
		string& append(InputIterator first, InputIterator last);				// (6) range
		string& append(std::initializer_list<char> il);							// (7) initializer list


		// Assign content to string
		//	Assigns a new value to the string, replacing its current contents.
		string& assign(const string& str);										// (1) string
		string& assign(const string& str, size_type subpos, size_type sublen);	// (2) substring
		string& assign(const char* s);											// (3) c - string
		string& assign(const char* s, size_type n);								// (4) buffer
		string& assign(size_type n, char c);									// (5) fill
		template <typename InputIterator>
		string& assign(InputIterator first, InputIterator last);				// (6) range
		string& assign(std::initializer_list<char> il);							// (7) initializer list
		string& assign(string&& str); ///*noexcept*/;							// (8) move


		// Insert into string
		//	Inserts additional characters into the string right before the character indicated by pos(or p)
		string& insert(size_type pos, const string& str);						//	(1) string
		string& insert(size_type pos, const string& str, size_type subpos, size_type sublen);	//	(2) substring
		string& insert(size_type pos, const char* s);							//	(3) c - string
		string& insert(size_type pos, const char* s, size_type n);				//	(4) buffer
		string& insert(size_type pos, size_type n, char c);						//	(5) fill
		iterator insert(const_iterator p, size_type n, char c);
		iterator insert(const_iterator p, char c);								//	(6) single character
		template <typename InputIterator>
		iterator insert(iterator p, InputIterator first, InputIterator last);	//	(7) range
		string& insert(const_iterator p, std::initializer_list<char> il);		//	(8) initializer list


		//Erase characters from string
		//	Erases part of the string, reducing its length :
		string& erase(size_type pos = 0, size_type len = npos);		//  (1) sequence
		iterator erase(iterator p);									//	(2) character
		iterator erase(iterator first, iterator last);				//	(3) range


		// Replace portion of string
		//	Replaces the portion of the string that begins at character pos and spans len characters
		//  (or the part of the string in the range between[i1, i2)) by new contents
		string& replace(size_type pos, size_type len, const string& str);				//  (1) string
		string& replace(const_iterator i1, const_iterator i2, const string& str);
		string& replace(size_type pos, size_type len, const string& str, size_type subpos, size_type sublen);//	(2) substring
		string& replace(size_type pos, size_type len, const char* s);					//	(3) c - string
		string& replace(const_iterator i1, const_iterator i2, const char* s);
		string& replace(size_type pos, size_type len, const char* s, size_type n);		//	(4) buffer
		string& replace(const_iterator i1, const_iterator i2, const char* s, size_type n);
		string& replace(size_type pos, size_type len, size_type n, char c);				//	(5) fill
		string& replace(const_iterator i1, const_iterator i2, size_type n, char c);
		template <typename InputIterator>
		string& replace(const_iterator i1, const_iterator i2, InputIterator first, InputIterator last);	//	(6) range
		string& replace(const_iterator i1, const_iterator i2, std::initializer_list<char> il);	//	(7) initializer list


		void swap(string& str);

		void push_back(char c);
		void pop_back();



		//////////////////// string operations ////////////////////

		const char* c_str() const /*noexcept*/;

		const char* data() const /*noexcept*/;

		allocator_type get_allocator() const /*noexcept*/;

		size_type copy(char* s, size_type len, size_type pos = 0) const;

		//	Find content in string
		//	Searches the string for the first occurrence of the sequence specified by its arguments.
		size_type find(const string& str, size_type pos = 0) const /*noexcept*/;		// string(1)
		size_type find(const char* s, size_type pos = 0) const;							// c - string(2)
		size_type find(const char* s, size_type pos, size_type n) const;				// buffer(3)
		size_type find(char c, size_type pos = 0) const /*noexcept*/;					// character(4)

		//  Find last occurrence of content in string
		//	Searches the string for the last occurrence of the sequence specified by its arguments.
		size_type rfind(const string& str, size_type pos = npos) const /*noexcept*/;	// string(1)
		size_type rfind(const char* s, size_type pos = npos) const;						// c - string(2)
		size_type rfind(const char* s, size_type pos, size_type n) const;				// buffer(3)
		size_type rfind(char c, size_type pos = npos) const /*noexcept*/;				// character(4)

		// Find character in string
		// Searches the string for the first character that matches any of the characters specified in its arguments.
		size_type find_first_of(const string& str, size_type pos = 0) const /*noexcept*/;// string(1)
		size_type find_first_of(const char* s, size_type pos = 0) const;				// c - string(2)
		size_type find_first_of(const char* s, size_type pos, size_type n) const;		// buffer(3)
		size_type find_first_of(char c, size_type pos = 0) const /*noexcept*/;			// character(4)

		// Find character in string from the end
		// Searches the string for the last character that matches any of the characters specified in its arguments.
		size_type find_last_of(const string& str, size_type pos = npos) const /*noexcept*/;	// string(1)
		size_type find_last_of(const char* s, size_type pos = npos) const;				// c - string(2)
		size_type find_last_of(const char* s, size_type pos, size_type n) const;		// buffer(3)
		size_type find_last_of(char c, size_type pos = npos) const /*noexcept*/;		// character(4)

		// Find absence of character in string
		// Searches the string for the first character that does not match any of the characters specified in its arguments.
		size_type find_first_not_of(const string& str, size_type pos = 0) const /*noexcept*/;// string(1)
		size_type find_first_not_of(const char* s, size_type pos = 0) const;			// c - string(2)
		size_type find_first_not_of(const char* s, size_type pos, size_type n) const;	// buffer(3)
		size_type find_first_not_of(char c, size_type pos = 0) const /*noexcept*/;		// character(4)

		// Find non-matching character in string from the end
		// Searches the string for the last character that does not match any of the characters specified in its arguments.
		size_type find_last_not_of(const string& str, size_type pos = npos) const /*noexcept*/;// string(1)
		size_type find_last_not_of(const char* s, size_type pos = npos) const;			// c - string(2)
		size_type find_last_not_of(const char* s, size_type pos, size_type n) const;	// buffer(3)
		size_type find_last_not_of(char c, size_type pos = npos) const /*noexcept*/;	// character(4)

		// Generate substring
		//	Returns a newly constructed string object with its value initialized to a copy of a substring of this object.
		string substr(size_type pos = 0, size_type len = npos) const;

		// Compare strings
		//	Compares the value of the string object(or a substring) to the sequence of characters specified by its arguments.
		int compare(const string& str) const /*noexcept*/;								// string(1)
		int compare(size_type pos, size_type len, const string& str) const;				// string(1)
		int compare(size_type pos, size_type len, const string& str, size_type subpos, size_type sublen) const;// substrings(2)
		int compare(const char* s) const;												// c - string(3)
		int compare(size_type pos, size_type len, const char* s) const;					// c - string(3)
		int compare(size_type pos, size_type len, const char* s, size_type n) const;	// buffer(4)



		//////////////////// member constants ////////////////////

	private:
		static const size_type npos = -1;

		iterator _begin;
		iterator _end;
		iterator _cap;

		std::allocator<char> alloc;

		void chk_n_alloc();
		void alloc_n_copy(const_iterator first, const_iterator second);
		void alloc_n_fill_n(const char &c, size_type n);
		void _free();
		void _reallocate();

		static size_type strlen(const char *s);

		template <typename InputIterator>
		int _compare_aux(size_type pos, size_type len, InputIterator first, InputIterator last);


	public:
		//////////////////// non-member functions overloads ////////////////////

		// Concatenate strings
		// Returns a newly constructed string object with its value being the concatenation of the
		//	characters in lhs followed by those of rhs.
		friend string operator+ (const string& lhs, const string& rhs);		// string(1)
		friend string operator+ (string&&      lhs, string&&      rhs);
		friend string operator+ (string&&      lhs, const string& rhs);
		friend string operator+ (const string& lhs, string&&      rhs);
		friend string operator+ (const string& lhs, const char*   rhs);		// c - string(2)
		friend string operator+ (string&&      lhs, const char*   rhs);
		friend string operator+ (const char*   lhs, const string& rhs);
		friend string operator+ (const char*   lhs, string&&      rhs);
		friend string operator+ (const string& lhs, char          rhs);		// character(3)
		friend string operator+ (string&&      lhs, char          rhs);
		friend string operator+ (char          lhs, const string& rhs);
		friend string operator+ (char          lhs, string&&      rhs);


		// Relational operators for string
		// Performs the appropriate comparison operation between the string objects lhs and rhs.
		friend bool operator== (const string& lhs, const string& rhs);		// (1)
		friend bool operator== (const char*   lhs, const string& rhs);
		friend bool operator== (const string& lhs, const char*   rhs);
		friend bool operator!= (const string& lhs, const string& rhs);		// (2)
		friend bool operator!= (const char*   lhs, const string& rhs);
		friend bool operator!= (const string& lhs, const char*   rhs);
		friend bool operator<  (const string& lhs, const string& rhs);		// (3)
		friend bool operator<  (const char*   lhs, const string& rhs);
		friend bool operator<  (const string& lhs, const char*   rhs);
		friend bool operator<= (const string& lhs, const string& rhs);		// (4)
		friend bool operator<= (const char*   lhs, const string& rhs);
		friend bool operator<= (const string& lhs, const char*   rhs);
		friend bool operator>  (const string& lhs, const string& rhs);		// (5)
		friend bool operator>  (const char*   lhs, const string& rhs);
		friend bool operator>  (const string& lhs, const char*   rhs);
		friend bool operator>= (const string& lhs, const string& rhs);		// (6)
		friend bool operator>= (const char*   lhs, const string& rhs);
		friend bool operator>= (const string& lhs, const char*   rhs);


		// Exchanges the values of two strings
		//	Exchanges the values of string objects x and y, such that after the call to this function,
		//  the value of x is the one which was on y before the call, and the value of y is that of x.
		friend void swap (string& x, string& y);


		// Extract string from stream
		//	Extracts a string from the input stream is, storing the sequence in str, which is overwritten
		//  (the previous value of str is replaced).
		friend std::istream& operator>> (std::istream& is, string& str);
		// Insert string into stream
		//	Inserts the sequence of characters that conforms value of str into os.
		friend std::ostream& operator<< (std::ostream& os, const string& str);


		// Get line from stream into string
		// Extracts characters from is and stores them into str until the delimitation character delim is found
		//	(or the newline character, '\n', for (2)).
		friend std::istream& getline(std::istream&  is, string& str, char delim);	// (1)
		friend std::istream& getline(std::istream&& is, string& str, char delim);
		friend std::istream& getline(std::istream&  is, string& str);				// (2)
		friend std::istream& getline(std::istream&& is, string& str);
	};

	//////////////////// friend functions' declaration ////////////////////

	string operator+ (const string& lhs, const string& rhs);	// string(1)
	string operator+ (string&&      lhs, string&&      rhs);
	string operator+ (string&&      lhs, const string& rhs);
	string operator+ (const string& lhs, string&&      rhs);
	string operator+ (const string& lhs, const char*   rhs);	// c - string(2)
	string operator+ (string&&      lhs, const char*   rhs);
	string operator+ (const char*   lhs, const string& rhs);
	string operator+ (const char*   lhs, string&&      rhs);
	string operator+ (const string& lhs, char          rhs);	// character(3)
	string operator+ (string&&      lhs, char          rhs);
	string operator+ (char          lhs, const string& rhs);
	string operator+ (char          lhs, string&&      rhs);

	bool operator== (const string& lhs, const string& rhs);		// (1)
	bool operator== (const char*   lhs, const string& rhs);
	bool operator== (const string& lhs, const char*   rhs);
	bool operator!= (const string& lhs, const string& rhs);		// (2)
	bool operator!= (const char*   lhs, const string& rhs);
	bool operator!= (const string& lhs, const char*   rhs);
	bool operator<  (const string& lhs, const string& rhs);		// (3)
	bool operator<  (const char*   lhs, const string& rhs);
	bool operator<  (const string& lhs, const char*   rhs);
	bool operator<= (const string& lhs, const string& rhs);		// (4)
	bool operator<= (const char*   lhs, const string& rhs);
	bool operator<= (const string& lhs, const char*   rhs);
	bool operator>  (const string& lhs, const string& rhs);		// (5)
	bool operator>  (const char*   lhs, const string& rhs);
	bool operator>  (const string& lhs, const char*   rhs);
	bool operator>= (const string& lhs, const string& rhs);		// (6)
	bool operator>= (const char*   lhs, const string& rhs);
	bool operator>= (const string& lhs, const char*   rhs);

	void swap(string& x, string& y);

	std::istream& operator>> (std::istream& is, string& str);
	std::ostream& operator<< (std::ostream& os, const string& str);

	std::istream& getline(std::istream&  is, string& str, char delim);	// (1)
	std::istream& getline(std::istream&& is, string& str, char delim);
	std::istream& getline(std::istream&  is, string& str);				// (2)
	std::istream& getline(std::istream&& is, string& str);
}

#endif	// INCLUDED_STRING_H_
