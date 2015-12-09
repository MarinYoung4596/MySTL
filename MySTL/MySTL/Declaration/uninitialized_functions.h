#ifndef INCLUDED_UNINITIALIZED_FUNCTIONS_H
#define INCLUDED_UNINITIALIZED_FUNCTIONS_H

// implementation of
// uninitialized_copy, uninitialized_fill, uninitialized_fill_n
#include <cstring>          // function:    memcpy
#include "construct.h"      // function:    construct
#include "iterator.h"       // struct:      iterator_traits
#include "type_traits.h"    // struct:      _type_traits
//#include "algorithm.h"      // function:    copy, fill, fill_n


namespace MySTL
{
    template <typename InputIterator, typename ForwardIterator>
    inline typename ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
    {
        // iterator_traits<InputIterator> invokes the partial specialization version of iterator_traits
        // (e.g. iterator_traits<const T*>), and get the value type of InputIterator;
        // After that, invokes the corresponding version partial specialization version of _type_traits
        // (e.g. _type_traits<int>), so as to judge whether it is POD_TYPE or not.
        // We've try our best to improve the efficency with respect to different objects :-)
        typedef typename _type_traits<iterator_traits<InputIterator>::value_type>::is_POD_type POD_TYPE;
        return _uninitialized_copy_aux(first, last, result, POD_TYPE);
    }

    template <typename InputIterator, typename ForwardIterator, typename T>
    inline typename ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _true_type)
    {
        return copy(first, last, result);
    }

    template <typename InputIterator, typename ForwardIterator, typename T>
    inline typename ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _false_type)
    {
        ForwardIterator curr = result;      // don't change the position of result
        for ( ; first != last; ++first, ++curr)
            construct(&*first, curr);
        return curr;
    }

    // partial specialization
    inline char* uninitialized_copy(const char *first, const char *last, char *result)
    {
        std::memcpy(result, first, static_cast<std::size_t>(last - first));
        return result + (last - first);
    }

    inline wchar_t* uninitialized_copy(const wchar_t *first, const wchar_t *last, wchar_t *result)
    {
        std::memcpy(result, first, sizeof(wchar_t) * static_cast<std::size_t>(last - first));
        return result + (last - first);
    }


    //////////////////////////////////////////////
    template <typename ForwardIterator, typename T>
    inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, consta T &x)
    {
        typedef typename _type_traits<iterator_traits<ForwardIterator>::value_type>::is_POD_type POD_TYPE;
        return _uninntialized_fill_aux(first, last, x, POD_TYPE);
    }

    template <typename ForwardIterator, typename T>
    inline void _uninntialized_fill_aux(ForwardIterator first, ForwardIterator last, consta T &x, _true_type)
    {
        return fill(first, last, x);
    }

    template <typename ForwardIterator, typename T>
    inline void _uninntialized_fill_aux(ForwardIterator first, ForwardIterator last, consta T &x, _false_type)
    {
        ForwardIterator curr = first;
        for (; curr != first; ++curr)
            construct(&*curr, x);
    }


    //////////////////////////////////////////////
    template <typename ForwardIterator, typename Size, typename T>
    typename ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T &x)
    {
        typedef typename _type_traits<iterator_traits<ForwardIterator>::value_type>::is_POD_type POD_TYPE;
        return _uninntialized_fill_n_aux(first, n, x, POD_TYPE);
    }

    template <typename ForwardIterator, typename Size, typename T>
    typename ForwardIterator _uninntialized_fill_n_aux(ForwardIterator first, Size n, const T &x, _true_type)
    {
        return fill_n(first, n, x);
    }

    template <typename ForwardIterator, typename Size, typename T>
    typename ForwardIterator _uninntialized_fill_n_aux(ForwardIterator first, Size n, const T &x, _false_type)
    {
        ForwardIterator curr = first;   // ForwardIterator doesn't support operator+= operation
        for (; n; --n, ++curr)
            construct(&*first, x);
        return curr;
    }
}

#endif
