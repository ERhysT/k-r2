/* Exercise 1-18: Write a program to remove trailing blanks and tabs
   from each line of input, and to delete entirely blank lines. */

#include <stdio.h>
#include <ctype.h>

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

/* trim: removes trailing whitespace from buf and returns new length
   (exculding null terminating character). */
int trim(char buf[], int len)
{
    int i;

    /* move backwards until non whitespace is reached starts at two
       less than length to avioid the terminating null and newline */
    for (i=len-2; i>=0 && isspace(buf[i]); --i)
	--len;

    buf[len-1] = '\n';
    buf[len]   = '\0';

    return len;
}

/* ismt: returns zero if buffer has at least one non-whitespace
   character in first len elements. */
int ismt(char buf[], int len)
{
    while (--len >= 0)
	if (!isspace(buf[len]))
	    return 0;		/* not empty */

    return 1;			/* empty */
}


int main()
{
    char buf[MAX+1];		/* fits max chars and null byte  */
    int len;			/* characters in buffer */

    while ((len = readaline(buf, MAX))) { /* each line */
	while (len==MAX && buf[MAX-1]!='\n') { /* loop read long line */
	    printaline(buf);
	    readaline(buf, MAX);
	}
	
	len = trim(buf, len);	/* trip whitespace off end */

	if (!ismt(buf,len))	/* print if not empty */
	    printaline(buf);
    }

	/* blank lines */

    return 0;
}
