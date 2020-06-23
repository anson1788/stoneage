#ifndef _DIRECT_DRAW_H_
#define _DIRECT_DRAW_H_

#include "sprmgr.h"

extern const int MAX_PAL;
#define DEF_PAL	0	//?????
#define PAL_CHANGE_TIME			3600
#define SYSTEM_PAL_WHITE 		255
#define SYSTEM_PAL_AQUA 		254
#define SYSTEM_PAL_PURPLE 		253
#define SYSTEM_PAL_BLUE 		252
#define SYSTEM_PAL_YELLOW 		251
#define SYSTEM_PAL_GREEN 		250
#define SYSTEM_PAL_RED 			249
#define SYSTEM_PAL_GRAY 		248
#define SYSTEM_PAL_BLUE3 		247
#define SYSTEM_PAL_GREEN3 		246

#define SYSTEM_PAL_WHITE2 		7
#define SYSTEM_PAL_AQUA2 		6
#define SYSTEM_PAL_PURPLE2 		5
#define SYSTEM_PAL_BLUE2 		4
#define SYSTEM_PAL_YELLOW2 		3
#define SYSTEM_PAL_GREEN2 		2
#define SYSTEM_PAL_RED2 		1
#define SYSTEM_PAL_BLACK 		0
#define SYSTEM_PAL_BLUE4 		8
#define SYSTEM_PAL_GREEN4 		9

enum{
	FONT_PAL_WHITE,
	FONT_PAL_AQUA,
	FONT_PAL_PURPLE,
	FONT_PAL_BLUE,
	FONT_PAL_YELLOW,
	FONT_PAL_GREEN,
	FONT_PAL_RED,
	FONT_PAL_GRAY,
	FONT_PAL_BLUE2,
	FONT_PAL_GREEN2,
	FONT_PAL_10,
	FONT_PAL_11,
	FONT_PAL_12,
	FONT_PAL_13,
	FONT_PAL_14,
	FONT_PAL_15,
	FONT_PAL_16,
	FONT_PAL_17,
	FONT_PAL_18,
	FONT_PAL_19,
	FONT_PAL_20,
	FONT_PAL_21,
	FONT_PAL_22,
	FONT_PAL_23,
	FONT_PAL_24,
	FONT_PAL_25,
	FONT_PAL_NUM
};


enum{
	DRAW_BACK_NORMAL,
	DRAW_BACK_NON,	
	DRAW_BACK_PRODUCE,
	DRAW_BACK_BATTLE,
};
typedef struct
{
	LPDIRECTDRAW			lpDD;				// DirectDraw??????
	LPDIRECTDRAW2			lpDD2;				// DirectDraw2??????
	LPDIRECTDRAWSURFACE		lpFRONTBUFFER;		// 牙????
	LPDIRECTDRAWSURFACE		lpBACKBUFFER;		// ?????
#ifdef _READ16BITBMP
	LPDIRECTDRAWSURFACE		lpBACKBUFFERSYS;
#endif
	LPDIRECTDRAWCLIPPER		lpCLIPPER;			// ?????
	DDSURFACEDESC			ddsd;				// ??????????????﹨▍叉?
	LPDIRECTDRAWPALETTE		lpPALETTE;			// ????
	int						xSize, ySize;		// ?????????
} DIRECT_DRAW;
extern PALETTEENTRY			Palette[256];
typedef struct{
	int palNo;		// ????吻?
	int time;		// ?????
	int flag;		// ??????
}PALETTE_STATE;
extern DIRECT_DRAW	*lpDraw;
extern BOOL DDinitFlag;	
extern HFONT hFont;
extern PALETTE_STATE PalState;	
extern BOOL	PalChangeFlag;	
extern int BackBufferDrawType;
BOOL InitDirectDraw( void );
BOOL InitPalette( void );
void ClearBackSurface( void );
void ClearSurface( LPDIRECTDRAWSURFACE lpSurface );
LPBITMAPINFO LoadDirectDrawBitmap( char *pFile );
LPDIRECTDRAWSURFACE CreateSurface( short sizeX, short sizeY, DWORD ColorKey, unsigned int VramOrSysram );
#ifdef _READ16BITBMP
HRESULT DrawSurfaceFast( long bx, long by, LPDIRECTDRAWSURFACE lpSurface,LPDIRECTDRAWSURFACE lpSurfaceSys);
void DrawBitmapToSurface2(SURFACE_INFO *surface_info,LPDIRECTDRAWSURFACE lpSurfaceSys,int offsetX,int offsetY,int sizeX,int sizeY,LPBITMAPINFO pBmpInfo);
void Draw16BitmapToSurface2(SURFACE_INFO *surface_info,LPDIRECTDRAWSURFACE lpSurfaceSys,int offsetX,int offsetY,int sizeX,int sizeY,LPBITMAPINFO pBmpInfo);
#endif
HRESULT DrawSurfaceFast( short bx, short by, LPDIRECTDRAWSURFACE lpSurface );
void DrawBitmapToSurface2(SURFACE_INFO *surface_info, int offsetX, int offsetY, int sizeX, int sizeY, LPBITMAPINFO pBmpInfo );
HRESULT DrawSurfaceFast2( short bx, short by, RECT *rect, LPDIRECTDRAWSURFACE lpSurface );
void DrawBitmapToSurface( LPDIRECTDRAWSURFACE lpSurface, int offsetX, int offsetY, LPBITMAPINFO pInfo );
void Flip( void );
BOOL CheckSurfaceLost( void );
void ReleaseDirectDraw( void );
void DrawDebugLine( unsigned char color );
void DrawBox( RECT *rect, unsigned char color, BOOL fill );
void InitFont( int fontNo );
void PutText( char fontPrio ); 
void PaletteProc( void );
#ifdef _CACHE_SURFACE_
BOOL IsSurfaceExpired(SURFACE_INFO *surface);
void DrawSurfaceFromPalette(SURFACE_INFO* surface_info);
#endif
void PaletteChange( int palNo, int time );
void DrawAutoMapping( int x, int y, unsigned char *autoMap, int w, int h );
int getAutoMapColor( unsigned int GraphicNo );
int getNearestColorIndex( COLORREF color, PALETTEENTRY *palette, int entry );
void snapShot( void );
BOOL saveBmpFile( const char *, BYTE *,	int, int, int, int, int, RGBQUAD *, int );
BOOL saveBmpFile16( const char *, BYTE *,	int, int);
void DrawMapEffect( void );
#endif
