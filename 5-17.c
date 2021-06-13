/* Excercise 5-15. Add a field hadling ability to sort within feilds. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLEN       1000		/* max #characters in a line */
#define MAXLINES     5000		/* max #lines to be sorted */
#define MAXFIELDS    100		/* max #fields in a line */
#define MAXELEMENTS  100		/* max #elements in a field */

char *lineptr[MAXLINES];	/* pointers to text lines */
char *fieldptr[MAXFIELDS];	/* pointers to fields in lines */
char *elementptr[MAXELEMENTS];	/* pointers to elements in fields */

#define isalsp(X) (isblank(X) || isalnum(X)) 

#define FLAG_NUMERIC   ( 1 << 0 ) /* 0x01 */
#define FLAG_REVERSE   ( 1 << 1 ) /* 0x02 */
#define FLAG_FOLD      ( 1 << 2 ) /* 0x04 */
#define FLAG_DIRECTORY ( 1 << 3 ) /* 0x08 */

int readlines(char *lineptr[], int nlines);
void put_line(char *s);
    
int delimit(char *s, char *d, char *fieldptr[], int max);
int getflags(char *s);
int (*getcmpfn(unsigned char flags))(void*, void*);

void quicksort(void *lineptr[], int left, int right,
	    int (*cmpfn)(void *, void *));

/* sort input lines and fields according to flags. For example:

   Zeynep öz	12 312 9
   Guneş Ozgan	123 10
   Rhys Thomas	50 49 20

   lines will be sorted according to flags then all subsequent fields
   (only 1 here) after the tab will have their space delimited
   elements (the numbers) sorted.... 

   couldnt be arsed to finished flag reading for each field's element
   sorting (assumes numerical) but basicaly works when fields are tab
   delimited and elements delimited with a single space. also missing
   some free() of malloc'd field and line ptr mem..... todo later
   maybe...
 */
int main(int argc, char *argv[])
{
    int nlines, nfields, nelements, flags, (*cmpfn)(void*, void*);
    char *fielddelim = "\t";
    char *elementdelim = " ";

    nlines = readlines(lineptr, MAXLINES);
    if (nlines < 0) {
	fprintf(stderr, "ERROR: input too big to sort\n");
	return 1;
    }

    flags = (argc > 1) ? getflags(argv[1]) : 0;
    cmpfn = getcmpfn(flags);
    if (cmpfn == NULL) {
	fprintf(stderr, "Invalid comparison mode.\n");
	return 1;
    }
    quicksort((void **) lineptr, 0, nlines-1, cmpfn); /* Sort lines */

    for (int i = 0; i < nlines; ++i) {
	nfields = delimit(lineptr[i], fielddelim, fieldptr, MAXFIELDS);
	put_line(fieldptr[0]);
	put_line(fielddelim);
	for (int j = 1;  j < nfields; ++j) {
	    nelements = delimit(fieldptr[j++], elementdelim, elementptr, MAXELEMENTS);
	    quicksort((void **)elementptr, 0, nelements-1, getcmpfn(1));
	    for (int k = 0; k < nelements; ++k) {
		put_line(elementptr[k]);
		put_line(elementdelim);
		free(elementptr[k]);
	    }
	}
	putchar('\n');
    }

    return 0;
}

/* dumpfields(): prints fields to stderr. */
void dumpfields(char *fieldptr[], int nfields)
{
    for (int j = 0; j < nfields; ++j) {
	fprintf(stderr, "{%s}", fieldptr[j]);
    }
    putchar('\n');
}

/* delimit(): Determines 'd' delimited substrings in s and dynamically
   stores up to 'max' strings in fieldptr. Returns the number of
   strings written to fieldptr or -1 on malloc error. */
int delimit(char *s, char *d, char *results[], int max)
{
    int i, n = 0;
    char *substr, *t = s; 

    while (*t++ != '\0') {
	for (i = 0; i<strlen(d) && *(t+i)==*(d+i); ++i)
	    ;
	if (i > 0 && i == strlen(d)) { /* matched delimiter */
	    if (!(substr = results[n++] = malloc(t-s+1)))
		return -1;
	    while (s < t)
		*substr++ = *s++;
	    *substr = '\0';
	    t = s += i;
	}
    }
    /* Get last field */
    if (!(substr = results[n++] = malloc(t-s+1)))
	return -1;
    while (s < t)
	*substr++ = *s++;
    *substr = '\0';

    return n;
}

/* quicksort(): sort v[left]...v[right] into increasing order */
void quicksort(void *v[], int left, int right,
	       int (*cmpfn)(void *, void *))
{
    int i, last;
    void swap(void *v[], int, int);

    if (left >= right)		/* do nothing if array contains */
	return;			/* fewer than two elements */

    swap(v, left, (left+right) / 2);
    last = left;
    for (i=left+1; i <= right; i++)
	if ((*cmpfn)(v[i], v[left]) < 0)
	    swap(v, ++last, i);
    swap(v, left, last);
    quicksort(v, left, last-1, cmpfn);
    quicksort(v, last+1, right, cmpfn);
}

/* getcmpfn(): select appropriate comparison algorithm according to flags */
int (*getcmpfn(unsigned char flags))(void *, void *)
{
    int strcmp1    (char *s1, char *s2); /* 0x00 */
    int numcmp     (char *s1, char *s2); /* 0x01 */
    int rstrcmp    (char *s1, char *s2); /* 0x02 */
    int rnumcmp    (char *s1, char *s2); /* 0x03 */
    int fstrcmp    (char *s1, char *s2); /* 0x04 */
    /*         fnumcmp()   not defined                  0x05 */
    int frstrcmp   (char *s1, char *s2); /* 0x06 */
    /*         frncmp()    not defined                  0x07 */
    int dstrcmp    (char *s1, char *s2); /* 0x08 */
    /*         dnumcmp()   not defined                  0x09 */
    int drstrcmp   (char *s1, char *s2); /* 0x0A */
    /*         drnumcmp() not defined                   0x0B */
    int dfstrcmp   (char *s1, char *s2); /* 0x0C */
    /*         dfnumcmp()  not defined                  0x0D */
    int dfrstrcmp  (char *s1, char *s2); /* 0x0E */
    /*         dfrnumcmp() not defined                  0x0F */

    static int (*cmpfn[16])(char *, char *) = {
	/* 0x00      0x01       0x02      0x03	*/
	strcmp1,   numcmp,   rstrcmp,  rnumcmp,
	/* 0x04      0x05       0x06      0x07	*/
	fstrcmp,  NULL,     frstrcmp, NULL,
	/* 0x08      0x09       0x0A      0x0B	*/
        dstrcmp,  NULL,     drstrcmp, NULL,
	/* 0x0C      0x0D       0x0E      0x0F	*/
	dfstrcmp, NULL,     dfrstrcmp,  NULL  
    };

    return (int (*)(void*, void*)) cmpfn[flags & 0x0F];
}

/* swap(): exchange two pointers */
void swap(void *v[], int i, int j)
{
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
    
/* getflags(): determines bitfield of flags from string s */
int getflags(char *s)
{
    int flags = 0;

    if (*s == '-')
	while (*++s) 
	    switch (*s) {
	    case 'n': flags |= FLAG_NUMERIC;   break;
	    case 'r': flags |= FLAG_REVERSE;   break;
	    case 'f': flags |= FLAG_FOLD;      break;
	    case 'd': flags |= FLAG_DIRECTORY; break;
	    default :
		fprintf(stderr, "WARNING: unrecognised flag '%c'\n", *s);
	    }
    else
	fprintf(stderr, "WARNING: unrecognised argument \"%s\"\n", s);

    return flags;
}

/* readlines(): read input lines */
int readlines(char *lineptr[], int maxlines)
{
    int len, nlines = 0;
    char *p, line[MAXLEN+1];
    size_t get_line(char *s, size_t len);

    while ((len = get_line(line, MAXLEN)) > 0)
	if (nlines >= maxlines || (p = malloc(len)) == NULL)
	    return -1;
        else {
	    if (line[len-1] == '\n')
		line[len-1] = '\0';
	    strcpy(p, line);
	    lineptr[nlines++] = p;
	}
    return nlines;
}

/*
  COMPARISON FUNCTIONS
*/

/* strcmp1(): flag 0x0: compare s1, s2 in ascending order */
int strcmp1(char *s1, char *s2)
{
    for ( ; *s1 == *s2; ++s1, ++s2)
	if (*s1 == '\0')
	    return 0;
    return *s1 - *s2;
}


/* numcmp(): flag 0x01: compare s1, s2 numerically in ascending order  */
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

/* rstrcmp(): flag 0x01: compare s1,s2 in descending order */
int rstrcmp(char *s1, char *s2)
{
    return -1 * strcmp1(s1, s2);
}

/* rnumcmp(): flag 0x03: compare s1,s2 numerically in descending order */
int rnumcmp(char *s1, char *s2)
{
    return -1 * numcmp(s1, s2);
}

/* fstrcmp(): flag 0x04: compare s1,s2 case insensitively in ascending order */
int fstrcmp(char *s1, char *s2)
{
    while (tolower(*s1) == tolower(*s2)) {
	if (*s1 == '\0')
	    return 0;
	++s1, ++s2;
    }
    return *s1 - *s2;
}

/* fnumcmp() flag 0x05: not defined, cannot compare numbers with case
   insensitivity */

/* frstrcmp(): flag 0x06: compare s1,s2 case insensitively in
   decending order*/
int frstrcmp(char *s1, char *s2) 
{
    return -1 * fstrcmp(s1, s2);
}

/* frncmp(): flag 0x07: not defined, cannot compare numbers with case
   insensitivity */

/* dstrcmp(): flag 0x08: compare s1,s2 in ascending order considering
   only blanks and alphanumeric characters */
int dstrcmp(char *s1, char *s2)
{
    while (*s1 == *s2  || ((!isalsp(*s1) && !isalsp(*s2)))) {
	if (*s1 == '\0')
	    return 0;
	++s1, ++s2;
    }
    return *s1 - *s2;
}

/* dnumcmp(): flag 0x09: not defined, cannot compare numbers with
   directory flag */

/* drstrcmp(): flag 0x0A: compare s1,s2 in decending order considering
   only blanks and alphanumeric characters */
int drstrcmp(char *s1, char *s2)
{
    return -1 * dstrcmp(s1, s2);
}

/* drnumcmp(): flag 0x0B: not defined, cannot compare numbers with
   directory flag */

/* dfstrcmp(): flag 0x0C: compare s1,s2 in case insensitively
   ascending order considering only blanks and alphanumeric values. */
int dfstrcmp(char *s1, char *s2)
{
    while (tolower(*s1) == tolower(*s2)  || ((!isalsp(*s1) && !isalsp(*s2)))) {
	if (*s1 == '\0')
	    return 0; 
	++s1, ++s2;
    }
    return *s1 - *s2;
}

/* drnumcmp(): flag 0x0D: not defined, cannot compare numbers with
   directory flag */

/* dfrstrcmp(): flag 0x0E: compare s1,s2 in case insensitively
   ascending order considering only blanks and alphanumeric values. */
int dfrstrcmp(char *s1, char *s2)
{
    return -1 * dfstrcmp(s1, s2);
}

/* dfrnumcmp(): flag 0x0F: not defined, cannot compare numbers with
   directory flag */
