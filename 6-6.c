/* Excercise 6-5. Write a function undef that will remove a name and
   definition from the table maintained by lookup and install. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* hastab: array of elements pointing to start of linked list */
#define HASHSIZE 101			/* table size  */
static struct nlist *hashtab[HASHSIZE]; /* pointer table */
struct nlist {				/* table entry */
    struct nlist *next;
    char *name;
    char *defn;
};
unsigned hash(char *s);
struct nlist *lookup(char *s); 
struct nlist *install(char *name, char *defn);
void undef(char *name);

/* io: stdin tokenised, processesed and output to stout */
#define TOKENMAX 2048
void puttoken(char *s);
int gettoken(char *token, int lim); 
int getch(void);
void ungetch(int);

/* hash(): form hash value for string s */
unsigned hash(char *s)
{
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++)
	hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

/* lookup(): look for s in hashtab */
struct nlist *lookup(char *s)
{
    struct nlist *np;

    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
	if (strcmp(s, np->name) == 0)
	    return np;		/* found */
    return NULL;		/* not found */
}

/* install(): put (name, defn) in hashtab, overwriting any previous
   definition */
struct nlist *install(char *name, char *defn)
{
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL) { /* not found */
	np = malloc(sizeof *np);
	if (np == NULL || (np->name = strdup(name)) == NULL)
	    return NULL;
	hashval = hash(name);
	np->next = hashtab[hashval];
	hashtab[hashval] = np;
    } else {			/* already there */
	free(np->defn);
    }
    if ((np->defn = strdup(defn)) == NULL)
	return NULL;
    return np;    
}

/* undef(): removes defn of name from hashtab */
void undef(char *name)
{
    struct nlist *np;  

    if ((np = lookup(name))) {
	free(np->name);
	free(np->defn);
    }
}

int gettoken(char *token, int lim)
{
    enum { TOOFAR    = -1,
	   DONE      =  0,
	   LCOMMENT  =  1,
	   BCOMMENT  =  2,
	   BCOMMENT2 =  3  };	/* characters are also valid states */

    int c, state; 
    char *t = token;

    while (isspace(c=getch()))
	;
    if (c != EOF)
	state = *t++ = c;
    while ((c = getch()) != EOF && lim--) {
	*t++ = c;
	if (state == '#') {
	    if (isspace(c))
		state = TOOFAR;
	} else if (state == '"') {
	    if (c == '\\')
		state *= -1;          
	    else if (c == '"') 
		state = DONE;
	} else if (state == '"' * -1) {	/* string escape sequence state */
	    state *= -1;
	} else if (state == '\'') {
	    if (c == '\\')
		state *= -1;
	    else if (c == '\'')
		state = DONE;
	} else if (state == '\'' * -1) { /* char escape sequence state */
	    state *= -1;
	} else if (isdigit(state)) {
	    if (c == 'L' || c == 'U')
		state = DONE;
	    else if (!isdigit(c))
		state = TOOFAR;
	} else if (isalpha(state)) {
	    if (!isalnum(c) || c == '_')
		state = TOOFAR;
	} else if (state == '/') {
	    if (c == '/')
		state = LCOMMENT;
	    if (c == '*')
		state = BCOMMENT;
	    else		/* '/' operator */
		state = TOOFAR;
	} else if (state == LCOMMENT) {
	    if (c == '\n')
		state = TOOFAR; 
	} else if (state == BCOMMENT) {
	    if (c == '*')
		state = BCOMMENT2;
	} else if (state == BCOMMENT2) {
	    if (c == '/')
		state = DONE;
	    else
		state = BCOMMENT;
	} else {		/* single char operator */
	    state = TOOFAR;
	}

	switch (state) {
	case TOOFAR:
	    ungetch(*--t);
	case DONE:		/* fallsthrough to return */
	    *t = '\0';
	    return token[0];
	}
    }
	return EOF;
}
    
void puttoken(char *s)
{
    char c;

    while ((c = *s++)) {
	putchar(c);
	if ( c == '{' || c == '}' || c == ';')
	    putchar('\n');
    }
    putchar(' ');
}

/* die(): print s to stderr and exit with status 1 */
void die(char *s)
{
    fprintf(stderr, "Error: %s.\n", s);
    exit(1);
}

/* simple version of the #define processor with no function like macros.

   Usage: ./6-6 < source.c 
 */
int main(void)
{
    char token[TOKENMAX], name[TOKENMAX];
    enum { TOKEN, DEFINE, INSTALL } state;  
    struct nlist *np;

    state = TOKEN;
    while (gettoken(token, TOKENMAX) != EOF) {
	if (!strcmp(token, "#define")) {
	    state = DEFINE;
	} else if ( state == DEFINE ) {
	    strcpy(name, token);
	    state = INSTALL;
	} else if (state == INSTALL) {
	    install(name, token);
	    state = TOKEN;
	}  else if (state == TOKEN) {
	    puttoken((np = lookup(token)) ? np->defn: token);
	}
    }
}
