#include<future.h>
syscall future_receive(future *f,int le){
intmask mask;
mask=disable();
char remoteip[]="192.168.1.100";
char msg[]="START";
char *msg1=getmem(sizeof(char)*le);
uint32 remoteipnew;
uint32 server;
uint32 rrecv;
uint16 lport=1026;
int i,j=0;
	if (f->flag==FUTURE_EXCLUSIVE){
		if(f->state==FUTURE_EMPTY){
			f->state=FUTURE_WAITING;
			f->tid=getpid();
			suspend(f->tid);
		}
		dot2ip(remoteip,&remoteipnew);
		server=udp_register(0,0,1026);
                rrecv=udp_recv(server,msg1,50,5000);
		if(rrecv==TIMEOUT){
			printf("\n Timed out");
			f->state=FUTURE_EMPTY;
			resume(f->tid);
			restore(mask);
			udp_release(server);
			return SYSERR;
		}
                else if(rrecv==SYSERR){
                        printf("\nError from future receive!");
			f->state=FUTURE_EMPTY;
			resume(f->tid);
			restore(mask);
			udp_release(server);
			return SYSERR;
		}
		else {
			printf("\nMessage received from future producer...");
			f->futmsg=msg1;
			f->state=FUTURE_VALID;
			resume(f->tid);
//			}
			printf("\nOutput from future producer is: ");
                        while(j<le)
				{printf("%c",f->futmsg[j]);
				j++;
				}
		}
	}
freemem(*msg,sizeof(char)*le);
udp_release(server);
restore(mask);
return OK;
}
