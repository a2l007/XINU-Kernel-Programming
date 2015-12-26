#include<future.h>
#include <stdio.h>
syscall future_set(future *f, int *value){
intmask mask;
mask=disable();
int processid;
if(f->flag==FUTURE_QUEUE){

if(f->state==FUTURE_WAITING){
	if(!isGetQueueEmpty){
		wait(prodsem);
		processid=deq(f,FROM_GET);
		//f->state=FUTURE_VALID;
		f->value=*value;
//		sleep(1);
//		printf("Produced value %d pid %d\n",f->value,getpid());
		signal(prodsem);
		resume(processid);
	}
	else {
		wait(prodsem);
		f->tid=getpid();
		f->state=FUTURE_VALID;
//		printf("Producer %d waiting\n",getpid());
		enq(f->tid,f,FROM_SET);
		signal(prodsem);
		suspend(f->tid);
		f->value=*value;
//		sleep(1);
//		printf("Produced value %d pid %d\n",f->value,getpid());

	}
}

else if (f->state==FUTURE_VALID){
	wait(prodsem);
	f->tid=getpid();
//	printf("Producer %d waiting\n",getpid());
	enq(f->tid,f,FROM_SET);
	signal(prodsem);
	suspend(f->tid);
	f->value=*value;
//	sleep(1);
//	printf("Produced value %d pid %d\n",f->value,getpid());

}

else if(f->state==FUTURE_EMPTY){
		wait(prodsem);
		f->tid=getpid();
//		printf("Producer %d waiting\n",getpid());
		enq(f->tid,f,FROM_SET);
		f->state=FUTURE_VALID;
		signal(prodsem);
		suspend(f->tid);
		f->value=*value;
//		sleep(1);
//		printf("Produced value %d pid %d\n",f->value,getpid());
	}
}
else{
	if(f->state==FUTURE_VALID){
		restore(mask);
		return SYSERR;
	}
	f->value=*value;
	if(f->flag==FUTURE_SHARED){
//	printf("\nSET: Front is %d, Back is %d",f->get_queue.front,f->get_queue.back);
	while(!isGetQueueEmpty){
		processid=deq(f,FROM_GET);
		//signal(futuresem);
		resume(processid);
	}
	}
	else if (f->flag==FUTURE_EXCLUSIVE){
		if(f->state==FUTURE_WAITING)
			{
				signal(futuresem);
			}
	f->value=*value;
	f->state=FUTURE_VALID;
	}	
	}

//f->state=FUTURE_VALID;
restore(mask);
return OK;

}
