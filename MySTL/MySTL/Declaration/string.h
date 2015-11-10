#ifndef INCLUDED_STRING_H
#define INCLUDED_STRING_H

// reference:
// http://www.cplusplus.com/reference/string/string/?kw=string

#include <memory>		// allocator, uninitialized_copy, uninitialized_fill_n
#include <type_traits>
#include <initializer_list>
#include <iterator>		// reverse_iterator
#include <iostream>		// std::istream, std::ostream
#include <cstddef>		// size_t, ptrdiff_t
#include <climits>      // UINT_MAX

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
		string(const string &str, std::size_t pos, std::size_t len = npos);	// (3) substring constructor
		string(const char* s);												// (4) from c-string
		string(const char* s, std::size_t n);								// (5) from buffer
		string(std::size_t n, char c);										// (6) fill constructor
		template <typename InputIterator>
		string(InputIterator first, InputIterator last);					// (7) range constructor
		string(std::initializer_list<char> il);								// (8) initializer list
		string(string &&str) noexcept;										// (9) move constructor



		//////////////////// destructor ////////////////////
		~string();



		//////////////////// string assignment ////////////////////
		// Assigns a new value to the string, replacing its current contents.
		string& operator= (const string& str);				// string
		string& operator= (const char* s);					// c-string
		string& operator= (char c);							// character
		string& operator= (std::initializer_list<char> il);	// initializer list
		string& operator= (string&& str) noexcept;			// move



		//////////////////// iterators ////////////////////
		iterator begin() { return _begin; } // noexcept;
		//const_iterator begin() const { return _begin; }// noexcept;
		iterator end() { return _end; } // noexcept;
		//const_iterator end() const { return _end; }// noexcept;
		reverse_iterator rbegin() { return reverse_iterator(_begin); }//noexcept;
		//const_reverse_iterator rbegin() const { return const_reverse_iterator(_begin); } // noexcept;
		reverse_iterator rend() { return reverse_iterator(_begin); } // noexcept;
		//const_reverse_iterator rend() const { return const_reverse_iterator(_end); } // noexcept;
		const_iterator cbegin() const { return _begin; } // noexcept;
		const_iterator cend() const { return _end; } // noexcept;
		const_reverse_iterator crbegin() const { return const_reverse_iterator(_begin); } // noexcept;
		const_reverse_iterator crend() const { return const_reverse_iterator(_end); } // noexcept;



		//////////////////// capacity ////////////////////

		std::size_t size() const { return _end - _begin; }// noexcept;

		std::size_t length() const { return _end - _begin; } // noexcept;

		std::size_t max_size() const { return size_type(UINT_MAX / sizeof(char)); }//noexcept;

		void resize(std::size_t n);
		void resize(std::size_t n, char c);

		std::size_t capacity() const { return _cap - _begin; }// noexcept;

		void reserve(std::size_t n = 0);

		void clear() noexcept;

		bool empty() const { return _begin == _end; }//noexcept;

		void shrink_to_fit();



		//////////////////// element access ////////////////////

		char& operator[] (std::size_t pos) { return *(_begin + pos); }
		const char& operator[] (std::size_t pos) const { return *(_begin + pos); }

		char& at(std::size_t pos);
		const char& at(std::size_t pos) const;

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
		string& append(const string& str);											// (1) string
		string& append(const string& str, std::size_t subpos, std::size_t sublen);	// (2) substring
		string& append(const char* s);												// (3) c - string
		string& append(const char* s, std::size_t n);								// (4) buffer
		string& append(std::size_t n, char c);										// (5) fill
		template <typename InputIterator>
		string& append(InputIterator first, InputIterator last);					// (6) range
		string& append(std::initializer_list<char> il);								// (7) initializer list


		// Append character to string
		//	Appends character c to the end of the string, increasing its length by one.
		void push_back(char c);


		// Assign content to string
		//	Assigns a new value to the string, replacing its current contents.
		string& assign(const string& str);											// (1) string
		string& assign(const string& str, std::size_t subpos, std::size_t sublen);	// (2) substring
		string& assign(const char* s);												// (3) c - string
		string& assign(const char* s, std::size_t n);								// (4) buffer
		string& assign(std::size_t n, char c);										// (5) fill
		template <typename InputIterator>
		string& assign(InputIterator first, InputIterator last);					// (6) range
		string& assign(std::initializer_list<char> il);								// (7) initializer list
		string& assign(string&& str); //noexcept;									// (8) move


		// Insert into string
		//	Inserts additional characters into the string right before the character indicated by pos(or p)
		string& insert(std::size_t pos, const string& str);											//	(1) string
		string& insert(std::size_t pos, const string& str, std::size_t subpos, std::size_t sublen);	//	(2) substring
		string& insert(std::size_t pos, const char* s);												//	(3) c - string
		string& insert(std::size_t pos, const char* s, std::size_t n);								//	(4) buffer
		string& insert(std::size_t pos, std::size_t n, char c);										//	(5) fill
		iterator insert(const_iterator p, std::size_t n, char c);
		iterator insert(const_iterator p, char c);													//	(6) single character
		template <typename InputIterator>
		iterator insert(iterator p, InputIterator first, InputIterator last);						//	(7) range
		string& insert(const_iterator p, std::initializer_list<char> il);							//	(8) initializer list


		//Erase characters from string
		//	Erases part of the string, reducing its length :
		string& erase(std::size_t pos = 0, std::size_t len = npos);	//  (1) sequence
		iterator erase(const_iterator p);							//	(2) character
		iterator erase(const_iterator first, const_iterator last);	//	(3) range


		// Replace portion of string
		//	Replaces the portion of the string that begins at character pos and spans len characters
		//  (or the part of the string in the range between[i1, i2)) by new contents
		string& replace(std::size_t pos, std::size_t len, const string& str);							//  (1) string
		string& replace(const_iterator i1, const_iterator i2, const string& str);
		string& replace(std::size_t pos, std::size_t len, const string& str, std::size_t subpos, std::size_t sublen);//	(2) substring
		string& replace(std::size_t pos, std::size_t len, const char* s);								//	(3) c - string
		string& replace(const_iterator i1, const_iterator i2, const char* s);
		string& replace(std::size_t pos, std::size_t len, const char* s, std::size_t n);				//	(4) buffer
		string& replace(const_iterator i1, const_iterator i2, const char* s, std::size_t n);
		string& replace(std::size_t pos, std::size_t len, std::size_t n, char c);						//	(5) fill
		string& replace(const_iterator i1, const_iterator i2, std::size_t n, char c);
		template <typename InputIterator>
		string& replace(const_iterator i1, const_iterator i2, InputIterator first, InputIterator last);	//	(6) range
		string& replace(const_iterator i1, const_iterator i2, std::initializer_list<char> il);			//	(7) initializer list


		void swap(string& str);

		void pop_back();



		//////////////////// string operations ////////////////////

		const char* c_str() const noexcept;

		const char* data() const noexcept;

		allocator_type get_allocator() const noexcept;

		std::size_t copy(char* s, std::size_t len, std::size_t pos = 0) const;

		//	Find content in string
		//	Searches the string for the first occurrence of the sequence specified by its arguments.
		std::size_t find(const string& str, std::size_t pos = 0) const noexcept;			// string(1)
		std::size_t find(const char* s, std::size_t pos = 0) const;							// c - string(2)
		std::size_t find(const char* s, std::size_t pos, std::size_t n) const;				// buffer(3)
		std::size_t find(char c, std::size_t pos = 0) const noexcept;						// character(4)

		//  Find last occurrence of content in string
		//	Searches the string for the last occurrence of the sequence specified by its arguments.
		std::size_t rfind(const string& str, std::size_t pos = npos) const noexcept;		// string(1)
		std::size_t rfind(const char* s, std::size_t pos = npos) const;						// c - string(2)
		std::size_t rfind(const char* s, std::size_t pos, std::size_t n) const;				// buffer(3)
		std::size_t rfind(char c, std::size_t pos = npos) const noexcept;					// character(4)

		// Find character in string
		// Searches the string for the first character that matches any of the characters specified in its arguments.
		std::size_t find_first_of(const string& str, std::size_t pos = 0) const noexcept;	// string(1)
		std::size_t find_first_of(const char* s, std::size_t pos = 0) const;				// c - string(2)
		std::size_t find_first_of(const char* s, std::size_t pos, std::size_t n) const;		// buffer(3)
		std::size_t find_first_of(char c, std::size_t pos = 0) const noexcept;				// character(4)

		// Find character in string from the end
		// Searches the string for the last character that matches any of the characters specified in its arguments.
		std::size_t find_last_of(const string& str, std::size_t pos = npos) const noexcept;	// string(1)
		std::size_t find_last_of(const char* s, std::size_t pos = npos) const;				// c - string(2)
		std::size_t find_last_of(const char* s, std::size_t pos, std::size_t n) const;		// buffer(3)
		std::size_t find_last_of(char c, std::size_t pos = npos) const noexcept;			// character(4)

		// Find absence of character in string
		// Searches the string for the first character that does not match any of the characters specified in its arguments.
		std::size_t find_first_not_of(const string& str, std::size_t pos = 0) const noexcept;// string(1)
		std::size_t find_first_not_of(const char* s, std::size_t pos = 0) const;			// c - string(2)
		std::size_t find_first_not_of(const char* s, std::size_t pos, std::size_t n) const;	// buffer(3)
		std::size_t find_first_not_of(char c, std::size_t pos = 0) const noexcept;			// character(4)

		// Find non-matching character in string from the end
		// Searches the string for the last character that does not match any of the characters specified in its arguments.
		std::size_t find_last_not_of(const string& str, std::size_t pos = npos) const noexcept;// string(1)
		std::size_t find_last_not_of(const char* s, std::size_t pos = npos) const;			// c - string(2)
		std::size_t find_last_not_of(const char* s, std::size_t pos, std::size_t n) const;	// buffer(3)
		std::size_t find_last_not_of(char c, std::size_t pos = npos) const noexcept;		// character(4)

		// Generate substring
		//	Returns a newly constructed string object with its value initialized to a copy of a substring of this object.
		string substr(std::size_t pos = 0, std::size_t len = npos) const;

		// Compare strings
		//	Compares the value of the string object(or a substring) to the sequence of characters specified by its arguments.
		int compare(const string& str) const noexcept;										// string(1)
		int compare(std::size_t pos, std::size_t len, const string& str) const;				// string(1)
		int compare(std::size_t pos, std::size_t len, const string& str, std::size_t subpos, std::size_t sublen) const;// substrings(2)
		int compare(const char* s) const;													// c - string(3)
		int compare(std::size_t pos, std::size_t len, const char* s) const;					// c - string(3)
		int compare(std::size_t pos, std::size_t len, const char* s, std::size_t n) const;	// buffer(4)



		//////////////////// member constants ////////////////////

	private:
		static const std::size_t npos = -1;

		//char *_begin;
		//char *_end;
		//char *_cap;

		iterator _begin;
		iterator _end;
		iterator _cap;

		std::allocator<char> alloc;

		// check and allocate
		void chk_n_alloc();
		// allocate and copy
		void alloc_n_copy(const_iterator first, const_iterator second);
		// allocate and fill n
		void alloc_n_fill_n(const char &c, std::size_t n);
		// destroy and deallocate
		void free();
		// re allocate
		void reallocate();

		// cstring - strlen
		std::size_t strlen(const char *s) const;
		// compare_aux
		template <typename InputIterator>
		int compare_aux(std::size_t pos, std::size_t len, InputIterator first, InputIterator last);



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

	string operator+ (const string& lhs, const string& rhs);		// string(1)
	string operator+ (string&&      lhs, string&&      rhs);
	string operator+ (string&&      lhs, const string& rhs);
	string operator+ (const string& lhs, string&&      rhs);
	string operator+ (const string& lhs, const char*   rhs);		// c - string(2)
	string operator+ (string&&      lhs, const char*   rhs);
	string operator+ (const char*   lhs, const string& rhs);
	string operator+ (const char*   lhs, string&&      rhs);
	string operator+ (const string& lhs, char          rhs);		// character(3)
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
