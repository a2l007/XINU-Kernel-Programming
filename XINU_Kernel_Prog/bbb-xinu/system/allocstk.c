/* getmem.c - getmem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getmem  -  Allocate heap storage, returning lowest word address
 *------------------------------------------------------------------------
 */
char  	*allocstk(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr, *leftover;
	struct  memblk *wfit, *wfitprev;
	uint32 wfsize,bytes=nbytes;
	mask = disable();
	char *memptr;
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/
	prev = &memlist;
	curr = memlist.mnext;
	wfsize=curr->mlength;
	wfit=NULL;
	wfitprev=NULL;
	while (curr != NULL) {	/* Search free list	*/
		if(((curr->mlength)>=wfsize) && (curr->mlength)>=nbytes){
		wfsize=curr->mlength;
		wfit=curr;
		wfitprev=prev;
		}
		prev=curr;
		curr=curr->mnext;
	}
	
	if(wfit==NULL) {
		printf("Memory Full!!\n");
		restore(mask);
		return (char *)SYSERR;
	}
		if (nbytes==wfit->mlength) {	/* Block is exact match	*/
			wfitprev->mnext = wfit->mnext;

		} else { /* Split big block	*/
			wfit->mlength-=nbytes;
			wfit = (struct memblk *)((uint32) wfit +
					wfit->mlength);
		} 
			memlist.mlength -= nbytes;
			restore(mask);
			memptr=(char *)((uint32)wfit+nbytes-sizeof(uint32));
			while(bytes>0){
				*memptr='#';
				*--memptr;
				--bytes;
			}
			memptr=(char *)((uint32)wfit+nbytes-sizeof(uint32));
			return (char *)((uint32)wfit+nbytes-sizeof(uint32));
}
