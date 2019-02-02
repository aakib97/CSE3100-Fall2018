#include <stdio.h>

int main()
{
    double total = 0, average = 0, count = 0, x; //Initialize variables

    while (scanf("%lf", &x) == 1)  // pay attention to %lf
    {
        count += 1; //count the number of x(s)
        total += x; //add up the values of x(s)
        average = total/count;
        printf("Total=%f Average=%f\n", total, average); // pay attention to %f
    }

    /* Let the OS know everything is just peachy. */
    return 0;
}

