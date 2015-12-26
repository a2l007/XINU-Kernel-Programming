#include "prodconssync.h"
//#include <stdio.h>
 void prodsync(
	sid32 consumed,	sid32 produced)
 {
      //Code to produce values less than equal to count, 
      //produced value should get assigned to global variable 'n'.
      //print produced value e.g. produced : 8
int i=0,c=0;
wait(produced);
while(1){
arr[i]=i++;
printf("Produced %d ",arr[i]);
	if(i==14){
		signal(consumed);
//		wait(produced);
		i=0;
		++c;
	}
if(c==2)
	break;
 }

}
