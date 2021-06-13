/* Excercise 5-12. Modify the programs entab and detab to accept a
   list of tab stops as arguements: i.e. n tab stops every m
   columns. Use defaults if no arguements provided.

   e.g. detab -m +n

   see entab.c and detab.c 

    */

#include <stdio.h>
#include <stdlib.h>

#define LINEMAX 511

int get_line(char *s, int n);
void put_line(const char *s);

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

int main(int argc, char *argv[])
{
    char s[LINEMAX+1], t[LINEMAX+1];
    int len;
    int n=80, m=10;	  /* default to 10 tabstops ever 80 columns */
    while (--argc) {
	switch (**++argv) {
	case '-': m = atoi(*argv + 1); break;
	case '+': n = atoi(*argv + 1); break;
	default:
	    fprintf(stderr, "ERROR: invalid argument %s.\n", *argv);
	    return 1;
	}
    }

    while ((len=get_line(s, LINEMAX))) {
	put_line(s);
	detab(s, t, n/m, LINEMAX);
	put_line(t);
	entab(t, s, n/m);
	put_line(s);
    }

    putchar('\n');

    return 0;
}
