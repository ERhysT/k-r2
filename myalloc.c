/* An example storage allocator. */

#include <stddef.h>
#include <stdio.h>
#include <limits.h>

#define NALLOC 1024		/* minimum units to request */

/* Header: memory aligned linked list */
typedef max_align_t Align;	
union header {
    struct {
	union header *ptr;	/* next block if on free list */
	unsigned size;		/* size of this block */
    } s;
    Align x;			/* force alignment */
};
typedef union header Header;
static Header base; 		/* empty list to get started */
static Header *freep = NULL;	/* start of free list */

static Header *morecore(unsigned nu);
void *myalloc(unsigned nbytes);
void myfree(void *ap);

/* between(): returns non zero if a is between lower and upper bounds */
#define between(a,l,u) ((a)>(l) && (a)<(u))

/* malloc(): general-purpose storage allocator. */
void *myalloc(unsigned nbytes)
{
    Header *p, *prevp; 
    unsigned nunits;

    if (nbytes == 0 || nbytes > UINT_MAX - NALLOC)
	return NULL;
    nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;
    if ((prevp = freep) == NULL) { /* no free list yet */
	base.s.ptr = freep = prevp = &base;
	base.s.size = 0;
    }
    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
	if (p->s.size >= nunits) { /* big enough */
	    if (p->s.size == nunits) /* exactly */
		prevp->s.ptr = p->s.ptr;
	    else {		/* allocate tail end */
		p->s.size -= nunits;
		p += p->s.size;
		p->s.size = nunits;
	    }
	    freep = prevp;
	    return (void *)(p+1);
	}
	if (p == freep)		/* wrapped around free list */
	    if ((p = morecore(nunits)) == NULL)
		return NULL;
    }
}

/* calloc(): allocates nmemb blocks of nbytes and initialises to zero */
void *mycalloc(unsigned nmemb, unsigned nbytes)
{
    unsigned char *p;
    unsigned n; 

    n = nmemb * nbytes;
    if ((p = myalloc(n)) == NULL)
	return NULL;
    while (n--)
	p[n] = 0;
    return p; 
}

/* free(): put block ap in free list in order of increasing address */
void myfree(void *ap)
{
    Header *bp, *p;		/* header of ap, of upper block */

    bp = (Header *)ap - 1;	/* point block to header */
    if (bp->s.size == 0) 
	return;

    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
	if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
	    break;	    /* freed block at start or end of arena */

    if (bp + bp->s.size == p->s.ptr) {/* join to upper nbr */
	bp->s.size += p->s.ptr->s.size;
	bp->s.ptr = p->s.ptr->s.ptr;
    } else {
	bp->s.ptr = bp->s.ptr;
    }
    if (p + p->s.size == bp) {	/* join to lower nbr */
	p->s.size += bp->s.size;
	p->s.ptr = bp->s.ptr;
    } else {
	p->s.ptr = bp;
    }
    freep = p;
}

/* bfree(): add arbitary block of len charaters to the free list. */
int bfree(char *blk, unsigned len)
{
    Header *new, *p;		/* Header of new blk, of seqential blk */

    if (len <= sizeof *new)
	return -1;		/* block size too small */
    new = (Header *)blk;
    new->s.size  = len - sizeof *new; 

    for (p = freep; !between(new, p, p->s.ptr); p = p->s.ptr) /* find slot  */
	if (p >= p->s.ptr && (new > p || new < p->s.ptr)) /* eol */
	    break;

    new->s.ptr = p->s.ptr;
    p->s.ptr = new;
    return 0;
}

/* freeprint(): print all blocks in free list. */
void freeprint(void)
{
    int n;
    Header *p;

    if ((p = freep) == NULL) {
	puts("free list empty");
	return;
    }
    
    n  = 0; 
    do {
	printf("%d: @0x%p { .size=%uB, .ptr=0x%p }\n",
	       n++, p, p->s.size, p->s.ptr);
	p = p->s.ptr;
    } while (p != NULL && p != freep);
}


/* morecore(): ask system for more memory */
static Header *morecore(unsigned nu)
{
    char *cp, *sbrk(int);
    Header *up;

    if (nu < NALLOC)
	nu = NALLOC;
    cp = sbrk(nu * sizeof(Header));
    if (cp == (char *) -1)	/* no space at all */
	return NULL;
    up = (Header *) cp;
    up->s.size = nu;
    myfree((void *)(up + 1));
    return freep;
}
    
/* myalloc: testing the allocation and freeing of memory */
int main(void)
{
    char *s, *sptr; 
    const char mesg[] = "Hi.\n";
    const char *t;
    char c;
    static char tobfree [1024];

    t = mesg;
    if ((s = myalloc(10)) == NULL)
	return 1;

    freeprint();

    sptr = s;
    while ((*s++ = *t++))
	;
    while ((c = *sptr++))
	putchar(c);



    myfree(s);

    bfree(tobfree, 1024);

    return 0;
}
    
