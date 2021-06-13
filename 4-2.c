/* Exercise 4-2. Extend atof to handle scientific notation of the form

       123.45e-6

   where a floating point number may be followed by e or E and an
   optionally signed exponent.
   */

#include <stdio.h>
#include <ctype.h>

#include "knr2.h"

/* atof0() converts s string in the form:

       -123.456e-78

   to a floating point value using the formula:
       val  = sign * (coefficient / precision-1) * 10^exponent
   e.g.
       val = -1 * (123456 / (10^3)) * 10^(1/78) = 123.456e-78

   Decimal point is optional. The start of the exponent field can
   be represented as 'e' or 'E' or omitted if no exponent. If the sign
   of the coefficient or exponent is ommited it is assumed positive.
*/
double
atof0(char *s)
{
    double coef, prec, exp;	/* coefficient, precision, exponent */
    int i, csign, esign;

    for (i=0; s[i]!='\0' && isspace(s[i]); ++i)
	;
    csign = s[i]=='-' ? -1 : 1;	/* sign */
    if (s[i]=='-' || s[i]=='+')
	++i;
    for (coef=0.0; isdigit(s[i]); ++i) /* coeficient */
	coef = 10.0*coef + (s[i]-'0');
    if (s[i]=='.')
	++i;
    for (prec=1.0; isdigit(s[i]); ++i, prec*=10) /* precision */
	coef = 10.0*coef + (s[i]-'0');
    if (s[i]=='e' || s[i]=='E') 
	++i;
    esign = s[i]=='-' ? -1 : 1;
    if (s[i]=='-' || s[i]=='+')
	++i;
    for(exp=0.0; s[i]!='\0' && isdigit(s[i]); ++i) /* exponent */
	exp = 10*exp + (s[i]- '0'); 
    if (esign == -1)
	exp = 1 / exp;

    return csign * (coef / power(10,prec-1)) * power(10, exp);
}

int
main(int argc, char *argv[])
{
    int i;

    if (argc<2) {
	fprintf(stderr, "USAGE: %s [float1] ... [floatN]\n", argv[0]);
	return 1;
    }

    for (i=1; i<argc; ++i)
	printf("%s:%e\n", argv[i], atof0(argv[i]));

    return 0;
}
    
