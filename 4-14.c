/* Exercise 4-14. Define a macro swap(t,x,y) that interchanges two
   arguements of type t.  */

#include <stdio.h>

#define swap(t,x,y) { t tmp = x; x=y; y=tmp; }

int main()
{
    int x, y;

    x = -5;
    y = 10;

    printf("x=%d, y=%d\n", x, y);
    puts("swap(t,x,y)");
    swap(int, x, y);
    printf("x=%d, y=%d\n", x, y);

    return 0;
}
