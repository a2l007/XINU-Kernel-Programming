/* getmem.c - getmem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getmem  -  Allocate heap storage, returning lowest word address
 *------------------------------------------------------------------------
 */
char  	*allocmem(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr, *leftover;
	struct  memblk *bfit, *bfitprev;
	uint32 bfsize;
	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/
	prev = &memlist;
	curr = memlist.mnext;
	bfsize=curr->mlength;
	bfit=NULL;
	bfitprev=NULL;
	while (curr != NULL) {	/* Search free list	*/
		if(((curr->mlength)<=bfsize) && (curr->mlength)>=nbytes){
		bfsize=curr->mlength;
		bfit=curr;
		bfitprev=prev;
		}
		prev=curr;
		curr=curr->mnext;
	}
	
	if(bfit==NULL) {
		printf("Memory Full!!\n");
		restore(mask);
		return (char *)SYSERR;
	}
		if (nbytes==bfit->mlength) {	/* Block is exact match	*/
			bfitprev->mnext = bfit->mnext;

		} else { /* Split big block	*/
			leftover = (struct memblk *)((uint32) bfit +
					nbytes);
			bfitprev->mnext = leftover;
			leftover->mnext = bfit->mnext;
			leftover->mlength = bfit->mlength - nbytes;
		} 
			memlist.mlength -= nbytes;
			restore(mask);
			return (char *)(bfit);
}
