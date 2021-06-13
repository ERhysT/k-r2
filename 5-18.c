/* Excercise 5-18. Make dcl recover from input errors. */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

int dcl(void);
int dirdcl(void);

int gettoken(void);
int tokentype;			/* type of last token  */
char token[MAXTOKEN];		/* last token string */
char name[MAXTOKEN];		/* iidentifier name */
char datatype[MAXTOKEN];	/* data type = int, char etc. */
char out[100];			/* output string */

/* dcl(): parse a decelerator */
int dcl(void)
{
    int ns;

    for (ns = 0; gettoken() == '*'; ) /* count *'s  */
	ns++;
    if (dirdcl() < 0)
	return -1;
    while (ns-- > 0)
	strcat(out, " pointer to");
    return 0;
}

/* dirdcl(): parse a direct decelerator */
int dirdcl(void)
{
    int type;

    if (tokentype == '(') {	/* dcl */
	dcl();
	if (tokentype != ')')
	    printf("Error: missing ')'.\n");
    } else if (tokentype == NAME) {/* variable name */
	strcpy(name, token);
    } else {
	printf("Error: expected name or (dcl).\n");
	return -1;
    }
    while ((type=gettoken())==PARENS || type==BRACKETS)
	if (type == PARENS)
	    strcat(out, " function returning");
	else {
	    strcat(out, " array");
	    strcat(out, token);
	    strcat(out, " of");
	}
    return 0;
}

/* gettoken(): returns next token */
int gettoken(void)
{
    int c, getch(void);
    void ungetch(int);
    char *p = token;

    while ((c = getch()) == ' ' || c == '\t')
	;
    if (c == '(') {
	if ((c = getch()) == ')') {
	    strcpy(token, "()");
	    return tokentype = PARENS;
	} else {
	    ungetch(c);
	    return tokentype = '(';
	}
    } else if (c == '[') {
	for (*p++ = c; (*p++ = getch()) != ']'; )
	    ;
	*p = '\0';
	return tokentype = BRACKETS;
    } else if (isalpha(c)) {
	for (*p++ = c; isalnum(c=getch()); )
	    *p++ = c;
	*p = '\0';
	ungetch(c);
	return tokentype = NAME;
    } else 
	return tokentype = c;
}

int main()			/* convert declaration into words */
{
    int c; 

    while ((c = gettoken()) != EOF) {	/* 1st token on line is the data type */
	if (c == '\n' || c == ';')
	    continue;
	strcpy(datatype, token);
	out[0] = '\0';
	if (dcl() < 0)
	    continue;
	if (tokentype != '\n' && tokentype != ';') {
	    printf("Syntax error.\n");
	    continue;
	}
	printf("%s: %s %s\n", name, out, datatype);
    }

    return 0;
}
