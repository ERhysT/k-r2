/* Exercise 4-3. Given the basic framework, it's straightforward to
   extend the calculator. Add the modulus operator and provisions for
   negative numbers. */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAXOP   100		/* max size of operand or operator */
#define NUMBER  '0'

int getop(char []);	   /* get next operator or numeric operand. */

/* val[]: FIFO stack for floating point values */
#define MAXVAL  100
double val[MAXVAL];		
int sp = 0;			/* next free stack position */
void push(double);
double pop(void);

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
    double op2;
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
	    op2 = pop();
	    push(pop() - op2);
	    break;
	case '/':
	    op2 = pop();
	    if (op2 != 0)
		push(pop() / op2);
	    else
		fprintf(stderr, "ERROR: cannot divide by 0.\n");
	    break;
	case '%': 
	    op2 = pop();
	    push(fmod(pop(), op2));
	    break;
	case '\n': case EOF:
	    printf("\t%.8g\n", pop());
	    break;
	default:
	    fprintf(stderr, "ERROR: unknown command '%s'.\n", s);
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

/* getop(): get next operator or numeric operand.

   When an optionally signed decimal string is read, NUMBER is
   returned and the string stored in s.
   
   When a non digit (i.e. not 0-9) is read, it is returned and s will
   be a string of length 1 also containing the character.

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
    if (c != NUMBER)
	return c;		/* is an operator */
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
    
