/* Excercise 5-10. Write the program expr, which evaluates a reverse
   Polish expression from the command line, where each operator or
   operand is a separate argument. For example,

   expr 2 3 4 + *

   evaluates 2(3+4)  */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define issign(c) ( c=='+' || c=='-' )		/* truthy if + or -  */

double str_to_flt(const char *s);
char *strn_cpy(char *s, const char *t, size_t n);
void pushval(double n);
double popval(void);

enum optype { OPERAND, OPERATOR }; 
union op { double operand; char operator[8]; }; 

enum optype getop(char *arg, union op *out)
{
    if (isdigit(arg[0]) || (issign(arg[0]) && isdigit(arg[1]))) {
	out->operand = str_to_flt(arg);
	return OPERAND;
    } 

    strn_cpy(out->operator, arg, sizeof *out);

    return OPERATOR;
}

int main(int argc, char *argv[])
{
    union op type;

    if (argc < 2)
	puts("error: no arguments."); 

    while (--argc) {
	switch (getop(*++argv, &type)) {
	case OPERATOR:
	    switch (*type.operator) {
	    case '+':
		pushval(popval() + popval());		
		break;
	    case '*':
		pushval(popval() * popval());
		break;
	    default:
		printf("error: unknown operator %c.", *type.operator); 
	    }
	    break;
	case OPERAND:
	    pushval(type.operand);
	    break;
	}
    }

    printf("\t%f\n", popval());

    return 0;
}

/* valstack: a stack containing at most VALMAX double precision
   floating point values. */
#define VALMAX 20
double valstack[VALMAX];
double *valpos = valstack;

void pushval(double n)
{
    if (valpos == valstack+VALMAX)
	puts("error: value stack full.");

    *valpos++ = n;
}

double popval(void)
{
    if (valpos > valstack)
	return *--valpos;
    else {
	puts("error: value stack empty.");
	return 0;
    }
}
