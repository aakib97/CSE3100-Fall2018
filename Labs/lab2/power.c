#include <stdio.h>
#include <assert.h>

int power(int base, int n)
{
   assert(n>=0);
   if (n==0)
      return 1;
   else
      return base * power(base,n-1);
}

int main()
{
    double x;
    int n;

    while( 1 )
    {
        printf( "Enter x and n (negative n to exit): " );
        assert( scanf("%lf %d", &x, &n) == 2);

        if( n < 0 ) return 0;

        double pow = power(x,n);
        printf( "%lf^%d = %lf\n", x, n, pow );
    }
}
