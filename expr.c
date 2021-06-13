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

void pushval(double n);
double popval(void);

enum OPTYPE { OPERAND, OPERATOR }; 

enum OPTYPE get_optype(char *arg)
{
    return isdigit(arg[0]) || (issign(arg[0]) && isdigit(arg[1]))
	? OPERAND : OPERATOR;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
	puts("error: no arguments."); 

    while (--argc) {
	switch (get_optype(*++argv)) {
	case OPERAND:
	    pushval(str_to_flt(*argv));
	    break;
	case OPERATOR:
	    switch (**argv) {
	    case '+':
		pushval(popval() + popval());
		break;
	    case '*':
		pushval(popval() * popval());
		break;
	    default:
		printf("error: unknown operator %c", **argv);
		break;
	    }
	    break;
	}
    }

    printf("\t%g\n", popval());
    return 0;
}

/* valstack: a stack containing at most VALMAX double precision
   floating point values. */
#define VALMAX 20
double valstack[VALMAX];
double *valpos = valstack;

void pushval(double n)
{
    if (valpos >= valstack+VALMAX)
	puts("error: value stack full.");
    else
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
