/* Excercise 8-2. Implement fseek() */

#include "io.h"

int main(void)
{
    int c;
    FILE *fp;

    if ((fp = fopen("txt", "r")) == NULL)
	return 1;
    if (fseek(fp, 4, 0) < 0)
	return 2;
    while ((c = getc(fp)) != EOF)
	putchar(c);

    fclose(fp); 
    fclose(stdout);
    return 0;
}
