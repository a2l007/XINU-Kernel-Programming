/* xsh_chat.c - xsh_chat */

#include <future.h>
/*------------------------------------------------------------------------
 * xsh_chat - Sends and receives user input messages to the Linux VM
 *------------------------------------------------------------------------
 */
shellcmd xsh_chat(int nargs, char *args[]) {

	char remoteip[]="192.168.1.100";
	char *msg;
	char msg1[50];
	future *f;
	uint32 remoteipnew;
	uint32 server,client;
	uint32 rsend,rrecv,rip;
	uint16 rport;
	int i;
	//Handles futures version of chat
        if(strncmp(args[1], "-f", 3) == 0) {
		if(nargs==2){
			printf("\n Missing parameter. Enter chat --help for more information");
			return 0;
		}
		int len=strlen(args[2]);
		f=future_alloc(FUTURE_EXCLUSIVE);
		msg=getmem(sizeof(char)*len);
		msg=args[2];
		resume( create(future_send, 1024, 20, "fsend1", 3, f,msg,getpid() ));
                resume( create(future_receive, 1024, 20, "frecv1", 2, f,len) );
		suspend(getpid());
		freemem(*msg,sizeof(char)*len);
		return 0;
	}

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s <option> <message>\n\n", args[0]);
		printf("Description:\n");
		printf("\tInitiates a conversation with the VM\n");
		printf("Options (one per invocation):\n");
		printf("\t-f\tInvokes future mode\n");
		printf("\t--help\tdisplay this help and exit\n");
		return 0;
	}
	// Handles non futures version of chat
	if (nargs == 2) {
		dot2ip(remoteip,&remoteipnew);
		server=udp_register(0,0,1026);
		client=udp_register(remoteipnew,1025,1026);
		rrecv=udp_recv(server,msg1,50,5000);
		if(rrecv==SYSERR){
			printf("\n Error receiving data");
			udp_release(server);
			udp_release(client);
			return  SYSERR;
		}
		else if(rrecv==TIMEOUT){
			printf("\n Request timed out!");
			udp_release(server);
			udp_release(client);
			return SYSERR;
		}
		//Successful reception of data
		else{
			printf("\nReceived message from %s: ",remoteip); 
			while((msg1[i]>='a'&&msg1[i]<='z') || (msg1[i]>='A' && msg1[i]<='Z')){
				printf("%c",msg1[i]); 
				i++;
			}
		}
	printf("\nSending message %s...",args[1]);
	rsend=udp_send(client,args[1],strlen(args[1]));
	if(rsend==SYSERR){
		printf("\n Error sending data");
		udp_release(server);
		udp_release(client);
		return  SYSERR;
	}
	udp_release(server);
	udp_release(client);
	return 0;
	}

	/* Check argument count */

	if (nargs > 2) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;
	}
	else {
		fprintf(stderr,"Invalid argument.\n Try %s --help for more information\n",args[0]);
		return 1;
	}
	return 0;
}
