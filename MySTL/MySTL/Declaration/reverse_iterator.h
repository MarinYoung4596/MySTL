#ifndef INCLUDED_REVERSE_ITERATOR_H
#define INCLUDED_REVERSE_ITERATOR_H

#include "type_traits.h"
#include "iterator.h"

namespace MySTL
{
	template <typename Iterator>
	class reverse_iterator
	{
	protected:
		Iterator current;

	public:
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		typedef typename iterator_traits<Iterator>::value_type value_type;
		typedef typename iterator_traits<Iterator>::difference_type difference_type;
		typedef typename iterator_traits<Iterator>::pointer pointer;
		typedef typename iterator_traits<Iterator>::reference reference;

		typedef Iterator iterator_type;             // Forward iterator
		typedef reverse_iterator<Iterator> self;    // Reverse iterator
	
	public:
		reverse_iterator() {}
		reverse_iterator(iterator_type x) : current(x) {}
		reverse_iterator(const reverse_iterator &x) : current(x.current) {}

		iterator_type base() const { return current; }
		reference operator*() const
		{
			Iterator tmp = current;
			return *--tmp;
		}

		pointer operator->() const { return &(operator*()); }
		
		// converse ++ to --
		self& operator++() // prefix
		{
			--current;
			return *this;
		}
		self operator++(int) // postfix
		{
			self tmp = *this;
			--current;
			return tmp;
		}

		self& operator--() // prefix
		{
			++current;
			return *this;
		}
		self operator--(int) // postfix
		{
			self tmp = *this;
			++current;
			return tmp;
		}

		self  operator+ (difference_type n) const { return self(current - n); }
		self  operator- (difference_type n) const { return self(current + n); }
		self& operator+=(difference_type n) 
		{
			current -= n;
			return *this;
		}
		self& operator-=(difference_type n)
		{
			current += n;
			return *this;
		}
		reference operator[](difference_type n) const { return *(*this + n); }
	};
}
#endif
