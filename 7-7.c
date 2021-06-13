/* Excercise 7-7: Modify the pattern finding program from Chapter 5 to
   take its input from a set of named files or, if no files are named
   as arguments, from the standard input. Should the file name be
   printed when a matching line is found? */

#include <stdio.h>
#include <string.h>

/* line: character buffer read from file   */
#define MAXLINE 80
char line[MAXLINE];
long lineno;

size_t fget_line(FILE *fh, char *s, size_t lim);
size_t get_line(char *line, size_t lim);

/* 7-7: print lines that match the pattern in first arguement */
int main(int argc, char **argv)
{
    int c, except = 0, number = 0, found = 0, done = 0;
    FILE *fh = stdin;
    
    /* this is not a good way to process arguements */
    while (--argc > 0 && **++argv == '-')
	while ((c = *++*argv) && !done)
	    switch (c) {
	    case 'x':
		except = 1;
		break;
	    case 'n':
		number = 1;
		break;
	    case 'f':
		if (*++*argv == '=') {
		    fh = fopen(++*argv, "r"); /* should do error checking */
		    done = 1;
		}
		break;
	    default:
		printf("find: illegal option %c\n", c);
		argc = 0;
		found = -1;
		break;
	    }

    if (argc != 1)
	puts("Usage: find -x -n pattern");
    else
	while (fget_line(fh, line, MAXLINE)) {
	    lineno++;
	    if ((strstr(line, *argv) != NULL) != except) {
		if (number)
		    printf("%ld:", lineno);
		printf("%s", line);
		found++;
	    }
	}
    return found;
}
