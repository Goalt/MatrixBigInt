#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include "matrix.h"
#include "generators.h"

BigInt generate_rand_big_int(unsigned long int l, unsigned long int r)
{
    return BigInt(rand()%(r-l+1) + l);
}

RationalNumber generate_rand_rational_number(unsigned long int l, unsigned long int r)
{
    return RationalNumber(rand()%(r-l+1) + l, 1);
}

Matrix generate_rand_matrix(unsigned long int l, unsigned long int r, size_t n, size_t m)
{
    Matrix res(n,m);
    for (size_t i = 1; i <= n ; i++)
    {
        for (size_t j = 1; j <= m; j++)
        {
            res.set(i, j, generate_rand_rational_number(l,r));
        }
    }
    return res;
}

void generate_rand_rational_number(unsigned long int l, unsigned long int r, FILE *out)
{
    generate_rand_rational_number(l, r).print(out, 1, 0);
}

void generate_rand_matrix(unsigned long int l, unsigned long int r, size_t n, size_t m, FILE *out)
{
    Matrix tmp = generate_rand_matrix(l, r, n, m);
    tmp.write(out);
}