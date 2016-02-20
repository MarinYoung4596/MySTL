#ifndef INCLUDED_TEST_STRING
#define INCLUDED_TEST_STRING

#include <iostream>
#include <fstream>
#include "../Declaration/string.h"

namespace MySTL
{
	namespace TestString
	{
		void tc_constructor()
		{
			string s0("Initial string");

			// constructors used in the same order as described above:
			string s1;
			string s2(s0);
			string s3(s0, 8, 3);
			string s4("A character sequence", 6);
			string s5("Another character sequence");
			string s6a(10, 'x');
			string s6b(10, 42);      // 42 is the ASCII code for '*'
			string s7(s0.begin(), s0.begin() + 7);

			std::cout << "s1: " << s1 << "\ns2: " << s2 << "\ns3: " << s3;
			std::cout << "\ns4: " << s4 << "\ns5: " << s5 << "\ns6a: " << s6a;
			std::cout << "\ns6b: " << s6b << "\ns7: " << s7 << '\n';
		}

		void tc_assignmentOperator()
		{
			string str1, str2, str3;
			str1 = "Test string: ";   // c-string
			str2 = 'x';               // single character
			str3 = str1 + str2;       // string

			std::cout << str3 << '\n';
		}

		void tc_size()
		{
			string str("Test string");
			std::cout << "The size of str is " << str.size() << " bytes.\n";
		}

		void tc_maxSize()
		{
			string str("Test string");
			std::cout << "size: " << str.size() << "\n";
			std::cout << "length: " << str.length() << "\n";
			std::cout << "capacity: " << str.capacity() << "\n";
			std::cout << "max_size: " << str.max_size() << "\n";
		}

		void tc_resize()
		{
			string str("I like to code in C");
			std::cout << str << '\n';

			unsigned sz = str.size();

			str.resize(sz + 2, '+');
			std::cout << str << '\n';

			str.resize(14);
			std::cout << str << '\n';
		}

		void tc_reserve()
		{
			string str;

			std::ifstream file("test.txt", std::ios::in | std::ios::ate);
			if (file) 
			{
				std::ifstream::streampos filesize = file.tellg();
				str.reserve(filesize);

				file.seekg(0);
				while (!file.eof())
				{
					str += file.get();
				}
				std::cout << str;
			}
		}

		void tc_clear()
		{
			char c;
			string str;
			std::cout << "Please type some lines of text. Enter a dot (.) to finish:\n";
			do {
				c = std::cin.get();
				str += c;
				if (c == '\n')
				{
					std::cout << str;
					str.clear();
				}
			} while (c != '.');
		}

		void tc_empty()
		{
			string content;
			string line;
			std::cout << "Please introduce a text. Enter an empty line to finish:\n";
			do {
				getline(std::cin, line);
				content += line + '\n';
			} while (!line.empty());
			std::cout << "The text you introduced was:\n" << content;
		}

		void tc_shrink_to_fit()
		{
			string str(100, 'x');
			std::cout << "1. capacity of str: " << str.capacity() << '\n';

			str.resize(10);
			std::cout << "2. capacity of str: " << str.capacity() << '\n';

			str.shrink_to_fit();
			std::cout << "3. capacity of str: " << str.capacity() << '\n';
		}


		void tc_at()
		{
			string str("Test string");
			for (int i = 0; i < str.length(); ++i)
			{
				std::cout << str[i];
			}
			for (unsigned i = 0; i < str.length(); ++i)
			{
				std::cout << str.at(i);
			}
		}

		void tc_back()
		{
			string str("hello world.");
			str.back() = '!';
			std::cout << str << '\n';
		}

		void tc_front()
		{
			string str("test string");
			str.front() = 'T';
			std::cout << str << '\n';
		}

		void tc_addEqualOperator()
		{
			string name("John");
			string family("Smith");
			name += " K. ";         // c-string
			name += family;         // string
			name += '\n';           // character

			std::cout << name;
		}

		void tc_append()
		{
			string str;
			string str2 = "Writing ";
			string str3 = "print 10 and then 5 more";

			// used in the same order as described above:
			str.append(str2);                       // "Writing "
			str.append(str3, 6, 3);                   // "10 "
			str.append("dots are cool", 5);          // "dots "
			str.append("here: ");                   // "here: "
			str.append(10u, '.');                    // ".........."
			str.append(str3.begin() + 8, str3.end());  // " and then 5 more"
			str.append<int>(5, 0x2E);                // "....."

			std::cout << str << '\n';
		}

		void tc_push_back()
		{
			string str;
			std::ifstream file("test.txt", std::ios::in);
			if (file) {
				while (!file.eof()) str.push_back(file.get());
			}
			std::cout << str << '\n';
		}

		void tc_assign()
		{
			string str;
			string base = "The quick brown fox jumps over a lazy dog.";

			// used in the same order as described above:

			str.assign(base);
			std::cout << str << '\n';

			str.assign(base, 10, 9);
			std::cout << str << '\n';         // "brown fox"

			str.assign("pangrams are cool", 7);
			std::cout << str << '\n';         // "pangram"

			str.assign("c-string");
			std::cout << str << '\n';         // "c-string"

			str.assign(10, '*');
			std::cout << str << '\n';         // "**********"

			str.assign<int>(10, 0x2D);
			std::cout << str << '\n';         // "----------"

			str.assign(base.begin() + 16, base.end() - 12);
			std::cout << str << '\n';         // "fox jumps over"
		}

		void tc_insert()
		{
			string str = "to be question";
			string str2 = "the ";
			string str3 = "or not to be";
			string::iterator it;

			// used in the same order as described above:
			str.insert(6, str2);                 // to be (the )question
			str.insert(6, str3, 3, 4);             // to be (not )the question
			str.insert(10, "that is cool", 8);    // to be not (that is )the question
			str.insert(10, "to be ");            // to be not (to be )that is the question
			str.insert(15, 1, ':');               // to be not to be(:) that is the question
			it = str.insert(str.begin() + 5, ','); // to be(,) not to be: that is the question
			str.insert(str.end(), 3, '.');       // to be, not to be: that is the question(...)
			str.insert(it + 2, str3.begin(), str3.begin() + 3); // (or )

			std::cout << str << '\n';
		}

		void tc_erase()
		{
			string str("This is an example sentence.");
			std::cout << str << '\n';
			// "This is an example sentence."
			str.erase(10, 8);                        //            ^^^^^^^^
			std::cout << str << '\n';
			// "This is an sentence."
			str.erase(str.begin() + 9);               //           ^
			std::cout << str << '\n';
			// "This is a sentence."
			str.erase(str.begin() + 5, str.end() - 9);  //       ^^^^^
			std::cout << str << '\n';
			// "This sentence."
		}

		void tc_replace()
		{
			string base = "this is a test string.";
			string str2 = "n example";
			string str3 = "sample phrase";
			string str4 = "useful.";

			// replace signatures used in the same order as described above:

			// Using positions:                 0123456789*123456789*12345
			string str = base;           // "this is a test string."
			str.replace(9, 5, str2);          // "this is an example string." (1)
			str.replace(19, 6, str3, 7, 6);     // "this is an example phrase." (2)
			str.replace(8, 10, "just a");     // "this is just a phrase."     (3)
			str.replace(8, 6, "a shorty", 7);  // "this is a short phrase."    (4)
			str.replace(22, 1, 3, '!');        // "this is a short phrase!!!"  (5)

											   // Using iterators:                                               0123456789*123456789*
			str.replace(str.begin(), str.end() - 3, str3);                    // "sample phrase!!!"      (1)
			str.replace(str.begin(), str.begin() + 6, "replace");             // "replace phrase!!!"     (3)
			str.replace(str.begin() + 8, str.begin() + 14, "is coolness", 7);    // "replace is cool!!!"    (4)
			str.replace(str.begin() + 12, str.end() - 4, 4, 'o');                // "replace is cooool!!!"  (5)
			str.replace(str.begin() + 11, str.end(), str4.begin(), str4.end());// "replace is useful."    (6)
			std::cout << str << '\n';
		}

		void tc_swap()
		{
			string buyer("money");
			string seller("goods");

			std::cout << "Before the swap, buyer has " << buyer;
			std::cout << " and seller has " << seller << '\n';

			seller.swap(buyer);

			std::cout << " After the swap, buyer has " << buyer;
			std::cout << " and seller has " << seller << '\n';
		}

		void tc_pop_back()
		{
			string str("hello world!");
			str.pop_back();
			std::cout << str << '\n';
		}

		void tc_c_str()
		{
			string str("Please split this sentence into tokens");

			char * cstr = new char[str.length() + 1];
			std::strcpy(cstr, str.c_str());

			// cstr now contains a c-string copy of str

			char * p = std::strtok(cstr, " ");
			while (p != 0)
			{
				std::cout << p << '\n';
				p = std::strtok(NULL, " ");
			}

			delete[] cstr;
		}

		void tc_data()
		{
			int length;

			string str = "Test string";
			char* cstr = "Test string";

			if (str.length() == std::strlen(cstr))
			{
				std::cout << "str and cstr have the same length.\n";

				if (memcmp(cstr, str.data(), str.length()) == 0)
					std::cout << "str and cstr have the same content.\n";
			}
		}

		void tc_copy()
		{
			char buffer[20];
			string str("Test string...");
			std::size_t length = str.copy(buffer, 6, 5);
			buffer[length] = '\0';
			std::cout << "buffer contains: " << buffer << '\n';
		}

		void tc_find()
		{
			string str("There are two needles in this haystack with needles.");
			string str2("needle");

			// different member versions of find in the same order as above:
			std::size_t found = str.find(str2);
			if (found != std::string::npos)
				std::cout << "first 'needle' found at: " << found << '\n';

			found = str.find("needles are small", found + 1, 6);
			if (found != std::string::npos)
				std::cout << "second 'needle' found at: " << found << '\n';

			found = str.find("haystack");
			if (found != std::string::npos)
				std::cout << "'haystack' also found at: " << found << '\n';

			found = str.find('.');
			if (found != std::string::npos)
				std::cout << "Period found at: " << found << '\n';

			// let's replace the first needle:
			str.replace(str.find(str2), str2.length(), "preposition");
			std::cout << str << '\n';
		}

		void tc_rfind()
		{
			string str("The sixth sick sheik's sixth sheep's sick.");
			string key("sixth");

			std::size_t found = str.rfind(key);
			if (found != std::string::npos)
				str.replace(found, key.length(), "seventh");

			std::cout << str << '\n';
		}

		void tc_find_first_of()
		{
			string str("Please, replace the vowels in this sentence by asterisks.");
			std::size_t found = str.find_first_of("aeiou");
			while (found != std::string::npos)
			{
				str[found] = '*';
				found = str.find_first_of("aeiou", found + 1);
			}

			std::cout << str << '\n';
		}


		void SplitFilename(const std::string& str)
		{
			std::cout << "Splitting: " << str << '\n';
			std::size_t found = str.find_last_of("/\\");
			std::cout << " path: " << str.substr(0, found) << '\n';
			std::cout << " file: " << str.substr(found + 1) << '\n';
		}

		void tc_find_last_of()
		{
			string str1("/usr/bin/man");
			string str2("c:\\windows\\winhelp.exe");

			SplitFilename(str1);
			SplitFilename(str2);
		}

		void tc_find_first_not_of()
		{
			string str("look for non-alphabetic characters...");

			std::size_t found = str.find_first_not_of("abcdefghijklmnopqrstuvwxyz ");

			if (found != std::string::npos)
			{
				std::cout << "The first non-alphabetic character is " << str[found];
				std::cout << " at position " << found << '\n';
			}
		}

		void tc_find_last_not_of()
		{
			string str("Please, erase trailing white-spaces   \n");
			string whitespaces(" \t\f\v\n\r");

			std::size_t found = str.find_last_not_of(whitespaces);
			if (found != std::string::npos)
				str.erase(found + 1);
			else
				str.clear();            // str is all whitespace

			std::cout << '[' << str << "]\n";
		}

		void tc_substr()
		{
			string str = "We think in generalities, but we live in details.";
			// (quoting Alfred N. Whitehead)

			std::string str2 = str.substr(3, 5);     // "think"

			std::size_t pos = str.find("live");      // position of "live" in str

			std::string str3 = str.substr(pos);     // get from "live" to the end

			std::cout << str2 << ' ' << str3 << '\n';
		}

		void tc_compare()
		{
			string str1("green apple");
			string str2("red apple");

			if (str1.compare(str2) != 0)
				std::cout << str1 << " is not " << str2 << '\n';

			if (str1.compare(6, 5, "apple") == 0)
				std::cout << "still, " << str1 << " is an apple\n";

			if (str2.compare(str2.size() - 5, 5, "apple") == 0)
				std::cout << "and " << str2 << " is also an apple\n";

			if (str1.compare(6, 5, str2, 4, 5) == 0)
				std::cout << "therefore, both are apples\n";
		}

		void tc_addOperator()
		{
			string firstlevel("com");
			string secondlevel("cplusplus");
			string scheme("http://");
			string hostname;
			string url;

			hostname = "www." + secondlevel + '.' + firstlevel;
			url = scheme + hostname;

			std::cout << url << '\n';
		}

		void tc_relationalOperators()
		{
			string foo = "alpha";
			string bar = "beta";

			if (foo == bar) std::cout << "foo and bar are equal\n";
			if (foo != bar) std::cout << "foo and bar are not equal\n";
			if (foo < bar) std::cout << "foo is less than bar\n";
			if (foo > bar) std::cout << "foo is greater than bar\n";
			if (foo <= bar) std::cout << "foo is less than or equal to bar\n";
			if (foo >= bar) std::cout << "foo is greater than or equal to bar\n";
		}

		void tc_swap_friendFunction()
		{
			string buyer("money");
			string seller("goods");

			std::cout << "Before the swap, buyer has " << buyer;
			std::cout << " and seller has " << seller << '\n';

			swap(buyer, seller);

			std::cout << " After the swap, buyer has " << buyer;
			std::cout << " and seller has " << seller << '\n';
		}

		void tc_istreamOperator()
		{
			string name;

			std::cout << "Please, enter your name: ";
			std::cin >> name;
			std::cout << "Hello, " << name << "!\n";
		}

		void tc_ostreamOperator()
		{
			string str = "Hello world!";
			std::cout << str << '\n';
		}

		void tc_getline()
		{
			string name;

			std::cout << "Please, enter your full name: ";
			getline(std::cin, name);
			std::cout << "Hello, " << name << "!\n";
		}
	}
}

#endif
