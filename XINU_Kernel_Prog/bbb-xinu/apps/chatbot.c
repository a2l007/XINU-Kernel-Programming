#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
 
#define SERVER "192.168.1.101"
#define BUFLEN 512  //Max length of buffer
#define RPORT 1025   //The port on which to receive data
#define SPORT 1026   //The port on which to send data
 
void die(char *s)
{
    perror(s);
    exit(1);
}
 
int main(void)
{
    struct sockaddr_in si_other,si_me,si_other1;
    int s,r, i, slen=sizeof(si_other),recv_len,slen1=sizeof(si_other1);
    char buf[BUFLEN];
    char message[BUFLEN];
 
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
     
    if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

         memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(RPORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(r , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }

        printf("\n Enter message : ");
        gets(message);
         
        //send the message
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            die("sendto()");
        }
        printf("Waiting for data...");
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(r, buf, BUFLEN, 0, (struct sockaddr *) &si_other1, &slen1)) == -1)
        {
            die("recvfrom()");
        }
         
        //print details of the client/peer and the data received
        printf("\nReceived packet from %s:%d\n", inet_ntoa(si_other1.sin_addr), ntohs(si_other1.sin_port));
        printf("\nData: %s\n" , buf);
  
/*        memset(buf,'\0', BUFLEN);
        //try to receive some data, this is a blocking call
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
        {
            die("recvfrom()");
        }
         
        puts(buf);*/
    
 
    close(s);
    close(r);
    return 0;
}
