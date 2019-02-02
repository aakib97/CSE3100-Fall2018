#include <stdio.h>
#include <assert.h>

// add any functions you need here
unsigned int sequence(unsigned int term)
{
    unsigned int nxt_Term;
    if (term % 2 == 0) //check if the number is even
    {
        nxt_Term = term/2;
    }
    else
    {
        nxt_Term = (3*term)+1;
    }
    return nxt_Term; //return the next term in the series
}

unsigned int counter(unsigned int n)
{
    unsigned int count = 0;
    while (n != 1)
    {
        //change current term to the next term
        n = sequence(n);
        //increment the count
        count++;
    }
    return count;
}

int main(void)
{
    unsigned int a, b, x, max_steps;
    // you can use additional variables if needed

    printf( "Enter a and b: ");
    assert( scanf("%d %d", &a, &b)==2 && a<=b);

    // add code to find x between a and b that needs
    // largest number of Collatz steps to reach 1
    while(a<=b)
    {
        unsigned int step_check = counter(a);
        if(step_check > max_steps)
        {
            max_steps = step_check;
            x = a;
        }
        a++;
    }

    printf("Longest Collatz chain starts at %d and takes %d steps\n", x, max_steps);
    return 0;
}