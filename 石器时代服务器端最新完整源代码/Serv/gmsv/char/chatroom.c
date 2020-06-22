#include "version.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "readmap.h"
#include "object.h"
#include "char.h"
#include "char_base.h"
#include "chatmagic.h"
#include "battle.h"
#include "log.h"
#include "configfile.h"
#include "lssproto_serv.h"
#include "saacproto_cli.h"
#include "family.h"
#include "chatroom.h"
#include "net.h"
#include "util.h"

#ifdef _CHATROOMPROTOCOL
/*
CHAR_WORKCHATROOMTYPE :
	0 : 无
	1 : 聊天室室长
	2 : 聊天室成员
*/
typedef struct {
	BOOL useFlag ;
	int NowPeople;
	int Maker;
	int MemberList[MAX_PPLINROOM];
	char RoomName[32];
} CHATROOM_CLASS ; 
CHATROOM_CLASS ChatRoom[MAX_CHATROOM];


BOOL ChatCheck_BeMaster( int myindex, int chatnum)
{
	if( !CHAR_CHECKINDEX ( myindex ) ) 
		return FALSE;

	if( CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMNUM ) == 0 ) return FALSE;

	if( chatnum != -1 ){
		if( CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMNUM ) != chatnum )
			return FALSE;
	}
	if( CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMTYPE ) == 1 ) return TRUE;

	return FALSE;
}

BOOL ChatCheck_Free( int myindex)
{
	int old_gold;
	if ( !CHAR_CHECKINDEX ( myindex ) ) 
		return FALSE;
	old_gold = CHAR_getInt( myindex, CHAR_GOLD );
	if( old_gold < 200 ){
		CHAR_talkToCli ( myindex , -1 , "成立聊天室需花费２００石币" , CHAR_COLORYELLOW ); 
		return FALSE;
	}
	if ( CHAR_getInt ( myindex , CHAR_LV ) < 30 &&
		CHAR_getInt ( myindex , CHAR_TRANSMIGRATION ) < 1 ) {
		CHAR_talkToCli ( myindex , -1 , "成立聊天室需０转３０级以上！" , CHAR_COLORYELLOW );
		return FALSE;
	}
	if ( CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMTYPE ) != 0 ) {
		CHAR_talkToCli ( myindex , -1 , "你已经在其他聊天室中！" , CHAR_COLORYELLOW );
		return FALSE;
	}
	return TRUE;
}

void InitChatRoom ( void ) {
	int i , j ; 
	for ( i = 0 ; i < MAX_CHATROOM ; i ++ ) {
		ChatRoom[ i ].useFlag = FALSE ; 
		ChatRoom[ i ].Maker = -1 ; 
		ChatRoom[ i ].NowPeople = -1 ; 
		sprintf ( ChatRoom[ i ].RoomName , " " ) ; 
		for ( j = 0 ; j < MAX_PPLINROOM ; j ++ ) 
			ChatRoom[ i ].MemberList[ j ] = -1 ; 
	}
}

BOOL ChatRoom_Create ( int myindex , char *message )
{
	int i ; 

	if( ChatCheck_Free( myindex) == FALSE ) return FALSE;

	for ( i = 0 ; i < MAX_CHATROOM ; i ++ ) {
		if ( !ChatRoom[ i ].useFlag ) {
			ChatRoom[ i ].useFlag = TRUE ; 
			ChatRoom[ i ].Maker = myindex ; 
			ChatRoom[ i ].NowPeople = 1 ; 
			ChatRoom[ i ].MemberList[ 0 ] = myindex ; 
			sprintf ( ChatRoom[ i ].RoomName , "%s" , message ) ; 
			CHAR_setWorkInt ( myindex , CHAR_WORKCHATROOMTYPE , 1 ) ; 
			CHAR_setWorkInt ( myindex , CHAR_WORKCHATROOMNUM , i ) ; 
			ChatRoom_Refresh ( i ) ; 

			CHAR_DelGold( myindex, 200);
			CHAR_talkToCli ( myindex , -1 , "成立聊天室扣除２００石币" , CHAR_COLORYELLOW ); 
			return TRUE; 
		}
	}
	CHAR_talkToCli ( myindex , -1 , "聊天室已满无法建立新的聊天频道！" , CHAR_COLORYELLOW ); 
	return FALSE; 
}

BOOL ChatRoom_Destroy ( int myindex )
{
	int i , j ; 
	int fd ;/*= getfdFromCharaIndex(myindex);*/
	char buf[16];
	if ( !CHAR_CHECKINDEX ( myindex ) ) 
		return FALSE;
	if ( CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMTYPE ) == 1 ) {
		i = CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMNUM ) ; 
		ChatRoom[ i ].useFlag = FALSE ; 
		ChatRoom[ i ].Maker = -1 ; 
		ChatRoom[ i ].NowPeople = -1 ; 

		// WON FIX 
		memset( ChatRoom[ i ].RoomName, 0 , sizeof(ChatRoom[ i ].RoomName) );
		//sprintf ( ChatRoom[ i ].RoomName , " " ) ; 

		for ( j = 0 ; j < MAX_PPLINROOM ; j ++ ) {
			if ( !CHAR_CHECKINDEX ( ChatRoom[ i ].MemberList[ j ] ) ) {
				ChatRoom[ i ].MemberList[ j ] = -1 ; 
				continue;
			}
			CHAR_setWorkInt ( ChatRoom[ i ].MemberList[ j ] , CHAR_WORKCHATROOMTYPE , 0 ) ; 
			CHAR_setWorkInt ( ChatRoom[ i ].MemberList[ j ] , CHAR_WORKCHATROOMNUM , 0 ) ; 
			fd = getfdFromCharaIndex( ChatRoom[ i ].MemberList[ j ] );
			sprintf ( buf , "D|%d" , i ) ; 
			lssproto_CHATROOM_send ( fd , buf ) ; 
			ChatRoom[ i ].MemberList[ j ] = -1 ; 
		}
		return TRUE;
	}
	return FALSE;
}

void ChatRoom_Message ( int myindex ,  char *message )
{
	int i , j , fd ; 
	char buf[1024];
	if ( !CHAR_CHECKINDEX ( myindex ) ) 
		return ;
	if ( CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMTYPE ) > 0 ) {
		i = CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMNUM ) ; 
		
		// WON FIX
		if( i < 0 ) return;

		for ( j = 0 ; j < MAX_PPLINROOM ; j ++ ) {
			if ( !CHAR_CHECKINDEX ( ChatRoom[ i ].MemberList[ j ] ) ) 
				continue;
			snprintf ( buf , sizeof ( buf ) , "T|%s" , message ) ; 
			fd = getfdFromCharaIndex ( ChatRoom[ i ].MemberList[ j ] );
			lssproto_CHATROOM_send ( fd , buf ) ; 
		}
	}
}

void ChatRoom_Kick ( int myindex , int toindex )
{
	int i , fd ; 
	int Num = CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMNUM );
	if ( !CHAR_CHECKINDEX ( myindex ) ) 
		return ;
	if ( !CHAR_CHECKINDEX ( toindex ) ) 
		return ;
	if ( myindex == toindex ) return;
	if ( CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMTYPE ) == 1 ) {
		if ( Num != CHAR_getWorkInt ( toindex , CHAR_WORKCHATROOMNUM ) )
			return;
		else {
			CHAR_setWorkInt ( toindex , CHAR_WORKCHATROOMTYPE , 0 ) ; 
			CHAR_setWorkInt ( toindex , CHAR_WORKCHATROOMNUM , -1) ; 
			//CHAR_talkToCli ( toindex , -1 , "室长将你踢出聊天室！" , CHAR_COLORRED ) ; 
			fd = getfdFromCharaIndex( toindex );
			lssproto_CHATROOM_send ( fd , "K|" ) ; 
			ChatRoom[ Num ].NowPeople --;
			for ( i = 0 ; i < MAX_PPLINROOM ; i ++ ) {
				if ( ChatRoom[ Num ].MemberList[ i ] == toindex ) {
					ChatRoom[ Num ].MemberList[ i ] = -1 ; 
					break;
				}
			}
		}
	}
	ChatRoom_Refresh ( Num ) ; 
}

void ChatRoom_Make ( int myindex , int toindex )
{
	int Num = CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMNUM );
	if ( !CHAR_CHECKINDEX ( myindex ) ) 
		return ;
	if ( !CHAR_CHECKINDEX ( toindex ) ) 
		return ;
	if ( CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMTYPE ) == 1 ) {
		if ( Num != CHAR_getWorkInt ( toindex , CHAR_WORKCHATROOMNUM ) )
			return;
		else if ( CHAR_getInt ( toindex , CHAR_LV ) >= 30 || 
			CHAR_getInt ( toindex , CHAR_TRANSMIGRATION ) >= 1 ) {
			CHAR_setWorkInt ( myindex , CHAR_WORKCHATROOMTYPE , 2 ) ; 
			CHAR_setWorkInt ( toindex , CHAR_WORKCHATROOMTYPE , 1 ) ; 
			CHAR_talkToCli ( toindex , -1 , "你现在是聊天室的室长！" , CHAR_COLORRED ) ; 
		
			ChatRoom[ Num ].Maker = toindex ; 			
		}
		else {
			CHAR_talkToCli ( myindex , -1 , "您选择的继任人物等级不足以担任室长！" , CHAR_COLORRED ) ; 
		}
	}
	ChatRoom_Refresh ( Num ) ; 
}

void ChatRoom_Leave ( int myindex )
{
	int i , NextMaker = -1 ; 
	int Num = CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMNUM );
	if ( !CHAR_CHECKINDEX ( myindex ) ) 
		return ;
	if ( CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMTYPE ) == 1 ) {
		for ( i = 0 ; i < MAX_PPLINROOM ; i ++ ) {
			if ( ChatRoom[ Num ].MemberList[ i ] != myindex &&
				ChatRoom[ Num ].MemberList[ i ] != -1 ) {
				if ( CHAR_CHECKINDEX ( ChatRoom[ Num ].MemberList[ i ] ) ) {
					if ( CHAR_getInt ( ChatRoom[ Num ].MemberList[ i ] , CHAR_LV ) >= 30 || 
						CHAR_getInt ( ChatRoom[ Num ].MemberList[ i ] , CHAR_TRANSMIGRATION ) >= 1 ) {
						NextMaker = ChatRoom[ Num ].MemberList[ i ] ; 
						break;
					}
				}
			}
		}
		if ( NextMaker != -1 ) {
			ChatRoom_Make( myindex , NextMaker ) ; 
			CHAR_setWorkInt ( myindex , CHAR_WORKCHATROOMTYPE , 0) ; 
			CHAR_setWorkInt ( myindex , CHAR_WORKCHATROOMNUM, -1) ; 
			ChatRoom[ Num ].NowPeople --;
			for ( i = 0 ; i < MAX_PPLINROOM ; i ++ ) {
				if ( ChatRoom[ Num ].MemberList[ i ] == myindex ) {
					ChatRoom[ Num ].MemberList[ i ] = -1 ; 
					break;
				}
			}
		}else{
			ChatRoom_Destroy( myindex ) ; 
			return ; 
		}

	}else if ( CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMTYPE ) == 2 ) {
		CHAR_setWorkInt ( myindex, CHAR_WORKCHATROOMTYPE, 0) ; 
		CHAR_setWorkInt ( myindex, CHAR_WORKCHATROOMNUM, -1) ; 
		ChatRoom[ Num ].NowPeople --;
		for ( i = 0 ; i < MAX_PPLINROOM ; i ++ ) {
			if ( ChatRoom[ Num ].MemberList[ i ] == myindex ) {
				ChatRoom[ Num ].MemberList[ i ] = -1 ; 
				break;
			}
		}
	}
	ChatRoom_Refresh ( Num ) ; 
}

void ChatRoom_Join ( int myindex , int num )
{
	int fd ;
	char buf[64];
	if ( !CHAR_CHECKINDEX ( myindex ) ) 
		return ;
	if ( CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMTYPE ) != 0 ) 
		return ; 
	if ( ChatRoom[ num ].useFlag == TRUE && ChatRoom[ num ].NowPeople < MAX_PPLINROOM ) {
		sprintf ( buf , "J|%s|%d" , CHAR_getChar( myindex , CHAR_NAME ) , myindex ) ; 
		fd = getfdFromCharaIndex( ChatRoom[ num ].Maker );
		lssproto_CHATROOM_send ( fd , buf );
	}else if ( ChatRoom[ num ].NowPeople >= MAX_PPLINROOM ) 
		CHAR_talkToCli ( myindex , -1 , "聊天室人数已满！" , CHAR_COLORRED ) ; 

}

void ChatRoom_Agree ( int myindex , int toindex , int YesNo ) {
	int i ; 
	int Num = CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMNUM );
	if ( !CHAR_CHECKINDEX ( myindex ) ) 
		return ;
	if ( !CHAR_CHECKINDEX ( toindex ) ) 
		return ;
	if ( CHAR_getWorkInt ( toindex , CHAR_WORKCHATROOMTYPE ) != 0 ) 
		return;
	if ( CHAR_getWorkInt ( myindex , CHAR_WORKCHATROOMTYPE ) == 1 ) {
		if ( ChatRoom[ Num ].NowPeople < MAX_PPLINROOM && YesNo == 1 ) {
			CHAR_setWorkInt ( toindex , CHAR_WORKCHATROOMTYPE , 2 ) ; 
			CHAR_setWorkInt ( toindex , CHAR_WORKCHATROOMNUM , Num ) ; 
			ChatRoom[ Num ].NowPeople ++ ; 
			for ( i = 0 ; i < MAX_PPLINROOM ; i ++ ) {
				if ( ChatRoom[ Num ].MemberList[ i ] == -1 ) {
					ChatRoom[ Num ].MemberList[ i ] = toindex ; 
					break;
				}
			}
		}else if ( YesNo == 0 ) {
			CHAR_talkToCli ( toindex , -1 , "您申请的聊天室室长拒绝您的加入！" , CHAR_COLORRED ) ; 
		}else if ( ChatRoom[ Num ].NowPeople >= MAX_PPLINROOM ) {
			CHAR_talkToCli ( toindex , -1 , "您申请的聊天室人数已满！" , CHAR_COLORRED ) ; 
			CHAR_talkToCli ( myindex , -1 , "聊天室人数已满！" , CHAR_COLORRED ) ; 
		}
	}
	ChatRoom_Refresh ( Num ) ; 
}

void ChatRoom_List ( int fd )
{
	int i ; 
	char buf[128];
	char token[2048] = "B|";
	for ( i = 0 ; i < MAX_CHATROOM ; i ++ ) {
		if ( ChatRoom[ i ].useFlag == TRUE ) {
			sprintf ( buf , "聊天室%2d资讯：室名=>%20s , 室长=>%16s , 人数=>%2d" , 
				i , 
				ChatRoom[ i ].RoomName , 
				CHAR_getChar ( ChatRoom[ i ].Maker , CHAR_NAME ) , 
				ChatRoom[ i ].NowPeople );

			sprintf ( buf , "r%d|%s|%d|%s|p%d|" , 
				i , 
				ChatRoom[ i ].RoomName , 
				ChatRoom[ i ].Maker , 
				CHAR_getChar ( ChatRoom[ i ].Maker , CHAR_NAME ) , 
				ChatRoom[ i ].NowPeople  );
			strncat ( token , buf , sizeof ( buf ) ) ; 
		}
	}
	lssproto_CHATROOM_send ( fd , token ) ; 
}

void ChatRoom_Refresh ( int Num )
{
	int fd , i , RoomLeader = -1 ; 
	char token[2048] ;
	char token2[2096] ;
	char buf[64] ; 
	if ( ChatRoom[ Num ].useFlag == TRUE ) {
		sprintf ( token , "R|r%d|%s|%d|%s|p%d|" , 
			Num , 
			ChatRoom[ Num ].RoomName , 
			ChatRoom[ Num ].Maker , 
			CHAR_getChar ( ChatRoom[ Num ].Maker , CHAR_NAME ) , 
			ChatRoom[ Num ].NowPeople  );
		for ( i = 0 ; i < MAX_PPLINROOM ; i ++ ) {
			if ( ChatRoom[ Num ].MemberList[ i ] != -1 ) {
				sprintf( buf , "I%d|%s|%s|" , 
					ChatRoom[ Num ].MemberList[ i ] , 
					CHAR_getChar ( ChatRoom[ Num ].MemberList[ i ] , CHAR_NAME ) , 
					CHAR_getChar ( ChatRoom[ Num ].MemberList[ i ] , CHAR_OWNTITLE )  
					);
				strncat ( token , buf , sizeof ( buf ) ) ; 
			}
		}
	}
	for ( i = 0 ; i < MAX_PPLINROOM ; i ++ ) {
		if ( ChatRoom[ Num ].MemberList[ i ] != -1 ) {
			if ( CHAR_getWorkInt ( ChatRoom[ Num ].MemberList[ i ] , CHAR_WORKCHATROOMTYPE ) == 1 ) RoomLeader = 1;
			else RoomLeader = 0 ;
			sprintf ( token2 , "%s%d|" , token , RoomLeader ) ; 
			fd= getfdFromCharaIndex ( ChatRoom[ Num ].MemberList[ i ] );
			lssproto_CHATROOM_send ( fd , token2 ) ; 
		}
	}
}

void ChatRoom_recvall ( int fd , char *data )
{ 
	char Head[4];
	char message[1024];
	char buf[16];
	int myindex ;

	myindex = CONNECT_getCharaindex(fd);
	if ( !CHAR_CHECKINDEX ( myindex ) )return ;
	getStringFromIndexWithDelim( data , "|", 1, Head, sizeof(Head));
	getStringFromIndexWithDelim( data , "|", 2, message, sizeof(message));

	if ( strcmp ( Head , "C" ) == 0 ) { // 成立频道
		if ( !ChatRoom_Create ( myindex , message ) )
			print("\nSyu log Create Channel Error" );
	}else if ( strcmp ( Head , "D" ) == 0 ) { // 删除频道
		if ( !ChatRoom_Destroy ( myindex ) )
			print("\nSyu log Destroy Channel Error" ) ; 
	}else if ( strcmp ( Head , "A" ) == 0 ) {// 同意加入频道
		getStringFromIndexWithDelim( data , "|", 2, message, sizeof(message));
		getStringFromIndexWithDelim( data , "|", 3, buf, sizeof(buf));
		ChatRoom_Agree ( myindex , atoi( message ) , atoi( buf ) ) ; 
	}else if ( strcmp ( Head , "J" ) == 0 ) {// 申请频道
		getStringFromIndexWithDelim( data , "|", 2, message, sizeof(message));
		ChatRoom_Join ( myindex , atoi( message ) ) ; 
	}else if ( strcmp ( Head , "L" ) == 0 ) {// 离开频道
		ChatRoom_Leave ( myindex ) ; 
	}else if ( strcmp ( Head , "K" ) == 0 ) {//踢出频道
		getStringFromIndexWithDelim( data , "|", 2, message, sizeof(message));
		ChatRoom_Kick ( myindex , atoi( message ) ); 
	}else if ( strcmp ( Head , "M" ) == 0 ) { // 更换室长
		getStringFromIndexWithDelim( data , "|", 2, message, sizeof(message));
		ChatRoom_Make ( myindex , atoi( message ) ); 
	}else if ( strcmp ( Head , "T" ) == 0 ) {// 频道讯息
		getStringFromIndexWithDelim( data , "|", 2, message, sizeof(message));
		ChatRoom_Message ( myindex , message ) ; 
	}else if ( strcmp ( Head , "B" ) == 0 ) {// 聊天室清单
		ChatRoom_List ( fd );
	}else 
		print("\nSyu log None");
}
#endif

