/* Exercise 2-1: Write a program to determine the ranges of char,
   short, int and long variables, both signed and unsigned, by
   printing appropirate values from standard headers and by direct
   computation. Harder if you compute them: determine the ranges of
   floating point types */

#include <stdio.h>
#include <limits.h>
#include <float.h>

int
main(int argc, char *argv[])
{
    puts("Sizes and limits for integer types from limits.h (computed values in brackets)\n");

    /* Unsigned integer types */
    printf("unsigned char (%luB) max=%hhu(%hhu)\n", sizeof (unsigned char), UCHAR_MAX, (unsigned char)-1);
    printf("unsinged short (%luB) max=%hu(%hu)\n", sizeof (unsigned short), USHRT_MAX, (unsigned short)-1); 
    printf("unsinged int (%luB) max=%u(%u)\n", sizeof (unsigned int), UINT_MAX, (unsigned)-1); 
    printf("unsinged long (%luB) max=%lu(%lu)\n", sizeof (unsigned long), ULONG_MAX, -1L); 
    printf("unsinged long long (%luB) max=%llu(%llu)\n", sizeof(unsigned long long), ULLONG_MAX, -1LL); 
    putchar('\n');
    
    /* Signed integer types */
    printf("signed char min=%hhd(%hhd), max=%hhd(%hhd)\n", CHAR_MIN, 0x80, CHAR_MAX, ~0x80);
    printf("short min=%hd(%hd) max=%hd(%hd)\n", SHRT_MIN, 0x8000, SHRT_MAX, ~0x8000); 
    printf("int min=%d(%d) max=%d(%d)\n", INT_MIN, 0x80000000, INT_MAX, ~0x80000000); 
    printf("long min=%ld(%ld) max=%ld(%ld)\n", LONG_MIN, 0x8000000000000000, LONG_MAX, ~0x8000000000000000);
    printf("long long min=%lld(%lld) max=%lld(%lld)\n", LLONG_MIN, (long long)0x8000000000000000, LLONG_MAX, (long long)~0x8000000000000000); 
    putchar('\n');

    /* Signedness of char is not defined in C standard */
    printf("char is %dbits %s\n", CHAR_BIT, UCHAR_MAX==CHAR_MAX ? "unsigned" : "signed"); 
    putchar('\n');

    puts("Sizes and limits for floating point types from float.h (computed values in brackets)\n");

    printf("float (%luB) epsilon=%#g min=%#g max=%#g\n", sizeof (float), FLT_EPSILON, FLT_MIN, FLT_MAX);
    printf("double (%luB) epsilon=%#g min=%#g max=%#g\n", sizeof (double), DBL_EPSILON, DBL_MIN, DBL_MAX);    
    printf("long double (%luB) epsilon=%#Lg min=%#Lg max=%#Lg\n", sizeof (double long), LDBL_EPSILON, LDBL_MIN, LDBL_MAX);    
    return 0;
}
    
