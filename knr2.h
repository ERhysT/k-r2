/* k&r.h: useful functions from k&r2 */

#ifndef KNR2_H
#define KNR2_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <stdarg.h>

#define MAX 255

#define swapt(t,x,y) { t tmp = x; x=y; y=tmp; } /* swap t x with y */
#define issign(c) ( c=='+' || c=='-' )		/* truthy if + or -  */

/*
   I/O and FORMATTED PRINTING
*/

/* get_line(): reads one line or lim characters (whichever is less)
   from stdin and writes them into a buffer at line as a null
   terminated string. Returns the number of characters written
   excluding the null byte. */
size_t get_line(char *line, size_t lim);
/* get_str(): reads one line or lim characters (whichever is less)
   from stdin and writes them into a buffer at line as a null
   terminated string. Newlines are not stored in line or counted in
   return value. Returns the number of characters written excluding
   the null byte. */
/* fget_line(): reads max lim chars from fh into s */
size_t fget_line(FILE *fh, char *s, size_t lim);
size_t get_str(char *line, size_t lim);
/* get_int(): read an integer from stdin. */
int get_int(void);
/* get_unit(): read an unsigned integer from stdin */
unsigned get_uint(unsigned base);
/* get_flt(): read a double precision floating point value from
   stdin */
double get_flt(void);
/* put_str(): writes a string to stdout */
void put_str(const char *line);
/* minprintf(): minimal printf supporting following conversion
   specifiers: %[d,f,u,x,t,s] */
void minprintf(char *fmt, ...);
/* minscanf(): minimal scanf supporting following conversion
   specifiers: %[d,f,u,x,t,s] */
void minscanf(char *fmt, ...);
/* Print an unsigned integer in the format:
   0xFFFFFFFF |1111 1111|1111 1111|1111 1111|1111 1111| 4294967295 label \n */
void uprintb(unsigned x, char *label);
/*
  ERROR MESSAGES
*/
/* die(): printf format error message to stderr at exit with status */
void die(int status, const char *fmt, ...);
/* warn(): printf format warning message to stderr  */
void warn(const char *fmt, ...);
/*
   STRING OPERATIONS
*/
/* str_len(): returns the number of characters in the string s. */
size_t str_len(const char *s);
/* str_reverse(): reverses the string in s */
void str_reverse(char *s);
/* str_cat(): Concatenate t to s; s must be big enough */
size_t str_cat(char *s, const char *t);
/* str_cmp(): compares string s to t.
   Returns zero when strings are equal or the difference between the
   characters at the position where they first disagree. */
int str_cmp(const char *s, const char *t);
/* str_ridx(s,t): returns start index of the rightmost occurrence of t
   in s, or -1 if there is none. */
int str_ridx(char *s, const char *t);
/* strend(): returns 1 if t is at the end of s, otherwise 0.  */
int str_end(const char *s, const char *t);
/* entab(): replaces consecutive spaces in the string s with tabs in
   t. Replaces spaces with the equivalent number of spaces required to
   acheive the same indentation for a tab width w. The length of t is
   returned, and will always be less than or equal to the length of
   s. */
size_t entab(const char *s, char *t, unsigned w);
/* detab(): replaces tabs in the string s with spaces in t. Replaces
   tabs with the equivalent number of spaces required to acheive the
   same indentation for a given tab width w. The string stored in t
   will be trucated if lim characters is exceeded. The length of t is
   returned. */
size_t detab(const char *s, char *t, unsigned w, size_t lim);
/*
   The following strn_xxx() functions behave similarly to the
   str_xxx() counterparts except that in the case of the string being
   longer than n, the copy is truncated and the nth character replaced
   with a null character to terminate the string.
*/
/* strn_cpy(): copy at most n-1 characters of t into c. */
char *strn_cpy(char *s, const char *t, size_t n);
/* strn_cat(): concatenate at most n-1 characters of t onto s. */
char *strn_cat(char *s, const char *t, size_t n);
/* strn_cmp(): compare at most n characters of string t to s.
   Returns zero when strings are equal or the difference between the
   characters at the position where they first disagree. */
int strn_cmp(const char *s, const char *t, size_t n);
/* strn_clear(): sets n+1 bytes in s to null. */
void strn_clear(char *s, int n);
/*
  STRING TO VALUE CONVERSIONS
*/
/* str_to_int(): converts decimal string into its signed integer value */
int str_to_int(const char *s);
/* str_to_flt(): convert a string in scientific notation s to double
   precision floating point value. String format is below, the only
   required field is the integer componant of the coefficient. If an
   exponent is present it must be delimited with 'E' or 'e'.

   [+|-][0-9][.][0-9][e|E][+|-][0-9]} 

   Empty strings return 0, invalid strings produced undefined
   results. */
double str_to_flt(const char *s);
/* int_to_str(): store the base 10 string representation of n in s
   with a minimum field width of w. If sign is non zero, positive
   integers are also signed. Returns length of new string s. */
size_t int_to_str(int n, char *s, int sign, int w);
/* uint_to_str(): store the base 2<=b<=16 string representation of
   unsigned integer n in s with a minimum field width of w. Returns
   length of s. */
size_t uint_to_str(unsigned n, char *s, unsigned b, int w);
/* flt_to_str(): Converts float to string in the format
   +0.00e+00. Returns s. */
char *flt_to_str(double f, char *s);
/*
   MATHEMATICAL FUNCTIONS
*/
/* u_power(): returns the value of base to the power of exp. If the
   base is not a positive integer the result is undefined.
   Formula:   val = base^exp; base > 0, exp >= 0   */
unsigned u_power(unsigned base, unsigned exp);
/* tohexval(): returns integer value of c from base 2 to 16 */
unsigned tohexval(char c);
/*
  BITWISE OPERATIONS
*/
/* setbits(): returns x with the n bits that begin at position p set
   to the rightmost n bits of y, leaving the other bits unchanged. */
unsigned setbits(unsigned x, unsigned p, unsigned n, unsigned y);
/* invert(): that returns x with the n lsb that begin at p inverted */
unsigned invert(unsigned x, unsigned p, unsigned n);
/* rightrot(x,n): that returns the value of the integer x rotated by n
   bit positions. */
unsigned rightrot(unsigned x, unsigned n);
/* del_ls1(): unsets right most set bit in x */
unsigned del_ls1(unsigned x);
/* 
   DATE AND TIME
*/
/* month_name(): return name of n-th month */
const char *month_name(int n);
/* day_of_the_year(): set day of year from month and day */
unsigned day_of_year(unsigned year, unsigned month, unsigned day);
/* month_day(): set month, day from day of year */
void month_day(unsigned year, unsigned yearday, unsigned *pmonth, unsigned *pday);
/* ordinate(): returns the appropriate ordinate suffix for n.   */
const char *ordinate(unsigned n);

/* SIMPLE CHARACTER STACK */
/* getch(): get a (possibly pushed back) character */
int getch(void);
/* ungetch(): push character back on input */
void ungetch(int c);

/*
  BINARY SEARCH TREE
*/
/* addtree():  add a tnode with w and line, at or below p */
struct tnode *addtree(struct tnode *p, char *w, unsigned line);
/* talloc(): make a tnode */
struct tnode *talloc(void);
/* treeprint(): in-order print of tree p */
void treeprint(struct tnode *p);
/* tprintcount(): prints tree p sorted by decreasing count */
void tprintcount(struct tnode *p);
/* tflatten(): copy pointers of each node in tree p to array treeptr,
   starting at index i. Returns number of node pointers written to
   treeptr. */
unsigned tflatten(struct tnode *p, struct tnode **treeptr, unsigned i);
/* tsize: number of nodes in tree */
unsigned tsize(struct tnode *root);
/* countcmp(): returns value greater than, less than or equal to zero
   when the count field of n1 is bigger, smaller or less than that of
   n2 respectively. */
int countcmp(const void *v1, const void *v2);
    
#endif	/* KNR2_H */

