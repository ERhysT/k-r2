/* Exercise: Write a loop equivalent to the foor loop below (original)
   without using && or ||.  */


/* Writes one line of characters or one less than lim (whichever is
   less) to the buffer pointed to by line as a null terminated
   string. Returns the number of characters read.*/

#include <stdio.h>

#define MAX 80

int 
getline_original(char *s, int lim)
{
    char c;
    int i;

    for (i=0; i<lim-1 && (c=getchar())!='\n' && c!=EOF; ++i)
	s[i] = c;
    if (c=='\n')
	s[i++] = '\n';

    s[i] = '\0'; 

    return i;
}

/* As above without && or || operators */
int 
getline_new(char *s, int lim)
{
    char c;
    int i;

    for (i=0; i<lim-1; ++i) {
	if ((c=getchar())=='\n')
	    break;
	if (c==EOF)
	    break;
	s[i] = c;
    }
    if (c=='\n')
	s[i++] = '\n';

    s[i] = '\0'; 

    return i;
}


int
main(int argc, char *argv[])
{
    char lbuf[MAX];

    while (getline_new(lbuf, MAX))
	puts(lbuf);

    while (getline_original(lbuf, MAX))
	puts(lbuf);

    return 0;
}

    
