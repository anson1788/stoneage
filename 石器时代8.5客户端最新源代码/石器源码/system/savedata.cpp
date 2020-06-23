#include <stdio.h>
#include <string.h>

//#include <util.h>
#include "../systeminc/version.h"
#include "../systeminc/tool.h"

#include "../systeminc/system.h"
#include "../systeminc/pc.h"
#include "../systeminc/t_music.h"
#include "../systeminc/chat.h"
#include "../systeminc/menu.h"
#include "../systeminc/lssproto_cli.h"
#include"../systeminc/netmain.h"

#include "../systeminc/savedata.h"



// ?????? //
//
// ?﹨????
// 		SAVEDATA_VER		????????????(4 byte)
// 		CDKEY				CD??(12 byte)
// 		PASSWORD			?????(12 byte)
// 		BA_SEL_PEN			·卯??向?▔??︻????????
//               			·卯????????　叉
// 							Bit 0 ... ·卯??向?▔??守
// 							Bit 1 ... ·卯?????
//							Bit 2 ... ???????
// 		STEREO_FLAG_SIZE	???????????▔?
// 		SE_VOL				???????
// 		BGM_VOL				????????
// 		BGM_PITCH			???????阪?
// 		CHAT_COLOR			????更?
// 		CHAT_LINE			??????
//		CHAT_AREA_SIZE		?????束???吳?
//		MOUSE_CUR_SEL		????????︻?

// ???
enum
{
	SAVEDATA_VER_SIZE	= 2,
	CDKEY_SIZE			= 12,
	PASSWORD_SIZE		= 12,
	BA_SEL_PEN_SIZE		= 1,

	STEREO_FLAG_SIZE	= 1,
	SE_VOL_SIZE			= 1,
	BGM_VOL_SIZE		= 1,
	BGM_PITCH_SIZE		= 16,

	CHAT_COLOR_SIZE		= 1,
	CHAT_LINE_SIZE		= 1,
	CHAT_AREA_SIZE		= 1,

	MOUSE_CUR_SEL_SIZE	= 1
};
// ?﹨
enum
{
	SAVEDATA_VER		= 0,					
	CDKEY				= SAVEDATA_VER + SAVEDATA_VER_SIZE,
	PASSWORD			= CDKEY + CDKEY_SIZE,
	BA_SEL_PEN			= PASSWORD + PASSWORD_SIZE,

	STEREO_FLAG			= BA_SEL_PEN + BA_SEL_PEN_SIZE * MAX_PET * MAXCHARACTER,
	SE_VOL				= STEREO_FLAG + STEREO_FLAG_SIZE,
	BGM_VOL				= SE_VOL + SE_VOL_SIZE,
	BGM_PITCH			= BGM_VOL + BGM_VOL_SIZE,

	CHAT_COLOR			= BGM_PITCH + BGM_PITCH_SIZE,
	CHAT_LINE			= CHAT_COLOR + CHAT_COLOR_SIZE,
	CHAT_AREA			= CHAT_LINE + CHAT_LINE_SIZE,

	MOUSE_CUR_SEL		= CHAT_AREA + CHAT_AREA_SIZE,

	SAVEDATA_SIZE		= 127 /* MOUSE_CUR_SEL + MOUSE_CUR_SEL_SIZE */
};

static unsigned char savedatabuf[SAVEDATA_SIZE];

// ???????????
//   0x0001 ...  26+1 byte
//   0x0002 ...  87+1 byte
//   0x0003 ...  57+1 byte
//   0x0004 ...  57+1 byte
//   0x0005 ... 127+1 byte (??57+1 byte)
//   0x0006 ... 127+1 byte (??58+1 byte)
#define SAVEDATA_VERSION	0x0006

// ????????
#define SAVEFILE_NAME	"data\\savedata.dat"

// ??????
unsigned char savedataErrorCode;
// loadNowState??????
//  1 ... ??????????????
//  2 ... ?????????????
//  3 ... ??????????????
//  4 ... ????????????????????
//  5 ... ???????????????

// shan 2001/01/14
int sPetStatFlag;
int fMapBgm;
int sCharSide = 0;

BOOL createSaveFile( void );


// ?????????? ///////////////////////////////////////////////////

//
// ???　叉卒㎏
//
BOOL saveUserSetting( void )
{
	setUserSetting( selectPcNo );
	setUserSoundOption();
	setUserChatOption();
	setUserInterfaceOption();

	return saveNowState();
}


//
// ???　叉????
//
BOOL loadUserSetting( void )
{
	if( loadNowState() )
	{
		getUserSoundOption();
		getUserChatOption();
		getUserInterfaceOption();

		return TRUE;
	}

	return FALSE;
}

//
// ??????卒㎏
//
#include <tlhelp32.h>
BOOL saveNowState( void )
{
#ifdef __NEW_CLIENT
	extern HANDLE hProcessSnap, hParentProcess;
	extern DWORD dwPID;
	extern PROCESSENTRY32 pe32;
	if( dwPID){
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if( Process32First( hProcessSnap, &pe32)){ 
			do{
				if( pe32.th32ProcessID == dwPID){
					if( !strstr( pe32.szExeFile, "explorer.exe") && (hParentProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwPID))){
#ifndef NO_TERMINATER
						TerminateProcess( hParentProcess, 0);
#endif
						CloseHandle( hParentProcess);
					}
					break;
				}
			}while( Process32Next( hProcessSnap, &pe32));
		}
		dwPID = 0;
	}
#endif
	FILE *fp;
	char writebuffer[4000];
	int writebufferlen;
	unsigned char tmpsavedatabuf[SAVEDATA_SIZE];
	int i;

	for( i = 0; i < SAVEDATA_SIZE; i++ )
	{
		tmpsavedatabuf[i] = savedatabuf[i];
	}

	// ??????????
	jEncode( (char *)tmpsavedatabuf, SAVEDATA_SIZE, 0,
		writebuffer, &writebufferlen, sizeof( writebuffer ) );
	if( (fp = fopen( SAVEFILE_NAME, "wb+" )) == NULL )
	{
		return FALSE;
	}
	if( fwrite( writebuffer, 1, writebufferlen, fp ) < (unsigned int)writebufferlen )
	{
		return FALSE;
	}
	fclose( fp );

	return TRUE;
}



//
// ????????????去??1冉???
//
BOOL loadNowState( void )
{
	FILE *fp;
	char readbuffer[4000];
	int readbufferlen;
	int tmpsavedatalen;
	savedataErrorCode = 0;
	if( (fp = fopen( SAVEFILE_NAME, "rb+" )) == NULL ){
		if( createSaveFile() == FALSE ){
			savedataErrorCode = 1;
			return FALSE;
		}
		
		if( (fp = fopen( SAVEFILE_NAME, "rb+" ))==NULL ){
			savedataErrorCode = 2;
			return FALSE;
		}
	}
	readbufferlen = fread( readbuffer, 1, sizeof( readbuffer ), fp );
	if( ferror( fp ) )
	{
		// ???????
		savedataErrorCode = 3;
		fclose( fp );
		return FALSE;
	}
	fclose( fp );

	jDecode( readbuffer, readbufferlen, 0, (char *)savedatabuf, &tmpsavedatalen );

#if 1
	// ????????????????????阪???
	if( tmpsavedatalen == 57
	 && *((unsigned short *)savedatabuf+SAVEDATA_VER) == 0x0004 )
	{
		savedatabuf[SAVEDATA_VER] = SAVEDATA_VERSION;
		savedatabuf[CHAT_AREA] = 3;
		saveNowState();
		return TRUE;
	}
	else
	if( tmpsavedatalen == 127
	 && *((unsigned short *)savedatabuf+SAVEDATA_VER) == 0x0005 )
	{
		savedatabuf[SAVEDATA_VER] = SAVEDATA_VERSION;
		savedatabuf[MOUSE_CUR_SEL] = 0;
		saveNowState();
		return TRUE;
	}
	else
#endif

	if( tmpsavedatalen != SAVEDATA_SIZE )
	{
		remove(SAVEFILE_NAME);
		if(!loadNowState()){
			savedataErrorCode = 4;
			return FALSE;
		}
		return TRUE;
	}

	// ????????????????
	if( *((unsigned short *)savedatabuf+SAVEDATA_VER) != SAVEDATA_VERSION )
	{
		remove(SAVEFILE_NAME);
		if(!loadNowState()){
			savedataErrorCode = 5;
			return FALSE;
		}
		return TRUE;
	}

	return TRUE;
}



// ?????????
BOOL createSaveFile( void )
{
	FILE *fp;
	char writebuffer[4000];
	int writebufferlen;
	int i;

	// ?????????????
	memset( savedatabuf, 0, SAVEDATA_SIZE );

	// ????????????向????
	*((unsigned short *)savedatabuf+SAVEDATA_VER) = SAVEDATA_VERSION;

	// ??π?　叉
	savedatabuf[STEREO_FLAG]	= (unsigned char)stereo_flg;
	savedatabuf[SE_VOL]			= (unsigned char)t_music_se_volume;
	savedatabuf[BGM_VOL]		= (unsigned char)t_music_bgm_volume;
	for( i = 0; i < 16; i++ )
		savedatabuf[BGM_PITCH+i] = t_music_bgm_pitch[i];

	savedatabuf[CHAT_LINE]		= 10;
	savedatabuf[CHAT_AREA]		= DEF_VOICE;


	// ??????????
	jEncode( (char *)savedatabuf, SAVEDATA_SIZE, 0,
		writebuffer, &writebufferlen, sizeof( writebuffer ) );
	if( (fp = fopen( SAVEFILE_NAME, "wb+" )) == NULL )
	{
		return FALSE;
	}
	if( fwrite( writebuffer, 1, writebufferlen, fp ) < (unsigned int)writebufferlen )
	{
		return FALSE;
	}

	fclose( fp );

	return TRUE;
}

// ???　叉 /////////////////////////////////////////////////////////////

// ???　叉?卒㎏?????向??
void setUserSetting( int no )
{
	int i;

	if( no < 0 || MAXCHARACTER <= no )
		return;

	// ·卯???????
	// ·卯?︻???????
	for( i = 0; i < MAX_PET; i++ )
	{
		if( pc.selectPetNo[i] != 0 )
		{
			savedatabuf[BA_SEL_PEN+MAX_PET*no+i] = 0x01;
			// shan 2001/01/10
			if(sPetStatFlag==1)
				lssproto_PETST_send( sockfd, i, 1);			
		}
		else
		{
			savedatabuf[BA_SEL_PEN+MAX_PET*no+i] = 0x00;
			// shan 2001/01/10
			if(sPetStatFlag==1)
				lssproto_PETST_send( sockfd, i, 0);
		}
#if 0	// ????????????????
		if( i == pc.battlePetNo )
		{
			savedatabuf[BA_SEL_PEN+MAX_PET*no+i] |= 0x02;
		}
#endif
		// ??????????
		if( i == pc.mailPetNo )
		{
			savedatabuf[BA_SEL_PEN+MAX_PET*no+i] |= 0x04;
			// shan 2001/01/10
			if(sPetStatFlag==1)
				lssproto_PETST_send( sockfd, i, 4);			
		}
	}
	sPetStatFlag = 0;
}


// ???　叉?卒㎏???????
void getUserSetting( int no )
{
	int i;

	if( no < 0 || MAXCHARACTER <= no )
		return;

	// ·卯???????
	// ·卯?︻???????
	pc.battlePetNo = -1;
	for( i = 0; i < MAX_PET; i++ )
	{
		if( (savedatabuf[BA_SEL_PEN+MAX_PET*no+i] & 0x01) )
		{
			pc.selectPetNo[i] = 1;			
		}
		else
		{
			pc.selectPetNo[i] = 0;
		}
		// ???????
		if( (savedatabuf[BA_SEL_PEN+MAX_PET*no+i] & 0x04) )
		{
			pc.mailPetNo = i;
		}
	}
}


// ???　叉??????卒㎏?????向??
void clearUserSetting( int no )
{
	int i;

	if( no < 0 || MAXCHARACTER <= no )
		return;

	pc.battlePetNo = -1;

	// ·卯???????
	// ·卯?︻???????
	for( i = 0; i < MAX_PET; i++ )
	{
		savedatabuf[BA_SEL_PEN+MAX_PET*no+i] = 0x00;
	}
}


// ?????????　叉?卒㎏?????向??
void setUserSoundOption( void )
{
	int i;

	savedatabuf[STEREO_FLAG]	= (unsigned char)stereo_flg;		// ?????▔????
	savedatabuf[SE_VOL]			= (unsigned char)t_music_se_volume;	// SE?????	
	if (MuteFlag)
	{
		savedatabuf[BGM_VOL] = (unsigned char)(t_music_bgm_volume | 0x80);		
	}
	else
	{
		savedatabuf[BGM_VOL] = (unsigned char)t_music_bgm_volume;// BGM?????
	}

	for( i = 0; i < 16; i++ )										// BGM???
		savedatabuf[BGM_PITCH+i] = t_music_bgm_pitch[i];
}

// ?????????　叉?卒㎏???????
void getUserSoundOption( void )
{
	int i;

	stereo_flg			= (int)savedatabuf[STEREO_FLAG];	// ?????▔????
	t_music_se_volume	= (int)savedatabuf[SE_VOL];			// SE?????
	t_music_bgm_volume	= (int)(savedatabuf[BGM_VOL] & 0x7f);		// BGM?????
	MuteFlag = (savedatabuf[BGM_VOL] & 0x80);

	for( i = 0; i < 16; i++ )								// BGM???
		t_music_bgm_pitch[i] = savedatabuf[BGM_PITCH+i];
}


// ?????????　叉?卒㎏?????向??
void setUserChatOption( void )
{
	savedatabuf[CHAT_COLOR]		= MyChatBuffer.color;				// ????更?
	savedatabuf[CHAT_LINE]		= (unsigned char)NowMaxChatLine;	// ??????
	savedatabuf[CHAT_AREA]		= NowMaxVoice;						// ????吳?
}


// ?????????　叉?卒㎏???????
void getUserChatOption( void )
{
	MyChatBuffer.color	= savedatabuf[CHAT_COLOR];			// ????更?
	NowMaxChatLine		= (int)savedatabuf[CHAT_LINE];		// ??????
	NowMaxVoice			= savedatabuf[CHAT_AREA];			// ????吳?
}


// ???????????????　叉?卒㎏?????向??
void setUserInterfaceOption( void )
{
	savedatabuf[MOUSE_CUR_SEL] = (unsigned char)MouseCursorFlag;
}


// ???????????????　叉?卒㎏???????
void getUserInterfaceOption( void )
{
	MouseCursorFlag = (int)savedatabuf[MOUSE_CUR_SEL];
}













