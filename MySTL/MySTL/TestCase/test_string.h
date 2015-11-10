#ifndef INCLUDED_TEST_STRING_H
#define INCLUDED_TEST_STRING_H

#include <iostream>
#include <fstream>
#include <cstring>

#include "../Declaration/string.h"

using namespace MySTL;

namespace MySTL{
	namespace TestCase{
		namespace StringTest{

			// constructor
			void tc_constructor()
			{
				MySTL::string s0("Initial string");

				// constructors used in the same order as described above:
				MySTL::string s1;
				MySTL::string s2(s0);
				MySTL::string s3(s0, 8, 3);
				MySTL::string s4("A character sequence", 6);
				MySTL::string s5("Another character sequence");
				MySTL::string s6a(10, 'x');
				MySTL::string s6b(10, 42);      // 42 is the ASCII code for '*'
				MySTL::string s7(s0.begin(), s0.begin() + 7);

				std::cout << "s1: " << s1 << "\ns2: " << s2 << "\ns3: " << s3;
				std::cout << "\ns4: " << s4 << "\ns5: " << s5 << "\ns6a: " << s6a;
				std::cout << "\ns6b: " << s6b << "\ns7: " << s7 << '\n';
			}


			// string assignment
			void tc_assignment()
			{
				string str1, str2, str3;
				str1 = "Test string: ";   // c-string
				str2 = 'x';               // single character
				str3 = str1 + str2;       // string

				std::cout << str3 << '\n';
			}

			// iterator
			void tc_iterator()
			{
				// begin, end
				MySTL::string str("Test string");
				for (MySTL::string::iterator it = str.begin(); it != str.end(); ++it)
					std::cout << *it;
				std::cout << '\n';

				// rbegin, rend
				MySTL::string str("now step live...");
				for (MySTL::string::reverse_iterator rit = str.rbegin(); rit != str.rend(); ++rit)
					std::cout << *rit;
				std::cout << '\n';

				//cbegin, cend
				MySTL::string str("Lorem ipsum");
				for (auto it = str.cbegin(); it != str.cend(); ++it)
					std::cout << *it;
				std::cout << '\n';

				// crbegin, crend
				MySTL::string str("lorem ipsum");
				for (auto rit = str.crbegin(); rit != str.crend(); ++rit)
					std::cout << *rit;
				std::cout << '\n';
			}

			// capacity
			void tc_capacity()
			{
				MySTL::string str("Test string");
				std::cout << "size: " << str.size() << "\n";
				std::cout << "length: " << str.length() << "\n";
				std::cout << "capacity: " << str.capacity() << "\n";
				std::cout << "max_size: " << str.max_size() << "\n";
			}

			void tc_reverse()
			{
				MySTL::string str;

				std::ifstream file("test.txt", std::ios::in | std::ios::ate);
				if (file) {
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
				MySTL::string str;
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
				MySTL::string content;
				MySTL::string line;
				std::cout << "Please introduce a text. Enter an empty line to finish:\n";
				do {
					getline(std::cin, line);
					content += line + '\n';
				} while (!line.empty());
				std::cout << "The text you introduced was:\n" << content;
			}

			void tc_shrink_to_fit()
			{
				MySTL::string str(100, 'x');
				std::cout << "1. capacity of str: " << str.capacity() << '\n';

				str.resize(10);
				std::cout << "2. capacity of str: " << str.capacity() << '\n';

				str.shrink_to_fit();
				std::cout << "3. capacity of str: " << str.capacity() << '\n';
			}


			// element access
			void tc_elementAccess()
			{
				MySTL::string str("test string");
				for (int i = 0; i < str.length(); ++i)
					std::cout << str[i];

				for (unsigned i = 0; i < str.length(); ++i)
					std::cout << str.at(i);

				str.back() = '!';
				std::cout << str << '\n';
				str.front() = 'T';
				std::cout << str << '\n';
			}


			// modifiers
			void tc_append()
			{
				MySTL::string name("John");
				MySTL::string family("Smith");
				name += " K. ";         // c-string
				name += family;         // string
				name += '\n';           // character

				std::cout << name;

				// append
				MySTL::string str1;
				MySTL::string str2 = "Writing ";
				MySTL::string str3 = "print 10 and then 5 more";

				// used in the same order as described above:
				str1.append(str2);                       // "Writing "
				str1.append(str3, 6, 3);                   // "10 "
				str1.append("dots are cool", 5);          // "dots "
				str1.append("here: ");                   // "here: "
				str1.append(10u, '.');                    // ".........."
				str1.append(str3.begin() + 8, str3.end());  // " and then 5 more"
				str1.append<int>(5, 0x2E);                // "....."

				std::cout << str1 << '\n';


				// push_back
				MySTL::string str4;
				std::ifstream file("test.txt", std::ios::in);
				if (file) {
					while (!file.eof()) str4.push_back(file.get());
				}
				std::cout << str4 << '\n';
			}


			void tc_assign()
			{
				// assign
				MySTL::string str5;
				MySTL::string base = "The quick brown fox jumps over a lazy dog.";

				// used in the same order as described above:

				str5.assign(base);
				std::cout << str5 << '\n';

				str5.assign(base, 10, 9);
				std::cout << str5 << '\n';         // "brown fox"

				str5.assign("pangrams are cool", 7);
				std::cout << str5 << '\n';         // "pangram"

				str5.assign("c-str5ing");
				std::cout << str5 << '\n';         // "c-str5ing"

				str5.assign(10, '*');
				std::cout << str5 << '\n';         // "**********"

				str5.assign<int>(10, 0x2D);
				std::cout << str5 << '\n';         // "----------"

				str5.assign(base.begin() + 16, base.end() - 12);
				std::cout << str5 << '\n';         // "fox jumps over"
			}


			void tc_insert()
			{
				// insert
				MySTL::string str = "to be question";
				MySTL::string str2 = "the ";
				MySTL::string str3 = "or not to be";
				MySTL::string::iterator it;

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


			void tc_replace()
			{
				MySTL::string base = "this is a test string.";
				MySTL::string str2 = "n example";
				MySTL::string str3 = "sample phrase";
				MySTL::string str4 = "useful.";

				// replace signatures used in the same order as described above:

				// Using positions:                 0123456789*123456789*12345
				MySTL::string str = base;           // "this is a test string."
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
				MySTL::string buyer("money");
				MySTL::string seller("goods");

				std::cout << "Before the swap, buyer has " << buyer;
				std::cout << " and seller has " << seller << '\n';

				seller.swap(buyer);

				std::cout << " After the swap, buyer has " << buyer;
				std::cout << " and seller has " << seller << '\n';
			}


			void tc_cstr()
			{
				MySTL::string str("Please split this sentence into tokens");

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

		}
	}
}
#endif
