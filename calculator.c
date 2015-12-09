#include <stdio.h>

int compute(int a, int b, char op) {
//    printf("got a: %i | b: %i | op: %c\n", a, b, op);
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
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
            return 3;
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
    //        printf("%c is higher than %c\n", p, last);
            return 1;
        }


    // return 0 in all wrong cases
    return 0;
}

int parse_it(char *exp) {
    char *p = exp;
    int nums[30];
    char ops[30];
    ops[0] = ' ';
    int opp; // ops pointer
    int np; // nums pointer
    opp = 1; // set them to zero
    np = 0;
    int curr = 0; // used to parse nums
    int curr_ch = 0; // to check if curr was changed between whitespaces and stuff
    for (; *p != '\0'; p++) {
        if (*p >= '0' && *p <= '9') {
            curr = curr * 10 + *p - '0';
            curr_ch = 1;
            continue;
        }
        // if that's a parsed 0 then add it to the stack
        if (curr_ch) {
            nums[np++] = curr;
            curr = 0;
            curr_ch = 0; // set it back after adding to the stack
        }

        // in this part we will do the comparison
        if (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
            if (opp == 1 || higher(*p, ops[opp-1]))
            //    if (higher(*p, ops[opp-1]))
                // if the opstack is empty or the current operator has a higher precedence, add it to the opstack
                    ops[opp++] = *p;

            else {
             /* Although if the current operator has lower precedence than the top of the stack,
              * pop off all the ones with higher precedence and calculate the product
              */
              //higher(*p, ops[opp-1]);
              int t = np - 1;
              int test = compute(nums[np-2], nums[np-1], ops[--opp]);
        //      printf("it's a test: %i | np is: %i\n", test, np);
              nums[np-2] = test;
        //      printf("nums[np-1]: %i\n", nums[np-1]);
              np--;
              ops[opp++] = *p;
              //np = t+1;
            }
        }
    }
    nums[np++] = curr;
    ops[opp] = '\0';
    // done with parsing numbers and ops
    // DEBUG PRINT:
    for (int i = 0; i <= np; i++)
//        printf("nums[%i]: %i || ops[%i]: %c\n", i, nums[i], i, ops[i]);

    // now the expression is parsed
    // let's calculate the rest
    while (opp > 0) {
        int t = compute(nums[np-2], nums[np-1], ops[--opp]);
        nums[np-2] = t;
        np--;
    }
//    printf("result: %i\n", nums[0]);
    return nums[0];

}

void test(char msg[]) {
    printf("%s = %i\n", msg, parse_it(msg));
}

int main() {
    //parse_it("1 + 5 * 10 - 9 / 3");
    test("1 + 5 * 10 - 9 / 3");
    test("1 + 5 * 10 - 9 / 3");
    test("1 + 5 * 10 - 9");
    test("1+5*10-9");
    return 0;
}
