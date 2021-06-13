/* Excercise 6-1. Our version of getword does not properly handle
   underscores (like_this) , string constants "mesela this", comments,
   or preprocessor control lines. Write a better version. */

#include <stdio.h>
#include <ctype.h>

#define MAX 1024

/* getword(): get token from line */
int getword(char *word, int lim)
{
    int c, getch(void);
    void ungetch(int);
    char *w = word;
    
    while (isspace(c = getch()))
	;
    if (c != EOF) 
	*w++ = c;
    if (c == '/') {
	if ((c = getch()) == '*') { /* comment block */
	    for (*w++='*'; --lim ; )
		if ((*w++ = getch()) == '*' && (*w++ = getch()) == '/')
		    goto done;
	} else if ( c == '/') {	    // line comment
	    for (*w++='/'; --lim ; )
		if ((*w++=getchar())=='\n')
		    goto done;
	} else {
	    ungetch(c);
	    goto done;
	}
    } else if (c == '"') {		/* string literal */
	while (--lim) {
	    if ((*w++ = getch()) == '\\')
		*w++ = getch();
	    if ((*w++ = getch()) == '"')
		goto done;
	}
    } else if (c == '\'') {		/* character */
	while (--lim) {
	    if ((*w++ = getch()) == '\\')
		*w++ = getch();
	    if ((*w++ = getch()) == '\'')
		goto done;
	}
    } else if (c == '#') {		/* cpp directive */
	while (--lim)
	    if ((*w++ = getchar()) == '\n')
		goto done;
    } else if (!isalpha(c)) {
	goto done;
    } else {
	for ( ; --lim ; w++)
	    if (!isalnum(*w = getch()) && *w != '_') {
		ungetch(*w);
		goto done;
	    }
    }
 done:
    *w = '\0';
    return (c == EOF) ? EOF : word[0];
}

int main(void)
{
    char word[MAX];

    while (getword(word, MAX) != EOF) 
	puts(word);

    return 0;
}
