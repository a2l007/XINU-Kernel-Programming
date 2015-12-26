#include<future.h>
uint32 future_prod(future *fut) {
  int i, j;
  j = (int)fut;
  for (i=0; i<1000; i++) {
    j += i;
  }
// printf("Producer has produced %d \n",j);
  syscall stats=future_set(fut, &j);
// printf("Producer has set value  %d for pid %d\n",j,getpid());
	if(stats==SYSERR)
		return SYSERR;
	else
		return OK;
}

