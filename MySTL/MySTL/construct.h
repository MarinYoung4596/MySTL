#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_

#include <new>
#include “type_traits.h”
#include "iterator.h"

namespace MySTL
{
    template <typename T1, typename T2>
    inline void construct(T1 *p, const T2 &value)
    {
        new (p) T1(value);  // placement new; invoke T1:T1(value)
    }

    template <typename T>
    inline void destory(T *p)
    {
        p->~T();    // default destructor
    }

    template <typename ForwardIterator>
    inline void destory(ForwardIterator first, ForwardIterator last)
    {
        _destory(first, last, value_type(first));
    }

    template <typename ForwardIterator, typename T>
    inline void _destory(ForwardIterator first, ForwardIterator last, T*) // T* and value_type were used to get the real type of T
    {
        typedef typename _type_traits<T>::has_trivial_destructor trivial_destructor;
        return _destory_aux(first, last, trivial_destructor);
    }

    template <typename ForwardIterator>
    inline void _destory_aux(ForwardIterator first, ForwardIterator last, _true_type) {}

    template <typename ForwardIterator>
    inline void _destory_aux(ForwardIterator first, ForwardIterator last, _false_type)
    {
        for ( ; first != last; ++first)
            destory(&*first);   // invoke default destructor
    }

    // partical specialization
    inline void destory(char *, char *) {}
    inline void destory(wchar_t *, wchar_t *) {}
}

#endif
