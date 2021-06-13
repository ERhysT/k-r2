/* Excercise 6-3. Write a cross-referencer that prints a list of all
   words in a document, and, for each word, a list of the line numbers
   on which it occurs. Remove noise words like "the", "and" and so
   on.*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>


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
void treeprint(struct tnode *p);
void treematch(struct tnode *p, int n);

/* word: input from stdin */
#define WORDMAX 10000
int get_word(char *word, int lim);

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

/* isnoise(): returns true if w is in blocklist. */
int isnoise(char *w)
{
    int i;

    static const char *bl[] = /* blocklist to ignore */
	{ "the", "and", "to", "of", "I", "a", "in", "my",
	  "was", "it", "that", "which", "when" };

    for (i = 0; i < (sizeof bl / sizeof *bl); ++i) 
	if (!strcmp(w, bl[i]))
	    return 1;

    return 0;
}
    
/* treeprint(): in-order print of tree p */
void treeprint(struct tnode *p)
{
    int i;

    if (p != NULL) {
	treeprint(p->left);
	if (!isnoise(p->word)) {
	    printf("%4d %s: ", p->count, p->word);
	    for (i = 0; i < p->count; ++i)
		printf("%d ", p->lines[i]);
	    putchar('\n');
	}
	treeprint(p->right);
    }
}

/* Prints a list of all words unique words while referening the line
   of each occurence.
   
   USAGE: ./6-3 < file.txt  */
int main(int argc, char *argv[])
{
    struct tnode *root;
    unsigned line;
    char word[WORDMAX];
    int c;

    root = NULL;
    line = 1;
    while ((c = get_word(word, WORDMAX)) != EOF) 
	if (isalpha(c))
	    root = addtree(root, word, line);
	else if (c == '\n') 
	    ++line;
		
    treeprint(root);

    return 0;
}
