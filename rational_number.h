#ifndef RationalNumber_h
#define RationalNumber_h

#include "big_int.h"

class RationalNumber
{
private:
    BigInt _numerator;
    BigInt _denominator;
    
public:
    RationalNumber();
    RationalNumber(long int);
    RationalNumber(long int, long int);
    RationalNumber(char *, char*);
    RationalNumber(char *c);
    RationalNumber(const RationalNumber &);

    BigInt get_numerator();
    BigInt get_denominator();
    
    void print(FILE *, bool, bool make_bold);
    explicit operator int () const;
    explicit operator long int () const;
    explicit operator short () const;
    void make_canonical();
    void round();
    void floor();
    
    RationalNumber operator+(const RationalNumber &)const;
    RationalNumber operator-(const RationalNumber &)const;
    RationalNumber operator*(const RationalNumber &)const;
    RationalNumber operator/(const RationalNumber &)const;
    bool operator<(const RationalNumber &)const;
    bool operator<=(const RationalNumber &)const;
    bool operator>(const RationalNumber &)const;
    bool operator>=(const RationalNumber &)const;
    bool operator==(const RationalNumber &)const;
    bool operator!=(const RationalNumber &)const;
    RationalNumber operator++();
    RationalNumber operator--();
    RationalNumber operator+=(const RationalNumber &);
    RationalNumber operator-=(const RationalNumber &);
    RationalNumber operator*=(const RationalNumber &);
    RationalNumber operator/=(const RationalNumber &);
    RationalNumber operator=(const RationalNumber &);
    
    RationalNumber operator+(long int)const;
    RationalNumber operator-(long int)const;
    RationalNumber operator*(long int)const;
    RationalNumber operator/(long int)const;
    bool operator<(long int)const;
    bool operator<=(long int)const;
    bool operator>(long int)const;
    bool operator>=(long int)const;
    bool operator==(long int)const;
    bool operator!=(long int)const;
    RationalNumber operator+=(long int);
    RationalNumber operator-=(long int);
    RationalNumber operator*=(long int);
    RationalNumber operator/=(long int);
    RationalNumber operator=(long int);
    
    friend RationalNumber operator+(long int, const RationalNumber &);
    friend RationalNumber operator-(long int, const RationalNumber &);
    friend RationalNumber operator*(long int, const RationalNumber &);
    friend RationalNumber operator/(long int, const RationalNumber &);
    friend bool operator<(long int, const RationalNumber &);
    friend bool operator<=(long int, const RationalNumber &);
    friend bool operator>(long int, const RationalNumber &);
    friend bool operator>=(long int, const RationalNumber &);
    friend bool operator==(long int, const RationalNumber &);
    friend bool operator!=(long int, const RationalNumber &);
};

#endif
