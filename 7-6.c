/* Exercise 7-6. Write a program to compare two files, printing the
   first line where they differ. */

#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

/* src: a file handle and line buffer for each file to be compared */
#define MAXLINE 80
#define readsrc(src) fgets(src.s, MAXLINE, src.fh)
struct {
    FILE *fh;
    char s[MAXLINE];
} src[2];

/* 7-6: diff two files line by line */
int main(int argc, char **argv)
{
    int i, n; 			/* each file, each line */

    if (argc != 3) {
	fprintf(stderr, "USAGE: %s [file1] [file2]\n", *argv);
	return 1;
    }

    for (i = 0; *++argv; ++i) { 
	if ((src[i].fh = fopen(*argv, "r")) == NULL) {
	    perror("Failed to open file");
	    exit(1);
	} 
	printf("%c %s\n", i == 0 ? '<' : '>', *argv);
    }
    
    for (n = 0; readsrc(src[0]) && readsrc(src[1]); ++n) 
	if (strcmp(src[0].s, src[1].s)) 
	    printf("@ n=%d\n< %s> %s", n, src[0].s, src[1].s);
    
    for (i = 0; i < (sizeof src) / (sizeof *src); ++i) {
	if (!feof(src[i].fh)) {	
	    printf("@ n=%d\n", n);
	    while (readsrc(src[i])) 
		printf("%c %s", i == 0 ? '<' : '>', src[i].s);
	}
	if (fclose(src[i].fh))
	    perror("Failed to close file");
    }
	
    return 0;
}
    
	

