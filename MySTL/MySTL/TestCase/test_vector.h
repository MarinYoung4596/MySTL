#ifndef _TEST_VECTOR_H_
#define _TEST_VECTOR_H_

#include "Declaration/vector.h"
#include "Implementation/vector_impl.h"

#include <string>

using namespace MySTL;

template <typename T>
void printVector(vector<T> &v)
{
	for (auto i = v.begin(); i != v.end(); ++i)
		std::cout << *i << '\t';
	std::cout << std::endl;
}

void testcase_constructor()
{
	vector<int> v1;		// constructor: default
	v1 = { 1, 2, 3, 4, 5 };		// assign content: initializer list

	vector<std::string> v2(5, "aa");	// constructor: fill
	vector<int> v3({ 4, 5, 6, 7, 8 });	// constructor: initializer list
	vector<int> v4(v1);		// copy constructor
	vector<int> v5 = v1;	// assign content: copy



	// print



	// insert



	// erase




}

void testcase_elementAccess()
{

}

#endif // _TEST_VECTOR_H_