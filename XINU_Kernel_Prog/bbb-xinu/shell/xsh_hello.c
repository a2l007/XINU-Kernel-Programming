/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_hello - Greets the input user with a  hello message
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[]) {

	/* Output info for '--help' argument */

	if(nargs==1){
		fprintf(stderr,"Too few arguments. Enter %s --help for more information\n",args[0]);
		return 1;
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

	if (nargs > 2) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;
	}
	if (nargs == 2 && (args[1][0]>='a'&& args[1][0]<='z') || (args[1][0]>='A' && args[1][0]<='Z')) {
			printf("Hello %s, Welcome to the world of Xinu\n",args[1]);
	}
	else {
		fprintf(stderr,"Invalid argument.\n Try %s --help for more information\n",args[0]);
		return 1;
	}
	return 0;
}
