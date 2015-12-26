/*
    Simple udp client
    Silver Moon (m00n.silv3r@gmail.com)
*/
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
 
#define SERVER "192.168.1.101"
#define BUFLEN 512  //Max length of buffer
#define SPORT 1026   //The port on which to send data
#define RPORT 1025   //The port on which to send data
char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
} 
void die(char *s)
{
    perror(s);
    exit(1);
}
 
int main(void)
{
    struct sockaddr_in si_other,si_me;
    int s, r,i, slen=sizeof(si_other),recv_len,smlen=sizeof(si_me),j=0;
    char buf[BUFLEN],*targetmsg;
    char message[]="LOLX";
    targetmsg=malloc(sizeof(char)*BUFLEN);

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
if ((r=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    } 
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(SPORT);
     

    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(RPORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if( bind(r , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }


    if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
        printf("Here1: ");
 
    while(1)
    {
    printf("\nWaiting for input... ");
        fflush(stdout);

        if ((recv_len = recvfrom(r, buf, BUFLEN, 0, (struct sockaddr *) &si_me, &smlen)) == -1)
       {
            die("recvfrom()");
       }

	printf("\nReceived input: %s",buf);	
		sleep(4);
		while(buf[j]>='a' && buf[j]<='z'||buf[j]>='A'&&buf[j]<='Z'){
			targetmsg[j]=buf[j];
			j++;
		}
		targetmsg=realloc(targetmsg,j);
		strrev(targetmsg);
		if (sendto(s, targetmsg, strlen(targetmsg) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        	{
           	 die("sendto()");
        	}
         	printf("\nOutput %s sent\n",targetmsg);
		break;
	}
    //}
        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
/*        memset(buf,'\0', BUFLEN);
        //try to receive some data, this is a blocking call
         
        puts(buf);*/
 
    close(s);
 

   return 0;
}
