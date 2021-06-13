/* Exercise 4-10. An alternate organisation uses getline to read an
   entire input line; this makes getch and ungetch unnecessary. Revise
   the calculator to use this approach. */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include "knr2.h"

#define MAXOP   100		/* max size of operand or operator */
#define NUMBER  '0'
#define ERROR   -2;

int getop(char *s);	   /* get next operator or operand. */
int issign(char c);

/* val[]: FIFO stack for floating point values */
#define MAXVAL  100
double val[MAXVAL];		
int sp = 0;			/* next free stack position */
int dontpop = 0;		/* prevents stack pop on newline */
void push(double);
double pop(void);
double peak(void);
void swap(void);
void clear(void);

/* reverse polish calculator */
int main()
{
    int type;
    double last, tmp, save[24];
    char s[MAXOP];

    last = 0;

    while ((type=getop(s))!=EOF) {
	switch (type) {
	case NUMBER:
	    push(atof(s));
	    break;
	case '+':
	    push(pop() + pop());
	    break;
	case '*':
	    push(pop() * pop());
	    break;
	case '-':
	    tmp = pop();
	    push(pop() - tmp);
	    break;
	case '/':
	    tmp = pop();
	    if (tmp != 0)
		push(pop() / tmp);
	    else
		fprintf(stderr, "MATH ERROR: division by 0.\n");
	    break;
	case '%': 		/* modulo */
	    tmp = pop();
	    push(fmod(pop(), tmp));
	    break;
	case '?':		/* most recently printed */
	    push(last);
	    break;
	case 'f':		/* math function */
	    if (!strcmp(s, "sin"))
		push(sin(pop()));
	    else if (!strcmp(s, "cos"))
		push(cos(pop()));
	    else if (!strcmp(s, "tan"))
		push(tan(pop()));
	    else if (!strcmp(s, "asin"))
		push(asin(pop()));
	    else if (!strcmp(s, "acos"))
		push(acos(pop()));
	    else if (!strcmp(s, "atan"))
		push(atan(pop()));
	    else if (!strcmp(s, "exp"))
		push(exp(pop()));
	    else if (!strcmp(s, "ln"))
		push(log(pop()));		
	    else if (!strcmp(s, "log"))
		push(log10(pop()));		
	    else if (!strcmp(s, "pow")) {
		tmp = pop(); 
		push(pow(pop(), tmp));
	    } else if (!strcmp(s, "sqrt"))
		push(sqrt(pop()));
	    else if (!strcmp(s, "abs"))
		push(abs(pop()));
	    else if (!strcmp(s, "ceil"))
		push(ceil(pop()));
	    else if (!strcmp(s, "floor"))
		push(floor(pop()));
	    else
		fprintf(stderr, "SYNTAX ERROR: unknown function %s.\n", s);
	    switch (errno) {
	    case 0:		/* no error */
		break;
	    case EDOM:
		fprintf(stderr, "MATH ERROR: outside function domain (%s).\n", s);  
		break;
	    case ERANGE:
		fprintf(stderr, "MATH ERROR: floating point overflow.\n"); 
		break;
	    default:		/* "unreachable" */
		fprintf(stderr, "UNKNOWN ERROR: (bug).\n");
		break;
	    }
	    break;
	case 's':		/* swap n*/
	    swap();
	    dontpop = 1;
	    break;
	case 'p':		/* next print, dont pop*/
	    dontpop = 1;
	    break;
	case '\n': 
	    last = dontpop ? peak() : pop();
	    printf("\t%.8g\n", last);
	    dontpop = 0;
	    break;
	case '=': 		/* assignment */
	    if (isupper(s[0]))
		save[s[0]-'A'] = peak();
	    break;
	default:
	    if (isupper(type))
		push(save[type-'A']);
	    else
		fprintf(stderr, "SYNTAX ERROR: unknown command '%s'.\n", s);
	    break;
	}
    }

    if (sp > 0)			/* print on EOF */
	printf("\t%.8g\n", pop());

    return 0;
}

/* push(): push f onto value stack */
void push(double f)
{
    if (sp < MAXVAL)
	val[sp++] = f;
    else
	fprintf(stderr, "ERROR: stack full, can't push %g.\n", f);
}

/* pop(): pop and return top value from value stack */
double pop(void)
{
    if (sp > 0)
	return val[--sp];
    else {
	fprintf(stderr, "ERROR: stack empty.\n");
	return 0.0;
    }
}

/* swap(): swap the positions of the two elements at the top of the
   stack */
void swap(void)
{
    double tmp;

    tmp = val[sp-2];

    sp[val-2] = val[sp-1];
    sp[val-1] = tmp;
}

/* clear(): clear all values from the stack */
void clear(void)
{
    sp = 0;
}

/* peak(): returns the top value from the stack without removing it */
double peak(void)
{
    if (sp > 0)
	return val[sp-1];
    else {
	fprintf(stderr, "ERROR: stack empty.\n");
	return 0.0;
    }	
}

/* getop(): returns next operator or numeric operand from stdin.

   When a operand (0-9) is read, NUMBER is returned and the numeric
   string stored in s.
   
   When a operator (i.e. not 0-9) is read, it is returned. Some
   operators are followed by a space delimited arguement, which will
   be stored in s. If the operator doesn't take arguements, s is a
   string containing the operator.

   Leading blanks are ignored. Reading is terminated at reaching EOF
   or by reading a newline.
 */
int getop(const char *l, char *s)
{
    char c, l[MAX+1];
    int i, j, len;

    len = get_line(l, MAX); 
    if (len == 0)
	return EOF;

    /* skip whitespace */
    for (i=0; i<len && isblank(l[i]); ++i)
	;

    if (l[i] == '\n')
	return l[0];

    /* decimal strings can have a sign character */
    if (isdigit(l[i]) || (issign(l[i]) && isdigit(l[i+1]))) {
	return getoperand(l, s);
    } 

    /* some operators can have optional arguement field */
    j = 0;
    c = l[i];
    switch (c) {
    case '=':
	for ( ; i<len && isblank(l[i]); ++i)
	    ;
	s[j++] = l[i];
	break;
    case 'f':
	for ( ; i<len && isblank(l[i]); ++i)
	    ;
	while (islower(l[i]))
	    s[j++] = l[i++];
	break;
    default:
	s[j++] = l[i];
	break;
    }

    s[j] = '\0';

    return c;
}

/* getoperand(): Retrieve the next operand from the string l and store
   it in s. */
int getoperand(const char *l, char *s)
{
    int j;

    j = 0;
    while (isdigit(l[i])) /* integer part */
	s[j++] = l[i++];
    if (l[i]=='.')
	s[j++] = l[i++];
    while (isdigit(l[i])) /* decimal part */
	s[j++] = l[i++];

    s[j] = '\0';

    return j < 1 ? ERROR : NUMBER;
}

/* getoperator(): Retrieve the next operator from the string and store
   arguement */
{





/* issign(): returns non zero if c is '-' or '+' */
int issign(char c)
{
    return c =='+' || c=='-';
}
