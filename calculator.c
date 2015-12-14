#include <stdio.h>
#include <stdlib.h>
#include "useful.h"

long double compute(long double a, long double b, char op) {
//    printf("got a: %i | b: %i | op: %c\n", a, b, op);
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            /* Check if the second operand is 0 and operator is '/' */
            if (b == 0)
                die("Sorry, but you can't divide by zero.");
            return a / b;
        case '%':
            return (long double) ((long long)a % (long long)b);
        case '^':
            return power(a, b);
        default:
            return -1;
    }
}

// get the precedence of an operator
int prec(char p) {
    switch (p) {
        case '+':
        case '-':
            return 2;
        case '*':
        case '/':
        case '%':
            return 3;
        case '^':
            return 4;
        case '(':
            /* Here is the trick: we need to add all operators
             * to the stack after the bracket, that's why it has a 'lower precedence' here
             */
            return 1;
        default:
            return -1;
    }
}

int higher(char p, char last) {
    if (p == last)
        return 0;

    // check for errors
    int a, b;
    //char c = p;
    //printf("");
    //printf("%c is higher than %c\n", p, last);
    if ((a = prec(p) != -1) && (b = prec(last) != -1))
        if (prec(p) > prec(last)) {
//            printf("%c is higher than %c\n", p, last);
            return 1;
        }


    // return 0 in all wrong cases
    return 0;
}

long double parse_it(char *exp) {
    char *p = exp;
    long double nums[MAX_EXPR];
    char ops[MAX_EXPR];
    ops[0] = ' ';
    int opp; /* ops pointer */
    int np; /* nums pointer */
    opp = 1; /* set them to zero */
    np = 0;
    int sign = 1; // sign for the current number
    long long tail = 0;
    long double curr = 0; /* used to parse nums */
    long double commas = 1; /* used to set comma back in floats */
    int in_float = 0; /* check if inside a float */
    int curr_ch = 0; /* to check if curr was changed between whitespaces and stuff */
    for (; *p != '\0'; p++) {

        /* DEBUG: */
//        for (int i = 0; i < np; i++)
             //printf("nums[%i]: %li || ops[%i]: %c\n", np-1, nums[np-1], opp-1, ops[opp-1]);

        if (*p >= '0' && *p <= '9') {
            /* If we are parsing the after-comma part of a floating-point, we should also check
             * if the after-comma part is a non-zero value
             */
            if (in_float) {
                commas *= 10;
                tail = tail * 10 + (*p - '0');
            }
            curr = curr * 10 + *p - '0';
            curr_ch = 1;
            continue;
        }

        // let's add floating point functionality
        if ((*p == '.') && curr_ch) {
            //commas *= 10; /* multiply commas by 10 for every comma shift */
            in_float = 1;
            continue;
        }
        // if that's a parsed 0 then add it to the stack
        if (curr_ch) {
            if (tail > 0) {
                nums[np++] = (long double) ((curr * sign) / commas);
            } else
                nums[np++] = (long double) (curr * sign);
            curr = 0;
            commas = 1;
            tail = 0;
            in_float = 0;
            curr_ch = 0; // set it back after adding to the stack
            sign = 1; /* Set the sign back to positive */
        }

        /* To avoid interference of the unary minus and the operator minus, we will add code
         * that evaluates the unary minus (if there is one).
         * It's important to remember, that we don't care about how much whitespace there is between the unary minus
         * and the number itself. So we have to check if our ops stack has the same height as the nums stack
         */
         if (*p == '-' && (opp > np) && (ops[opp-1] != '(')) {
            sign = -1;
            continue;
        }

        /*
         * The point of the bracket is that the parser works the same way as always,
         * but when there's a closing bracket, we have to execute the whole
         * bracket and putting it back to the stack
         */
         if (*p == '(') {
             ops[opp++] = *p;
            // np++; /* increment the np pointer to make place for the bracket operands */
             continue;
         }

         if (*p == ')') {
             while (ops[opp-1] != '(') {
                 long double test = compute(nums[np-2], nums[np-1], ops[--opp]);
                 nums[np-2] = test;
                 nums[--np] = 0;
             }
             ops[--opp] = ' ';
            continue;
         }

        /* Next goes the case if we hit a token that is an operator */
        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '%' || *p == '^') {
            /* if the opstack is empty or the current operator has a higher precedence, add it to the opstack */
            if (opp == 1 || higher(*p, ops[opp-1]))
                ops[opp++] = *p;
            else {
             /* Although if the current operator has lower precedence than the top of the stack,
              * pop off all the ones with higher precedence and calculate the product
              */

                /* Here we compute the result and put it back on the number stack */
                //while(higher(ops[opp-1], *p) && opp > 1) {
                //if (!higher(ops[opp-1], *p))
                //    printf("not higher: %c >= %c\n", ops[opp-1], *p);
                  long double test = compute(nums[np-2], nums[np-1], ops[--opp]);
                  nums[np-2] = test;
                  nums[--np] = 0;
                //}
                  ops[opp++] = *p;
            }
        }
    }
    /* the last parsed curr has to be added to the number stack */
    if (curr_ch) {
        if (tail > 0) {
            nums[np++] = (long double) (curr * sign / commas);
        } else {
            nums[np++] = (long double) (curr * sign);
        }
    }

    ops[opp] = '\0';
    // done with parsing numbers and ops

    //    for (int i = 0; i < np; i++)
    //        printf("nums[%i]: %li || ops[%i]: %c\n", i, nums[i], i, ops[i]);

    /*
     * now the expression is parsed
     * let's calculate the rest
     */
     //printf("compute opp: %i | np: %i\n", opp, np);
    while (opp > 0) {
        long double t = compute(nums[np-2], nums[np-1], ops[--opp]);
        //printf("compute nums[%i-2]: %li %c nums[%i-1]%li = %li\n", np, nums[np-2], ops[opp], np, nums[np-1], t);
        nums[np-2] = t;
        np--;
    //    for (int i = 0; i <= np; i++)
    //        printf("nums[%i]: %i || ops[%i]: %c\n", i, nums[i], i, ops[i]);
    }
    //printf("result: %Lf\n", nums[0]);
    return nums[0];

}

long double power(long double what, long double where) {
    /* use a faster method later on */
    long ret = what;
    for (; where > 1; where--)
        ret *= what;
    return ret;
}

void die (char *msg) {
    printf("%s\n", msg);
    exit(1);
}

void test(char msg[], long double right) {
    long double d = right;
    long double r = parse_it(msg);
    //unsigned long = right - r;
    int res = ((int)(right - r)) == 0 ? 1 : 0;
    if (r - (long long)r == 0)
        printf("%s | Predicted: %lli | %.100s = %lli\n", res ? "TRUE" : "FALSE", (long long)right, msg, (long long)r);
    else
        printf("%s | Predicted: %Lf | %.100s = %Lf\n", res ? "TRUE" : "FALSE", right, msg, r);
}

int main() {
    puts("================================================================================");
    puts("Simple random tests:");
    puts("================================================================================");
    test("1 + 5 * 10 * -9", -449);
    test("1 + 5 * 10 * -9   / 15 * 8 / 10 - - 1 + 5", -17);
    test("1 + 5 * 10 - 9 / 3", 48);
    test("1 + 5 + 10 + 13 % 2^ 3", 21);
    test("1+5*10-9", 42);
    test("3+4*5/6-7", -1);
    test("5 * (3 + 2)", 25);
    test("5 * (3 + 2) * 5", 125);
    test("5 * (3 + 2) + (4 + (3 - (8 - 8)))", 32);
    test("5 / 2", 2.5);
    test("2.5 + 2.6", 5.1);
        return 0;
}
