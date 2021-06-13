/* Excercise 5-20. Expand dcl to handle declarations with function
   argument types and type qualifiers. */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

const char *qualifiers[] = { "const", "volatile" };
const char *storageclass[] = { "auto", "register", "static", "extern", "typedef" };
enum { NAME, QUALIFIER, PARENS, BRACKETS };

int dcl(void);
int dirdcl(void);

int gettoken(void);
int  tokentype;			/* type of last token  */
char token[MAXTOKEN];		/* last token string */

char name[MAXTOKEN];		/* identifier name */
char datatype[MAXTOKEN];	/* data type = int, char etc. */
char qualifier[MAXTOKEN];	/* type qualifier const, volatile etc. */
char out[100];			/* output string */

/* dcl(): parse a decelerator */
int dcl(void)
{
    int ns, c;

    for (ns = 0; (c=gettoken()) == '*' || c == QUALIFIER; ) {/* count *'s  */
	if (c == '*')
	    ns++;
	else  
	    dirdcl();
    }
    if (dirdcl() < 0)
	return -1;
    while (ns-- > 0) {
	strcat(out, qualifier);
	strcat(out, "pointer to");
    }
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
    } else if (tokentype == NAME) {/* variable name or type */
	strcpy(name, token);
    } else if (tokentype == QUALIFIER) {
	strcat(qualifier, token);
	strcat(qualifier, " ");
	return 0;
    } else {
	printf("Error: expected name or (dcl).\n");
	return -1;
    }
    while ((type=gettoken())==PARENS || type==BRACKETS)
	if (type == PARENS)
	    strcat(out, "function returning ");
	else {
	    strcat(out, "array");
	    strcat(out, token);
	    strcat(out, " of ");
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
    if (c == '(') {		/* parens */
	if ((c = getch()) == ')') {
	    strcpy(token, "()");
	    return tokentype = PARENS;
	} else {
	    ungetch(c);
	    return tokentype = '(';
	}
    } else if (c == '[') {	/* brackets */
	for (*p++ = c; (*p++ = getch()) != ']'; )
	    ;
	*p = '\0';
	return tokentype = BRACKETS;
    } else if (isalpha(c)) {	/* name or type */
	for (*p++ = c; isalnum(c=getch()); )
	    *p++ = c;
	*p = '\0';
	ungetch(c);
	return strcmp("const", token) && strcmp("volatile", token)
	    ? (tokentype = NAME) : (tokentype = QUALIFIER);
    } else 			/* other */
	return tokentype = c;
}

int main()			/* convert declaration into words */
{
    int c; 

    while ((c = gettoken()) != EOF) {
	if (c == '\n' || c == ';')
	    continue;
	if (tokentype == QUALIFIER) {
	    dirdcl();
	    continue;
	}
	strcpy(datatype, qualifier);
	strcat(datatype, token); 
	qualifier[0] = '\0';

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







