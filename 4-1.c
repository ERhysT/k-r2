/* Excercise 4-1. Write the function strrindex(s,t) which returns the
   position of the rightmost occurrence of t in s, or -1 if there is
   none. */

#include <stdio.h>
#include "knr2.h"

/* strrindex(s,t): returns start index of the rightmost occurrence
   of t in s, or -1 if there is none. */
int
strrindex0(char *s, const char *t)
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

int
main(int argc, char *argv[])
{
    int i, j;
    char s[MAX+1];

    if (argc!=2) {
	fprintf(stderr, "USAGE: [needle] < haystack.txt\n");
	return 1;
    }

    for (i=1; get_line(s, MAX); ++i) {
	j=strrindex0(s, argv[1]);
	if (j!=-1)
	    printf("(%.2d:%.2d) %s\n", i, j, s);
    }

    return 0;
}
