/* io.c: UNIX stdio EXAMPLE */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "8-2.h"

FILE _iob[OPEN_MAX] = {
    { 0, NULL, NULL, {1,0,0,0,0}, 0 },
    { 0, NULL, NULL, {0,1,0,0,0}, 1 },
    { 0, NULL, NULL, {0,1,1,0,0}, 2 }
};

/* fileopen(): open file, return file ptr */
FILE *fileopen(char *name, char *mode)
{
    int fd;
    FILE *fp;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
	return NULL;
    for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
	if ((fp->flags.read && fp->flags.write) == 0)
	    break;		/* found free slot */
    if (fp >= _iob + OPEN_MAX)
	return NULL;		/* no free slots */

    if (*mode == 'w')
	fd = creat(name, PERMS);
    else if (*mode == 'a') {
	if ((fd = open(name, O_WRONLY, 0)) == - 1)
	    fd = creat(name, PERMS);
	lseek(fd, 0L, 2);
    } else
	fd = open(name, PERMS);

    if (fd == -1)		/* couldnt access name */
	return NULL;
    fp->fd   = fd;
    fp->cnt  = 0;
    fp->base = NULL;
    if (*mode == 'r')
	fp->flags.read = 1;
    else
	fp->flags.write = 1;
    return fp;
}

/* _fillbuf(): allocate and fill input buffer */
int _fillbuf(FILE *fp)
{
    int bufsize;

    if (!fp->flags.read)
	return EOF;
    bufsize = (fp->flags.unbuf) ? 1 : BUFSIZ;
    if (fp->base == NULL)	/* no buffer yet */
	if ((fp->base = malloc(bufsize)) == NULL)
	    return EOF;		/* cant get buffer */
    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->ptr, bufsize);
    if (--fp->cnt < 0) {
	if (fp->cnt == -1)
	    fp->flags.eof = 1;
	else
	    fp->flags.err = 1;
	fp->cnt = 0;
	return EOF;
    }
    return (unsigned char) *fp->ptr++;
}
