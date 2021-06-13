/* Exercise 1-16. Revise the main routine of the longest-line program
   so it will correctly print the length of arbitrarily long input
   lines, and as much as possible of the text. */

/* The line below is exactly 207 characters long (including newline)
   and should show as the longest line and print this value when this
   program is run as: $ ./1-16 < 1-16.c */

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */

#include <stdio.h>
#define MAXLINE 100		/* maximum input line size
				   longer lines are 'long lines'*/

int getaline(char line[], int maxline);
void copy(char to[], char from[]);

/* print longest input line */
int main()
{
    int len;			/* current line length */
    int max;			/* maximum length seen so far */
    char line[MAXLINE];		/* current input line */
    char longest[MAXLINE];	/* longest line saved here */

    char overflow[MAXLINE];	/* stores start of long lines */
    int longlen;		/* tmp length counter */

    max = 0;
    while ((len = getaline(line, MAXLINE)) > 0) {
	if (len==MAXLINE-1 && line[len-1]!='\n') { /* over-long line */
	    copy(overflow, line);
	    while ((longlen = getaline(line, MAXLINE)) > 0) {
		len += longlen;
		if (longlen < MAXLINE-1) /* long line end */
		    break;
	    }
	    copy(line, overflow);
	}
	if (len > max) {
	    max = len;
	    copy(longest, line);
	}
    }
    if (max > 0)		/* there was a line */
	printf("longest line is %d:\n%s\n", max, longest);
    return 0;
}

/* getaline: read a line into s, return length (up to lim - 1) */
int getaline(char s[], int lim)
{
    int c, i;

    for (i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
	s[i] = c;
    if (c == '\n') {
	s[i] = c;
	++i;
    }
    s[i] = '\0';
    return i;
}

/* copy: copy 'from' into 'to'; assume to is big enough */
void copy(char to[], char from[])
{
    int i;
    i = 0;
    while ((to[i] = from[i]) != '\0')
	++i;
}

    
