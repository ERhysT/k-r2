/* Excercise 5-13. Write a program tail, which prints the last n lines
   of its input. By default, n is 10, let us sat, but it can be
   changed by an optional argument so that
   
   tail -n

   prints the last n lines. The program shold behave reationally no
   matter how unreasonable the input or the value of n. Write the
   program so it makes the best use of avaliable storage; lines should
   be sroted as in the sorting program of Section 5.6 (pointer to
   array of char), not in a two-dimensional array of fixed size.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINEMAX 511

size_t get_line(char *s, size_t n);
void put_line(const char *s);

int main(int argc, char *argv[])
{
    int i, n, len;
    char s[LINEMAX+1], **lines;

    if (argc == 1)
	n = 2;
    else if (argc == 2 && *argv[1] == '-')
	n = atoi(argv[1] + 1);
    else {
	fprintf(stderr, "USAGE: %s [nlines]\n", *argv);  
	return 1;
    }

    if (n < 1) {		/* validate n */
	fprintf(stderr, "Invalid number of lines, %d.\n", n);
	return 1;
    }

    lines = calloc(n, sizeof *lines); /* allocate list of strings */
    if (!lines) {
	fprintf(stderr, "Memory Error.\n");
	return 1;
    }

    for (i=0; (len=get_line(s, LINEMAX)); ++i) { /* populate list */
	lines[i%n] = malloc(len+1);
	if (!lines) {
	    fprintf(stderr, "Memory Error.\n");
	    return 1;
	}
	strcpy(lines[i%n], s);
    }
	    
    for (i=0; i<n; ++i) {	/* print strings in list */
	if (lines[i]) {
	    put_line(lines[i]);
	    free(lines[i]);
	}
    }

    free(lines);

    return 0;
}
