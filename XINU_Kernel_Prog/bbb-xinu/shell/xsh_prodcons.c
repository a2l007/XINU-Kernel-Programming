#include <prodcons.h>
#include <future.h>
int n;                 //Definition for global variable 'n'
sid32 produced,consumed,futuresem,prodsem,conssem;
/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/

shellcmd xsh_prodcons(int nargs, char *args[])
{	int len=0,j,num=0,digit,k=1;
        int count = 2000;       	//local varible to hold count
        future *f_shared, *f_exclusive, *f_queue;
      	produced=semcreate(1);
	consumed=semcreate(0);		//Argument verifications and validations
		if(nargs>2){
		fprintf(stderr,"Too many arguments. Enter --help option for more information\n");
		return 0;
	}   	
	if(nargs==2){
	 if (strncmp(args[1], "--help", 7) == 0) {
                printf("Usage: %s <count>\n\n", args[0]);
                printf("Description:\n");
                printf("\tRuns the producer-consumer problem for user-specified number of times. Default value is 2000\n");
                printf("Options (one per invocation):\n");
                printf("\t--help\tdisplay this help and exit\n");
                return 0;
		}
	else if(strncmp(args[1], "-f", 3) == 0) {
			f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
  			f_shared = future_alloc(FUTURE_SHARED);
  			f_queue = future_alloc(FUTURE_QUEUE);
		if(f_exclusive==SYSERR){
			printf("Memory allocation failed");
			return 0;
		}
		if(f_shared==SYSERR){
			printf("Memory allocation failed");
			return 0;
		}
		if(f_queue==SYSERR){
			printf("Memory allocation failed");
			return 0;
		}
		futuresem=semcreate(0);
		prodsem=semcreate(1);
		conssem=semcreate(1);
//TEST EXCLUSIVE
		  resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive) );
  		  resume( create(future_prod, 1024, 20, "fprod1", 1, f_exclusive) );		 
		  
//TEST FUTURE SHARED
		  resume( create(future_cons, 1024, 20, "fcons2", 1, f_shared) );
		  resume( create(future_cons, 1024, 20, "fcons3", 1, f_shared) );
  		  resume( create(future_cons, 1024, 20, "fcons4", 1, f_shared) ); 
  		  resume( create(future_cons, 1024, 20, "fcons5", 1, f_shared) );
  		  resume( create(future_prod, 1024, 20, "fprod2", 1, f_shared) );

		//Test FUTURE_QUEUE
		 resume( create(future_cons, 1024, 20, "fcons6", 1, f_queue) );
		 resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
  		 resume( create(future_cons, 1024, 20, "fcons8", 1, f_queue) );
  		 resume( create(future_cons, 1024, 20, "fcons9", 1, f_queue) );
  		 resume( create(future_prod, 1024, 20, "fprod3", 1, f_queue) );
  		 resume( create(future_prod, 1024, 20, "fprod4", 1, f_queue) );
  		 resume( create(future_prod, 1024, 20, "fprod5", 1, f_queue) );
  		 resume( create(future_prod, 1024, 20, "fprod6", 1, f_queue) );

/*                 resume( create(future_prod, 1024, 20, "fprod3", 1, f_queue) );
                 resume( create(future_prod, 1024, 20, "fprod4", 1, f_queue) );
                 resume( create(future_prod, 1024, 20, "fprod5", 1, f_queue) );
                 resume( create(future_prod, 1024, 20, "fprod6", 1, f_queue) );
		 resume( create(future_cons, 1024, 20, "fcons6", 1, f_queue) );
                 resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
                 resume( create(future_cons, 1024, 20, "fcons8", 1, f_queue) );
                 resume( create(future_cons, 1024, 20, "fcons9", 1, f_queue) );
*/
	}

        	else {
		len=strlen(args[1]);
		for(j=len-1;j>=0;j--){
  			if(args[1][j]>='0'&&args[1][j]<='9'){
	   			digit=args[1][j]-'0';
	   			num=num+k*digit;
	   			k=k*10;
			}
			else {
				fprintf(stderr,"Invalid argument. Enter --help for more information\n");
				return 0;
			}
		}
		count=num;
      		resume( create(producer, 1024, 20, "producer", 1, count) );
		resume( create(consumer, 1024, 20, "consumer", 1, count) );

	}
     }
      //check args[1] if present assign value to count
      //create the process producer and consumer and put them in ready queue.
      //Look at the definations of function create and resume in exinu/system folder for reference.      
}
