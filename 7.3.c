/* Excercise 7-3. Revise minprintf to handle more of the other
   facilities of printf. */

#include <stdarg.h>

/* minprintf(): minimal printf with variable argument list  */
void minprintf(char *fmt, ...)
{
    va_list ap;
    char *p, *sval;
    int ival;
    double dval;

    va_start(ap, fmt); 		/* ap points to 1st unnamed arg  */
    for (p = fmt; *p; p++) {
	if (*p != '%') {
	    putchar(*p);
	    continue;
	}
	switch (*++p) {
	case 'd':
	    ival = va_arg(ap, int);
	    printf("%d", ival);
	    break;
	case 'f':
	    dval = va_arg(ap, double);
	    printf("%f", dval);
	case 's':
	    for (sval = va_arg(ap, char *); *sval; sval++)
		puchar(*sval);
	    break;
	default:
	    putchar(*p);
	    break;
	}
    }
    va_end(ap);
}
	
