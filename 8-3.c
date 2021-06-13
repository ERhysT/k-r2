/* Excercise 8-3. Design and write _flushbuf, fflush and fclose */

#include "io.h"

int main(void)
{
    int c;

    while ((c = getchar()) != EOF) {
	putchar(c);
	fflush(stdout);
    }

    fclose(stdin);
    fclose(stdout);
    fclose(stderr);

    return 0;
}
