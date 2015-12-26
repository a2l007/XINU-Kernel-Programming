#include <stddef.h>
#include <xinu.h>
/*Global variable for producer consumer*/
extern int arr[15]; /*this is just declaration*/

/*function Prototype*/

void conssync(sid32 consumed, sid32 produced);
void prodsync(sid32 consumed, sid32 produced);
