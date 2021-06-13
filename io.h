/* io.h: UNIX stdio example  */

#ifndef KNR_IO_H
#define KNR_IO_H

#ifndef NULL
#define NULL 0
#endif
#define EOF       (-1)
#define BUFSIZ    1024
#define OPEN_MAX  20		/* max #files open at once */

typedef struct __iobuf {
    int  cnt;			/* characters left */
    char *ptr;			/* next character position */
    char *base;			/* location of buffer */
    int  flag;			/* mode of file descriptor */
    int  fd;			/* file descriptor */
} FILE;
extern FILE __iob[OPEN_MAX];

#define stdin  (&__iob[0])
#define stdout (&__iob[1])
#define stderr (&__iob[2])

enum __flags {
    _READ  = 01,		/* file open for reading  */
    _WRITE = 02,		/* file open for writing */
    _UNBUF = 04,		/* file is unbuffered */
    _EOF   = 010,		/* EOF has occurred on this file */
    _ERR   = 020,		/* error occurred on this file */
};

int __fillbuf(FILE *);
int __flushbuf(FILE *);
int __clearbuf(FILE *);

/* fileopen(): open file, return file ptr */
FILE *fopen(char *name, char *mode);
/* fclose(): flush buffer and close file */
int fclose(FILE *fp);
/* fflush(): For output streams, write buffer. For input streams,
   clear buffer. */
#define fflush(p) ((p)->flag &_WRITE \
		   ? __flushbuf(p) : ((p)->flag &_READ ? __clearbuf(p) : -1))
/* fseek(): set file offset */
int fseek(FILE *fp, long offset, int origin);
/* feof(), ferror(), fileno(): return int with respective
   state/property */
#define feof(p)   (((p)->flag & _EOF) != 0)
#define ferror(p) (((p)->flag & _ERR) != 0) 
#define fileno(p) ((p)->fd) 
/* getc(): reads and returns a character from p or buffer if count is
   1 or more */
#define getc(p)   (--(p)->cnt >= 0 \
		   ? (unsigned char) *(p)->ptr++ : __fillbuf(p))
/* putc(): writes and returns a character to p's buffer. Flushes
   buffer if count is 1 or less. */
#define putc(x,p) (--(p)->cnt >= 0 \
		   ? *(p)->ptr++ = (x) : (__flushbuf(p) == -1 \
					  ? -1 : (*(p)->ptr++ = (x))))
/* getchar(), putchar(): read, write a single char to stdin */
#define getchar()  (getc(stdin))
#define putchar(x) (putc((x), stdout))

#endif
