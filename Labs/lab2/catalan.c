#include <stdio.h>
#include <assert.h>

/* This function calculates n-th Catalan number.
 * n is assumed to be non-negative.
 * Do not worry about overflow.
 * Do not print anything in this function.
 * Do not use floating-point numbers (or operations).
 */
static long catalan_number(int n)
{
    assert(n>=0);
    // TODO
    // base step
    // recursion
    long results = 1;
    if (n == 0)
        return results;
    return results =  ( ((4 * n) - 2) * catalan_number(n-1) )/ (n + 1);

}

/* do not change main function */
int main(int argc, char **argv)
{
    int n;
    for (n = 0; n < 34; n ++)
        printf("C(%2d)=%18ld\n", n, catalan_number(n));
    return 0;
}
