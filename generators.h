#ifndef generators_h
#define generators_h

#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include "matrix.h"

BigInt generate_rand_big_int(unsigned long int l = 0, unsigned long int r = 0);

RationalNumber generate_rand_rational_number(unsigned long int l, unsigned long int r);

Matrix generate_rand_matrix(unsigned long int l, unsigned long int r, size_t n, size_t m);

void generate_rand_rational_number(const RationalNumber &l, const RationalNumber &r, FILE *out);

void generate_rand_matrix(unsigned long int l, unsigned long int r, size_t n, size_t m, FILE *out);

#endif /* generators_h */
