#ifndef vector_h
#define vector_h
#include "stdlib.h"
#include "matrix.h"
#include "rational_number.h"

class Vector : protected Matrix
{
    
public:
    Vector();
    Vector(size_t n);
    Vector(const Vector &);
    Vector(FILE *);
    Vector(size_t n, const RationalNumber&);
    
    void write(FILE *f);
    
    void print(FILE *f);
    
    RationalNumber operator[](size_t i)const;
    
    void set(size_t,const RationalNumber &);
    
    size_t get_n()const;
    
    Vector operator+(const Vector&)const;
    Vector operator-(const Vector&)const;
    Vector operator*(const Matrix&)const;
    Vector operator*(const RationalNumber&)const;
    Vector operator/(const RationalNumber&)const;
    Vector operator=(const Vector&);
};


Vector matrix_to_vector(const Matrix&);
#endif /* vector_h */
