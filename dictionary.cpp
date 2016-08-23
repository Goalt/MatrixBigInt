#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include "exception.h"
#include "dictionary.h"

Dictionary :: Dictionary()
{
    mas = NULL;
    size = 0;
}

Dictionary :: Dictionary(const Dictionary &b)
{
    mas = NULL;
    size = 0;
    for (size_t i = 0; i < b.size; i++)
    {
        this->add(b.mas[i].i, b.mas[i].j, b.mas[i].x);
    }
}

void Dictionary :: add(size_t i, size_t j, const RationalNumber &x)
{
    for (size_t k = 0; k < size; k++)
    {
        if ((mas[k].i == i) && (mas[k].j == j))
        {
            mas[k].x = x;
            return;
        }
    }
    
    Point* tmp;
    tmp = new Point[size+1];
    if (tmp == NULL)
        throw Exception_Memory(size+1);
    
    for (size_t k = 0; k < size; k++)
    {
        tmp[k].i = mas[k].i;
        tmp[k].j = mas[k].j;
        tmp[k].x = mas[k].x;
    }
    
    if (mas != NULL)
        delete[] mas;
    
    mas = tmp;
    size++;
    mas[size-1].i = i;
    mas[size-1].j = j;
    mas[size-1].x = x;
}

void Dictionary :: rem(size_t i, size_t j)
{
    size_t k;
    
    if (mas == NULL)
        return;
    
    for (k = 0; k < size; k++)
    {
        if ((mas[k].i == i) && (mas[k].j == j))
            break;
    }
    
    if ((size == 1) && (k == 0))
    {
        delete []mas;
        mas = NULL;
        size = 0;
    }
    
    Dictionary tmp;
    for (size_t i = 0; i < size; i++)
    {
        if (i != k)
            tmp.add(mas[i].i, mas[i].j, mas[i].x);
    }
    
    *this = tmp;
}

Dictionary :: ~Dictionary()
{
    delete[] mas;
}

RationalNumber Dictionary :: get(size_t i, size_t j) const
{
    for (size_t k = 0; k < size; k++)
    {
        if ((mas[k].i == i) && (mas[k].j == j))
            return mas[k].x;
    }
    return RationalNumber();
}

RationalNumber& Dictionary :: get_ref(size_t i, size_t j)
{
    for (size_t k = 0; k < size; k++)
    {
        if ((mas[k].i == i) && (mas[k].j == j))
            return mas[k].x;
    }
    return mas[0].x;
}

Dictionary Dictionary :: operator=(const Dictionary &b)
{
    delete[] mas;
    size = 0;
    mas = NULL;
    for (size_t i = 0; i < b.size; i++)
    {
        this->add(b.mas[i].i, b.mas[i].j, b.mas[i].x);
    }
    return *this;
}