/* Excercise 4-12. Write a recursive version of itoa() */

#include <stdio.h>
#include "knr2.h"

/* itoa_r(): converts an i into a string, populating s using a recursive
   routine. */
void itoa_r(int n, char *s)
{
    static int i;

    if (n/10) 
	itoa_r(n/10, s);

    s[i++] = n%10 + '0';
    s[i] = '\0';
}

int main()
{
    char s[64];
    int n;

    n = 123;
    itoa_r(n, s);

    printf("%d:%s\n", n, s);

    return 0;
}
