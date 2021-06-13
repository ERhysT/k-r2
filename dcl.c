/* Excercise 5-18. Make dcl recover from input errors. */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

void dcl(void);
void dirdcl(void);

int gettoken(void);
int tokentype;			/* type of last token  */
char token[MAXTOKEN];		/* last token string */
char name[MAXTOKEN];		/* iidentifier name */
char datatype[MAXTOKEN];	/* data type = int, char etc. */
char out[100];			/* output string */

/* dcl(): parse a decelerator */
void dcl(void)
{
    int ns;

    for (ns = 0; gettoken() == '*'; ) /* count *'s  */
	ns++;
    dirdcl();
    while (ns-- > 0)
	strcat(out, " pointer to");

}

/* dirdcl(): parse a direct decelerator */
void dirdcl(void)
{
    int type;

    if (tokentype == '(') {	/* dcl */
	dcl();
	if (tokentype != ')')
	    printf("Error: missing ')'.\n");
    } else if (tokentype == NAME) /* variable name */
	strcpy(name, token);
    else
	printf("Error: expected name or (dcl).\n");
    while ((type=gettoken())==PARENS || type==BRACKETS)
	if (type == PARENS)
	    strcat(out, " function returning");
	else {
	    strcat(out, " array");
	    strcat(out, token);
	    strcat(out, " of");
	}
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
    while (gettoken() != EOF) {	 /* 1st token on line */
	strcpy(datatype, token); /* is the data type */
	out[0] = '\0';
	dcl();
	if (tokentype != '\n')
	    printf("Syntax error.\n");
	printf("%s: %s %s\n", name, out, datatype);
    }

    return 0;
}
