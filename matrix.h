#ifndef matrix_h
#define matrix_h

#include "dictionary.h"
#include "rational_number.h"
struct Matrix_coords
{
    size_t i, j;
    Matrix_coords(){i = 1; j= 1;}
    Matrix_coords(size_t a, size_t b){i = a; j = b;}
};

struct Matrix_row_coord
{
    size_t i;
    Matrix_row_coord(){i = 1;}
    Matrix_row_coord(size_t a){i = a;}
};

struct Matrix_column_coord
{
    size_t j;
    Matrix_column_coord(){j = 1;}
    Matrix_column_coord(size_t a){j = a;}
};

class Matrix
{
protected:
    size_t _n;
    size_t _m;
    Dictionary _dictionary;
public:
    Matrix();
    Matrix(size_t, size_t, bool is_Identity = false);
    Matrix(size_t, size_t, const RationalNumber &);
    Matrix(const Matrix&);
    Matrix(FILE *);
    
    size_t get_n()const;
    size_t get_m()const;
    
    void read(FILE *);
    void write(FILE *);
    void print(FILE *);
    void set(size_t,size_t,const RationalNumber &);
    void make_elems_canonical();
    
    Matrix operator+(const Matrix&)const;
    Matrix operator-(const Matrix&)const;
    Matrix operator*(const Matrix&)const;
    Matrix operator-();
    Matrix operator~()const;
    Matrix operator*(const RationalNumber&)const;
    Matrix operator/(const RationalNumber&)const;
    Matrix operator=(const Matrix&);
    
    friend Matrix operator*(const RationalNumber&, const Matrix&);
    
    RationalNumber operator[](const Matrix_coords&)const;
    Matrix operator[](const Matrix_row_coord&)const;
    Matrix operator[](const Matrix_column_coord&)const;
};

#endif /* matrix_h */
