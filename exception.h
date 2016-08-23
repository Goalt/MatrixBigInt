#ifndef Exception_hpp
#define Exception_hpp

#include <cstdio>

class Exception
{
private:
    char *message;
    
public:
    Exception();
    Exception(const char *);
    Exception(const Exception &);
    
    void print(FILE *);
    
    ~Exception();
};

class Exception_Memory:protected Exception
{
    size_t _bytes;
public:
    Exception_Memory(size_t n_bytes){_bytes = n_bytes;};
    void print(FILE *f){fprintf(f, "Ошибка при выделении %zu байт\n",_bytes);};
};

class Exception_Invalid_string : protected Exception
{
    char _c;
public:
    Exception_Invalid_string(char c){_c = c;};
    void print(FILE *f){fprintf(f, "Во входной строке найден некорректный символ %c\n",_c);};
};

class Exception_Matrix_border_out : protected Exception
{
    size_t _n,_m,_k,_l;
public:
    Exception_Matrix_border_out(size_t n, size_t m, size_t k, size_t l){_n = n;_m = m;_k = k;_l = l;};
    void print(FILE *f){fprintf(f, "Выход за границу матрицы(%zu,%zu), запрашивалось (%zu,%zu)\n",_n,_m,_k,_l);};
};


class Exception_Vector_border_out : protected Exception
{
    size_t _n,_k;
public:
    Exception_Vector_border_out(size_t n, size_t k){_n = n;_k = k;};
    void print(FILE *f){fprintf(f, "Выход за границу вектора(%zu), запрашивалось (%zu)\n",_n,_k);};
};


class Exception_matrix_sizes : protected Exception
{
    size_t _n1,_m1,_n2,_m2;
public:
    Exception_matrix_sizes(size_t n1, size_t m1, size_t n2, size_t m2){_n1=n1;_m1=m1;_n2=n2;_m2=m2;};
    void print(FILE *f){fprintf(f, "Размерности матриц должны совпадать((%zu,%zu),(%zu,%zu))\n",_n1,_m1,_n2,_m2);};
};

#endif
