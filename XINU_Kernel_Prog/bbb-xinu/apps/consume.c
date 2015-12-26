#include <prodcons.h>
//#include <stdio.h>
 void consumer(int count)
 {
     //Code to consume values of global variable 'n' until the value
//     of n is less than or equal to count
     //print consumed value e.g. consumed : 8
int i;
for(i=1;i<=count;i++){
	wait(consumed);
	printf("Consumed value %d \n", n--);
	signal(produced);
 }
//printf("\n");
semdelete(produced);
semdelete(consumed);
}
