/* Exercise 1-21. Write a program entab that replaces strings of
   blanks by the minimum number of tabs and blanks to achieve the same
   spacing. Use the same tab stops as for detab. When either a tab or
   a single blank would suffice to read a tab stop, which should be
   given preference? */

#include <stdio.h>

#define MAX 512		      /* max line width */
#define TABW 8		      /* horizontal tab spacing from origin */

/* tabstop: Returns the index of the subsequent tabstop from i

   Example: at initial index i=9 and tabspace s=8, the returned index
   should be after that of the next tabstop 16.

   [1] integer division of i/s gives us the previous tab number
   i/s = 9/8 = 1
   [2] the subsequent tab number is desired, so one is added.
   1 + 1 = 2
   [3] multiply by the tabspacing to determine the tabstop position
   2 x 8 = 16 */
int tabstop(int i, int s)
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

/* ismt: returns true if the buffer 'str' contains only whitespace
   between 'start' and 'stop'. The element at start is tested, stop is
   not tested. */
int ismt(char str[], int start, int stop)
{
    int mt = 1;

    do {
	if (str[start] != ' ')
	    mt = 0;
    } while (mt && ++start < stop);

    return mt;
}

/* entab: replaces sequences of blank spaces in 'in' with the minimum
   number of tabs and blank spaces and stores result in 'out'. Single
   blank spaces remain unchanged to preserve strings of text. */
void entab(char in[], int len, int tabw, char out[], int lim)
{
    int i, o; 			/* buffer cursors; input, output */
    int stp;			/* next tabstop; index of */

    for (i=o=0; i<len; o++) {
	stp=tabstop(i, TABW);
	if (stp<len && (stp-i)>1 && ismt(in,i,stp)) {
	    out[o] = '\t';
	    i = stp;
	} else {
	    out[o] = in[i++];
	}
    }

    out[o] = '\0';
    return;
}

int main()
{
    char tab[MAX], notab[MAX];	/* string buffers */
    int len;			/* length of string in buffer */

    while ((len = readaline(notab, MAX))) {
	printaline(notab);
	entab(notab, len, TABW, tab, MAX);
	printaline(tab);
    }

    return 0;
}
