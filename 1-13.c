/* Exercise 1-13,4.

   Write a program to print a histogram of the lengths of words in its
   input. It is easy to draw the histogram with the bars horizontal,
   vertical is much more challenging. */

#include <stdio.h>

#define MAX 30			/* max word length */
#define HIST_MAXW 20.0		/* max histogram width */

int   freq[MAX];
float rfreq[MAX];
int   hplot[MAX];

int main(int argc, char *argv[])
{
    int w, ch, len, bin, sum, max;

    /* frequency */
    for (w = 1; w < argc; ++w) { /* each word */
	ch = len = 0;
	while (argv[w][ch++] != '\0') /* each character */
	    ++len;
	++freq[len];
    }

    printf("Frequency: ");
    for (bin = 0; bin < MAX; bin++)
	printf("%d ", freq[bin]);
    printf("\n");

    /* relative freq (r = freq / sum) */
    for (bin = sum = max = 0; bin < MAX; bin++) {
	sum += freq[bin];
	if (freq[bin] > freq[max])
	    max = bin;		/* index of max result */
    }
    printf("Max = %d at %d\n", freq[max], max);

    for (bin = 0; bin < MAX; bin++)
	rfreq[bin] = (float)freq[bin] / sum;
    printf("Relative frequency: ");
    for (bin = 0; bin < MAX; bin++)
	printf("%.2f ", rfreq[bin]);
    printf("\n");



    /* Draw histogram */
    printf("Histogram:\n");
    for (bin = 0; bin < MAX; bin++) {
	hplot[bin] = HIST_MAXW * (rfreq[bin] / rfreq[max]);
	printf("%03d [%02d]|", hplot[bin], bin);
	for (int i = 0; i < hplot[bin]; ++i)
	    printf("-");
	printf("\n");
    }

    return 0;
}
