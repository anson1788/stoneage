#ifndef _SAVEDATA_H_
#define _SAVEDATA_H_

BOOL saveUserSetting( void );
BOOL loadUserSetting( void );
void setUserSetting( int );
void getUserSetting( int );
void clearUserSetting( int );
void setUserSoundOption( void );
void getUserSoundOption( void );
void setUserChatOption( void );
void getUserChatOption( void );
void setUserInterfaceOption( void );
void getUserInterfaceOption( void );

BOOL saveNowState( void );
BOOL loadNowState( void );

extern unsigned char savedataErrorCode;
// shan 2001/01/14
extern int sPetStatFlag;
extern int fMapBgm;
extern int sCharSide;


// ????????
#define SAVE_ERRMSG_loadNowState	"黍揣湔紫偶囮啖﹝(code = %d)"



#endif // _SAVEDATA_H_
