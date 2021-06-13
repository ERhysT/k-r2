/* Exercise 1-24: Write a program to check a C program for rudimentary
   syntax errors like unbalanced parentheses, brackets and
   braces. Don't forget about quotes, single and double, escape
   sequences and comments. This program is hard if you do it in full
   generality. */

#include <stdio.h>

#define N_ERR 6
enum {PARENTHESES, BRACKETS, BRACES, QUOTES, DQUOTES, COMMENTS};  
const char *errstr[N_ERR] = {"Parentheses", "Brackets", "Braces", "Quotes", "Double Quotes", "Comments"};

/* lazy way not allowed to use stdlib */
int absolute(int n)
{
    return (n*n) / n;
}


/* Prints syntax errors to stdout, returning the total number of
   errors */
int
print_err(int *err)
{
    int i, n;

    for (n=i=0; i<N_ERR; ++i) {
	n += absolute(err[i]);
	if (err[i])
	    printf("Syntax Error:%s:%d\n", errstr[i], err[i]);
    }

    return n;
}

/* resets the counts to zero */
void
clear_count(int *err)
{
    int i;

    for (i=0; i<N_ERR; ++i)
	err[i] = 0;
}

/* Reads a character or EOF from stdin and returns it, if it is a
   valid character, it is written to stdout. */
int
passchar(void)
{
    int ch;

    ch = getchar();
    if (ch!=EOF)
	putchar(ch);

    return ch;
}

int
main(int argc, char *argv[])
{
    int ch;			/* current character */
    int nopen[N_ERR];		/* array of open counts */

    clear_count(nopen);

    while (ch=passchar(), ch!=EOF) {

	/* Add up the comments unless in a " */
	if (!nopen[DQUOTES] && !nopen[QUOTES]) {
	    if (ch=='/' && passchar()=='*') {
		++nopen[COMMENTS];
	    }
	    if (ch=='*' && passchar()=='/') {
		--nopen[COMMENTS];
	    }
	}

	/* Pass escape sequences within any quotes " ' */
	if (nopen[DQUOTES] || nopen[QUOTES])
	    if (ch=='\\')
		passchar();

	/* Tally single quotes ' */
	if (!nopen[DQUOTES] && !nopen[COMMENTS])
	    if (ch == '\'') 
		nopen[QUOTES] = !nopen[QUOTES];

	/* Tally double quotes " */
	if (!nopen[COMMENTS] && !nopen[QUOTES]) {
	    if (ch=='"')
		nopen[DQUOTES] = !(nopen[DQUOTES]);
	}

	/* Following tallies not performed within comments or quotes */
	if (!nopen[COMMENTS] && !nopen[DQUOTES] && !nopen[QUOTES]) {
	    /* Parentheses ( */
	    if (ch=='(')
		++nopen[PARENTHESES];
	    if (ch==')')
		--nopen[PARENTHESES];
	    /* Brackets [ */
	    if (ch=='[')
		++nopen[BRACKETS];
	    if (ch==']')
		--nopen[BRACKETS];
	    /* Braces { */
	    if (ch=='{')
		++nopen[BRACES];
	    if (ch=='}')
		--nopen[BRACES];
	}

	/* Fatal Errors */
	if (absolute(nopen[COMMENTS]) > 1) {/* cannot nest comments */
	    puts(" <<< Nested comments");
	    break;
	}
	if (nopen[PARENTHESES] < -1) { /* too many ) */
	    puts(" <<< Missmatched parentheses");
	    break;
	}
	if (nopen[BRACKETS] < -1) { /* too many ] */
	    puts(" <<< Missmatched brackets");
	    break;
	}
	if (nopen[BRACES] < -1) { /* too many } */
	    puts(" <<< Missmatched braces");
	    break;
	}
    }

    return print_err(nopen);
}
