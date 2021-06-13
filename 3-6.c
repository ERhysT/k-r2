/* Exercise 3-6: Write a version of itoa that accepts three arguements
   instead of two. The third arguement is the mimimum field width; the
   converted number must be padded with blanks on the left if
   necessary to make it wide enough. */

#include <stdio.h>
#include "knr2.h"

/* itoaw() Converts the integer n into a decimal string s with a minimum
   field width of w. */
void
itoaw(int n, char *s, int w)
{
    unsigned i, m;
    
    i = 0;
    m = n<0 ? -n : n;

    do {
	s[i++] = (m%10) + '0';
	m /= 10;
    } while (--w > 0 || m > 0);	/* stop when field width and m are depleted */

    if (n<0)
	s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

int
main(int argc, char *argv[])
{
    int i, n;
    char s[MAX+1];
    
    if (argc<2) {
	fprintf(stderr, "USAGE: %s [integer 0] ... [integer N]\n", argv[0]); 
	return 1;
    }
    
    clear(s, MAX);
    for (i=1; i<argc; ++i) {
	n = atoi(argv[i]);
	itobw(n, s, 16, 5);
	printf("%s:%d:%s\n", argv[i], n, s);
	clear(s, MAX);
    }

    return 0;
}
