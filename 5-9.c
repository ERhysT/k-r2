/* Excercise 5-9. Rewrite the routines day_of the year and month_day
   with pointers instead of indexing. */

#include <stdio.h>

/* From knr2.c (reverted to indices because it looks better) */
const char *month_name(int n);
unsigned day_of_year(unsigned year, unsigned month, unsigned day);
void month_day(unsigned year, unsigned yearday, unsigned *pmonth, unsigned *pday);
const char *ordinate(unsigned n);

int main(int argc, char *argv[])
{
    unsigned y = 1989, m = 1, d = 21;
    unsigned doy = day_of_year(y, m, d);

    printf("%d/%d/%d was the %d%s day of the year.\n",
	   d, m, y, doy, ordinate(doy));

    doy += 10; 
    month_day(y, doy, &m, &d);
    printf("10 days laters was the %d%s day of the year -> %d/%d/%d.\n",
	   doy, ordinate(doy), d, m, y); 

    printf("Which is the same as %d%s of %s %d.\n",
	   d, ordinate(d), month_name(m), y);

    return 0;
}
