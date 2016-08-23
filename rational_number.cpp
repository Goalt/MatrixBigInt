#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "exception.h"
#include "big_int.h"
#include "rational_number.h"

RationalNumber :: RationalNumber()
{
    _numerator = 0;
    _denominator = 0;
}

RationalNumber :: RationalNumber(long int x)
{
    _numerator = x;
    _denominator = 1;
}

RationalNumber :: RationalNumber(long int numerator, long int denominator)
{
    _numerator = numerator;
    if (numerator == 0)
        _denominator = 0;
    else if(denominator == 0)
        throw Exception("Деление на ноль\n");
    else
        _denominator = denominator;
    
    if (_denominator < 0)
    {
        _numerator *= -1;
        _denominator *= -1;
    }
}

RationalNumber :: RationalNumber(char *s1, char *s2)
{
    BigInt a(s1), b(s2);
    
    _numerator = a;
    if (a == 0)
        _denominator = 0;
    else if(b == 0)
        throw Exception("Деление на ноль\n");
    else
        _denominator = b;
    
    if (_denominator < 0)
    {
        _numerator *= -1;
        _denominator *= -1;
    }
}

RationalNumber :: RationalNumber(const RationalNumber &b)
{
    _numerator = b._numerator;
    _denominator = b._denominator;
}

RationalNumber :: RationalNumber(char *temp)
{
    size_t i = 0, j =0;
    size_t size = strlen(temp);
    char c = '\0', *tmp2 = NULL, *tmp3 = NULL, *checker = NULL;
    
    if (size == 0)
    {
        _numerator = 0;
        _denominator = 0;
        return;
    }
    while (i < size)
    {
        c = temp[i];
        i++;
        if (c != ' ')
        {
            j++;
            checker = (char*) realloc(tmp2, j);
            if (checker == NULL)
            {
                free(tmp2);
                throw Exception_Memory(j);
            }
            tmp2 = checker;
            tmp2[j-1] = c;
        }
    }
    
    i = 0;
    size = j;
    j = 0;
    c = tmp2[0];
    checker = NULL;
    _numerator = 0;
    _denominator = 1;
    while ((i < size) && (c != '/'))
    {
        j++;
        checker = (char*) realloc(tmp3, j);
        if (checker == NULL)
        {
            free(tmp3);
            free(tmp2);
            throw Exception_Memory(j);
        }
        tmp3 = checker;
        tmp3[j-1] = c;
        i++;
        if (i < size)
            c = tmp2[i];
    }
    j++;
    checker = (char*) realloc(tmp3, j);
    if (checker == NULL)
    {
        free(tmp3);
        free(tmp2);
        throw Exception_Memory(j);
    }
    tmp3 = checker;
    tmp3[j-1] = '\0';
    try
    {
        _numerator = BigInt(tmp3);
    }
    catch (Exception_Invalid_string &ex)
    {
        free(tmp3);
        free(tmp2);
        throw ex;
    }
    
    j = 0;
    i++;
    if (i < size)
        c = tmp2[i];
    free(tmp3);
    tmp3 = NULL;
    checker = NULL;
    while ((i < size) && (c != '/'))
    {
        j++;
        checker = (char*) realloc(tmp3, j);
        if (checker == NULL)
        {
            free(tmp3);
            free(tmp2);
            throw Exception_Memory(j);
        }
        tmp3 = checker;
        tmp3[j-1] = c;
        i++;
        if (i < size)
            c = tmp2[i];
    }
    j++;
    checker = (char*) realloc(tmp3, j);
    if (checker == NULL)
    {
        free(tmp3);
        free(tmp2);
        throw Exception_Memory(j);
    }
    tmp3 = checker;
    tmp3[j-1] = '\0';
    
    if (j == 1)
        _denominator = 1;
    else
    {
        try
        {
            _denominator = BigInt(tmp3);
        }
        catch (Exception_Invalid_string &ex)
        {
            free(tmp3);
            free(tmp2);
            throw ex;
        }
    }
    
    if(_denominator == 0)
    {
        free(tmp3);
        free(tmp2);
        throw Exception("Деление на ноль\n");
    }
    
    if (_numerator == 0)
        _denominator = 0;
    
    free(tmp3);
    free(tmp2);
}

/***********************************************************/

void RationalNumber:: print(FILE *f, bool flag_enter, bool make_bold)
{
    if ((_numerator == 0) || (_denominator == 1))
    {
        _numerator.print(f, 0, make_bold);
        if (flag_enter)
            fprintf(f, "\n");
        return;
    }
    
    _numerator.print(f, 0, make_bold);
    if (make_bold)
        fprintf(f, "\e[1m\\\e[0m");
    else
        fprintf(f, "/");
    _denominator.print(f, 0, make_bold);
    if (flag_enter)
        fprintf(f, "\n");
}

RationalNumber :: operator int() const
{
    if(*this == RationalNumber())
        return 0;
    return (_numerator / _denominator).to_int();
}

RationalNumber :: operator long() const
{
    if(*this == RationalNumber())
        return 0;
    return (_numerator / _denominator).to_long_int();
}

RationalNumber :: operator short() const
{
    if(*this == RationalNumber())
        return 0;
    return (_numerator / _denominator).to_short();
}

void RationalNumber :: floor()
{
    if(*this == RationalNumber())
        return ;
    _numerator = _numerator / _denominator;
    if (_numerator == 0)
        _denominator = 0;
    else
        _denominator = 1;
}

void RationalNumber :: round()
{
    if(*this == RationalNumber())
        return ;
    RationalNumber tmp;
    tmp._numerator = this->_numerator % _denominator;
    tmp._denominator = this->_denominator;
    
    if (tmp >= RationalNumber(1,2))
        this->_numerator += tmp._denominator - tmp._numerator;
    else
        this->_numerator -= tmp._numerator;
    
    if (this->_numerator == 0)
        this->_denominator = 0;
    
}

void RationalNumber :: make_canonical()
{
    bool sign = 1;
    BigInt tmp, numerator(_numerator), denominator(_denominator);
    if (_numerator < 0)
    {
        numerator.set_sign(1);
        sign = 0;
    }
    
    if (*this == RationalNumber())
        return;
    while ((numerator != 0) && (denominator != 0))
    {
        if (numerator > denominator)
            numerator = numerator % denominator;
        else
            denominator = denominator % numerator;
    }
    tmp = numerator + denominator;
    _numerator /= tmp;
    _denominator /= tmp;
    _numerator.set_sign(sign);
}

/***********************************************************/

BigInt RationalNumber:: get_numerator()
{
    return _numerator;
}

BigInt RationalNumber:: get_denominator()
{
    return _denominator;
}

/***********************************************************/

RationalNumber RationalNumber :: operator+(const RationalNumber &b)const
{
    if (*this == RationalNumber())//Сравнения с нулем
        return b;
    if (b == RationalNumber())
        return *this;
    
    RationalNumber res;
    res._numerator = this->_numerator * b._denominator + b._numerator * this->_denominator;
    res._denominator = this->_denominator * b._denominator;
    
    if (res._numerator == 0)
        res._denominator = 0;
    
    return res;
}

RationalNumber RationalNumber :: operator-(const RationalNumber &b)const
{
    RationalNumber tmp;
    if (*this == RationalNumber())//Сравнения с нулем
    {
        tmp = b;
        tmp._numerator *= -1;
        return tmp;
    }
    if (b == RationalNumber())
        return *this;
    
    RationalNumber res;
    res._numerator = this->_numerator * b._denominator - b._numerator * this->_denominator;
    res._denominator = this->_denominator * b._denominator;
    
    if (res._numerator == 0)
        res._denominator = 0;
    
    return res;
}

RationalNumber RationalNumber :: operator*(const RationalNumber &b)const
{
    if (*this == RationalNumber())//Сравнения с нулем
        return RationalNumber();
    if (b == RationalNumber())
        return RationalNumber();
    
    RationalNumber res;
    res._numerator = this->_numerator * b._numerator;
    res._denominator = this->_denominator * b._denominator;
    
    return res;
}

RationalNumber RationalNumber :: operator/(const RationalNumber &b)const
{
    if (*this == RationalNumber())//Сравнения с нулем
        return RationalNumber();
    if (b == RationalNumber())
        throw Exception("Деление на ноль\n");
    
    RationalNumber res;
    res._numerator = this->_numerator * b._denominator;
    res._denominator = this->_denominator * b._numerator;
    
    return res;
}

bool RationalNumber :: operator<(const RationalNumber &b)const
{
    if (b._denominator == 0)
        return this->_numerator < 0;
    
    if (this->_denominator == 0)
        return b._numerator > 0;
    
    return this->_numerator * b._denominator < b._numerator * this->_denominator;
}

bool RationalNumber :: operator<=(const RationalNumber &b)const
{
    if (b._denominator == 0)
        return this->_numerator <= 0;
    
    if (this->_denominator == 0)
        return b._numerator >= 0;
    
    return this->_numerator * b._denominator <= b._numerator * this->_denominator;
}

bool RationalNumber :: operator>(const RationalNumber &b)const
{
    if (b._denominator == 0)
        return this->_numerator > 0;
    
    if (this->_denominator == 0)
        return b._numerator < 0;
    
    return this->_numerator * b._denominator > b._numerator * this->_denominator;
}

bool RationalNumber :: operator>=(const RationalNumber &b)const
{
    if (b._denominator == 0)
        return this->_numerator >= 0;
    
    if (this->_denominator == 0)
        return b._numerator <= 0;
    
    return this->_numerator * b._denominator >= b._numerator * this->_denominator;
}

bool RationalNumber :: operator==(const RationalNumber &b)const
{
    if (b._denominator == 0)
        return this->_numerator == 0;
    
    if (this->_denominator == 0)
        return b._numerator == 0;
    
    return this->_numerator * b._denominator == b._numerator * this->_denominator;
}

bool RationalNumber :: operator!=(const RationalNumber &b)const
{
    return !(*this == b);
}

RationalNumber RationalNumber:: operator=(const RationalNumber &b)
{
    this->_numerator = b._numerator;
    this->_denominator = b._denominator;
    return *this;
}

RationalNumber RationalNumber :: operator++()
{
    return *this + RationalNumber(1,1);
}

RationalNumber RationalNumber :: operator--()
{
    return *this - RationalNumber(1,1);
}

RationalNumber RationalNumber :: operator+=(const RationalNumber &b)
{
    *this = *this + b;
    return *this;
}

RationalNumber RationalNumber :: operator-=(const RationalNumber &b)
{
    *this = *this - b;
    return *this;
}

RationalNumber RationalNumber :: operator*=(const RationalNumber &b)
{
    *this = *this * b;
    return *this;
}

RationalNumber RationalNumber :: operator/=(const RationalNumber &b)
{
    *this = *this / b;
    return *this;
}

/***********************************************************/

RationalNumber RationalNumber :: operator+(long int b)const
{
    return *this + RationalNumber(b);
}

RationalNumber RationalNumber :: operator-(long int b)const
{
    return *this - RationalNumber(b);
}

RationalNumber RationalNumber :: operator*(long int b)const
{
    return *this * RationalNumber(b);
}

RationalNumber RationalNumber :: operator/(long int b)const
{
    return *this / RationalNumber(b);
}

bool RationalNumber :: operator<(long int b)const
{
    return *this < RationalNumber(b);
}

bool RationalNumber :: operator<=(long int b)const
{
    return *this <= RationalNumber(b);
}

bool RationalNumber :: operator>(long int b)const
{
    return *this > RationalNumber(b);
}

bool RationalNumber :: operator>=(long int b)const
{
    return *this >= RationalNumber(b);
}

bool RationalNumber :: operator==(long int b)const
{
    return *this == RationalNumber(b);
}

bool RationalNumber :: operator!=(long int b)const
{
    return *this != RationalNumber(b);
}

RationalNumber RationalNumber:: operator+=(long int b)
{
    *this += RationalNumber(b);
    return *this;
}

RationalNumber RationalNumber :: operator-=(long int b)
{
    *this -= RationalNumber(b);
    return *this;
}

RationalNumber RationalNumber :: operator*=(long int b)
{
    *this *= RationalNumber(b);
    return *this;
}

RationalNumber RationalNumber :: operator/=(long int b)
{
    *this /= RationalNumber(b);
    return *this;
}

RationalNumber RationalNumber :: operator=(long int b)
{
    *this = RationalNumber(b);
    return *this;
}

/***********************************************************/

RationalNumber operator+(long int a, const RationalNumber &b)
{
    return b+a;
}

RationalNumber operator-(long int a, const RationalNumber &b)
{
    return b-a;
}

RationalNumber operator*(long int a, const RationalNumber &b)
{
    return b*a;
}

RationalNumber operator/(long int a, const RationalNumber &b)
{
    return b/a;
}

bool operator<(long int a, const RationalNumber &b)
{
    return b < a;
}

bool operator<=(long int a, const RationalNumber &b)
{
    return b <= a;
}

bool operator>(long int a, const RationalNumber &b)
{
    return b > a;
}

bool operator>=(long int a, const RationalNumber &b)
{
    return b >= a;
}

bool operator==(long int a, const RationalNumber &b)
{
    return b == a;
}

bool operator!=(long int a, const RationalNumber &b)
{
    return b != a;
}
