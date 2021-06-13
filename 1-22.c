/* Exercise 1-22. Write a program to "fold" long input lines into two
   or more shorter lines after the last non-blank character that
   occurs before the n-th column of input. Make sure your program does
   something intelligent with very long lines, and if there are no
   blanks or tabs before the specified column. */

#include <stdio.h>

#define MAX 79			/* characters in one line  */

/* Reads a line from stdin or max characters, whichever is
   less. Returns the number of characters read. */
int
get_line(char *out, int max)
{
    int ch, len;

    for (len=0; len<max && (ch=getchar())!=EOF; ++len) {
	out[len] = ch;
	if (ch == '\n') {
	    ++len;
	    break;
	}
    }

    out[len] = '\0';

    return len;
}

/* Prints at most len characters from line to stdout  */
void
put_line(char *line, int len)
{
    for (int i=0; i<len && line[i]!='\0'; ++i)
	putchar(line[i]);
}

/* Folds line by replacing the last whitespace character with a
   newline.

   Searches for whitespace starting at the last character, and ending
   on the second character.

   Returns the number of characters remaining after the fold, or if
   unfoldable, the original length len. */
int
fold_line(char *line, int len)
{
    int i;

    for (i=len-1; i>0; --i) 	
	if (line[i]=='\t' || line[i]==' ') {
 	    line[i] = '\n';
	    return len-1 - i;
	}

    return len;
}

/* Reads stdin and prints a 'folded' version to std out. */
int
main(int argc, char *argv[])
{
    char lbuf[MAX+1];
    int len;
    
    while (len=get_line(lbuf, MAX), len > 0) 
	if (len<MAX)		/* short line - printable */
	    put_line(lbuf, len);
	else			/* long line - try to fold */
	    if (fold_line(lbuf, len) < len) 
		put_line(lbuf,len);
	    else {
		put_line(lbuf,len);
		putchar('\n');
	    }

    /* This is a comment */
    puts("This isn't /* a comment */ as it's inside a string "); /* I'm a comment */

    return 0;
}
