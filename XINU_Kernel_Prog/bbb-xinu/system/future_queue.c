#include <future.h>
#include <prodcons.h>
syscall enq(pid32 p,future *fut, int call){
	intmask mask;
	mask=disable();
	if(call==FROM_GET){
	int x=fut->get_queue.back;
	fut->get_queue.qarr[x]=p;
	++(fut->get_queue.back);
//	printf("\nEnqueued pid for getQ %d",p);
//	printf("Value of back %d and front %d",fut->get_queue.back,fut->get_queue.front);	
	restore(mask);
	}

	else if(call==FROM_SET){
	int x=fut->set_queue.back;
	fut->set_queue.qarr[x]=p;
	++(fut->set_queue.back);
//	printf("\nEnqueued pid for setQ%d",p);
//	printf("Value of back %d and front %d",fut->get_queue.back,fut->get_queue.front);	
	restore(mask);
	}

	return OK;
}
pid32 deq(future *fut,int call){
	pid32 procid;
	intmask mask;
	mask=disable();
	int x;
	if(call==FROM_GET)
	{
		x=fut->get_queue.front;
		procid=fut->get_queue.qarr[x];
		++(fut->get_queue.front);
	}
	else if (call==FROM_SET)
	{	x=fut->set_queue.front;
		procid=fut->set_queue.qarr[x];
		++(fut->set_queue.front);
	}
	//printf("Deq: Value of back %d and front %d",fut->get_queue.back,fut->get_queue.front);	
	restore(mask);
	return procid;
} 
