/**** SYSTEM INCLUDE ****/
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<direct.h>
#include<errno.h>

#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/loadrealbin.h"
#include "../systeminc/map.h"
#include "../systeminc/anim_tbl.h"

#define PAL_CHANGE_INTERVAL_WIN		120		// ??????????????????
#define PAL_CHANGE_INTERVAL_FULL	60		// ????????????????????
int MessageBoxNew(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
#ifdef _NEW_COLOR_
extern int NewColor16Flg;
#endif
//---------------------------------------------------------------------------//
// ???????                                                        //
//---------------------------------------------------------------------------//
DIRECT_DRAW	*lpDraw;		// DirectDraw??????????
BOOL DDinitFlag = FALSE;	// DirectDraw??????

HFONT hFont;				// ????????

int displayBpp;			// ??

int rBitLShift;				// ?????????
int gBitLShift;				// ?????????
int bBitLShift;				// ?????????

int rBitRShift;				// ?????????
int gBitRShift;				// ?????????
int bBitRShift;				// ?????????

PALETTEENTRY	Palette[256];	// ?????
PALETTE_STATE 	PalState;		// ?????
BOOL 			PalChangeFlag;	// ???????????
#ifdef _HI_COLOR_32
unsigned int highColor32Palette[256];
#endif
unsigned short highColorPalette[256];

#ifdef _READ16BITBMP
extern RGBQUAD g_rgbPal[256];
BOOL g_bUseAlpha = FALSE;
#endif
#ifdef _NEW_COLOR_
extern BOOL g_bUseAlpha = FALSE;
#endif

#ifdef _READ16BITBMPVARIABLES	//关 _READ16BITBMP 后还须要的参数
BOOL g_bUseAlpha = FALSE;
#endif

#ifdef _TALK_WINDOW
BOOL g_bTalkWindow = FALSE;
#endif

#ifdef _STONDEBUG_
extern int g_iMallocCount;
#endif

void SetAnimTbl();

// ???????????
char *palFileName[] = {
#include "../systeminc/palName.h"
};

const int MAX_PAL = sizeof(palFileName) / sizeof(palFileName[0]);
// ????????????????
int getBitCount(int bit)
{
	int i, j, k;
	j = 1;
	k = 0;
	for (i = 0; i < sizeof(int)* 8; i++){
		if ((bit & j))
			k++;
		j <<= 1;
	}
	return k;
}

#ifdef SWITCH_MODE
DEVMODE g_OriginalMode;
#endif

BOOL InitDirectDraw(void)
{
	DWORD dwWriteByte;
	HANDLE hErrorLogFile;
	HRESULT hResult;
	char szErrMsg[256];
	DDSCAPS ddscaps;		// ?????????????
	if ((hResult = DirectDrawCreate(NULL, &lpDraw->lpDD, NULL)) != DD_OK){
		hErrorLogFile = CreateFile("ErrorLog.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		sprintf_s(szErrMsg, "DirectDrawCreate error(1):error result (%x)", hResult);
		WriteFile(hErrorLogFile, szErrMsg, sizeof(szErrMsg), &dwWriteByte, NULL);
		CloseHandle(hErrorLogFile);
		if ((hResult = DirectDrawCreate((GUID *)DDCREATE_EMULATIONONLY, &lpDraw->lpDD, NULL)) != DD_OK){
			MessageBoxNew(hWnd, "DirectDrawCreate Error", "确定", MB_OK | MB_ICONSTOP);
			hErrorLogFile = CreateFile("ErrorLog.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			sprintf_s(szErrMsg, "DirectDrawCreate error(2):error result (%x)", hResult);
			WriteFile(hErrorLogFile, szErrMsg, sizeof(szErrMsg), &dwWriteByte, NULL);
			CloseHandle(hErrorLogFile);
			return FALSE;
		}
	}
	if ((hResult = lpDraw->lpDD->QueryInterface(IID_IDirectDraw2, (LPVOID *)&lpDraw->lpDD2)) != DD_OK){
		MessageBoxNew(hWnd, "QueryInterface Error", "确定", MB_OK | MB_ICONSTOP);
		hErrorLogFile = CreateFile("ErrorLog.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		sprintf_s(szErrMsg, "QueryInterface error:error result (%x)", hResult);
		WriteFile(hErrorLogFile, szErrMsg, sizeof(szErrMsg), &dwWriteByte, NULL);
		CloseHandle(hErrorLogFile);
		return FALSE;
	}
#ifdef _BACK_WINDOW
#undef _BACK_VERSION
#endif
#ifndef _BACK_VERSION
	if (WindowMode){
#ifdef SWITCH_MODE
		if( g_OriginalMode.dmSize == 0){
			g_OriginalMode.dmSize = sizeof(DEVMODE);
			EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &g_OriginalMode);
		}
		if( displayBpp!=(int)g_OriginalMode.dmBitsPerPel || lpDraw->xSize!=(int)g_OriginalMode.dmPelsWidth || lpDraw->ySize!=(int)g_OriginalMode.dmPelsHeight){
			DEVMODE	DevMode;
			int done = 0, iMode = 0;
			DevMode.dmSize = sizeof(DEVMODE);
			while( EnumDisplaySettings( NULL, iMode, &DevMode)){
				if( (int)DevMode.dmBitsPerPel==displayBpp && DevMode.dmPelsWidth==g_OriginalMode.dmPelsWidth && DevMode.dmPelsHeight==g_OriginalMode.dmPelsHeight){
					ChangeDisplaySettingsEx( NULL, &DevMode, NULL, CDS_UPDATEREGISTRY, NULL);
					done = 1;
					break;
				}
				iMode++;
				DevMode.dmSize = sizeof(DEVMODE);
			}
			if( done == 0){
				iMode = 0;
				DevMode.dmSize = sizeof(DEVMODE);
				while( EnumDisplaySettings( NULL, iMode, &DevMode)){
					if( (int)DevMode.dmBitsPerPel==displayBpp && (int)DevMode.dmPelsWidth>=lpDraw->xSize && (int)DevMode.dmPelsHeight>=lpDraw->ySize){
						DevMode.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
						ChangeDisplaySettingsEx( NULL, &DevMode, NULL, CDS_UPDATEREGISTRY, NULL);
						break;
					}
					iMode++;
					DevMode.dmSize = sizeof(DEVMODE);
				}
				if( done == 0)   return FALSE;
			}
		}
#endif
		if (lpDraw->lpDD2->SetCooperativeLevel(hWnd, DDSCL_NORMAL) != DD_OK){
			MessageBoxNew(hWnd, "SetCooperativeLevel Error", "确定", MB_OK | MB_ICONSTOP);
			return FALSE;
		}

		ZeroMemory(&lpDraw->ddsd, sizeof(lpDraw->ddsd));
		lpDraw->ddsd.dwSize = sizeof(lpDraw->ddsd);
		lpDraw->ddsd.dwFlags = DDSD_CAPS;
		lpDraw->ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		if ((hResult = lpDraw->lpDD2->CreateSurface(&lpDraw->ddsd, &lpDraw->lpFRONTBUFFER, NULL)) != DD_OK){
			MessageBoxNew(hWnd, "主画面处理失败。", "确定", MB_OK | MB_ICONSTOP);
			hErrorLogFile = CreateFile("ErrorLog.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			sprintf_s(szErrMsg, "Create frontbuffer error(1):error result (%x)", hResult);
			WriteFile(hErrorLogFile, szErrMsg, sizeof(szErrMsg), &dwWriteByte, NULL);
			CloseHandle(hErrorLogFile);
			return FALSE;
		}
		if (lpDraw->lpDD2->CreateClipper(0, &lpDraw->lpCLIPPER, NULL) != DD_OK){
			MessageBoxNew(hWnd, "clipper处理失败。", "确定", MB_OK | MB_ICONSTOP);
			return FALSE;
		}
		lpDraw->lpCLIPPER->SetHWnd(0, hWnd);
		lpDraw->lpFRONTBUFFER->SetClipper(lpDraw->lpCLIPPER);
		ZeroMemory(&lpDraw->ddsd, sizeof(lpDraw->ddsd));
		lpDraw->ddsd.dwSize = sizeof(lpDraw->ddsd);
		lpDraw->ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		lpDraw->ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		lpDraw->ddsd.dwWidth = lpDraw->xSize;
		lpDraw->ddsd.dwHeight = lpDraw->ySize;
		if ((hResult = lpDraw->lpDD2->CreateSurface(&lpDraw->ddsd, &lpDraw->lpBACKBUFFER, NULL)) != DD_OK){
			MessageBoxNew(hWnd, "暂存区处理失败", "确定", MB_OK | MB_ICONSTOP);
			hErrorLogFile = CreateFile("ErrorLog.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			sprintf_s(szErrMsg, "Create backbuffer error:error result (%x)", hResult);
			WriteFile(hErrorLogFile, szErrMsg, sizeof(szErrMsg), &dwWriteByte, NULL);
			CloseHandle(hErrorLogFile);
			return FALSE;
		}
#ifdef _READ16BITBMP
		if(g_bUseAlpha){
			lpDraw->ddsd.ddsCaps.dwCaps    = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
			if((hResult = lpDraw->lpDD2->CreateSurface(&lpDraw->ddsd,&lpDraw->lpBACKBUFFERSYS,NULL)) != DD_OK){
				MessageBoxNew(hWnd,"暂存区处理失败(sys)","确定",MB_OK | MB_ICONSTOP);
				hErrorLogFile = CreateFile("ErrorLog.txt",GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
				sprintf_s(szErrMsg,"Create backbuffer error:error result (%x)",hResult);
				WriteFile(hErrorLogFile,szErrMsg,sizeof(szErrMsg),&dwWriteByte,NULL);
				CloseHandle(hErrorLogFile);
				return FALSE;
			}
		}
#endif
	}
	else
#endif
#ifdef _BACK_WINDOW
#define _BACK_VERSION
#endif
	{
		if (lpDraw->lpDD2->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX) != DD_OK){
			MessageBoxNew(hWnd, "SetCooperativeLevel Error", "确定", MB_OK | MB_ICONSTOP);
			return FALSE;
		}
		lpDraw->lpDD2->SetDisplayMode(lpDraw->xSize, lpDraw->ySize, displayBpp, 0, 0);
		ZeroMemory(&lpDraw->ddsd, sizeof(lpDraw->ddsd));
		lpDraw->ddsd.dwSize = sizeof(lpDraw->ddsd);
		lpDraw->ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		lpDraw->ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
		lpDraw->ddsd.dwBackBufferCount = 1;
		if ((hResult = lpDraw->lpDD2->CreateSurface(&lpDraw->ddsd, &lpDraw->lpFRONTBUFFER, NULL)) != DD_OK){
			MessageBoxNew(hWnd, "主画面处理失败二。", "确定", MB_OK | MB_ICONSTOP);
			hErrorLogFile = CreateFile("ErrorLog.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			sprintf_s(szErrMsg, "Create frontbuffer error(2):error result (%x)", hResult);
			WriteFile(hErrorLogFile, szErrMsg, sizeof(szErrMsg), &dwWriteByte, NULL);
			CloseHandle(hErrorLogFile);
			return FALSE;
		}
		ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
		lpDraw->lpFRONTBUFFER->GetAttachedSurface(&ddscaps, &lpDraw->lpBACKBUFFER);
#ifdef _READ16BITBMP
		if(g_bUseAlpha){
			ZeroMemory( &lpDraw->ddsd, sizeof( lpDraw->ddsd ) ); 
			lpDraw->ddsd.dwSize = sizeof( lpDraw->ddsd );
			lpDraw->ddsd.dwFlags           = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
			lpDraw->ddsd.ddsCaps.dwCaps    = DDSCAPS_OFFSCREENPLAIN;
			lpDraw->ddsd.dwWidth           = lpDraw->xSize;
			lpDraw->ddsd.dwHeight          = lpDraw->ySize;
			lpDraw->ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
			if((hResult = lpDraw->lpDD2->CreateSurface(&lpDraw->ddsd,&lpDraw->lpBACKBUFFERSYS,NULL)) != DD_OK){
				MessageBoxNew(hWnd,"暂存区处理失败二(sys)","确定",MB_OK | MB_ICONSTOP);
				hErrorLogFile = CreateFile("ErrorLog.txt",GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
				sprintf_s(szErrMsg,"Create backbuffer error:error result (%x)",hResult);
				WriteFile(hErrorLogFile,szErrMsg,sizeof(szErrMsg),&dwWriteByte,NULL);
				CloseHandle(hErrorLogFile);
				return FALSE;
			}
		}
#endif
	}
	// WON REM 
#ifdef _HI_COLOR_32
	if (displayBpp == 32){
		if (lpDraw->lpBACKBUFFER == NULL)
			return FALSE;

		DDPIXELFORMAT ddPixelFormat;
		ZeroMemory(&ddPixelFormat, sizeof(DDPIXELFORMAT));
		ddPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

		if (lpDraw->lpBACKBUFFER->GetPixelFormat(&ddPixelFormat) != DD_OK)
			return FALSE;

		rBitRShift = 8 - getBitCount(ddPixelFormat.dwRBitMask);	// ?????????
		gBitRShift = 8 - getBitCount(ddPixelFormat.dwGBitMask);	// ?????????
		bBitRShift = 8 - getBitCount(ddPixelFormat.dwBBitMask);	// ?????????

		rBitLShift = getBitCount(ddPixelFormat.dwBBitMask) + getBitCount(ddPixelFormat.dwGBitMask);	// ?????????
		gBitLShift = getBitCount(ddPixelFormat.dwBBitMask);							// ?????????
		bBitLShift = 0;													// ?????????
	}
	else

#endif
#ifdef _HI_COLOR_16
		// ??????????????????
	if (displayBpp == 16){
		// ??????????NULL????
		if (lpDraw->lpBACKBUFFER == NULL)
			return FALSE;

		DDPIXELFORMAT ddPixelFormat;	// ?????
		// ?????
		ZeroMemory(&ddPixelFormat, sizeof(DDPIXELFORMAT));
		ddPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

		if (lpDraw->lpBACKBUFFER->GetPixelFormat(&ddPixelFormat) != DD_OK)
			return FALSE;

		rBitRShift = 8 - getBitCount(ddPixelFormat.dwRBitMask);	// ?????????
		gBitRShift = 8 - getBitCount(ddPixelFormat.dwGBitMask);	// ?????????
		bBitRShift = 8 - getBitCount(ddPixelFormat.dwBBitMask);	// ?????????

		rBitLShift = getBitCount(ddPixelFormat.dwBBitMask) + getBitCount(ddPixelFormat.dwGBitMask);	// ?????????
		gBitLShift = getBitCount(ddPixelFormat.dwBBitMask);							// ?????????
		bBitLShift = 0;													// ?????????
	}

#endif
	// ??????????
	if ((lpBattleSurface = CreateSurface(DEF_APPSIZEX, DEF_APPSIZEY, DEF_COLORKEY, DDSCAPS_VIDEOMEMORY)) == NULL){
#ifdef _STONDEBUG_
		MessageBoxNew( hWnd ,"建立VideoRam BattleSurface失败！" ,"确定",MB_OK | MB_ICONSTOP );
#endif
		if ((lpBattleSurface = CreateSurface(DEF_APPSIZEX, DEF_APPSIZEY, DEF_COLORKEY, DDSCAPS_SYSTEMMEMORY)) == NULL){
			MessageBoxNew(hWnd, "建立BattleSurface失败！", "确定", MB_OK | MB_ICONSTOP);
			return FALSE;
		}
	}
#ifdef _READ16BITBMP
	if(g_bUseAlpha){
		if((lpBattleSurfaceSys = CreateSurface(DEF_APPSIZEX,DEF_APPSIZEY,DEF_COLORKEY,DDSCAPS_SYSTEMMEMORY)) == NULL){
			MessageBoxNew(hWnd,"建立BattleSurface(sys)失败！","确定",MB_OK | MB_ICONSTOP);
			return FALSE;
		}
	}
#endif
#ifdef __SKYISLAND
	ClearSurface(lpBattleSurface);
#ifdef _READ16BITBMP
	if(g_bUseAlpha) ClearSurface(lpBattleSurfaceSys);
#endif
	extern void SkyIslandLoadBmp();
	SkyIslandLoadBmp();
#endif
	DDinitFlag = TRUE;
	SetAnimTbl();
	return TRUE;
}

BOOL InitPalette(void)
{
	int i;
	FILE *fp;

	PALETTEENTRY pal[32] = {
		// ?????
		{ 0x00, 0x00, 0x00, PC_NOCOLLAPSE | PC_RESERVED }, // 0:?
		{ 0x80, 0x00, 0x00, PC_NOCOLLAPSE | PC_RESERVED }, // 1:??
		{ 0x00, 0x80, 0x00, PC_NOCOLLAPSE | PC_RESERVED }, // 2:???
		{ 0x80, 0x80, 0x00, PC_NOCOLLAPSE | PC_RESERVED }, // 3:???
		{ 0x00, 0x00, 0x80, PC_NOCOLLAPSE | PC_RESERVED }, // 4:??
		{ 0x80, 0x00, 0x80, PC_NOCOLLAPSE | PC_RESERVED }, // 5:???
		{ 0x00, 0x80, 0x80, PC_NOCOLLAPSE | PC_RESERVED }, // 6:???
		{ 0xc0, 0xc0, 0xc0, PC_NOCOLLAPSE | PC_RESERVED }, // 7:???
		{ 0xc0, 0xdc, 0xc0, PC_NOCOLLAPSE | PC_RESERVED }, // 8:?
		{ 0xa6, 0xca, 0xf0, PC_NOCOLLAPSE | PC_RESERVED }, // 9:?

		//新系统色盘// ????????
		{ 0xde, 0x00, 0x00, PC_NOCOLLAPSE | PC_RESERVED },
		{ 0xff, 0x5f, 0x00, PC_NOCOLLAPSE | PC_RESERVED },
		{ 0xff, 0xff, 0xa0, PC_NOCOLLAPSE | PC_RESERVED },
		{ 0x00, 0x5f, 0xd2, PC_NOCOLLAPSE | PC_RESERVED },
		{ 0x50, 0xd2, 0xff, PC_NOCOLLAPSE | PC_RESERVED },
		{ 0x28, 0xe1, 0x28, PC_NOCOLLAPSE | PC_RESERVED },

		//新系统色盘// ????????
		{ 0xf5, 0xc3, 0x96, PC_NOCOLLAPSE | PC_RESERVED },
		{ 0xe1, 0xa0, 0x5f, PC_NOCOLLAPSE | PC_RESERVED },
		{ 0xc3, 0x7d, 0x46, PC_NOCOLLAPSE | PC_RESERVED },
		{ 0x9b, 0x55, 0x1e, PC_NOCOLLAPSE | PC_RESERVED },
		{ 0x46, 0x41, 0x37, PC_NOCOLLAPSE | PC_RESERVED },
		{ 0x28, 0x23, 0x1e, PC_NOCOLLAPSE | PC_RESERVED },

		// ?????
		{ 0xff, 0xfb, 0xf0, PC_NOCOLLAPSE | PC_RESERVED }, // 246:?
		{ 0xa0, 0xa0, 0xa4, PC_NOCOLLAPSE | PC_RESERVED }, // 247:?
		{ 0x80, 0x80, 0x80, PC_NOCOLLAPSE | PC_RESERVED }, // 248:?
		{ 0xff, 0x00, 0x00, PC_NOCOLLAPSE | PC_RESERVED }, // 249:
		{ 0x00, 0xff, 0x00, PC_NOCOLLAPSE | PC_RESERVED }, // 250:?
		{ 0xff, 0xff, 0x00, PC_NOCOLLAPSE | PC_RESERVED }, // 251:?
		{ 0x00, 0x00, 0xff, PC_NOCOLLAPSE | PC_RESERVED }, // 252:
		{ 0xff, 0x00, 0xff, PC_NOCOLLAPSE | PC_RESERVED }, // 253:?
		{ 0x00, 0xff, 0xff, PC_NOCOLLAPSE | PC_RESERVED }, // 254:?
		{ 0xff, 0xff, 0xff, PC_NOCOLLAPSE | PC_RESERVED }  // 255:?
	};


	// ?????????
	for (i = 0; i < 10; i++){
		Palette[i].peBlue = pal[i].peBlue;
		Palette[i].peGreen = pal[i].peGreen;
		Palette[i].peRed = pal[i].peRed;
		Palette[i].peFlags = PC_EXPLICIT;

		Palette[i + 246].peBlue = pal[i + 22].peBlue;
		Palette[i + 246].peGreen = pal[i + 22].peGreen;
		Palette[i + 246].peRed = pal[i + 22].peRed;
		Palette[i + 246].peFlags = PC_EXPLICIT;
	}

	// ?????????
	for (i = 0; i < 6; i++){
		Palette[i + 10].peBlue = pal[i + 10].peBlue;
		Palette[i + 10].peGreen = pal[i + 10].peGreen;
		Palette[i + 10].peRed = pal[i + 10].peRed;
		Palette[i + 10].peFlags = PC_NOCOLLAPSE | PC_RESERVED;

		Palette[i + 240].peBlue = pal[i + 16].peBlue;
		Palette[i + 240].peGreen = pal[i + 16].peGreen;
		Palette[i + 240].peRed = pal[i + 16].peRed;
		Palette[i + 240].peFlags = PC_NOCOLLAPSE | PC_RESERVED;
	}

	//只有第一次才作(没有初始化时)// ????????????????
	if (PalState.flag == FALSE){
		fp = fopen(palFileName[0], "rb");
		if (fp == NULL){
			MessageBoxNew(hWnd, "色盘档读取失败", "Error", MB_OK | MB_ICONSTOP);
			return FALSE;
		}
		else{
			//可自由使用的调色盘设定// ?????????
			for (i = 16; i < 240; i++){
				//档案读入// ????????
				Palette[i].peBlue = fgetc(fp);
				Palette[i].peGreen = fgetc(fp);
				Palette[i].peRed = fgetc(fp);
				Palette[i].peFlags = PC_NOCOLLAPSE | PC_RESERVED;
			}
			fclose(fp);
		}
	}
	if (!transmigrationEffectFlag)
	{
		Palette[168].peBlue = 0;
		Palette[168].peGreen = 0;
		Palette[168].peRed = 0;
		//Palette[168].peFlags 	= PC_EXPLICIT;
	}

#if 0
	else{
		// ?????????
		for( i = 16; i < 240; i++ ){
			// ??????????
			if( WindowMode ){
				Palette[i].peFlags = PC_NOCOLLAPSE | PC_RESERVED;
			}else{
				Palette[i].peFlags = PC_EXPLICIT;
			}
		}
	}
#endif
	lpDraw->lpDD2->CreatePalette(DDPCAPS_8BIT, Palette, &lpDraw->lpPALETTE, NULL);
	if (lpDraw->lpPALETTE == NULL){
		MessageBoxNew(hWnd, "调色盘处理失败", "Error", MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	// WON REM 
#ifdef _HI_COLOR_16
	//#ifdef _STONDEBUG_
	if (displayBpp == 8){
		if (lpDraw->lpFRONTBUFFER->SetPalette(lpDraw->lpPALETTE) != DD_OK){
			MessageBoxNew(hWnd, "调色盘处理失败", "Error", MB_OK);
			MessageBoxNew(hWnd, "请使用１６位元高彩色或３２位元高彩色模示", "Error", MB_OK);
			return FALSE;
		}
	}
#else
	// Robin 05/02
#ifdef SWITCH_MODE
	if( lpDraw->lpFRONTBUFFER->SetPalette( lpDraw->lpPALETTE ) != DD_OK ){
		if( MessageBoxNew(hWnd, "请使用２５６色的显示模示", "确定", MB_RETRYCANCEL | MB_ICONEXCLAMATION ) == IDCANCEL)
			return FALSE;
		return FALSE;
	}
#else
	while( lpDraw->lpFRONTBUFFER->SetPalette( lpDraw->lpPALETTE ) != DD_OK ){
		if( MessageBoxNew(hWnd, "请使用２５６色的显示模示", "确定", MB_RETRYCANCEL | MB_ICONEXCLAMATION ) == IDCANCEL)
			return FALSE;
	}
#endif

#endif

#ifdef _HI_COLOR_32
	if (displayBpp == 32){
		highColor32Palette[0] = 0;
		for (i = 1; i < 256; i++){
			highColor32Palette[i] =
				((Palette[i].peBlue >> bBitRShift) << bBitLShift)
				+ ((Palette[i].peGreen >> gBitRShift) << gBitLShift)
				+ ((Palette[i].peRed >> rBitRShift) << rBitLShift);
		}
	}
	else
#endif
	if (displayBpp == 16){
		// ??????????????
		highColorPalette[0] = 0;
		for (i = 1; i < 256; i++){
			highColorPalette[i] =
				((Palette[i].peBlue >> bBitRShift) << bBitLShift)
				//cary 2001 10 16
				+ (((Palette[i].peGreen >> gBitRShift) | 1) << gBitLShift)
				+ ((Palette[i].peRed >> rBitRShift) << rBitLShift);
		}
	}

	PalState.flag = TRUE;
	return TRUE;
}

// ???????? ***********************************************************/
void PaletteChange(int palNo, int time)
{
	// ????????
	if (palNo >= MAX_PAL)
		return;
	// ?????
	PalState.palNo = palNo;
	// ???????
	PalState.time = time;
	// ????????
	if (PalState.time <= 0)
		PalState.time = 1;
}

#ifdef _CACHE_SURFACE_

BOOL IsSurfaceExpired(SURFACE_INFO *surface)
{
	return (surface->palNo != PalState.palNo);
}

#endif

// 色盘处理 ***************************************************************/
void PaletteProc(void)
{
	FILE *fp; // ????????
	static PALETTEENTRY	pal[256];	// ?????
	static float	dRed[256];		// ?????
	static float	dGreen[256];	// ?????
	static float	dBlue[256];		// ?????
	static float	dRedBak[256];	// ?????
	static float	dGreenBak[256];	// ?????
	static float	dBlueBak[256];	// ?????
	static int 	timeCnt;			// ????????
	static int 	changeCnt;			// ?????????
	static int 	palNoBak = 0;		// ?????
	static int 	openFlag = FALSE;	// ???????????
	int i;
	// ????????????
	if (palNoBak == PalState.palNo && openFlag == FALSE)
		return;
	// ???????????
	if (palNoBak != PalState.palNo){
		// ????????????
		fp = fopen(palFileName[PalState.palNo], "rb");
		// ?????????
		for (i = 16; i < 240; i++){
			pal[i].peBlue = fgetc(fp);
			pal[i].peGreen = fgetc(fp);
			pal[i].peRed = fgetc(fp);
			// 168??( 0, 0, 0 )??
			if (i == 168){
				pal[168].peBlue = 0;
				pal[168].peGreen = 0;
				pal[168].peRed = 0;
			}
			// ?????
			dBlueBak[i] = Palette[i].peBlue;
			dGreenBak[i] = Palette[i].peGreen;
			dRedBak[i] = Palette[i].peRed;
			// ??
			dBlue[i] = (float)(pal[i].peBlue - Palette[i].peBlue) / (float)PalState.time;
			dGreen[i] = (float)(pal[i].peGreen - Palette[i].peGreen) / (float)PalState.time;
			dRed[i] = (float)(pal[i].peRed - Palette[i].peRed) / (float)PalState.time;
#if 0
			// ??????????
			if( WindowMode ){
				Palette[i].peFlags = PC_NOCOLLAPSE | PC_RESERVED;
			}else{
				Palette[i].peFlags = PC_EXPLICIT;
			}
#endif
		}
		fclose(fp);				// ??????
		timeCnt = 0;				// ????????????????????
		changeCnt = 0;				// ????????????????????;
		palNoBak = PalState.palNo;	// ??????
		openFlag = TRUE;			// ?????
	}
	// ?????
	timeCnt++;
	// ????
	if (timeCnt <= PalState.time){
		// ?????????
		for (i = 16; i < 240; i++){
			// ?????
			dBlueBak[i] += dBlue[i];
			dGreenBak[i] += dGreen[i];
			dRedBak[i] += dRed[i];
			// ?????
			Palette[i].peBlue = (BYTE)dBlueBak[i];
			Palette[i].peGreen = (BYTE)dGreenBak[i];
			Palette[i].peRed = (BYTE)dRedBak[i];
		}
	}
	else{
		// ????????????????????
		// ?????????
		for (i = 16; i < 240; i++){
			Palette[i].peBlue = pal[i].peBlue;
			Palette[i].peGreen = pal[i].peGreen;
			Palette[i].peRed = pal[i].peRed;
		}
		openFlag = FALSE;	// ??????
		transEffectPaletteStatus = 2;
	}
	// ??????????????????
	if (changeCnt == 0 || openFlag == FALSE){
		// ?????????????
		PalChangeFlag = TRUE;
		// ???????
		//lpDraw->lpPALETTE->SetEntries( 0, 0, 256, Palette );
	}
	// ????????
	changeCnt++;
	// ??????????
	if (transmigrationEffectFlag)
	{
		// ??????
		if (changeCnt >= 10) changeCnt = 0;
	}
	if (WindowMode){
		if (changeCnt >= PAL_CHANGE_INTERVAL_WIN)
			changeCnt = 0;
	}
	else{
		if (changeCnt >= PAL_CHANGE_INTERVAL_FULL)
			changeCnt = 0;
	}
}

//---------------------------------------------------------------------------//
// ?? ：???????????????????????????       //
// ?? ：??                         										 //
// ? ：??                                                               //
//---------------------------------------------------------------------------//

extern	RECT	g_clientRect;
extern	POINT	g_clientPoint;
extern	RECT	g_moveRect;
#ifdef _MO_SHOW_FPS
int framesToShow;
int skipFramesToShow;
extern int	  NoDrawCnt;
void CalculateFrameRate()
{
	static int framesSkipPerSecond = 0;
	static float framesPerSecond = 0.0f;       // This will store our fps
	static float lastTime = 0.0f;       // This will hold the time from the last frame
	float currentTime = TimeGetTime() * 0.001f;
	++framesPerSecond;
	framesSkipPerSecond += NoDrawCnt - 1;

	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		framesToShow= int(framesPerSecond);
		framesPerSecond = 0;
		skipFramesToShow = framesSkipPerSecond;
		framesSkipPerSecond = 0;
	}
}
#endif


void Flip(void)
{
#ifdef _MO_SHOW_FPS
	CalculateFrameRate();
#endif

	if (WindowMode == TRUE){
		lpDraw->lpFRONTBUFFER->Blt(&g_clientRect, lpDraw->lpBACKBUFFER, &g_moveRect, DDBLT_WAIT, NULL);
	}
	else
	{
		lpDraw->lpDD2->WaitForVerticalBlank(DDWAITVB_BLOCKEND, NULL);
		lpDraw->lpFRONTBUFFER->Flip(NULL, DDFLIP_WAIT);
	}
	return;
}

//---------------------------------------------------------------------------//
// ?? ：?????????????                                         //
// ?? ：DIRECT_DRAW *lpDraw : DirectDraw???                         //
// ? ：??                                                               //
//---------------------------------------------------------------------------//
void ClearBackSurface(void)
{
	DDBLTFX ddbltfx;

	ZeroMemory(&ddbltfx, sizeof(DDBLTFX));
	ddbltfx.dwSize = sizeof(DDBLTFX);
#ifdef _STONDEBUG_
	QueryPerformanceCounter(&tf);
	iTotalProcTime = (int)tf.QuadPart;
#endif
	HRESULT hr = lpDraw->lpBACKBUFFER->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
#ifdef _READ16BITBMP
	if(g_bUseAlpha){
		lpDraw->lpBACKBUFFERSYS->Blt(NULL,NULL,NULL,DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);
	}
#endif
#ifdef _STONDEBUG_
	QueryPerformanceCounter(&tf);
	iTotalUseTime += (((int)tf.QuadPart - iTotalProcTime)) /100;
	iTotalRunCount++;
#endif
}

//---------------------------------------------------------------------------//
// ?? ：??????????                 		                         //
// ?? ：DIRECT_DRAW *lpDraw : DirectDraw???                         //
// ? ：??                                                               //
//---------------------------------------------------------------------------//
void ClearSurface(LPDIRECTDRAWSURFACE lpSurface)
{
	DDBLTFX ddbltfx;

	ZeroMemory(&ddbltfx, sizeof(DDBLTFX));
	ddbltfx.dwSize = sizeof(DDBLTFX);
	ddbltfx.dwFillColor = DEF_COLORKEY; // ?

	lpSurface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);

	return;
}

// ???????????????????
int BmpOffBits;
//---------------------------------------------------------------------------//
// ?? ：?????????????????????                     //
// ?? ：char * pFile  : ??????????                             //
// ??：LPBITMAPINFO : NULL .????Or????                     //
//                NULL??.LPBITMAPINFO????                              //
//---------------------------------------------------------------------------//
LPBITMAPINFO LoadDirectDrawBitmap(char *pFile)
{
	HFILE hFile;
	OFSTRUCT ofSt;
	BITMAPFILEHEADER BmpFileHeader;
	LPBITMAPINFO lpBmpInfo;

	//???????
	if ((hFile = OpenFile(pFile, &ofSt, OF_READ)) == HFILE_ERROR)
		return (LPBITMAPINFO)NULL; // File Open Error

	//?????????????????
	_hread(hFile, &BmpFileHeader, sizeof(BITMAPFILEHEADER));

	//??????????
	if ((lpBmpInfo = (LPBITMAPINFO)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, BmpFileHeader.bfSize)) == NULL){
		MessageBoxNew(hWnd, "Heap的配置记忆体失败！", "确定", MB_OK | MB_ICONSTOP);
		return (LPBITMAPINFO)NULL; //Memory Error
	}

	//????????
	_hread(hFile, (void *)lpBmpInfo, BmpFileHeader.bfSize);

	//????????
	_lclose(hFile);

	// ???????????????????
	BmpOffBits = BmpFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);

	return lpBmpInfo;
}

//---------------------------------------------------------------------------//
// ?? ：????????????                                         //
// ?? ：short bxsize           : ?赶怐???巍纰)                       //
//        short bysize           : ?赶怐?c?(巍纰)                       //
//        DWORD ColorKey         : ????(0?255)                //
// ??：? ... ?????????? /  ... NULL                 //
//---------------------------------------------------------------------------//
LPDIRECTDRAWSURFACE CreateSurface(short bxsize, short bysize, DWORD ColorKey, unsigned int VramOrSysram)
{
	DDCOLORKEY ddck;
	LPDIRECTDRAWSURFACE lpSurface;
#ifdef _NEW_ALPHA_
	VramOrSysram = DDSCAPS_SYSTEMMEMORY;
#endif
	lpDraw->ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	lpDraw->ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | VramOrSysram;
	lpDraw->ddsd.dwWidth = bxsize;
	lpDraw->ddsd.dwHeight = bysize;
	if (lpDraw->lpDD2->CreateSurface(&lpDraw->ddsd, &lpSurface, NULL) != DD_OK)
		return (LPDIRECTDRAWSURFACE)NULL;

	ddck.dwColorSpaceLowValue = ColorKey;
	ddck.dwColorSpaceHighValue = ColorKey;
	lpSurface->SetColorKey(DDCKEY_SRCBLT, &ddck);

	return lpSurface;
}


//---------------------------------------------------------------------------//
// ?? ：??????????????                                 //
// ?? ：LPDIRECTDRAWSURFACE lpSurface : ??????                     //
//        short Xpoint        : ???????                         //
//        short Ypoint        :	??????                         //
//        LPBITMAPINFO pInfo  : ??????????                   //
// ? ：??                                                               //
//---------------------------------------------------------------------------//
// ????????????? ? StretchDIBits ?? ?
void DrawBitmapToSurface(LPDIRECTDRAWSURFACE lpSurface, int offsetX, int offsetY, LPBITMAPINFO pBmpInfo)
{
	HDC hDcDest;
	// ??????????????????
	lpSurface->GetDC(&hDcDest);
	StretchDIBits(hDcDest,
		0, 0,
		//cary
		pBmpInfo->bmiHeader.biWidth, pBmpInfo->bmiHeader.biHeight,
		//SurfaceSizeX, SurfaceSizeY,
		offsetX, offsetY,
		pBmpInfo->bmiHeader.biWidth, pBmpInfo->bmiHeader.biHeight,
		//SurfaceSizeX, SurfaceSizeY,
		//end
		(void *)((BYTE *)pBmpInfo + (sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)* 256)),
		pBmpInfo,

		//DIB_PAL_COLORS,	
		DIB_RGB_COLORS,
		//DIB_PAL_INDICES,

		SRCCOPY);
	//NOTSRCCOPY );
	//DSTINVERT );
	//BLACKNESS );
	lpSurface->ReleaseDC(hDcDest);

	return;
}

// ????????????? ? memcpy ?? ?****************************/
#ifdef _READ16BITBMP
void DrawBitmapToSurface2(SURFACE_INFO *surface_info,LPDIRECTDRAWSURFACE lpSurfaceSys,int offsetX,int offsetY,int sizeX,int sizeY,LPBITMAPINFO pBmpInfo)
{


	LPDIRECTDRAWSURFACE lpSurface = surface_info->lpSurface;
	DDSURFACEDESC ddsd;	// ??????
	char *pDest;			//目的地指标// ??????
	char *pSource; 		//来源指标// ???????
	short *pDest2;		//目的地指标(WORD type)// ????????????
	int surfacePitch;	//source face 宽度// ??????????
	int bmpWidth;			//bmp图的宽度// ????????
	int i;
#ifdef _READ16BITBMP
	DDSURFACEDESC ddsdsys;
	short *pDestSys;			//目的地指标 systemmemory
	int surfacePitchSys;	//source face
#endif
#ifdef _HI_COLOR_32
	int *pDest32;
	int *pDestSys32;
	int surfacePitchSys32;
#endif

	// ??????????NULL????
	if( lpSurface == NULL ) return;
#ifdef _READ16BITBMP
	if(lpSurfaceSys == NULL ) return;
#endif
	// ?????
	ZeroMemory( &ddsd, sizeof( DDSURFACEDESC ) );
	ddsd.dwSize = sizeof( DDSURFACEDESC );
#ifdef _READ16BITBMP
	ZeroMemory(&ddsdsys,sizeof(DDSURFACEDESC));
	ddsdsys.dwSize = sizeof(DDSURFACEDESC);
#endif
	if( lpSurface->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) != DD_OK ){
		return; 
	}	
#ifdef _READ16BITBMP
	if(lpSurfaceSys->Lock(NULL,&ddsdsys,DDLOCK_WAIT,NULL) != DD_OK){
		lpSurface->Unlock(NULL);
		return;
	}
#endif
	pDest = ( char *)( ddsd.lpSurface );
#ifdef _READ16BITBMP
	pDestSys = (short*)(ddsdsys.lpSurface);
#endif
#ifdef _HI_COLOR_32
	pDestSys32 = (int*)(ddsdsys.lpSurface);
#endif

#if 0
	pSource = ( char *)pBmpInfo + BmpOffBits
		+ offsetY * pBmpInfo->bmiHeader.biWidth + offsetX;

#else
	pSource = pRealBinBits
		+ offsetY * RealBinWidth + offsetX;
#endif
	surfacePitch = ddsd.lPitch;
#ifdef _READ16BITBMP
	surfacePitchSys = ddsdsys.lPitch >> 1;
#ifdef _HI_COLOR_32
	surfacePitchSys32 = ddsdsys.lPitch >> 2;
#endif

#endif
#if 0
	bmpWidth = pBmpInfo->bmiHeader.biWidth;

#else
	bmpWidth = RealBinWidth;
#endif
	if(ResoMode == 1){
		pSource -= bmpWidth;
		sizeY >>= 1;
	}
	for( i = 0 ; i < sizeY ; i++ ){
		if(ResoMode == 1){
			_asm{
				mov		edi,[pDest]		//?????
					mov		esi,[pSource]	//??????
					mov		eax,[sizeX]		//????????
					//			mov		ah,al			//
					shr		ax,1
					mov		cx,ax
					inc		esi				//??????????????

				loop_100:
				mov		al,[esi]		//???????
					//			cmp		al,240			//?????
					//			jne		loop_200		//?????????

					//			xor		al,al			//????

					//loop_200:
					mov		[edi],al		//????????
					inc		esi				//???????
					inc		esi				//???????
					inc		edi				//??????
					//			dec		ah				//??
					dec		cx				//??
					jne		loop_100		//??????????
			}
			// ???????????????? ?????????? ?
			pSource -= bmpWidth*2;
		} else {
#ifdef _HI_COLOR_32
			if( displayBpp == 32 )
			{
				int j;
				int pixel;
#ifdef _HI_COLOR_32
				pDest32 = (int *)pDest;
#endif
				for( j = 0; j < sizeX; j++ ){
#ifdef _NEW_COLOR_
					if(NewColor16Flg==1){
						short *pDest3 = (short *)((pSource-pRealBinBits)*2+pRealBinBits);
						if(pDest3[j]==DEF_COLORKEY){
							pDest32[j] = 0;
#ifdef _READ16BITBMP
							pDestSys32[j] =0;
#endif
						}else{
							pDest32[j] = (((pDest3[j]&0x001F)<<3)|((pDest3[j]&0x07E0 )<<5)|((pDest3[j]&0xF800 )<<8 )|0xFF000000 );
#ifdef _READ16BITBMP
							pDestSys32[j] = (((pDest3[j]&0x001F)<<3)|((pDest3[j]&0x07E0 )<<5)|((pDest3[j]&0xF800 )<<8 )|0xFF000000 );
#endif
						}
					}else if(NewColor16Flg==2){

						int *pDest3 = (int *)((pSource-pRealBinBits)*4+pRealBinBits);
						if ((pDest3[j] & 0xff000000) == DEF_COLORKEY || ((pDest3[j] & 0xff) < 3 && ((pDest3[j] >> 16) & 0xff) < 3 && ((pDest3[j]>> 8) & 0xff) < 3 )){
							pDest32[j] = 0;

#ifdef _READ16BITBMP
							pDestSys32[j] =0;
#endif
						}else{
							pDest32[j] = pDest3[j];
#ifdef _READ16BITBMP
							pDestSys32[j] = pDest3[j];
#endif
						}
					}else{
#endif
						if( pSource[j] == DEF_COLORKEY ){
							pDest32[j] = 0;
#ifdef _READ16BITBMP
							pDestSys32[j] = 0;
#endif
						}else{
							pixel = highColor32Palette[(unsigned char)pSource[j]];
							if( pixel == 0 ){
								pDest32[j] = 1;
#ifdef _READ16BITBMP
								pDestSys32[j] = 1;
#endif
							}else{
								pDest32[j] = pixel;
#ifdef _READ16BITBMP
								pDestSys32[j] = pixel;
#endif
							}
						}
#ifdef _NEW_COLOR_
					}
#endif
				}
			}else
#endif
			if( displayBpp == 16 )
			{
				int j;
				short pixel;

				pDest2 = (short *)pDest;

				for( j = 0; j < sizeX; j++ )
				{
#ifdef _NEW_COLOR_
					if(NewColor16Flg==1){
						short *pDest3 = (short *)((pSource-pRealBinBits)*2+pRealBinBits);
						if(pDest3[j]==DEF_COLORKEY){
							pDest2[j] = 0;
#ifdef _READ16BITBMP
							pDestSys[j] = 0;
#endif
						}else{
							pDest2[j] = pDest3[j];
#ifdef _READ16BITBMP
							pDestSys[j] = pDest3[j];
#endif
						}
					}else if(NewColor16Flg==2){
						int *pDest3 = (int *)((pSource-pRealBinBits)*4+pRealBinBits);
						if((pDest3[j] & 0xff000000) == DEF_COLORKEY || ((pDest3[j] & 0xff) < 3 && ((pDest3[j] >> 16) & 0xff) < 3 && ((pDest3[j]>> 8) & 0xff) < 3 )){
							pDest2[j] = 0;
#ifdef _READ16BITBMP
							pDestSys[j] = 0;
#endif
						}else{
							pDest2[j] = ((pDest3[j] >> 8) & 0xF8)  |  ((pDest3[j]>>5) & 0x7E) | ((pDest3[j]>>3) & 0x1F);
#ifdef _READ16BITBMP
							pDestSys[j] = ((pDest3[j] >> 8) & 0xF8)  |  ((pDest3[j]>>5) & 0x7E) | ((pDest3[j]>>3) & 0x1F);
#endif
						}
					}else{
#endif
						if( pSource[j] == DEF_COLORKEY )
						{
							pDest2[j] = 0;
#ifdef _READ16BITBMP
							pDestSys[j] = 0;
#endif
						}
						else
						{
							pixel = highColorPalette[(unsigned char)pSource[j]];
							if( pixel == 0 )
							{
								pDest2[j] = 1;
#ifdef _READ16BITBMP
								pDestSys[j] = 1;
#endif
							}
							else
							{
								pDest2[j] = pixel;
#ifdef _READ16BITBMP
								pDestSys[j] = pixel;
#endif
							}
						}
#ifdef _NEW_COLOR_
					}
#endif
				}
			}else{
				memcpy( pDest, pSource, sizeX );
			}
			//HiO????????

			// ???????????????? ?????????? ?
			pSource -= bmpWidth;
		}
		// ???????????????
		pDest += surfacePitch;
#ifdef _READ16BITBMP
		pDestSys += surfacePitchSys;
#endif
#ifdef _HI_COLOR_32
		pDestSys32 += surfacePitchSys32;
#endif
	}

	// ???????????????????
	if( lpSurface->Unlock( NULL ) != DD_OK ){
		//MessageBoxNew( hWnd, "Surface的Unlock失败！", "确定", MB_OK | MB_ICONSTOP );
		return; 
	}	
#ifdef _READ16BITBMP
	lpSurfaceSys->Unlock(NULL);
#endif
	return;
}
#endif

#ifdef _CACHE_SURFACE_
void DrawSurfaceFromPalette(SURFACE_INFO* surface_info)
{
	int sizeX, sizeY;
	DDSURFACEDESC ddsd;
	BYTE *pSource; 			//来源指标//
	LPDIRECTDRAWSURFACE lpSurface = surface_info->lpSurface;
	if (lpSurface == NULL) return;
	//如果不为256色补丁
	if (surface_info->colordepth > 0)	return;
	//如果调试板未改变并且不是切换调色板过程，那么就返回，使用缓存中的数据
	if (surface_info->palNo == PalState.palNo && PalState.time == 1) return;
	sizeX = surface_info->sizeX;
	sizeY = surface_info->sizeY;
	surface_info->palNo = PalState.palNo;
	pSource = (BYTE *)surface_info->lpCacheData;

	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC));
	ddsd.dwSize = sizeof(DDSURFACEDESC);
	if (lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK){
		return;
	}

	if (displayBpp == 32)
	{
		DWORD *pDest = (DWORD *)(ddsd.lpSurface);
		for (int i = 0; i < sizeY; i++)
		{
			for (int j = 0; j < sizeX; j++)
			{
				BYTE v = *(pSource + i * SurfaceSizeX + j);
				*(pDest + i * SurfaceSizeX + j) = (v == DEF_COLORKEY ? 0 : ((highColor32Palette[v] == 0 ? 1 : highColor32Palette[v])));
			}
		}
	}
	else
	{
		WORD *pDest = (WORD *)(ddsd.lpSurface);
		for (int i = 0; i < sizeY; i++)
		{
			for (int j = 0; j < sizeX; j++)
			{
				BYTE v = *(pSource + i * SurfaceSizeX + j);
				*(pDest + i * SurfaceSizeX + j) = (v == DEF_COLORKEY ? 0 : (highColorPalette[v] == 0 ? 1 : highColorPalette[v]));
			}
		}
	}
	if (lpSurface->Unlock(NULL) != DD_OK){
		return;
	}

}
#endif

void DrawBitmapToSurface2(SURFACE_INFO *surface_info, int offsetX, int offsetY, int sizeX, int sizeY, LPBITMAPINFO pBmpInfo)
{
#ifdef _CACHE_SURFACE_
	LPDIRECTDRAWSURFACE lpSurface = surface_info->lpSurface;
	DDSURFACEDESC ddsd;
	char *pDest;			//目的地指标//
	char *pCache;			//缓存
	char *pSource; 			//来源指标//
	BYTE *alphatemp;
	int surfacePitch;		//source face 宽度//
	int bmpWidth;			//bmp图的宽度//
	int i;

	if (lpSurface == NULL) return;

	surface_info->sizeX = sizeX;
	surface_info->sizeY = sizeY;
	surface_info->palNo = -1;
	surface_info->colordepth = NewColor16Flg;

	if (surface_info->lpCacheData == NULL)
	{
		surface_info->lpCacheData = (char *)MALLOC(SurfaceSizeX*SurfaceSizeY);
		if (surface_info->lpCacheData == NULL) return;
	}
	memset(surface_info->lpCacheData, 0, SurfaceSizeX*SurfaceSizeY);
	pCache = surface_info->lpCacheData;

	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC));
	ddsd.dwSize = sizeof(DDSURFACEDESC);
	if (lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK){
		return;
	}
	pDest = (char *)(ddsd.lpSurface);



#if 0
	pSource = ( char *)pBmpInfo + BmpOffBits
		+ offsetY * pBmpInfo->bmiHeader.biWidth + offsetX;
#else
	pSource = pRealBinBits
		+ offsetY * RealBinWidth + offsetX;
	if (NewColor16Flg == 2 && displayBpp == 16){
		surface_info->lpAlphaData = (BYTE*)MALLOC(SurfaceSizeX*SurfaceSizeY);
		memset(surface_info->lpAlphaData, 0, SurfaceSizeX*SurfaceSizeY);
		alphatemp = surface_info->lpAlphaData;
	}
#endif
	surfacePitch = ddsd.lPitch;
#if 0
	bmpWidth = pBmpInfo->bmiHeader.biWidth;
#else
	bmpWidth = RealBinWidth;
#endif
	if (ResoMode == 1){
		pSource -= bmpWidth;
		sizeY >>= 1;
	}
	for (i = 0; i < sizeY; i++){
		if (ResoMode == 1){
			_asm{
				mov		edi, [pDest]
					mov		esi, [pSource]
					mov		eax, [sizeX]
					shr		ax, 1
					mov		cx, ax
					inc		esi
				loop_100 :
				mov		al, [esi]
					mov[edi], al
					inc		esi
					inc		esi
					inc		edi
					dec 	cx
					jne		loop_100
			}
			pSource -= bmpWidth * 2;
		}
		else {
#ifdef _HI_COLOR_32
			if (displayBpp == 32){
				if (NewColor16Flg)
				{
					int j;
					int pixel;
					int * pDest2;
					pDest2 = (int *)pDest;
					for (j = 0; j < sizeX; j++){
#ifdef _NEW_COLOR_
						if (NewColor16Flg == 1){		//rgb565
							short *pDest3 = (short *)((pSource - pRealBinBits) * 2 + pRealBinBits);
							if (pDest3[j] == DEF_COLORKEY)
								pDest2[j] = 0;
							else{
								pDest2[j] = (((pDest3[j] & 0x001F) << 3) | ((pDest3[j] & 0x07E0) << 5) | ((pDest3[j] & 0xF800) << 8) | 0xFF000000);
							}
						}
						else if (NewColor16Flg == 2){	//rgba8888
							int *pDest3 = (int *)((pSource - pRealBinBits) * 4 + pRealBinBits);
							if ((pDest3[j] & 0xFF000000) >> 24 == DEF_COLORKEY)
								pDest2[j] = 0;
							else{
								pDest2[j] = pDest3[j];
							}
						}
#endif
					}
				}
				else //256色 缓存数据
				{
					memcpy(pCache, pSource, sizeX);
				}
			}
			else
#endif
			if (displayBpp == 16){
				if (NewColor16Flg)
				{

					int j;
					short pixel;
					short *pDest2 = (short *)pDest;
					for (j = 0; j < sizeX; j++){
#ifdef _NEW_COLOR_
						if (NewColor16Flg == 1){		//rgb565
							short *pDest3 = (short *)((pSource - pRealBinBits) * 2 + pRealBinBits);
							if (pDest3[j] == DEF_COLORKEY)
								pDest2[j] = 0;
							else pDest2[j] = pDest3[j];
						}
						else if (NewColor16Flg == 2){	//rgba8888
#define COLOR_TO_MTK_COLOR_SIMUL(color) ((((color) >> 19) & 0x1f) << 11) \
	| ((((color) >> 10) & 0x3f) << 5)\
	| (((color) >> 3) & 0x1f)
							int *pDest3 = (int *)((pSource - pRealBinBits) * 4 + pRealBinBits);
							if ((pDest3[j] & 0xFF000000) >> 24 == DEF_COLORKEY){
								pDest2[j] = 0;
								alphatemp[i * 64 + j] = 0;
							}
							else{
								alphatemp[i * 64 + j] = (pDest3[j] & 0xFF000000) >> 24;
								pDest2[j] = COLOR_TO_MTK_COLOR_SIMUL(pDest3[j]);
							}
						}
#endif
					}
				}
				else  //256色
				{
					memcpy(pCache, pSource, sizeX);
				}
			}
			else{
				memcpy(pDest, pSource, sizeX);
			}
			pSource -= bmpWidth;

		}
		pDest += surfacePitch;
		pCache += SurfaceSizeX;
	}
	if (lpSurface->Unlock(NULL) != DD_OK){
		return;
	}
#else
	LPDIRECTDRAWSURFACE lpSurface = surface_info->lpSurface;
	DDSURFACEDESC ddsd;
	char *pDest;			//目的地指标//
	char *pSource; 		//来源指标//
	BYTE *alphatemp;
	int surfacePitch;	//source face 宽度//
	int bmpWidth;			//bmp图的宽度//
	int i;

	if( lpSurface == NULL ) return;
	ZeroMemory( &ddsd, sizeof( DDSURFACEDESC ) );
	ddsd.dwSize = sizeof( DDSURFACEDESC );
	if( lpSurface->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) != DD_OK ){
		return; 
	}	
	pDest = ( char *)( ddsd.lpSurface );
#if 0
	pSource = ( char *)pBmpInfo + BmpOffBits
		+ offsetY * pBmpInfo->bmiHeader.biWidth + offsetX;
#else
	pSource = pRealBinBits
		+ offsetY * RealBinWidth + offsetX;
	if(NewColor16Flg==2 && displayBpp == 16){
		surface_info->lpAlphaData = (BYTE*)MALLOC(SurfaceSizeX*SurfaceSizeY);
		memset(surface_info->lpAlphaData,0,SurfaceSizeX*SurfaceSizeY);
		alphatemp = surface_info->lpAlphaData;
	}
#endif
	surfacePitch = ddsd.lPitch;
#if 0
	bmpWidth = pBmpInfo->bmiHeader.biWidth;
#else
	bmpWidth = RealBinWidth;
#endif
	if(ResoMode == 1){
		pSource -= bmpWidth;
		sizeY >>= 1;
	}
	for( i = 0 ; i < sizeY ; i++ ){
		if(ResoMode == 1){
			_asm{
				mov		edi,[pDest]
					mov		esi,[pSource]
					mov		eax,[sizeX]
					shr		ax,1
					mov		cx,ax
					inc		esi
				loop_100:
				mov		al,[esi]
					mov		[edi],al
					inc		esi
					inc		esi
					inc		edi
					dec 	cx
					jne		loop_100
			}
			pSource -= bmpWidth*2;
		} else {
#ifdef _HI_COLOR_32
			if( displayBpp == 32 ){
				int j;
				int pixel;
				int * pDest2;
				pDest2 = (int *)pDest;
				for( j = 0; j < sizeX; j++ ){
#ifdef _NEW_COLOR_
					if(NewColor16Flg==1){
						short *pDest3 = (short *)((pSource-pRealBinBits)*2+pRealBinBits);
						if(pDest3[j]==DEF_COLORKEY)
							pDest2[j] = 0;
						else{
							pDest2[j] = (((pDest3[j]&0x001F)<<3)|((pDest3[j]&0x07E0 )<<5)|((pDest3[j]&0xF800 )<<8 )|0xFF000000 );
						}
					}else if(NewColor16Flg==2){
						int *pDest3 = (int *)((pSource-pRealBinBits)*4+pRealBinBits);
						if((pDest3[j] & 0xFF000000)>>24==DEF_COLORKEY)
							pDest2[j] = 0;
						else{
							pDest2[j] = pDest3[j];
						}
					}else{
#endif
						if( pSource[j] == DEF_COLORKEY ){
							pDest2[j] = 0;
						}else{
							pixel = highColor32Palette[(unsigned char)pSource[j]];
							if( pixel == 0 ){
								pDest2[j] = 1;
							}else{
								pDest2[j] = pixel;
							}
						}
#ifdef _NEW_COLOR_
					}
#endif
				}
			}else
#endif
			if( displayBpp == 16 ){
				int j;
				short pixel;
				short *pDest2 = (short *)pDest;
				for( j = 0; j < sizeX; j++ ){
#ifdef _NEW_COLOR_
					if(NewColor16Flg==1){
						short *pDest3 = (short *)((pSource-pRealBinBits)*2+pRealBinBits);
						if(pDest3[j]==DEF_COLORKEY)
							pDest2[j] = 0;
						else pDest2[j] = pDest3[j];
					}else if(NewColor16Flg==2){
#define COLOR_TO_MTK_COLOR_SIMUL(color) ((((color) >> 19) & 0x1f) << 11) \
	|((((color) >> 10) & 0x3f) << 5)\
	|(((color) >> 3) & 0x1f)
						int *pDest3 = (int *)((pSource-pRealBinBits)*4+pRealBinBits);
						if((pDest3[j] & 0xFF000000)>>24==DEF_COLORKEY){
							pDest2[j] = 0;
							alphatemp[i*64+j]=0;
						}else{
							alphatemp[i*64+j]=(pDest3[j] &0xFF000000)>>24;
							pDest2[j] = COLOR_TO_MTK_COLOR_SIMUL(pDest3[j]);
						}
					}else{
#endif
						if( pSource[j] == DEF_COLORKEY ){
							pDest2[j] = 0;
						}else{
							pixel = highColorPalette[(unsigned char)pSource[j]];
							if( pixel == 0 ){
								pDest2[j] = 1;
							}else{
								pDest2[j] = pixel;
							}
						}
#ifdef _NEW_COLOR_
					}
#endif
				}





			}else{
				memcpy( pDest, pSource, sizeX );
			}
			pSource -= bmpWidth;

		}
		pDest += surfacePitch;
	}
	if( lpSurface->Unlock( NULL ) != DD_OK ){
		return; 
	}
	return;
#endif
}

// ?????? **************************************************************/
void DrawBox(RECT *rect, unsigned char color, BOOL fill)
{
	DDSURFACEDESC ddsd;	// ??????
#ifdef _READ16BITBMP
	DDSURFACEDESC ddsdsys;
	int surfacePitchsys,bottomSys;
	surfacePitchsys = bottomSys = 0;
	short *pDestSys;
#endif
#ifdef _HI_COLOR_32
	int *pDest32;
#ifdef _READ16BITBMP
	int *pDestSys32;
#endif
#endif
	char *pDest;		// ??????
	short *pDest2;		// ????????????
	int surfacePitch;	// ??????????
	int i, j;			// ???????
	int bottom;			// ?????????
	int w;				// ??
	int h;				// ?
	short pixel;		// 16BitColor????

	//???????????????
	if (ResoMode == 1){
		rect->top = (rect->top >> 1) - 1;
		rect->left = (rect->left >> 1) - 1;
		rect->right = (rect->right >> 1) + 1;
		rect->bottom = (rect->bottom >> 1) + 1;
	}

	// ????????
	if (rect->left < 0) rect->left = 0;
	if (rect->right >= lpDraw->xSize) rect->right = lpDraw->xSize;
	if (rect->top < 0) rect->top = 0;
	if (rect->bottom >= lpDraw->ySize) rect->bottom = lpDraw->ySize;

	w = rect->right - rect->left;
	h = rect->bottom - rect->top;

	if (fill != 2 && (w <= 2 || h <= 2)) return;
	if (fill == 2 && w == 0) return;

	// ?????
	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC));
	ddsd.dwSize = sizeof(DDSURFACEDESC);
#ifdef _READ16BITBMP
	if(g_bUseAlpha){
		ZeroMemory(&ddsdsys,sizeof(DDSURFACEDESC));
		ddsdsys.dwSize = sizeof(DDSURFACEDESC);
	}
#endif

	// ?????????????????( ?? ddsd ??????? )
	if (lpDraw->lpBACKBUFFER->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK){
		//MessageBoxNew( hWnd, "Surface的lock失败！", "确定", MB_OK | MB_ICONSTOP );
		return;
	}
#ifdef _READ16BITBMP
	if(g_bUseAlpha){
		if(lpDraw->lpBACKBUFFERSYS->Lock(NULL,&ddsdsys,DDLOCK_WAIT,NULL) != DD_OK ){
			lpDraw->lpBACKBUFFER->Unlock(NULL);
			return; 
		}
		surfacePitchsys = ddsdsys.lPitch;
	}
#endif

	// ??????????
	surfacePitch = ddsd.lPitch;
	// ?????????????
#ifdef _HI_COLOR_32
	if (displayBpp == 32){
		// 32 Bit Color
		surfacePitch = surfacePitch >> 1;
		pDest32 = (int *)ddsd.lpSurface + rect->top * surfacePitch + rect->left + 1;
#ifdef _READ16BITBMP
		if(g_bUseAlpha){
			surfacePitchsys = surfacePitchsys >> 2;
			pDestSys32 = (int *)ddsdsys.lpSurface + rect->top * surfacePitchsys + rect->left + 1;
		}
#endif
	}
	else
#endif
	if (displayBpp == 16){
		// 16 Bit Color
		surfacePitch = surfacePitch >> 1;
		pDest2 = (short *)ddsd.lpSurface + rect->top * surfacePitch + rect->left + 1;
#ifdef _READ16BITBMP
		if(g_bUseAlpha){
			surfacePitchsys = surfacePitchsys >> 1;
			pDestSys = (short*)ddsdsys.lpSurface + rect->top * surfacePitchsys + rect->left + 1;
		}
#endif
	}
	else{
		// 8 Bit Color
		pDest = (char *)(ddsd.lpSurface) + rect->top * surfacePitch + rect->left + 1;
	}
	// ???????
	if (fill == FALSE){
#ifdef _HI_COLOR_32
		if (displayBpp == 32){
			// 32 Bit Color
			int pixel;
			surfacePitch = surfacePitch >> 1;
			pDest32 = (int *)ddsd.lpSurface + rect->top * surfacePitch + rect->left + 1;
			// ?????????
			bottom = (h - 1) * surfacePitch;
#ifdef _READ16BITBMP
			bottomSys = ( h - 1 ) * surfacePitchsys;
#endif

			pixel = highColor32Palette[color];

			// ????
			for (i = 0; i < w - 2; i++){
				// ???
				*(pDest32 + i) = pixel;
				*(pDest32 + surfacePitch + i) = pixel;
				// ????
				*(pDest32 + bottom - surfacePitch + i) = pixel;
				*(pDest32 + bottom + i) = pixel;
#ifdef _READ16BITBMP
				if(g_bUseAlpha){
					*(pDestSys32 + i) = pixel;
					*(pDestSys32 + surfacePitchsys + i) = pixel;
					*(pDestSys32 + bottomSys - surfacePitchsys + i) = pixel;
					*(pDestSys32 + bottomSys + i) = pixel;
				}
#endif
			}

			// ???
			*(pDest32 + surfacePitch + surfacePitch + 1) = pixel;					// ?
			*(pDest32 + surfacePitch + surfacePitch + w - 4) = pixel;				// ?
			*(pDest32 + bottom - (surfacePitch + surfacePitch) + 1) = pixel;		// ??
			*(pDest32 + bottom - (surfacePitch + surfacePitch) + w - 4) = pixel;	// ??

			// ???????????????
			pDest32 += surfacePitch - 1;
#ifdef _READ16BITBMP
			if(g_bUseAlpha){
				*(pDestSys32 + surfacePitchsys + surfacePitchsys + 1) = pixel;
				*(pDestSys32 + surfacePitchsys + surfacePitchsys + w - 4) = pixel;
				*(pDestSys32 + bottomSys - ( surfacePitchsys + surfacePitchsys ) + 1) = pixel;
				*(pDestSys32 + bottomSys - ( surfacePitchsys + surfacePitchsys ) + w - 4)  = pixel;
				pDestSys32 += surfacePitchsys - 1;
			}
#endif

			// ???
			for (i = 0; i < h - 2; i++){
				// ????
				*pDest32 = pixel;
				*(pDest32 + 1) = pixel;
				// ????
				*(pDest32 + w - 1) = pixel;
				*(pDest32 + w - 2) = pixel;
				// ???????????????
				pDest32 += surfacePitch;
#ifdef _READ16BITBMP
				if(g_bUseAlpha){
					*pDestSys32 = pixel;
					*(pDestSys32 + 1) = pixel;
					*(pDestSys32 + w - 1) = pixel;
					*(pDestSys32 + w - 2) = pixel;
					pDestSys32 += surfacePitchsys;
				}
#endif
			}
		}
		else
#endif
		if (displayBpp == 16){
			// 16 Bit Color

			// ?????????
			bottom = (h - 1) * surfacePitch;
#ifdef _READ16BITBMP
			bottomSys = ( h - 1 ) * surfacePitchsys;
#endif

			pixel = highColorPalette[color];

			// ????
			for (i = 0; i < w - 2; i++){
				// ???
				*(pDest2 + i) = pixel;
				*(pDest2 + surfacePitch + i) = pixel;
				// ????
				*(pDest2 + bottom - surfacePitch + i) = pixel;
				*(pDest2 + bottom + i) = pixel;
#ifdef _READ16BITBMP
				if(g_bUseAlpha){
					*(pDestSys + i) = pixel;
					*(pDestSys + surfacePitchsys + i) = pixel;
					*(pDestSys + bottomSys - surfacePitchsys + i) = pixel;
					*(pDestSys + bottomSys + i) = pixel;
				}
#endif
			}

			// ???
			*(pDest2 + surfacePitch + surfacePitch + 1) = pixel;					// ?
			*(pDest2 + surfacePitch + surfacePitch + w - 4) = pixel;				// ?
			*(pDest2 + bottom - (surfacePitch + surfacePitch) + 1) = pixel;		// ??
			*(pDest2 + bottom - (surfacePitch + surfacePitch) + w - 4) = pixel;	// ??

			// ???????????????
			pDest2 += surfacePitch - 1;
#ifdef _READ16BITBMP
			if(g_bUseAlpha){
				*(pDestSys + surfacePitchsys + surfacePitchsys + 1) = pixel;
				*(pDestSys + surfacePitchsys + surfacePitchsys + w - 4) = pixel;
				*(pDestSys + bottomSys - ( surfacePitchsys + surfacePitchsys ) + 1) = pixel;
				*(pDestSys + bottomSys - ( surfacePitchsys + surfacePitchsys ) + w - 4)  = pixel;
				pDestSys += surfacePitchsys - 1;
			}
#endif

			// ???
			for (i = 0; i < h - 2; i++){
				// ????
				*pDest2 = pixel;
				*(pDest2 + 1) = pixel;
				// ????
				*(pDest2 + w - 1) = pixel;
				*(pDest2 + w - 2) = pixel;
				// ???????????????
				pDest2 += surfacePitch;
#ifdef _READ16BITBMP
				if(g_bUseAlpha){
					*pDestSys = pixel;
					*(pDestSys + 1) = pixel;
					*(pDestSys + w - 1) = pixel;
					*(pDestSys + w - 2) = pixel;
					pDestSys += surfacePitchsys;
				}
#endif
			}
		}
		else{
			// 8 Bit Color

			// ????????
			bottom = (h - 1) * surfacePitch;

			// ????
			for (i = 0; i < w - 2; i++){
				// ???
				*(pDest + i) = color;
				*(pDest + i + surfacePitch) = color;
				// ????
				*(pDest + i + bottom) = color;
				*(pDest + i + bottom - surfacePitch) = color;
			}

			// ???
			*(pDest + surfacePitch + surfacePitch + 1) = color;					// ?
			*(pDest + surfacePitch + surfacePitch + w - 4) = color;				// ?
			*(pDest + bottom - (surfacePitch + surfacePitch) + 1) = color;		// ??
			*(pDest + bottom - (surfacePitch + surfacePitch) + w - 4) = color;	// ??

			// ???????????????
			pDest += surfacePitch - 1;

			// ???
			for (i = 0; i < h - 2; i++){
				// ????
				*pDest = color;
				*(pDest + 1) = color;
				// ????
				*(pDest + w - 1) = color;
				*(pDest + w - 2) = color;
				// ???????????????
				pDest += surfacePitch;
			}
		}
	}
	else if (fill == 1){	// ??????
#ifdef _HI_COLOR_32
		if (displayBpp == 32){
			int pixel;
			surfacePitch = surfacePitch >> 1;
			// 32 Bit Color
			pDest32 = (int *)ddsd.lpSurface + rect->top * surfacePitch + rect->left + 1;
			pixel = highColor32Palette[color];

			// ???????
			for (i = 0; i < w - 2; i++){
				*(pDest32 + i) = pixel;
#ifdef _READ16BITBMP
				if(g_bUseAlpha){
					*(pDestSys32 + i) = pixel;
				}
#endif
			}
			pDest32 += surfacePitch;
#ifdef _READ16BITBMP
			if(g_bUseAlpha){
				pDestSys32 += surfacePitchsys;
			}
#endif
			for (i = 0; i < h - 2; i++){
				for (j = 0; j < w; j++){
					*(pDest32 + j) = pixel;
#ifdef _READ16BITBMP
					if(g_bUseAlpha) *(pDestSys32+j) = pixel;
#endif
				}
				pDest32 += surfacePitch;
#ifdef _READ16BITBMP
				if(g_bUseAlpha) pDestSys32 += surfacePitchsys;
#endif
			}
			pDest32++;
#ifdef _READ16BITBMP
			if(g_bUseAlpha) pDestSys32++;
#endif
			for (i = 0; i < w - 2; i++){
				*(pDest32 + i) = pixel;
#ifdef _READ16BITBMP
				if(g_bUseAlpha) *(pDestSys32 + i) = pixel;
#endif
			}
		}
		else
#endif
		if (displayBpp == 16){
			pixel = highColorPalette[color];
			for (i = 0; i < w - 2; i++){
				*(pDest2 + i) = pixel;
#ifdef _READ16BITBMP
				if(g_bUseAlpha){
					*(pDestSys + i) = pixel;
				}
#endif
			}
			pDest2 += surfacePitch - 1;
#ifdef _READ16BITBMP
			if(g_bUseAlpha){
				pDestSys += surfacePitchsys - 1;
			}
#endif
			for (i = 0; i < h - 2; i++){
				for (j = 0; j < w; j++){
					*(pDest2 + j) = pixel;
#ifdef _READ16BITBMP
					if(g_bUseAlpha) *(pDestSys+j) = pixel;
#endif
				}
				pDest2 += surfacePitch;
#ifdef _READ16BITBMP
				if(g_bUseAlpha) pDestSys += surfacePitchsys;
#endif
			}
			pDest2++;
#ifdef _READ16BITBMP
			if(g_bUseAlpha) pDestSys++;
#endif
			for (i = 0; i < w - 2; i++){
				*(pDest2 + i) = pixel;
#ifdef _READ16BITBMP
				if(g_bUseAlpha) *(pDestSys + i) = pixel;
#endif
			}
		}
		else{
			pDest++;
			memset(pDest, color, w - 2);
			pDest += surfacePitch - 1;
			for (i = 0; i < h - 2; i++){
				memset(pDest, color, w);
				pDest += surfacePitch;
			}
			memset(pDest + 1, color, w - 2);
		}
	}
	else if (fill == 2){
#ifdef _HI_COLOR_32
		if (displayBpp == 32){
			int pixel;
			surfacePitch = surfacePitch >> 1;
			pixel = highColor32Palette[color];
			pDest32 = (int *)ddsd.lpSurface + rect->top * surfacePitch + rect->left + 1;
			for (i = 0; i < w; i++){
				*(pDest32 + i) = pixel;
#ifdef _READ16BITBMP
				if(g_bUseAlpha) *(pDestSys32 + i) = pixel;
#endif
			}
		}
		else
#endif
		if (displayBpp == 16){
			pixel = highColorPalette[color];
			for (i = 0; i < w; i++){
				*(pDest2 + i) = pixel;
#ifdef _READ16BITBMP
				if(g_bUseAlpha) *(pDestSys + i) = pixel;
#endif
			}
		}
		else{
			memset(pDest, color, w);
		}
	}
	if (lpDraw->lpBACKBUFFER->Unlock(NULL) != DD_OK){
		return;
	}
#ifdef _READ16BITBMP
	if(g_bUseAlpha) if(lpDraw->lpBACKBUFFERSYS->Unlock(NULL) != DD_OK ) return; 
#endif

	return;
}

void DrawAutoMapping(int x, int y, unsigned char *autoMap, int w, int h)
{
	DDSURFACEDESC ddsd;	// ??????
	char *ptDest;		// ??????
	char *tmpPtDest;	// ???
	char *tmpPtDest2;	// ????
	int surfacePitch;	// ??????????
	int i, j;			// ???????
	int color, pc_color;
	static short pcFlush = 0;
	static unsigned int pcFlushTime = 0;
	int xx, yy;
	int ww, hh;
	int mul;

	xx = 18;
	yy = 118;
	ww = w;
	hh = h;
	mul = 1;

	//???????????????
	if (ResoMode == 1)
	{
		x = x / 2;
		y = y / 2;
		xx = xx / 2 + 1;
		yy = yy / 2 - 1;
		ww = ww / 2;
		hh = hh / 2;
		mul = 2;
	}

	if (pcFlushTime + 1000 <= TimeGetTime())
	{
		pcFlushTime = TimeGetTime();
		pcFlush++;
		pcFlush &= 1;
	}
	if (pcFlush){
		pc_color = 255;
	}
	else{
		pc_color = 0;
	}
	// ?????
	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC));
	ddsd.dwSize = sizeof(DDSURFACEDESC);

	// ?????????????????( ?? ddsd ??????? )
	if (lpDraw->lpBACKBUFFER->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK)
	{
		//MessageBoxNew( hWnd, "Surface的lock失败！", "确定", MB_OK | MB_ICONSTOP );
		return;
	}
	// ??????????
	surfacePitch = ddsd.lPitch;
	// ?????????????
	ptDest = (char *)(ddsd.lpSurface) + (y + yy) * surfacePitch;
	if (displayBpp == 8){
		ptDest += (x + xx);
		tmpPtDest = ptDest;
		tmpPtDest2 = ptDest;
		for (i = 0; i < hh; i++){
			ptDest = tmpPtDest;
			for (j = 0; j < ww; j++){
				color = autoMap[(i*mul)*w + (j*mul)];
				*(ptDest - 1) = color;
				*(ptDest) = color;
				*(ptDest + 1) = color;
				*(ptDest - surfacePitch) = color;
				ptDest -= (surfacePitch - 2);
			}
			tmpPtDest += (surfacePitch + 2);
		}
		ptDest = tmpPtDest2 + (surfacePitch + 2)*hh / 2 - (surfacePitch - 2)*ww / 2;
		// ????????????????
		*(ptDest - 1) = pc_color;
		*(ptDest) = pc_color;
		*(ptDest + 1) = pc_color;
		*(ptDest - surfacePitch) = pc_color;
	}
	else
#ifdef _HI_COLOR_32
	if (displayBpp == 32){
		ptDest += ((x + xx) << 2);
		tmpPtDest = ptDest;
		tmpPtDest2 = ptDest;
		for (i = 0; i < hh; i++){
			ptDest = tmpPtDest;
			for (j = 0; j < ww; j++){
				color = highColor32Palette[autoMap[(i*mul)*w + (j*mul)]];
				*(int *)(ptDest - 4) = color;
				*(int *)(ptDest) = color;
				*(int *)(ptDest + 4) = color;
				*(int *)(ptDest - surfacePitch) = color;
				ptDest -= (surfacePitch - 8);
			}
			tmpPtDest += (surfacePitch + 8);
		}
		ptDest = tmpPtDest2 + (surfacePitch + 4)*hh - (surfacePitch - 4)*ww;
		color = highColor32Palette[pc_color];
		// ????????????????
		*(int *)(ptDest - 4) = color;
		*(int *)(ptDest) = color;
		*(int *)(ptDest + 4) = color;
		*(int *)(ptDest - surfacePitch) = color;
	}
	else
#endif
	if (displayBpp == 16){
		ptDest += ((x + xx) << 1);
		tmpPtDest = ptDest;
		tmpPtDest2 = ptDest;
		for (i = 0; i < hh; i++){
			ptDest = tmpPtDest;
			for (j = 0; j < ww; j++){
				color = highColorPalette[autoMap[(i*mul)*w + (j*mul)]];
				*(short*)(ptDest - 2) = color;
				*(short*)(ptDest) = color;
				*(short*)(ptDest + 2) = color;
				*(short*)(ptDest - surfacePitch) = color;
				ptDest -= (surfacePitch - 4);
			}
			tmpPtDest += (surfacePitch + 4);
		}
		ptDest = tmpPtDest2 + (surfacePitch + 4)*hh / 2 - (surfacePitch - 4)*ww / 2;
		color = highColorPalette[pc_color];
		// ????????????????
		*(short*)(ptDest - 2) = color;
		*(short*)(ptDest) = color;
		*(short*)(ptDest + 2) = color;
		*(short*)(ptDest - surfacePitch) = color;
	}

	// ???????????????????
	if (lpDraw->lpBACKBUFFER->Unlock(NULL) != DD_OK)
	{
		//MessageBoxNew( hWnd, "Surface的Unlock失败！", "确定", MB_OK | MB_ICONSTOP );
		return;
	}

	return;
}


// ?????????????????
int getAutoMapColor(unsigned int GraphicNo)
{
	int index = 0;
	static int width, height;
	static unsigned char *graBuf;
	unsigned int red = 0, green = 0, blue = 0;
	unsigned int cnt = 0;
	int color;

	int i, j;
	// real.bin????????????????
	if (realGetImage(GraphicNo, (unsigned char **)&graBuf, &width, &height) == FALSE) return 0;
	// ??????????
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			index = graBuf[i*width + j];
			if (index != DEF_COLORKEY){	// ???????
				red += Palette[index].peRed;
				green += Palette[index].peGreen;
				blue += Palette[index].peBlue;
				cnt++;
			}
		}
	}

	if (cnt == 0) return 0;
	// ??????????????????
	color = getNearestColorIndex(RGB(red / cnt, green / cnt, blue / cnt), Palette, 256);

	return color;
}

//---------------------------------------------------------------------------//
// ?entry?????palette?????color???????index???
//---------------------------------------------------------------------------//
//ref 寻找色盘中最接近的颜色
int getNearestColorIndex(COLORREF color, PALETTEENTRY *palette, int entry)
{
	double distance, mindist;
	int min_index;
	int i;
	mindist = (palette[0].peRed - GetRValue(color))*(palette[0].peRed - GetRValue(color))
		+ (palette[0].peGreen - GetGValue(color))*(palette[0].peGreen - GetGValue(color))
		+ (palette[0].peBlue - GetBValue(color))*(palette[0].peBlue - GetBValue(color));
	min_index = 0;
	for (i = 16; i < entry - 16; i++){
		distance = (palette[i].peRed - GetRValue(color))*(palette[i].peRed - GetRValue(color))
			+ (palette[i].peGreen - GetGValue(color))*(palette[i].peGreen - GetGValue(color))
			+ (palette[i].peBlue - GetBValue(color))*(palette[i].peBlue - GetBValue(color));
		if (distance < mindist){
			min_index = i;
			mindist = distance;
		}
	}
	return min_index;
}



//---------------------------------------------------------------------------//
// ??????????                                                    //
//---------------------------------------------------------------------------//
//ref 画出地图的特效 
void DrawMapEffect(void)
{
	DDSURFACEDESC ddsd;
	char *ptDest;
	short *ptDest2;
#ifdef _HI_COLOR_32
	int *ptDest32;
#endif
	int surfacePitch;
	int ww, hh;
	MAP_EFFECT *buf;
	char color;
	int i, j;

	ww = DEF_APPSIZEX;
	hh = DEF_APPSIZEY;
	//???????????????
	if (ResoMode == 1){
		ww >>= 1;
		hh >>= 1;
	}
	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC));
	ddsd.dwSize = sizeof(DDSURFACEDESC);
	if (lpDraw->lpBACKBUFFER->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK)
		return;
#ifdef _HI_COLOR_32
	if (displayBpp == 32)
		surfacePitch = ddsd.lPitch >> 2;
#endif
	if (displayBpp == 16)
		surfacePitch = ddsd.lPitch >> 1;
	buf = useBufMapEffect;
	while (buf != (MAP_EFFECT *)NULL){
		if (0 < buf->x && buf->x < ww - 4 && 0 < buf->y && buf->y < hh - 12){
#ifdef _HI_COLOR_32
			if (displayBpp == 32){
				ptDest32 = (int *)(ddsd.lpSurface) + buf->y * surfacePitch + buf->x;
			}
			else
#endif
			if (displayBpp == 16)
				ptDest2 = (short *)(ddsd.lpSurface) + buf->y * surfacePitch + buf->x;
			else
				ptDest = (char *)(ddsd.lpSurface) + buf->y * surfacePitch + buf->x;
			if (buf->type == MAP_EFFECT_TYPE_RAIN){
#ifdef _HI_COLOR_32
				if (displayBpp == 32){
					*(ptDest32) = highColor32Palette[143];
					*(ptDest32 + surfacePitch) = highColor32Palette[143];
					*(ptDest32 + surfacePitch * 2 + 1) = highColor32Palette[143];
					*(ptDest32 + surfacePitch * 3 + 1) = highColor32Palette[143];
				}
				else
#endif
				if (displayBpp == 16){
					*(ptDest2) = highColorPalette[143];
					*(ptDest2 + surfacePitch) = highColorPalette[143];
					*(ptDest2 + surfacePitch * 2 + 1) = highColorPalette[143];
					*(ptDest2 + surfacePitch * 3 + 1) = highColorPalette[143];
				}
				else{
					*(ptDest) = (char)143;
					*(ptDest + surfacePitch) = (char)143;
					*(ptDest + surfacePitch * 2 + 1) = (char)143;
					*(ptDest + surfacePitch * 3 + 1) = (char)143;
				}
			}
			else if (buf->type == MAP_EFFECT_TYPE_SNOW){
				if (buf->mode == 0){
#ifdef _HI_COLOR_32
					if (displayBpp == 32){
						*(ptDest32) = highColor32Palette[161];
						*(ptDest32 + 1) = highColor32Palette[159];
						*(ptDest32 + 2) = highColor32Palette[161];
						*(ptDest32 + surfacePitch) = highColor32Palette[159];
						*(ptDest32 + surfacePitch + 1) = highColor32Palette[159];
						*(ptDest32 + surfacePitch + 2) = highColor32Palette[159];
						*(ptDest32 + surfacePitch * 2) = highColor32Palette[161];
						*(ptDest32 + surfacePitch * 2 + 1) = highColor32Palette[159];
						*(ptDest32 + surfacePitch * 2 + 2) = highColor32Palette[161];
					}
					else
#endif
					if (displayBpp == 16){
						*(ptDest2) = highColorPalette[161];
						*(ptDest2 + 1) = highColorPalette[159];
						*(ptDest2 + 2) = highColorPalette[161];
						*(ptDest2 + surfacePitch) = highColorPalette[159];
						*(ptDest2 + surfacePitch + 1) = highColorPalette[159];
						*(ptDest2 + surfacePitch + 2) = highColorPalette[159];
						*(ptDest2 + surfacePitch * 2) = highColorPalette[161];
						*(ptDest2 + surfacePitch * 2 + 1) = highColorPalette[159];
						*(ptDest2 + surfacePitch * 2 + 2) = highColorPalette[161];
					}
					else{
						*(ptDest) = (char)161;
						*(ptDest + 1) = (char)159;
						*(ptDest + 2) = (char)161;
						*(ptDest + surfacePitch) = (char)159;
						*(ptDest + surfacePitch + 1) = (char)159;
						*(ptDest + surfacePitch + 2) = (char)159;
						*(ptDest + surfacePitch * 2) = (char)161;
						*(ptDest + surfacePitch * 2 + 1) = (char)159;
						*(ptDest + surfacePitch * 2 + 2) = (char)161;
					}
				}
				else if (buf->mode == 1){
#ifdef _HI_COLOR_32
					if (displayBpp == 32){
						*(ptDest32) = highColor32Palette[162];
						*(ptDest32 + 1) = highColor32Palette[159];
						*(ptDest32 + 2) = highColor32Palette[159];
						*(ptDest32 + 3) = highColor32Palette[162];
						*(ptDest32 + surfacePitch) = highColor32Palette[159];
						*(ptDest32 + surfacePitch + 1) = highColor32Palette[159];
						*(ptDest32 + surfacePitch + 2) = highColor32Palette[159];
						*(ptDest32 + surfacePitch + 3) = highColor32Palette[159];
						*(ptDest32 + surfacePitch * 2) = highColor32Palette[159];
						*(ptDest32 + surfacePitch * 2 + 1) = highColor32Palette[159];
						*(ptDest32 + surfacePitch * 2 + 2) = highColor32Palette[159];
						*(ptDest32 + surfacePitch * 2 + 3) = highColor32Palette[159];
						*(ptDest32 + surfacePitch * 3) = highColor32Palette[162];
						*(ptDest32 + surfacePitch * 3 + 1) = highColor32Palette[159];
						*(ptDest32 + surfacePitch * 3 + 2) = highColor32Palette[159];
						*(ptDest32 + surfacePitch * 3 + 3) = highColor32Palette[162];
					}
					else
#endif
					if (displayBpp == 16){
						*(ptDest2) = highColorPalette[162];
						*(ptDest2 + 1) = highColorPalette[159];
						*(ptDest2 + 2) = highColorPalette[159];
						*(ptDest2 + 3) = highColorPalette[162];
						*(ptDest2 + surfacePitch) = highColorPalette[159];
						*(ptDest2 + surfacePitch + 1) = highColorPalette[159];
						*(ptDest2 + surfacePitch + 2) = highColorPalette[159];
						*(ptDest2 + surfacePitch + 3) = highColorPalette[159];
						*(ptDest2 + surfacePitch * 2) = highColorPalette[159];
						*(ptDest2 + surfacePitch * 2 + 1) = highColorPalette[159];
						*(ptDest2 + surfacePitch * 2 + 2) = highColorPalette[159];
						*(ptDest2 + surfacePitch * 2 + 3) = highColorPalette[159];
						*(ptDest2 + surfacePitch * 3) = highColorPalette[162];
						*(ptDest2 + surfacePitch * 3 + 1) = highColorPalette[159];
						*(ptDest2 + surfacePitch * 3 + 2) = highColorPalette[159];
						*(ptDest2 + surfacePitch * 3 + 3) = highColorPalette[162];
					}
					else{
						*(ptDest) = (char)162;
						*(ptDest + 1) = (char)159;
						*(ptDest + 2) = (char)159;
						*(ptDest + 3) = (char)162;
						*(ptDest + surfacePitch) = (char)159;
						*(ptDest + surfacePitch + 1) = (char)159;
						*(ptDest + surfacePitch + 2) = (char)159;
						*(ptDest + surfacePitch + 3) = (char)159;
						*(ptDest + surfacePitch * 2) = (char)159;
						*(ptDest + surfacePitch * 2 + 1) = (char)159;
						*(ptDest + surfacePitch * 2 + 2) = (char)159;
						*(ptDest + surfacePitch * 2 + 3) = (char)159;
						*(ptDest + surfacePitch * 3) = (char)162;
						*(ptDest + surfacePitch * 3 + 1) = (char)159;
						*(ptDest + surfacePitch * 3 + 2) = (char)159;
						*(ptDest + surfacePitch * 3 + 3) = (char)162;
					}
				}
			}
			else if (buf->type == MAP_EFFECT_TYPE_STAR){
				if (buf->mode == 0){
#ifdef _HI_COLOR_32
					if (displayBpp == 32){
						// ?????
						*(ptDest32 - surfacePitch - 1) = highColor32Palette[8];
						*(ptDest32 - surfacePitch) = highColor32Palette[255];
						*(ptDest32 - surfacePitch + 1) = highColor32Palette[8];
						*(ptDest32 - 1) = highColor32Palette[255];
						*(ptDest32) = highColor32Palette[255];
						*(ptDest32 + 1) = highColorPalette[255];
						*(ptDest32 + surfacePitch - 2) = highColor32Palette[8];
						*(ptDest32 + surfacePitch) = highColor32Palette[255];
						*(ptDest32 + surfacePitch + 2) = highColor32Palette[8];
					}
					else
#endif
					if (displayBpp == 16){
						// ?????
						*(ptDest2 - surfacePitch - 1) = highColorPalette[8];
						*(ptDest2 - surfacePitch) = highColorPalette[255];
						*(ptDest2 - surfacePitch + 1) = highColorPalette[8];
						*(ptDest2 - 1) = highColorPalette[255];
						*(ptDest2) = highColorPalette[255];
						*(ptDest2 + 1) = highColorPalette[255];
						*(ptDest2 + surfacePitch - 1) = highColorPalette[8];
						*(ptDest2 + surfacePitch) = highColorPalette[255];
						*(ptDest2 + surfacePitch + 1) = highColorPalette[8];
					}
					else{
						// ?????
						*(ptDest - surfacePitch - 1) = (char)8;
						*(ptDest - surfacePitch) = (char)255;
						*(ptDest - surfacePitch + 1) = (char)8;
						*(ptDest - 1) = (char)255;
						*(ptDest) = (char)255;
						*(ptDest + 1) = (char)255;
						*(ptDest + surfacePitch - 1) = (char)8;
						*(ptDest + surfacePitch) = (char)255;
						*(ptDest + surfacePitch + 1) = (char)8;
					}
				}
				else if (buf->mode == 1){
#ifdef _HI_COLOR_32
					if (displayBpp == 32){
						// ?????
						*(ptDest32 - surfacePitch) = highColor32Palette[251];
						*(ptDest32 - 1) = highColor32Palette[251];
						*(ptDest32) = highColor32Palette[251];
						*(ptDest32 + 1) = highColor32Palette[251];
						*(ptDest32 + surfacePitch) = highColor32Palette[251];
					}
					else
#endif
					if (displayBpp == 16){
						// ?????
						*(ptDest2 - surfacePitch) = highColorPalette[251];
						*(ptDest2 - 1) = highColorPalette[251];
						*(ptDest2) = highColorPalette[251];
						*(ptDest2 + 1) = highColorPalette[251];
						*(ptDest2 + surfacePitch) = highColorPalette[251];
					}
					else{
						// ?????
						*(ptDest - surfacePitch) = (char)251;
						*(ptDest - 1) = (char)251;
						*(ptDest) = (char)251;
						*(ptDest + 1) = (char)251;
						*(ptDest + surfacePitch) = (char)251;
					}
				}
				else if (buf->mode == 2){
#ifdef _HI_COLOR_32
					if (displayBpp == 32){
						// ???????
						*(ptDest32) = highColor32Palette[251];
						*(ptDest32 + 1) = highColor32Palette[255];
						*(ptDest32 + surfacePitch) = highColor32Palette[255];
						*(ptDest32 + surfacePitch + 1) = highColor32Palette[251];
					}
					else
#endif
					if (displayBpp == 16){
						// ???????
						*(ptDest2) = highColorPalette[251];
						*(ptDest2 + 1) = highColorPalette[255];
						*(ptDest2 + surfacePitch) = highColorPalette[255];
						*(ptDest2 + surfacePitch + 1) = highColorPalette[251];
					}
					else{
						// ???????
						*(ptDest) = (char)251;
						*(ptDest + 1) = (char)255;
						*(ptDest + surfacePitch) = (char)255;
						*(ptDest + surfacePitch + 1) = (char)251;
					}
				}
				else if (buf->mode == 3){
#ifdef _HI_COLOR_32
					if (displayBpp == 32){
						// ???????
						*(ptDest32) = highColor32Palette[255];
						*(ptDest32 + 1) = highColor32Palette[251];
						*(ptDest32 + surfacePitch) = highColor32Palette[251];
						*(ptDest32 + surfacePitch + 1) = highColor32Palette[255];
					}
					else
#endif
					if (displayBpp == 16){
						// ???????
						*(ptDest2) = highColorPalette[255];
						*(ptDest2 + 1) = highColorPalette[251];
						*(ptDest2 + surfacePitch) = highColorPalette[251];
						*(ptDest2 + surfacePitch + 1) = highColorPalette[255];
					}
					else{
						// ???????
						*(ptDest) = (char)255;
						*(ptDest + 1) = (char)251;
						*(ptDest + surfacePitch) = (char)251;
						*(ptDest + surfacePitch + 1) = (char)255;
					}
				}
				else if (buf->mode == 4){
#ifdef _HI_COLOR_32
					if (displayBpp == 32){
						// ???????
						*(ptDest32) = highColor32Palette[251];
						*(ptDest32 + 1) = highColor32Palette[193];
						*(ptDest32 + surfacePitch) = highColor32Palette[193];
						*(ptDest32 + surfacePitch + 1) = highColor32Palette[193];
					}
					else
#endif	
					if (displayBpp == 16){
						// ???????
						*(ptDest2) = highColorPalette[251];
						*(ptDest2 + 1) = highColorPalette[193];
						*(ptDest2 + surfacePitch) = highColorPalette[193];
						*(ptDest2 + surfacePitch + 1) = highColorPalette[193];
					}
					else{
						// ???????
						*(ptDest) = (char)251;
						*(ptDest + 1) = (char)193;
						*(ptDest + surfacePitch) = (char)193;
						*(ptDest + surfacePitch + 1) = (char)193;
					}
				}
				else if (buf->mode == 5){
#ifdef _HI_COLOR_32
					if (displayBpp == 32){
						// ?????
						*(ptDest32) = highColor32Palette[198];
					}
					else
#endif
					if (displayBpp == 16){
						// ?????
						*(ptDest2) = highColorPalette[198];
					}
					else{
						// ?????
						*(ptDest) = (char)198;
					}
				}
				else if (buf->mode == 6){
#ifdef _HI_COLOR_32
					if (displayBpp == 32){
						// ?????
						*(ptDest32) = highColor32Palette[193];
					}
					else
#endif
					if (displayBpp == 16){
						// ?????
						*(ptDest2) = highColorPalette[193];
					}
					else{
						// ?????
						*(ptDest) = (char)193;
					}
				}
				else if (buf->mode == 7){
#ifdef _HI_COLOR_32
					if (displayBpp == 32){
						// ?????
						*(ptDest32) = highColor32Palette[208];
					}
					else
#endif
					if (displayBpp == 16){
						// ?????
						*(ptDest2) = highColorPalette[208];
					}
					else{
						// ?????
						*(ptDest) = (char)208;
					}
				}
				else if (buf->mode == 8){
#ifdef _HI_COLOR_32
					if (displayBpp == 32){
						// ???
						*(ptDest32) = highColor32Palette[251];
					}
					else
#endif
					if (displayBpp == 16){
						// ???
						*(ptDest2) = highColorPalette[251];
					}
					else{
						// ???
						*(ptDest) = (char)251;
					}
				}
			}
			else if (buf->type == MAP_EFFECT_TYPE_KAMIFUBUKI){
				// ????
				if (buf->mode == 0){
					if (buf->type2 == 0)
						color = (char)223;
					else if (buf->type2 == 1)
						color = (char)217;
					else
						color = (char)159;
				}
				else if (buf->mode == 1){
					if (buf->type2 == 0)
						color = (char)212;
					else if (buf->type2 == 1)
						color = (char)208;
					else
						color = (char)159;
				}
				else if (buf->mode == 2){
					if (buf->type2 == 0)
						color = (char)130;
					else if (buf->type2 == 1)
						color = (char)147;
					else
						color = (char)159;
				}
				else{
					if (buf->type2 == 0)
						color = (char)116;
					else if (buf->type2 == 1)
						color = (char)99;
					else
						color = (char)159;
				}
				for (i = 0; i < buf->h; i++){
					for (j = 0; j < buf->w; j++){
						if (0 <= (buf->ex + j) && (buf->ex + j) < ww && 0 <= (buf->ey + i) && (buf->ey + i) < hh){
#ifdef _HI_COLOR_32
							if (displayBpp == 32){
								*(ptDest32 + surfacePitch*(i + buf->ey) + j + buf->ex) = highColor32Palette[(unsigned char)color];
							}
							else
#endif
							if (displayBpp == 16)
								*(ptDest2 + surfacePitch*(i + buf->ey) + j + buf->ex) = highColorPalette[(unsigned char)color];
							else
								*(ptDest + surfacePitch*(i + buf->ey) + j + buf->ex) = color;
						}
					}
				}
			}
			else if (buf->type == MAP_EFFECT_TYPE_HOTARU){
			}
		}
		buf = buf->next;
	}
	if (lpDraw->lpBACKBUFFER->Unlock(NULL) != DD_OK)
		return;
	return;
}






// ???????????? ????? ?************************************/
void DrawDebugLine(unsigned char color)
{
	DDSURFACEDESC ddsd;	// ??????
	char *pDest;		// ??????
	short *pDest2;		// ????????????
	int surfacePitch;	// ??????????
	int i, j, k, l, m;
	short pixel;

	// ????????????
	if (WindowMode) return;

	// ?????
	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC));
	ddsd.dwSize = sizeof(DDSURFACEDESC);

	// ?????????????????( ?? ddsd ??????? )
	if (lpDraw->lpFRONTBUFFER->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK){
		//MessageBoxNew( hWnd, "Surface的lock失败！", "确定", MB_OK | MB_ICONSTOP );
		return;
	}
	//??
	j = lpDraw->xSize;
	k = lpDraw->ySize;
	l = 8;
	//???????????????
	if (ResoMode == 1){
		j >>= 1;
		k >>= 1;
		l >>= 1;
	}
#ifdef _HI_COLOR_32
	if (displayBpp == 32)
	{
		int pixel;
		int *pDest32;
		pixel = highColor32Palette[color];
		// ?????????????
		pDest32 = (int *)ddsd.lpSurface + j;
		// ??????????
		surfacePitch = ddsd.lPitch >> 1;
		// ???
		for (i = 0; i < k; i++)
		{
			// ??????
			for (m = 0; m < l; m++)
			{
				*(pDest32 + m) = pixel;
			}
			// ???????????????
			pDest32 += surfacePitch;
		}
	}
	else
#endif
	if (displayBpp == 16)
	{
		pixel = highColorPalette[color];
		// ?????????????
		pDest2 = (short *)ddsd.lpSurface + j;
		// ??????????
		surfacePitch = ddsd.lPitch >> 1;
		// ???
		for (i = 0; i < k; i++)
		{
			// ??????
			for (m = 0; m < l; m++)
			{
				*(pDest2 + m) = pixel;
			}
			// ???????????????
			pDest2 += surfacePitch;
		}
	}
	else
	{
		// ?????????????
		pDest = (char *)(ddsd.lpSurface) + j;

		// ??????????
		surfacePitch = ddsd.lPitch;

		// ???
		for (i = 0; i < k; i++)
		{
			// ??????
			memset(pDest, color, l);
			// ???????????????
			pDest += surfacePitch;
		}
	}

	// ???????????????????
	if (lpDraw->lpFRONTBUFFER->Unlock(NULL) != DD_OK){
		MessageBoxNew(hWnd, "Surface的Unlock失败！", "确定", MB_OK | MB_ICONSTOP);
		return;
	}

	return;
}
//---------------------------------------------------------------------------//
// ?? ：RECT???忣帮搎匏直??忤站赶怐????]?                    //
// ?? ：DIRECT_DRAW *lpDraw : DirectDraw???                         //
//        short  bx           : ???                                 //
//        short  by           : ??                                 //
//        LPDIRECTDRAWSURFACE lpSurface : ???????                   //
// ? ：DD_OK:?                                                     //
//---------------------------------------------------------------------------//
#ifdef _READ16BITBMP
HRESULT DrawSurfaceFast( long bx, long by, LPDIRECTDRAWSURFACE lpSurface,LPDIRECTDRAWSURFACE lpSurfaceSys)
{
	long x0, y0;
	long w, h;

	RECT rect = { 0, 0, SurfaceSizeX, SurfaceSizeY };
	x0 = bx;
	y0 = by;
	w = rect.right - rect.left;
	h = rect.bottom - rect.top;

	if( bx >= lpDraw->xSize || bx + w <= 0 || by >= lpDraw->ySize || by + h <= 0 ){
		return DD_OK;
	}

	// ??????
	if( bx < 0 ){
		rect.left -= bx;
		x0 = 0;
	}
	// ??????
	if( bx + w > lpDraw->xSize ){
		rect.right -= bx + w - lpDraw->xSize;
	}
	// ?????
	if( by < 0 ){
		rect.top -= by;
		y0 = 0;
	}
	// ??????
	if( by + h > lpDraw->ySize ){
		rect.bottom -= by + h - lpDraw->ySize;
	}

#ifdef _STONDEBUG_		
	// ?????????????????
	SurfaceDispCnt++;
#endif
	RECT rectD;

	rectD.left = x0;
	rectD.top = y0;
	rectD.right = x0 + (rect.right - rect.left);
	rectD.bottom = y0 + (rect.bottom - rect.top);
	lpDraw->lpBACKBUFFERSYS->Blt(&rectD,lpSurfaceSys,&rect,DDBLT_KEYSRC | DDBLT_WAIT,NULL);
	return lpDraw->lpBACKBUFFER->Blt(&rectD,lpSurface,&rect,DDBLT_KEYSRC | DDBLT_WAIT,NULL);
}
#endif

HRESULT DrawSurfaceFast(short bx, short by, LPDIRECTDRAWSURFACE lpSurface)
{
	short x0, y0;
	long w, h;
	RECT rect = { 0, 0, SurfaceSizeX, SurfaceSizeY };
	x0 = bx;
	y0 = by;
	w = rect.right - rect.left;
	h = rect.bottom - rect.top;
	if (bx >= lpDraw->xSize || bx + w <= 0 || by >= lpDraw->ySize || by + h <= 0){
		return DD_OK;
	}
	if (bx < 0){
		rect.left -= bx;
		x0 = 0;
	}
	if (bx + w > lpDraw->xSize){
		rect.right -= bx + w - lpDraw->xSize;
	}
	if (by < 0){
		rect.top -= by;
		y0 = 0;
	}
	if (by + h > lpDraw->ySize){
		rect.bottom -= by + h - lpDraw->ySize;
	}

#ifdef _STONDEBUG_		
	SurfaceDispCnt++;
#endif
	RECT rectD;

	rectD.left = x0;
	rectD.top = y0;
	rectD.right = x0 + (rect.right - rect.left);
	rectD.bottom = y0 + (rect.bottom - rect.top);
	return lpDraw->lpBACKBUFFER->Blt(&rectD, lpSurface, &rect, DDBLT_KEYSRC | DDBLT_WAIT, NULL);
}

//---------------------------------------------------------------------------//
// ?? ：RECT???忣帮搎匏直??忤站赶怐????]?                    //
// ?? ：DIRECT_DRAW *lpDraw : DirectDraw???                         //
//        short  bx           : ???                                 //
//        short  by           : ??                                 //
//        RECT * rect         : ?????                             //
//        LPDIRECTDRAWSURFACE lpSurface : ???????                   //
// ? ：DD_OK:?                                                     //
//---------------------------------------------------------------------------//
HRESULT DrawSurfaceFast2(short bx, short by, RECT *rect, LPDIRECTDRAWSURFACE lpSurface)
{
	short x0, y0;
	long w, h;

	x0 = bx;
	y0 = by;
	w = rect->right - rect->left;
	h = rect->bottom - rect->top;

	// ????????
	//   ????? RECT ??????????????

	// ??????????
	if (bx >= lpDraw->xSize || bx + w <= 0 || by >= lpDraw->ySize || by + h <= 0){
		return DD_OK;
	}

	// ??????
	if (bx < 0){
		rect->left -= bx;
		x0 = 0;
	}
	// ??????
	if (bx + w > lpDraw->xSize){
		rect->right -= bx + w - lpDraw->xSize;
	}
	// ?????
	if (by < 0){
		rect->top -= by;
		y0 = 0;
	}
	// ??????
	if (by + h > lpDraw->ySize){
		rect->bottom -= by + h - lpDraw->ySize;
	}

#ifdef _STONDEBUG_		
	// ?????????????????
	SurfaceDispCnt++;
#endif

	// ???????????
	//return lpDraw->lpBACKBUFFER->BltFast( x0, y0, lpSurface, rect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );
	return lpDraw->lpBACKBUFFER->BltFast(x0, y0, lpSurface, rect, DDBLTFAST_WAIT);
}


/* ?????????? ****************************************************/
void ReleaseDirectDraw(void)
{
	int i;
	// ????????????????
	for (i = 0; i < SurfaceCnt; i++){
		if (SurfaceInfo[i].lpSurface != NULL){
			if (SurfaceInfo[i].lpAlphaData != NULL){
				FREE(SurfaceInfo[i].lpAlphaData);
				SurfaceInfo[i].lpAlphaData = NULL;
			}
			SurfaceInfo[i].lpSurface->Release();
			SurfaceInfo[i].lpSurface = NULL;
#ifdef _READ16BITBMP
			if(g_bUseAlpha){
				SurfaceInfoSys[i].lpSurface->Release();
				SurfaceInfoSys[i].lpSurface = NULL;
			}
#endif
		}
#ifdef _READ16BITBMP
		if(g_bUseAlpha){
			if(SurfaceInfo[i].lpAlphaData != NULL){
				FREE(SurfaceInfo[i].lpAlphaData);
#ifdef _STONDEBUG_
				g_iMallocCount--;
#endif
				SurfaceInfo[i].lpAlphaData = NULL;
			}
		}
#endif
	}
#ifdef __SKYISLAND
	extern void SkyIslandRelease();
	SkyIslandRelease();
#endif
	// ??????????
	if (lpBattleSurface != NULL){
		// ????
		lpBattleSurface->Release();
		lpBattleSurface = NULL;
	}
	if (lpDraw){
		// ?????
		if (lpDraw->lpPALETTE != NULL){
			lpDraw->lpPALETTE->Release();
			lpDraw->lpPALETTE = NULL;
		}
		// ??????
		if (lpDraw->lpCLIPPER != NULL){
			lpDraw->lpCLIPPER->Release();
			lpDraw->lpCLIPPER = NULL;
		}
		// ?????????
		if (lpDraw->lpBACKBUFFER != NULL){
			lpDraw->lpBACKBUFFER->Release();
			lpDraw->lpBACKBUFFER = NULL;
		}
#ifdef _READ16BITBMP
		if(g_bUseAlpha){
			if(lpDraw->lpBACKBUFFERSYS != NULL){
				lpDraw->lpBACKBUFFERSYS->Release();
				lpDraw->lpBACKBUFFERSYS = NULL;
			}
			if(lpBattleSurfaceSys != NULL){
				lpBattleSurfaceSys->Release();
				lpBattleSurfaceSys = NULL;
			}
		}
#endif
		// ???????????
		if (lpDraw->lpFRONTBUFFER != NULL){
			lpDraw->lpFRONTBUFFER->Release();
			lpDraw->lpFRONTBUFFER = NULL;
		}
		// DirectDraw??
		if (lpDraw->lpDD2 != NULL){
			lpDraw->lpDD2->Release();
			lpDraw->lpDD2 = NULL;
		}

		// DIRECT_DRAW ??
		HeapFree(GetProcessHeap(), NULL, lpDraw);
		lpDraw = NULL;
	}
	// DirectDraw ??????? FALSE ???
	DDinitFlag = FALSE;
}

// ??????????????? ////////////////////////////////////////////
BOOL CheckSurfaceLost(void)
{
	BOOL SurfaceLostFlag = FALSE;
	int i;
	// ?????????????????
	for (i = 0; i < SurfaceCnt; i++){
		// ??????????
		if (SurfaceInfo[i].lpSurface != NULL){
			// ????????????????
#ifdef _READ16BITBMP
			if(g_bUseAlpha) 
			if(SurfaceInfoSys[i].lpSurface->IsLost()) SurfaceLostFlag = TRUE;
#endif
			if (SurfaceInfo[i].lpSurface->IsLost()) SurfaceLostFlag = TRUE;
		}
	}
	// ???????????
	if (lpBattleSurface != NULL){
		// ????
		if (lpBattleSurface->IsLost()){
			SurfaceLostFlag = TRUE;
		}
	}
#ifdef __SKYISLAND
#ifndef __CARYTEST
	extern BOOL SkyIslandCheckSurface();
	if (SkyIslandCheckSurface())
		SurfaceLostFlag = TRUE;
#endif
#endif
	// ??????????
	if (lpDraw->lpBACKBUFFER != NULL){
		// ????????????????
		if (lpDraw->lpBACKBUFFER->IsLost()){
			SurfaceLostFlag = TRUE;
		}
	}
	// ????????????
	if (lpDraw->lpFRONTBUFFER != NULL){
		// ????????????????
		if (lpDraw->lpFRONTBUFFER->IsLost()){
			SurfaceLostFlag = TRUE;
		}
	}

	return SurfaceLostFlag;
}

// ???????????? *************************************************/
void InitFont(int fontNo)
{
	//宋体
	//char *fontName[] = { "Microsoft JhengHei","楷体_GB2312" };
#ifdef _NEWFONT_
	extern int 编码;
	char fontName[2][128];
	if(编码==950){
		sprintf(fontName[0],"Microsoft JhengHei");
		sprintf(fontName[1],"Microsoft JhengHei");
	}else{
		sprintf(fontName[0],"Microsoft JhengHei");
		sprintf(fontName[1],"Microsoft JhengHei");
	}
#else
	extern int 编码;
	char fontName[2][128];
	if(编码==950){
		sprintf(fontName[0],"Microsoft JhengHei");
		sprintf(fontName[1],"Microsoft JhengHei");
	}else{
		sprintf(fontName[0],"宋体");
		sprintf(fontName[1],"楷体_GB2312");
	}

	//char *fontName[] = { "宋体", "楷体_GB2312" };
#endif
	// ?????????
	if (hFont != NULL){
		// ????????????
		DeleteObject(hFont);
	}

	//???????????????
	if (ResoMode == 1){
		// MS??????????????????????
		hFont = CreateFont(
#ifdef _NEWFONT_
			FONT_SIZE1, 	/* ????????	*/ 
#else
			FONT_SIZE1,
#endif
			0, 		/* ??????	*/
			0, 		/* ???????	*/
			0,		/* ??? ????x??	*/
			/* ??????	*/
			FW_NORMAL,		// 0 
			/* ?????????	*/
			FALSE,
			/* ??????	*/
			FALSE,
			/* ???????	*/
			FALSE,
			/* ????	*/
			GB2312_CHARSET/*CHINESEBIG5_CHARSET*/, 	// ??? GB2312_CHARSET
			/* ?	*/
			OUT_DEFAULT_PRECIS,
			/* ??????	*/
			CLIP_DEFAULT_PRECIS,
			/* ?	*/
			DEFAULT_QUALITY,	// ????????????????
			/* ??? */
			FIXED_PITCH |
			/* ????	*/
			FF_ROMAN,			// ???????????????????????MS(R) Serif????????
			fontName[fontNo]);
	}
	else {
#ifdef _NEWFONT_
		char strfame[128];
		if(编码==950){
			sprintf(strfame,"Microsoft JhengHei");
		}else{
			sprintf(strfame,"Microsoft JhengHei");
		}
		hFont = CreateFont(FONT_SIZE1,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,1,
			0,0,0,17,(LPCTSTR)strfame
			);
#else

		char strfame[128];
		if(编码==950){
			sprintf(strfame,"Microsoft JhengHei");
			hFont = CreateFont(FONT_SIZE2,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,1,
			0,0,0,17,(LPCTSTR)strfame
			);
		}else{
			hFont = CreateFont(
			FONT_SIZE1,
			0,
			0,
			0,
			FW_NORMAL,
			FALSE,
			FALSE,
			FALSE,
			GB2312_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			FIXED_PITCH |
			FF_ROMAN,
			fontName[fontNo]);
		}
#endif
	}
}

#ifdef _CHANNEL_MODIFY
char g_szChannelTitle[][13] = { "[普]", "[密]", "[队]", "[族]"
#ifdef _CHAR_PROFESSION
,"[职]"
#endif
#ifdef _CHATROOMPROTOCOL
,"[聊]"
#endif
#ifdef _CHANNEL_WORLD
, "[世]"
#endif
#ifdef _CHANNEL_ALL_SERV
, "[星]"
#endif
};
extern int TalkMode;
#endif
// ????????????????? ////////////////////////////////////////
void PutText(char fontPrio)
{
	HDC  hDc;
#ifdef _READ16BITBMP
	HDC  hDcSys;
#endif
	int i;
	BOOL colorFlag = FALSE;
	int	color;
	if (FontCnt == 0) return;
	lpDraw->lpBACKBUFFER->GetDC(&hDc);
#ifdef _READ16BITBMP
	if(g_bUseAlpha) lpDraw->lpBACKBUFFERSYS->GetDC(&hDcSys);
#endif
	SetBkMode(hDc, TRANSPARENT);
#ifdef _READ16BITBMP
	if(g_bUseAlpha) SetBkMode(hDcSys,TRANSPARENT);
#endif
	HFONT hOldFont = (HFONT)SelectObject(hDc, hFont);
#ifdef _READ16BITBMP
	if(g_bUseAlpha) SelectObject(hDcSys,hFont);
#endif
	SetTextColor(hDc, 0);
#ifdef _READ16BITBMP
	if(g_bUseAlpha) SetTextColor(hDcSys,0);
#endif
	for (i = 0; i < FontCnt; i++){
		if (FontBuffer[i].fontPrio == fontPrio){
			if (ResoMode == 1){
				TextOut(hDc, FontBuffer[i].x / 2 + 1, FontBuffer[i].y / 2 + 1, FontBuffer[i].str, (int)strlen(FontBuffer[i].str));
			}
			else {
#ifdef _FONT_SIZE
				static HFONT newFont = NULL;
				static HFONT oldFont = NULL;
				if (newFont != NULL) {
					DeleteObject(newFont);
				}
				if (FontBuffer[i].size <= 0)
					SelectObject(hDc, hFont);
				else {
					newFont = CreateNewFont(FontBuffer[i].size);
					if (newFont != NULL)
						oldFont = (HFONT)SelectObject(hDc, newFont);
				}
#endif
#ifndef _CHANNEL_MODIFY
				TextOut( hDc, FontBuffer[ i ].x + 1, FontBuffer[ i ].y + 1, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
#ifdef _READ16BITBMP
				if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + 1, FontBuffer[ i ].y + 1, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
#endif
#else
				if (FontBuffer[i].fontPrio != FONT_PRIO_CHATBUFFER){
#ifdef _FONT_PARAGRAPH_COLOR
					char *s = FontBuffer[ i ].str;
					int x = 0;
					int len = 0;
					while(char *ss = strstr(s, "/")){
						ss ++;
						len = ss - s - 1;
						if(ss[0] >= 'a' && ss[0] <= 'z'){
							ss++;
						}else{
							len++;
						}
						TextOut( hDc, FontBuffer[ i ].x + 1 + x * 7, FontBuffer[ i ].y + 1, s, len);
#ifdef _READ16BITBMP
						if(g_bUseAlpha){
							TextOut( hDcSys, FontBuffer[ i ].x + x * 7, FontBuffer[ i ].y + 1, s, len);
						}
#endif
						s = ss;
						x += len;
					}
					TextOut( hDc, FontBuffer[ i ].x + 1 + x * 7, FontBuffer[ i ].y + 1, s, strlen(s));
#ifdef _READ16BITBMP
					if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + x * 7, FontBuffer[ i ].y + 1, s, strlen(s));
#endif
#else
					TextOut(hDc, FontBuffer[i].x + 1, FontBuffer[i].y + 1, FontBuffer[i].str, (int)strlen(FontBuffer[i].str));
#ifdef _READ16BITBMP
					if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + 1, FontBuffer[ i ].y + 1, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
#endif
#endif
				}
				else{
#ifdef _TALK_WINDOW
					if(g_bTalkWindow) {
						break;
					}
#endif

#ifdef _FONT_PARAGRAPH_COLOR
					char *s = FontBuffer[ i ].str;
					int x = 0;
					int len = 0;
					while(char *ss = strstr(s, "/")){
						ss ++;
						len = ss - s - 1;

						if(ss[0] < 'a' || ss[0] > 'z'){
							len--;
						}
						if(false){
							TextOut( hDc, FontBuffer[ i ].x + 1 + x * 7, FontBuffer[ i ].y + 1, s, len + 2);
#ifdef _READ16BITBMP
							if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + 1 + x * 7, FontBuffer[ i ].y + 1, s, len + 2);
#endif
						}
						else{
							TextOut( hDc, FontBuffer[ i ].x + 1 + 22 + x * 7, FontBuffer[ i ].y + 1, s, len + 2);
#ifdef _READ16BITBMP
							if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + 1 + 22 + x * 7, FontBuffer[ i ].y + 1, s, len + 2);
#endif
						}
						if(ss[0] >= 'a' && ss[0] <= 'z'){
							ss++;
						}
						s = ss;
						x += len + 2;
					}

					if(false){
						TextOut( hDc, FontBuffer[ i ].x + 1 + x * 7, FontBuffer[ i ].y + 1, s, strlen(s));
#ifdef _READ16BITBMP
						if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + 1 + x * 7, FontBuffer[ i ].y + 1, s, strlen(s));
#endif
					}
					else{
						TextOut( hDc, FontBuffer[ i ].x + 1 + 22 + x * 7, FontBuffer[ i ].y + 1, s, strlen(s));
#ifdef _READ16BITBMP
						if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + 1 + 22 + x * 7, FontBuffer[ i ].y + 1, s, strlen(s));
#endif
					}
#else

					if (false){
						TextOut(hDc, FontBuffer[i].x + 1, FontBuffer[i].y + 1, FontBuffer[i].str, (int)strlen(FontBuffer[i].str));
#ifdef _READ16BITBMP
						if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + 1, FontBuffer[ i ].y + 1, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
#endif
					}
					else{
						TextOut(hDc, FontBuffer[i].x + 1 + 22, FontBuffer[i].y + 1, FontBuffer[i].str, (int)strlen(FontBuffer[i].str));
#ifdef _READ16BITBMP
						if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + 1 + 22, FontBuffer[ i ].y + 1, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
#endif
					}
#endif
#ifndef LABEL_STR_
					TextOut(hDc, 1 + 1, 432 + DISPLACEMENT_Y + 1, g_szChannelTitle[TalkMode], (int)strlen(g_szChannelTitle[TalkMode]));
#ifdef _READ16BITBMP
					if(g_bUseAlpha) TextOut(hDcSys,2 + 1,432 + DISPLACEMENT_Y + 1,g_szChannelTitle[TalkMode],(int)strlen(g_szChannelTitle[TalkMode])); 
#endif
#endif
				}
#endif
#ifdef _FONT_SIZE
				if (newFont != NULL) {
					DeleteObject(newFont);
					SelectObject(hDc, oldFont);
				}
#endif
			}
		}
	}
	// ????? SetTextColor?????????????? ?
	for (color = 0; color < FONT_PAL_NUM; color++){
		for (i = 0; i < FontCnt; i++){
			if (FontBuffer[i].fontPrio == fontPrio){
				if (FontBuffer[i].color == color){
					// ?????????????????
					if (colorFlag == FALSE){
						SetTextColor(hDc, FontPal[color]);
#ifdef _READ16BITBMP
						if(g_bUseAlpha) SetTextColor( hDcSys, FontPal[ color ] );
#endif
						colorFlag = TRUE;
					}
					//???????????????
					if (ResoMode == 1){
						// ?????????
						TextOut(hDc, FontBuffer[i].x >> 1, FontBuffer[i].y >> 1, FontBuffer[i].str, (int)strlen(FontBuffer[i].str));
					}
					else {
						// ?????????
#ifdef _FONT_SIZE
						static HFONT newFont = NULL;
						static HFONT oldFont = NULL;

						if (newFont != NULL) {
							DeleteObject(newFont);
							//SelectObject( hDc, oldFont );
						}
						if (FontBuffer[i].size <= 0)
							SelectObject(hDc, hFont);
						else {
							newFont = CreateNewFont(FontBuffer[i].size);
							if (newFont != NULL)
								oldFont = (HFONT)SelectObject(hDc, newFont);
						}
#endif

#ifndef _CHANNEL_MODIFY
						TextOut( hDc, FontBuffer[ i ].x, FontBuffer[ i ].y, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
#ifdef _READ16BITBMP
						if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x, FontBuffer[ i ].y, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
#endif
#else
						if (FontBuffer[i].fontPrio != FONT_PRIO_CHATBUFFER){
#ifdef _FONT_PARAGRAPH_COLOR
							char *s = FontBuffer[ i ].str;
							int x = 0;
							int len = 0;
							while(char *ss = strstr(s, "/")){
								ss ++;
								len = ss - s - 1;
								if(ss[0] < 'a' || ss[0] > 'z'){
									len++;
								}
								TextOut( hDc, FontBuffer[ i ].x + x * 7, FontBuffer[ i ].y, s, len);
#ifdef _READ16BITBMP
								if(g_bUseAlpha){
									TextOut( hDcSys, FontBuffer[ i ].x + x * 7, FontBuffer[ i ].y, s, len);
								}
#endif

								if(ss[0] >= 'a' && ss[0] <= 'z'){
									SetTextColor( hDc, FontPal[ ss[0] - 'a' ] );
									ss++;
								}

								s = ss;
								x += len;
							}
							TextOut( hDc, FontBuffer[ i ].x + x * 7, FontBuffer[ i ].y, s, strlen(s));
#ifdef _READ16BITBMP
							if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + x * 7, FontBuffer[ i ].y, s, strlen(s));
#endif
							SetTextColor( hDc, FontPal[ color ] );
#else
							TextOut(hDc, FontBuffer[i].x, FontBuffer[i].y, FontBuffer[i].str, (int)strlen(FontBuffer[i].str));
#ifdef _READ16BITBMP
							if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x, FontBuffer[ i ].y, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) );
#endif
#endif
						}
						else{
#ifdef _TALK_WINDOW
							if(g_bTalkWindow) break;
#endif

#ifdef _FONT_PARAGRAPH_COLOR
							char *s = FontBuffer[ i ].str;
							int x = 0;
							int len = 0;
							while(char *ss = strstr(s, "/")){
								ss ++;
								len = ss - s - 1;

								if(ss[0] < 'a' || ss[0] > 'z'){
									len--;
								}
								if(false){
									TextOut( hDc, FontBuffer[ i ].x + x * 7, FontBuffer[ i ].y, s, len + 2);
#ifdef _READ16BITBMP
									if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + x * 7, FontBuffer[ i ].y, s, len + 2);
#endif
								}
								else{
									TextOut( hDc, FontBuffer[ i ].x + 22 + x * 7, FontBuffer[ i ].y, s, len + 2);
#ifdef _READ16BITBMP
									if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + 22 + x * 7, FontBuffer[ i ].y, s, len + 2);
#endif
								}

								if(ss[0] >= 'a' && ss[0] <= 'z'){
									SetTextColor( hDc, FontPal[ ss[0] - 'a' ] );
									ss++;
								}

								s = ss;
								x += len + 2;
							}

							if(false){
								TextOut( hDc, FontBuffer[ i ].x + x * 7, FontBuffer[ i ].y, s, strlen(s));
#ifdef _READ16BITBMP
								if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + x * 7, FontBuffer[ i ].y, s, strlen(s));
#endif
							}
							else{
								TextOut( hDc, FontBuffer[ i ].x + 22 + x * 7, FontBuffer[ i ].y, s, strlen(s));
#ifdef _READ16BITBMP
								if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + 22 + x * 7, FontBuffer[ i ].y, s, strlen(s));
#endif
							}
							SetTextColor( hDc, FontPal[ color ] );
#else

							if (false){
								TextOut(hDc, FontBuffer[i].x, FontBuffer[i].y, FontBuffer[i].str, (int)strlen(FontBuffer[i].str));
#ifdef _READ16BITBMP
								if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x, FontBuffer[ i ].y, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
#endif
							}
							else{
								TextOut(hDc, FontBuffer[i].x + 22, FontBuffer[i].y, FontBuffer[i].str, (int)strlen(FontBuffer[i].str));
#ifdef _READ16BITBMP
								if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + 22, FontBuffer[ i ].y, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
#endif
							}
#endif
#ifndef LABEL_STR_
							TextOut(hDc, 1, 432 + DISPLACEMENT_Y, g_szChannelTitle[TalkMode], (int)strlen(g_szChannelTitle[TalkMode]));
#ifdef _READ16BITBMP
							if(g_bUseAlpha) TextOut(hDcSys,2,432 + DISPLACEMENT_Y,g_szChannelTitle[TalkMode],(int)strlen(g_szChannelTitle[TalkMode])); 
#endif
#endif
						}
#endif
#ifdef _FONT_SIZE
						if (newFont != NULL) {
							DeleteObject(newFont);
							SelectObject(hDc, oldFont);
						}
#endif
					}
				}
			}
		}
		colorFlag = FALSE;
	}
	SelectObject(hDc, hOldFont);
	lpDraw->lpBACKBUFFER->ReleaseDC(hDc);
#ifdef _READ16BITBMP
	if(g_bUseAlpha){
		SelectObject(hDcSys,hOldFont);
		lpDraw->lpBACKBUFFERSYS->ReleaseDC(hDcSys);
	}
#endif
}

//---------------------------------------------------------------------------//
// ?????????
//---------------------------------------------------------------------------//
void snapShot(void)
{
	DDSURFACEDESC ddsdDesc;
	RGBQUAD rgbpal[256];
	char fileName[256];
	int i;
	static struct tm nowTime;
	time_t longTime;
	FILE *fp;
	int w, h;
	POINT 	clientPoint; // ????????????????????

	// ?????????
	clientPoint.x = 0;
	clientPoint.y = 0;
	// ??????????????????
	ClientToScreen(hWnd, &clientPoint);

	// ??????
	if (_mkdir("screenshot") != 0)
	{
		if (errno != EEXIST)
			return;
	}

	// ??????
	time(&longTime);					// ?????
	localtime_s(&nowTime, &longTime);

	for (i = 0; i < 1000; i++)
	{
		sprintf_s(fileName, "screenshot\\sa%02d%02d%02d_%03d.bmp",
			(nowTime.tm_year % 100), nowTime.tm_mon + 1, nowTime.tm_mday, i);

		if ((fp = fopen(fileName, "r")) != NULL)
		{
			fclose(fp);
			continue;
		}
		else
		{
			break;
		}
	}

	if (displayBpp != 16){
		for (i = 0; i < 256; i++){
			rgbpal[i].rgbRed = Palette[i].peRed;
			rgbpal[i].rgbGreen = Palette[i].peGreen;
			rgbpal[i].rgbBlue = Palette[i].peBlue;
			rgbpal[i].rgbReserved = 0;
		}
	}
	memset(&ddsdDesc, 0, sizeof(DDSURFACEDESC));
	ddsdDesc.dwSize = sizeof(DDSURFACEDESC);
	if (lpDraw->lpFRONTBUFFER->Lock(NULL, &ddsdDesc, 0, NULL) != DD_OK){
#ifdef _STONDEBUG_
		MessageBoxNew(hWnd,"前景缓冲区锁定失败！", "确定", MB_OK | MB_ICONSTOP );
#endif
		return;
	}

	if (ResoMode == 1)
	{
		w = 320;
		h = 240;
	}
	else
	{
		w = lpDraw->xSize;
		h = lpDraw->ySize;
	}
	if (displayBpp == 16){
		BYTE *mem = new BYTE[w * h * 3], *pmem, pR, pG, pB;
		if (mem == NULL){
#ifdef _STONDEBUG_
			MessageBoxNew(hWnd,"记忆体配置失败失败！", "确定", MB_OK | MB_ICONSTOP );
#endif
			return;
		}
		WORD *work = (WORD*)ddsdDesc.lpSurface;
		pmem = mem;
		pmem += w * h * 3;

		// source face 一次移动二个byte
		ddsdDesc.lPitch >>= 1;
		// work 是整个荧幕的位置,要作偏移
		work += ddsdDesc.lPitch * g_clientPoint.y + g_clientPoint.x;
		for (int y = 0; y < h; y++){
			pmem -= w * 3;
			for (int x = 0; x < w; x++){
				// 565 显示模式
				if (gBitRShift == 2){
					pR = (BYTE)((((work[x] & 0xf800)) >> 11) << 3);
					pG = (BYTE)((((work[x] & 0x07e0)) >> 5) << 2);
					pB = (BYTE)((work[x] & 0x001f) << 3);
				}
				// 555 显示模式
				else{
					pR = (BYTE)((work[x] >> 10) << 3);
					pG = (BYTE)(((work[x] & 0x03e0) >> 5) << 3);
					pB = (BYTE)((work[x] & 0x001f) << 3);
				}
				*pmem++ = pB;
				*pmem++ = pG;
				*pmem++ = pR;
			}
			// 换行
			work += ddsdDesc.lPitch;
			pmem -= w * 3;
		}

		saveBmpFile16(fileName, mem, w, h);
		delete[] mem;
	}
	else
		saveBmpFile(fileName, (BYTE*)ddsdDesc.lpSurface,
		clientPoint.x, clientPoint.y, w, h, ddsdDesc.lPitch, rgbpal, 256);

	if (lpDraw->lpFRONTBUFFER->Unlock(NULL) != DD_OK)
		return;
}


// ???????????
BOOL saveBmpFile(const char *filename, BYTE *buf,
	int x, int y, int width, int height, int srcpitch,
	RGBQUAD *rgbpal, int colorCnt)
{
	HANDLE fh;
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;

	int linesize = (width + 3) / 4;
	DWORD writtensize;
	BYTE zero = 0;
	int i;

	fh = CreateFile(filename, GENERIC_WRITE, (DWORD)NULL,
		(LPSECURITY_ATTRIBUTES)NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if (fh == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}


	fileheader.bfType = 0x4D42;
	fileheader.bfSize = sizeof(BITMAPFILEHEADER)
		+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*(colorCnt)+linesize*height;
	fileheader.bfReserved1 = 0;
	fileheader.bfReserved2 = 0;
	fileheader.bfOffBits = sizeof(BITMAPFILEHEADER)
		+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*(colorCnt);

	WriteFile(fh, (void*)(&fileheader), sizeof(fileheader), &writtensize, NULL);


	infoheader.biSize = sizeof(BITMAPINFOHEADER);
	infoheader.biWidth = width;
	infoheader.biHeight = height;
	infoheader.biPlanes = 1;
	infoheader.biBitCount = 8;
	infoheader.biCompression = NULL;
	infoheader.biSizeImage = sizeof(BITMAPINFOHEADER)
		+sizeof(RGBQUAD)*(colorCnt)+linesize*height;
	infoheader.biXPelsPerMeter = 0;
	infoheader.biYPelsPerMeter = 0;
	infoheader.biClrUsed = colorCnt;
	infoheader.biClrImportant = 0;

	WriteFile(fh, (void*)(&infoheader), sizeof(BITMAPINFOHEADER), &writtensize, NULL);
	WriteFile(fh, (void*)rgbpal, sizeof(RGBQUAD)*(colorCnt), &writtensize, NULL);

	for (i = 0; i < height; i++)
	{
		WriteFile(fh, (void*)(buf + srcpitch*(height + y - 1 - i) + x), width, &writtensize, NULL);
		WriteFile(fh, (void*)(&zero), linesize - width, &writtensize, NULL);
	}

	CloseHandle(fh);

	return TRUE;
}

BOOL saveBmpFile16(const char *filename, BYTE *buf, int width, int height)
{
	HANDLE fh;
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;

	DWORD writtensize;
	BYTE zero = 0;

	fh = CreateFile(filename, GENERIC_WRITE, (DWORD)NULL,
		(LPSECURITY_ATTRIBUTES)NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if (fh == INVALID_HANDLE_VALUE) return FALSE;

	fileheader.bfType = 0x4D42;
	fileheader.bfSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+width * height * 3;
	fileheader.bfReserved1 = 0;
	fileheader.bfReserved2 = 0;
	fileheader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	infoheader.biSize = sizeof(BITMAPINFOHEADER);
	infoheader.biWidth = width;
	infoheader.biHeight = height;
	infoheader.biPlanes = 1;
	infoheader.biBitCount = 24;
	infoheader.biCompression = 0;
	infoheader.biSizeImage = width * height * 3;
	infoheader.biXPelsPerMeter = 0;
	infoheader.biYPelsPerMeter = 0;
	infoheader.biClrUsed = 0;
	infoheader.biClrImportant = 0;

	WriteFile(fh, (void*)(&fileheader), sizeof(fileheader), &writtensize, NULL);
	WriteFile(fh, (void*)(&infoheader), sizeof(BITMAPINFOHEADER), &writtensize, NULL);
	WriteFile(fh, (void*)buf, width * height * 3, &writtensize, NULL);

	CloseHandle(fh);

	return TRUE;
}

#ifdef _READ16BITBMP
void Draw16BitmapToSurface2(SURFACE_INFO *surface_info,LPDIRECTDRAWSURFACE lpSurfaceSys,int offsetX,int offsetY,int sizeX,int sizeY,LPBITMAPINFO pBmpInfo)
{
	DDSURFACEDESC ddsd;
	WORD *pDest;
	BYTE R,G,B;
	char *pSource;
	int surfacePitch,i,j;
	DDSURFACEDESC ddsdsys;
	WORD *pDestSys;
	int surfacePitchSys;
#ifdef _HI_COLOR_32
	int *pDest32;
	int *pDestSys32;
	int surfacePitch32;
	int surfacePitchSys32;
#endif

	if(surface_info->lpSurface == NULL) return;
	if(g_bUseAlpha && lpSurfaceSys == NULL) return;

	ZeroMemory(&ddsd,sizeof(DDSURFACEDESC));
	ddsd.dwSize = sizeof(DDSURFACEDESC);
	ZeroMemory(&ddsdsys,sizeof(DDSURFACEDESC));
	ddsdsys.dwSize = sizeof(DDSURFACEDESC);

	if(FAILED(surface_info->lpSurface->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL))) return; 
	if(g_bUseAlpha){
		if(FAILED(lpSurfaceSys->Lock(NULL,&ddsdsys,DDLOCK_WAIT,NULL))){
			surface_info->lpSurface->Unlock(NULL);
			return; 
		}
	}
#ifdef _HI_COLOR_32
	if( displayBpp == 32 ){
		pDest32 = (int*)(ddsd.lpSurface);
		if(g_bUseAlpha) pDestSys32 = (int*)(ddsdsys.lpSurface);
		pSource = pRealBinBits + offsetY * RealBinWidth + offsetX;
		surfacePitch32 = ddsd.lPitch >> 2;
		if(g_bUseAlpha) surfacePitchSys32 = ddsdsys.lPitch >> 2;

		for(i=0;i<sizeY;i++){
			for(j=0;j<sizeX;j++){
				R = g_rgbPal[(BYTE)*pSource].rgbRed;
				G = g_rgbPal[(BYTE)*pSource].rgbGreen;
				B = g_rgbPal[(BYTE)*pSource].rgbBlue;
				*pDest32 = (int)((R << 16) | (G << 8) | B);
				pDest32++;
				pSource++;
				if(g_bUseAlpha){
					*pDestSys32 = (int)((R << 16) | (G << 8) | B);
					pDestSys32++;
				}
			}
			// 换下一行
			pDest32 += surfacePitch32 - sizeX;
			pSource += RealBinWidth - sizeX;
			if(g_bUseAlpha) pDestSys32 += surfacePitchSys32 - sizeX;
		}
	}else
#endif
	{
		pDest = (WORD*)(ddsd.lpSurface);
		if(g_bUseAlpha) pDestSys = (WORD*)(ddsdsys.lpSurface);
		pSource = pRealBinBits + offsetY * RealBinWidth + offsetX;
		surfacePitch = ddsd.lPitch >> 1;
		if(g_bUseAlpha) surfacePitchSys = ddsdsys.lPitch >> 1;

		for(i=0;i<sizeY;i++){
			for(j=0;j<sizeX;j++){
				R = g_rgbPal[(BYTE)*pSource].rgbRed >> 3;
				G = g_rgbPal[(BYTE)*pSource].rgbGreen >> 2;
				B = g_rgbPal[(BYTE)*pSource].rgbBlue >> 3;
				*pDest = (WORD)((R << 11) | (G << 5) | B);
				pDest++;
				pSource++;
				if(g_bUseAlpha){
					*pDestSys = (WORD)((R << 11) | (G << 5) | B);
					pDestSys++;
				}
			}
			// 换下一行
			pDest += surfacePitch - sizeX;
			pSource += RealBinWidth - sizeX;
			if(g_bUseAlpha) pDestSys += surfacePitchSys - sizeX;
		}
	}

	surface_info->lpSurface->Unlock(NULL);
	if(g_bUseAlpha) lpSurfaceSys->Unlock(NULL);
	// 有带alpha channel
	static UCHAR a[1];

	if(g_bUseAlpha){
		if(surface_info->useAlpha == TRUE){
			BYTE *pAlphaSource = pRealBinAlpha + offsetY * RealBinWidth + offsetX; 
			BYTE *pAlphaDest = surface_info->lpAlphaData;
			memset(pAlphaDest,0,SurfaceSizeX*SurfaceSizeY);
			for(i=0;i<sizeY;i++){
				for(j=0;j<sizeX;j++){
					*pAlphaDest = (*pAlphaSource) >> 3;
					pAlphaDest++;
					pAlphaSource++;
				}
				// 换下一行
				pAlphaDest += SurfaceSizeX - sizeX;
				pAlphaSource += RealBinWidth - sizeX;
			}
		}
	}
	return;
}
#endif

int CG_PKSERVER_PANEL;
int CG_BATTTLE_SKILLCHOICE;
int CG_FIELD_SKILL_PANEL;
int CG_FIELD_CHATROOM_PANEL;
int CG_FIELD_SV_SELL_PANEL;
int CG_FIELD_SV_SELL_PRICE_PANEL;
int CG_NEW_STATUS_WND;
int CG_NEWITEM_WND;
int CG_TRADE_WND;
int CG_TRADE_VIEWWND;
// ??????
int CG_WND_G_0;
int CG_WND_G_1;
int CG_WND_G_2;
int CG_WND_G_3;
int CG_WND_G_4;
int CG_WND_G_5;
int CG_WND_G_6;
int CG_WND_G_7;
int CG_WND_G_8;
// ???????
int CG_WND2_G_0;
int CG_WND2_G_1;
int CG_WND2_G_2;
int CG_WND2_G_3;
int CG_WND2_G_4;
int CG_WND2_G_5;
int CG_WND2_G_6;
int CG_WND2_G_7;
int CG_WND2_G_8;
// ???????
int CG_WND3_G_7;
int CG_WND3_G_8;
int CG_WND3_G_9;
// ?????
int CG_BTL_PET_CHANGE_WND;
// ????????
int CG_PET_WND_VIEW;
int CG_PET_WND_DETAIL;
int CG_NAME_CHANGE_WND;
// ?????????
int CG_ITEM_WND_1;
int CG_JUJUTU_WND;
int CG_ITEM_WND_SELECT_WND;
int CG_STATUS_WND_GROUP_WND;
// ????????
int CG_MAP_WND;
// ??????????
int CG_STATUS_WND;
// ????????
int CG_MAIL_WND;
int CG_MAIL_WND_SEND_WND;
int CG_MAIL_WND_PET_SEND_WND;
int CG_MAIL_WND_HISTORY_WND;
// ?????????
int CG_ALBUM_WND;
// ????????????
int CG_CHAT_REGISTY_WND;
// ????????
int CG_COMMON_WIN_YORO;
int CG_FIELD_HELP_WND;
// ??????????
int CG_MSG_WND;
int CG_PET_WAZA_WND;
// ?????????????
int CG_ITEMSHOP_WIN;
// ??????????
int CG_SKILLSHOP_WIN;
// ??????????
int CG_ITEMSHOP_KOSU_WIN;
int CG_FAMILY_DETAIL_WIN;
int CG_FAMILY_BANK_WIN;
int CG_BM_WND;

void SetAnimTbl()
{
#ifdef _READ16BITBMP
	if(g_bUseAlpha){
		CG_PKSERVER_PANEL = OLD_GRAPHICS_START+37;
		CG_BATTTLE_SKILLCHOICE = OLD_GRAPHICS_START+53;
		CG_FIELD_SKILL_PANEL = OLD_GRAPHICS_START+50;
		CG_FIELD_CHATROOM_PANEL = OLD_GRAPHICS_START+54;
		CG_FIELD_SV_SELL_PANEL = OLD_GRAPHICS_START+55;
		CG_FIELD_SV_SELL_PRICE_PANEL = OLD_GRAPHICS_START+56;
		CG_NEW_STATUS_WND = OLD_GRAPHICS_START+51;
		CG_NEWITEM_WND = OLD_GRAPHICS_START+52;
		CG_TRADE_WND = OLD_GRAPHICS_START+48;
		CG_TRADE_VIEWWND = OLD_GRAPHICS_START+49;
		CG_WND_G_0 = OLD_GRAPHICS_START;
		CG_WND_G_1 = OLD_GRAPHICS_START+1;
		CG_WND_G_2 = OLD_GRAPHICS_START+2;
		CG_WND_G_3 = OLD_GRAPHICS_START+3;
		CG_WND_G_4 = OLD_GRAPHICS_START+4;
		CG_WND_G_5 = OLD_GRAPHICS_START+5;
		CG_WND_G_6 = OLD_GRAPHICS_START+6;
		CG_WND_G_7 = OLD_GRAPHICS_START+7;
		CG_WND_G_8 = OLD_GRAPHICS_START+8;
		CG_WND2_G_0 = OLD_GRAPHICS_START+9;
		CG_WND2_G_1 = OLD_GRAPHICS_START+10;
		CG_WND2_G_2 = OLD_GRAPHICS_START+11;
		CG_WND2_G_3 = OLD_GRAPHICS_START+12;
		CG_WND2_G_4 = OLD_GRAPHICS_START+13;
		CG_WND2_G_5 = OLD_GRAPHICS_START+14;
		CG_WND2_G_6 = OLD_GRAPHICS_START+15;
		CG_WND2_G_7 = OLD_GRAPHICS_START+16;
		CG_WND2_G_8 = OLD_GRAPHICS_START+17;
		CG_WND3_G_7 = OLD_GRAPHICS_START+18;
		CG_WND3_G_8 = OLD_GRAPHICS_START+19;
		CG_WND3_G_9 = OLD_GRAPHICS_START+20;
		CG_BTL_PET_CHANGE_WND = OLD_GRAPHICS_START+21;
		CG_PET_WND_VIEW = OLD_GRAPHICS_START+22;
		CG_PET_WND_DETAIL	= OLD_GRAPHICS_START+23;
		CG_NAME_CHANGE_WND = OLD_GRAPHICS_START+24;
		CG_ITEM_WND_1 = OLD_GRAPHICS_START+25;
		CG_JUJUTU_WND = OLD_GRAPHICS_START+26;
		CG_ITEM_WND_SELECT_WND = OLD_GRAPHICS_START+27;
		CG_STATUS_WND_GROUP_WND = OLD_GRAPHICS_START+28;
		CG_MAP_WND = OLD_GRAPHICS_START+29;
		CG_STATUS_WND = OLD_GRAPHICS_START+452;
		CG_MAIL_WND = OLD_GRAPHICS_START+30;
		CG_MAIL_WND_SEND_WND = OLD_GRAPHICS_START+38;
		CG_MAIL_WND_PET_SEND_WND = OLD_GRAPHICS_START+39;
		CG_MAIL_WND_HISTORY_WND = OLD_GRAPHICS_START+40;
		CG_ALBUM_WND = OLD_GRAPHICS_START+41;
		CG_CHAT_REGISTY_WND = OLD_GRAPHICS_START+42;
		CG_COMMON_WIN_YORO = OLD_GRAPHICS_START+31;
		CG_FIELD_HELP_WND = OLD_GRAPHICS_START+45;
		CG_MSG_WND = OLD_GRAPHICS_START+47;
		CG_PET_WAZA_WND = OLD_GRAPHICS_START+32;
		CG_ITEMSHOP_WIN = OLD_GRAPHICS_START+33;
		CG_SKILLSHOP_WIN = OLD_GRAPHICS_START+34;
		CG_ITEMSHOP_KOSU_WIN = OLD_GRAPHICS_START+35;
		CG_FAMILY_DETAIL_WIN = OLD_GRAPHICS_START+43;
		CG_FAMILY_BANK_WIN = OLD_GRAPHICS_START+44;
		CG_BM_WND = OLD_GRAPHICS_START+36;
	}
	else
#endif
	{
		CG_PKSERVER_PANEL = 26192;							// 选取星系人物框
		CG_BATTTLE_SKILLCHOICE = 26389;					// 战斗中选择技能
		CG_FIELD_SKILL_PANEL = 26352;						// 职业技能介面
		CG_FIELD_CHATROOM_PANEL = 26427;				// 聊天室介面
		CG_FIELD_SV_SELL_PANEL = 35221;					// 摆摊介面(卖方)
		CG_FIELD_SV_SELL_PRICE_PANEL = 35223;		// 输入售价视窗
		CG_NEW_STATUS_WND = 26386;
#ifdef _PET_ITEM
		CG_NEWITEM_WND = 26455;									// 人物装备栏位视窗(有标签)
#else
		CG_NEWITEM_WND = 26388;									// 人物装备栏位视窗(左手、脚、手套)
#endif

		CG_TRADE_WND = 26328;										// 交易主视窗

		//end modified by lsh
		CG_TRADE_VIEWWND = 26329;								// 交易检视视窗
		CG_WND_G_0 = 26001;
		CG_WND_G_1 = 26002;
		CG_WND_G_2 = 26003;
		CG_WND_G_3 = 26004;
		CG_WND_G_4 = 26005;
		CG_WND_G_5 = 26006;
		CG_WND_G_6 = 26007;
		CG_WND_G_7 = 26008;
		CG_WND_G_8 = 26009;
		CG_WND2_G_0 = 26021;
		CG_WND2_G_1 = 26022;
		CG_WND2_G_2 = 26023;
		CG_WND2_G_3 = 26024;
		CG_WND2_G_4 = 26025;
		CG_WND2_G_5 = 26026;
		CG_WND2_G_6 = 26027;
		CG_WND2_G_7 = 26028;
		CG_WND2_G_8 = 26029;
		CG_WND3_G_7 = 26037;
		CG_WND3_G_8 = 26038;
		CG_WND3_G_9 = 26039;
		CG_BTL_PET_CHANGE_WND = 26040;					// ????????????
		CG_PET_WND_VIEW = 26044;								// ???????????????
		CG_PET_WND_DETAIL = 26045;							// ??????????
		CG_NAME_CHANGE_WND = 26049;							// ??????
		CG_ITEM_WND_1 = 26061;									// ??????????
		CG_JUJUTU_WND = 26068;									// ?????
		CG_ITEM_WND_SELECT_WND = 26070;					// ?????
		CG_STATUS_WND_GROUP_WND = 26071;				// ????????????
		CG_MAP_WND = 26081;											// ????????
		CG_STATUS_WND = 26073;									// ???????????????
		CG_MAIL_WND = 26082;										// ????????
		CG_MAIL_WND_SEND_WND = 26200;						// ?????????
		CG_MAIL_WND_PET_SEND_WND = 26201;				// ?????????????
		CG_MAIL_WND_HISTORY_WND = 26203;				// ??????????
		CG_ALBUM_WND = 26230;										// ?????????
		CG_CHAT_REGISTY_WND = 26232;
		CG_COMMON_WIN_YORO = 26090;							// "???????"??????????
		CG_FIELD_HELP_WND = 26258;
		CG_MSG_WND = 26296;											// Message's Main Window ,added by LeiBoy
		CG_PET_WAZA_WND = 26130;
		CG_ITEMSHOP_WIN = 26138;
		CG_SKILLSHOP_WIN = 26139;
		CG_ITEMSHOP_KOSU_WIN = 26140;
		CG_FAMILY_DETAIL_WIN = 26239;
		CG_FAMILY_BANK_WIN = 26240;
		CG_BM_WND = 26141;
	}
}
#ifdef _FONT_SIZE
HFONT CreateNewFont(int size) {


#ifdef _NEWFONT_
		char strfame[128];
		extern int 编码;
		if(编码==950){
			sprintf(strfame,"Microsoft JhengHei");
		}else{
			sprintf(strfame,"Microsoft JhengHei");
		}
	 
		return CreateFont(size,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,1,
			0,0,0,17,(LPCTSTR)strfame);
#else
		char strfame[128];
		extern int 编码;
		if(编码==950){
			sprintf(strfame,"Microsoft JhengHei");
			return CreateFont(size,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,1,
			0,0,0,17,(LPCTSTR)strfame);
		}else{
			return CreateFont(size,0,0,0,400,FALSE,FALSE,FALSE,134,
			OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH|FF_ROMAN,(LPCTSTR)"宋体");
		}

#endif		
}
#endif