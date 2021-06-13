/* Exercise 7-8. Write a program to print a set of files, starting
   each new oe on a new page with a title and a running page count. */

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define MAXLINE 80
#define PAGELEN 30

size_t fget_line(FILE *fh, char *s, size_t lim);
void put_str(const char *s);
size_t str_len(const char *s);

/* 7-8: prints all files in args starting each on a new page */
int main (int argc, char **argv)
{
    FILE *fp;
    char s[MAXLINE+1];
    size_t npage, nlines, len;

    npage = 0;
    while(*++argv) {
	fp = fopen(*argv, "r");
	if (fp == NULL) {
	    perror("cannot open file");
	    exit(1);
	}
	printf("\nTitle: %s\n\n", *argv);
	nlines = 4;
	while ((len = fget_line(fp, s, MAXLINE))) {
	    put_str(s);
	    if (len == MAXLINE && s[len-1] != '\n') 
		putchar('\n');
	    ++nlines;
	    if (nlines == PAGELEN) {
		printf("\n[Page: %lu]\n\n", ++npage);
		nlines = 0;
	    }
	}
	if (s[str_len(s)-1] != '\n')
	    putchar('\n');
	if (nlines != 0)
	    printf("\n[Page: %lu]\n\n", ++npage);
    }


    return 0;
}

     
