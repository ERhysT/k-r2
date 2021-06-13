/* Excercise 8-1. Rewrite cat from chapter using read write open and
   close instead of the standard library equivalents. */

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void die(int status, const char *fmt, ...);
void warn(const char *fmt, ...);

/* 8-1 copies file from src to dest where src and dest are command
   line arguements or if not provided stdin and stdout */
int main(int argc, char **argv)
{
    int src, dest, n;
    char c;

    if (argc > 3)
	die(1, "incorrect format\nusage: %s [src] [dest]", *argv);
    src  = argc > 1 ? open(*++argv, O_RDONLY) : 0;
    if (src < 0 ) 
	die(1, "Cannot open source file");
    dest = argc > 2 ? open(*++argv, O_WRONLY) : 1;
    if (dest < 0) 
	die(1, "cannot open destination file");

    while ((n = read(src, &c, sizeof c)) == sizeof c)
	if (write(dest, &c, sizeof c) != sizeof c)
	    die(1, "cannot write to destination file");
    if (n < 0)
	die(1, "cannot read source file"); 

    if (close(src) < 0)
	warn("failed to close source file");
    if (close(dest) < 0)
	warn("failed to close destination file");

    return 0;
}
