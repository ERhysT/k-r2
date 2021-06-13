/* Excercise 3-2. Write a function escape(s,t) that converts
   characters like newline and tab into visiable escape sequences line
   \n and \t as it copies the string to s. Use a switch. Write a
   function for the other direction as well, converting escape
   sequences into the real characters. */

#include <stdio.h>

#define MAX 79

/* escape(out, in): copies in to out replacing '\n' and '\t' with
   with their character representations */
int
escape(char *out, const char *in)
{
    int i, o;			/* index counters for in and out */

    for (i=o=0; in[i]!='\0'; ++i) {
	switch (in[i]) {
	case '\n':
	    out[o++] = '\\';
	    out[o++] = 'n';
	    break;
	case '\t':
	    out[o++] = '\\';
	    out[o++] = 't';
	    break;
	default:
	    out[o++] = in[i];
	    break;
	}
    }
    out[o] = '\0';

    return o;
}

/* get_line(s, lim): Reads one line or lim-1 characters (whichever is
   less) from stdin into the buffer pointed to by s as a null
   terminated string.

   Returns the number of characters written excluding the null
   byte. */
int
get_line(char *s, int lim)
{
    int i, ch;

    for (i=0; i<lim-1 && (ch=getchar())!=EOF && ch!='\n'; ++i)
	s[i] = ch;
    if (ch=='\n')
	s[i++] = ch;
	
    s[i] = '\0';

    return i;
}

/* put_line(s): writes string s to stdout returning the number of
   characters written */
int
put_line(const char *s)
{
    int i;

    for (i=0; s[i]!='\0'; ++i)
	putchar(s[i]);

    return i-1;
}

int main(int argc, char *argv[])
{
    char s[MAX+1], t[(MAX*2)+1]; /* escape sequence, string representations */

    while (get_line(s, MAX)) {
	put_line(s);
	escape(t, s);
	put_line(t);
	putchar('\n'); 
    }


    return 0;
}
