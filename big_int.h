#ifndef BigInt_h
#define BigInt_h

#include <stdio.h>
#include <ctype.h>

/*!
    \brief Класс длинной арфметики, все умеет, где-то можно прооптимизировать.
*/

class BigInt
{
private:
    
    bool sign;
    size_t size;
    unsigned char *digits;
    
    void push_back(int);
    void push_front(int);
    void pop_back();
    
    void sum(const BigInt &);//Гарантируется что a и b >= 0
    void sub(const BigInt &);//Гарантируется что a >= b >= 0
    void mul(const BigInt &);//Гарантируется что a и b >= 0
    void div(const BigInt &);//Гарантируется что a > 0 и b > 0, и a > b
    
    void delete_zeros();
public:
    
    BigInt();
    BigInt(long int);
    BigInt(const BigInt &);
    BigInt(char *);

    void set_sign(bool);
    void print(FILE *, bool, bool);
    int to_int();
    long int to_long_int();
    short to_short();
    
    BigInt operator+(const BigInt &)const;
    BigInt operator-(const BigInt &)const;
    BigInt operator*(const BigInt &)const;
    BigInt operator/(const BigInt &)const;
    BigInt operator%(const BigInt &)const;
    
    bool operator<(const BigInt &)const;
    bool operator<=(const BigInt &)const;
    bool operator>(const BigInt &)const;
    bool operator>=(const BigInt &)const;
    bool operator==(const BigInt &)const;
    bool operator!=(const BigInt &)const;
    BigInt operator++();
    BigInt operator--();
    BigInt operator+=(const BigInt &);
    BigInt operator-=(const BigInt &);
    BigInt operator*=(const BigInt &);
    BigInt operator/=(const BigInt &);
    BigInt operator=(const BigInt &);
    
    BigInt operator+(long int)const;
    BigInt operator-(long int)const;
    BigInt operator*(long int)const;
    BigInt operator/(long int)const;
    BigInt operator%(long int)const;
    bool operator<(long int)const;
    bool operator<=(long int)const;
    bool operator>(long int)const;
    bool operator>=(long int)const;
    bool operator==(long int)const;
    bool operator!=(long int)const;
    BigInt operator+=(long int);
    BigInt operator-=(long int);
    BigInt operator*=(long int);
    BigInt operator/=(long int);
    BigInt operator=(long int);
    
    friend BigInt operator+(long int, const BigInt &);
    friend BigInt operator-(long int, const BigInt &);
    friend BigInt operator*(long int, const BigInt &);
    friend BigInt operator/(long int, const BigInt &);
    friend bool operator<(long int, const BigInt &);
    friend bool operator<=(long int, const BigInt &);
    friend bool operator>(long int, const BigInt &);
    friend bool operator>=(long int, const BigInt &);
    friend bool operator==(long int, const BigInt &);
    friend bool operator!=(long int, const BigInt &);
    
    ~BigInt();
    
};

#endif
