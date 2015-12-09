#include <stdio.h>
#include <stdlib.h>
#include "useful.h"

long compute(long a, long b, char op) {
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
            return a % b;
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

long parse_it(char *exp) {
    char *p = exp;
    long nums[30];
    char ops[30];
    ops[0] = ' ';
    int opp; /* ops pointer */
    int np; /* nums pointer */
    opp = 1; /* set them to zero */
    np = 0;
    int sign = 1; // sign for the current number
    int curr = 0; /* used to parse nums */
    int curr_ch = 0; /* to check if curr was changed between whitespaces and stuff */
    for (; *p != '\0'; p++) {
        if (*p >= '0' && *p <= '9') {
            curr = curr * 10 + *p - '0';
            curr_ch = 1;
            continue;
        }
        // if that's a parsed 0 then add it to the stack
        if (curr_ch) {
            nums[np++] = curr * sign;
            curr = 0;
            curr_ch = 0; // set it back after adding to the stack
            sign = 1; /* Set the sign back to positive */
        }

        /* To avoid interference of the unary minus and the operator minus, we will add code
         * that evaluates the unary minus (if there is one).
         * It's important to remember, that we don't care about how much whitespace there is between the unary minus
         * and the number itself. So we have to check if our ops stack has the same height as the nums stack
         */
         if (*p == '-' && (opp > np)) {
            sign = -1;
        //    printf("opp and np: %i | %i\n", opp, np);
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
                  long test = compute(nums[np-2], nums[np-1], ops[--opp]);
                  //printf("test: %li\n", test);
                  nums[np-2] = test;
                  nums[--np] = 0;
                //}
                  ops[opp++] = *p;
            }
        }
    }
    /* the last parsed curr has to be added to the number stack */
    nums[np++] = curr * sign;
    ops[opp] = '\0';
    // done with parsing numbers and ops
    // DEBUG PRINT:


    /*
     * now the expression is parsed
     * let's calculate the rest
     */
    while (opp > 0) {
        long t = compute(nums[np-2], nums[np-1], ops[--opp]);
        nums[np-2] = t;
        np--;
    //    for (int i = 0; i <= np; i++)
    //        printf("nums[%i]: %i || ops[%i]: %c\n", i, nums[i], i, ops[i]);
    }
//    printf("result: %i\n", nums[0]);
    return nums[0];

}

long power(long what, long where) {
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

void test(char msg[], long right) {
    long r = parse_it(msg);
    printf("%s | %.100s = %li\n", r == right ? "TRUE" : "FALSE", msg, r);
}

int main() {
    //parse_it("1 + 5 * 10 - 9 / 3");
    test("1 + 5 * 10 * -9", -449);
    test("1 + 5 * 10 * -9   / 15 * 8 / 10 - - 1 + 5", -17);
    test("1 + 5 * 10 - 9 / 3", 48);
    test("1 + 5 + 10 + 13 % 2^ 3", 21);
    test("1+5*10-9", 42);
    test("3+4*5/6-7", -1);
    return 0;
}
