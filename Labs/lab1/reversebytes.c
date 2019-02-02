#include <stdio.h>

/*
 * Print the bits in n in four formats:
 *
 * hexadecimal
 * decimal with bits interpreted as signed int.
 * decimal with bits interpreted as unsigned int.
 * binary
 */
void print_bits(int n)
{
    // print the integer in different format
    printf("hex:%08X signed:%d unsigned:%u bits:", n, n, n);

    // a loop to print bits from most significant to least significant
    // also an example of sizeof use
    for (int i = sizeof(int) * 8 - 1; i >= 0; i --)
       printf("%d", (n >> i) & 1);
    printf("\n");
}

int main()
{
    int n;

    // Repeatedly read an integer 'n' from standard input and
    // reverse its bytes into 'n1' if successful.
    // To exit, press Ctrl-D or Ctr-C
    while (scanf("%d", &n) == 1)
    {
        int n1 = 0;

        // Your code to reverse the bytes in n and save the reversed bytes in n1.
        // TODO
        int newbyte0 = (n >> 24);
        newbyte0 &= 0xff;
        int newbyte1 = (n >> 8);
        newbyte1 &= 0xff00;
        int newbyte2 = (n << 8);
        newbyte2 &= 0xff0000;
        int newbyte3 = n << 24;
        n1 = newbyte0 | newbyte1 | newbyte2 | newbyte3;
        // call function to print the bits in n and n1
        print_bits(n);
        print_bits(n1);
    };

    /* Let the OS know everything is just peachy. */
    return 0;
}
