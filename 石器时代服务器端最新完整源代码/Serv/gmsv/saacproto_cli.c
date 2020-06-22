#include "version.h"
#define _SAACPROTOCLI_C_
#include "version.h"
#include "saacproto_cli.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "common.h"
#include "net.h"
#include "lssproto_serv.h"
#ifdef _SEND_EFFECT	   	        // WON ADD AC送下雪、下雨等特效
#include "chatmagic.h"
#endif

#define IS_2BYTEWORD( _a_ ) ( (char)(0x80) <= (_a_) && (_a_) <= (char)(0xFF) )

/*
90 :   <LI>  <a name="ACServerLoginC"><font color=red>clienttoserver ACServerLogin(string servername,string serverpas);<br></font></a>
91 :      必□丞扔□田□互失市它件玄扔□田□卞夫弘奶件允月午五卞银丹［
92 :      涛粮毛舰匀凶丐午｝仇及夫弘奶件毛仄卅中午允屯化及扔□申旦毛仄卅中［
93 :      <br><br>
94 :      <dl>
95 :        <dt>string servername
96 :        <dd>扔□田□  ［acserv.cf卞涩烂今木化中月［
97 :        <dt>string serverpas
98 :        <dd>扔□田□由旦伐□玉［acserv.cf卞涩烂今木化中月［
99 :      </dl>
100 :   
*/

#ifdef _ALLDOMAN // (不可开) Syu ADD 排行榜NPC
void saacproto_UpdataStele_send( int fd , char *cdkey , char *name , char *title , int level , int trns , int time , int floor) 
{
	saacproto_CreateHeader( saacproto.work , "UpdataStele" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( cdkey ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( name ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( title ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( level ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( trns ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( time ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( floor ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}
#endif

#if _ATTESTAION_ID == 1
void saacproto_ACServerLogin_send( int fd, int id, char* servername,char* serverpas )
#else
void saacproto_ACServerLogin_send( int fd,char* servername,char* serverpas )
#endif
{
	saacproto_CreateHeader( saacproto.work , "ACServerLogin" );

#if _ATTESTAION_ID == 1
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( id ) ,saacproto.workbufsize );
#endif
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( servername ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( serverpas ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}
/*
114 : <LI><a name="ACServerLogoutC"><font color=red>clienttoserver ACServerLogout(void);<br></font></a>
115 :      扔□田□互失市它件玄扔□田□井日夫弘失它玄允月午五卞银丹［
116 :      涛粮反扔□田卞方匀化  澎卞濠日木月［仄凶互匀化仇木卞覆允月失市它件玄扔□田
117 :      及忒蚕反戊生弁扑亦件毛濠月仪匹丐月［
118 :      <br>
119 :      <br>
120 :      <br>
121 : 
122 : 
*/

void saacproto_ACServerLogout_send( int fd )
{
	saacproto_CreateHeader( saacproto.work , "ACServerLogout" );
	saacproto_strcatsafe( saacproto.work , "" ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}

#ifdef _PKSEVER_VER
void saacproto_ACCharList_send( int fd,char* id,char* pas,int mesgid, int star)
#else
void saacproto_ACCharList_send( int fd,char* id,char* pas,char* ip,char* mac,int mesgid,int charlistflg )
#endif
{
	saacproto_CreateHeader( saacproto.work , "ACCharList" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( pas ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( ip ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( mac ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( mesgid ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( charlistflg ) ,saacproto.workbufsize );

#ifdef _PKSEVER_VER
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( star ) ,saacproto.workbufsize );
#endif
	saacproto_Send( fd , saacproto.work );
}

void saacproto_ACCharLoad_send( int fd,char* id,char* pas,char* charname,int lock,char* opt,int mesgid )
{
	saacproto_CreateHeader( saacproto.work , "ACCharLoad" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( pas ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charname ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( lock ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( opt ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( mesgid ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}

#ifdef _NEWSAVE
void saacproto_ACCharSave_send( int fd,char* id,char* charname,char* opt,char* charinfo,int unlock,int mesgid,int saveindex )
#else
void saacproto_ACCharSave_send( int fd,char* id,char* charname,char* opt,char* charinfo,int unlock,int mesgid )
#endif
{
	saacproto_CreateHeader( saacproto.work , "ACCharSave" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charname ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( opt ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charinfo ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( unlock ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( mesgid ) ,saacproto.workbufsize );
#ifdef _NEWSAVE
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( saveindex ) ,saacproto.workbufsize );
#endif
	saacproto_Send( fd , saacproto.work );
}

void saacproto_ACCharDelete_send( int fd,char* id,char* passwd,char* charname,char* option,int mesgid )
{
	saacproto_CreateHeader( saacproto.work , "ACCharDelete" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( passwd ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charname ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( option ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( mesgid ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}

void saacproto_ACLock_send( int fd,char* id,int lock,int mesgid )
{
	saacproto_CreateHeader( saacproto.work , "ACLock" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( lock ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( mesgid ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}

void saacproto_ACUCheck_send( int fd,char* mem_id,int status )
{
	saacproto_CreateHeader( saacproto.work , "ACUCheck" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( mem_id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( status ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}

void saacproto_DBUpdateEntryString_send( int fd,char* table,char* key,char* value,int msgid,int msgid2 )
{
	saacproto_CreateHeader( saacproto.work , "DBUpdateEntryString" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( table ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( key ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( value ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid2 ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}

void saacproto_DBDeleteEntryString_send( int fd,char* table,char* key,int msgid,int msgid2 )
{
	saacproto_CreateHeader( saacproto.work , "DBDeleteEntryString" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( table ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( key ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid2 ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}

void saacproto_DBGetEntryString_send( int fd,char* table,char* key,int msgid,int msgid2 )
{
	saacproto_CreateHeader( saacproto.work , "DBGetEntryString" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( table ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( key ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid2 ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}

void saacproto_DBUpdateEntryInt_send( int fd,char* table,char* key,int value,char* info,int msgid,int msgid2 )
{
	saacproto_CreateHeader( saacproto.work , "DBUpdateEntryInt" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( table ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( key ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( value ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( info ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid2 ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}

void saacproto_DBGetEntryRank_send( int fd,char* table,char* key,int msgid,int msgid2 )
{
	saacproto_CreateHeader( saacproto.work , "DBGetEntryRank" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( table ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( key ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid2 ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}

void saacproto_DBDeleteEntryInt_send( int fd,char* table,char* key,int msgid,int msgid2 )
{
	saacproto_CreateHeader( saacproto.work , "DBDeleteEntryInt" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( table ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( key ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid2 ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}

void saacproto_DBGetEntryInt_send( int fd,char* table,char* key,int msgid,int msgid2 )
{
	saacproto_CreateHeader( saacproto.work , "DBGetEntryInt" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( table ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( key ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid2 ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}

void saacproto_DBGetEntryByRank_send( int fd,char* table,int rank_start,int rank_end,int msgid,int msgid2 )
{
	saacproto_CreateHeader( saacproto.work , "DBGetEntryByRank" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( table ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( rank_start ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( rank_end ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid2 ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}

void saacproto_DBGetEntryByCount_send( int fd,char* table,int count_start,int num,int msgid,int msgid2 )
{
	saacproto_CreateHeader( saacproto.work , "DBGetEntryByCount" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( table ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( count_start ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( num ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( msgid2 ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}
/*
608 :  <LI><a name="Broadcast"><font color=red>clienttoserver Broadcast( string id, string charname, string message , int flag );</font></a><BR>
609 : 	 必□丞扔□田□卞覆仄化仇及戊穴件玉毛霜耨允月午｝
610 : 	 允屯化及必□丞扔□田□卞凶中仄化  元丢永本□斥互霜耨今木月［
611 : 	 id , charname 反方仁勾井歹木月及匹｝蜊  及娄醒毛手勾方丹卞仄凶［
612 : 	 中切中切  侬  由□旦及伙□民件毛踏井卅仁化手中中方丹卞［
613 : 	 <dl>
614 : 	   <dt>string id
615 : 	   <dd>user ID
616 : 	   <dt>string charname
617 : 	   <dd>平乓仿  (巨旦弗□皿卅仄)
618 : 	   <dt>string message
619 : 	   <dd>  霜今木月丢永本□斥［必□丞扔□田□互驯五卞银丹［
620 : 	 </dl>
*/

void saacproto_Broadcast_send( int fd,char* id,char* charname,char* message,int flag )
{
/*
	saacproto_CreateHeader( saacproto.work , "Broadcast" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charname ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( message ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( flag ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
*/
}
/*
634 :  <LI><a name="Message"><font color=red>clienttoserver Message( string id_from, string charname_from, string id_to, string charname_to, string message , int option );</font></a><BR>
635 : 	 奶件旦正件玄丢永本□斥毛霜耨允月
636 : 	 <dl>
637 : 	   <dt>string id_from
638 : 	   <dd>from ID
639 : 	   <dt>string charname_from
640 : 	   <dd>from Character name
641 : 	   <dt>string id_to
642 : 	   <dd>to ID
643 : 	   <dt>string charname_to
644 : 	   <dd>to character name
645 : 	   <dt>string message
646 : 	   <dd>丢永本□斥
647 : 	   <dt>int option
648 : 	   <dd>左皿扑亦件［民乓永玄及缙午井中欠中欠  木木月［漆  及傀舰迕
649 : 		   仇木毛升丹银丹井反必□丞扔□田□及  澎匹｝失市它件玄扔□田□反
650 : 		   域濠仇及    卞正永民仄卅中
651 : 	 </dL>
*/

void saacproto_Message_send( int fd,char* id_from,char* charname_from,char* id_to,char* charname_to,char* message,int option )
{
	saacproto_CreateHeader( saacproto.work , "Message" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id_from ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charname_from ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id_to ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charname_to ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( message ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( option ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}

void saacproto_MessageAck_send( int fd,char* id,char* charname,char* result,int mesgid )
{
	saacproto_CreateHeader( saacproto.work , "MessageAck" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charname ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( result ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( mesgid ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}

void saacproto_MessageFlush_send( int fd,char* id,char* charname )
{
	saacproto_CreateHeader( saacproto.work , "MessageFlush" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charname ) ,saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}

/************************************************************
 * CoolFish: Family 2001/5/18
 ***********************************************************/
#ifdef _PERSONAL_FAME
void saacproto_ACAddFM_send(int fd, char *fmname, char *fmleadername,
	char *fmleaderid, int fmleaderlv, char *petname, char *petattr,
	char *fmrule, int fmsprite, int fmleadergrano, int fame,
#ifdef _FAMILYBADGE_
	int	fmbadge,
#endif
	int charfdid)
#else
void saacproto_ACAddFM_send(int fd, char *fmname, char *fmleadername,
	char *fmleaderid, int fmleaderlv, char *petname, char *petattr,
	char *fmrule, int fmsprite, int fmleadergrano, int charfdid)
#endif
{
	saacproto_CreateHeader(saacproto.work, "ACAddFM");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(fmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(fmleadername), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(fmleaderid), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmleaderlv), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(petname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(petattr), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(fmrule), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmsprite), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmleadergrano), saacproto.workbufsize);
// #ifdef _PERSONAL_FAME
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fame), saacproto.workbufsize);
// #endif
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charfdid), saacproto.workbufsize);
#ifdef _FAMILYBADGE_
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmbadge), saacproto.workbufsize);
#endif
	saacproto_Send(fd, saacproto.work);
}
#ifdef _PERSONAL_FAME	// Arminius: 家族个人声望
void saacproto_ACJoinFM_send(int fd, char *fmname, int fmindex, char *charname,
        char *charid, int charlv, int index, int fame, int charfdid)
#else
void saacproto_ACJoinFM_send(int fd, char *fmname, int fmindex, char *charname,
        char *charid, int charlv, int index, int charfdid)
#endif
{
	saacproto_CreateHeader(saacproto.work, "ACJoinFM");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(fmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(charname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(charid), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charlv), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(index), saacproto.workbufsize);
// #ifdef _PERSONAL_FAME	// Arminius: 家族个人声望
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fame), saacproto.workbufsize);
// #endif
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charfdid), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);	
}
void saacproto_ACLeaveFM_send(int fd, char *fmname, int fmindex, char *charname,
	char *charid, int index, int charfdid)
{
	saacproto_CreateHeader(saacproto.work, "ACLeaveFM");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(fmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(charname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(charid), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(index), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charfdid), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);	
}
void saacproto_ACDelFM_send(int fd, char *fmname, int fmindex, int index,
	char *charname, char *charid, int charfdid)
{
	saacproto_CreateHeader(saacproto.work, "ACDelFM");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(fmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(index), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(charname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(charid), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charfdid), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);
}

void saacproto_ACShowFMList_send(int fd)
{
	saacproto_CreateHeader(saacproto.work, "ACShowFMList");
	saacproto_Send(fd, saacproto.work);
}

void saacproto_ACShowMemberList_send(int fd, int index)
{
	saacproto_CreateHeader(saacproto.work, "ACShowMemberList");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string("MemberList"), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(index), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);
}
void saacproto_ACFMDetail_send(int fd, char *fmname, int fmindex, int index, int charfdid)
{
	saacproto_CreateHeader(saacproto.work, "ACFMDetail");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(fmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(index), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charfdid), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);
}
#ifdef _FMVER21
void saacproto_ACMemberJoinFM_send(int fd, char *fmname, int fmindex,
	char *charname, int charindex, int index, int result, int meindex,
	int charfdid)
#else
void saacproto_ACMemberJoinFM_send(int fd, char *fmname, int fmindex,
	char *charname, int charindex, int index, int result, int charfdid)
#endif
{
	saacproto_CreateHeader(saacproto.work, "ACMemberJoinFM");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(fmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(charname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(index), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(result), saacproto.workbufsize);
#ifdef _FMVER21
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(meindex), saacproto.workbufsize);
#endif
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charfdid), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);	
}
#ifdef _FMVER21
void saacproto_ACMemberLeaveFM_send(int fd, char *fmname, int fmindex,
	char *charname, int charindex, int index, int meindex, int charfdid)
#else
void saacproto_ACMemberLeaveFM_send(int fd, char *fmname, int fmindex,
	char *charname, int charindex, int index, int charfdid)
#endif
{
	saacproto_CreateHeader(saacproto.work, "ACMemberLeaveFM");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(fmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(charname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(index), saacproto.workbufsize);
#ifdef _FMVER21
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(meindex), saacproto.workbufsize);
#endif	
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charfdid), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);	
}
#ifdef _FM_MODIFY
void saacproto_ACFMCharLogin_send(int fd, char *fmname, int fmindex, char *charname,
		      char *charid, int charlv, int eventflag, int charfdid,int gsnum)
#else
	#ifdef _FMVER21
	void saacproto_ACFMCharLogin_send(int fd, char *fmname, int fmindex, char *charname,
		      char *charid, int charlv, int eventflag, int charfdid)
	#else
	void saacproto_ACFMCharLogin_send(int fd, char *fmname, int fmindex, char *charname,
		      char *charid, int charlv, int charfdid)
	#endif
#endif
{
	saacproto_CreateHeader(saacproto.work, "ACFMCharLogin");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(fmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(charname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(charid), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charlv), saacproto.workbufsize);
#ifdef _FMVER21
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(eventflag), saacproto.workbufsize);
#endif
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charfdid), saacproto.workbufsize);
#ifdef _FM_MODIFY
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(gsnum), saacproto.workbufsize);
#endif
	saacproto_Send(fd, saacproto.work);	
}

void saacproto_ACFMCharLogout_send(int fd, char *fmname, int fmindex, char *charname,
	char *charid, int charlv, int index, int charfdid)
{
	saacproto_CreateHeader(saacproto.work, "ACFMCharLogout");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(fmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(charname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(charid), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charlv), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(index), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charfdid), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);	
}
void saacproto_ACFMReadMemo_send(int fd, int index)
{
	saacproto_CreateHeader(saacproto.work, "ACFMReadMemo");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string("ReadMemo"), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(index), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);
}
void saacproto_ACFMWriteMemo_send(int fd, char *fmname, int fmindex,
        char *data, int index)
{
	saacproto_CreateHeader(saacproto.work, "ACFMWriteMemo");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(fmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(data), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(index), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);
}        
void saacproto_ACFMPointList_send(int fd)
{
	saacproto_CreateHeader(saacproto.work, "ACFMPointList");
	saacproto_Send(fd, saacproto.work);
}

void saacproto_ACSetFMPoint_send(int fd, char *fmname, int fmindex, int index,
	int fmpointindex, int fl, int x, int y, int charfdid)
{
	saacproto_CreateHeader(saacproto.work, "ACSetFMPoint");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(fmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(index), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmpointindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fl), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(x), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(y), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charfdid), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);
}
void saacproto_ACFixFMPoint_send(int fd, char *winfmname, int winfmindex,
        int winindex, char *losefmname, int losefmindex, int loseindex,
        int village)
{
	saacproto_CreateHeader(saacproto.work, "ACFixFMPoint");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(winfmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(winfmindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(winindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(losefmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(losefmindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(loseindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(village), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);
}
void saacproto_ACFMAnnounce_send(int fd, char *fmname, int fmindex, int index,
        char *data, int color)
{
	saacproto_CreateHeader(saacproto.work, "ACFMAnnounce");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(fmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(index), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(data), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(color), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);
}
void saacproto_ACShowTopFMList_send(int fd, int kindflag)
{
	saacproto_CreateHeader(saacproto.work, "ACShowTopFMList");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string("ShowTopFMList"), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(kindflag), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);
}

void saacproto_ACFixFMData_send(int fd, char *fmname, int fmindex, int index,
	int kindflag, char *data1, char *data2, int charindex, int charfdid)
{
	saacproto_CreateHeader(saacproto.work, "ACFixFMData");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(fmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(index), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(kindflag), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(data1), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(data2), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charfdid), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);
}
void saacproto_ACFixFMPK_send(int fd, char *winfmname, int winfmindex,
        int winindex, char *losefmname, int losefmindex, int loseindex)
{
	saacproto_CreateHeader(saacproto.work, "ACFixFMPK");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(winfmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(winfmindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(winindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(losefmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(losefmindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(loseindex), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);
}
void saacproto_ACGMFixFMData_send(int fd, int index, char *charid, char *cmd,
	char *data, int charfdid)
{
	saacproto_CreateHeader(saacproto.work, "ACGMFixFMData");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string("ACGMFixFMData"), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(index), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(charid), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(cmd), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(data), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charfdid), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);
}
void saacproto_ACGetFMData_send(int fd, char *fmname, int fmindex, int index,
	int kindflag, int charfdid)
{
	saacproto_CreateHeader(saacproto.work, "ACGetFMData");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(fmname), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(index), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(kindflag), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(charfdid), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);
}

// Arminius 7.26 manor pk acknowledge
void saacproto_ACManorPKAck_send(int fd, char *data)
{
	saacproto_CreateHeader(saacproto.work, "ACManorPKAck");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(data), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);
}

void saacproto_ACreLoadFmData_send( int fd, int type, int data)
{
	saacproto_CreateHeader( saacproto.work, "ACreLoadFmData");
	saacproto_strcatsafe( saacproto.work, saacproto_mkstr_int( type), saacproto.workbufsize);
	saacproto_strcatsafe( saacproto.work, saacproto_mkstr_int( data), saacproto.workbufsize);
	saacproto_Send( fd, saacproto.work);
}

#ifdef _ACFMPK_LIST
void saacproto_ACLoadFmPk_send(int fd, int fmpks_pos)
{
	saacproto_CreateHeader(saacproto.work, "ACLoadFmPk");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmpks_pos), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);
}


void saacproto_ACSendFmPk_send(int fd, int toindex, int PkFlg, int fmpks_pos, char *msg)
{
	saacproto_CreateHeader(saacproto.work, "ACSendFmPk");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(fmpks_pos), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int( toindex), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int( PkFlg), saacproto.workbufsize);
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(msg), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);
}
#endif

void saacproto_ACAuctionSold_send(int fd, char *data)
{
	saacproto_CreateHeader(saacproto.work, "ACAuctionSold");
	saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(data), saacproto.workbufsize);
	saacproto_Send(fd, saacproto.work);
}


extern char saacretfunc[255];
#ifdef _ABSOLUTE_DEBUG
extern int lastfunctime;
#endif
int saacproto_ClientDispatchMessage(int fd ,char*line)
{
	int msgid;
	char funcname[255];
	saacproto_strcpysafe( saacproto.work , line, saacproto.workbufsize );
	saacproto_splitString( saacproto.work);
	saacproto_GetMessageInfo( &msgid , funcname , sizeof(funcname),saacproto.token_list);
	strcpy( saacretfunc , funcname );
#ifdef _ABSOLUTE_DEBUG
	lastfunctime=1;
#endif
	if( strcmp( funcname , "ACGmsvDownRequest" ) == 0 ){
		int min;
		min = saacproto_demkstr_int( saacproto.token_list[2] );
		saacproto_ACGmsvDownRequest_recv( fd,min);
		return 0;
	}

	if( strcmp( funcname , "ACServerLogin" ) == 0 ){
		char* result;
		char* data;
		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		data = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		saacproto_ACServerLogin_recv( fd,result,data);
		return 0;
	}

	if( strcmp( funcname , "ACCharList" ) == 0 ){
		char* result;
		char* output;
		int id;
		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		output = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		id = saacproto_demkstr_int( saacproto.token_list[4] );
		saacproto_ACCharList_recv( fd,result,output,id);
		return 0;
	}

	if( strcmp( funcname , "ACCharLoad" ) == 0 ){
		char* result=NULL;
		char* data=NULL;
		int id;
#ifdef _NEWSAVE
		int saveindex;
#endif
		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		data = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		id = saacproto_demkstr_int( saacproto.token_list[4] );
		if( data == NULL ) return 0;
		if( result == NULL ) return 0;

#ifdef _NEWSAVE
		saveindex = saacproto_demkstr_int( saacproto.token_list[5] );
		saacproto_ACCharLoad_recv( fd,result,data,id,saveindex);
#else
		saacproto_ACCharLoad_recv( fd,result,data,id, -1);
#endif
		return 0;
	}

	if( strcmp( funcname , "ACCharSave" ) == 0 ){
		char* result;
		char* data;
		int id;
		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		data = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		id = saacproto_demkstr_int( saacproto.token_list[4] );
		saacproto_ACCharSave_recv( fd,result,data,id);
		return 0;
	}

	if( strcmp( funcname , "ACCharDelete" ) == 0 ){
		char* result;
		char* data;
		int id;
		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		data = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		id = saacproto_demkstr_int( saacproto.token_list[4] );
		saacproto_ACCharDelete_recv( fd,result,data,id);
		return 0;
	}

	if( strcmp( funcname , "ACLock" ) == 0 ){
		char* result;
		char* data;
		int id;
		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		data = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		id = saacproto_demkstr_int( saacproto.token_list[4] );
		saacproto_ACLock_recv( fd,result,data,id);
		return 0;
	}

#ifdef _WAEI_KICK
	if( strcmp( funcname , "ACKick" ) == 0 ){
		int act;
		char* data;
		int id;
		act = saacproto_demkstr_int( saacproto.token_list[2] );
		data = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		id = saacproto_demkstr_int( saacproto.token_list[4] );
		saacproto_ACKick_recv( fd, act, data, id);
		return 0;
	}
#endif

#ifdef _CHAR_POOLITEM

	if( strcmp( funcname , "ACSavePoolItem" ) == 0 ){
		char* result;
		char* data;
		int id;

		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		data = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		id = saacproto_demkstr_int( saacproto.token_list[4] );
		saacproto_ACCharSavePoolItem_recv( fd, result, data, id);
		return 0;
	}

	if( strcmp( funcname , "ACGetPoolItem" ) == 0 ){
		char* result;
		char* data;
		int id, npcid;

		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		data = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		id = saacproto_demkstr_int( saacproto.token_list[4] );
		npcid = saacproto_demkstr_int( saacproto.token_list[5] );
		saacproto_ACCharGetPoolItem_recv( fd, result, data, id, npcid);
		return 0;
	}
#endif


#ifdef _CHAR_POOLPET

	if( strcmp( funcname , "ACSavePoolPet" ) == 0 ){
		char* result;
		char* data;
		int id;

		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		data = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		id = saacproto_demkstr_int( saacproto.token_list[4] );
		saacproto_ACCharSavePoolPet_recv( fd, result, data, id);
		return 0;
	}

	if( strcmp( funcname , "ACGetPoolPet" ) == 0 ){
		char* result;
		char* data;
		int id, npcid;

		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		data = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		id = saacproto_demkstr_int( saacproto.token_list[4] );
		npcid = saacproto_demkstr_int( saacproto.token_list[5] );
		saacproto_ACCharGetPoolPet_recv( fd, result, data, id, npcid);
		return 0;
	}
#endif

	if( strcmp( funcname , "ACUCheck" ) == 0 ){
		char* mem_id;
		mem_id = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		saacproto_ACUCheck_recv( fd,mem_id);
		return 0;
	}
/*
354 :  <LI><a name="DBUpdateEntryStringS"><font color=blue>servertoclient DBUpdateEntryString( string result, string table, string key, int msgid, int msgid2 );</font></a><BR>
355 : 	 晓及戊穴件玉及瑛绊［撩  仄化手荚汊尕第  卅及匹  骰仄化方中冗
356 : 	 <dl>
357 : 	   <dt>string result
358 : 	   <dD>successful 井failed
359 : 	   <dt>string table
360 : 	   <dD>
361 : 	   <dt>int msgid
362 : 	   <dD>
363 : 	   <dt>int msgid2
364 : 	   <dD>
365 : 	 </dL>
366 : 
*/

	if( strcmp( funcname , "DBUpdateEntryString" ) == 0 ){
		char* result;
		char* table;
		char* key;
		int msgid;
		int msgid2;
		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		table = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		key = saacproto_wrapStringAddr( saacproto_stringwrapper[3] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[4] ));
		msgid = saacproto_demkstr_int( saacproto.token_list[5] );
		msgid2 = saacproto_demkstr_int( saacproto.token_list[6] );
		saacproto_DBUpdateEntryString_recv( fd,result,table,key,msgid,msgid2);
		return 0;
	}
/*
379 :  <LI><a name="DBDeleteEntryStringS"><font color=blue>servertoclient DBDeleteEntryString( string result, string table, string key , int msgid , int msgid2 );</font></a><BR>
380 : 	   侬  巨件玄伉及绰轮及瑛绊
381 : 	 <dl>
382 : 	   <dt>string result
383 : 	   <dd>successful / failed
384 : 	   <dt>string table
385 : 	   <dd>
386 : 	   <dt>string key
387 : 	   <dd>
388 : 	   <dt>int msgid
389 : 	   <dd>
390 : 	   <dt>int msgid2
391 : 	   <dd>		   
392 : 	 </dl>
393 : 	 
*/

	if( strcmp( funcname , "DBDeleteEntryString" ) == 0 ){
		char* result;
		char* table;
		char* key;
		int msgid;
		int msgid2;
		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		table = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		key = saacproto_wrapStringAddr( saacproto_stringwrapper[3] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[4] ));
		msgid = saacproto_demkstr_int( saacproto.token_list[5] );
		msgid2 = saacproto_demkstr_int( saacproto.token_list[6] );
		saacproto_DBDeleteEntryString_recv( fd,result,table,key,msgid,msgid2);
		return 0;
	}
/*
406 :  <Li><a name="DBGetEntryStringS"><font color=blue>servertoclient DBGetEntryString( string result, string value, string table, string key, int msgid, int msgid2 );</font></a><BR>
407 : 	 潸曰分仄及忒曰袄
408 : 	 <dl>
409 : 	   <dt>string result
410 : 	   <dd>successful / failed
411 : 	   <dt>string value
412 : 	   <dd>袄
413 : 	   <dt>string table
414 : 	   <dd>		   
415 : 	   <dt>string key
416 : 	   <dd>
417 : 	   <dt>int msgid
418 : 	   <dd>
419 : 	   <dt>int msgid2
420 : 	   <dd>		   
421 : 	 </dl>
422 : 
*/

	if( strcmp( funcname , "DBGetEntryString" ) == 0 ){
		char* result;
		char* value;
		char* table;
		char* key;
		int msgid;
		int msgid2;
		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		value = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		table = saacproto_wrapStringAddr( saacproto_stringwrapper[3] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[4] ));
		key = saacproto_wrapStringAddr( saacproto_stringwrapper[4] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[5] ));
		msgid = saacproto_demkstr_int( saacproto.token_list[6] );
		msgid2 = saacproto_demkstr_int( saacproto.token_list[7] );
		saacproto_DBGetEntryString_recv( fd,result,value,table,key,msgid,msgid2);
		return 0;
	}
/*
445 :  <LI><a name="DBUpdateEntryIntS"><font color=blue>servertoclient DBUpdateEntryInt( string result , string table, string key, int msgid, int msgid2 );</font></a><BR>
446 : 	 DBUpdateEntry及瑛绊［
447 : 	 <dl>
448 : 	   <dt>string result
449 : 	   <dd>succesful/failed
450 : 	   <dt>string table
451 : 	   <dd>  □皮伙  毛公及引引井尹允
452 : 	   <dt>string key
453 : 	   <dd>平□毛公及引引井尹允
454 : 	   <dt>int msgid, int msgid2
455 : 	   <dd>msgid 毛公及引引忒允［
456 : 	 </dl>
457 : 	 <BR><BR>
458 : 
*/

	if( strcmp( funcname , "DBUpdateEntryInt" ) == 0 ){
		char* result;
		char* table;
		char* key;
		int msgid;
		int msgid2;
		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		table = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		key = saacproto_wrapStringAddr( saacproto_stringwrapper[3] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[4] ));
		msgid = saacproto_demkstr_int( saacproto.token_list[5] );
		msgid2 = saacproto_demkstr_int( saacproto.token_list[6] );
		saacproto_DBUpdateEntryInt_recv( fd,result,table,key,msgid,msgid2);
		return 0;
	}
/*
473 :  <LI><a name="DBGetEntryRankS"><font color=blue>servertoclient DBGetEntryRank( string result , int rank , int count, string table, string key, int msgid, int msgid2 );</font></a><BR>
474 : 	 赐匏毛忒允［
475 : 	 <dl>
476 : 	   <dt>string result
477 : 	   <dd>successful/failed
478 : 	   <dt>int rank
479 : 	   <dd>赐匏［1动晓及袄毛潸月［
480 : 	   <dt>int count
481 : 	   <dd>晓井日中仁勾户井［  赓及巨件玄伉反0井日反元引月［
482 : 	   <dt>string table
483 : 	   <dd>  □皮伙  毛公及引引井尹允
484 : 	   <dt>string key
485 : 	   <dd>平□毛公及引引井尹允
486 : 	   <dt>int msgid, int msgid2
487 : 	   <dd>msgid 毛公及引引忒允
488 : 	 </dl>
489 : 	 <BR><BR>
490 : 
491 : 	 
*/

	if( strcmp( funcname , "DBGetEntryRank" ) == 0 ){
		char* result;
		int rank;
		int count;
		char* table;
		char* key;
		int msgid;
		int msgid2;
		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		rank = saacproto_demkstr_int( saacproto.token_list[3] );
		count = saacproto_demkstr_int( saacproto.token_list[4] );
		table = saacproto_wrapStringAddr( saacproto_stringwrapper[4] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[5] ));
		key = saacproto_wrapStringAddr( saacproto_stringwrapper[5] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[6] ));
		msgid = saacproto_demkstr_int( saacproto.token_list[7] );
		msgid2 = saacproto_demkstr_int( saacproto.token_list[8] );
		saacproto_DBGetEntryRank_recv( fd,result,rank,count,table,key,msgid,msgid2);
		return 0;
	}
/*
504 :  <LI><a name="DBDeleteEntryIntS"><font color=blue>servertoclient DBDeleteEntryInt( string result , string table, string key, int msgid, int msgid2 );</font></a><BR>
505 : 	 绰轮及瑛绊
506 : 	 <dl>
507 : 	   <dt>string result
508 : 	   <dd>successful/failed
509 : 	   <dt>string table
510 : 	   <dd>  □皮伙  毛公及引引井尹允
511 : 	   <dt>string key
512 : 	   <dd>平□毛公及引引井尹允
513 : 	   <dt>int msgid, int msgid2
514 : 	   <dd>msgid 毛公及引引忒允
515 : 	 </dl>
516 : 	 <BR><BR>
517 : 
*/

	if( strcmp( funcname , "DBDeleteEntryInt" ) == 0 ){
		char* result;
		char* table;
		char* key;
		int msgid;
		int msgid2;
		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		table = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		key = saacproto_wrapStringAddr( saacproto_stringwrapper[3] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[4] ));
		msgid = saacproto_demkstr_int( saacproto.token_list[5] );
		msgid2 = saacproto_demkstr_int( saacproto.token_list[6] );
		saacproto_DBDeleteEntryInt_recv( fd,result,table,key,msgid,msgid2);
		return 0;
	}
/*
530 :  <LI><a name="DBGetEntryIntS"><font color=blue>servertoclient DBGetEntryInt( string result , int value , string table, string key, int msgid, int msgid2 );</font></a><BR>
531 : 	 巨件玄伉及袄毛井尹允
532 : 	 <dl>
533 : 	   <dt>string result
534 : 	   <dd>successful/failed
535 : 	   <dt>int value
536 : 	   <dd>袄公及手及
537 : 	   <dt>string table
538 : 	   <dd>  □皮伙  毛公及引引井尹允
539 : 	   <dt>string key
540 : 	   <dd>平□毛公及引引井尹允
541 : 	   <dt>int msgid, int msgid2
542 : 	   <dd>msgid 毛公及引引忒允
543 : 	 </dl>
544 : 
545 : 
546 : 
*/

	if( strcmp( funcname , "DBGetEntryInt" ) == 0 ){
		char* result;
		int value;
		char* table;
		char* key;
		int msgid;
		int msgid2;
		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		value = saacproto_demkstr_int( saacproto.token_list[3] );
		table = saacproto_wrapStringAddr( saacproto_stringwrapper[3] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[4] ));
		key = saacproto_wrapStringAddr( saacproto_stringwrapper[4] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[5] ));
		msgid = saacproto_demkstr_int( saacproto.token_list[6] );
		msgid2 = saacproto_demkstr_int( saacproto.token_list[7] );
		saacproto_DBGetEntryInt_recv( fd,result,value,table,key,msgid,msgid2);
		return 0;
	}
/*
562 :  <LI><a name="DBGetEntryByRankS"><font color=blue>servertoclient DBGetEntryByRank( string result , string list , string table, int msgid, int msgid2 );</font></a><BR>
563 : 	 DBGetEntryByRank及瑛绊［
564 : 	 <dl>
565 : 	   <dt>string result
566 : 	   <dd>
567 : 	   <dt>string list
568 : 	   <dd>"RANK,KEY,SCORE,INFO|RANK,KEY,SCORE,INFO|...."<BR>
569 : 		   RANK反赐匏匹丐月［KEY反公及赐匏卞卅匀化中月旦戊失毛手匀化中月
570 : 		   平□［SCORE反帮醒袄匹旦戊失匹丐月［
571 : 		     ［<br>
572 : 		   "1,RINGO,123465,data1|2,HOGE,4567,data2|2,FUCK,4567,data3"<BR>
573 : 		   仇及  分午｝2匏反2谛中月仇午卞卅月［
574 : 	   <dt>string table
575 : 	   <dd>  □皮伙
576 : 	   <dt>int msgid, int msgid2
577 : 	   <dd>
578 : 	 </dl>
579 : 
580 : 
*/

	if( strcmp( funcname , "DBGetEntryByRank" ) == 0 ){
		char* result;
		char* list;
		char* table;
		int msgid;
		int msgid2;
		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		list = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		table = saacproto_wrapStringAddr( saacproto_stringwrapper[3] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[4] ));
		msgid = saacproto_demkstr_int( saacproto.token_list[5] );
		msgid2 = saacproto_demkstr_int( saacproto.token_list[6] );
		saacproto_DBGetEntryByRank_recv( fd,result,list,table,msgid,msgid2);
		return 0;
	}
/*
593 :  <LI><a name="DBGetEntryByCountS"><font color=blue>servertoclient DBGetEntryByCount( string result , string list , string table, int count_start, int msgid, int msgid2 );</font></a><BR>
594 : 	 DBGetEntryByCount 及瑛绊［
595 : 	 <dl>
596 : 	   <dt>string result
597 : 	   <dd>SUCCESSFUL/FAILED
598 : 	   <dt>string list
599 : 	   <dd>请  伉旦玄［1巨件玄伉丐凶曰 "INDEX,RANK,KEY,SCORE,INFO" 及4蜊及树
600 : 		   及赚匹丐曰｝公木互蜊醒坌 "|" 匹勾卅互月［蜊醒互0及桦宁反
601 : 		   坞  侬  ［
602 : 	   <dt>int num
603 : 	   <dd>中仁勾午曰分允井［1卅日1蜊［2卅日2蜊［
604 : 	   <dt>int msgid ,msgid2
605 : 	   <dd>丢永本□斥ID.
606 : 	 </dL>
607 : 
*/

	if( strcmp( funcname , "DBGetEntryByCount" ) == 0 ){
		char* result;
		char* list;
		char* table;
		int count_start;
		int msgid;
		int msgid2;
		result = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		list = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		table = saacproto_wrapStringAddr( saacproto_stringwrapper[3] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[4] ));
		count_start = saacproto_demkstr_int( saacproto.token_list[5] );
		msgid = saacproto_demkstr_int( saacproto.token_list[6] );
		msgid2 = saacproto_demkstr_int( saacproto.token_list[7] );
		saacproto_DBGetEntryByCount_recv( fd,result,list,table,count_start,msgid,msgid2);
		return 0;
	}
#ifdef _ALLDOMAN // (不可开) Syu ADD 排行榜NPC
	if ( strcmp( funcname , "UpdataStele" ) == 0 ) {
		char* token;
		token = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		saacproto_UpdataStele_recv( fd , token) ;
		return 0;
	}
	if ( strcmp( funcname , "S_UpdataStele" ) == 0 ) {
		char* ocdkey;
		char* oname;
		char* ncdkey;
		char* nname;
		char* title;
		int level;
		int trns;
		int floor;
		ocdkey = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		oname = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		ncdkey = saacproto_wrapStringAddr( saacproto_stringwrapper[3] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[4] ));
		nname = saacproto_wrapStringAddr( saacproto_stringwrapper[4] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[5] ));
		title = saacproto_wrapStringAddr( saacproto_stringwrapper[5] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[6] ));
		level = saacproto_demkstr_int( saacproto.token_list[7] );
		trns = saacproto_demkstr_int( saacproto.token_list[8] );
		floor = saacproto_demkstr_int( saacproto.token_list[9] );
		saacproto_S_UpdataStele_recv( fd , ocdkey , oname , ncdkey , nname , title , level , 
			trns , floor );
		return 0;
	}
#endif
/*
621 :  <LI><a name="Broadcast"><font color=blue>servertoclient Broadcast( string id, string charname, string message );</font></a><BR>
622 : 	 Broadcast及失市它件玄扔□田□井日必□丞扔□田□尺及霜耨迕
623 : 	 <dl>
624 : 	   <dt>string id
625 : 	   <dd>user ID
626 : 	   <dt>string charname
627 : 	   <dd>character name
628 : 	   <dt>string message
629 : 	   <dd>丢永本□斥
630 : 	   <dt>int flag
631 : 	   <dd>1卞允月午｝愤坌卞卞反  霜今木卅中［
632 : 	 </dl>
633 : 
*/

	if( strcmp( funcname , "Broadcast" ) == 0 ){
		char* id;
		char* charname;
		char* message;
		id = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		charname = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		message = saacproto_wrapStringAddr( saacproto_stringwrapper[3] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[4] ));
		saacproto_Broadcast_recv( fd,id,charname,message);
		return 0;
	}
/*
652 :  <LI><a name="Message"><font color=blue>servertoclient Message( string id_from, string charname_from, string id_to, string charname_to, string message , int option , int mesgid );</font></a><BR>
653 : 	 失市它件玄扔□田□井日必□丞扔□田□卞丢永本□斥毛霜耨允月［
654 : 	 <dl>
655 : 	   <dt>string id_from
656 : 	   <dd>from ID
657 : 	   <dt>string charname_from
658 : 	   <dd>from character name
659 : 	   <dt>string id_to
660 : 	   <dd>to ID
661 : 	   <dt>string charname_to
662 : 	   <dd>to character name
663 : 	   <dt>string message
664 : 	   <dd>丢永本□斥及
665 : 	   <dt>int option
666 : 	   <dd>左皿扑亦件(晓及楮醒毛辅寰)
667 : 	   <dt>int mesgid
668 : 	   <dd>丢永本□斥ID 失市它件玄扔□田□及栋  迕
669 : 	 </dl>
670 : 
*/

	if( strcmp( funcname , "Message" ) == 0 ){
		char* id_from;
		char* charname_from;
		char* id_to;
		char* charname_to;
		char* message;
		int option;
		int mesgid;
		id_from = saacproto_wrapStringAddr( saacproto_stringwrapper[1] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[2] ));
		charname_from = saacproto_wrapStringAddr( saacproto_stringwrapper[2] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[3] ));
		id_to = saacproto_wrapStringAddr( saacproto_stringwrapper[3] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[4] ));
		charname_to = saacproto_wrapStringAddr( saacproto_stringwrapper[4] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[5] ));
		message = saacproto_wrapStringAddr( saacproto_stringwrapper[5] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[6] ));
		option = saacproto_demkstr_int( saacproto.token_list[7] );
		mesgid = saacproto_demkstr_int( saacproto.token_list[8] );
		saacproto_Message_recv( fd,id_from,charname_from,id_to,charname_to,message,option,mesgid);
		return 0;
	}
	
	// CoolFish: Family 2001/5/9
	if( strcmp( funcname, "ACAddFM") == 0){
		int fmindex, index, charfdid;
		char *result;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		fmindex = saacproto_demkstr_int(saacproto.token_list[3]);
		index = saacproto_demkstr_int(saacproto.token_list[4]);
		charfdid = saacproto_demkstr_int(saacproto.token_list[5]);
		saacproto_ACAddFM_recv(fd, result, fmindex, charfdid, index);
		return 0;
	}
	if( strcmp( funcname, "ACJoinFM") == 0){
		int charfdid, recv;
		char *result;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		recv = saacproto_demkstr_int(saacproto.token_list[3]);
		charfdid = saacproto_demkstr_int(saacproto.token_list[4]);
		saacproto_ACJoinFM_recv(fd, result, recv, charfdid);
		return 0;
	}
	if( strcmp( funcname, "ACLeaveFM") == 0){
		int  charfdid, resultflag;
		char *result;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		resultflag = saacproto_demkstr_int(saacproto.token_list[3]);
		charfdid = saacproto_demkstr_int(saacproto.token_list[4]);
		saacproto_ACLeaveFM_recv(fd, result, resultflag, charfdid);
		return 0;
	}
	if( strcmp( funcname, "ACDelFM") == 0){
		int charfdid;
		char *result;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		charfdid = saacproto_demkstr_int(saacproto.token_list[3]);
		saacproto_ACDelFM_recv(fd, result, charfdid);
		return 0;
	}
	if( strcmp( funcname, "ACShowFMList") == 0){
		int fmnum;
		char *result, *data;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		fmnum = saacproto_demkstr_int(saacproto.token_list[3]);
		data = saacproto_wrapStringAddr(saacproto_stringwrapper[3], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[4]));
		saacproto_ACShowFMList_recv(fd, result, fmnum, data);
		return 0;
	}

	if( strcmp( funcname, "ACShowMemberList") == 0){
		int index, fmmemnum, fmacceptflag, fmjoinnum;
		char *result, *data;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		index = saacproto_demkstr_int(saacproto.token_list[3]);
		fmmemnum = saacproto_demkstr_int(saacproto.token_list[4]);
		fmacceptflag = saacproto_demkstr_int(saacproto.token_list[5]);
		fmjoinnum = saacproto_demkstr_int(saacproto.token_list[6]);
		data = saacproto_wrapStringAddr(saacproto_stringwrapper[6], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[7]));
		
#ifdef _FAMILYBADGE_
		int  badge = saacproto_demkstr_int(saacproto.token_list[8]);
#endif
		saacproto_ACShowMemberList_recv(fd, result, index, fmmemnum, fmacceptflag, fmjoinnum, data
#ifdef _FAMILYBADGE_
			,badge
#endif
			);
		return 0;
	}
	if( strcmp( funcname, "ACFMDetail") == 0){
		int charfdid;
		char *result, *data;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		data = saacproto_wrapStringAddr(saacproto_stringwrapper[2], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[3]));
		charfdid = saacproto_demkstr_int(saacproto.token_list[4]);
		saacproto_ACFMDetail_recv(fd, result, data, charfdid);
		return 0;
	}
	if( strcmp( funcname, "ACMemberJoinFM") == 0){
		int charfdid;
		char *result, *data;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		data = saacproto_wrapStringAddr(saacproto_stringwrapper[2], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[3]));
		charfdid = saacproto_demkstr_int(saacproto.token_list[4]);
		saacproto_ACMemberJoinFM_recv(fd, result, data, charfdid);
		return 0;
	}
	if( strcmp( funcname, "ACMemberLeaveFM") == 0){
		int charfdid;
		char *result, *data;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		data = saacproto_wrapStringAddr(saacproto_stringwrapper[2], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[3]));
		charfdid = saacproto_demkstr_int(saacproto.token_list[4]);
		saacproto_ACMemberLeaveFM_recv(fd, result, data, charfdid);
		return 0;
	}
	if( strcmp( funcname, "ACFMCharLogin") == 0){
		int charfdid, index, floor, fmdp, joinflag, fmsetupflag, flag, charindex;
#ifdef _PERSONAL_FAME   // Arminius: 家族个人声望
		int charfame;
#endif
#ifdef _NEW_MANOR_LAW
		int momentum;
#endif
		char *result;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		index = saacproto_demkstr_int(saacproto.token_list[3]);
		floor = saacproto_demkstr_int(saacproto.token_list[4]);
		fmdp = saacproto_demkstr_int(saacproto.token_list[5]);
		joinflag = saacproto_demkstr_int(saacproto.token_list[6]);
		fmsetupflag = saacproto_demkstr_int(saacproto.token_list[7]);
		flag = saacproto_demkstr_int(saacproto.token_list[8]);
		charindex = saacproto_demkstr_int(saacproto.token_list[9]);
#ifdef _PERSONAL_FAME   // Arminius: 家族个人声望
		charfame = saacproto_demkstr_int(saacproto.token_list[10]);
		charfdid = saacproto_demkstr_int(saacproto.token_list[11]);
	#ifdef _NEW_MANOR_LAW
		momentum = saacproto_demkstr_int(saacproto.token_list[12]);
	#endif
		saacproto_ACFMCharLogin_recv(fd, result, index, floor, fmdp,
			joinflag, fmsetupflag, flag, charindex, charfame, charfdid
	#ifdef _NEW_MANOR_LAW
			,momentum
	#endif
			);
#else
		charfdid = saacproto_demkstr_int(saacproto.token_list[10]);
		saacproto_ACFMCharLogin_recv(fd, result, index, floor, fmdp,
			joinflag, fmsetupflag, flag, charindex, charfdid);
#endif
		return 0;
	}
	if( strcmp( funcname, "ACFMCharLogout") == 0){
		int  charfdid;
		char *result;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		charfdid = saacproto_demkstr_int(saacproto.token_list[3]);
		saacproto_ACFMCharLogout_recv(fd, result, charfdid);
		return 0;
	}
	if( strcmp( funcname, "ACFMReadMemo") == 0){
		int index, num, dataindex;
		char *result, *data;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		index = saacproto_demkstr_int(saacproto.token_list[3]);
		num = saacproto_demkstr_int(saacproto.token_list[4]);
		dataindex = saacproto_demkstr_int(saacproto.token_list[5]);
		data = saacproto_wrapStringAddr(saacproto_stringwrapper[5], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[6]));
		saacproto_ACFMReadMemo_recv(fd, result, index, num, dataindex, data);
		return 0;
	}
	if( strcmp( funcname, "ACFMWriteMemo") == 0){
		int index;
		char *result;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		index = saacproto_demkstr_int(saacproto.token_list[3]);
		saacproto_ACFMWriteMemo_recv(fd, result, index);
		return 0;
	}
	if( strcmp( funcname, "ACFMPointList") == 0){
		char *result, *data;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		data = saacproto_wrapStringAddr(saacproto_stringwrapper[2], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[3]));
		saacproto_ACFMPointList_recv(fd, result, data);
		return 0;
	}


	if( strcmp( funcname, "ACSetFMPoint") == 0){
		int charfdid, r;
		char *result;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		r = saacproto_demkstr_int(saacproto.token_list[3]);
		charfdid = saacproto_demkstr_int(saacproto.token_list[4]);
		saacproto_ACSetFMPoint_recv(fd, result, r, charfdid);
		return 0;
	}
	if( strcmp( funcname, "ACFixFMPoint") == 0){
		int  r;
		char *result;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		r = saacproto_demkstr_int(saacproto.token_list[3]);
		saacproto_ACFixFMPoint_recv(fd, result, r);
		return 0;
	}
	if( strcmp( funcname, "ACFMAnnounce") == 0){
		int fmindex, index, color, kindflag;
		char *result, *fmname, *data;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		fmname = saacproto_wrapStringAddr(saacproto_stringwrapper[2], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[3]));
		fmindex = saacproto_demkstr_int(saacproto.token_list[4]);
		index = saacproto_demkstr_int(saacproto.token_list[5]);
		kindflag = saacproto_demkstr_int(saacproto.token_list[6]);
		data = saacproto_wrapStringAddr(saacproto_stringwrapper[6], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[7]));
		color = saacproto_demkstr_int(saacproto.token_list[8]);
		saacproto_ACFMAnnounce_recv(fd, result, fmname, fmindex, index, kindflag, data, color);
		return 0;
	}
	if( strcmp( funcname, "ACShowTopFMList") == 0){
		int num, kindflag;
		char *result, *data;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		kindflag = saacproto_demkstr_int(saacproto.token_list[3]);
		num = saacproto_demkstr_int(saacproto.token_list[4]);
		data = saacproto_wrapStringAddr(saacproto_stringwrapper[4], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[5]));
		saacproto_ACShowTopFMList_recv(fd, result, kindflag, num, data);
		return 0;
	}
	if( strcmp( funcname, "ACFixFMData") == 0){
		int kindflag, charfdid;
		char *result, *data1, *data2;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		kindflag = saacproto_demkstr_int(saacproto.token_list[3]);
		data1 = saacproto_wrapStringAddr(saacproto_stringwrapper[3], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[4]));
		data2 = saacproto_wrapStringAddr(saacproto_stringwrapper[4], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[5]));
		charfdid = saacproto_demkstr_int(saacproto.token_list[6]);
		saacproto_ACFixFMData_recv(fd, result, kindflag, data1, data2, charfdid);
		return 0;
	}
	if( strcmp( funcname, "ACFixFMPK") == 0){
		int  data, winindex, loseindex;
		char *result;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		data = saacproto_demkstr_int(saacproto.token_list[3]);
		winindex = saacproto_demkstr_int(saacproto.token_list[4]);
		loseindex = saacproto_demkstr_int(saacproto.token_list[5]);
		saacproto_ACFixFMPK_recv(fd, result, data, winindex, loseindex);
		return 0;
	}
	if( strcmp( funcname, "ACGMFixFMData") == 0){
		int charfdid;
		char *result, *fmname;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		fmname = saacproto_wrapStringAddr(saacproto_stringwrapper[2], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[3]));
		charfdid = saacproto_demkstr_int(saacproto.token_list[4]);
		saacproto_ACGMFixFMData_recv(fd, result, fmname, charfdid);
		return 0;
	}
	if( strcmp( funcname, "ACGetFMData") == 0){
		int kindflag, charfdid, data;
		char *result;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		kindflag = saacproto_demkstr_int(saacproto.token_list[3]);
		data = saacproto_demkstr_int(saacproto.token_list[4]);
		charfdid = saacproto_demkstr_int(saacproto.token_list[5]);
		saacproto_ACGetFMData_recv(fd, result, kindflag, data, charfdid);
		return 0;
	}
	if( strcmp( funcname, "ACFMClearPK") == 0){
		int fmindex, index;
		char *result, *fmname;
		result = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		fmname = saacproto_wrapStringAddr(saacproto_stringwrapper[2], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[3]));
		fmindex = saacproto_demkstr_int(saacproto.token_list[4]);
		index = saacproto_demkstr_int(saacproto.token_list[5]);
		saacproto_ACFMClearPK_recv(fd, result, fmname, fmindex, index);
		return 0;
	}
	// Arminius 7.26 manor pk acknowledge
	if( strcmp( funcname, "ACManorPKAck") == 0){
		char *data;
		data = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		saacproto_ACManorPKAck_recv(fd, data);
		return 0;
	}

#ifdef _ACFMPK_LIST
	if( strcmp( funcname, "ACLoadFmPk") == 0){
			char *data;
 		data = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		saacproto_ACLoadFmPk_recv( fd, data);
		return 0;
	}
	if( strcmp( funcname, "ACSendFmPk") == 0){
		int toindex, flg;
		toindex = saacproto_demkstr_int( saacproto.token_list[2] );
		flg  =saacproto_demkstr_int( saacproto.token_list[3] );
		saacproto_ACSendFmPk_recv( fd, toindex, flg);
		return 0;
	}
#endif
#ifdef _SEND_EFFECT	   	        // WON ADD AC送下雪、下雨等特效
  if( strcmp(funcname, "EFFECT") == 0){
		char *effect;
		effect = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		print("\n AC send to GS weather effect !!");

		CHAR_CHAT_DEBUG_sendeffect( fd, effect );	  
		return 0;
	}
#endif

#ifdef _NEW_VIP_SHOP
	if( strcmp( funcname , "QueryPoint" ) == 0 ){
		int fd = saacproto_demkstr_int( saacproto.token_list[2] );
		int point = saacproto_demkstr_int( saacproto.token_list[3] );
		saacproto_QueryPoint_recv(fd, point); 
		return 0;
	}
	
	if( strcmp( funcname , "NewVipShop" ) == 0 ){
		int fd = saacproto_demkstr_int( saacproto.token_list[2] );
		int point = saacproto_demkstr_int( saacproto.token_list[3] );
		char *data = saacproto_wrapStringAddr(saacproto_stringwrapper[3], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[4]));
		int flag = saacproto_demkstr_int( saacproto.token_list[5] );
		saacproto_NewVipShop_recv(fd, point, data, flag); 
		return 0;
	}
#endif

#ifdef _ITEM_PET_LOCKED
	if( strcmp( funcname , "ItemPetLocked" ) == 0 ){
		int fd = saacproto_demkstr_int( saacproto.token_list[2] );
		int flag = saacproto_demkstr_int( saacproto.token_list[3] );
		char *data = saacproto_wrapStringAddr(saacproto_stringwrapper[3], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[4]));
		saacproto_ItemPetLocked_recv(fd, flag, data); 
		return 0;
	}
	
	if( strcmp( funcname , "ItemPetLockedPasswd" ) == 0 ){
		int fd = saacproto_demkstr_int( saacproto.token_list[2] );
		char *data = saacproto_wrapStringAddr(saacproto_stringwrapper[2], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[3]));
		saacproto_ItemPetLockedPasswd_recv(fd, data); 
		return 0;
	}
#endif

#ifdef _ONLINE_COST
	if( strcmp( funcname , "OnlineCost" ) == 0 ){
		int fd = saacproto_demkstr_int( saacproto.token_list[2] );
		char *data = saacproto_wrapStringAddr(saacproto_stringwrapper[2], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[3]));
		saacproto_OnlineCost_recv(fd, data); 
		return 0;
	}
#endif

#ifdef _SQL_BUY_FUNC
	if( strcmp( funcname , "OnlineBuy" ) == 0 ){
		int fd = saacproto_demkstr_int( saacproto.token_list[2] );
		char *data = saacproto_wrapStringAddr(saacproto_stringwrapper[2], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[3]));
		saacproto_OnlineBuy_recv(fd, data); 
		return 0;
	}
#endif

#ifdef _VIPPOINT_OLD_TO_NEW
	if( strcmp( funcname , "OldToNew" ) == 0 ){
		int fd = saacproto_demkstr_int( saacproto.token_list[2] );
		char *data = saacproto_wrapStringAddr(saacproto_stringwrapper[2], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[3]));
		saacproto_OldToNew_recv(fd, data); 
		return 0;
	}
#endif

#ifdef _FORMULATE_AUTO_PK
	if( strcmp( funcname , "FormulateAutoPk" ) == 0 ){
		int fd = saacproto_demkstr_int( saacproto.token_list[2] );
		char *data = saacproto_wrapStringAddr(saacproto_stringwrapper[2], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[3]));
		saacproto_FormulateAutoPk_recv(fd, data); 
		return 0;
	}
#endif

#ifdef _LOTTERY_SYSTEM
	if( strcmp( funcname , "LotterySystem" ) == 0 ){
		char *data = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		saacproto_LotterySystem_recv(data); 
		return 0;
	}
#endif

#ifdef _ALL_SERV_SEND
	if( strcmp( funcname , "AllServSend" ) == 0 ){
		char *data = saacproto_wrapStringAddr(saacproto_stringwrapper[1], saacproto.workbufsize, saacproto_demkstr_string(saacproto.token_list[2]));
		saacproto_AllServSend_recv(data);
		return 0;
	}
#endif

#ifdef _ANGEL_SUMMON
	if( strcmp( funcname , "ACMissionTable" ) == 0 ){
		int num;
		int type;
		char* data;
		//int charaindex;
		char* angelinfo;

		num = saacproto_demkstr_int( saacproto.token_list[2] );
		type = saacproto_demkstr_int( saacproto.token_list[3] );
		data = saacproto_wrapStringAddr( saacproto_stringwrapper[3] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[4] ));
		//charaindex = saacproto_demkstr_int( saacproto.token_list[5] );
		angelinfo = saacproto_wrapStringAddr( saacproto_stringwrapper[4] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[5] ));

		//saacproto_ACMissionTable_recv( fd, num, type, data, charaindex);
		saacproto_ACMissionTable_recv( fd, num, type, data, angelinfo);
		return 0;
	}
#endif

#ifdef _TEACHER_SYSTEM
	if(strcmp(funcname,"ACCheckCharacterOnLine") == 0){
		int flag,charaindex,iOnline;
		char* data;
		
		charaindex = saacproto_demkstr_int(saacproto.token_list[2]);
		iOnline = saacproto_demkstr_int(saacproto.token_list[3]);
		data = saacproto_wrapStringAddr( saacproto_stringwrapper[3] , saacproto.workbufsize , saacproto_demkstr_string( saacproto.token_list[4] ));
		flag = saacproto_demkstr_int( saacproto.token_list[5] );
		saacproto_ACCheckCharacterOnLine_recv(fd,charaindex,iOnline,data,flag);
		return 0;
	}
#endif

	if( strcmp( funcname , "ACCharLogin" ) == 0 ){
		int flag;
		int clifd;
		clifd = saacproto_demkstr_int( saacproto.token_list[2] );
		flag = saacproto_demkstr_int( saacproto.token_list[3] );
		saacproto_ACCharLogin_recv(fd, clifd, flag); 

		return 0;
	}
	printf("无法找到SAAC接口:%s",funcname);
	return -1;
}
void saacproto_SetClientLogFiles( char *r , char *w )
{
	saacproto_strcpysafe( saacproto_readlogfilename , r , sizeof( saacproto_readlogfilename ) );
	saacproto_strcpysafe( saacproto_writelogfilename , w , sizeof( saacproto_writelogfilename ) );
}
int saacproto_InitClient( int (*writefunc)(int,char*,int) ,int bufsiz ,int fd)
{
	int i;
	if( (void*)writefunc == NULL){saacproto.write_func = saacproto_default_write_wrap;} else {saacproto.write_func = writefunc;}
	saacproto_AllocateCommonWork(bufsiz);
	saacproto_stringwrapper = (char**)calloc( 1,sizeof(char*) * MAXLSRPCARGS);
	if(saacproto_stringwrapper ==NULL)return -1;
	memset( saacproto_stringwrapper , 0, sizeof(char*)*MAXLSRPCARGS);
	for(i=0;i<MAXLSRPCARGS;i++){
		saacproto_stringwrapper[i] = (char*)calloc( 1, bufsiz );
		if( saacproto_stringwrapper[i] == NULL){
			for(i=0;i<MAXLSRPCARGS;i++){free( saacproto_stringwrapper[i]);return -1;}
		}
	}
	return 0;
}
void saacproto_CleanupClient( void )
{
	int i;
	free( saacproto.work );
	free( saacproto.arraywork);
	free( saacproto.escapework );
	free( saacproto.val_str);
	free( saacproto.token_list );
	for(i=0;i<MAXLSRPCARGS;i++){free( saacproto_stringwrapper[i]);}
	free( saacproto_stringwrapper );
}
/* end of the generated client code */

#ifdef _WAEI_KICK
void saacproto_ACKick_send( int fd, char* kickid,int kickfd, int flg)
{
	saacproto_CreateHeader( saacproto.work , "ACKick");
	saacproto_strcatsafe( saacproto.work, saacproto_mkstr_string( kickid) , saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work, saacproto_mkstr_int( kickfd) , saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work, saacproto_mkstr_int( flg) , saacproto.workbufsize );
	saacproto_Send( fd , saacproto.work );
}
#endif

#ifdef _CHAR_POOLITEM
void saacproto_ACCharSavePoolItem_send( int acfd, int charaindex, int clifdid, char *CdKey, char *Pooldataarg)
{
	saacproto_CreateHeader( saacproto.work , "ACSavePoolItem" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( CdKey) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( Pooldataarg ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( charaindex ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( clifdid ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}

void saacproto_ACCharGetPoolItem_send( int acfd, int meindex, int charaindex, int clifdid, char * CdKey)
{
	saacproto_CreateHeader( saacproto.work , "ACGetPoolItem" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( CdKey) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( charaindex ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( clifdid ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( meindex ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}

void saacproto_ACCharInsertPoolItem_send( int acfd, int charaindex, int clifdid, char *CdKey, char *Pooldataarg, int TYPE)
{
	saacproto_CreateHeader( saacproto.work , "ACInsertPoolItem" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( CdKey) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( Pooldataarg ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( charaindex ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( clifdid ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( TYPE ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}
#endif


#ifdef _CHAR_POOLPET
void saacproto_ACCharSavePoolPet_send( int acfd, int charaindex, int clifdid, char *CdKey, char *Pooldataarg)
{
	saacproto_CreateHeader( saacproto.work , "ACSavePoolPet" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( CdKey) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( Pooldataarg ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( charaindex ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( clifdid ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}

void saacproto_ACCharGetPoolPet_send( int acfd, int meindex, int charaindex, int clifdid, char * CdKey)
{
	saacproto_CreateHeader( saacproto.work , "ACGetPoolPet" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( CdKey) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( charaindex ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( clifdid ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( meindex ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}

void saacproto_ACCharInsertPoolPet_send( int acfd, int charaindex, int clifdid, char *CdKey, char *Pooldataarg, int TYPE)
{
	saacproto_CreateHeader( saacproto.work , "ACInsertPoolPet" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( CdKey) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( Pooldataarg ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( charaindex ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( clifdid ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( TYPE ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}
#endif

#ifdef _ANGEL_SUMMON
//void saacproto_ACMissionTable_send( int acfd, int num, int type, char *data, int charaindex)
void saacproto_ACMissionTable_send( int acfd, int num, int type, char *data, char* angelinfo )
{
	saacproto_CreateHeader( saacproto.work , "ACMissionTable" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( num ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( type ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( data) ,saacproto.workbufsize );
	//saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( charaindex ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( angelinfo) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}
#endif

#ifdef _TEACHER_SYSTEM
void saacproto_ACCheckCharacterOnLine_send( int acfd, int charaindex, char *id, char *name, int flag)
{
	saacproto_CreateHeader( saacproto.work , "ACCheckCharacterOnLine" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( charaindex ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( name) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( flag ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}
#endif


void saacproto_ACCharLogin_send( int fd, int clifd, char* id,char* pas,char* ip
#ifdef _NEWCLISETMAC
,char* mac 
#endif
)
{
	saacproto_CreateHeader( saacproto.work , "ACCharLogin" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( clifd ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( pas ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( ip ) ,saacproto.workbufsize );
#ifdef _NEWCLISETMAC
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( mac ) ,saacproto.workbufsize );
#endif
	saacproto_Send( acfd , saacproto.work );
}

void saacproto_LockLogin_send( int fd, char* id, char* ip, int flag )
{
	saacproto_CreateHeader( saacproto.work , "LockLogin" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( ip ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( flag ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}

#ifdef _NEW_VIP_SHOP
void saacproto_QueryPoint_send( int acfd, int fd, char* id )
{
	if(acfd == -1)return;
	saacproto_CreateHeader( saacproto.work , "QueryPoint" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( fd ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}

void saacproto_NewVipShop_send( int acfd, int fd, char* id, int point, char* buf, int flag)
{
	if(acfd == -1)return;
	saacproto_CreateHeader( saacproto.work , "NewVipShop" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( fd ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( point ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( buf ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( flag ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}

#ifdef _COST_ITEM
void saacproto_CostItem_send( int acfd, int fd, char* id, int point)
{
	if(acfd == -1)return;
	saacproto_CreateHeader( saacproto.work , "CostItem" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( fd ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( point ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}
#endif
#endif

#ifdef _ITEM_PET_LOCKED
void saacproto_ItemPetLocked_send( int fd, char* id, char *safepasswd )
{
	saacproto_CreateHeader( saacproto.work , "ItemPetLocked" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( fd ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( safepasswd ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}
void saacproto_ItemPetLockedPasswd_send( int fd, char* id, char *safepasswd )
{
	saacproto_CreateHeader( saacproto.work , "ItemPetLockedPasswd" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( fd ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( safepasswd ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}
#endif

#ifdef _ONLINE_COST
void saacproto_OnlineCost_send( int acfd, int fd, char* id, char *costpasswd, int fmindex, char *fmname )
{
	if(acfd == -1)return;
	saacproto_CreateHeader( saacproto.work , "OnlineCost" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( fd ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( costpasswd ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( fmindex ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( fmname ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}
#endif

#ifdef _SQL_BUY_FUNC
void saacproto_OnlineBuy_send( int acfd, int fd, char* id, char *costpasswd )
{
	if(acfd == -1)return;
	saacproto_CreateHeader( saacproto.work , "OnlineBuy" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( fd ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( costpasswd ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}
#endif

#ifdef _VIPPOINT_OLD_TO_NEW
void saacproto_OldToNew_send( int acfd, int fd, char* id, int point )
{
	if(acfd == -1)return;
	saacproto_CreateHeader( saacproto.work , "OldToNew" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( fd ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( point ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}
#endif

#ifdef _FORMULATE_AUTO_PK
void saacproto_FormulateAutoPk_send( int fd, char* id, int point )
{
	saacproto_CreateHeader( saacproto.work , "FormulateAutoPk" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( fd ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( point ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}
#endif

#ifdef _OTHER_SAAC_LINK
void saacproto_OtherSaacLink_send( int acfd, char* filename, char* data )
{
	saacproto_CreateHeader( saacproto.work , "OtherSaacLink" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( filename ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( data ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}
#endif

#ifdef _CHARADATA_SAVE_SQL
void saacproto_CharadataSaveSQL_send( int fd, char* id, Charadata charadata, int saveindex, int flag )
{
	saacproto_CreateHeader( saacproto.work , "CharadataSaveSQL" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( fd ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( id ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charadata.CHAR_list_String ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charadata.CHAR_list_info1_String ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charadata.CHAR_list_info2_String ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charadata.CHAR_list_count_String ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charadata.CHAR_list_info3_String ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charadata.CHAR_list_event_String ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charadata.CHAR_list_info4_String ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charadata.CHAR_list_attackmagic_String ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charadata.CHAR_list_info5_String ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charadata.CHAR_list_profession_String ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charadata.CHAR_list_info6_String ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charadata.CHAR_list_info_String ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charadata.CHAR_list_flg_String ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charadata.CHAR_list_skill_String ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charadata.CHAR_list_item_String ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charadata.CHAR_list_title_String ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( charadata.CHAR_list_mail_String ) ,saacproto.workbufsize );

	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( saveindex ) ,saacproto.workbufsize );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_int( flag ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}
#endif


#ifdef _LOTTERY_SYSTEM
void saacproto_LotterySystem_send()
{
	saacproto_CreateHeader( saacproto.work , "LotterySystem" );
	saacproto_Send( acfd , saacproto.work );
}
#endif

#ifdef _ALL_SERV_SEND
void saacproto_AllServSend_send(char *data)
{
	saacproto_CreateHeader( saacproto.work , "AllServSend" );
	saacproto_strcatsafe( saacproto.work , saacproto_mkstr_string( data ) ,saacproto.workbufsize );
	saacproto_Send( acfd , saacproto.work );
}
#endif


