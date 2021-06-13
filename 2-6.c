#include <stdio.h>

#define MAX 79

void uprintb(unsigned x, char *label);

/* Excercise 2-6: Write a function setbits(x,p,n,y) that returns x
   with the n bits that begin at position p set to the rightmost n
   bits of y, leaving the other bits unchanged. */
unsigned
setbits(unsigned x, unsigned p, unsigned n, unsigned y)
{
    unsigned xmask, ymask;

    ymask = ~(~0 << n);		/* n lsb set 1  */
    xmask = ~(ymask << p);	/* n through n+p set 1 */

    uprintb(ymask, "y bitmask");
    uprintb(xmask, "x bitmask");

    return ((y & ymask) << p) | (x & xmask);
}

/* Excercise 2-7: Write a function invert(x,p,n) that returns x with
   the n lsb that begin at p inverted */
unsigned
invert(unsigned x, unsigned p, unsigned n)
{
    unsigned mask; 

    mask = ~(~0 << n) << p;	/* n through n+p set 1 */
    uprintb(mask, "bitmask");

    return (~x & mask) | (x & ~mask);
}

/* Excercise 2-8: Write a function rightrot(x,n) that returns the
   value of the integer xrotated by n bit positions. */
unsigned
rightrot(unsigned x, unsigned n)
{
    unsigned mask, nlsb;

    mask = ~(~0 << n); 		/* n lsb set 1 */
    nlsb = (x & mask) << ((8U*sizeof x)-n);

    uprintb(mask, "bitmask");
    uprintb(nlsb, "n lsb");

    return (x >> n) | nlsb;
}

/* Excercise 2-9: In a two's compliment system, del_ls1(x) below deletes the
   rightmost 1 bit in x. Explain why. Use this observation to write a
   faster version of bitcount. */
unsigned
del_ls1(unsigned x)
{
    uprintb(x, "x");
    uprintb(x-1, "bitmask ==(x-1)");
    uprintb(x & (x-1), "x & (x-1)"); 
    
    return x & (x-1);
}

/* Explanation of x &= x-1:

   Subtracting 1 will always unset the lowest 1bit in x. When the
   number is odd, it is trival question as the only number changed is
   the least significant bit - the lsb is only set in one of the &'d
   terms and so is unset in the result.

   0x0000ff01 |0000 0000|0000 0000|1111 1111|0000 0001| 65281 == x
   0x0000ff00 |0000 0000|0000 0000|1111 1111|0000 0000| 65280 == x-1
   0x0000ff00 |0000 0000|0000 0000|1111 1111|0000 0000| 65280 == x & (x-1)

   However subtracting 1 will set some other less significant bits
   0bits when the number is even, e.g.  

   0x0000ff00 |0000 0000|0000 0000|1111 1111|0000 0000| 65280 == x
   0x0000feff |0000 0000|0000 0000|1111 1110|1111 1111| 65279 == x-1
   0x0000fe00 |0000 0000|0000 0000|1111 1110|0000 0000| 65024 == x & (x-1)

   Since these newly set 1bits are at less significant positions than
   the lowest 1bit in x; when &'d, these changes do not persist. */

/* bitcount: count 1bits in x (from p50) */
int 
bitcount_old(unsigned x)
{
    int b;

    for (b=0; x!=0; x >>= 1) {
	if (x & 01)
	    b++;
    }
	return b;
}

int
bitcount_new(unsigned x)
{
    int b;

    for (b=0; x!=0; ++b)
	x &= x-1; 

    return b;
}

/* Print an unsigned integer in the format:

   0xFFFFFFFF |1111 1111|1111 1111|1111 1111|1111 1111| 4294967295 label \n */
void
uprintb(unsigned x, char *label)
{
    unsigned n, N;

    N = 8U*sizeof x;
    if (N == 32)
	printf("0x%08x ", x);
    if (N == 16)
	printf("0x%04x ", x);
    
    for (n=0; n<N; ++n) {
	if (n%8 == 0)
	    putchar('|');
	else if (n%4 == 0)
	    putchar(' ');
	putchar(x &(1 << (N-n-1)) ? '1' : '0');
    }

    printf("| %.10u %s\n", x, label); 
}

/* Reads one line or lim characters (whichever is less) from stdin and
   writes them into buffer at s as a null terminated string.

   Reading a newline character '\n' terminates reading, it is not
   written to s.

   Returns the number of characters written to s excluding the null
   byte. */
int
get_string(char *s, int lim)
{
    int i;
    char c;

    for (i=0; i<lim && (c=getchar())!=EOF && c!='\n'; ++i) 
	s[i] = c;

    s[i] = '\0';

    return i;
}

void
put_line(const char *line)
{
    int i;

    for (i=0; line[i]!='\0'; ++i)
	putchar(line[i]);
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
    unsigned val;

    if (exp == 0)
	return 1;

    for (val=base; exp-1; --exp)
	val *= base;

    return val;
}

/* Converts decimal string into an unsigned integer. Result is
   undefined if unsigned int overflows.

   Returns after reading n characters or non decimal character (0
   through 9) read, whichever occurs first. */
unsigned
atou(char *s, int n)
{
    unsigned u, pos;
    char a;

    for (u=pos=0; pos<n && (a=s[pos])>='0' && a<='9'; ++pos)
	u += (a-'0') * power(10, n-1-pos);
	
    return u;
}

int
main(int argc, char *argv[])
{
    unsigned x, n, p, y;

    p = 4;
    n = 8;
    y = 0xFFFFFFFF;
    x = 0x0000FFFA;

    /* Ex 2-6 */
    printf("2-6 setbits(x=0x%.8x,p=%u,n=%u,y=0x%.8x)\n"
	   "Returns x with n bits starting from p set to n lsb in y.\n",
	   x, p, n, y); 
    uprintb(x, "x");
    uprintb(y, "y");
    uprintb(setbits(x,p,n,y), "setbits(x,p,n,y)");
    putchar('\n');
	
    /* Ex 2-7 */
    printf("2-7 invert(x=0x%.8x,p=%u,n=%u)\n" 
	   "Returns x with n lsb from p inverted\n",
	   x, p, n); 
    uprintb(x, "x");
    uprintb(invert(x,p,n), "invert(x,p,n)");
    putchar('\n');

    /* Ex 2-8 */
    printf("2-8 rightrot(x=0x%.8x,n=%u)\n"
	   "Returns x rotated right by n bits\n",
	   x, n);
    uprintb(x, "x");
    uprintb(rightrot(x,n), "rightrot(x,n)");
    putchar('\n');

    /* Ex 2-9 */
    puts("2-9 bitcount(x) using x &= x-1");
    for (x=0xFF00; x<0xFF10; ++x) {
	del_ls1(x);
	if (bitcount_old(x) != bitcount_new(x))
	    return 1;
    }

    return 0;
}
