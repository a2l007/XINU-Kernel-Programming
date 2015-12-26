/* traverse.c - traverse */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  traverse  -  Traverses the list of free memory blocks and prints them
 *------------------------------------------------------------------------
 */
syscall	traverse()
{
	struct memblk *head;
	unsigned int s;
	intmask	mask;		/* Saved interrupt mask		*/
	mask = disable();
	head=&memlist;
	if(head->mnext==NULL){
		printf("\nMemory full..!!");
		return SYSERR;
	}
	 while(head->mnext!=NULL)
        {
                head=head->mnext;
                s=(unsigned int)head->mlength;
                printf("\nBlock Size:%u", s);
                printf(" --- Address: %u\n", head);
        }
	restore(mask);
	return OK;


}
