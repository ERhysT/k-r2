/* Exercise 7-1. Write a program that coverts upper case to lower or
   lower case to upper, depending on the name it is invoked with, as
   found in argv[0]. */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    int c, (*f)(int);

    if (!strcmp(*argv, "./tolower"))
	f = tolower;
    else if (!strcmp(*argv, "./toupper"))
	f = toupper;
    else
	return 1;

    while ((c = getchar()) != EOF)
	putchar(f(c));

    return 0;
}
