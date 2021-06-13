/* Excercise 5-6. Rewrite appropriate programs from earlier chapters
   and exercises with pointers instead of array indexing. Good
   possibilites include getline(), atoi, itoa and their varients
   (Chapters 2, 3 and 4), reverse (Chapter 3) and strindex() and
   getop() (Chapter 4) */

#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>

#define MAX 79
#define issign(c) ( c=='+' || c=='-' )		/* truthy if + or -  */

/* I/O */
int get_line(char *s, int n);
int get_str(char *s, int n);
void put_line(const char *s);

/* Strings */
size_t str_len(const char *s);
size_t str_reverse(char *s);
size_t str_cat(char *s, const char *t); /* not tested */
void strn_clear(char *s);

/* String <-> Integer */
int str_to_int(const char *s);
double str_to_flt(const char *s);
size_t int_to_str(int n, char *s, int b, int w);

int main()
{
    char s[MAX+1], t[MAX+1];
    int len, n;
    double f;
    
    while ((len = get_str(s, MAX))) {
	put_line("Read one line with get_str():\n");
	int_to_str(len, t, 10, 0);
	putchar('('); put_line(t); putchar('B'); putchar(')');
	putchar(' '); put_line(s); putchar('\n');
	
	put_line("Append to the string with str_cat().\n");
	str_cat(s, " That's hot!");
	put_line(s); putchar('\n');

	put_line("Reverse string with str_reverse():\n");
	str_reverse(s);
	put_line(s); putchar('\n');
	str_reverse(s);

	put_line("Try to read an integer from the line with str_to_int(),\n");
	n = str_to_int(s);
	put_line("and print the integer with int_to_str(): ");
	int_to_str(n, t, 10, 5);
	put_line(t); putchar('\n');

	put_line("Try to read a float from the line with str_to_flt(),\n");
	f = str_to_flt(s);
	put_line("and print the float the flt_to_str(): ");
    }

    return 0;
}
