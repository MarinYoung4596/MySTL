#ifndef _TEST_VECTOR_H_
#define _TEST_VECTOR_H_

#include "Declaration/Vector.h"
#include "Implementation/Vector_impl.h"

#include <string>


namespace MySTL{
	namespace TestCase{
		namespace VectorTest{

			template <typename T>
			void printVector(Vector<T> &v)
			{
				for (auto i = v.begin(); i != v.end(); ++i)
					std::cout << *i << '\t';
				std::cout << std::endl;
			}

			void tc_constructor_1()
			{
				Vector<int> v1;		// constructor: default
				v1 = { 1, 2, 3, 4, 5 };		// assign content: initializer list
				printVector(v1);

				//Vector<std::string> v2(5, "aa");	// constructor: fill
				//printVector(v2);

				Vector<int> v3({ 4, 5, 6, 7, 8 });	// constructor: initializer list
				printVector(v3);

				Vector<int> v4(v1);		// copy constructor
				printVector(v4);

				Vector<int> v5 = v1;	// assign content: copy
				printVector(v5);

				// the iterator constructor can also be used to construct from arrays:
				int myints[] = { 16, 2, 77, 29 };
				MySTL::Vector<int> v6(myints, myints + sizeof(myints) / sizeof(int));
				printVector(v6);
			}

			void tc_constructor()
			{
				// constructors used in the same order as described above:
				MySTL::Vector<int> first;                                // empty Vector of ints
				MySTL::Vector<int> second(4, 100);                       // four ints with value 100
				MySTL::Vector<int> third(second.begin(), second.end());  // iterating through second
				MySTL::Vector<int> fourth(third);                       // a copy of third

				// the iterator constructor can also be used to construct from arrays:
				int myints[] = { 16, 2, 77, 29 };
				MySTL::Vector<int> fifth(myints, myints + sizeof(myints) / sizeof(int));

				std::cout << "The contents of fifth are:";
				for (MySTL::Vector<int>::iterator it = fifth.begin(); it != fifth.end(); ++it)
					std::cout << ' ' << *it;	// 16 2 77 29
				std::cout << '\n';
			}

			void tc_assignment()
			{
				MySTL::Vector<int> foo(3, 0);
				MySTL::Vector<int> bar(5, 0);

				bar = foo;
				foo = MySTL::Vector<int>();

				std::cout << "Size of foo: " << int(foo.size()) << '\n';	// 0
				std::cout << "Size of bar: " << int(bar.size()) << '\n';	// 3
			}

			// iterator
			//void tc_iterators()
			//{
			//	MySTL::Vector<int> myVector(5);  // 5 default-constructed ints

			//	int i = 0;

			//	MySTL::Vector<int>::reverse_iterator rit = myVector.rbegin();
			//	for (; rit != myVector.rend(); ++rit)
			//		*rit = ++i;

			//	std::cout << "myVector contains:";
			//	for (MySTL::Vector<int>::iterator it = myVector.begin(); it != myVector.end(); ++it)
			//		std::cout << ' ' << *it;
			//	std::cout << '\n';
			//}

			// capacity
			void tc_size()
			{
				MySTL::Vector<int> myVector;

				// set some content in the Vector:
				for (int i = 0; i < 100; i++) myVector.push_back(i);

				std::cout << "size: " << myVector.size() << "\n";
				std::cout << "capacity: " << myVector.capacity() << "\n";
				std::cout << "max_size: " << myVector.max_size() << "\n";
			}

			void tc_empty()
			{
				MySTL::Vector<int> myVector;
				int sum(0);

				for (int i = 1; i <= 10; i++) myVector.push_back(i);

				while (!myVector.empty())
				{
					sum += myVector.back();
					myVector.pop_back();
				}

				std::cout << "total: " << sum << '\n';
			}

			void tc_resize()
			{
				MySTL::Vector<int> myVector;

				// set some initial content:
				for (int i = 1; i < 10; i++) myVector.push_back(i);

				myVector.resize(5);
				myVector.resize(8, 100);
				myVector.resize(12);

				std::cout << "myVector contains:";
				for (size_t i = 0; i < myVector.size(); i++)
					std::cout << ' ' << myVector[i];
				std::cout << '\n';
			}

			void tc_reserve()
			{
				MySTL::Vector<int>::size_type sz;

				MySTL::Vector<int> foo;
				sz = foo.capacity();
				std::cout << "making foo grow:\n";
				for (int i = 0; i < 100; ++i)
				{
					foo.push_back(i);
					if (sz != foo.capacity())
					{
						sz = foo.capacity();
						std::cout << "capacity changed: " << sz << '\n';
					}
				}

				MySTL::Vector<int> bar;
				sz = bar.capacity();
				bar.reserve(100);   // this is the only difference with foo above
				std::cout << "making bar grow:\n";
				for (int i = 0; i < 100; ++i)
				{
					bar.push_back(i);
					if (sz != bar.capacity())
					{
						sz = bar.capacity();
						std::cout << "capacity changed: " << sz << '\n';
					}
				}
			}

			void tc_shrink_to_fit()
			{
				MySTL::Vector<int> myVector(100);
				std::cout << "1. capacity of myVector: " << myVector.capacity() << '\n';

				myVector.resize(10);
				std::cout << "2. capacity of myVector: " << myVector.capacity() << '\n';

				myVector.shrink_to_fit();
				std::cout << "3. capacity of myVector: " << myVector.capacity() << '\n';
			}

			// element access
			void tc_elementAccess()
			{
				MySTL::Vector<int> myVector(10);   // 10 zero-initialized elements

				MySTL::Vector<int>::size_type sz = myVector.size();

				// assign some values:
				for (unsigned i = 0; i < sz; i++) myVector[i] = i;

				// reverse Vector using operator[]:
				for (unsigned i = 0; i < sz / 2; i++)
				{
					int temp;
					temp = myVector[sz - 1 - i];
					myVector[sz - 1 - i] = myVector[i];
					myVector[i] = temp;
				}

				std::cout << "myVector contains:";
				for (unsigned i = 0; i < sz; i++)
					std::cout << ' ' << myVector[i];
				std::cout << '\n';
			}

			void tc_at()
			{
				MySTL::Vector<int> myVector(10);   // 10 zero-initialized ints

				// assign some values:
				for (unsigned i = 0; i < myVector.size(); i++)
					myVector.at(i) = i;

				std::cout << "myVector contains:";
				for (unsigned i = 0; i < myVector.size(); i++)
					std::cout << ' ' << myVector.at(i);
				std::cout << '\n';
			}

			void tc_front_back()
			{
				MySTL::Vector<int> myVector;

				myVector.push_back(78);
				myVector.push_back(16);

				// now front equals 78, and back 16

				myVector.front() -= myVector.back();

				std::cout << "myVector.front() is now " << myVector.front() << '\n';
				std::cout << "myVector.back() is now " << myVector.back() << '\n';
			}

			void tc_data()
			{
				MySTL::Vector<int> myVector(5);

				int* p = myVector.data();

				*p = 10;
				++p;
				*p = 20;
				p[2] = 100;

				std::cout << "myVector contains:";
				for (unsigned i = 0; i < myVector.size(); ++i)
					std::cout << ' ' << myVector[i];
				std::cout << '\n';
			}

			// modifiers
			void tc_assign()
			{
				MySTL::Vector<int> first;
				MySTL::Vector<int> second;
				MySTL::Vector<int> third;

				first.assign(7, 100);             // 7 ints with a value of 100

				MySTL::Vector<int>::iterator it;
				it = first.begin() + 1;

				second.assign(it, first.end() - 1); // the 5 central values of first

				int myints[] = { 1776, 7, 4 };
				third.assign(myints, myints + 3);   // assigning from array.

				std::cout << "Size of first: " << int(first.size()) << '\n';
				std::cout << "Size of second: " << int(second.size()) << '\n';
				std::cout << "Size of third: " << int(third.size()) << '\n';
			}

			void tc_push_back()
			{
				MySTL::Vector<int> myVector;
				int myint;

				std::cout << "Please enter some integers (enter 0 to end):\n";

				do {
					std::cin >> myint;
					myVector.push_back(myint);
				} while (myint);

				std::cout << "myVector stores " << int(myVector.size()) << " numbers.\n";
			}

			void tc_pop_back()
			{
				MySTL::Vector<int> myVector;
				int sum(0);
				myVector.push_back(100);
				myVector.push_back(200);
				myVector.push_back(300);

				while (!myVector.empty())
				{
					sum += myVector.back();
					myVector.pop_back();
				}

				std::cout << "The elements of myVector add up to " << sum << '\n';
			}

			void tc_insert()
			{
				MySTL::Vector<int> myVector(3, 100);
				MySTL::Vector<int>::iterator it;

				it = myVector.begin();
				it = myVector.insert(it, 200);

				myVector.insert(it, 2, 300);

				// "it" no longer valid, get a new one:
				it = myVector.begin();

				MySTL::Vector<int> anotherVector(2, 400);
				myVector.insert(it + 2, anotherVector.begin(), anotherVector.end());

				int myarray[] = { 501, 502, 503 };
				myVector.insert(myVector.begin(), myarray, myarray + 3);

				std::cout << "myVector contains:";
				for (it = myVector.begin(); it < myVector.end(); it++)
					std::cout << ' ' << *it;
				std::cout << '\n';
			}

			void tc_erase()
			{
				MySTL::Vector<int> myVector;

				// set some values (from 1 to 10)
				for (int i = 1; i <= 10; i++) myVector.push_back(i);

				// erase the 6th element
				myVector.erase(myVector.begin() + 5);

				// erase the first 3 elements:
				myVector.erase(myVector.begin(), myVector.begin() + 3);

				std::cout << "myVector contains:";
				for (unsigned i = 0; i < myVector.size(); ++i)
					std::cout << ' ' << myVector[i];
				std::cout << '\n';
			}

			void tc_swap()
			{
				MySTL::Vector<int> foo(3, 100);   // three ints with a value of 100
				MySTL::Vector<int> bar(5, 200);   // five ints with a value of 200

				foo.swap(bar);

				std::cout << "foo contains:";
				for (unsigned i = 0; i < foo.size(); i++)
					std::cout << ' ' << foo[i];
				std::cout << '\n';

				std::cout << "bar contains:";
				for (unsigned i = 0; i < bar.size(); i++)
					std::cout << ' ' << bar[i];
				std::cout << '\n';
			}

			void tc_clear()
			{
				MySTL::Vector<int> myVector;
				myVector.push_back(100);
				myVector.push_back(200);
				myVector.push_back(300);

				std::cout << "myVector contains:";
				for (unsigned i = 0; i < myVector.size(); i++)
					std::cout << ' ' << myVector[i];
				std::cout << '\n';

				myVector.clear();
				myVector.push_back(1101);
				myVector.push_back(2202);

				std::cout << "myVector contains:";
				for (unsigned i = 0; i < myVector.size(); i++)
					std::cout << ' ' << myVector[i];
				std::cout << '\n';
			}

			void tc_emplace()
			{
				MySTL::Vector<int> myVector = { 10, 20, 30 };

				myVector.emplace(myVector.begin() + 1, 100);
				myVector.emplace(myVector.end(), 300);

				std::cout << "myVector contains:";
				for (auto& x : myVector)
					std::cout << ' ' << x;
				std::cout << '\n';
			}

			void tc_emplace_back()
			{
				MySTL::Vector<int> myVector = { 10, 20, 30 };

				myVector.emplace_back(100);
				myVector.emplace_back(200);

				std::cout << "myVector contains:";
				for (auto& x : myVector)
					std::cout << ' ' << x;
				std::cout << '\n';
			}

			// allocaor
			//void tc_get_allocator()
			//{
			//	MySTL::Vector<int> myVector;
			//	int * p;
			//	unsigned int i;

			//	// allocate an array with space for 5 elements using Vector's allocator:
			//	p = myVector.get_allocator().allocate(5);

			//	// construct values in-place on the array:
			//	for (i = 0; i < 5; i++) myVector.get_allocator().construct(&p[i], i);

			//	std::cout << "The allocated array contains:";
			//	for (i = 0; i < 5; i++) std::cout << ' ' << p[i];
			//	std::cout << '\n';

			//	// destroy and deallocate:
			//	for (i = 0; i < 5; i++) myVector.get_allocator().destroy(&p[i]);
			//	myVector.get_allocator().deallocate(p, 5);
			//}

			//void tc_relationalOperators()
			//{
			//	MySTL::Vector<int> foo(3, 100);   // three ints with a value of 100
			//	MySTL::Vector<int> bar(2, 200);   // two ints with a value of 200

			//	if (foo == bar) std::cout << "foo and bar are equal\n";
			//	if (foo != bar) std::cout << "foo and bar are not equal\n";
			//	if (foo< bar) std::cout << "foo is less than bar\n";
			//	if (foo> bar) std::cout << "foo is greater than bar\n";
			//	if (foo <= bar) std::cout << "foo is less than or equal to bar\n";
			//	if (foo >= bar) std::cout << "foo is greater than or equal to bar\n";
			//}


			void testCase_vector()
			{
				tc_constructor_1();
				tc_assignment();
				tc_size();
				tc_empty();
				tc_resize();
				tc_reserve();
				tc_shrink_to_fit();
				tc_elementAccess();
				tc_at();
				tc_front_back();
				tc_data();
				tc_assign();
				tc_push_back();
				tc_pop_back();
				tc_insert();
				tc_erase();
				tc_swap();
				tc_clear();
				tc_emplace();
				tc_emplace_back();
				//tc_get_allocator();
				//tc_relationalOperators();
			}

		}
	}
}
#endif // _TEST_VECTOR_H_