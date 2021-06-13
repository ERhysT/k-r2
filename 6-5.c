/* Excercise 6-5. Write a function undef that will remove a name and
   definition from the table maintained by lookup and install. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* simple version of the #define processor with no function like macros.

   Usage: ./6-6 < source.c 
 */
int main(void)
{
    return 0;
}
