/* knr2.c: useful functions from k&r2 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <float.h>


#include "knr2.h"

/*
   I/O and FORMATTED PRINTING
*/

/* get_line(): populates s with stdin until a new line, end of file or n
   characters are read.

   String is terminated with null byte at maximum n+1 bytes from first
   character.

   Returns the number of characters written to s or EOF */
size_t get_line(char *s, size_t n)
{
    int c;
    char *cur = s;

    while (cur<s+n && (c=getchar())!=EOF && c!='\n')
	*cur++ = c;
    if (c == '\n')
	*cur++ = c;
    *cur = '\0';

    return cur - s;
}

/* fget_line(): reads max lim chars from fh into s */
size_t fget_line(FILE *fh, char *s, size_t lim)
{
    size_t n;
    int c;

    for (n = 0; n < lim && (c = fgetc(fh)) != EOF && c != '\n'; ++n)
	s[n] = c;
    if (c == '\n')
	s[n++] = '\n'; 
    s[n] = '\0';
    return n;
}

/* get_str(): functions the same as get_line() except any newline
   characters read are not stored in s. */
size_t get_str(char *s, size_t n)
{
    int c;
    char *cur = s;

    while (cur<s+n && (c=getchar())!=EOF && c!='\n')
	*cur++ = c;
    *cur = '\0';

    return cur - s;
}

/* get_int(): read an integer from stdin. */
int get_int(void)
{
    int c, sign, n;

    while (isblank(c = getchar()))
	;
    sign = (c == '-') ? -1 : 1;
    if (c == '-')
	c = getchar();
    for (n = 0; isdigit(c); c = getchar()) 
	n = 10*n + c - '0';
    return n * sign;
}

/* get_unit(): read an unsigned integer from stdin */
unsigned get_uint(unsigned base)
{
    unsigned n;
    int c;
    
    while (isblank(c = getchar()))
	;
    for (n = 0; isxdigit(c); c = getchar()) 
	n = base*n + tohexval(c);
    return n;
}

/* get_flt(): read a double precision floating point value from
   stdin */
double get_flt(void)
{
    double coef, prec, exp;
    int c, csign, esign;

    while (isblank(c = getchar()))
	;
    csign = (c == '-') ? -1 : 1;
    if (c == '-' || c == '+') 
	c = getchar();    
    for (coef = 0.0; isdigit(c); c = getchar())
	coef = coef * 10.0 + c - '0';
    if (c == '.') {
	c = getchar();
	for (prec = 1.0; isdigit(c); c = getchar(), prec /= 10)
	    coef = coef * 10.0 + c - '0';
    }
    if (c == 'E' || 'e') {
	esign = ((c = getchar()) == '-') ? -1 : 1;
	if (c == '-' || c == '+') 
	    c = getchar();
	for (exp = 0.0; isdigit(c); c = getchar())
	    exp = exp * 10.0 + c - '0';
    }
    return (coef * csign) * prec * powf(10.0, exp * esign);
}

/* put_str(): writes s to stdout */
void put_str(const char *s)
{
    while (*s)
	putchar(*s++);
}

/* minprintf(): minimal printf supporting following conversion
   specifiers: %[d,f,u,x,t,s] */
void minprintf(char *fmt, ...)
{
    static char buf[1024];
    va_list ap;
    char *p, *s;
    int d;
    unsigned u;
    double f;

    va_start(ap, fmt); 		/* ap points to 1st unnamed arg  */
    for (p = fmt; *p; p++) {
	if (*p != '%') {
	    putchar(*p);
	    continue;
	}
	switch (*++p) {
	case 'd':
	    d = va_arg(ap, int);
	    int_to_str(d, buf, 10, 1);
	    put_str(buf);
	    break;
	case 'f':
	    f = va_arg(ap, double);
	    put_str(flt_to_str(f, buf));
	    break;
	case 'u':
	    u = va_arg(ap, unsigned);
	    uint_to_str(u, buf, 10, 1);
	    put_str(buf);
	    break;
	case 'x':
	    u = va_arg(ap, unsigned);
	    uint_to_str(u, buf, 16, 1);
	    put_str(buf);
	    break;
	case 't':		/* binary */
	    u = va_arg(ap, unsigned);
	    uint_to_str(u, buf, 2, 1);
	    put_str(buf);
	    break;
	case 's':
	    for (s = va_arg(ap, char *); *s; s++)
		putchar(*s);
	    break;
	default:
	    putchar(*p);
	    break;
	}
    }
    va_end(ap);
}

/* minscanf(): minimal scanf supporting following conversion
   specifiers: %[d,f,u,x,t,s] */
void minscanf(char *fmt, ...)
{
    va_list ap;			
    char *p;			/* fmt string cursor */

    va_start(ap, fmt);
    for (p = fmt; *p; p++) {
	if (*p == '%')
	    switch (*++p) {
	    case 'd': {
		int *d = va_arg(ap, int *);
		*d = get_int();
		break;
	    }
	    case 'f': {
		double *f = va_arg(ap, double *);
		*f = get_flt();
		break;
	    }
	    case 'u': {
		unsigned *u = va_arg(ap, unsigned *);
		*u = get_uint(10);
		break;
	    }
	    case 'x': {
		unsigned *x = va_arg(ap, unsigned *);
		*x = get_uint(16);
		break;
	    }
	    case 't': {
		unsigned *t = va_arg(ap, unsigned *);
		*t = get_uint(2);
		break;
	    }
	    case 's': {
		int c;
		char *s = va_arg(ap, char *);
		while ((c = getchar()) != EOF && !isspace(c))
		    *s++ = c;
	    }
	    default: 
		ungetc(*p, stdin);
		break;
	    }
	else if (getchar() != *p) /* mismatch, give up */
	    break;
    }
    va_end(ap); 

    return;
}

/* Print an unsigned integer in the format:

   0xFFFFFFFF |1111 1111|1111 1111|1111 1111|1111 1111| 4294967295 label \n */
void uprintb(unsigned x, char *label)
{
    unsigned n, N;

    N = 8U*sizeof x;
    if (N == 32)
	printf("0x%08x ", x);
    if (N == 16)
	printf("0x%04x ", x);
    
    for (n=0; n<N; ++n) {
	if (n%8 == 0)
	    putchar('|');
	else if (n%4 == 0)
	    putchar(' ');
	putchar(x &(1 << (N-n-1)) ? '1' : '0');
    }

    printf("| %.10u %s\n", x, label); 
}

/* ERROR REPORTING  */

/* die(): printf format error message to stderr at exit with status */
void die(int status, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "Error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(status);
}

/* warn(): printf format warning message to stderr  */
void warn(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "Warning: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}


/*
   STRING OPERATIONS
*/

/* str_len(): returns the number of characters in the string s. */
size_t str_len(const char *s)
{
    const char *t = s;

    while (*t)
	t++;
    return t - s;
}

/* str_reverse(): reverses the string in s and returns its length. */
void str_reverse(char *s)
{
    char tmp, *t = s + str_len(s);

    while (--t > s) {
	tmp = *t;
	*t  = *s;
	*s++  = tmp;
    }
}

/* str_cat(): Concatenate t to s; s must be big enough */
size_t str_cat(char *s, const char *t)
{
    char *r = s;

    while (*s)
	s++;
    while (*t)
	*s++ = *t++;
    return r - s;
}

/* str_cmp(): compares string s to t.

   Returns zero when strings are equal or the difference between the
   characters at the position where they first disagree. */
int str_cmp(const char *s, const char *t)
{
    for ( ; *s == *t; ++s, ++t)
	if (*s == '\0')
	    return 0;
    return *s - *t;
}


/* str_ridx(s,t): returns start index of the rightmost occurrence
   of t in s, or -1 if there is none. */
int str_ridx(char *s, const char *t)
{
    int i, j, k;

    for (i=str_len(s)-1; i>=str_len(t)-1; --i) {
	for (k=0, j=str_len(t)-1; t[j]==s[i-k] && j>=0; --j, ++k)
	    //printf("matched '%c' at i=%d, j=%d\n", t[j], i, j);   
	    ;
	if (j<0)		/* match */
	    return i-str_len(t)+1;
    }

    return -1;
}

/* str_end(): returns 1 if t is at the end of s, otherwise 0.  */
int str_end(const char *s, const char *t)
{   
    const char *a;		/* anticipated start of t in s */

    a = s + str_len(s) - str_len(t);
    while (*a!='\0' && *t!='\0' && (*a++ == *t++)) 
	;
    return !(a - (s+str_len(s))); 
}

/* strn_cpy(): copy at most n-1 characters of t into c.

   In the case of t being longer than n, the copy is truncated and the
   nth character replaced with a null character to terminate the
   string. */
char *strn_cpy(char *s, const char *t, size_t n)
{
    while (n-- && (*s++ = *t++))
	;
    *--s = '\0';

    return s;
}

/* strncat_(): concatenate at most n-1 characters of t onto s. 

   In the case of t being longer than n, the copy is truncated and the
   nth character replaced with a null character to terminate the
   string. */
char *strn_cat(char *s, const char *t, size_t n)
{
    while (*s)
	s++; 		/* move to end */
    strn_cpy(s, t, n);

    return s;
}

/* strn_cmp(): compare at most n characters of string t to s.

   Returns zero when strings are equal or the difference between the
   characters at the position where they first disagree. */
int strn_cmp(const char *s, const char *t, size_t n)
{
    for ( ; n-- && *s == *t; ++s, ++t)
	if (*s == '\0')
	    return 0;
    return *s - *t;
}

/* strn_clear(): sets n+1 bytes in s to null. */
void
strn_clear(char *s, int n)
{
    do {
	*(s + n) = '\0';
    } while (--n);
}

/* detab(): replaces tabs in the string s with spaces in t.

   Replaces tabs with the equivalent number of spaces required to
   acheive the same indentation for a given tab width w.

   The string stored in t will be trucated if lim characters is
   exceeded. The length of t is returned. */
size_t detab(const char *s, char *t, unsigned w, size_t lim)
{
    unsigned i, j;

    for (i=j=0; s[i]!='\0' && j<lim; t[j++] = s[i++]) {
	if (s[i] == '\t') {
	    do {
		t[j++] = ' ';
	    } while (j%w);
	    i++;
	}
    }
    t[j] = '\0';

    return j;
}

/* entab(): replaces consecutive spaces in the string s with tabs in t.

   Replaces spaces with the equivalent number of spaces required to
   acheive the same indentation for a tab width w.

   The length of t is returned, and will always be less than or equal
   to the length of s. */
size_t entab(const char *s, char *t, unsigned w)
{
    int i, j, n;

    for (i=j=0; s[i]!='\0'; t[j++] = s[i++]) {
	n = 0;
	while (s[i+n]==' ' && (i+n)%w)
	    ++n;
	if (n > 1 && !((i+n)%w)) {
	    t[j++] = '\t';
	    i += n;
	}
    }
    t[j] = '\0';

    return j;
}

/*
  STRING TO VALUE CONVERSIONS
*/

/* str_to_int(): converts first decimal string in s into a signed
   integer */
int str_to_int(const char *s)
{
    int n = 0;
    const char *t;

    while (*s!='\0' && !isdigit(*s))
	t = ++s;		/* move to 1st digit */
    while (isdigit(*t))
	n = n*10 + (*t++ - '0');

    return n * (*s-1=='-' ? -1 : 1);
}

/* str_to_flt(): convert s to double precision floating point value.

   The only required field is the integer componant of the
   coefficient. If an exponent is present it must be delimited with
   'E' or 'e'.

   Format of s: [+|-][0-9][.][0-9][e|E][+|-][0-9] 

   Empty strings return 0, invalid strings produced undefined
   results. */
double str_to_flt(const char *s)
{
    double coef, prec, exp;
    const char *t;		/* digit cursor for loops */

    while (*s!='\0' && !isdigit(*s))
	++s;
    for (t=s, coef=0.0; isdigit(*t); ++t) 
	coef = coef * 10.0 + (*t - '0'); /* integer part */
    if (*t == '.')		
	++t;
    for (prec=1.0; isdigit(*t); ++t, prec /= 10.0)
	coef = coef * 10.0 + (*t - '0'); /* fractional part */
    if (t > s && *(s-1) == '-')
	coef *= -1;
    if (*t == 'E' || *t == 'e')
	s = t++;
    if (issign(*t) && (*s == 'e' || *s == 'E'))
	++t;
    for (exp=0.0; (*s == 'e' || *s == 'E') && isdigit(*t); ++t)
	exp = exp * 10.0 + (*t - '0'); /* exponent part */
    if (t > s && (*s=='e' || *s=='E') && *(s+1) == '-')
	exp *= -1;		

    return coef * prec * powf(10.0, exp);
}

/* int_to_str(): store the base 10 string representation of n in s
   with a minimum field width of w. If sign is non zero, positive
   integers are also signed. Returns length of new string s. */
size_t int_to_str(int n, char *s, int sign, int w)
{
    unsigned m;
    char *t = s;

    m = (n < 0) ? -n : n; 
    do {			/* generate digits in reverse order */
	*t++ = n % 10;
	m /= 10;  
    } while (--w > 0 || m > 0);

    if (n < 0)
	*t++ = '-';
    else if (n >= 0 && sign)
	*t++ = '+';
    *t = '\0';

    str_reverse(s);

    return t - s;
}

/* uint_to_str(): store the base 2<=b<=16 string representation of
   unsigned integer n in s with a minimum field width of w. Returns
   length of new string s. */
size_t uint_to_str(unsigned n, char *s, unsigned b, int w)
{
    static const char c[] = "0123456789ABCDEF";
    char *t = s;

    do {			/* generate digits in reverse order */
	*t++ = c[n%b];
	n /= b;
    } while (--w > 0 || n > 0);

    *t = '\0';
    str_reverse(s);

    return t - s;
}

/* flt_to_str(): Converts float to string in the format
   +0.00e+00. Returns s which must be a buffer of at least 10B. */
char *flt_to_str(double f, char *s)
{
    int exp;
    unsigned coef, prec;
    char *t;

    for (exp = 0; f != 0 && f < 1; --exp)
	f *= 10;
    for (; f != 0 && f >= 10; ++exp)
	f /= 10;

    t = s;  
    coef = (unsigned)f;
    prec = (unsigned)((f - coef + 0.005) * 100); 
    *t++ = f < 0 ? (f *= -1, '-') : '+';
    t += uint_to_str(coef, t, 10, 1);
    *t++ = '.';
    t += uint_to_str(prec, t, 10, 2);
    *t++ = 'e';
    t += int_to_str(exp, t, 1, 2);
    *t = '\0';

    return s;
}

/*
   MATHEMATICAL FUNCTIONS
*/

/* u_power() returns the value of base to the power of exp. If the
   base is not a positive integer the result is undefined.

   Formula:   Value = base^exp

   Where:
       base > 0
       exp >= 0         */
unsigned u_power(unsigned base, unsigned exp)
{
    unsigned val;

    if (exp == 0)
	return 1;

    for (val=base; exp-1; --exp)
	val *= base;

    return val;
}

/* tohexval(): returns integer value of c from base 2 to 16 */
unsigned tohexval(char c)
{
    if (c >= 'a' && c <= 'f')
	return c + 10 - 'a';
    else if (c >= 'A' && c <= 'F')
	return c + 10 - 'A' ;
    else
	return c - '0';
}

/*
  BITWISE OPERATIONS
*/

/* setbits(): returns x with the n bits that begin at position p set
   to the rightmost n bits of y, leaving the other bits unchanged. */
unsigned setbits(unsigned x, unsigned p, unsigned n, unsigned y)
{
    unsigned xmask, ymask;

    ymask = ~(~0 << n);		/* n lsb set 1  */
    xmask = ~(ymask << p);	/* n through n+p set 1 */

    return ((y & ymask) << p) | (x & xmask);
}

/* invert(): that returns x with the n lsb that begin at p inverted */
unsigned invert(unsigned x, unsigned p, unsigned n)
{
    unsigned mask; 

    mask = ~(~0 << n) << p;	/* n through n+p set 1 */

    return (~x & mask) | (x & ~mask);
}

/* rightrot(x,n): that returns the value of the integer x rotated by n
   bit positions. */
unsigned rightrot(unsigned x, unsigned n)
{
    unsigned mask, nlsb;

    mask = ~(~0 << n); 		/* n lsb set 1 */
    nlsb = (x & mask) << ((8U*sizeof x)-n);

    return (x >> n) | nlsb;
}

/* del_ls1(): unsets right most set bit in x */
unsigned del_ls1(unsigned x)
{
    return x & (x-1);
}

/* 
   DATE AND TIME
*/

/* day */
static const char daytab[2][13] = {
    { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

/* month_name(): return name of n-th month */
const char *month_name(int n)
{
    static const char *name[] = {
	"Illegal Month",
	"January", "February", "March",
	"April", "May", "June",
	"July", "August", "September",
	"October", "November", "December"
    };

    return (n < 1 || n > 12) ? name[0] : name[n];
}

/* day_of_the_year(): set day of year from month and day */
unsigned day_of_year(unsigned year, unsigned month, unsigned day)
{
    unsigned i, leap;

    leap = (year%4==0 && year%100) || year%400==0;

    for (i = 1; i < month; ++i)
	day += daytab[leap][i];

    return day > 365 ? 0 : day ;
}

/* month_day(): set month, day from day of year */
void month_day(unsigned year, unsigned yearday, unsigned *pmonth, unsigned *pday)
{
    unsigned i, leap;

    leap = (year%4==0 && year%100!=0) || year%400==0;
    for (i=1; yearday > daytab[leap][i]; ++i)
	yearday -= daytab[leap][i];
    *pmonth = i;
    *pday = yearday;
}

/* ordinate(): returns the appropriate ordinate suffix for n.   */
const char *ordinate(unsigned n)
{
    char s[12];			/* longest string is -2147483648 */
    size_t len;

    static const char *ordinal[10] = {
	"th", "st", "nd", "rd", "th", "th" , "th" , "th" , "th"
    };

    len = uint_to_str(n, s, 10, 0);

    return ordinal[s[len-1]-'0'];
}

/*
   SIMPLE CHARACTER STACK
 */

#define BUFSIZE 100

char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0; /* next free position in buf */

/* getch(): get a (possibly pushed back) character */
int getch(void) 
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

/* ungetch(): push character back on input */
void ungetch(int c) 
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

/*
  BINARY SEARCH TREE
*/

#define REFMAX 1024
struct tnode {			/* a tree node */
    char *word;
    int count;			/* # occurences of word  */
    unsigned lines[REFMAX];	/* line references to word */
    struct tnode *left;
    struct tnode *right;
};

/* addtree():  add a node with w and line, at or below p */
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

/* treeprint(): in-order print of tree p */
void treeprint(struct tnode *p)
{
    int i;

    if (p != NULL) {
	treeprint(p->left);
	printf("%4d %s: ", p->count, p->word);
	for (i = 0; i < p->count; ++i)
	    printf("%d ", p->lines[i]);
	putchar('\n');
	treeprint(p->right);
    }
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
