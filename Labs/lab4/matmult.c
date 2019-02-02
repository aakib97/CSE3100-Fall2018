#include <stdio.h>
#include <stdlib.h>

typedef double element_t;

/*
 * Perform multiplication of square matrices
 * Parameters:
 * - n : matrix dimension
 * - a and b : nxn matrices to be multiplied
 * - c : nxn result matrix
 */
void  multiply(int n, element_t** a, element_t** b, element_t** c)
{
    // TODO: add code to compute in c the product of nxn matrices a and b

    int i, j, k;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n ; j++)
        {
            c[i][j] = 0;

            for (k = 0; k < n; k++)
            {
                c[i][j] += (a[i][k] * b[k][j]);
            }
        }
    }

    return;
}

/* do not modify this function  */
element_t trace(int n, element_t** mat)
{
    element_t trace = 0;
    for(int i=0; i<n; i++) trace += mat[i][i];
    return trace;
}

/* do not modify main function  */
int main(int argc, char **argv)
{
    int  n, iterations;

    if(argc < 3) {
        fprintf(stderr, "usage: matmult n iterations\n");
        return 1;
    }

    n = atoi(argv[1]);
    iterations = atoi(argv[2]);
    if(n <= 0 || iterations <= 0) {
        fprintf(stderr, "usage: matmult n iterations, n > 0, iterations > 0\n");
        return 2;
    }

    element_t* astor = malloc(n * n * sizeof(element_t));
    element_t* bstor = malloc(n * n * sizeof(element_t));
    element_t* cstor = malloc(n * n * sizeof(element_t));

    element_t** a = malloc(n * sizeof(element_t*));
    element_t** b = malloc(n * sizeof(element_t*));
    element_t** c = malloc(n * sizeof(element_t*));

    if(!astor || !bstor || !cstor || !a || !b || !c) {
        fprintf(stderr, "out of memory\n");
        return 3;
    }

    for(int i=0; i<n; i++) {
        a[i] = astor+i*n;
        b[i] = bstor+i*n;
        c[i] = cstor+i*n;
    }

    srand(1);    /* always generate the same sequence of random numbers */
    for(int i=0; i<n*n; i++)
        astor[i] = rand() % 2;
    for(int i=0; i<n*n; i++)
        bstor[i] = rand() % 2;

    for(int j=0; j<iterations; j++) {
        multiply(n, a, b, c);
        if(j==0) printf( "trace of AxB: %lf\n", trace(n,c));
        multiply(n, b, a, c);
        if(j==0) printf( "trace of BxA: %lf\n", trace(n,c));
    }

    free(astor);
    free(bstor);
    free(cstor);
    free(a);
    free(b);
    free(c);
    return 0;
}


