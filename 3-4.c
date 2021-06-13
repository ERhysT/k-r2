/* Excercise 3-4. In a two's compilment number representation our
   version of itoa() doesn't not handle the largest negative integer,
   that is, the value of 2^(wordsize-1). Modify it to print
   the value correctly, regardless of the machine on which it runs. */

#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAX 80

void
reverse(char *s)
{
    int c, i, j;

    for (i=0, j=strlen(s)-1; i<j; i++, j--) {
	c = s[i];
	s[i] = s[j];
	s[j] = c;
    }
}

/* In a 2's compliment number representation the range expressiable is
   -((~0/2)+1) <= n <= (~0/2) for example on this system int is 4
   bytes (32bits) long so:

        -(2^(wordsize-1) <= n <  2^(wordsize-1)
             -(2^(32-1)) <= n <  2^(32-1)

   -((0xFFFFFFFF/2) + 1) <= n <= (0xFFFFFFFF/2)
             -2147483648 <= n <= 2147483647
                   nmin  <= n <= nmax

   As can be seen |nmin| > |nmax| so when n is made positive
   during the sign check and n==nmin there is an overflow.
*/

void
itoa(int n, char *s)
{
    int i, sign;

    if((sign=n)<0)		/* record sign */
	n = -n;			/* make n positive */
    i = 0;
    do {			/* generate digits in reverse order */
	s[i++] = n%10 + '0';	/* get next digit */
    } while ((n/=10) > 0);	/* delete it */
    if (sign<0)
	s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

/* itoa1(): as itoa() but overflow was fixed by using an unsigned
   integer to hold the absolute value of n preventing the overflow
   when n=nmin. */
void
itoa1(int n, char *s)
{
    int i, sign;
    unsigned abs;
    
    if (n<0) {
	sign = -1;
	abs = -n;
    } else {
	sign = 1;
	abs = n;
    }

    i = 0;
    do {			/* generate digits in reverse order */
	s[i++] = abs%10 + '0';	/* get next digit */
    } while ((abs/=10) > 0);	/* delete it */
    if (sign<0)
	s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

int
atoi(const char *s)
{
    int n, i, sign;

    i = 0;
    sign = s[i]=='-' ? -1 : 1;
    if (s[i]=='-' || s[i]=='+')
	++i;
    for (; s[i]!='\0'; ++i) 
	n = 10*n + (s[i]-'0');

    return n * sign;
}

int 
main(int argc, char *argv[])
{
    int i, n;
    char s[MAX];

    if (argc<2) {
	fprintf(stderr, "USAGE: %s [str1] ... [strn]\n", argv[0]);
	return 1;
    }

    for (i=1; i<argc; ++i) {
	n = atoi(argv[i]);
	itoa1(n, s);
	printf("%s:%d:%s\n", argv[i], n, s);
    }
	
    return 0;
}
