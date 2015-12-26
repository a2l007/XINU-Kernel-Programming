#include <queue.h>
#include <xinu.h>
#ifndef _FUTURE_H_
#define _FUTURE_H_
 
/* define states */
#define FUTURE_EMPTY	  0
#define FUTURE_WAITING 	  1         
#define FUTURE_VALID 	  2         
#define QSIZE		20
#define MSGSIZE		50
/* modes of operation for future*/
#define FUTURE_EXCLUSIVE  1	
#define FUTURE_SHARED 	2
#define FUTURE_QUEUE	3

#define FROM_SET 5
#define FROM_GET 6
#define isGetQueueEmpty (f->get_queue.front == f->get_queue.back)
#define isSetQueueEmpty (f->set_queue.front == f->set_queue.back)
typedef struct qdata 
{ 
  pid32 qarr[QSIZE];
  int front;
  int back;
} queue;		

typedef struct futent
{
   int *value;		
   int flag;		
   int state;         	
   pid32 tid;
   char *futmsg;
  queue set_queue;
  queue get_queue;
} future;

/* Interface for system call */
future* future_alloc(int future_flags);
syscall future_free(future*);
syscall future_get(future*, int*);
syscall future_set(future*, int*);
syscall future_send(future*,char*,int);
syscall future_receive(future*,int);
extern sid32 futuresem;
extern sid32 prodsem;
extern sid32 conssem; 
extern sid32 sharedsem; 
uint32 future_prod(future *);
uint32 future_cons(future *);
syscall enq(pid32, future *, int );
pid32 deq(future *, int);
#endif /* _FUTURE_H_ */
