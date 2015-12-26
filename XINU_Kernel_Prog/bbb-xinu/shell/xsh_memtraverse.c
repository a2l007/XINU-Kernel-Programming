/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_hello - Greets the input user with a  hello message
 *------------------------------------------------------------------------
 */
shellcmd xsh_memtraverse(int nargs, char *args[]) {

	/* Output info for '--help' argument */
	struct memblk *head, *prev;
	unsigned int s;
	head=&memlist;
	if(nargs==1){
	int *a=getmem(sizeof(int));
	long *al=getmem(sizeof(long));
	freemem(a,sizeof(int));
	traverse();
	return 0;
	}
	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s <name>\n\n", args[0]);
		printf("Description:\n");
		printf("\tGreets the user with a message\n");
		printf("Options (one per invocation):\n");
		printf("\t--help\tdisplay this help and exit\n");
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
