/* Excercise 5-3. Write a pointer version of the function strcat that
   we showed in Chapter 2: strcat(s,t) copies the string to to the end
   of s. */

#include <stdio.h>

/* str_cat_(): Concatenate t to s; s must be big enough */
void str_cat_(char *s, const char *t)
{
    while (*s++)
	;
    --s;
    while ((*s++ = *t++))
	;
}

int main(int argc, char **argv)
{
    char s[20];
    const char t[] = "Rhys";

    s[0]='M', s[1]='y', s[2]=' ';
    s[3]='n', s[4]='a', s[5]='m', s[6]='e'; s[7]=' ';
    s[8]='i', s[9]='s', s[10]=' ', s[11]='\0';

    str_cat_(s, t);
    puts(s);

    return 0;
}
