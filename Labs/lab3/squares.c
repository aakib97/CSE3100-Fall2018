#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/* This program should print the sum of the elements
 *     1^2, 2^2, 3^2, ..., n^2
 * where n is an integer provided by the user on the
 * command line. Hunt down the bugs and squash them!
 * Seek out the memory leaks and plug them up! */

/* Computes the sum of the first n elements in the array. */
int sum(int n, int* arr)
{
    int i, sum;
    for(i = 0; i <= n; i++)
        sum += *(arr+i);
    return sum;
}

/* Fills the given array with the values
 * 1^2, 2^2, 3^2, ..., (n-1)^2, n^2. */
void fillSquares(int n, int* arr)
{
    int i;
    for(i = 1; i <= n; i++)
        arr[i] = i*i;
}


/* Reads an integer n from arguments,
 * fills array with squares, and computes
 * the sum of the squares. Prints out the
 * sum before freeing all used memory. */
int main(int argc, char* argv[])
{
    int n, total;
    int* arr;

    if(argc < 2) {
        printf("usage: ./squares n\n");
        return 1;
    }

    // convert the the first argument from a string to a number
    n = atoi(argv[1]);
    assert(n > 0);
    arr = (int*) malloc(n);

    fillSquares(n, arr);
    total = sum(n, arr);
    printf("total: %d\n", total);

    free(arr);

    return 0;
}

