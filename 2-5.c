/* Exercise 2-5: Write the function any(s1,s2), which returns the
   first location in the string s1 where any character from the string
   s2 occurs, or -1 if s1 contains no characters from s2. */

#include <stdio.h>

#define MAX 79

/* Returns the first location in the string s1 where any character
   from the string s2 occurs, or -1 if s1 contains no characters from
   s2. */
int
any(const char *s1, const char *s2)
{
    int i, j, idx; 

    for (i=0, idx=-1; s2[i]!='\0' && idx<0; ++i)
	for (j=0; s1[j]!='\0' && idx<0; ++j)
	    if (s1[j]==s2[i])
		idx = j;

    return idx;
}


/* Reads one line or lim characters (whichever is less) from stdin and
   writes them into a buffer at line as a null terminated
   string.

   Returns the number of characters written excluding the null
   byte. */
int
get_line(char *line, int lim)
{
    int i;
    char c;

    for (i=0; i<lim && (c=getchar())!=EOF && c!='\n'; ++i) 
	line[i] = c;

    if (c=='\n')
	line[i++] = '\n';

    line[i] = '\0';

    return i;
}

void
put_line(const char *line)
{
    int i;

    for (i=0; line[i]!='\0'; ++i)
	putchar(line[i]);
}

int
main(int argc, char *argv[])
{
    char line[MAX+1];
    int res;

    if (argc!=3) {
	puts("USAGE: ./2-5 s1 s2");
	return 1;
    }

    while (get_line(line, MAX)) {
	printf("s1:%s\n", argv[1]);
	printf("s2:%s\n", argv[2]);
	
	res = any(argv[1], argv[2]);
	if (res < 0)
	    puts("No match");
	else
	    printf("Character from s2 matches at s1[%d]", res);
    }

    return 0;
}
