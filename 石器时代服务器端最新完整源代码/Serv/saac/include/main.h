#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <time.h>
#include <sys/time.h>

#include <errno.h>
#include <string.h>
#include "version.h"

#ifdef _MAIN_C_
#define EXT
#else
#define EXT extern
#endif

#define SA_NODEFER	0x40000000
#define SA_NOMASK	SA_NODEFER

#define CHARDATASIZE ( 1024 * 1024 )

char *chartime();
#define LOGBASE( filename , format , args...)\
{\
    FILE *f;\
    f = fopen( filename , "a" );\
    if( f != NULL ){\
        fprintf( f , format , ## args);\
        fclose(f);\
    } else{ fprintf( stderr,"fuckup! log fail!:%s:%s\n", filename,strerror(errno));}\
}

// Spock 2000/10/27
#define log(format, args...)    \
{\
    fprintf( stderr, format , ## args );\
}
// Spock end
#define SUCCESSFUL "successful"
#define FAILED "failed"

#define MAXCONNECTION 15
#define USERID_MAX 32
#define CHARNAME_MAX 32

EXT char svpass[64];
EXT char chardir[64];
EXT char logdir[64];
EXT char dbdir[64];
EXT char maildir[64];

// CoolFish: Family 2001/5/9
EXT char familydir[64];
EXT char fmpointdir[64];
EXT char fmsmemodir[64];

EXT int sameipmun;

#ifdef	_OLDPS_TO_MD5PS	
EXT int usemd5;
#endif

EXT int log_rotate_interval;
EXT int total_ok_charlist, total_ng_charlist;

#ifdef _AUTO_BACKUP
EXT int autobackupday;
EXT int autobackuphour;
#endif

#ifdef _LOTTERY_SYSTEM
EXT int lotterysystem;
#endif

#ifdef _SLEEP_CHAR
EXT char sleepchardir[64];
#endif

typedef struct _gmsv
{
    int use;
	  int fd;
    char name[128];
} gmsv;

int get_rotate_count(void );
void checkGSUCheck( char *id );
int logout_game_server( int ti );
int is_game_server_login( int ti );
char * getGSName( int i );

void gmsvBroadcast( int fd, char *p1, char *p2, char *p3 , int flag );
#if _ATTESTAION_ID == 1

int login_game_server( int ti, int id, char *svname , char *svpas ,
               char *result , int resultlen ,
               char *retdata , int retdatalen );
#else
int login_game_server( int ti , char *svname , char *svpas ,
               char *result , int resultlen ,
               char *retdata , int retdatalen );
#endif

#ifdef _ANGEL_SUMMON
void delMissionTableOnedata( int index);

typedef enum
{
	MISSION_NONE =0,
	MISSION_WAIT_ANSWER,
	MISSION_DOING,
	MISSION_HERO_COMPLETE,
	MISSION_TIMEOVER,
}ANGEL_MISSIONFLAG;
#endif


#undef EXT
#endif





