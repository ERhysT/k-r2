/* Exercise 4-13. Write a recursive version of reverse(s). */

#include <stdio.h>
#include "knr2.h"
#include <string.h>

void reverse(char *s);		/* old iterative method */

/* reverse_r(): reverses the string in s using a recursive method */
void reverse_r(char *s)
{
    char tmp;
    static int i, j;

    j = strlen(s)-1-i;
    tmp = s[i];

    s[i] = s[j];
    s[j] = tmp;
    
    if (++i < --j)
	reverse_r(s);
    else {
	i = 0;
	j = 0;
    }
}

int main()
{
    char s[MAX+1], t[MAX+1];
    
    while (get_str(s, MAX)) {
	strcpy(t, s);
	reverse_r(t);

	puts(s);
	puts(t);

	reverse_r(t);
	printf("%s.\n", strcmp(s, t) ? "Fail" : "Success");
    }

    return 0;
}


