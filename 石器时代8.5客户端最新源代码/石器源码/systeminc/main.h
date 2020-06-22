// main.cpp ???????

#ifndef _MAIN_H_
#define _MAIN_H_

#include "version.h"

//---------------------------------------------------------------------------//
// ?? £º????define)¡L?                                                //
//---------------------------------------------------------------------------//

extern int DEF_APPSIZEX;
extern int DEF_APPSIZEY;
extern int SCREEN_WIDTH_CENTER;
extern int SCREEN_HEIGHT_CENTER;

//---------------------------------------------------------------------------//
// ?? £º????????¡L?                                               //
//---------------------------------------------------------------------------//

int PASCAL WinMain( HINSTANCE 		hInstance, 	HINSTANCE 		hPrevInstance, 	LPSTR lpCmdLine, 	int nCmdShow );	// ?????

LRESULT CALLBACK PASCAL WindMsgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );				//??????????
BOOL SystemTask( void );																		// ??????????

// ?????¨Á?
extern HWND hWnd;		// ?????????
extern BOOL WindowMode;		// ????????
extern int ResoMode;
extern int LowResoCmdFlag;		// ??¥T¨Á????????
extern int	CmdShow;	// Wi5nMain??????????????
extern LPSTR CmdLine; 	// WinMain?????????????????????
extern HANDLE hMutex;	// ¥i??¥h???????????????

extern char realBinName[];
extern char adrnBinName[];
extern char sprBinName[];
extern char sprAdrnBinName[];
#ifdef _READ16BITBMP
extern char realtrueBinName[];
extern char adrntrueBinName[];
#endif
//add jeffrey
extern struct gameserver gmsv[];
extern struct gamegroup  gmgroup[];
//end

extern BOOL offlineFlag;
extern char DebugKey0[ 256 ];		
extern char DebugKey1[ 256 ];		
extern char DebugKey2[ 256 ];		


#endif

