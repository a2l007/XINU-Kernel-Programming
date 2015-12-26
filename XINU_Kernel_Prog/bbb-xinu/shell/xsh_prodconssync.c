#include <prodcons.h>
//#include <xinu.h>
int arr[15];                 //Definition for global variable 'n'
/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/
void prodsync(sid32, sid32), conssync(sid32, sid32);

shellcmd xsh_prodconssync(int nargs, char *args[])
{	int len=0,j,num=0,digit,k=1;
        int count = 2000;             //local varible to hold count
      //Argument verifications and validations
      	sid32 produced, consumed;
	consumed = semcreate(0);
	produced = semcreate(1);
	if(nargs==2){
	 if (strncmp(args[1], "--help", 7) == 0) {
                printf("Usage: %s <count>\n\n", args[0]);
                printf("Description:\n");
                printf("\tRuns the producer-consumer problem for user-specified number of times. Default value is 2000\n");
                printf("Options (one per invocation):\n");
                printf("\t--help\tdisplay this help and exit\n");
                return 0;
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
	}
     }
	if(nargs>2){
		fprintf(stderr,"Too many arguments. Enter --help option for more information\n");
		return 0;
	}
      //check args[1] if present assign value to count
      //create the process producer and consumer and put them in ready queue.
      //Look at the definations of function create and resume in exinu/system folder for reference.      
      resume( create(prodsync, 1024, 20, "producer", 2,consumed,produced) );
      resume( create(conssync, 1024, 20, "consumer", 2,consumed,produced) );
}
