/* Exercise 1-17. Write a program to print all input lines that are
   longer than 80 characters. */

#include <stdio.h>

#define MAX 80		      /* threshold over which to print line */

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
    char buf[MAX+1];	  /* buffer to hold max chars and null byte */
    int len;		  /* length of a line */
    
    while ((len = readaline(buf, MAX+1))) {
	if (len == MAX)	{	/* threshold reached  - print line*/
	    printaline(buf);
	    while (len == MAX && buf[MAX-1] != '\n') { /* line unfinished */
		len = readaline(buf, MAX+1);
		printaline(buf);
	    }
	}
    }
    return 0;
}
