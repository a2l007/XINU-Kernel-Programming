#include<future.h>
syscall future_free(future* f,queue* q1,queue* q2) {
intmask mask;
mask=disable();
syscall sig=freemem(f,sizeof(f));
syscall sig1=freemem(f->value,sizeof(int));
freemem(q1,sizeof(queue));
freemem(q2,sizeof(queue));
restore(mask);
return sig;
}
