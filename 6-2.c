/* Excercise 6-1. Write a program that reads a C program and prints in
   alphabetical order each group of variable names that are identical
   in the first 6 characters, but different somewhere
   thereafter. Don't count words within strings and comments. Make 6 a
   parameter that can be set from the command line. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * Declerations
*/

/* tnode: binary search tree */
struct tnode {			/* a tree node */
    char *word;
    int count;
    struct tnode *left;
    struct tnode *right;
};
struct tnode *addtree(struct tnode *p, char *w);
struct tnode *talloc(void);
void treeprint(struct tnode *p);
void treematch(struct tnode *p, int n);

/* matchstack: contains words with matching prefixes */
#define MATCHMAX 1000
char *matchstack[MATCHMAX];
char **matchpos = matchstack;
int matchn;			   /* number of leading characters to match */
char *matchprefix;		   /* current cpword to matchn chars from */
void pushmatch(char *w);
char *popmatch();

/* word: input from stdin */
#define WORDMAX 10000
int get_word(char *word, int lim);

/*
 * Implementation
*/

/* addtree():  add a node with w, at or below p */
struct tnode *addtree(struct tnode *p, char *w)
{
    int cond;

    if (p == NULL) {		/* w is a new word */
	p = talloc();
	p->word = strdup(w);
	p->count = 1;
	p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0)
	p->count++;		/* w is here */
    else if (cond < 0)
	p->left = addtree(p->left, w);
    else if (cond > 0)
	p->right = addtree(p->right, w);
    
    return p;
}

/* talloc(): make a tnode */
struct tnode *talloc(void)
{
    return malloc(sizeof (struct tnode)); 
}

/* treeprint(): in-order print of tree p */
void treeprint(struct tnode *p)
{
    if (p != NULL) {
	treeprint(p->left);
	printf("%4d %s\n", p->count, p->word);
	treeprint(p->right);
    }	    
}

/* treematch(): print words that with same first n characters */
void treematch(struct tnode *p, int n)
{
    char *w ;

    if (p == NULL)
	return;

    treematch(p->left, n);
    
    if (!strncmp(matchprefix, p->word, n)) {
	pushmatch(p->word);
    } else {
	if ((w = popmatch()) != NULL) { 
	    printf("%s ", matchprefix);
	    do {
		printf("%s ", w);
	    } while ((w = popmatch()) != NULL);
	    putchar('\n');
	}
	matchprefix = p->word;
    }

    treematch(p->right, n);
}


/* pushmatch(): push w to matchstack */
void pushmatch(char *w)
{
    if (matchpos < matchstack+MATCHMAX)  
	*matchpos++ = w;
    else
	fprintf(stderr, "Error: stack full.\n");
}

/* popmatch(): pop next word from matchstack */
char *popmatch(void)
{
    return (matchpos > matchstack) ? *--matchpos : NULL;
}

/* Reads a C program and prints in alphabetical order each group of
   variable names that are identical in the first n characters.

   USAGE: ./6-2 [n]

   n defaults to 6 if not specified.
   */
int main(int argc, char *argv[])
{
    struct tnode *root;
    char word[WORDMAX];

    matchn = (argc < 2) ? 6 : atoi(argv[1]);
    if ((matchprefix = malloc(matchn+1)) == NULL)
	return 1;
    memset(matchprefix, '\0', matchn+1);

    root = NULL;
    while (get_word(word, WORDMAX) != EOF) 
	if (isalpha(word[0]))
	    root = addtree(root, word);
    //treeprint(root);
    treematch(root, matchn);

    return 0;
}
