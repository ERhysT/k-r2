/* Excercise 7-5. Rewrite the postfix calculator of Chapter 4 to
   use scanf and/or sscanf to do the input and number conversion. */

#include <stdio.h>

#define TOKENMAX     8
#define DOUBLE_SPEC  "%lf"
#define STRING_SPEC  "%8s"

struct token {
    enum { OPERATOR, OPERAND } type;    
    union {
	double operand;
	char operator[20];
    } word;
};
    
/* getword(): returns non zero if t is populated from stdin. */
int getword(struct token *t)
{
    if (scanf(DOUBLE_SPEC, &t->word.operand) == 1) {
	t->type = OPERAND;
	return 1;
    }
    if (scanf(STRING_SPEC, t->word.operator) == 1) {
	t->type = OPERATOR;
	return 1;
    }
    return 0;
}

/* postfix calculator */
int main(void)
{
    struct token t;

    while (getword(&t))
	if (t.type == OPERATOR)
	    printf("operator: %s\n", t.word.operator);
	else if (t.type == OPERAND)
	    printf("operand: %g\n", t.word.operand);
    return 0;
}

    
