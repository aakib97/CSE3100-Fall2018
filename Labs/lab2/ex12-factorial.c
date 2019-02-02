/* This program should read a single integer from standard input then
 * compute its factorial. For example, if the user enters 5, the program should
 * output 120 because 1*2*3*4*5=120. */

#include <stdio.h>
#include <assert.h>

int factorial(int n)
{
    assert(n>=0);
    int result = 1;
    if(n == 0)
        return result;
    return result = factorial(n-1) * n;
}

int main()
{
    int n, result;

    assert( scanf("%d", &n) == 1 );
    result = factorial(n);
    printf("%d\n", result);
    return 0;
}
