/* Excercise 5-14. Modify the program to work with an '-r' flag that
   reverses the sorting order */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 5000		/* max #lines to be sorted */
char *lineptr[MAXLINES];	/* pointers to text lines */

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);

void q_sort(void *lineptr[], int left, int right, int (*comp)(void *, void *));
void rq_sort(void *lineptr[], int left, int right, int (*comp)(void *, void *));
int numcmp(char *, char *);
int rnumcmp(char *, char *);
int rstrcmp(char *, char *);

/* sort input lines */
int main(int argc, char *argv[])
{
    int nlines;			/* number of input lines read */
    int numeric = 0;		/* 1 if numeric sort */
    int reverse = 0;

    while (*++argv)
	if (!strcmp(*argv, "-n"))
	    numeric = 1;
	else if (!strcmp(*argv, "-r"))
	    reverse = 1;
	    
    fprintf(stderr, "MODE: numeric=%d, reverse=%d\n", numeric, reverse);

    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {

	if (reverse)
	    q_sort((void **) lineptr, 0, nlines-1, 
		   (int (*)(void*,void*))(numeric ? rnumcmp : rstrcmp));
	else
	    q_sort((void **) lineptr, 0, nlines-1, 
		   (int (*)(void*,void*))(numeric ? numcmp : strcmp));

	writelines(lineptr, nlines);
	return 0;
    } else {
	fprintf(stderr, "ERROR: input too big to sort\n");
	return 1;
    }
}

/* q_sort(): sort v[left]...v[right] into increasing order */
void q_sort(void *v[], int left, int right, int (*comp)(void *, void *))
{
    int i, last;
    void swap(void *v[], int, int);

    if (left >= right)		/* do nothing if array contains */
	return;			/* fewer than two elements */

    swap(v, left, (left+right) / 2);
    last = left;
    for (i=left+1; i <= right; i++)
	if ((*comp)(v[i], v[left]) < 0)
	    swap(v, ++last, i);
    swap(v, left, last);
    q_sort(v, left, last-1, comp);
    q_sort(v, last+1, right, comp);
}

/* rq_sort(): sort v[left]...v[right] into decreasing order */
void rq_sort(void *v[], int left, int right, int (*comp)(void *, void *))
{
    int i, last;
    void swap(void *v[], int, int);

    if (left >= right)		/* do nothing if array contains */
	return;			/* fewer than two elements */

    swap(v, left, (left+right) / 2);
    last = left;
    for (i=left+1; i <= right; i++)
	if ((*comp)(v[i], v[left]) < 0)
	    swap(v, ++last, i);
    swap(v, left, last);
    q_sort(v, left, last-1, comp);
    q_sort(v, last+1, right, comp);
}

/* numcmp(): compare s1 and s2 numerically */
int numcmp(char *s1, char *s2)
{
    double v1, v2;

    v1 = atof(s1);
    v2 = atof(s2);
    if (v1 < v2)
	return -1;
    else if (v1 > v2)
	return 1;
    else
	return 0;
}

/* numcmp(): opposite of numcmp().  */
int rnumcmp(char *s1, char *s2)
{
    return -1 * numcmp(s1, s2);
}

/* rstrcmp(): opposite of strcmp(). */
int rstrcmp(char *s1, char *s2)
{
    return -1 * strcmp(s1, s2);
}
    
/* swap(): exchange two pointers */
void swap(void *v[], int i, int j)
{
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
    
#define MAXLEN 1000

size_t get_line(char *s, size_t len);

/* readlines(): read input lines */
int readlines(char *lineptr[], int maxlines)
{
    int len, nlines = 0;
    char *p, line[MAXLEN+1];

    while ((len = get_line(line, MAXLEN)) > 0)
	if (nlines >= maxlines || (p = malloc(len)) == NULL)
	    return -1;
        else {
	    line[len-1] = '\0';	/* delete newline */
	    strcpy(p, line);
	    lineptr[nlines++] = p;
	}
    return nlines;
}

void put_line(char *s);

/* writelines(): write output lines */
void writelines(char *lineptr[], int nlines)
{
    while (nlines-- > 0) {
	put_line(*lineptr++);
	putchar('\n');
    }
}
    
