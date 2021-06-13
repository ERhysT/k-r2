/* io.c: Linux stdio implementation example */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "io.h"

#define PERMS 0666		/* user/group/other:rw/rw/rw */

FILE __iob[OPEN_MAX] = {
    { 0, NULL, NULL, _READ,           0 }, /* 0: stdin */
    { 0, NULL, NULL, _WRITE,          1 }, /* 1: stdout */
    { 0, NULL, NULL, _WRITE | _UNBUF, 2 }  /* 2: stderr */
};

#define __sizeofbuf(p)  (((p)->flag &_UNBUF) ? 1 : BUFSIZ)
#define __allocbuf(p)   ((p)->ptr = (p)->base = malloc((p)->cnt = __sizeofbuf(p)))

/* fileopen(): open file, return file ptr */
FILE *fopen(char *name, char *mode)
{
    int fd;
    FILE *fp;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
	return NULL;
    for (fp = __iob; fp < __iob + OPEN_MAX; fp++)
	if ((fp->flag & (_READ | _WRITE)) == 0)
	    break;		/* found free slot */
    if (fp >= __iob + OPEN_MAX)
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
    fp->fd     = fd;
    fp->cnt    = 0;
    fp->base   = NULL;
    fp->flag   = (*mode == 'r') ? _READ : _WRITE;

    return fp;
}

/* fclose(): flush buffer and close file */
int fclose(FILE *fp)
{
    fflush(fp);
    if (fp->base)
	free(fp->base);
    return close(fp->fd);
}
    
/* fseek(): set file offset */
int fseek(FILE *fp, long offset, int origin)
{
    if (fflush(fp) < 0)
	return -1;
    else
	return lseek(fp->fd, offset, SEEK_SET) == -1 ? -1 : 0;
}

/* __fillbuf(): fill input buffer */
int __fillbuf(FILE *fp)
{
    if ((fp->flag&(_READ|_EOF|_ERR)) != _READ)
	return EOF;
    if ((fp->base == NULL) && (__allocbuf(fp) == NULL))
	return EOF;
    fp->cnt = read(fp->fd, fp->ptr, __sizeofbuf(fp));
    if (--fp->cnt < 0) {
	if (fp->cnt == -1)
	    fp->flag |= _EOF;
	else
	    fp->flag |= _ERR;
	fp->cnt = 0;
	return EOF;
    }
    return (unsigned char) *fp->ptr++;
}

/* __clearbuf(): discards any data in the buffer */
int __clearbuf(FILE *fp)
{
    if (fp->base != NULL) {
	fp->ptr = fp->base;
	fp->cnt = __sizeofbuf(fp);
    }
    return 0;
}

/* __flushbuf(): write buffer to file */
int __flushbuf(FILE *fp)
{
    int n;

    if ((fp->flag&(_WRITE|_EOF|_ERR)) != _WRITE)
	return EOF;
    if ((fp->base == NULL) && (__allocbuf(fp) == NULL))
	return EOF;
    if ((fp->ptr - fp->base > 0)) {
	if ((n = write(fp->fd, fp->base, fp->ptr - fp->base)) < 0) {
	    fp->flag |= _ERR;
	    return EOF;
	} else {
	    fp->cnt += n;
	    fp->ptr = fp->base;
	}
    }
    return 0;
}
