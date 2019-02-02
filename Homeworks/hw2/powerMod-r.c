#include <stdio.h>
#include <assert.h>

// complete the function to compute modular exponentiation recursively.
// you cannot use loop in the function.
long powerMod(int n, int e, int m)
{
    // return 1 for now. Replace it with your implementation.
    if (e == 0)
        return 1;
    if (e % 2 == 0)
        return powerMod( ((n * n) % m), (e / 2) , m);
    if (e % 2 == 1)
        return ( powerMod( ((n * n) % m), (e / 2) , m) * n % m)  % m;
    return 0;
}

// do not modify the main program
int main()
{
    long n, e, m;

    printf("Please enter n, e, and m:"); 
    assert(scanf("%ld %ld %ld",&n, &e, &m) == 3);

    printf("%ld ** %ld mod %ld = %ld\n", n, e, m, powerMod(n, e, m));
    return 0;
}
