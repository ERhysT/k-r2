/* Write a function expand(s1,s2) that expands shorthand notations
   like a-z in the string s1 into the equivalent complete list
   abc...xyz in s2. Allow for letters of either case and digits, and
   be prepared to handle cases like a-b-c and a-z0-9 and -a-z. Arrage
   that a leading or trailing - is taken literally. */

#include <stdio.h>

#define MAX 100

int
is_digit(char c)
{
    return (c>='0' && c<='9');
}

int
is_lcase(char c)
{
    return (c>='a' && c<='z');
}

int
is_ucase(char c)
{
    return (c>='A' && c<='Z');
}

/* alnum(c): Returns c cast to int when c is 0-9 or a-z or A-Z,
   otherwise returns 0. */
int
alnum(char c)
{
    return (is_digit(c) || is_lcase(c) || is_ucase(c)) ? c : 0; 
}

int
is_samecase(char c, char d)
{
    return
	(is_digit(c) && is_digit(d))  ||
	(is_lcase(c) && is_lcase(d))  ||
	(is_ucase(c) && is_ucase(d));
}

/* expand(in, out): expands hypen delimited character ranges for
   letters of either case and digits. Leading and trailing hyphens are
   taken literally and copied to out. */
int
expand(const char *in, char *out)
{
    int i, j, k;

    for (i=j=0; in[i]!='\0' && j<MAX; ) {
	out[j++] = in[i++];
	if (in[i]=='-' && is_samecase(in[i-1], in[i+1])) {
	    for (k=1; (in[i-1]+k)!=in[i+1]; ++k)
		out[j++] = in[i-1]+k;
	    ++i;
	}
    }

    return j;
}

/* contract(in, out): opposite of expand() */
int
contract(const char *in, char *out)
{
    int i, j, k;

    for (i=j=0; in[i]!='\0' && j<MAX; ) {
	out[j++] = in[i++];
	for (k=0; in[i+k-1]==(in[i+k]-1); ++k)
	    ;
	if (k>1) {
	    out[j++] = '-';
	    i += k-1;
	}
    }

    out[j] = '\0';

    return j;
}

/* str_len: returns number of characters in s.  */
unsigned
str_len(const char *s)
{
    int i;

    for (i=0; s[i]!='\0'; ++i)
	;

    return i;
}

int
main(int argc, char *argv[])
{
    char exp[MAX], cont[MAX];
    unsigned len[2];

    if (argc!=2) {
	fprintf(stderr, "USAGE %s [str]\n", argv[0]);
	return 1;
    }

    len[0] = expand(argv[1], exp);
    len[1] = contract(exp, cont);

    printf("%s:%s:%s\n", argv[1], exp, cont);
    printf("%u:%u:%u\n", str_len(argv[1]), len[0], len[1]);

    return !(str_len(argv[1]) == len[1]);
}
