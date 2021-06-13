#include <stdio.h>

/* Exercise 1-19: Write a function 'reverse(s)' that reverses the
   character string s. Use it to write a program that reverses its
   input a line at a time. */

#define MAX 80

/* reverse: reverses the 'in' string and stores in 'out'. Out is
   always null terminated and newlines are preserved at their original
   position. */
void reverse(const char in[], char out[], int len)
{
    int i;

    out[len--] = '\0';
    if (in[len] == '\n')
	out[len--] = '\n';

    for (i=0; len>=0; ++i, --len)
	out[i] = in[len];
    
    return;
}

/* readaline: Copies stdin into buf until newline, EOF or buffer is
   full and returns the number of bytes written (excluding the
   terminating null byte, which is always written). */
int readaline(char buf[], int max)
{
    int i, c;

    for (i=0; i<max-1 && (c=getchar())!=EOF && c!='\n'; ++i)
	buf[i] = c;

    /* newline character indicated that end of line was reached, but
       it is still a character and needs to be written. */
    if (c == '\n')
	buf[i++] = c;

    buf[i] = '\0';		/* ith always free i<max-1 */

    return i;
}

/* printline: prints one line. */
void printaline(char buf[])
{
    printf("%s", buf);
    return;
}

int main()
{
    char forw[MAX+1], back[MAX+1];
    int len;

    while ((len = readaline(forw, MAX))) {
	reverse(forw, back, len);
	printaline(back);
    }

    return 0;
}

  
