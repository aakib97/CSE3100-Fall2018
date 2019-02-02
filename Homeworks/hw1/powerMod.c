#include <stdio.h>

int main()
{
    int result = 1, n, e, m; //Initialize variables
    printf("Please enter n, e, and m: "); //Print statement to ask user for inputs
    scanf("%d %d %d", &n, &e, &m); //Add the stdinput to variables
    int num = n, exp = e, mod = m; //Initialize second set of variable for displaying the result
    while (e >0)
    {
        if (e % 2 == 1)
        {
            result = (result * n) % m; //Calculate the power and remainder
        }
        e = e / 2;
        n = (n * n) % m;
    }
    printf("%d", num);
    printf(" ** %d", exp);
    printf(" mod %d", mod);
    printf(" == %d \n", result);

    /* Let the OS know everything is just peachy. */
    return 0;
}
