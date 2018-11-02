/* vgetmem.c - vgetmem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <proc.h>
#include <paging.h>

#define SETZERO 0
#define SETONE  1
#define TWOTEN  1024

extern struct pentry proctab[];
/*------------------------------------------------------------------------
 * vgetmem  --  allocate virtual heap storage, returning lowest WORD address
 *------------------------------------------------------------------------
 */
WORD	*vgetmem(nbytes)
	unsigned nbytes;
{
	STATWORD ps;
	struct mblock *index;
	struct mblock *indexDos;
	struct mblock *indexTres;
	kprintf("VGETMEM To be implemented!\n");

int checkList = proctab[currpid].vmemlist->mnext;

if (nbytes == SETZERO || checkList == (struct mblock*)NULL) {
	/* code */
	restore(ps);
	return -SETONE;
}

int listFor;
nbytes = (unsigned int) roundmb(nbytes);
listFor = &proctab[currpid].vmemlist;
indexDos = listFor;
index = indexDos->mnext;
while (index != (struct mblock *)NULL) {
	/* code */
	int checkMlen = index->mlen;
	if (checkMlen == nbytes) {
		/* code */
		indexDos->mnext = index->mnext;
		restore (ps);
		return ((WORD*)index);
	} else if(checkMlen > nbytes) {
		int conv = (unsigned)index + nbytes;
		indexTres = (struct mblock *) (conv);

		indexDos->mnext = indexTres;
		indexTres->mnext = index->mnext;
		int sub = index->mlen - nbytes;
		indexTres->mlen = sub;

		restore(ps);
		return ((WORD*)index);
	}

	indexDos = index;
	index = index->mnext;
}
	restore(ps);
	return((WORD*) -SETONE );
}

//
// /* vgetmem.c - vgetmem */
//
// #include <conf.h>
// #include <kernel.h>
// #include <mem.h>
// #include <proc.h>
// #include <paging.h>
//
// extern struct pentry proctab[];
// /*------------------------------------------------------------------------
//  * vgetmem  --  allocate virtual heap storage, returning lowest WORD address
//  *------------------------------------------------------------------------
//  */
// WORD	*vgetmem(nbytes)
// 	unsigned nbytes;
// {
//
// 	//kprintf("To be implemented!\n");
// 	STATWORD ps;
// 	struct mblock *p, *q, *leftover;
// 	disable(ps);
//
// 	if(nbytes==0 || proctab[currpid].vmemlist->mnext== (struct mblock*) NULL ){
// 		restore(ps);
// 		return SYSERR;
// 	}
// 	nbytes=(unsigned int ) roundmb(nbytes);
// 	q=&proctab[currpid].vmemlist;
// 	for(p=q->mnext;p!=(struct mblock*) NULL ;q=p,p=p->mnext){
// 		if(p->mlen==nbytes){
// 			q->mnext=p->mnext;
// 			restore(ps);
// 			return ((WORD*)p);
// 		}
// 		else if(p->mlen>nbytes){
// 			leftover=(struct mblock *)((unsigned)p + nbytes);
// 			q->mnext=leftover;
// 			leftover->mnext=p->mnext;
// 			leftover->mlen=p->mlen-nbytes;
// 			restore(ps);
// 			return ((WORD*)p);
// 		}
// 	}
// 	restore(ps);
//
// 	return((WORD*) SYSERR );
// }
