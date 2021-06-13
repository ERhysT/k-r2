/* Write a program 'detab' that replaces tabs in the input with the
   proper number of blanks to space to the next tab stop. Assume a
   fixed set of tab stops, say every n columns. Should n be a variable
   or a symbolic parameter? */

#include <stdio.h>

#define MAX 512		      /* max line width */
#define TABW 8		      /* horizontal tab spacing from origin */
#define BLANK ' '	      /* tab replacement character */

/* nexttab: Returns the index of the subsequent tabstop from i

   Example: at initial index i=9 and tabspace s=8, the returned index
   should be after that of the next tabstop 17.

   [1] integer division of i/s gives us the previous tab number
           i/s = 9/8 = 1
   [2] the subsequent tab number is desired, so one is added.
           1 + 1 = 2
   [3] multiply by the tabspacing to determine the tabstop position
           2 x 8 = 16 */
int nexttab(int i, int s)
{
    return ((i/s)+1)*s;
}

/* readaline: reads one line from stdin and records in buf as a null
   terminated string; returns the number of bytes read from
   stdin. Number of bytes read will not exceed lim. */
int readaline(char buf[], int lim)
{
    int i, c;

    for(i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
	buf[i] = c;

    if (c == '\n')
	buf[i++] = c;

    buf[i] = '\0';	/* null char not included in string length */

    return i;
}

/* printaline: prints the buffer to stdout */
void printaline(char *buf)
{
    printf("%s", buf);
    return;
}

/* detab: replace tab characters in 'in' with equivalent number of
   blank spaces to the next tab stop and stores resultant string in
   'out'. */
void detab(char in[], int len, int tabw, char out[], int lim)
{
    int i, j, n;
    char c;

    for (i=j=0; i<len && j<lim-1 && (c=in[i])!='\0'; ++i)
	if (c == '\t')
	    for (n=nexttab(j,tabw); j<n; ++j)
		out[j] = BLANK;
	else
	    out[j++] = c;

    out[j] = '\0';

    return;
}

int main()
{
    char tab[MAX], notab[MAX];	/* string buffers */
    int len;			/* length of string in buffer */

    while ((len = readaline(tab, MAX))) {
	//printaline(tab);
	detab(tab, len, TABW, notab, MAX);
	printaline(notab);
    }

    return 0;
}
