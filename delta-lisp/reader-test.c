#include "common.h"

void tok_test(char *in)
{
    printf("input: %s\n", in);
    vector *v = tokenize(strdup(in));
    tok_dump(v);
    putchar('\n');
}

void tok_run()
{
    tok_test("-00123");
    tok_test("-.23"); 
    tok_test("-0.012");
    tok_test("\"!@#$ 45462 ABC\"");
    tok_test("\"abc\"\"def\"-123");
    tok_test("-123");
    tok_test("(lambda (x) (c123))");
    tok_test("(+ -00.1 12.)-0.");
    tok_test("(  * 2 3( / 23 3) )");
    tok_test("(if 1..0 !@#${} __1245)");
    tok_test("(if nil #t #f)");
    tok_test("'(1 2 quote)");
}

int main()
{
    tok_run();
}
