/* Excercise 7-2. Write a program that will print arbitary input in a
   sensible way. As a minimum, it should print non-grahic characters
   in octal or hexadecimal according to local custom, and break long
   text lines */

#include <stdio.h>
#include <ctype.h>

#define LINEMAX 75

int main(void)
{
    int c, llen;


    for ( llen = 0; ((c =  getchar())) != EOF; llen++) {
	if (c == '\n') 
	    llen = 0;
	else if (llen == LINEMAX) {
	    putchar('\n');
	    llen = 0;
	}

	if (iscntrl(c))
	    printf("/%03o", c);

	putchar(c);
    }

    return 0;
}
