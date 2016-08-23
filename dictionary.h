#ifndef Dictionary_h
#define Dictionary_h

#include <cstdio>

#include "rational_number.h"

struct Point
{
    size_t i; size_t j;
    RationalNumber x;
};

class Dictionary
{
public:
    Point* mas;
    size_t size;
    
    Dictionary();
    Dictionary(const Dictionary &);
    ~Dictionary();
    
    void add(size_t i, size_t j, const RationalNumber &);
    void rem(size_t i, size_t j);
    RationalNumber get(size_t i, size_t j) const;
    RationalNumber& get_ref(size_t i, size_t j);
    
    Dictionary operator=(const Dictionary&);
};

#endif /* Dictionary_h */
