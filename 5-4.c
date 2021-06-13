/* Exercise 5-4. Write a function strend(s,t) which returns 1 if the
   string t occurs at the end of the string s, and zero otherwise. */

#include <string.h>
#include <stdio.h>

/* strend(): returns 1 if t is at the end of s, otherwise 0.  */
int strend(const char *s, const char *t)
{   
    const char *a;		/* anticipated start of t in s */

    a = s + strlen(s) - strlen(t);
    while (*a!='\0' && *t!='\0' && (*a++ == *t++)) 
	;
    return !(a - (s+strlen(s))); 
}

int main(int argc, char **argv)
{
    const char s[] = "Hi my name is Rhys.";
    const char t[] = "Rhys";
    const char q[] = "Rhys.";
    
    printf("strend(s=%s, t=%s)\t= %d\n", s, t, strend(s,t));
    printf("strend(s=%s, q=%s)\t= %d\n", s, q, strend(s,q));

    return 0;
}
