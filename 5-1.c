/* Excercise 5-1. As written, getint treats a + or - not followed by a
   digit as a valid representation of zero. Fix it to push such a
   character back on the input.  */

#include <stdio.h>
#include <ctype.h>

#define MAX 10
#define NOT_INT 0


/* From knr2.c  */
#define issign(c) ( c=='+' || c=='-' )

int main()
{
    int ret, n, array[MAX], getint(int *);

    for (n=0; n<MAX && (ret=getint(&array[n]))!=EOF; n++)
	if (ret == NOT_INT)
	    n--;

    while (--n >= 0)
	printf("%d\n", array[n]);

    return 0;
}

int getch(void);
void ungetch(int c);

/* getint(): get next integer from input into *pn.

   If *pn is a valid integer getint() returns a positive value. When
   all input has been read, EOF is returned. If the string
   does not represent a valid integer, NOT_INT is returned. */
int getint(int *pn)
{
    int c, h, sign;
    
    while(isspace(c=getch()))	/* skip whitespace and newlines etc. */
	;
    if (c==EOF)
	return EOF;
    else if (!isdigit(c) && !issign(c)) {
	return NOT_INT;
    }

    sign = (c=='-') ? -1 : 1;

    h = getch(); /* need two chars to determine if signed int */
    if (issign(c) && isdigit(h)) {
	c = h;			/* sign has been stored, pass it */
    } else if (issign(c) && !isdigit(h)) {
	ungetch(c);
	ungetch(h);
	return NOT_INT;
    } else {
	ungetch(h);
    }
    if (c == EOF)
	return 1;

    for (*pn=0; isdigit(c); c=getch())
	*pn = 10 * *pn + (c-'0');
    *pn *= sign;
    ungetch(c);

    return 1;
}

#define BUFSIZE 100

char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0; /* next free position in buf */

/* getch(): get a (possibly pushed back) character */
int getch(void) 
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

/* ungetch(): push character back on input */
void ungetch(int c) 
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}
