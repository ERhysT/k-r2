/* io.h: UNIX stdio example  */

#ifndef KNR_IO_H
#define KNR_IO_H

#define PERMS 0666		/* user/group/other:rw/rw/rw */

#ifndef NULL
#define NULL 0;
#endif
#define EOF       (-1)
#define BUFSIZ    1024
#define OPEN_MAX  20		/* max #files open at once */

typedef struct _iobuf {
    int  cnt;			/* characters left */
    char *ptr;			/* next character position */
    char *base;			/* location of buffer */
    struct {
	unsigned read  : 1;     /* file open for reading  */	   
	unsigned write : 1;	/* file open for writing */	   
	unsigned unbuf : 1;	/* file is unbuffered */	   
	unsigned eof   : 1;	/* EOF has occurred on this file */
	unsigned err   : 1;	/* error occurred on this file */  
    } flags;			/* mode of file descriptor */
    int  fd;			/* file descriptor */
} FILE;
extern FILE _iob_[OPEN_MAX];

#define stdin  (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

int _fillbuf(FILE *);
int _flushbuf(int, FILE *);

#define feof(p)   ((p)->flag.eof)
#define ferror(p) ((p)->flag.err) 
#define fileno(p) ((p)->fd) 

#define getc(p)   (--(p)->cnt >= 0 \
		   ? (unsigned char) *(p)->ptr++ : _fillbuf(p))
#define putc(x,p) (--(p)->cnt >= 0 \
		   *(p)->ptr++ = (x) : _flushbuf(x),p)

#define getchar()  getch(stdin)
#define putchar(x) putc((x), stdout))

#endif
