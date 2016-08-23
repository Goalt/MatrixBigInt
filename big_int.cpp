#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <cmath>

#include "exception.h"
#include "big_int.h"

/***********************************/

void BigInt :: push_back(int x)
{
    unsigned char *tmp;
    if (size == 0)
        digits = NULL;
    tmp = (unsigned char *)realloc(digits, (size + 1) * sizeof(char));
    if (tmp == NULL)
        throw Exception_Memory(size+1);
    digits = tmp;
    size++;
    digits[size-1] = x + '0';
}

void BigInt :: push_front(int x)
{
    unsigned char *tmp;
    if ((size == 1) && (digits[0] == '0'))
    {
        digits[0] = '0' + x;
        return;
    }
    if (size == 0)
        digits = NULL;
    tmp = (unsigned char *)realloc(digits, (size + 1) * sizeof(char));
    if (tmp == NULL)
        throw Exception_Memory(size+1);
    digits = tmp;
    size++;
    for (size_t i = 0; i < size-1; i++)
    {
        digits[size-1-i] = digits[size-2-i];
    }
    digits[0] = '0' + x;
}

void BigInt :: pop_back()
{
    unsigned char *tmp;
    
    if (size == 1)
        return;
    
    tmp = (unsigned char *)realloc(digits, (size - 1) * sizeof(char));
    if (tmp == NULL)
        throw Exception_Memory(size-1);
    digits = tmp;
    size--;
}

void BigInt :: sum(const BigInt &b)
{
    int carry = 0;
    for (size_t i=0; i< fmax(this->size,b.size) || carry; ++i)
    {
        if (i == this->size)
            (*this).push_back(0);
        this->digits[i] += carry + (i < b.size ? (b.digits[i] - '0') : 0);
        carry = this->digits[i] - '0' >= 10;
        if (carry)  this->digits[i] -= 10;
    }
}

void BigInt :: sub(const BigInt &b)
{
    int carry = 0;
    for (size_t i=0; i<b.size || carry; ++i) {
        this->digits[i] -= carry + (i < b.size ? b.digits[i] - '0' : 0);
        carry = this->digits[i] - '0' < 0;
        if (carry)  this->digits[i] += 10;
    }
    while (this->size > 1 && this->digits[this->size-1] == '0')
        this->pop_back();
}

void BigInt :: mul(const BigInt &b)
{
    BigInt res;
    
    for (size_t i = 0; i < this->size + b.size; i++)
    {
        res.push_back(0);
    }
    
    for (size_t i=0; i < this->size; i++)
    {
        int r = 0;
        for (size_t j=0; (j < b.size) | r; j++)
        {
            if (j < b.size)
                res.digits[i+j] += (this->digits[i] - '0') * (b.digits[j] - '0') + r;
            else
                res.digits[i+j] += r;
            
            r = (res.digits[i+j] - '0') / 10;
            res.digits[i+j] -= r*10;
        }
    }
    res.delete_zeros();
    
    free(this->digits);
    this->size = 0;
    for (size_t i = 0; i < res.size; i++)
    {
        this->push_back(res.digits[i] - '0');
    }
}

void BigInt :: div(const BigInt &b)
{
    BigInt res, tmp, cur;
    bool f = false;
    for(size_t i = 0; i < size;)
    {
        tmp.push_front(this->digits[size-1-i] -  '0');
        i++;
        while (tmp < b)
        {
            res.push_front(0);
            if (i >= this->size)
            {
                f = true;
                break;
            }
            tmp.push_front(this->digits[size-1-i] -  '0');
            i++;
        }
        if (f) break;
        int l = 0,r = 10,m, x = 0;
        while (l <= r)
        {
            m = (l+r) >> 1;
            cur = b*m;
            if (tmp >= cur)
            {
                x = m;
                l = m+1;
            }
            else
                r = m-1;
        }
        res.push_front(x);
        cur = b*x;
        tmp = tmp -cur;
    }
    
    res.delete_zeros();
    free(this->digits);
    this->size = 0;
    for (size_t i = 0; i < res.size; i++)
    {
        this->push_back(res.digits[i] - '0');
    }
    
}

void BigInt :: delete_zeros()
{
    while ((digits[size-1] == '0') && (size != 1))
    {
        pop_back();
    }
}

BigInt ::BigInt()
{
    sign = 1;
    size = 0;
    push_back(0);
}

BigInt ::BigInt(long int x)
{
    int count = 0;
    long int tmp = x;
    size = 0;
    
    while (tmp!=0)
    {
        tmp/=10;
        count++;
    }
    
    if (x >= 0)
        sign = 1;
    else
    {
        x = -x;
        sign = 0;
    }
    
    if(count == 0)
    {
        push_back(0);
        return;
    }
    
    while (x != 0)
    {
        push_back(x%10);
        x/=10;
    }
}

BigInt ::BigInt(const BigInt &x)
{
    unsigned char *tmp;
    tmp = (unsigned char*)malloc(sizeof(char) * x.size);
    if (tmp == NULL)
        throw Exception_Memory(x.size);
    digits = tmp;
    size = x.size;
    sign = x.sign;
    std :: memcpy(digits, x.digits, sizeof(char)*x.size);
}

BigInt ::BigInt(char *x)
{
    if (strlen(x) == 0)
    {
        sign = 1;
        size = 0;
        push_back(0);
        return;
    }
    
    size_t i = 0, size_x = strlen(x);
    
    if (x[0] == '-')
    {
        if (size_x == 1)
        {
            free(x);
            throw Exception_Invalid_string('\0');
        }
        
        for (i = 1; i < size_x; i++)
        {
            if (!isdigit(x[i]))
            {
                free(x);
                throw Exception_Invalid_string(x[i]);
            }
        }
        
        sign = 0;
        size = 0;
        for (i = 1;  i < size_x; i++)
        {
            push_back(x[size_x - i] - '0');
        }
        delete_zeros();
        return;
    }
    
    for (i = 0; i < size_x; i++)
    {
        if (!isdigit(x[i]))
            throw Exception_Invalid_string(x[i]);
    }
    
    sign = 1;
    size = 0;
    for (i = 1;  i <= size_x; i++)
    {
        push_back(x[size_x - i] - '0');
    }
    delete_zeros();
    return;
    
}

void BigInt ::set_sign(bool x)
{
    sign = x;
}

int BigInt :: to_int()
{
    BigInt max((unsigned int)-1);
    int flag = 1;
    int res = 0;
    
    if (sign == 0)
        flag = -1;
    
    if (*this * flag > max)
        throw Exception("Выход за пределы типа преобразования\n");

    for (size_t i = 0; i < size; i++)
    {
        if (i == size-1)
            res += (digits[size - 1 - i] - '0');
        else
        {
            res += (digits[size - 1 - i] - '0');
            res *= 10;
        }
    }
    res *= flag;
    
    return res;
}

long int BigInt :: to_long_int()
{
    BigInt max((unsigned long int)-1);
    int res = 0;
    if (sign == 1)
    {
        if (*this > max)
            throw Exception("Выход за пределы типа преобразования\n");
        
        for (size_t i = 0; i < size; i++)
        {
            if (i == size-1)
                res += (digits[size - 1 - i] - '0');
            else
            {
                res += (digits[size - 1 - i] - '0');
                res *= 10;
            }
        }
        
    }
    if (sign == 0)
    {
        if ((*this) * (-1) > max)
            throw Exception("Выход за пределы типа преобразования\n");
        
        for (size_t i = 0; i < size; i++)
        {
            if (i == size - 1)
                res += (digits[size - 1 - i] - '0');
            else
            {
                res += (digits[size - 1 - i] - '0');
                res *= 10;
            }
        }
        res *= -1;
    }
    return res;
}

short BigInt :: to_short()
{
    BigInt max((unsigned short)-1);
    int res = 0;
    if (sign == 1)
    {
        if (*this > max)
            throw Exception("Выход за пределы типа преобразования\n");
        
        for (size_t i = 0; i < size; i++)
        {
            if (i == size-1)
                res += (digits[size - 1 - i] - '0');
            else
            {
                res += (digits[size - 1 - i] - '0');
                res *= 10;
            }
        }
        
    }
    if (sign == 0)
    {
        if ((*this) * (-1) > max)
            throw Exception("Выход за пределы типа преобразования\n");
        
        for (size_t i = 0; i < size; i++)
        {
            if (i == size - 1)
                res += (digits[size - 1 - i] - '0');
            else
            {
                res += (digits[size - 1 - i] - '0');
                res *= 10;
            }
        }
        res *= -1;
    }
    return res;
}

void BigInt ::print(FILE *f,bool flag_enter, bool make_bold)
{
    if(sign == 0)
        fprintf(f,"-");
    
    for (size_t i = 0; i < size; i++)
    {   if (make_bold)
            fprintf(f,"\e[1m%c\e[0m", digits[size - 1 -i]);
        else
            fprintf(f,"%c", digits[size - 1 -i]);
    }
    
    if (flag_enter)
        fprintf(f,"\n");
}

BigInt ::~BigInt()
{
    if (digits != NULL)
        free(digits);
}

/***********************************/

BigInt BigInt :: operator+(const BigInt &b) const
{
    BigInt c(b), res, d(*this);
    bool sign_a = this->sign, sign_c = c.sign;
    c.set_sign(1);
    d.set_sign(1);
    
    if (d >= c)
    {
        if ((sign_a == 0) && (sign_c == 0))
        {
            res = d;
            res.sum(c);
            res.set_sign(0);
        }
        if ((sign_a == 0) && (sign_c == 1))
        {
            res = d;
            res.sub(c);
            res.set_sign(0);
        }
        if ((sign_a == 1) && (sign_c == 0))
        {
            res = d;
            res.sub(c);
            res.set_sign(1);
        }
        if ((sign_a == 1) && (sign_c == 1))
        {
            res = d;
            res.sum(c);
            res.set_sign(1);
        }
    }
    else
    {
        if ((sign_a == 0) && (sign_c == 0))
        {
            res = d;
            res.sum(c);
            res.set_sign(0);
        }
        if ((sign_a == 0) && (sign_c == 1))
        {
            res = c;
            res.sub(d);
            res.set_sign(1);
        }
        if ((sign_a == 1) && (sign_c == 0))
        {
            res = c;
            res.sub(d);
            res.set_sign(0);
        }
        if ((sign_a == 1) && (sign_c == 1))
        {
            res = d;
            res.sum(c);
            res.set_sign(1);
        }
    }
    sign_a = res.sign;
    res.set_sign(1);
    if (res != 0)
        res.sign = sign_a;
    return res;
}

BigInt BigInt :: operator-(const BigInt &b) const
{
    BigInt c(b), res, d(*this);
    bool sign_a = this->sign, sign_c = c.sign;
    c.set_sign(1);
    d.set_sign(1);
    
    if (d >= c)
    {
        if ((sign_a == 0) && (sign_c == 0))
        {
            res = d;
            res.sub(c);
            res.set_sign(0);
        }
        if ((sign_a == 0) && (sign_c == 1))
        {
            res = d;
            res.sum(c);
            res.set_sign(0);
        }
        if ((sign_a == 1) && (sign_c == 0))
        {
            res = d;
            res.sum(c);
            res.set_sign(1);
        }
        if ((sign_a == 1) && (sign_c == 1))
        {
            res = d;
            res.sub(c);
            res.set_sign(1);
        }
    }
    else
    {
        if ((sign_a == 0) && (sign_c == 0))
        {
            res = c;
            res.sub(d);
            res.set_sign(1);
        }
        if ((sign_a == 0) && (sign_c == 1))
        {
            res = c;
            res.sum(d);
            res.set_sign(0);
        }
        if ((sign_a == 1) && (sign_c == 0))
        {
            res = c;
            res.sum(d);
            res.set_sign(1);
        }
        if ((sign_a == 1) && (sign_c == 1))
        {
            res = c;
            res.sub(d);
            res.set_sign(0);
        }
    }
    
    sign_a = res.sign;
    res.set_sign(1);
    if (res != 0)
        res.sign = sign_a;
    return res;
}

BigInt BigInt :: operator*(const BigInt &b) const
{
    BigInt res(*this);
    res.mul(b);
    
    if((this->sign == 0) && (b.sign == 0))
        res.sign = 1;
    else
        res.sign = this->sign * b.sign;
    
    return res;
}

BigInt BigInt :: operator/(const BigInt &b) const
{
    BigInt res(*this), c(b);
    
    if (b == 0)
        throw Exception((char*)"Division by zero");
    
    if (*this == 0)
    {
        res = 0;
        return res;
    }
    
    res.set_sign(1);
    c.set_sign(1);
    if (res < c)
    {
        res = 0;
        return res;
    }
    
    res.div(b);
    
    if((this->sign == 0) && (b.sign == 0))
        res.sign = 1;
    else
        res.sign = this->sign * b.sign;
    
    return res;
}

BigInt BigInt :: operator%(const BigInt &b)const
{
    return *this - (*this/b) * b;
}

BigInt BigInt :: operator++()
{
    return *this + BigInt(1);
}

BigInt BigInt :: operator--()
{
    return *this - BigInt(-1);
}

BigInt BigInt :: operator+=(const BigInt &b)
{
    *this = *this + b;
    return *this;
}

BigInt BigInt :: operator-=(const BigInt &b)
{
    *this = *this - b;
    return *this;
}

BigInt BigInt :: operator*=(const BigInt &b)
{
    *this = *this * b;
    return *this;
}

BigInt BigInt :: operator/=(const BigInt &b)
{
    *this = *this / b;
    return *this;
}

bool BigInt :: operator<(const BigInt &x) const
{
    if (this->sign != x.sign)
    {
        if(this->sign == 0)
            return true;
        else
            return false;
    }
    
    if (this -> sign == 0)
    {
        if (this->size > x.size)
            return true;
        if (this->size < x.size)
            return false;
        
        for (size_t i = 0; i < x.size; i++)
        {
            if (this->digits[x.size - i - 1] > x.digits[x.size - i - 1])
                return true;
            if (this->digits[x.size - i - 1] < x.digits[x.size - i - 1])
                return false;
        }
        return false;
    }
    
    if (this->size > x.size)
        return false;
    if (this->size < x.size)
        return true;
    
    for (size_t i = 0; i < x.size; i++)
    {
        if (this->digits[x.size - i - 1] > x.digits[x.size - i - 1])
            return false;
        if (this->digits[x.size - i - 1] < x.digits[x.size - i - 1])
            return true;
    }
    return false;
}

bool BigInt :: operator<=(const BigInt &x) const
{
    return !(*this > x);
}

bool BigInt :: operator==(const BigInt &x) const
{
    if (this->sign != x.sign)
        return false;
    if (this->size != x.size)
        return false;
    for (size_t i = 0; i < x.size; i++)
    {
        if(this->digits[i] != x.digits[i])
            return false;
    }
    return true;
}

bool BigInt :: operator!=(const BigInt &x) const
{
    return !(*this == x);
}

bool BigInt :: operator>(const BigInt &x) const
{
    return (!(*this < x) && (*this != x));
}

bool BigInt :: operator>=(const BigInt &x) const
{
    return !(*this < x);
}

BigInt BigInt :: operator=(const BigInt &b)
{
    if (this->size != 0)
    {
        free(this->digits);
        this->size = 0;
    }
    
    this->sign = b.sign;
    
    for (size_t i = 0; i < b.size; i++)
    {
        this->push_back(b.digits[i] - '0');
    }
    return *this;
}

/***********************************/

BigInt BigInt :: operator=(long int x)
{
    BigInt res(x);
    *this = res;
    return *this;
}

BigInt BigInt :: operator*(long int x) const
{
    BigInt res(x);
    res = res * (*this);
    return res;
}

BigInt BigInt :: operator+(long int x) const
{
    return *this + BigInt(x);
}

BigInt BigInt :: operator-(long int x) const
{
    return *this - BigInt(x);
}

BigInt BigInt :: operator/(long int x) const
{
    return *this / BigInt(x);
}

BigInt BigInt :: operator%(long int x)const
{
    return *this - (*this/x) * x;
}

bool BigInt :: operator<(long int x) const
{
    return *this < BigInt(x);
}

bool BigInt :: operator<=(long int x) const
{
    return *this <= BigInt(x);
}

bool BigInt :: operator>(long int x) const
{
    return *this > BigInt(x);
}

bool BigInt :: operator>=(long int x) const
{
    return *this >= BigInt(x);
}

bool BigInt :: operator==(long int x) const
{
    return *this == BigInt(x);
}

bool BigInt :: operator!=(long int x) const
{
    return *this != BigInt(x);
}

BigInt BigInt :: operator+=(long int x)
{
    return *this +=BigInt(x);
}

BigInt BigInt :: operator-=(long int x)
{
    return *this -=BigInt(x);
}

BigInt BigInt :: operator*=(long int x)
{
    return *this *=BigInt(x);
}

BigInt BigInt :: operator/=(long int x)
{
    return *this /=BigInt(x);
}

/***********************************/

BigInt operator+(long int a, const BigInt &b)
{
    return BigInt(a) + b;
}

BigInt operator-(long int a, const BigInt &b)
{
    return BigInt(a) - b;
}

BigInt operator*(long int a, const BigInt &b)
{
    return BigInt(a) * b;
}

BigInt operator/(long int a, const BigInt &b)
{
    return BigInt(a) / b;
}

bool operator<(long int a, const BigInt &b)
{
    return BigInt(a) < b;
}

bool operator<=(long int a, const BigInt &b)
{
    return BigInt(a) <= b;
}

bool operator>(long int a, const BigInt &b)
{
    return BigInt(a) > b;
}

bool operator>=(long int a, const BigInt &b)
{
    return BigInt(a) >= b;
}

bool operator==(long int a, const BigInt &b)
{
    return BigInt(a) == b;
}

bool operator!=(long int a, const BigInt &b)
{
    return BigInt(a) != b;
}
