/* Excercise 5-2. Write get float, the floating point analog of
   getint(). What type does getfloat() return as its function
   value. */

#include <stdio.h>
#include <ctype.h>
#include <math.h>

#define MAX 10
#define issign(c) ( c=='+' || c=='-' )

int main()
{
    int ret, n, i, getfloat(double *);
    double array[MAX]; 

    for (n=0; n<MAX && (ret=getfloat(&array[n]))!=EOF; ++n)
	if (!ret)
	    n--;		/* delete invalid input */

    for (i=0; i<n; ++i)
	printf("%e\n", array[i]);

    return 0;
}

int getch(void);		/* character buffer from knr2.c */
void ungetch(int c);

/* getfloat(): get next double precision floating point value from
   stdin into *pn. 

   Returns: the number of digits read, EOF at end of file, 0 if no
   valid float could be read. */
int getfloat(double *pn)
{
    double coef, prec, exp;
    int csign, esign;
    int i, c, delim;

    while (isspace(c=getch()))
	;
    if (c == EOF)
	return EOF;
    if (c == '\n')
	return 0;

    csign = (c == '-') ? -1 : 1;
    if (!issign(c))
	ungetch(c);
    for (coef=0.0, i=0; isdigit(c=getch()); ++i)
	coef = coef*10.0 + (c - '0');

    delim = c;
    for (prec=1.0; delim=='.' && isdigit(c=getch()); ++i, prec/=10)
	coef = coef*10.0 + (c - '0');

    delim = c;
    esign = (c=getchar())=='-' ? -1 : 1;
    if (!issign(c))
	ungetch(c);
    for (exp=0.0; (delim=='E' || delim=='e') && isdigit(c=getch()); ++i)
	exp = exp*10.0 + (c - '0');

    *pn = coef * csign * prec * powf(10, exp * esign);

    return i;
}
