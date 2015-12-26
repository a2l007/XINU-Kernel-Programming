#include "prodconssync.h"
 void conssync(sid32 consumed, sid32 produced)
 {
     //Code to consume values of global variable 'n' until the value
//     of n is less than or equal to count
     //print consumed value e.g. consumed : 8
int i=0,c=0;
wait(consumed);
while(1)
{	
	printf("Consumed %d",arr[i]);
	i++;
	if(i==15){
		signal(produced);
//		wait(consumed);
		i=0;
		++c;
	}
if(c==2)
	break;
//printf("\n");
}
}
