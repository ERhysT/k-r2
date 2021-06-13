/* Excercise 5-15. Add the option -f to fold upper and lower case
   together, so that case distinctions are not made during
   sorting. For example a and A compare equal. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINES 5000		/* max #lines to be sorted */
char *lineptr[MAXLINES];	/* pointers to text lines */

#define isalsp(X) (isblank(X) || isalnum(X)) 

#define FLAG_NUMERIC   ( 1 << 0 ) /* 0x01 */
#define FLAG_REVERSE   ( 1 << 1 ) /* 0x02 */
#define FLAG_FOLD      ( 1 << 2 ) /* 0x04 */
#define FLAG_DIRECTORY ( 1 << 3 ) /* 0x08 */

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);

void quicksort(void *lineptr[], int left, int right,
	    int (*comp)(const void *, const void *));
int (*cmp(unsigned char flags))(const void*, const void*);

/* sort input lines according to flags

   flags:
   -n: numeric sort 
   -r: descending order   
   -f: fold (case insensitive)
   -d: directory (ignore non-alphanumeric and non-blank characters)
   
   e.g. 
   ./5-15 -nr        (sorts numerically in descending order)    */
int main(int argc, char *argv[])
{
    int nlines;			/* number of input lines read */
    int (*f)(const void*, const void*);	/* comparison function */
    unsigned char flags = 0;

    while (*++argv) 
	if (**argv == '-')
	    while (*++*argv) 
		switch (**argv) {
		case 'n': flags |= FLAG_NUMERIC;   break;
		case 'r': flags |= FLAG_REVERSE;   break;
		case 'f': flags |= FLAG_FOLD;      break;
		case 'd': flags |= FLAG_DIRECTORY; break;
		default :
		    fprintf(stderr, "WARNING: unrecognised flag '%c'\n",
			    **argv);
		}
	else
	    fprintf(stderr, "WARNING: unrecognised argument \"%s\"\n",
		    *argv);

    fprintf(stderr, "Line sorting mode %x: n(%x) r(%x) f(%x) d(%x)\n", flags,
     	    flags & FLAG_NUMERIC, flags & FLAG_REVERSE,
	    flags & FLAG_FOLD, flags & FLAG_DIRECTORY);

    f = cmp(flags);		   /* determine sorting function from flags */
    if (!f) {
	fprintf(stderr, "ERROR: Invalid combination of arguments\n");
	return 1;
    }
    nlines = readlines(lineptr, MAXLINES);
    if (nlines < 0) {
	fprintf(stderr, "ERROR: input too big to sort\n");
	return 1;
    }
    quicksort((void **) lineptr, 0, nlines-1, f);
    writelines(lineptr, nlines);

    return 0;
}

/* quicksort(): sort v[left]...v[right] into increasing order */
void quicksort(void *v[], int left, int right,
	       int (*comp)(const void *, const void *))
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
    quicksort(v, left, last-1, comp);
    quicksort(v, last+1, right, comp);
}

/* cmp(): select appropriate comparison algorithm according to flags */
int (*cmp(unsigned char flags))(const void *, const void *)
{
     /*         strcmp() declared in string.h            0x00 */
     int numcmp     (const char *s1, const char *s2); /* 0x01 */
     int rstrcmp    (const char *s1, const char *s2); /* 0x02 */
     int rnumcmp    (const char *s1, const char *s2); /* 0x03 */
     int fstrcmp    (const char *s1, const char *s2); /* 0x04 */
     /*         fnumcmp()   not defined                  0x05 */
     int frstrcmp   (const char *s1, const char *s2); /* 0x06 */
     /*         frncmp()    not defined                  0x07 */
     int dstrcmp    (const char *s1, const char *s2); /* 0x08 */
     /*         dnumcmp()   not defined                  0x09 */
     int drstrcmp   (const char *s1, const char *s2); /* 0x0A */
     /*         drnumcmp() not defined                   0x0B */
     int dfstrcmp   (const char *s1, const char *s2); /* 0x0C */
     /*         dfnumcmp()  not defined                  0x0D */
     int dfrstrcmp  (const char *s1, const char *s2); /* 0x0E */
     /*         dfrnumcmp() not defined                  0x0F */

    static const int (*fn[16])(const char *, const char *) = {
	/* 0x00      0x01       0x02      0x03	*/
	strcmp,   numcmp,   rstrcmp,  rnumcmp,
	/* 0x04      0x05       0x06      0x07	*/
	fstrcmp,  NULL,     frstrcmp, NULL,
	/* 0x08      0x09       0x0A      0x0B	*/
        dstrcmp,  NULL,     drstrcmp, NULL,
	/* 0x0C      0x0D       0x0E      0x0F	*/
	dfstrcmp, NULL,     dfrstrcmp,  NULL  
    };

    return (int (*)(const void*, const void*)) fn[flags & 0x0F];
}
    
/* numcmp(): flag 0x01: compare s1, s2 numerically in ascending order  */
int numcmp(const char *s1, const char *s2)
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
int rstrcmp(const char *s1, const char *s2)
{
    return -1 * strcmp(s1, s2);
}

/* rnumcmp(): flag 0x03: compare s1,s2 numerically in descending order */
int rnumcmp(const char *s1, const char *s2)
{
    return -1 * numcmp(s1, s2);
}
/* fstrcmp(): flag 0x04: compare s1,s2 case insensitively in ascending order */
int fstrcmp(const char *s1, const char *s2)
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
int frstrcmp(const char *s1, const char *s2) 
{
    return -1 * fstrcmp(s1, s2);
}

/* frncmp(): flag 0x07: not defined, cannot compare numbers with case
   insensitivity */

/* dstrcmp(): flag 0x08: compare s1,s2 in ascending order considering
   only blanks and alphanumeric characters */
int dstrcmp(const char *s1, const char *s2)
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
int drstrcmp(const char *s1, const char *s2)
{
    return -1 * dstrcmp(s1, s2);
}

/* drnumcmp(): flag 0x0B: not defined, cannot compare numbers with
   directory flag */

/* dfstrcmp(): flag 0x0C: compare s1,s2 in case insensitively
   ascending order considering only blanks and alphanumeric values. */
int dfstrcmp(const char *s1, const char *s2)
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
int dfrstrcmp(const char *s1, const char *s2)
{
    return -1 * dfstrcmp(s1, s2);
}

/* dfrnumcmp(): flag 0x0F: not defined, cannot compare numbers with
   directory flag */

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
    
