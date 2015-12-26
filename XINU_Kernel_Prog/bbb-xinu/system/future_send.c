#include<future.h>
syscall future_send(future *f, char *msg,int p){
intmask mask;
mask=disable();
char remoteip[]="192.168.1.100";
//char msg[]="START";
int32 localport=1026;
int32 remoteport=1025;
char msg1[50];
uint32 remoteipnew;
uint32 client;
uint32 rsend;
uint16 rport;
int j=0;

if (f->flag==FUTURE_EXCLUSIVE){
                dot2ip(remoteip,&remoteipnew);
                client=udp_register(remoteipnew,remoteport,localport);
	        rsend=udp_send(client,msg,strlen(msg));
		if(rsend==OK){
			printf("\nInput sent to Futures Producer...");
			if(f->state==FUTURE_WAITING){
				resume(f->tid);
			}
			f->state=FUTURE_WAITING;
			f->tid=getpid();
			suspend(f->tid);
			if(f->state==FUTURE_EMPTY){
				printf("\n Exiting..");
				restore(mask);
				resume(p);
				return SYSERR;
			}
			printf("\nValue from producer is");
			while(j<strlen(msg))
                          {printf("%c",f->futmsg[j]);
                           j++;
                          }
			printf("\n");

		}
                else{
                        printf("\nInvoke failed");
			restore(mask);
			resume(p);
			return SYSERR;
		}
}
resume(p);
udp_release(client);
restore(mask);
return OK;

}
