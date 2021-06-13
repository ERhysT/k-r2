/* Excercise 3-1. Our binary search makes two tests inside the loop,
   when one would suffice (at the price of more tests outside). Write
   a version with only one test inside the loop and measure to
   difference in run-time. */

#include <stdio.h> 

#define MAX 10

/* binsearch: find x in v[0] <= [1] <= ... <= v[n-1] */
int
binsearch(int x, int *v, int n)
{
    int low, high, mid;

    low = 0;
    high = n-1;

    while (low <= high) {

	mid = (low+high) / 2;
	
	if (x < v[mid])		/* lower half */
	    high = mid - 1;
	else if (x > v[mid])	/* upper half */
	    low = mid + 1;
	else			/* match */
	    return mid;
    }
    
    return -1;			/* no match */
}

int
binsearch1(int x, int *v, int n)
{
    int low, mid, high;

    low  = 0;
    high = n-1;
    mid  = (low-high) / 2;


    
    return mid;
}

/* Converts string s representing decimal number (with or without
   optional leading sign character) into integer n. */
int
atoi(char *s)
{
    int i, n, sign;

    for (i=0; s[i]==' ' || s[i]=='\t'; ++i) /* skip whitespace */
	;

    sign = (s[i]=='-') ? -1 : 1; /* record and skip sign */
    if (s[i]=='-' || s[i]=='+')
	++i;
    
    for (n=0; s[i]>='0' && s[i]<='9'; ++i) /* count */
	n = 10*n + (s[i]-'0');

    return sign * n;
}

int
main(int argc, char *argv[])
{
    int i, x, v[MAX], res; 
    
    if (argc < 2)
	return 1;

    for (i=0; i<MAX; ++i)
	v[i] = i;

    x = atoi(argv[1]);

    printf("binsearch(x=%d, v=[%d, %d, ... %d, %d], n=%d) ",
	   x, v[0], v[1], v[MAX-2], v[MAX-1], MAX); 
    res = binsearch(x, v, MAX);
    if (res == -1) {
	printf("Did not find x=%d in v.\n", x);
    } else {
	printf("Found x=%d at v[%d].\n", x, res);
    }

    

    return res == -1 ? 1 : 0;
}

