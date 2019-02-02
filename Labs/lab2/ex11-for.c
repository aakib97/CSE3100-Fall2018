#include    <stdio.h>


int main()
{
    int i, odd, even;

    // count the number of odd numbers and even numbers from 0 to 199
    for (i = odd = even = 0; i < 200; i ++)
        if (i % 2 == 1)
            odd++;
        else
            even++;
    printf("odd=%d even=%d \n", odd, even);
    return 0;
}