/* xsh_memcmp.c - xsh_memcmp */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <prodcons.h>
/*------------------------------------------------------------------------
 * xsh_memcmp - Compares different memory allocation strategies
 *------------------------------------------------------------------------
 */
shellcmd xsh_memcmp(int nargs, char *args[]) {

	/* Output info for '--memcmp' argument */
	unsigned int s;
	if(nargs==1){
		printf("\n Implementation of allocmem\n");
		printf("Initial memory state");
		traverse();
		printf("\n Using getmem");
		char *a=getmem(sizeof(char)*500);
		long *b=getmem(sizeof(long)*1000);
		char *c=getmem(sizeof(char)*400);
		char *d=getmem(sizeof(char)*600);
		freemem(a,sizeof(char)*500);
		freemem(c,sizeof(char)*400);
		printf("\nUsing allocmem");
		char *e=allocmem(sizeof(char)*300);
		traverse();
		freemem(b,sizeof(char)*1000);
		freemem(d,sizeof(char)*600);
		printf("\nMemory state after allocation ");
		traverse();

		printf("\n\n\nImplementation of allocstk \n");
                
		resume( newcreate(producer, 1024, 20, "producer", 1, 2) );
                resume( newcreate(consumer, 1024, 20, "consumer", 1, 2) );
		printf("\nMemory state after process creation ");
		traverse();
		return 0;
	}
	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s \n\n", args[0]);
		printf("Description:\n");
		printf("\tCompares getstk and getmem with functions that do not release memory\n");
		return 0;

	}

	/* Check argument count */

	if (nargs >= 2) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;
	}
	
}
