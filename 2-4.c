/* Excercise 2-4: Write an alternative version of squeeze(s1,s2) that
   deletes each character in s1 that matches any character in the
   string s2. */

#include <stdio.h>

#define MAX 79

/* Removes all occurences of c in s  */
void
squeeze(char *s, int c)
{
    int i, j;

    for (i=j=0; s[i]!='\0'; ++i) 
	if (s[i]!=c)
	    s[j++] = s[i];	

    s[j] = '\0';
}

/* Removes all occurrences of each character in s2 from s1 */
void
squash(char *s1, const char *s2)
{
    int i, j, k;
    
    for (i=0; s2[i]!='\0'; ++i)	{     /* each char in s2  */
	for (j=k=0; s1[j]!='\0'; ++j) /* each char in s1 */
	    if (s1[j]!=s2[i])
		s1[k++] = s1[j];
	s1[k] = '\0';
    }
}

/* Reads one line or lim characters (whichever is less) from stdin and
   writes them into a buffer at line as a null terminated
   string.

   Reading a newline character '\n' terminates a line and is not
   written to the buffer.
   
   Returns the number of characters written excluding the null
   byte. */
int
get_line(char *line, int lim)
{
    int i;
    char c;

    for (i=0; i<lim && (c=getchar())!=EOF && c!='\n'; ++i) 
	line[i] = c;

    line[i] = '\0';

    return i;
}

int
main(int argc, char *argv[])
{
    char line[MAX+1];

    if (argc!=3) {
	puts("USAGE ./2-4 [SQUEEZE CHAR] [SQUASH STR]");
	return 1;
    }

    while (get_line(line, MAX)) {
	squeeze(line, argv[1][0]);
	puts(line);
	squash(line, argv[2]);
	puts(line);
    }
	


    return 0;
}
