#ifndef _UNINITIALIZED_FUNCTIONS_H_
#define _UNINITIALIZED_FUNCTIONS_H_

// implementation of
// uninitialized_copy, uninitialized_fill, uninitialized_fill_n
#include "construct.h"


namespace MySTL
{
    template <typename InputIterator, typename ForwardIterator>
    typename ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
    {

    }

    template <typename ForwardIterator, typename T>
    void uninitialized_fill(ForwardIterator first, ForwardIterator last, consta T &x)
    {

    }

    template <typename ForwardIterator, typename Size, typename T>
    typename ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T &x)
    {

    }
}

#endif
