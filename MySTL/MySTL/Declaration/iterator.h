#ifndef INCLUDED_ITERATOR_H
#define INCLUDED_ITERATOR_H

#include <cstddef>

namespace MySTL
{

	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};


	// base iterator
	template <typename Category,
		typename T,
		typename Distance = std::ptrdiff_t,
		typename Pointer = T*,
		typename Reference = T&>
	struct iterator
	{
		typedef Category	iterator_category;
		typedef				value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
	};



	// traits
	template <typename Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
	};


	// partial specialization of traits of the ##naive pointer##
	template <typename T>
	struct iterator_traits<T*>
	{
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef std::ptrdiff_t				difference_type;
		typedef	T*							pointer;
		typedef T&							reference;
	};


	// partial specialization of traits of the ##naive pointer-to-const##
	template <typename T>
	struct iterator_traits<const T*>
	{
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef std::ptrdiff_t				difference_type;
		typedef	T*							pointer;
		typedef T&							reference;
	};


	// category
	template <typename Iterator>
	inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&)
	{
		typedef typename iterator_traits<Iterator>::iterator_category	category;
		return category();
	}


	// distance type
	template <typename Iterator>
	inline typename iterator_traits<Iterator>::difference_type* difference_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::difference_type*> (0);
	}


	// value type
	template <typename Iterator>
	inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::value_type*> (0);
	}

}


#endif
