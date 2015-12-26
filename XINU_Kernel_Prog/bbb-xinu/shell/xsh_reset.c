/* xsh_hello.c - xsh_hello */
#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_hello - Greets the input user with a  hello message
 *------------------------------------------------------------------------
 */
shellcmd xsh_reset(int nargs, char *args[]) {
volatile uint32 *prm_rstctrl=(volatile uint32 *) 0x44E00F00;

*prm_rstctrl= 0x01;
	/* Output info for '--help' argument */

}
