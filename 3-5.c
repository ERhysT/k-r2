/* Excercise 3-5. Write the function itob(n,s,b) that converts the
   integer into a base b character representation in the string s. In
   particular itob(n,s,16) formats n as a hexadecimal integer in s. */

#include <stdio.h>

#define MAX 80

/* str_len(): returns the number of characters in s exculding the null
   terminating byte. */
unsigned
str_len(char *s)
{
    unsigned u;

    for (u=0; s[u]!='\0'; ++u)
	;
	
    return u;
}

/* reverse(): reverses the string in s */
void
reverse(char *s)
{
    int c, i, j;

    for (i=0, j=str_len(s)-1; i<j; i++, j--) {
	c = s[i];
	s[i] = s[j];
	s[j] = c;
    }
}

/* atoi(): converts decimal string into its signed integer value */
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

/* itob(): converts n into a base b character representation in string
   s for 0 < b <= 16 using the character set 0-9A-F. */
void
itob(int n, char *s, int b)
{
    unsigned i, m;		/* magnitute |n| */
    const char c[] = "0123456789ABCDEF";

    if (b<2 || b>16) {
	s[0] = '\0';
	return;
    }
    
    i = 0;
    m = n<0 ? -n : n; 
    do {			/* generate digits in reverse order */
	s[i++] = c[m%b];	/* get next digit */
    } while ((m/=b) > 0);	/* delete it */

    if (n<0)
	s[i++] = '-';

    s[i] = '\0';
    reverse(s);
}

/* clear() sets all bytes in s to null where the lenth of the buffer
   s, len > 0. */
void
clear(char *s, int len)
{
    do {
	s[len] = '\0';
    } while (--len);
}

int 
main(int argc, char *argv[])
{
    int i, n, base;
    char s[MAX];

    if (argc<2) {
	fprintf(stderr, "USAGE: %s [decimal0] ... [decimalN]\n", argv[0]);
	return 1;
    }

    for (i=1; i<argc; ++i) {
	clear(s, MAX);
	n = atoi(argv[i]);
	puts(argv[i]);

	for (base=2; base<=16; ++base)  {
	    itob(n, s, base);
	    printf("Base %.2d: %s\n", base, s);
	}
	putchar('\n');
    }
	
    return 0;
}
