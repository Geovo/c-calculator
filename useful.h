/* This is a header file with declarations of some useful functions */

#define MAX_EXPR 30

void die(char *msg);
long double power (long double what, long double where);
int prec(char p);
long double compute(long double a, long double b, char op);
long double parse_it(char *exp);
void test(char *msg, long double right);
