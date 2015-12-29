/*
 * This is a very minimal bignum library. Here I'll declare all the functions and structs
 * that will be needed in the development process. The library should support main arithmetical
 * and logical operations, such as:
 * -> addition
 * -> subtraction
 * -> multiplication
 * -> division
 * -> binary and (&)
 * -> binary or (|)
 * -> binary xor (^)
 * -> power(x, y) -- get a number x to the power of y
 *
 * Another feature I'll try to implement using this library is
 * 'fibonacci sequence' for huge values (using matrix multiplication)
 * and also 'factorial()' function for big values
 */

/* MAX_SIZE is adjustable, but we probably won't need numbers with more than 10'000 digits */
#define MAX_SIZE 100000

typedef struct unum {
    unsigned int val :4;
} unum;


typedef struct Bignum {
    unum self[MAX_SIZE];
    unsigned long start;
    int sign :2;
} Bignum;

/* Here go the main functions */
Bignum *new_num(char *num);
void printnum(Bignum *p);
Bignum *add(Bignum *a, Bignum *b);
Bignum *minus(Bignum *a, Bignum *b);
int bigger_nominal(Bignum *a, Bignum *b);
