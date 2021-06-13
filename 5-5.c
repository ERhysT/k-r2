/* Excercise 5-5. Write versions of the library functions strncpy,
   strncat, and strncmp, which operate on at most the first n
   characters of their arguement strings. For example, strncpy(s,t,n)
   copies at most n characters of t to s. Full descriptions are in
   Appendix B. */

#include <stdio.h>
#define MAX 15UL

/* strncpy_(): copy at most n-1 characters of t into c.

   In the case of t being longer than n, the copy is truncated and the
   nth character replaced with a null character to terminate the
   string. */
char *strncpy_(char *s, const char *t, size_t n)
{
    while (n-- && (*s++ = *t++))
	;
    *--s = '\0';

    return s;
}

/* strncat_(): concatenate at most n-1 characters of t onto s. 

   In the case of t being longer than n, the copy is truncated and the
   nth character replaced with a null character to terminate the
   string. */
char *strncat_(char *s, const char *t, size_t n)
{
    while (*s++)		/* move to end */
	;
    strncpy_(--s, t, n);

    return s;
}

/* strncmp_(): compare at most n characters of string t to string s.

   returns: < 0 when s<t
            > 0 when s>t
              0 when s=t  */
int strncmp_(const char *s, const char *t, size_t n)
{
    while (n-- && *s!='\0' && *t!='\0' && *s++ == *t++)
	;
    return *--s - *--t;
}

int main()
{
    char s[MAX+1];
    const char *s1 = "Rhys";
    const char *s2 = "Rhyt";
    const char *s3 = "Rhyr";

    strncpy_(s, "Merhaba!", MAX);
    puts(s);
    strncat_(s, " Rhys.", MAX);
    puts(s);

    printf ("strncmp(s1=%s, s2=%s, n=%lu)\t = %d\n",s1,s2,MAX,strncmp_(s1,s2,MAX));
    printf ("strncmp(s1=%s, s3=%s, n=%lu)\t = %d\n",s1,s3,MAX,strncmp_(s1,s3,MAX));
    printf ("strncmp(s1=%s, s1=%s, n=%lu)\t = %d\n",s1,s1,MAX,strncmp_(s1,s1,MAX));


    return 0;
}
