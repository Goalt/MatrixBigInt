#include <iostream>

//#include <string>
#include <ctime>
#include "generators.h"
#include "big_int.h"
#include "exception.h"
#include "rational_number.h"
#include "dictionary.h"
#include "matrix.h"
#include "vector.h"
using namespace std;

int main(int argc, const char * argv[])
{
    try
    {
        srand((unsigned int)time(0));//Необходимо для случайных чисел, не удалять
        //FILE *f = fopen("m", "r");
        Matrix a(2,2,true);
        a = a * a * RationalNumber(1,3) * a;
        a.make_elems_canonical();
        a.print(stdout);

        Matrix b(2,1);
        b.set(1,1, 2);
        b.set(2,1,1);
        Matrix c(1,3);
        c.set(1,1,3);
        c.set(1,2,4);
        c.set(1,3,2);
        c = ~(b*c);
        c.print(stdout);
        /*double x,y;
        x=(float)clock();
        BigInt num1("999998888877777666665555544444333332222211111"), num2("135792468"), num3;
        
        num3 = num1 / num2;
        num3.print(stdout, 1);
        
        y=(float)clock();
        x=(float)(y-x)/CLOCKS_PER_SEC;
        cout << x;*/
    }
    catch (Exception &ex)
    {
        ex.print(stdout);
    }
    catch (Exception_Memory &ex)
    {
        ex.print(stdout);
    }
    catch (Exception_Invalid_string &ex)
    {
        ex.print(stdout);
    }
    catch (Exception_Matrix_border_out &ex)
    {
        ex.print(stdout);
    }
    catch (Exception_Vector_border_out &ex)
    {
        ex.print(stdout);
    }
    catch (Exception_matrix_sizes &ex)
    {
        ex.print(stdout);
    }

    return 0;
}
