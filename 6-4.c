/* Excercise 6-4.  Write a program that prints the distinct words in
   its input sorted into decreasing order of frequency of
   occurrence. Preceed each word by its count. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* tnode: binary search tree */
#define REFMAX 10000
struct tnode {			/* a tree node */
    char *word;
    int count;			/* # occurences of word  */
    unsigned lines[REFMAX];	/* line references to word */
    struct tnode *left;
    struct tnode *right;
};
struct tnode *addtree(struct tnode *p, char *w, unsigned line);
struct tnode *talloc(void);
unsigned tsize(struct tnode *p);
void tprintcount(struct tnode *p);
unsigned tflatten(struct tnode *p, struct tnode **treeptr, unsigned i);

/* word: input from stdin */
#define WORDMAX 10000
int getword(char *word, int lim);

/*
 * Implementation
*/

/* addtree():  add a node with w, at or below p */
struct tnode *addtree(struct tnode *p, char *w, unsigned line)
{
    int cond;

    if (p == NULL) {		/* w is a new word */
	p = talloc();
	p->word = strdup(w);
	p->count = 1;
	p->lines[0] = line;  
	p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0)
	p->lines[p->count++] = line; /* w is here */
    else if (cond < 0)
	p->left = addtree(p->left, w, line);
    else if (cond > 0)
	p->right = addtree(p->right, w, line);
    
    return p;
}

/* talloc(): make a tnode */
struct tnode *talloc(void)
{
    struct tnode *new = malloc(sizeof *new);
    if (new == NULL) {
	fprintf(stderr, "Error: critical memory error\n");
	exit(1);
    } 

    return new; 
}

/* tprintcount(): prints tree p sorted by decreasing count */
void tprintcount(struct tnode *p)
{
    int n, countcmp(const void *n1, const void *n2);
    struct tnode **treeptr;

    treeptr = calloc(tsize(p), sizeof *treeptr);
    if (treeptr == NULL) {
	fprintf(stderr, "Error: critical memory error\n");
	exit(1);
    }
    n = tflatten(p, treeptr, 0);
    qsort(treeptr, n, sizeof *treeptr, countcmp);
    while (n--)
	printf("\tcount=%3d, word=%s\n", treeptr[n]->count, treeptr[n]->word); 
}

/* tflatten(): copy pointers of each node in tree p to array treeptr,
   starting at index i.

   Returns number of node pointers written to treeptr. */
unsigned tflatten(struct tnode *p, struct tnode **treeptr, unsigned i)
{
    if (p != NULL) {
	treeptr[i++] = p;
	i = tflatten(p->left, treeptr, i);
	i = tflatten(p->right, treeptr, i);
    }
    return i;
}

/* tsize: number of nodes in tree */
unsigned tsize(struct tnode *root)
{
    return (root == NULL) ? 0 : 1 + tsize(root->left) + tsize(root->right);
}

/* countcmp(): returns value greater than, less than or equal to zero
   when the count field of n1 is bigger, smaller or less than that of
   n2 respectively. */
int countcmp(const void *v1, const void *v2)
{
    struct tnode **n1 = (struct tnode **) v1;
    struct tnode **n2 = (struct tnode **) v2;

    return ((*n1)->count > (*n2)->count) - ((*n1)->count < (*n2)->count);
}

/* getword(): get token from stdin */
int getword(char *word, int lim)
{
    int c, getch(void);
    void ungetch(int);
    char *w = word;
    
    while (isblank(c = getch()))
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

/* Sorts words by frequency. Usage: ./6-3 < file.txt  */
int main(int argc, char *argv[])
{
    struct tnode *root;
    unsigned line;
    char word[WORDMAX];
    int c;

    root = NULL;
    line = 1;
    while ((c = getword(word, WORDMAX)) != EOF) 
	if (isalpha(c))
	    root = addtree(root, word, line);
	else if (c == '\n') 
	    ++line;
    puts("Printing tree counts in decending order:");
    tprintcount(root);

    return 0;
}
