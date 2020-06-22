#ifndef __CHATROOM_H__
#define __CHATROOM_H__
#include "version.h"

#define MAX_CHATROOM 10 
#define MAX_PPLINROOM 100

void InitChatRoom ( void ) ; 
void ChatRoom_List ( int fd );
BOOL ChatCheck_BeMaster( int myindex, int chatnum);
BOOL ChatCheck_Free( int myindex);
BOOL ChatRoom_Create ( int myindex , char *message);
BOOL ChatRoom_Destroy ( int myindex ) ; 
void ChatRoom_Kick ( int myindex , int toindex ) ; 
void ChatRoom_Make ( int myindex , int toindex ) ; 
void ChatRoom_Leave ( int myindex ) ; 
void ChatRoom_Join ( int myindex , int num ) ; 
void ChatRoom_Agree ( int myindex , int toindex , int YesNo ) ; 
void ChatRoom_Message ( int myindex , char *message ) ; 
void ChatRoom_Refresh ( int Num ) ; 
void ChatRoom_recvall ( int fd , char *data ) ; 
#endif
