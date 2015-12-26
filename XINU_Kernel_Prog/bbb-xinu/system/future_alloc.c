#include <future.h>
#include <prodcons.h>
future* future_alloc(int future_flag){
intmask mask;
mask=disable();
if(future_flag==FUTURE_EXCLUSIVE){
	future *f=getmem(sizeof(future));
	f->value=getmem(sizeof(int));
	if(f==SYSERR)
		{
		restore(mask);
		return SYSERR;
		}
	f->flag=FUTURE_EXCLUSIVE;
	f->state=FUTURE_EMPTY;
	restore(mask);
	return f;
}
else if(future_flag==FUTURE_SHARED){
	future *f = getmem(sizeof(future));
	f->value=getmem(sizeof(int));
	queue *q =getmem(sizeof(queue));
	f->get_queue.front=0;
	f->get_queue.back=0;
	if(f==SYSERR)
		{
		restore(mask);
		return SYSERR;
		}
	f->flag=FUTURE_SHARED;
	f->state=FUTURE_EMPTY;
	restore(mask);
	return f;
}

else if(future_flag==FUTURE_QUEUE){
	future *f = getmem(sizeof(future));
	f->value=getmem(sizeof(int));
	queue *q=getmem(sizeof(queue));
	f->get_queue.front=0;
	f->get_queue.back=0;
	queue *qset=getmem(sizeof(queue));
	f->set_queue.front=0;
	f->set_queue.back=0;
	if(f==SYSERR)
		{
		restore(mask);
		return SYSERR;
		}
	f->flag=FUTURE_QUEUE;
	f->state=FUTURE_EMPTY;
	restore(mask);
	return f;
}

else
 {
	restore(mask);
	return NULL;
}
} 
