/* Excercise 7-3. Revise minprintf to handle more of the other
   facilities of printf. */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "knr2.h"

int main(void)
{
    char s[100];

    minscanf("%s", s);
    minprintf("Read: %s\n", s);
    return 0;
}
