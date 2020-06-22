#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <net/if.h>

#include "util.h"
#include "version.h"
#include "main.h"

#ifdef _ONE_SERVER
int checkmac( void )
{

  struct ifreq ifreq;
  int sock;
  if((sock=socket(AF_INET,SOCK_STREAM,0))<0)
  {
     perror("socket");
     return 0;
  }
  strcpy(ifreq.ifr_name,"e");
  strcat(ifreq.ifr_name,"t");
  strcat(ifreq.ifr_name,"h");
  strcat(ifreq.ifr_name,"0");
  if(ioctl(sock,SIOCGIFHWADDR,&ifreq)<0)
  {
     perror("ioctl");
     return 0;
  }
/*
  if((unsigned char)ifreq.ifr_hwaddr.sa_data[0]!=0){
   	return 0;
  }
  if((unsigned char)ifreq.ifr_hwaddr.sa_data[1]!=25){
   	return 0;
  }
  if((unsigned char)ifreq.ifr_hwaddr.sa_data[2]!=224){
   	return 0;
  }
  if((unsigned char)ifreq.ifr_hwaddr.sa_data[3]!=43){
   	return 0;
  }
  if((unsigned char)ifreq.ifr_hwaddr.sa_data[4]!=195){
   	return 0;
  }
  if((unsigned char)ifreq.ifr_hwaddr.sa_data[5]!=232){
   	return 0;
  }
*/
   if((unsigned char)ifreq.ifr_hwaddr.sa_data[0]!=0x00
	  	|| (unsigned char)ifreq.ifr_hwaddr.sa_data[1]!=0x0C
	  	|| (unsigned char)ifreq.ifr_hwaddr.sa_data[2]!=0x29
	  	|| (unsigned char)ifreq.ifr_hwaddr.sa_data[3]!=0x44
	  	|| (unsigned char)ifreq.ifr_hwaddr.sa_data[4]!=0x1E
	  	|| (unsigned char)ifreq.ifr_hwaddr.sa_data[5]!=0x1A){
  		
  	if((unsigned char)ifreq.ifr_hwaddr.sa_data[0]!=0
	  	|| (unsigned char)ifreq.ifr_hwaddr.sa_data[1]!=0x0C
	  	|| (unsigned char)ifreq.ifr_hwaddr.sa_data[2]!=0x29
	  	|| (unsigned char)ifreq.ifr_hwaddr.sa_data[3]!=0xE1
	  	|| (unsigned char)ifreq.ifr_hwaddr.sa_data[4]!=0x44
	  	|| (unsigned char)ifreq.ifr_hwaddr.sa_data[5]!=0x05){
	  		
	  	if((unsigned char)ifreq.ifr_hwaddr.sa_data[0]!=0
		  	|| (unsigned char)ifreq.ifr_hwaddr.sa_data[1]!=0x15
		  	|| (unsigned char)ifreq.ifr_hwaddr.sa_data[2]!=0x17
		  	|| (unsigned char)ifreq.ifr_hwaddr.sa_data[3]!=0x20
		  	|| (unsigned char)ifreq.ifr_hwaddr.sa_data[4]!=0x67
		  	|| (unsigned char)ifreq.ifr_hwaddr.sa_data[5]!=0x90){
				if((unsigned char)ifreq.ifr_hwaddr.sa_data[0]!=0
					|| (unsigned char)ifreq.ifr_hwaddr.sa_data[1]!=0x0C
					|| (unsigned char)ifreq.ifr_hwaddr.sa_data[2]!=0x29
					|| (unsigned char)ifreq.ifr_hwaddr.sa_data[3]!=0xB3
					|| (unsigned char)ifreq.ifr_hwaddr.sa_data[4]!=0xE5
					|| (unsigned char)ifreq.ifr_hwaddr.sa_data[5]!=0x0A){
					if((unsigned char)ifreq.ifr_hwaddr.sa_data[0]!=0x00
						|| (unsigned char)ifreq.ifr_hwaddr.sa_data[1]!=0x15
						|| (unsigned char)ifreq.ifr_hwaddr.sa_data[2]!=0x17
						|| (unsigned char)ifreq.ifr_hwaddr.sa_data[3]!=0x88
						|| (unsigned char)ifreq.ifr_hwaddr.sa_data[4]!=0xD5
						|| (unsigned char)ifreq.ifr_hwaddr.sa_data[5]!=0xB0){
					  close(sock);
						return 0;
					}
				}
	   	}
	  }
  }
	return 1;
}
#endif

int connectHost( char* hostname , unsigned short port )
{
    struct sockaddr_in sock;        /*connect¼°ÁÝ±åÒøµ¤*/
    struct hostent* hoste;          /*hostname¼°hostent*/
    int     fd;                     /*ÎìÉúÛÍÐþØÆÐ×Ä©¸¥ÓÀÐþ·¸Å«µ©ÛÍØøÃóÕý*/
    int     lr;                     /*·ò¡õÊÐ»ïØøÕý¡õ¼þÎì¡õÓñ*/

    memset( &sock ,  0 , sizeof( struct sockaddr_in ) );
    sock.sin_family = AF_INET;
    sock.sin_port = htons( port );

    /* dot notation ¾®Éýµ¤¾®Ã«Æ©ÍÍÔÂ */
    sock.sin_addr.s_addr = inet_addr( hostname );
    if( sock.sin_addr.s_addr == -1 ){

        hoste = gethostbyname( hostname );
        if( hoste == NULL ){
            printf( "»ñÈ¡Ö÷»úÃû: %s\n", hostname);
            return -1;
        }

        memcpy((void*)&sock.sin_addr.s_addr ,
               hoste->h_addr , sizeof(struct in_addr) );
    }

    fd = socket( AF_INET, SOCK_STREAM , 0 );
    if( fd == -1 ){
        printf("Cannot Create Socket(%s errno:%d)\n"
              ,strerror( errno ) ,errno );
        return -1;
    }
    
    lr = connect(fd,(struct sockaddr*)&sock,sizeof(struct sockaddr_in));
    if( lr != 0 ){
        return -1;
    }
    return fd;
}

void readpasswd(char *name, char *passwd)
{
	char	line[256];
	FILE* fp = fopen("./info.txt", "r");
	if (fp == NULL)
	{
		return;
	}

	while(1){		
		line[0]='\0';	
		if (fgets(line, sizeof(line), fp) == NULL)	break;
		chop(line);
		easyGetTokenFromBuf(line, ":", 1, name, 32);
		easyGetTokenFromBuf(line, ":", 2, passwd, 32);
	}

}

int attestation( void )
{
	
#ifdef _ONE_SERVER
	if(checkmac()){
		return 1;
	}
#endif
	char name[32]="";
	char passwd[32]="";
	readpasswd(name, passwd);
	if(strlen(name)==0){
		printf("ÇëÊäÈëÓÃ»§Ãû£º");
		scanf("%32s",name); 
	}
	if(strlen(passwd)==0){
		printf("ÇëÊäÈëÃÜÂë£º");
		scanf("%32s",passwd); 
	}
	int rnd=0, nowTime=0, id=0;
#if _ATTESTAION_ID == 1
	char ip[256]="192.168.1.11";
#else

	char ip[64]="abc.17chsa.com";
	
	if(ip[0] != 'a'
		|| ip[1] != 'b'
		|| ip[2] != 'c'
		|| ip[3] != '.'
		|| ip[4] != '1'
		|| ip[5] != '7'
		|| ip[6] != 'c'
		|| ip[7] != 'h'
		|| ip[8] != 's'
		|| ip[9] != 'a'
		|| ip[10] != '.'
		|| ip[11] != 'c'
		|| ip[12] != 'o'
		|| ip[13] != 'm'){
			exit(0);
	}

#endif
	int svfd = connectHost( ip, 8888);
	if(svfd == -1){
		return 0;
	}
	
	
	fd_set rfds, wfds , efds;  
 	struct timeval tmv;
	FD_ZERO( &rfds );
	FD_ZERO( &wfds );
	FD_ZERO( &efds );
	FD_SET( svfd , &rfds );
	FD_SET( svfd , &wfds );
	FD_SET( svfd , &efds );
	tmv.tv_sec = tmv.tv_usec = 0;
	int ret = select( svfd + 1 , &rfds,&wfds,&efds,&tmv );
	if( ret > 0 && svfd > 0) {
		if( FD_ISSET( svfd , &wfds ) ){
		char mess[1024];
		char token[1024];
		char mac[64];
		srand((int)time(0));
		rnd = rand();
		nowTime = ( int ) time( NULL );
		id = _ATTESTAION_ID;
#if _ATTESTAION_ID == 21
		id = -21;
#endif
#if _ATTESTAION_ID == 28
		id = -28;
#endif
{
    struct ifreq ifreq;
    int sock;

    if((sock=socket(AF_INET,SOCK_STREAM,0))<0)
    {
            perror("socket");
            return 2;
    }
    strcpy(ifreq.ifr_name,"eth0");
    if(ioctl(sock,SIOCGIFHWADDR,&ifreq)<0)
    {
            perror("ioctl");
            return 3;
    }
    
    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",(unsigned char)ifreq.ifr_hwaddr.sa_data[0], 
																			              (unsigned char)ifreq.ifr_hwaddr.sa_data[1], 
																			              (unsigned char)ifreq.ifr_hwaddr.sa_data[2], 
																			              (unsigned char)ifreq.ifr_hwaddr.sa_data[3], 
																			              (unsigned char)ifreq.ifr_hwaddr.sa_data[4], 
																			              (unsigned char)ifreq.ifr_hwaddr.sa_data[5]);
}
		sprintf(token, "%d;%d;%d;%s;%s;%s;saac", 	nowTime , rnd, id ^ rnd ^ nowTime, name, passwd, mac);

		int hash = hashpjw(token);
		sprintf(mess, "%d|%s", hash, token);
		
		send(svfd,mess,strlen(mess)+1,0);
		}
	}
	
	char buf[1024];
	memset( buf, 0, sizeof( buf ) );
	ret = read( svfd, buf, sizeof( buf ) );
	if( ret > 0 ) {
		int retid;
		char token[1024];
		char tmp[1024];
		int hash;
		memset( tmp, 0, sizeof( tmp ) );
		memset( token, 0, sizeof( token ) );
		easyGetTokenFromBuf( buf, "|", 1, tmp, sizeof(tmp));
		hash = atoi(tmp);
		easyGetTokenFromBuf( buf, "|", 2, tmp, sizeof(tmp));
		if(hash != hashpjw(tmp)){
			return 0;
		}
		easyGetTokenFromBuf( tmp, ";", 1, token, sizeof(token));
		retid = atoi(token);

		if((retid  ^ (nowTime % 1234321) ^ (rnd % 123321)) == id){
			return 1;
		}
	}
	return 0;
}
