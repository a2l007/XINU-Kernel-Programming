#include<future.h>
#include <stdio.h>
syscall future_get(future *f, int *value){
intmask mask;
int processid;
mask=disable();
if(f->flag==FUTURE_QUEUE){
	if(f->state==FUTURE_EMPTY){
		f->tid=getpid();
		wait(conssem);
		f->state=FUTURE_WAITING;
		enq(f->tid,f,FROM_GET);
//		printf("Consumer  %d waiting\n",f->tid);
		signal(conssem);
		suspend(f->tid);
		*value=f->value;
		//if((f->get_queue.front < (f->get_queue.back))||(f->set_queue.front < (f->set_queue.back)))
		//f->state=FU	TURE_WAITING;
//		printf("& %d &",f->tid);
		sleep(1);
		printf("QUEUE:Consumer %d has consumed %d\n\n", getpid(),*value);
		restore(mask);
		return OK;
	}
	else if(f->state==FUTURE_WAITING){
		 if(!isSetQueueEmpty){
			wait(conssem);
                	processid=deq(f,FROM_SET);
			signal(conssem);
			resume(processid);
                	*value=f->value;
			sleep(1);
			printf("QUEUE:Consumer %d has consumed %d\n\n", getpid(),*value);
			restore(mask);
			return OK;
	        }
        	else {
                	f->tid=getpid();
			wait(conssem);
                	enq(f->tid,f,FROM_GET);
//			printf("Consumer  %d waiting\n",f->tid);
                	signal(conssem);
			suspend(f->tid);
			*value=f->value;
			sleep(1);
			printf("QUEUE:Consumer %d has consumed %d\n\n", getpid(),*value);
			restore(mask);
			return OK;
		}}
	else if(f->state==FUTURE_VALID) {
		 if(!isSetQueueEmpty){
			wait(conssem);
                	processid=deq(f,FROM_SET);
			signal(conssem);
			resume(processid);
                	*value=f->value;
			sleep(1);
			printf("QUEUE:Consumer %d has consumed %d\n\n", getpid(),*value);
			restore(mask);
			return OK;
	        }
		else {
                	f->tid=getpid();
			wait(conssem);
                	enq(f->tid,f,FROM_GET);
			f->state=FUTURE_WAITING;
//			printf("Consumer  %d waiting\n",f->tid);
			signal(conssem);
                	suspend(f->tid);
			*value=f->value;
			sleep(1);
			printf("QUEUE:Consumer %d has consumed %d\n\n", getpid(),*value);
			restore(mask);
			return OK;
		
		}
}
}

else{
if (f->state==FUTURE_VALID){
	*value=f->value;
	printf("QUEUE:Consumer %d has consumed %d\n\n", getpid(),*value);
	f->state=FUTURE_EMPTY;
	restore(mask);
	return OK;
}

if(f->flag==FUTURE_SHARED){
        f->tid=getpid();
        f->state=FUTURE_WAITING;
        enq(f->tid,f,FROM_GET);
        suspend(f->tid);
	*value=f->value;
   	printf("SHARED: Consumer %d has consumed %d\n\n", getpid(),*value);
	restore(mask);
        return OK;
}

else if(f->flag==FUTURE_EXCLUSIVE){
	if(f->state==FUTURE_EMPTY){
		f->tid=getpid();
        	f->state=FUTURE_WAITING;
        	wait(futuresem);
        	*value=f->value;
        	printf("EXCLUSIVE: Consumer %d has consumed %d\n\n", getpid(),*value);
		f->state=FUTURE_EMPTY;
        	restore(mask);
        	return OK;
	}
	else if(f->state==FUTURE_WAITING){
        	restore(mask);
        	return SYSERR;
		}
	}
}
}
