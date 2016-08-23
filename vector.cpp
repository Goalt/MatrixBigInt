//
//  vector.cpp
//  Classes
//
//  Created by Yuri Konkov on 23.03.16.
//  Copyright © 2016 yuri. All rights reserved.
//

#include <stdio.h>
#include "vector.h"
#include "exception.h"
#include <string.h>

Vector :: Vector() : Matrix(){}

Vector :: Vector(size_t n) : Matrix(1,n){}

Vector :: Vector(const Vector& b) : Matrix(b){}

Vector ::  Vector(size_t n, const RationalNumber &x) : Matrix(1,n,x){}

Vector :: Vector(FILE *f)
{
    size_t p;
    long int coord1;
    size_t tmp_size = 0;
    char c = '\0', *tmp = NULL;
    
    if (f == NULL)
    {
        _n = _m = 1;
        _dictionary = Dictionary();
        return;
    }
    
    fscanf(f, "%c", &c);
    while ((c != EOF) && ((c == '\n') || (c == '#')))
    {
        if (c == '#')
        {
            while (c != '\n') fscanf(f, "%c", &c);
            fscanf(f, "%c", &c);
            continue;
        }
        if (c == '\n')
            fscanf(f, "%c", &c);
    }
    
    if (c == EOF)
    {
        printf("Matrix not found in the file\n");
        _n = _m = 1;
        _dictionary = Dictionary();
        return;
    }
    
    while (c != ' ')
    {
        tmp = (char*)realloc(tmp, tmp_size+1);
        tmp_size++;
        tmp[tmp_size-1] = c;
        if(fscanf(f, "%c", &c) == EOF)
            c = EOF;
    }
    tmp = (char*)realloc(tmp, tmp_size+1);
    tmp_size++;
    tmp[tmp_size-1] = '\0';
    
    if (strcmp(tmp, "vector") == 0)
    {
        fscanf(f, "%zu", &p);
        _n = 1;
        _m = p;
        
        _dictionary = Dictionary();
        
        fscanf(f, "%c", &c);
        while (c != EOF)
        {
            if (c == '#')
            {
                while (c != '\n') fscanf(f, "%c", &c);
                if(fscanf(f, "%c", &c) == EOF)
                    c = EOF;
                continue;
            }
            coord1 = -1;
            fscanf(f, "%zu", &coord1);
            
            free(tmp);
            tmp = NULL;
            tmp_size = 0;
            if(fscanf(f, "%c", &c) == EOF)
                c = EOF;
            while ((c != '\n') && (c!= EOF))
            {
                tmp = (char*)realloc(tmp, tmp_size+1);
                tmp_size++;
                tmp[tmp_size-1] = c;
                if(fscanf(f, "%c", &c) == EOF)
                    c = EOF;
            }
            tmp = (char*)realloc(tmp, tmp_size+1);
            tmp_size++;
            tmp[tmp_size-1] = '\0';
            if (coord1 != -1)
                set(coord1, RationalNumber(tmp));
            
            
            if (c == EOF)
                break;
        }
        free(tmp);
        
    }
    else
    {
        printf("Vector not found in the file\n");
        free(tmp);
        _n = _m = 1;
        _dictionary = Dictionary();
    }
}

void Vector :: print(FILE *f)
{
    for (size_t i = 1; i <= _n; i++)
    {
        for (size_t j = 1; j <= _m; j++)
        {
            if (_dictionary.get(i, j) != 0)
                _dictionary.get(i, j).print(f,0,1);
            else
                _dictionary.get(i, j).print(f,0,0);
            fprintf(f, " ");
        }
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
}

void Vector :: write(FILE *f)
{
    fprintf(f, "vector %zu\n", _m);
    for (size_t k = 0; k < _dictionary.size; k++)
    {
        fprintf(f, "%zu ", _dictionary.mas[k].j);
        _dictionary.mas[k].x.print(f, 1,0);
    }
}

size_t Vector :: get_n() const
{
    return Matrix::get_m();
}

RationalNumber Vector :: operator[](size_t i)const
{
    if ((i > Matrix::_m) || (i <= 0))
        throw Exception_Vector_border_out(Matrix::_m,i);
    
    return Matrix::_dictionary.get(1, i);
}

void Vector :: set(size_t i,const RationalNumber &b)
{
    if ((i > Matrix::_m) || (i <= 0))
        throw Exception_Vector_border_out(Matrix::_m,i);
    
    if (b == RationalNumber())
    {
        Matrix::_dictionary.rem(1, i);
        return;
    }
    
    Matrix::_dictionary.add(1, i, b);
}

Vector Vector :: operator+(const Vector &b)const
{
    Matrix m1(*this), m2(b);
    return matrix_to_vector(m1+m2);
}

Vector Vector :: operator-(const Vector &b)const
{
    Matrix m1(*this), m2(b);
    return matrix_to_vector(m1-m2);
}

Vector Vector :: operator*(const Matrix &b)const
{
    Matrix m1(*this), m2(b);
    return matrix_to_vector(m1*m2);
}

Vector Vector :: operator*(const RationalNumber &b)const
{
    Matrix m1(*this);
    return matrix_to_vector(m1*b);
}

Vector Vector :: operator/(const RationalNumber &b)const
{
    Matrix m1(*this);
    return matrix_to_vector(m1/b);
}

Vector Vector :: operator=(const Vector &b)
{
    this->Matrix::_m = b.Matrix::_m;
    this->Matrix::_dictionary = b.Matrix::_dictionary;
    return *this;
}

Vector matrix_to_vector(const Matrix &b)
{
    
    if (b.get_n() != 1)
    {
        printf("Invalid size of matrix\n");
        return Vector();
    }
    
    Vector res(b.get_m());
    Matrix_coords coord;
    for (size_t k = 1; k <= b.get_m(); k++)
    {
        coord = Matrix_coords(1,k);
        res.set(k, b[coord]);
    }
    return res;
}

