/* Excercise 1-23 - write a program to remove all comments from a C
   program. Dont forget the handle quoted strings and character
   constants properly. C comments do not nest. */

#include "stdio.h"

int
main(int argc, char *argv[])
{
    int ch, nxt, print;

    print = 1;

    while (ch=getchar(), ch!=EOF) {

	if (print==1 && ch=='"') { /* dumps entire string */
	    putchar('"');
	    putchar(nxt);
	}

	if (print==1 && ch=='/') {
	    if (nxt=getchar(), nxt=='*') { /* disable printing */
		print = 0;
	    } else {
		putchar(ch);
		ch = nxt;
	    }
	}
	
	if (print==0 && ch=='*') { 
	    if (nxt=getchar(), nxt=='/') {/* enable printing */
		ch = getchar();
		print = 1;
	    }
	}

	if (print)
	    putchar(ch);
    }
	
    return 0;
}
