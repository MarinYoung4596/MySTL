#ifndef _STRING_H_
#define _STRING_H_

// reference:
// http://www.cplusplus.com/reference/string/string/?kw=string

#include <memory>		// allocator
#include <type_traits>	
#include <initializer_list>
#include <iterator>		// reverse_iterator

namespace MySTL
{

	class string
	{
	public:
		typedef char							value_type;
		typedef std::char_traits<char>			traits_type;
		typedef std::allocator<char>			allocator_type;
		typedef char &							reference;
		typedef const char &					const_reference;
		typedef char *							iterator;
		typedef const char *					const_iterator;
		typedef std::reverse_iterator<char *>		reverse_iterator;
		typedef std::reverse_iterator<const char *>	const_reverse_iterator;
		typedef std::ptrdiff_t					difference_type;
		typedef std::size_t						size_type;

	public:
		//////////////////// constructor

		// (1) empty string constructor (default constructor)
		// Constructs an empty string, with a length of zero characters.
		string();
		// (2) copy constructor
		// Constructs a copy of str.
		string(const string &str);
		// (3) substring constructor
		// Copies the portion of str that begins at the character position pos and spans len characters
		// (or until the end of str, if either str is too short or if len is string::npos).
		string(const string &str, std::size_t pos, std::size_t len = npos);
		// (4) from c-string
		// Copies the null - terminated character sequence(C - string) pointed by s.
		string(const char* s);
		// (5) from buffer
		//	Copies the first n characters from the array of characters pointed by s.
		string(const char* s, std::size_t n);
		// (6) fill constructor
		// Fills the string with n consecutive copies of character c.
		string(std::size_t n, char c);
		// (7) range constructor
		// Copies the sequence of characters in the range[first, last), in the same order.
		template <typename InputIterator>
		string(InputIterator first, InputIterator last);
		// (8) initializer list
		// Copies each of the characters in il, in the same order.
		string(std::initializer_list<char> il);	
		// (9) move constructor
		//	Acquires the contents of str.
		//	str is left in an unspecified but valid state.
		string(string &&str) noexcept;



		//////////////////// destructor
		// Destroys the string object.
		// This deallocates all the storage capacity allocated by the string using its allocator.
		~string();



		//////////////////// string assignment
		// Assigns a new value to the string, replacing its current contents.
		string& operator= (const string& str);				// string
		string& operator= (const char* s);					// c-string
		string& operator= (char c);							// character
		string& operator= (std::initializer_list<char> il);	// initializer list
		string& operator= (string&& str) noexcept;			// move



		//////////////////// iterators

		// Return iterator to beginning
		// Returns an iterator pointing to the first character of the string.
		iterator begin() noexcept;
		const_iterator begin() const noexcept;
		// Return iterator to end
		// Returns an iterator pointing to the past - the - end character of the string.
		iterator end() noexcept;
		const_iterator end() const noexcept;
		// Return reverse iterator to reverse beginning
		// Returns a reverse iterator pointing to the last character of the string(i.e., its reverse beginning).
		reverse_iterator rbegin() noexcept;
		const_reverse_iterator rbegin() const noexcept;
		// Return reverse iterator to reverse end
		// Returns a reverse iterator pointing to the theoretical element preceding the first character of the string
		//	(which is considered its reverse end).
		reverse_iterator rend() noexcept;
		const_reverse_iterator rend() const noexcept;
		// Return const_iterator to beginning
		// Returns a const_iterator pointing to the first character of the string.
		const_iterator cbegin() const noexcept;
		// Return const_iterator to end
		// Returns a const_iterator pointing to the past - the - end character of the string.
		const_iterator cend() const noexcept;
		// Return const_reverse_iterator to reverse beginning
		//	Returns a const_reverse_iterator pointing to the last character of the string(i.e., its reverse beginning).
		const_reverse_iterator crbegin() const noexcept;
		// Return const_reverse_iterator to reverse end
		//	Returns a const_reverse_iterator pointing to the theoretical character preceding the first character of the string
		//  (which is considered its reverse end).
		const_reverse_iterator crend() const noexcept;



		//////////////////// capacity
		// Return length of string
		// Returns the length of the string, in terms of bytes.
		size_type size() const noexcept;
		
		// Return length of string
		//	Returns the length of the string, in terms of bytes.
		size_type length() const noexcept;

		// Return maximum size of string
		//	Returns the maximum length the string can reach.
		size_type max_size() const noexcept;

		// Resize string
		//	Resizes the string to a length of n characters.
		void resize(std::size_t n);
		void resize(std::size_t n, char c);

		// Return size of allocated storage
		// 	Returns the size of the storage space currently allocated for the string, expressed in terms of bytes.
		size_type capacity() const noexcept;

		// Request a change in capacity
		//	Requests that the string capacity be adapted to a planned change in size to a length of up to n characters.
		void reserve(std::size_t n = 0);

		// Clear string
		//	Erases the contents of the string, which becomes an empty string(with a length of 0 characters).
		void clear() noexcept;

		// Test if string is empty
		//	Returns whether the string is empty(i.e.whether its length is 0).
		bool empty() const noexcept;
	
		// Shrink to fit
		// 	Requests the string to reduce its capacity to fit its size.
		void shrink_to_fit();



		//////////////////// element access
		// Get character of string
		// Returns a reference to the character at position pos in the string.
		char& operator[] (size_t pos);
		const char& operator[] (size_t pos) const;

		// Get character in string
		//	Returns a reference to the character at position pos in the string.
		char& at(size_t pos);
		const char& at(size_t pos) const;

		// Access last character
		//	Returns a reference to the last character of the string.
		char& back();
		const char& back() const;

		// Access first character
		//	Returns a reference to the first character of the string.
		char& front();
		const char& front() const;



		//////////////////// modifiers
		// Append to string
		// Extends the string by appending additional characters at the end of its current value :
		string& operator+= (const string& str);					// string(1)
		string& operator+= (const char* s);						// c - string(2)
		string& operator+= (char c);							// character(3)
		string& operator+= (std::initializer_list<char> il);	// initializer list(4)

		//Append to string
		//	Extends the string by appending additional characters at the end of its current value :

		// (1) string
		//	Appends a copy of str.
		string& append(const string& str);
		// (2) substring
		//	Appends a copy of a substring of str.The substring is the portion of str that begins at the character 
		//  position subpos and spans sublen characters(or until the end of str, if either str is too short or if sublen is string::npos).
		string& append(const string& str, size_t subpos, size_t sublen);
		// (3) c - string
		//	Appends a copy of the string formed by the null - terminated character sequence(C - string) pointed by s.
		string& append(const char* s);
		// (4) buffer
		//	Appends a copy of the first n characters in the array of characters pointed by s.
		string& append(const char* s, size_t n);
		// (5) fill
		//	Appends n consecutive copies of character c.
		string& append(size_t n, char c);
		// (6) range
		//	Appends a copy of the sequence of characters in the range[first, last), in the same order.
		template <typename InputIterator>
		string& append(InputIterator first, InputIterator last);
		// (7) initializer list
		//	Appends a copy of each of the characters in il, in the same order.
		string& append(std::initializer_list<char> il);


		// Append character to string
		//	Appends character c to the end of the string, increasing its length by one.
		void push_back(char c);


		// Assign content to string
		//	Assigns a new value to the string, replacing its current contents.

		//	(1) string
		//	Copies str.
		string& assign(const string& str);
		//	(2) substring
		//	Copies the portion of str that begins at the character position subpos and spans sublen characters
		//  (or until the end of str, if either str is too short or if sublen is string::npos).
		string& assign(const string& str, size_t subpos, size_t sublen);
		//	(3) c - string
		//	Copies the null - terminated character sequence(C - string) pointed by s.
		string& assign(const char* s);
		//	(4) buffer
		//	Copies the first n characters from the array of characters pointed by s.
		string& assign(const char* s, size_t n);
		//	(5) fill
		//	Replaces the current value by n consecutive copies of character c.
		string& assign(size_t n, char c);
		//	(6) range
		//	Copies the sequence of characters in the range[first, last), in the same order.
		template <typename InputIterator>
		string& assign(InputIterator first, InputIterator last);
		//	(7) initializer list
		//	Copies each of the characters in il, in the same order.
		string& assign(std::initializer_list<char> il);
		//	(8) move
		//	Acquires the contents of str.
		//	str is left in an unspecified but valid state.
		string& assign(string&& str) noexcept;


		// Insert into string
		//	Inserts additional characters into the string right before the character indicated by pos(or p) :

		//	(1) string
		//	Inserts a copy of str.
		string& insert(size_t pos, const string& str);
		//	(2) substring
		//	Inserts a copy of a substring of str.The substring is the portion of str that begins at the character position subpos 
		//  and spans sublen characters(or until the end of str, if either str is too short or if sublen is npos).
		string& insert(size_t pos, const string& str, size_t subpos, size_t sublen);
		//	(3) c - string
		//	Inserts a copy of the string formed by the null - terminated character sequence(C - string) pointed by s.
		string& insert(size_t pos, const char* s);
		//	(4) buffer
		//	Inserts a copy of the first n characters in the array of characters pointed by s.
		string& insert(size_t pos, const char* s, size_t n);
		//	(5) fill
		//	Inserts n consecutive copies of character c.
		string& insert(size_t pos, size_t n, char c);
		iterator insert(const_iterator p, size_t n, char c);
		//	(6) single character
		//	Inserts character c.
		iterator insert(const_iterator p, char c);
		//	(7) range
		//	Inserts a copy of the sequence of characters in the range[first, last), in the same order.
		template <typename InputIterator>
		iterator insert(iterator p, InputIterator first, InputIterator last);
		//	(8) initializer list
		//	Inserts a copy of each of the characters in il, in the same order.
		string& insert(const_iterator p, std::initializer_list<char> il);


		//Erase characters from string
		//	Erases part of the string, reducing its length :

		//  (1) sequence
		//	Erases the portion of the string value that begins at the character position pos and spans len characters
		//  (or until the end of the string, if either the content is too short or if len is string::npos.
		//	Notice that the default argument erases all characters in the string(like member function clear).
		string& erase(size_t pos = 0, size_t len = npos);
		//	(2) character
		//	Erases the character pointed by p.
		iterator erase(const_iterator p);
		//	(3) range
		//	Erases the sequence of characters in the range[first, last).
		iterator erase(const_iterator first, const_iterator last);


		// Replace portion of string
		//	Replaces the portion of the string that begins at character pos and spans len characters
		//  (or the part of the string in the range between[i1, i2)) by new contents:

		//  (1) string
		//	Copies str.
		string& replace(size_t pos, size_t len, const string& str);
		string& replace(const_iterator i1, const_iterator i2, const string& str);
		//	(2) substring
		//	Copies the portion of str that begins at the character position subpos and spans sublen characters
		//  (or until the end of str, if either str is too short or if sublen is string::npos).
		string& replace(size_t pos, size_t len, const string& str, size_t subpos, size_t sublen);
		//	(3) c - string
		//	Copies the null - terminated character sequence(C - string) pointed by s.
		string& replace(size_t pos, size_t len, const char* s);
		string& replace(const_iterator i1, const_iterator i2, const char* s);
		//	(4) buffer
		//	Copies the first n characters from the array of characters pointed by s.
		string& replace(size_t pos, size_t len, const char* s, size_t n);
		string& replace(const_iterator i1, const_iterator i2, const char* s, size_t n);
		//	(5) fill
		//	Replaces the portion of the string by n consecutive copies of character c.
		string& replace(size_t pos, size_t len, size_t n, char c);
		string& replace(const_iterator i1, const_iterator i2, size_t n, char c);
		//	(6) range
		//	Copies the sequence of characters in the range[first, last), in the same order.
		template <typename InputIterator>
		string& replace(const_iterator i1, const_iterator i2, InputIterator first, InputIterator last);
		//	(7) initializer list
		//	Copies each of the characters in il, in the same order.
		string& replace(const_iterator i1, const_iterator i2, std::initializer_list<char> il);


		// Swap string values
		//	Exchanges the content of the container by the content of str, which is another string object.Lengths may differ.
		void swap(string& str);

		// Delete last character
		//	Erases the last character of the string, effectively reducing its length by one.
		void pop_back();



		//////////////////// string operations



		//////////////////// member constants



		//////////////////// non-member functions overloads
		// friend

	};
}
#endif