/* Excercise 2-3: Write the function htoi(s), which converts a string
   for hexa-decimal digits (including an optional 0x or 0X into its
   equivalent integer value. The allowable digits are 0 through 9 a
   through f and A through F. */

#include <stdio.h>

#define MAX 79

/* Convert a single hexidecimal character into its value (or -1 if
   invalid hexidecimal i.e not 0123456789ABCDEFabcdef) */
int
htov(char hex)
{
	switch (hex) {
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case 'A':
	case 'a': return 10;
	case 'B':
	case 'b': return 11;
	case 'C':
	case 'c': return 12;
	case 'D':
	case 'd': return 13;
	case 'E':
	case 'e': return 14;
	case 'F':
	case 'f': return 15;
	}

	return -1;
}

/* Returns the value of base to the power of exp. If the base is not a
   positive integer the result is undefined.

   Formula:   Value = base^exp

   Where:
       base > 0
       exp >= 0         */
unsigned
power(unsigned base, unsigned exp)
{
    if (exp == 0)
	return 1;

    while (--exp)
	base *= base;

    return base;
}

/* Returns integer from hexadecimal string.  

   Formula:   Value = Σ hn * (16^n)

   Where:
       n, position.
       hn, hexadecimal value at position n.

   The string is processed from the end backwards, any invalid hex
   characters terminate processing and the value summated up to that
   point is returned.
*/
int
htoi(char *hex, int len)
{
    int i, hn, val;

    for (val=0, i=len-1; i>=0 && (hn=htov(hex[i]))>=0; --i)
	val += hn * power(16, len-i-1);

    return val;
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
    char lbuf[MAX+1];
    int len;
    
    while ((len=get_line(lbuf, MAX)))
	printf("HEXADECIMAL %s == DECIMAL %d\n", lbuf, htoi(lbuf, len));

    return 0;
}
