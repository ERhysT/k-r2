/* Excercise 7-5. Rewrite the postfix calculator of Chapter 4 to
   use scanf and/or sscanf to do the input and number conversion. */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define issign(c)    ( c=='+' || c=='-' ) /* non zero if + or -  */

/* token: parsed input */
#define TOKENMAX     8
#define DOUBLE_FMT  "%lf\n"
#define STRING_FMT  "%7s"	/* one less than tokenmax */
struct token {
    enum { OPERATOR, OPERAND } type;    
    union {
	double operand;
	char operator[TOKENMAX];
    } word;
};
int gettoken(struct token *t);
    
/* vstack: FIFO value stack */
#define VSTACKMAX 10
double vstack[VSTACKMAX];
double *vspos = vstack;
void vpush(double v);
double vpop(void);

/* gettoken(): returns non zero if t is populated from stdin. */
int gettoken(struct token *t)
{
    char s[TOKENMAX];

    if (scanf(STRING_FMT, s) != 1)
	return 0;

    if (isdigit(s[0]) || (issign(s[0]) && (strlen(s) > 1) && isdigit(s[1]))) {
	if (sscanf(s, DOUBLE_FMT, &t->word.operand) == 1) {
	    t->type = OPERAND;
	    return 1;
	}
    } else {
	if (sscanf(s, STRING_FMT, t->word.operator) == 1) {
	    t->type = OPERATOR;
	    return 1;
	}
    }

    return 0;
}

/* vpush(): push a value to vstack. */
void vpush(double v)
{
    if (vspos - vstack < VSTACKMAX)
	*vspos++ =  v;
    else
	fprintf(stderr, "error: value stack full.\n");
}

/* vpop(): get a value from vstack */
double vpop(void)
{
    if (vspos - vstack == 0) {
	fprintf(stderr, "error: value stack empty.\n");
	return 0;
    }
    return *--vspos;
}

/* postfix calculator */
int main(void)
{
    struct token t;
    double tmp;

    while (gettoken(&t))
	if (t.type == OPERATOR) {
	    switch (*t.word.operator) {
	    case '+':
		vpush(vpop() + vpop());
		break;
	    case '-':
		tmp = vpop(), vpush(vpop() - tmp);
		break;
	    case '*':
		vpush(vpop() * vpop());
		break;
	    case '/':
		tmp = vpop(), vpush(vpop() / tmp); 
		break;
	    case 'q':
		return 0;
	    default:
		fprintf(stderr, "error: unrecognised operator."); 
		break;
	    }
	    printf("\t= %g\n", vpop()); 
	} else if (t.type == OPERAND) {
	    vpush(t.word.operand);
	}
    return 0;
}

    
