#include<future.h>
#include<stdio.h>
uint32 future_cons(future *fut) {

  int *i, status;
  status = future_get(fut, &i);
  if (status < 1) {
    printf("future_get failed\n");
    return -1;
  }
	if(fut->flag==FUTURE_EXCLUSIVE)
  return OK;
}
