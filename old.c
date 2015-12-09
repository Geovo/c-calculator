#include <stdio.h>

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
    if ((a = prec(p) != -1) && (b = prec(last) != -1))
        if (prec(p) > prec(last))
            return 1;

    // return 0 in all wrong cases
    return 0;
}

void compile(char *origin) {
    char *p = origin;
    for (; *p != '\0'; p++)
        ;
    printf("distance: %li\n", p-origin);
    // after getting the length, we create a new 'stack'
    char res[p-origin+1];

    // parse the tokens
    p = origin;
    int curr[20];
    int num_or_op[20];
    char ops[20];
    int last = 0;
    ops[19] = '\0';
    int cp = 0;
    int ch_num = 0; // checker if a number was actually parsed
    int curr_num = 0;
    for (; *p != '\0'; p++) {
        if (*p >= '0' && *p <= '9') {
            curr_num = curr_num * 10 + *p - '0';
            ch_num = 1;
        } else if (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
            if (ch_num) { // if there was no whitespace between a number and an operand
                ch_num = 0;
                curr[cp++] = curr_num;
                curr_num = 0;
                num_or_op[cp-1] = 1;
            }
            // this part is essential. It checks if the last op in our operator stack has a higher precedence
            // then the current op
            char c = *p;
            if (higher(c, ops[last-1])) {
                ops[last++] = c;
            } else {
                while (!higher(c, ops[last-1] && last > 0)) {
                    curr[cp++] = ops[last--];
                    num_or_op[cp-1] = 0;
                }
                ops[last++] = *p;
            }
            //curr[cp] = *p;
            //num_or_op[cp++] = 0; // so we now that at cp there's an op
        } else if (ch_num) {
            curr[cp] = curr_num;
            ch_num = 0;
            num_or_op[cp++] = 1; // this indicates that there's a number at cp
            curr_num = 0; // set curr_num to 0 again
        }
    }
    // set the last number to the last element of the stack
    curr[cp] = curr_num;
    num_or_op[cp] = 1;
    for (int i = 0; i <= cp; i++)
        if (num_or_op[i] == 1)
            printf("curr[%i]: %i\n", i, curr[i]);
        else
            printf("curr[%i]: %c\n", i, curr[i]);
}

int main() {
    compile("19 + 5 * 8");
    return 0;
}
#include <stdio.h>

// let's try to parse an math expression with a stack

/*int operate(int a, int b, char op) {
    int res;
    switch (op) {
        case '+':
            res = a + b;
            break;
        case '-':
            res = a - b;
            break;
    }
    return res;
}*/

void parse_it(char *exp) {
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

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/')
            ops[opp++] = *p;
    }
    nums[np] = curr;
    ops[opp] = '\0';
    // done with parsing numbers and ops
    // DEBUG PRINT:
    for (int i = 0; i < 5; i++)
        printf("nums[%i]: %i || ops[%i]: %c\n", i, nums[i], i, ops[i]);

    // now move to finding the highest precedence (multiply and division)
    char opbuff[30];
    int numbuff[30];
    int opbuffp = 1; // opbuff pointer
    int nbuffp = 0; // numbuff pointer
    opp--;
    // first, let all the multiply and division happen
    while (opp > 0) {
        printf("at opp: %i | %c\n", opp, ops[opp]);
        if (ops[opp] == '*' || ops[opp] == '/') {
            int a = nums[np--];
            int b = nums[np];
            if (ops[opp] == '*')
                b *= a;
            else if (ops[opp] == '/')
                b /= a;
            printf("result is b: %i\n", b);
            nums[np] = b;
            opp--;
        } else {
            opbuff[opbuffp++] = ops[opp];
            numbuff[nbuffp++] = nums[np];
            ops[opp--] = ' ';
            //if (np > 0)
            nums[np--] = 0;
        }
//		printf("now at np: %i and opp: %i\n", np, opp);
//		opp--;
//		np--;
    }
//	printf("LAST: %i\n", nums[np]);

    int last = nums[np]; // have to do this, because the arrays don't begin from the same point

    for (int i = 0; i < 5; i++)
        printf("numbuff[%i]: %i || opbuff[%i]: %c\n", i, numbuff[i], i, opbuff[i]);

    while (opbuffp > 0) {
        ops[opp++] = opbuff[opbuffp];
        nums[np++] = numbuff[nbuffp];
        opbuffp--;
        nbuffp--;
    }

    nums[0] = last;

    // ==============================================================
    // now we changed our stack to only + and - operations
    // Next step is to first subtract and then add
    while (nums[np] == 0) {
        np--;
        opp--;
    }

    for (int i = 0; i <= np; i++)
        printf("nums[%i]: %i || ops[%i]: %c\n", i, nums[i], i, ops[i]);


    while (opp > 0) {
        printf("at opp: %i | %c\n", opp, ops[opp]);
        if (ops[opp] == '-') {
            int a = nums[np--];
            int b = nums[np] - a;
            printf("result is b: %i\n", b);
            nums[np] = b;
            opp--;
        } else {
            opbuff[opbuffp++] = ops[opp];
            numbuff[nbuffp++] = nums[np];
            ops[opp--] = ' ';
            //if (np > 0)
            nums[np--] = 0;
        }
//		printf("now at np: %i and opp: %i\n", np, opp);
//		opp--;
    }

    while (opbuffp > 0) {
        ops[opp++] = opbuff[opbuffp];
        nums[np++] = numbuff[nbuffp];
        opbuffp--;
        nbuffp--;
    }


    // now let's just add them up
    while (opp > 0) {
        int a = nums[np--];
        int b = nums[np];
        char op = ops[opp--];
        int oldb = b;
        if (op == '+')
            b += a;
        else if (np == '-')
            b -= a;
        nums[np] = b;
        printf("%i %c %i = %i\n", oldb, op, a, b);
        printf("last on stack: %i\n", a);
    }

    printf("yeah, result is here: %i\n", nums[np]);
}
int main() {
    parse_it("1 + 4 - 2 * 6 + 7");
    return 0;
}
