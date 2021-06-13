/* Exercise 4-4. Add accesss to math.h

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>
#include <string.h>

#define MAXOP   100		/* max size of operand or operator */
#define NUMBER  '0'

int getop(char []);	   /* get next operator or operand. */

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

/* bufp[]: FIFO stack for buffering charcters. stdin is used to pop
   only when buffer is empty */
#define BUFSIZE   100
char buf[BUFSIZE];
int bufp = 0;			/* next free position in buf */
int getch(void);
void ungetch(int c);

/* reverse polish calculator */
int main()
{
    int type;
    double tmp;
    char s[MAXOP];

    while ((type=getop(s)) != EOF) {
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
	    default:
		fprintf(stderr, "UNKNOWN ERROR: (bug).\n");
		break;
	    }
	    break;
	case 's':		/* swap n*/
	    swap();
	    dontpop = 1;
	    break;
	case 'p':		/* print */
	    dontpop = 1;
	    break;
	case '\n':		/* runs at the end of every command */
	    printf("\t%.8g\n", dontpop ? peak() : pop());
	    dontpop = 0;
	    break;
	default:
	    fprintf(stderr, "SYNTAX ERROR: unknown command '%s'.\n", s);
	    break;
	}
    }
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

/* getop(): get next operator or numeric operand.

   When an optionally signed decimal string is read, NUMBER is
   returned and the string stored in s.
   
   When a non digit (i.e. not 0-9) is read, it is returned and s will
   be a string of length 1 also containing the character, or in the
   case of the 'f' operator s is populated by the following space
   delimited math function name.

   Leading blanks are ignored. Reading is terminated by EOF or
   newline.
 */
int getop(char *s)
{
    int i, c, h;

    while (isblank(c=getch())) /* skip whitespace */
	;

    i = 0;
    s[i++] = c;
    s[i] = '\0';
    if (c==EOF || c=='\n')
    	return c;		/* end of input */

    /* Numbers can have a leading sign (+ or -) character c that can
       be confused with an operator. Need to check next character h.*/
    h = getch();
    if (isdigit(c) || isdigit(h)) 
	c = NUMBER;
    ungetch(h);

    if (c!=NUMBER && c!='f')	/* is an operator */
	return c;
    else if (c!=NUMBER && c=='f') { /* is a math function */
	while (isblank(h=getch()))
	    ;
	i = 0;			/* overwrite s with function */
	s[i++] = h;
	while(isalpha(h=getch()))
	    s[i++] = h;
	s[i] = '\0';
	ungetch(h);
	return c;
    }

    while (isdigit(h=getch()))	
	s[i++] = h;		/* integer part */
    if (h == '.') {
	s[i++] = h;		/* decimal point */
	while (isdigit(h=getch())) 
	    s[i++] = h;		/* fractional part */
    }

    s[i] = '\0';
    if (h != EOF)
	ungetch(h);

    return c;			/* NUMBER (value is in s) */
}

/* getch(): get a (possibly pushed back) character */
int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

/* ungetch(): push character back on input  */
void ungetch(int c)
{
    if (bufp >= BUFSIZE)
	fprintf(stderr, "ERROR: ungetc() too many charaters.\n");
    else
	buf[bufp++] = c;
}
    
