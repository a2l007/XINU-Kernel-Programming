#include <prodcons.h>
//#include <stdio.h>
 void producer(int count)
 {
      //Code to produce values less than equal to count, 
      //produced value should get assigned to global variable 'n'.
      //print produced value e.g. produced : 8
int i;
for(i=1;i<=count;i++){
	wait(produced);
	n=i;
	printf(" Produced value: %d\n  ", n);
	signal(consumed);
 }
}
