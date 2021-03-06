#include <string>
#include <iostream>
#include <ctime>

#include "test_vector.h"

namespace MySTL
{
	namespace TestVector
	{
		template <typename T>
		void printvector(vector<T> &v)
		{
			for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
				std::cout << *it << '\t';
			std::cout << std::endl;
		}

		void tc_constructor()
		{
			std::cout << "-----\t constructor" << '\n';
			vector<int> v1;		// constructor: default
			v1 = { 1, 2, 3, 4, 5 };		// assign content: initializer list
			printvector(v1);

			vector<int> v2(5, 10);	// constructor: fill
			printvector(v2);

			vector<int> v3({ 4, 5, 6, 7, 8 });	// constructor: initializer list
			printvector(v3);

			vector<int> v4(v1);		// copy constructor
			printvector(v4);

			vector<int> v5 = v1;	// assign content: copy
			printvector(v5);

			// the iterator constructor can also be used to construct from arrays:
			int myints[] = { 16, 2, 77, 29 };
			MySTL::vector<int> v6(myints, myints + sizeof(myints) / sizeof(int));
			printvector(v6);
		}

		void tc_assignment()
		{
			std::cout << "-----\t assignment" << '\n';
			MySTL::vector<int> foo(3, 0);
			MySTL::vector<int> bar(5, 0);

			bar = foo;
			foo = MySTL::vector<int>();

			std::cout << "Size of foo: " << int(foo.size()) << '\n';	// 0
			std::cout << "Size of bar: " << int(bar.size()) << '\n';	// 3
		}

		// iterator
		void tc_iterators()
		{
			std::cout << "-----\t iterators" << '\n';
			MySTL::vector<int> myvector(5);  // 5 default-constructed ints

			auto i = 0;

			//MySTL::vector<int>::reverse_iterator rit = myvector.rbegin();
			//for (; rit != myvector.rend(); ++rit)
			//	*rit = ++i;

			std::cout << "myvector contains:";
			printvector(myvector);
		}

		// capacity
		void tc_size()
		{
			std::cout << "-----\t size" << '\n';
			MySTL::vector<int> myvector;

			// set some content in the vector:
			for (int i = 0; i < 100; i++) myvector.push_back(i);

			std::cout << "size: " << myvector.size() << "\n";
			std::cout << "capacity: " << myvector.capacity() << "\n";
			std::cout << "max_size: " << myvector.max_size() << "\n";
		}

		void tc_empty()
		{
			std::cout << "-----\t empty" << '\n';
			MySTL::vector<int> myvector;
			int sum(0);

			for (int i = 1; i <= 10; i++) myvector.push_back(i);

			while (!myvector.empty())
			{
				sum += myvector.back();
				myvector.pop_back();
			}

			std::cout << "total: " << sum << '\n';
		}

		void tc_resize()
		{
			std::cout << "-----\t resize" << '\n';
			MySTL::vector<int> myvector;

			// set some initial content:
			for (int i = 1; i < 10; i++) myvector.push_back(i);

			myvector.resize(5);
			myvector.resize(8, 100);
			myvector.resize(12);

			std::cout << "myvector contains:";
			printvector(myvector);
		}

		void tc_reserve()
		{
			std::cout << "-----\t reserve" << '\n';
			MySTL::vector<int>::size_type sz;

			MySTL::vector<int> foo;
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

			MySTL::vector<int> bar;
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
			std::cout << "-----\t shrink_to_fit" << '\n';
			MySTL::vector<int> myvector(100);
			std::cout << "1. capacity of myvector: " << myvector.capacity() << '\n';

			myvector.resize(10);
			std::cout << "2. capacity of myvector: " << myvector.capacity() << '\n';

			//myvector.shrink_to_fit();
			//std::cout << "3. capacity of myvector: " << myvector.capacity() << '\n';
		}

		// element access
		void tc_elementAccess()
		{
			std::cout << "-----\t element access" << '\n';
			MySTL::vector<int> myvector(10);   // 10 zero-initialized elements

			MySTL::vector<int>::size_type sz = myvector.size();

			// assign some values:
			for (unsigned i = 0; i < sz; i++) myvector[i] = i;

			// reverse vector using operator[]:
			for (unsigned i = 0; i < sz / 2; i++)
			{
				int temp;
				temp = myvector[sz - 1 - i];
				myvector[sz - 1 - i] = myvector[i];
				myvector[i] = temp;
			}

			std::cout << "myvector contains:";
			printvector(myvector);
		}

		void tc_at()
		{
			std::cout << "-----\t at" << '\n';
			MySTL::vector<int> myvector(10);   // 10 zero-initialized ints

			// assign some values:
			for (unsigned i = 0; i < myvector.size(); i++)
				myvector.at(i) = i;

			std::cout << "myvector contains:";
			printvector(myvector);
		}

		void tc_front_back()
		{
			std::cout << "-----\t front back" << '\n';
			MySTL::vector<int> myvector;

			myvector.push_back(78);
			myvector.push_back(16);

			// now front equals 78, and back 16

			myvector.front() -= myvector.back();

			std::cout << "myvector.front() is now " << myvector.front() << '\n';
			std::cout << "myvector.back() is now " << myvector.back() << '\n';
		}

		void tc_data()
		{
			std::cout << "-----\t data" << '\n';
			MySTL::vector<int> myvector(5);

			int* p = myvector.data();

			*p = 10;
			++p;
			*p = 20;
			p[2] = 100;

			std::cout << "myvector contains:";
			printvector(myvector);
		}

		// modifiers
		void tc_assign()
		{
			std::cout << "-----\t assign" << '\n';
			MySTL::vector<int> first;
			MySTL::vector<int> second;
			MySTL::vector<int> third;

			first.assign(7, 100);             // 7 ints with a value of 100

			MySTL::vector<int>::iterator it;
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
			std::cout << "-----\t push_back" << '\n';
			MySTL::vector<int> myvector;

			for (auto i = 0; i < 10; ++i)
			{
				srand(static_cast<unsigned>(time(nullptr)));
				myvector.push_back(rand());
			}
			printvector(myvector);
		}

		void tc_pop_back()
		{
			std::cout << "-----\t pop_back" << '\n';
			MySTL::vector<int> myvector;
			int sum(0);
			myvector.push_back(100);
			myvector.push_back(200);
			myvector.push_back(300);

			while (!myvector.empty())
			{
				sum += myvector.back();
				myvector.pop_back();
			}
			std::cout << "The elements of myvector add up to " << sum << '\n';
		}

		void tc_insert()
		{
			std::cout << "-----\t insert" << '\n';
			MySTL::vector<int> myvector(3, 100);
			MySTL::vector<int>::iterator it;

			it = myvector.begin();
			it = myvector.insert(it, 200);

			myvector.insert(it, 2, 300);

			// "it" no longer valid, get a new one:
			it = myvector.begin();

			MySTL::vector<int> anothervector(2, 400);
			myvector.insert(it + 2, anothervector.begin(), anothervector.end());

			int myarray[] = { 501, 502, 503 };
			myvector.insert(myvector.begin(), myarray, myarray + 3);

			std::cout << "myvector contains:";
			printvector(myvector);
		}

		void tc_erase()
		{
			std::cout << "-----\t erase" << '\n';
			MySTL::vector<int> myvector;

			// set some values (from 1 to 10)
			for (int i = 1; i <= 10; i++) myvector.push_back(i);

			// erase the 6th element
			myvector.erase(myvector.begin() + 5);

			// erase the first 3 elements:
			myvector.erase(myvector.begin(), myvector.begin() + 3);

			std::cout << "myvector contains:";
			printvector(myvector);
		}

		void tc_swap()
		{
			std::cout << "-----\t swap" << '\n';
			MySTL::vector<int> foo(3, 100);   // three ints with a value of 100
			MySTL::vector<int> bar(5, 200);   // five ints with a value of 200

			foo.swap(bar);

			std::cout << "foo contains:";
			printvector(foo);

			std::cout << "bar contains:";
			printvector(bar);
		}

		void tc_clear()
		{
			std::cout << "-----\t clear" << '\n';
			MySTL::vector<int> myvector;
			myvector.push_back(100);
			myvector.push_back(200);
			myvector.push_back(300);

			std::cout << "myvector contains:";
			printvector(myvector);

			myvector.clear();
			myvector.push_back(1101);
			myvector.push_back(2202);

			std::cout << "myvector contains:";
			printvector(myvector);
		}


        /*
		void tc_emplace()
		{
			std::cout << "-----\t emplace" << '\n';
			MySTL::vector<int> myvector = { 10, 20, 30 };

			myvector.emplace(myvector.begin() + 1, 100);
			myvector.emplace(myvector.end(), 300);

			std::cout << "myvector contains:";
			for (auto& x : myvector)
				std::cout << ' ' << x;
			std::cout << '\n';
		}

		void tc_emplace_back()
		{
			std::cout << "-----\t emplace_back" << '\n';
			MySTL::vector<int> myvector = { 10, 20, 30 };

			myvector.emplace_back(100);
			myvector.emplace_back(200);

			std::cout << "myvector contains:";
			for (auto& x : myvector)
				std::cout << ' ' << x;
			std::cout << '\n';
		}*/


		// allocator
		/*
		void tc_get_allocator()
		{
		MySTL::vector<int> myvector;
		int * p;
		unsigned int i;

		// allocate an array with space for 5 elements using vector's allocator:
		p = myvector.get_allocator().allocate(5);

		// construct values in-place on the array:
		for (i = 0; i < 5; i++) myvector.get_allocator().construct(&p[i], i);

		std::cout << "The allocated array contains:";
		for (i = 0; i < 5; i++) std::cout << ' ' << p[i];
		std::cout << '\n';

		// destroy and deallocate:
		for (i = 0; i < 5; i++) myvector.get_allocator().destroy(&p[i]);
		myvector.get_allocator().deallocate(p, 5);
		}
		*/


		void tc_relationalOperators()
		{
			std::cout << "-----\t relational operators" << '\n';
			MySTL::vector<int> foo(3, 100);   // three ints with a value of 100
			MySTL::vector<int> bar(2, 200);   // two ints with a value of 200

			if (foo == bar) std::cout << "foo and bar are equal\n";
			if (foo != bar) std::cout << "foo and bar are not equal\n";
			if (foo < bar) std::cout << "foo is less than bar\n";
			if (foo > bar) std::cout << "foo is greater than bar\n";
			if (foo <= bar) std::cout << "foo is less than or equal to bar\n";
			if (foo >= bar) std::cout << "foo is greater than or equal to bar\n";
		}


		void test_all()
		{
			std::cout << "----------test vector----------" << std::endl;
			tc_constructor();
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
			//tc_emplace();
			//tc_emplace_back();
			//tc_get_allocator();
			tc_relationalOperators();
			std::cout << "----------test vector success----------\n" << std::endl;
		}

	}
}

