#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "matrix.h"
#include "exception.h"

Matrix :: Matrix()
{
    _n = _m = 1;
    _dictionary = Dictionary();
}

Matrix :: Matrix(size_t i, size_t j, bool is_Identity)
{
    if ((i <= 0) || (j <= 0))
        throw Exception("Invalid size");
    
    if (is_Identity && (i != j))
    {
        _n = _m = 1;
        _dictionary = Dictionary();
        throw Exception("Identity matrix must be a square matrix");
    }
    _n = i;
    _m = j;
    _dictionary = Dictionary();
    if (is_Identity)
    {
        for (size_t k = 1; k <= _n; k++)
        {
            _dictionary.add(k, k, RationalNumber(1));
        }
    }
}

Matrix :: Matrix(size_t i, size_t j, const RationalNumber &x)
{
    if ((i <= 0) || (j <= 0))
        throw Exception("Invalid size");
    
    _n = i;
    _m = j;
    _dictionary = Dictionary();
    if (x == 0)
        return;
    for (size_t k = 1; k <= _n; k++)
    {
        for (size_t l = 1; l <= _m; l++)
        {
            _dictionary.add(k, l, x);
        }
    }
}

Matrix :: Matrix(const Matrix &x)
{
    _n = x._n;
    _m = x._m;
    _dictionary = Dictionary(x._dictionary);
}

Matrix :: Matrix(FILE *f)
{
    size_t p;
    long int coord1, coord2;
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
    
    if (strcmp(tmp, "matrix") == 0)
    {
        fscanf(f, "%zu", &p);
        _n = p;
        
        fscanf(f, "%zu", &p);
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
            fscanf(f, "%zu", &coord2);
            
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
                set(coord1, coord2, RationalNumber(tmp));
            
            
            if (c == EOF)
                break;
        }
        free(tmp);
        
    }
    else
    {
        printf("Matrix not found in the file\n");
        free(tmp);
        _n = _m = 1;
        _dictionary = Dictionary();
    }
    
}

/*****************************************************/

size_t Matrix :: get_n()const
{
    return _n;
}

size_t Matrix :: get_m()const
{
    return _m;
}

/*****************************************************/

void Matrix :: write(FILE *f)
{
    fprintf(f, "matrix %zu %zu\n", _n, _m);
    for (size_t k = 0; k < _dictionary.size; k++)
    {
        fprintf(f, "%zu %zu ", _dictionary.mas[k].i, _dictionary.mas[k].j);
        _dictionary.mas[k].x.print(f, 1, 0);
    }
}

void Matrix :: read(FILE *f)
{
    Matrix tmp(f);
    *this = tmp;
}

void Matrix :: print(FILE *f)
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

void Matrix :: set(size_t i, size_t j, const RationalNumber &x)
{
    if ((i > _n) || (i <= 0) || (j > _m) || (j <= 0))
        throw Exception_Matrix_border_out(_n, _m, i, j);
    
    if (x == RationalNumber((char*)"0") && (_dictionary.get(i, j) == RationalNumber()))
        return;
    
    if (x == RationalNumber((char*)"0"))
    {
        _dictionary.rem(i, j);
        return;
    }
    
    _dictionary.add(i, j, x);
}

void Matrix :: make_elems_canonical()
{
    for (size_t i = 0; i < _dictionary.size; i++)
    {
        _dictionary.mas[i].x.make_canonical();
    }
}

/*****************************************************/

Matrix Matrix ::operator+(const Matrix &b)const
{
    if ((_n != b._n) || (_m != b._m))
        throw Exception_matrix_sizes(_n, _m, b._n, b._m);
    
    Matrix_coords coords;
    Matrix res(*this);
    for (size_t k = 0; k < b._dictionary.size; k++)
    {
        coords.i = b._dictionary.mas[k].i;
        coords.j = b._dictionary.mas[k].j;
        res.set(coords.i, coords.j, res[coords] + b._dictionary.mas[k].x);
    }
    return res;
}

Matrix Matrix ::operator-(const Matrix &b)const
{
    if ((_n != b._n) || (_m != b._m))
        throw Exception_matrix_sizes(_n, _m, b._n, b._m);
    
    Matrix_coords coords;
    Matrix res(*this);
    for (size_t k = 0; k < b._dictionary.size; k++)
    {
        coords.i = b._dictionary.mas[k].i;
        coords.j = b._dictionary.mas[k].j;
        res.set(coords.i, coords.j, res[coords] - b._dictionary.mas[k].x);
    }
    return res;
}

Matrix Matrix :: operator=(const Matrix &b)
{
    _n = b._n;
    _m = b._m;
    _dictionary = b._dictionary;
    return *this;
}

Matrix Matrix :: operator*(const RationalNumber &b)const
{
    Matrix res(*this);
    for (size_t k = 0; k < _dictionary.size; k++)
    {
        res._dictionary.mas[k].x *= b;
    }
    return res;
}

Matrix Matrix :: operator/(const RationalNumber &b)const
{
    Matrix res(*this);
    for (size_t k = 0; k < _dictionary.size; k++)
    {
        res._dictionary.mas[k].x /= b;
    }
    return res;
}

Matrix Matrix :: operator*(const Matrix &b)const
{
    if (_m != b._n)
        throw Exception_matrix_sizes(_n, _m, b._n, b._m);
    Matrix_coords coord1, coord2;
    RationalNumber tmp;
    Matrix res(_n,b._m,0);
    for (size_t i = 1; i <= _n; i++)
    {
        coord1.i = i;
        for (size_t j = 1; j <= b._m; j++)
        {
            tmp = 0;
            coord2.j = j;
            for (size_t k = 1; k <= _m; k++)
            {
                coord1.j = k;
                coord2.i = k;
                tmp += (*this)[coord1] * b[coord2];
            }
            res.set(i, j, tmp);
        }
    }
    return res;
}

Matrix Matrix ::operator-()
{
    Matrix res(*this);
    for (size_t k = 0; k < _dictionary.size; k++)
    {
        res._dictionary.mas[k].x *= -1;
    }
    return res;
}

Matrix Matrix :: operator~()const
{
    size_t tmp;
    Matrix res(*this);
    tmp = res._n;
    res._n = res._m;
    res._m = tmp;
    for (size_t k = 0; k < _dictionary.size; k++)
    {
        tmp = res._dictionary.mas[k].i;
        res._dictionary.mas[k].i = res._dictionary.mas[k].j;
        res._dictionary.mas[k].j = tmp;
    }
    return res;
}

Matrix operator*(const RationalNumber &rat, const Matrix &m)
{
    return m*rat;
}

/*****************************************************/

RationalNumber Matrix :: operator[](const Matrix_coords &a)const
{
    if ((a.i > _n) || (a.i <= 0) || (a.j > _m) || (a.j <= 0))
        throw Exception_Matrix_border_out(_n, _m, a.i, a.j);
    return _dictionary.get(a.i, a.j);
}

Matrix Matrix :: operator[](const Matrix_row_coord& row)const
{
    if ((row.i <= 0) || (row.i > _n))
        throw Exception_Matrix_border_out(_n, _m, row.i, 1);
    Matrix_coords coord;
    coord.i = row.i;
    Matrix res(1, _m, 0);
    for (size_t k = 1; k <= _m; k++)
    {
        coord.j = k;
        res.set(1, k, (*this)[coord]);
    }
    return res;
}


Matrix Matrix :: operator[](const Matrix_column_coord& col)const
{
    if ((col.j <= 0) || (col.j > _n))
        throw Exception_Matrix_border_out(_n, _m, 1, col.j);
    Matrix_coords coord;
    coord.j = col.j;
    Matrix res(_n, 1, 0);
    for (size_t k = 1; k <= _n; k++)
    {
        coord.i = k;
        res.set(k, 1, (*this)[coord]);
    }
    return res;
}

