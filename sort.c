/* Exercise 5-7. Rewrite readlines() to store lines in an array
   supplied by main, rather than calling alloc to maintain
   storage. How much faster is the program? */

#include <stdio.h>
#include <string.h>

#define MAXLINES 1000		/* max #lines to be sorted  */

char *lineptr[MAXLINES]; 	/* pointers to text lines */

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines); 

void q_sort(char *linetpr[], int left, int right);

/* Sort input lines   */
int main()
{
    int nlines;			/* number of input lines read */

    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
	q_sort(lineptr, 0, nlines-1);
	writelines(lineptr, nlines);
	return 0;
    } else {
	printf("error: input too big to sort\n");
	return 1;
    }
}

#define MAXLEN 1000		/* max length of any input line */
int get_line(char *, int);
char *alloc(int);

/* readlines(): read input line */
int readlines(char *lineptr[], int maxlines)
{
    int len, nlines;
    char *p, line[MAXLEN];

    nlines = 0;
    while ((len = get_line(line, MAXLEN)) > 0)
	if (nlines >= maxlines || (p = alloc(len)) == NULL)
	    return -1;
	else {
	    line[len-1] = '\0';	/* delete newline */
	    strcpy(p, line);
	    lineptr[nlines++] = p;
	}
    return nlines;
}

/* writelines(): write output lines. */
void writelines(char *lineptr[], int nlines)
{
    int i;

    for (i = 0; i < nlines; i++)
	printf("%s\n", lineptr[i]);
}

/* q_sort(): sort v[left]...v[right] into increasing order  */
void q_sort(char *v[], int left, int right)
{
    int i, last;
    void swap(char *v[], int i, int j);

    if (left >= right) /* do nothing if array contains fewer than two elements */	
	return;
    swap(v, left, (left+right)/2);
    last = left;
    for (i = left+1; i <= right; i++)
	if (strcmp(v[i], v[left]) < 0)
	    swap(v, ++last, i);
    swap(v, left, last);
    q_sort(v, left, last-1);
    q_sort(v, last+1, right);
}

/* swap(): interchange v[i] and v[j] */
void swap(char *v[], int i, int j)
{
    char *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
    
#define ALLOCSIZE 10000		 /* size of avaliable space */
static char allocbuf[ALLOCSIZE]; /* storage for alloc */
static char *allocp = allocbuf;	 /* next free position */

/* alloc(): return pointer to n characters. */
char *alloc(int n)
{
    if (allocbuf + ALLOCSIZE - allocp >= n) { /* it fits */
	allocp += n;
	return allocp - n;	/* old p */
    } else
	return 0;
}
