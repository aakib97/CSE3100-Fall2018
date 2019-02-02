#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
 * Polynomial ADT. This data type uses two structures
 * Monomial: is used to represent a term  c * x^p
 *           where c is the coefficient and p is the exponent.
 * Polynomial: is used to capture an entire polynomial as a linked list
 *             of monomials, in decreasing exponent order.
 * Note that the representation is sparse and monomials with a coefficient
 * of zero should not be in the list. For instance, the polynomial
 * 7 x^10 - 4 x^3 - 2 x would be represented by the list of three monomials
 * [(7,10), (-4,3), (-2,1)]
 */

typedef struct Monomial {
    int coeff;     /* coefficient */
    int exp;       /* exponent    */
    struct Monomial *next;  /* next monomial */
} Mono;

typedef struct Polynomial {
    Mono* first;   /* first monomial */
    Mono* last;    /* last monomial  */
} Poly;

/*
 * This function creates a monomial with a given coefficient and exponent.
 * c : the coefficient
 * k : the exponent
 * return value : a pointer to the newly created monomial
 */
Mono* newMono(int c, int k)
{
    // TODO
    Mono* monomial = (Mono*)malloc(sizeof(Mono));

    if (monomial == NULL){ return NULL;}

    monomial->coeff = c;

    monomial->exp = k;

    monomial->next = NULL;

    return monomial;
}

/*
 * This function creates and returns a new polynomial with no monomials.
 */
Poly* newPoly()
{
    // TODO
    Poly* polynomial = (Poly*)malloc(sizeof(Poly));

    if (polynomial == NULL){ return NULL;}

    polynomial->first = polynomial->last = NULL;

    return polynomial;
}

/*
 * This function deallocates a polynomial.
 * p : pointer to the polynomial to deallocate
 * Requirements:
 *  - the function deallocates not only the polynomial but also all the
 *    monomials comprising it
 */
void freePoly(Poly* p)
{
    // TODO

    if (p == NULL) { return;}

    Mono* mono_1 = p->first;

    while (mono_1 != NULL){

        Mono* mono_2 = mono_1->next;
        free(mono_1);
        mono_1 = mono_2;
    }

    free(p);
}

/*
 * This function adds a monomial to an existing polynomial
 * p : pointer to the polynomial to modify
 * m : pointer to the monomial to add
 * Requirements:
 * - the monomial should be added at the end of the monomial list
 */
void appendMono(Poly* p, Mono* m)
{
    // TODO

    if ( p->first == NULL){
        p->first = p->last = m;
    } else{
        p->last = p->last->next = m;
    }

}

/*
 * This function builds the linked list representation of a
 * polynomial read from the standard input. It reads the
 * monomials as (coefficient,exponent) pairs of (whitespace separated)
 * integers as described in the assignment document
 * and constructs the linked list representation.
 * return value : pointer to the constructed polynomial
 * Assumptions:
 * - input monomials are given in decreasing exponent order
 * - the monomial with exponent 0 is always present as the last
 *   monomial of the input (even when the coefficient is 0)
 * Requirements:
 * - only monomials with non-zero coefficients get added to the list
 * - the constructed list has monomials in decreasing exponent order
 */
Poly* readPoly()
{
    // TODO
    Poly* p = newPoly();

    if (p == NULL) { return NULL;}


    int c, k;

    scanf("%d %d", &c, &k);
    appendMono(p, newMono(c, k));

    return p;
}

/*
 * This function computes the derivative of a given polynomial.
 * p  : pointer to the polynomial to be differentiated
 * returned value : pointer to a new polynomial filled with the monomials of the derivative
 * Assumptions:
 * - the given polynomial has only monomials with non-zero coefficients
 * - the given polynomial has the monomials linked in decreasing exponent order
 * Requirements:
 * - the given polynomial remains unchanged
 * - the result polynomial has only monomials with non-zero coefficients
 * - the result polynomial has the monomials linked in decreasing exponent order
 */
Poly* polyDer(Poly* p)
{
    // TODO
    Poly* results = newPoly();
    Mono* mono = (p==NULL ? NULL : p->first);

    while (mono){
        appendMono(results, newMono(mono->exp * mono->coeff, mono->coeff - 1));
        mono = mono->next;
    }
    return results;

}

/*
 * This function computes a new polynomial that represent the difference
 * between two given polynomials.
 * p1  : pointer to first polynomial
 * p2  : pointer to second polynomial
 * returned value : pointer to a new polynomial filled with the monomials of the difference
 * Assumptions:
 * - the given polynomials have only monomials with non-zero coefficients
 * - the given polynomials have the monomials linked in decreasing exponent order
 * Requirements:
 * - the given polynomials remain unchanged
 * - the result polynomial has only monomials with non-zero coefficients
 * - the result polynomial has the monomials linked in decreasing exponent order
 * Notes:
 * - the result polynomial cannot include multiple monomials with the same exponent
 */
Poly* polyDiff(Poly* p1, Poly* p2)
{
    // TODO
    Poly* results = newPoly();
    Mono* mono_1 = (p1==NULL ? NULL : p1->first);
    Mono* mono_2 = (p2==NULL ? NULL : p2->first);

    while( mono_1 && mono_2 ) {
        if (mono_1->exp == mono_2->exp) {
            if (mono_1->coeff - mono_2->coeff != 0)
                appendMono(results, newMono(mono_1->coeff - mono_2->coeff, mono_1->exp));
            mono_1 = mono_1->next;
            mono_2 = mono_2->next;
        } else if (mono_1->exp < mono_2->exp) {
            appendMono(results, newMono(mono_1->coeff, mono_1->exp));
            mono_1 = mono_1->next;
        } else {
            appendMono(results, newMono(mono_2->coeff, mono_2->exp));
            mono_2 = mono_2->next;
        }
    }

        while( mono_1 ) {
            appendMono( results, newMono(mono_1->coeff, mono_1->exp));
            mono_1 = mono_1->next;
        }

        while( mono_2 ) {
            appendMono( results, newMono(mono_2->coeff, mono_2->exp));
            mono_2 = mono_2->next;
        }

        return results;
}
/*
 * This function prints the received polynomial to the standard output.
 * p  : pointer to the polynomial to print
 * DO NOT CHANGE THIS FUNCTION
 */
void printPoly(const char* msg, Poly* p)
{
    printf("%s\t", msg);
    if( !p || !p->first ) {
        printf( "empty polynomial\n" );
        return;
    } else {
        Mono* m = p->first;
        while( m != NULL ) {
            int c = m->coeff;
            int k = m->exp;
            assert( m->coeff != 0 );
            if( m != p->first && c > 0 )  printf( " + " );
            if( c < 0 )  printf( " - " );
            if( c*c != 1 || k == 0 )
                printf( "%d ", (c > 0 ? c : -c) );
            if( k > 0 )  printf( "x" );
            if( k > 1 )  printf( "^%d", k );
            m = m->next;
        }
        /* end with newline character */
        printf( "\n" );
    }
    return;
}

/* Only add necessary clean up statements at the end  */
int main()
{
    printf( "Enter polynomial: " );
    Poly* p = readPoly();

    if( p ) printPoly( "P(x):", p );
    else printf( "Could not read P(x)\n" );

    Poly* deriv = polyDer( p );
    if( deriv ) printPoly( "P'(x):", deriv );
    else printf( "Could not compute P'(x)\n" );

    Poly* diff1 = polyDiff( p, deriv );
    if( diff1 ) printPoly( "P(x) - P'(x):", diff1 );
    else printf( "Could not compute P(x) - P'(x)\n" );

    Poly* diff2 = polyDiff(deriv,p);
    if( diff2 ) printPoly( "P'(x) - P(x):", diff2 );
    else printf( "Could not compute P'(x) - P(x)\n" );

    Poly* diff3 = polyDiff( p, p );
    if( diff3 ) printPoly( "P(x) - P(x):", diff3 );
    else printf("Could not compute P(x) - P(x)\n");

    freePoly(p);
    freePoly(diff1);
    freePoly(diff2);
    freePoly(diff3);

    return 0;
}
