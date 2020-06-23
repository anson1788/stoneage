/************************/
/*	sprmgr.c			*/
/************************/
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/loadrealbin.h"
#include "../systeminc/loadsprbin.h"
LPDIRECTDRAWSURFACE lpBattleSurface;
#ifdef _READ16BITBMP
LPDIRECTDRAWSURFACE lpBattleSurfaceSys;
#endif
extern int MessageBoxNew(HWND hWnd,LPCSTR lpText,LPCSTR lpCaption,UINT uType);
#ifdef _STONDEBUG_
int SurfaceDispCnt;
#endif

int SurfaceSizeX;
int SurfaceSizeY;
UINT SurfaceDate = 3;

#define SURACE_INFO_MAX 3072
#define SURACE_BMP_DEATH_DATE 2
SPRITE_INFO SpriteInfo[ MAX_GRAPHICS ];
SURFACE_INFO SurfaceInfo[ SURACE_INFO_MAX ];
#ifdef _READ16BITBMP
SURFACE_INFO SurfaceInfoSys[SURACE_INFO_MAX];
extern BOOL g_bUseAlpha;
#endif
int SurfaceSearchPoint;

#ifdef _STONDEBUG_		
int SurfaceUseCnt;
extern int g_iMallocCount;
#endif

int SurfaceCnt;
int VramSurfaceCnt;
int SysramSurfaceCnt;
#ifdef _READ16BITBMP
void AllocateBmpToSurface( int bmpNo,BOOL useAlpha )
#else
void AllocateBmpToSurface( int bmpNo)
#endif
{
	int bmpWidth  = RealBinWidth;	// ????????
	int bmpHeight = RealBinHeight; 	// ???????
	
	int offsetX, offsetY; 			// ???????????????
	int sizeX, sizeY;				// ??????
	int surfaceCntX;				// ???????????
	int surfaceCntY;				// ???????????
	int totalSurface;				// ??????????
	int totalSurfaceCnt = 0;		// ??????????????? ??????? ?
	int	SurfaceSearchPointBak = SurfaceSearchPoint; // ??????????????????
	int amariSizeX = FALSE;		// ???????????
	int amariSizeY = FALSE;		// ??????????
	BOOL vramFullFlag = FALSE; 		// VRAM?????????
	SURFACE_INFO *prevSurfaceInfo; 	// ????????????????
	offsetX = 0; 
#ifdef _READ16BITBMP
	SURFACE_INFO *prevSurfaceInfoSys;
	RECT rect = {0,0,SurfaceSizeX,SurfaceSizeY};
	if(bmpNo >= OLD_GRAPHICS_START) offsetY = 0;
	else
#endif
	offsetY = bmpHeight;
	surfaceCntX = bmpWidth / SURFACE_WIDTH;
	if( (amariSizeX = bmpWidth % SURFACE_WIDTH)){
		surfaceCntX++;
	}
	
	surfaceCntY = bmpHeight / SURFACE_HEIGHT;
	if( ( amariSizeY = bmpHeight % SURFACE_HEIGHT ) ){
		surfaceCntY++;		// ??????
	}
	totalSurface  = surfaceCntX * surfaceCntY;
#ifdef _READ16BITBMP
	if(bmpNo < OLD_GRAPHICS_START)
#endif
	{
		if((RealBinWidth & 3)) RealBinWidth += 4 - RealBinWidth & 3;
	}
	while( 1 ){
		if( SurfaceInfo[ SurfaceSearchPoint ].date < SurfaceDate - SURACE_BMP_DEATH_DATE ){
#ifdef _STONDEBUG_		
			SurfaceUseCnt++;
#endif
			// 已在使用中
			if( SurfaceInfo[ SurfaceSearchPoint ].bmpNo != -1 ){
				SURFACE_INFO *lpSurfaceInfo;
#ifdef _READ16BITBMP
				SURFACE_INFO *lpSurfaceInfoSys;
#endif
				lpSurfaceInfo = SpriteInfo[ SurfaceInfo[ SurfaceSearchPoint ].bmpNo ].lpSurfaceInfo;
				SpriteInfo[ SurfaceInfo[ SurfaceSearchPoint ].bmpNo ].lpSurfaceInfo = NULL;
#ifdef _READ16BITBMP
				if(g_bUseAlpha){
					lpSurfaceInfoSys = SpriteInfo[ SurfaceInfo[ SurfaceSearchPoint ].bmpNo ].lpSurfaceInfoSys;
					SpriteInfo[SurfaceInfo[SurfaceSearchPoint].bmpNo].lpSurfaceInfoSys = NULL;
				}
#endif
				for( ; lpSurfaceInfo != NULL;
					lpSurfaceInfo = lpSurfaceInfo->pNext ){
					lpSurfaceInfo->bmpNo = -1;
#ifdef _READ16BITBMP
					if(g_bUseAlpha)	lpSurfaceInfoSys->bmpNo = -1;
#endif
#ifdef _STONDEBUG_		
					SurfaceUseCnt--;
#endif
				}
			}
			if( SpriteInfo[ bmpNo ].lpSurfaceInfo == NULL ){
				// 第一张图的surfaceinfo
				SpriteInfo[ bmpNo ].lpSurfaceInfo = &SurfaceInfo[ SurfaceSearchPoint ];
#ifdef _READ16BITBMP
				if(g_bUseAlpha)	SpriteInfo[bmpNo].lpSurfaceInfoSys = &SurfaceInfoSys[SurfaceSearchPoint];
#endif
			}else{
				// 指向下一张surfaceinfo
				prevSurfaceInfo->pNext = &SurfaceInfo[ SurfaceSearchPoint ];
#ifdef _READ16BITBMP
				if(g_bUseAlpha)	prevSurfaceInfoSys->pNext = &SurfaceInfoSys[SurfaceSearchPoint];
#endif
			}
			SurfaceInfo[ SurfaceSearchPoint ].bmpNo = bmpNo;
#ifdef _READ16BITBMP
			if(g_bUseAlpha) SurfaceInfoSys[SurfaceSearchPoint].bmpNo = bmpNo;
#endif
			if(ResoMode == 1){
				SurfaceInfo[ SurfaceSearchPoint ].offsetX = offsetX / 2;
#ifdef _READ16BITBMP
				if(g_bUseAlpha) SurfaceInfoSys[SurfaceSearchPoint].offsetX = offsetX / 2;
				if(bmpNo >= OLD_GRAPHICS_START){
					SurfaceInfo[SurfaceSearchPoint].offsetY = offsetY / 2;
					if(g_bUseAlpha) SurfaceInfoSys[SurfaceSearchPoint].offsetY = offsetY / 2;
				}
				else
#endif
				{
					SurfaceInfo[ SurfaceSearchPoint ].offsetY = ( bmpHeight - offsetY ) / 2;
#ifdef _READ16BITBMP
					if(g_bUseAlpha) SurfaceInfoSys[SurfaceSearchPoint].offsetY = ( bmpHeight - offsetY ) / 2;
#endif
				}
			}
			else{
				SurfaceInfo[ SurfaceSearchPoint ].offsetX = offsetX;
#ifdef _READ16BITBMP
				if(g_bUseAlpha) SurfaceInfoSys[SurfaceSearchPoint].offsetX = offsetX;
				if(bmpNo >= OLD_GRAPHICS_START){
					SurfaceInfo[SurfaceSearchPoint].offsetY = offsetY;
					if(g_bUseAlpha) SurfaceInfoSys[SurfaceSearchPoint].offsetY = offsetY;
				}
				else
#endif
				{
					SurfaceInfo[ SurfaceSearchPoint ].offsetY = bmpHeight - offsetY;
#ifdef _READ16BITBMP
					if(g_bUseAlpha)	SurfaceInfoSys[SurfaceSearchPoint].offsetY = bmpHeight - offsetY;
#endif
				}
			}

			if( offsetX >= bmpWidth - SURFACE_WIDTH && amariSizeX ){
				ClearSurface( SurfaceInfo[ SurfaceSearchPoint ].lpSurface );
#ifdef _READ16BITBMP
				if(g_bUseAlpha){
					ClearSurface(SurfaceInfoSys[SurfaceSearchPoint].lpSurface);
					if(SurfaceInfo[SurfaceSearchPoint].lpAlphaData)
						memset(SurfaceInfo[SurfaceSearchPoint].lpAlphaData,0,SurfaceSizeX*SurfaceSizeY);
				}
#endif
				sizeX = amariSizeX;				
			}
			else sizeX = SURFACE_WIDTH;

#ifdef _READ16BITBMP
			if(bmpNo >= OLD_GRAPHICS_START){
				if(offsetY >= bmpHeight - SURFACE_HEIGHT && amariSizeY){
					ClearSurface(SurfaceInfo[SurfaceSearchPoint].lpSurface);
					if(g_bUseAlpha){
						ClearSurface(SurfaceInfoSys[SurfaceSearchPoint].lpSurface);
						if(SurfaceInfo[SurfaceSearchPoint].lpAlphaData)
							memset(SurfaceInfo[SurfaceSearchPoint].lpAlphaData,0,SurfaceSizeX*SurfaceSizeY);
					}
					sizeY = amariSizeY;
				}else sizeY = SURFACE_HEIGHT;
			}
			else
#endif
			{
				if( offsetY - SURFACE_HEIGHT <= 0 && amariSizeY ){
					if( sizeX != amariSizeX ){
						ClearSurface( SurfaceInfo[ SurfaceSearchPoint ].lpSurface );
#ifdef _READ16BITBMP
						if(g_bUseAlpha) ClearSurface(SurfaceInfoSys[SurfaceSearchPoint].lpSurface);
#endif
					}
					sizeY = amariSizeY;
				}else sizeY = SURFACE_HEIGHT;
			}

#ifdef _READ16BITBMP
			SurfaceInfo[SurfaceSearchPoint].useAlpha = useAlpha;
			SurfaceInfoSys[SurfaceSearchPoint].useAlpha = useAlpha;
			if(bmpNo >= OLD_GRAPHICS_START){
				Draw16BitmapToSurface2(&SurfaceInfo[SurfaceSearchPoint],
									SurfaceInfoSys[SurfaceSearchPoint].lpSurface,
									offsetX, 
									offsetY,
									sizeX,
									sizeY,
									NULL);
			}
			else{
				if(g_bUseAlpha){
					DrawBitmapToSurface2(&SurfaceInfo[ SurfaceSearchPoint ],
									SurfaceInfoSys[SurfaceSearchPoint].lpSurface,
									offsetX, 
									offsetY - 1, 
									sizeX,
									sizeY,
									NULL );
				}
				else{
					DrawBitmapToSurface2(SurfaceInfo[ SurfaceSearchPoint ].lpSurface, 
									offsetX, 
									offsetY - 1, 
									sizeX,
									sizeY,
									NULL );
				}
			}
#else
			DrawBitmapToSurface2( 	&SurfaceInfo[ SurfaceSearchPoint ], 
									offsetX,
									offsetY - 1,
									sizeX,
									sizeY,
									NULL );
#endif
			totalSurfaceCnt++;
			// 所有的图都已存入offscreen
			if( totalSurfaceCnt >= totalSurface ){
				SurfaceInfo[ SurfaceSearchPoint ].pNext = NULL;
#ifdef _READ16BITBMP
				if(g_bUseAlpha)	SurfaceInfoSys[SurfaceSearchPoint].pNext = NULL;
#endif
				SurfaceSearchPoint++;

				if( SurfaceSearchPoint >= SurfaceCnt ) SurfaceSearchPoint = 0;
				break;
			}else{
				prevSurfaceInfo = &SurfaceInfo[ SurfaceSearchPoint ];
#ifdef _READ16BITBMP
				if(g_bUseAlpha)	prevSurfaceInfoSys = &SurfaceInfoSys[SurfaceSearchPoint];
#endif
				if( offsetX >= bmpWidth - SURFACE_WIDTH ){ 
					offsetX = 0;
#ifdef _READ16BITBMP
					if(bmpNo >= OLD_GRAPHICS_START) offsetY += SURFACE_HEIGHT;
					else
#endif
					offsetY -= SURFACE_HEIGHT;
				}else{ 
					offsetX += SURFACE_WIDTH;
				}
			}
		}
		SurfaceSearchPoint++;
		if( SurfaceSearchPoint >= SurfaceCnt ) SurfaceSearchPoint = 0;
		if( SurfaceSearchPoint == SurfaceSearchPointBak ) break;
	}
}

BOOL InitOffScreenSurface( void )
{
	int i;
	BOOL vramFullFlag = TRUE;
	SurfaceCnt = 0;
	VramSurfaceCnt = 0;
	SysramSurfaceCnt = 0;
	for( i = 0 ; i < SURACE_INFO_MAX ; i++ ){
		if( vramFullFlag == FALSE ){
			if ((SurfaceInfo[i].lpSurface = CreateSurface(SurfaceSizeX, SurfaceSizeY, DEF_COLORKEY, /*DDSCAPS_SYSTEMMEMORY*/ DDSCAPS_VIDEOMEMORY)) == NULL){
#ifdef _STONDEBUG_
				MessageBoxNew( hWnd ,"SurfaceInfo:建立VideoRAM Surface失败！" ,"确定",MB_OK | MB_ICONSTOP );
#endif
				vramFullFlag = TRUE;
			}else{
				VramSurfaceCnt++;
			}
		}
		if( vramFullFlag == TRUE ){
			if( ( SurfaceInfo[ i ].lpSurface = CreateSurface( SurfaceSizeX, SurfaceSizeY, DEF_COLORKEY, DDSCAPS_SYSTEMMEMORY )) == NULL ){
#ifdef _STONDEBUG_
				MessageBoxNew( hWnd ,"建立SysRAM Surface失败！" ,"确定",MB_OK | MB_ICONSTOP );
#endif
				return FALSE;
			}else SysramSurfaceCnt++;
		}
#ifdef _READ16BITBMP
		if(g_bUseAlpha){
			if((SurfaceInfo[i].lpAlphaData = (BYTE*)MALLOC(SurfaceSizeX*SurfaceSizeY)) == NULL){
	#ifdef _STONDEBUG_
				MessageBoxNew( hWnd ,"alpha记忆体配置失败！","确定",MB_OK | MB_ICONSTOP);
	#endif
				return FALSE;
			}
			else{
#ifdef _STONDEBUG_
				g_iMallocCount++;
#endif
				SysramSurfaceCnt++;
			}
		}
#endif
#ifdef _READ16BITBMP
		if(g_bUseAlpha){
			if((SurfaceInfoSys[i].lpSurface = CreateSurface(SurfaceSizeX,SurfaceSizeY,DEF_COLORKEY,DDSCAPS_SYSTEMMEMORY )) == NULL){
	#ifdef _STONDEBUG_
				MessageBoxNew(hWnd,"建立SysRAM Surface(2)失败！","确定",MB_OK | MB_ICONSTOP);
	#endif
				return FALSE;
			}
			else SysramSurfaceCnt++;
		}
#endif
	}
	SurfaceCnt = i;
	InitSurfaceInfo();
	InitSpriteInfo();
	return TRUE;
}

void InitSurfaceInfo( void )
{
	int i;
#ifdef _STONDEBUG_		
	SurfaceUseCnt = 0;
#endif
	SurfaceSearchPoint = 0;
	for( i = 0 ; i < SurfaceCnt ; i++ ){
		SurfaceInfo[ i ].bmpNo = -1;
		SurfaceInfo[ i ].date = 0;
		SurfaceInfo[ i ].pNext = NULL;
#ifdef _READ16BITBMP
		SurfaceInfo[i].useAlpha = FALSE;
		SurfaceInfoSys[i].bmpNo = -1;
		SurfaceInfoSys[i].date = 0;
		SurfaceInfoSys[i].pNext = NULL;
		SurfaceInfoSys[i].useAlpha = FALSE;
#endif
	}
}	

void InitSpriteInfo( void )
{
	int i;
	for( i = 0 ; i < MAX_GRAPHICS ; i++ ){
		SpriteInfo[ i ].lpSurfaceInfo = NULL;
#ifdef _READ16BITBMP
		SpriteInfo[i].lpSurfaceInfoSys = NULL;
#endif
	}
}	
extern int NewColor16Flg;
#define REALGETIMAGEMAXSIZE 1600*1600

extern int displayBpp;
BOOL LoadBmp( int bmpNo )
{
#ifndef _READ16BITBMP
	if((unsigned)bmpNo > MAX_GRAPHICS) return FALSE;
	if(SpriteInfo[bmpNo].lpSurfaceInfo == NULL){
		if( realGetImage(bmpNo,(unsigned char **)&pRealBinBits,
										 &RealBinWidth,&RealBinHeight ) == FALSE ){
			return FALSE;
		}
		SpriteInfo[ bmpNo ].width = RealBinWidth;
		SpriteInfo[ bmpNo ].height = RealBinHeight;
#ifdef _NEW_COLOR_
		if(NewColor16Flg==2)SpriteInfo[bmpNo].AlphaFlg=TRUE;
		else SpriteInfo[bmpNo].AlphaFlg=FALSE;
#endif
		AllocateBmpToSurface( bmpNo );
	}
#ifdef _CACHE_SURFACE_
	else{
		//如果已经缓存，刷新date，以免被释放
		for (SURFACE_INFO* info = SpriteInfo[bmpNo].lpSurfaceInfo; info->pNext != NULL; info = info->pNext)
		{
			info->date = SurfaceDate;
		}
	}
#endif
	return TRUE;
#else
	BOOL useAlpha;

	if((unsigned)bmpNo >= OLD_GRAPHICS_START){
		if((unsigned)bmpNo >= MAX_GRAPHICS) return FALSE;
		if(SpriteInfo[bmpNo].lpSurfaceInfo == NULL){
			pRealBinAlpha = NULL;
			if(Read16BMP(bmpNo - OLD_GRAPHICS_START,(unsigned char **)&pRealBinBits,
									 &RealBinWidth,&RealBinHeight,(unsigned char **)&pRealBinAlpha,&useAlpha) == FALSE) return FALSE;
			SpriteInfo[bmpNo].width = RealBinWidth;
			SpriteInfo[bmpNo].height = RealBinHeight;
			AllocateBmpToSurface(bmpNo,useAlpha);
		}
		return TRUE;
	}else{
		if(SpriteInfo[bmpNo].lpSurfaceInfo == NULL){
			pRealBinAlpha = NULL;
			if(realGetImage(bmpNo,(unsigned char **)&pRealBinBits,
											 &RealBinWidth,&RealBinHeight) == FALSE) return FALSE;
			
			if(NewColor16Flg==2){
				#define REALGETIMAGEMAXSIZE 1600*1600
				extern BYTE g_realgetimagebuf[REALGETIMAGEMAXSIZE];
				int i=0;
				int b = RealBinWidth*RealBinHeight;
				memset(g_realgetimagebuf,0,b);
				 pRealBinAlpha = g_realgetimagebuf;
				for(i;i<b;i++){
					*(pRealBinAlpha+i) = *(BYTE*)(pRealBinBits+3+i*4);
				}
			}
			SpriteInfo[bmpNo].width = RealBinWidth;
			SpriteInfo[bmpNo].height = RealBinHeight;
			AllocateBmpToSurface(bmpNo,FALSE);
		}
		return TRUE;
	}
#endif
}
