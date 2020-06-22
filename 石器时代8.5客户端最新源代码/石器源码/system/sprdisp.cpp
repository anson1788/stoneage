/************************/
/*	sprdisp.c			*/
/************************/
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/loadrealbin.h"
#include "../systeminc/loadsprbin.h"
#include "../systeminc/anim_tbl.h"
#include "../systeminc/map.h"
#include "../systeminc/battleMap.h"

#define STOCK_DISP_BUFFER_NO_BMP	( 1 << 31 ) 	// ???????
#define STOCK_DISP_BUFFER_LINE		( 1 << 30 ) 	// ???
#define STOCK_DISP_BUFFER_BOX		( 1 << 29 ) 	// ????
#define STOCK_DISP_BUFFER_BOX_FILL	( 1 << 28 )		// ????????
#define STOCK_DISP_BUFFER_CIRCLE	( 1 << 27 ) 	// ?
extern int displayBpp;
// ?????
DISP_BUFFER DispBuffer;

#ifndef __CARYTEST
// Realbin ???????????????
char *pRealBinBits;
// ?? Realbin ????????????
int RealBinWidth, RealBinHeight;
#endif
#ifdef _READ16BITBMP
BYTE *pRealBinAlpha;
#endif
// ????????????
int SurfaceBusyFlag = 0;

// ?????????
extern ACTION* pActMenuWnd2;
// ??????
extern unsigned int MenuToggleFlag;

// Robin 04/14 for Trade
extern ACTION* pActMenuWnd4;

// Bankman
extern ACTION* pActMenuWnd5;

extern unsigned short highColorPalette[256];
#ifdef _READ16BITBMP
extern AddressBin_s adrntruebuff[MAX_GRAPHICS_24];
extern BOOL g_bUseAlpha;
#endif

// ??????
int SortComp( DISP_SORT *pDisp1, DISP_SORT *pDisp2 );
typedef int CMPFUNC( const void *, const void * );
// ?????????????????
inline void GetBoxDispBuffer( DISP_INFO *pDispInfo, int bmpNo );
#ifdef _SURFACE_ANIM
void DrawAni(void);
int iProcessAniNum;
#endif
void DrawGrayA(LPDIRECTDRAWSURFACE lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne,int iGrayType);
#ifdef _READ16BITBMP
void DrawAlpha(LPDIRECTDRAWSURFACE lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,int Alpha,bool bLastOne);
extern int displayBpp;
void DrawStaturated(LPDIRECTDRAWSURFACE lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne);
#ifdef _SFUMATO
void DrawGray(LPDIRECTDRAWSURFACE lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne,int iGrayType, int sfumato);
#else
void DrawGray(LPDIRECTDRAWSURFACE lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne,int iGrayType);
#endif
void DrawAlphaChannel(SURFACE_INFO *surface_info,BYTE *AlphaData,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne);

#ifdef _SFUMATO
void DrawSfumato(LPDIRECTDRAWSURFACE lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne,int iGrayType, int sfumato);
#endif
#endif
void DrawStaturated(LPDIRECTDRAWSURFACE lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne);
void DrawGray(LPDIRECTDRAWSURFACE lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne,int iGrayType);
// ??????? ///////////////////////////////////////////////////////////
void SortDispBuffer( void )
{	
	//???????
	qsort( 	DispBuffer.DispSort,	// ??????
			DispBuffer.DispCnt,		// ?????
			sizeof( DISP_SORT ), 	// ?????
			( CMPFUNC * )SortComp 	// ?????????
		);				
}
DISP_SORT *pSortTileTail;


#ifdef __SKYISLAND

// ????? /////////////////////////////////////////////////////////////////
BOOL PutTileBmp( void )
{
	DISP_INFO 	*pDispInfo;
	int i;
	int bmpNo;
	RECT src;
	int sx, sy;
	BOOL retainbackbuffer = FALSE;
	pSortTileTail = DispBuffer.DispSort;
	SURFACE_INFO *lpSurfaceInfo;
#ifdef _READ16BITBMP
	SURFACE_INFO *lpSurfaceInfoSys;
#endif
	if( (ProcNo==PROC_GAME/* && SubProcNo==3*/) || ProcNo==PROC_BATTLE){
#ifdef _SURFACE_ANIM
		iProcessAniNum = 0;
#endif
		if( fastDrawTile && (fastDrawTileFlag==0 || amountXFastDraw || amountYFastDraw) ){
			retainbackbuffer = TRUE;
			// ?????????????????????????????
			if( ProcNo != PROC_BATTLE){
				src.top = 0;
				src.left = 0;
				src.right = DEF_APPSIZEX;
				src.bottom = DEF_APPSIZEY;
				sx = 0;
				sy = 0;
				//???????????????
				if( ResoMode == 1 ){
					//src.right /= 2;
					//src.bottom /= 2;
					src.right >>= 1;
					src.bottom >>= 1;
				}
				if( amountXFastDraw > 0 ){
					src.right -= amountXFastDraw;
					sx += amountXFastDraw;
				}else if( amountXFastDraw < 0 ){
					src.left -= amountXFastDraw;
				}
				if( amountYFastDraw > 0 ){
					src.bottom -= amountYFastDraw;
					sy += amountYFastDraw;
				}else if( amountYFastDraw < 0 ){
					src.top -= amountYFastDraw;
				}
				if( lpDraw->lpBACKBUFFER->BltFast( sx, sy, lpBattleSurface, &src, DDBLTFAST_WAIT ) == DDERR_SURFACEBUSY )
					SurfaceBusyFlag = TRUE;
#ifdef _READ16BITBMP
				if(g_bUseAlpha){
					if(lpDraw->lpBACKBUFFERSYS->BltFast(sx,sy,lpBattleSurfaceSys,&src,DDBLTFAST_WAIT) == DDERR_SURFACEBUSY)
						SurfaceBusyFlag = TRUE;
				}
#endif
			}
			// ???????????
			for( i = 0; i<DispBuffer.DispCnt ; i++, pSortTileTail++ ){
#ifdef _SURFACE_ANIM
				if(pSortTileTail->dispPrio == 0) continue;
#endif
				// 因为有排序过,所以如果目前的显示顺序比地表大的话,表示地表已经处理完了
				if( pSortTileTail->dispPrio > DISP_PRIO_TILE){
					DispBuffer.DispCnt -= i;
#ifdef _SURFACE_ANIM
					iProcessAniNum = i;
#endif
					break;
				}
				// ????????????????
				// 取得第一个要处理的图的 DispInfo 资料
				pDispInfo = DispBuffer.DispInfo + pSortTileTail->no;
				bmpNo = pDispInfo->bmpNo; // ????
				
				// ?????
				// ?????????
				if( pDispInfo->pAct != NULL ){
					if( pDispInfo->pAct->atr & ACT_ATR_HIDE2 ) continue;
				}
				// ?????????????????????????
				// ?????????
				if( LoadBmp( bmpNo ) == FALSE ) continue;
#ifdef _READ16BITBMP
				if(g_bUseAlpha){
					for(lpSurfaceInfo = SpriteInfo[ bmpNo ].lpSurfaceInfo,lpSurfaceInfoSys = SpriteInfo[bmpNo].lpSurfaceInfoSys
					;lpSurfaceInfo != NULL,lpSurfaceInfoSys != NULL
					;lpSurfaceInfo = lpSurfaceInfo->pNext,lpSurfaceInfoSys = lpSurfaceInfoSys->pNext){
						if(ResoMode == 1){
							if(DrawSurfaceFast((pDispInfo->x >> 1) + lpSurfaceInfo->offsetX,
								(pDispInfo->y >> 1) + lpSurfaceInfo->offsetY,
								lpSurfaceInfo->lpSurface,lpSurfaceInfoSys->lpSurface) == DDERR_SURFACEBUSY) SurfaceBusyFlag = TRUE;
						}
						else{
							if(DrawSurfaceFast(pDispInfo->x + lpSurfaceInfo->offsetX,
								pDispInfo->y + lpSurfaceInfo->offsetY,
								lpSurfaceInfo->lpSurface,lpSurfaceInfoSys->lpSurface) == DDERR_SURFACEBUSY) SurfaceBusyFlag = TRUE;
						}
						lpSurfaceInfo->date = SurfaceDate;
					}
				}
				else
#endif
				{
					for(lpSurfaceInfo = SpriteInfo[ bmpNo ].lpSurfaceInfo;lpSurfaceInfo != NULL;lpSurfaceInfo = lpSurfaceInfo->pNext ){
						if(ResoMode == 1){
							if(DrawSurfaceFast((pDispInfo->x >> 1) + lpSurfaceInfo->offsetX,
							(pDispInfo->y >> 1) + lpSurfaceInfo->offsetY,
							lpSurfaceInfo->lpSurface ) == DDERR_SURFACEBUSY) SurfaceBusyFlag = TRUE;
						}
						else{
#ifdef _CACHE_SURFACE_
							DrawSurfaceFromPalette(lpSurfaceInfo);
#endif
							if(DrawSurfaceFast(pDispInfo->x + lpSurfaceInfo->offsetX,
							pDispInfo->y + lpSurfaceInfo->offsetY,
							lpSurfaceInfo->lpSurface ) == DDERR_SURFACEBUSY) SurfaceBusyFlag = TRUE;
						}
						lpSurfaceInfo->date = SurfaceDate;
					}
				}
			}
		}
	}
	if(retainbackbuffer){
		if( lpBattleSurface->BltFast( 0, 0, lpDraw->lpBACKBUFFER, NULL, DDBLTFAST_WAIT ) == DDERR_SURFACEBUSY )	SurfaceBusyFlag = TRUE;
#ifdef _READ16BITBMP
		if(g_bUseAlpha) if(lpBattleSurfaceSys->BltFast(0,0,lpDraw->lpBACKBUFFERSYS,NULL,DDBLTFAST_WAIT) == DDERR_SURFACEBUSY)	SurfaceBusyFlag = TRUE;
#endif
	}
	if( ProcNo != PROC_BATTLE){
#ifdef _LOST_FOREST_FOG		
		if(nowFloor!=7450 && nowFloor!=7451
#ifdef _NEW_CLOUD
		   //&& nowFloor != 61000 && nowFloor != 61100
#endif
		){
#endif
			extern DWORD sky_island_no;
			if( sky_island_no){
#ifdef _NEW_CLOUD
				extern void SkyIslandDraw(int floor);
		 		SkyIslandDraw(nowFloor);
#else
				extern void SkyIslandDraw();
		 		SkyIslandDraw();
#endif
				return FALSE;
			}
#ifdef _LOST_FOREST_FOG
		}
#endif
	}
	return retainbackbuffer;
}
#endif
unsigned char AlphaBytes[64*48+1];
unsigned char AlphaColorBytes[64*48*4+1];

// ????? /////////////////////////////////////////////////////////////////
void PutBmp( void )
{
#ifdef __SKYISLAND
	// ???????????
	switch( BackBufferDrawType ){//背景类型
		case DRAW_BACK_NORMAL://无背景
			ClearBackSurface();	//清空背景显示
			void SkyIslandProc();
			SkyIslandProc();
			if( !PutTileBmp() ){
				if( fastDrawTile && lpDraw->lpBACKBUFFER->BltFast( 0, 0, lpBattleSurface, NULL, DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY ) == DDERR_SURFACEBUSY ){
					SurfaceBusyFlag = TRUE;
					return;
				}
			}
#ifdef _SURFACE_ANIM
			DrawAni();
#endif
			break;
		case DRAW_BACK_BATTLE:	//战斗背景
			amountXFastDraw = amountYFastDraw = 0;
			PutTileBmp();
			DrawBattleMap();
			break;
		default:
			if( !PutTileBmp() ){
				if( fastDrawTile && lpDraw->lpBACKBUFFER->BltFast( 0, 0, lpBattleSurface, NULL, DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY ) == DDERR_SURFACEBUSY ){
					SurfaceBusyFlag = TRUE;
					return;
				}
			}
			break;
	}
	DISP_SORT 	*pDispSort = pSortTileTail;
#else
	DISP_SORT 	*pDispSort = DispBuffer.DispSort;
#endif
	DISP_INFO 	*pDispInfo;
	int i;
	int bmpNo;
	UCHAR putTextFlag = 0;
	unsigned char drawMapEffectFlag = 0;
	unsigned char drawFastTileFlag = 0;
	unsigned char drawFastTileFlag2 = 0;
#ifndef __CARYTEST
	SURFACE_INFO *lpSurfaceInfo;
#ifdef _READ16BITBMP
	SURFACE_INFO *lpSurfaceInfoSys;
#endif
#else
	DDSURFACEDESC ddsd;	// ??????
	LPWORD surface;
	int pitch;
	BOOL locked = FALSE;
	int x, y;
	short width, height;
	ZeroMemory( &ddsd, sizeof( DDSURFACEDESC ) );
	ddsd.dwSize = sizeof( DDSURFACEDESC );
	if( lpDraw->lpBACKBUFFER->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) == DD_OK ){
		surface = (LPWORD)ddsd.lpSurface;
		pitch = ddsd.lPitch;
		locked = TRUE;
	}
#endif
	// ???????????
	for( i = 0; i < DispBuffer.DispCnt ; i++, pDispSort++ ){
		pDispInfo = &DispBuffer.DispInfo[pDispSort->no];
		bmpNo = pDispInfo->bmpNo;//秀图ID
		if( putTextFlag == 0 ){
			if( pDispSort->dispPrio >= DISP_PRIO_MENU ){
				PutText( FONT_PRIO_BACK );
#ifdef _CHANNEL_MODIFY
				PutText(FONT_PRIO_CHATBUFFER);
#endif
				putTextFlag = 1;	
			}
		}
		if( putTextFlag == 1 ){
			if( pDispSort->dispPrio >= DISP_PRIO_YES_NO_WND ){
				PutText( FONT_PRIO_FRONT );	
				putTextFlag = 2;	
			}
		}
#ifdef _TRADETALKWND				// Syu ADD 交易新增对话框架
		//增加一层新的文字显示顺序
		if( putTextFlag == 2 ){
			if( pDispSort->dispPrio >= DISP_PRIO_BOX3 ){
				PutText( FONT_PRIO_AFRONT );	
				putTextFlag = 2;	
			}
		}
#endif
		if( pDispSort->dispPrio > DISP_PRIO_MENU ){
			if( MenuToggleFlag & JOY_CTRL_M && pActMenuWnd2 != NULL ){
				if( pActMenuWnd2->hp > 0 ){
#ifdef _2005_ValentineDay
					// 不让玩家看到小地图
					if (nowFloor != 17006)
#endif
					drawAutoMap( pActMenuWnd2->x, pActMenuWnd2->y );
				}
			}
		}
		// ?????????					
		if( pDispSort->dispPrio >= DISP_PRIO_RESERVE ){
#ifdef __SKYISLAND
#ifdef _NEW_CLOUD
			extern void SkyIslandDraw2(int fl);
			SkyIslandDraw2(nowFloor);
#else
			extern void SkyIslandDraw2();
			SkyIslandDraw2();
#endif
#endif
			drawMapEffect();
			drawMapEffectFlag = 1;
		}
#ifndef __SKYISLAND
		// ??????
		if( fastDrawTile ){
			RECT src;
			int sx, sy;
			if( pDispSort->dispPrio >= DISP_PRIO_TILE && drawFastTileFlag2 == 0 ){
				if( ProcNo == PROC_GAME && SubProcNo == 3 && fastDrawTileFlag ){
					// ?????????????????????????????
					src.top = 0;
					src.left = 0;
					src.right = DEF_APPSIZEX;
					src.bottom = DEF_APPSIZEY;
					sx = 0;
					sy = 0;
					//???????????????
					if( ResoMode == 1 ){
						src.right >>= 1;
						src.bottom >>= 1;
					}
					if( amountXFastDraw > 0 ){
						src.right -= amountXFastDraw;
						sx += amountXFastDraw;
					}
					else if( amountXFastDraw < 0 ) src.left -= amountXFastDraw;

					if( amountYFastDraw > 0 ){
						src.bottom -= amountYFastDraw;
						sy += amountYFastDraw;
					}
					else if( amountYFastDraw < 0 ) src.top -= amountYFastDraw;

					if( lpDraw->lpBACKBUFFER->BltFast( sx, sy, lpBattleSurface, &src, DDBLTFAST_WAIT ) == DDERR_SURFACEBUSY )
						SurfaceBusyFlag = TRUE;
					drawFastTileFlag2 = 1;
				}
			}
			if( pDispSort->dispPrio > DISP_PRIO_TILE && drawFastTileFlag == 0 ){
				if( ProcNo == PROC_GAME && SubProcNo == 3 && fastDrawTileFlag ){
					// ????????????????????????????
					if( lpBattleSurface->BltFast( 0, 0, lpDraw->lpBACKBUFFER, NULL, DDBLTFAST_WAIT ) == DDERR_SURFACEBUSY )
						SurfaceBusyFlag = TRUE;
					drawFastTileFlag = 1;
				}
			}
		}
#endif
		// ???????
		if( bmpNo & STOCK_DISP_BUFFER_NO_BMP ){
			if( bmpNo & ( STOCK_DISP_BUFFER_BOX | STOCK_DISP_BUFFER_BOX_FILL | STOCK_DISP_BUFFER_LINE ) ){
				GetBoxDispBuffer( pDispInfo, bmpNo );
			}
			continue;
		}
		if( pDispInfo->pAct != NULL ){
			if( pDispInfo->pAct->atr & ACT_ATR_HIDE2 ) continue;
		}
		if( LoadBmp( bmpNo ) == FALSE ) continue;
		{
			for(lpSurfaceInfo = SpriteInfo[ bmpNo ].lpSurfaceInfo;lpSurfaceInfo != NULL;lpSurfaceInfo = lpSurfaceInfo->pNext){
#ifdef _CACHE_SURFACE_
				DrawSurfaceFromPalette(lpSurfaceInfo);
#endif
				if(pDispInfo->DrawEffect == 2)//饱和处理
					DrawStaturated(lpSurfaceInfo->lpSurface,
						pDispInfo->x + lpSurfaceInfo->offsetX,pDispInfo->y + lpSurfaceInfo->offsetY,
						lpSurfaceInfo->offsetX,lpSurfaceInfo->offsetY,
						SpriteInfo[bmpNo].width,SpriteInfo[bmpNo].height,(lpSurfaceInfo->pNext == NULL ? true:false));
				else if(pDispInfo->DrawEffect == 3 || pDispInfo->DrawEffect == 4)//石化和中毒处理
					DrawGray(lpSurfaceInfo->lpSurface,
						pDispInfo->x + lpSurfaceInfo->offsetX,pDispInfo->y + lpSurfaceInfo->offsetY,
						lpSurfaceInfo->offsetX,lpSurfaceInfo->offsetY,
						SpriteInfo[bmpNo].width,SpriteInfo[bmpNo].height,(lpSurfaceInfo->pNext == NULL ? true:false),pDispInfo->DrawEffect);
#ifdef _PETCOLOR_
				else if(pDispInfo->DrawEffect==5){
					DrawGray(lpSurfaceInfo->lpSurface,
					pDispInfo->x + lpSurfaceInfo->offsetX,pDispInfo->y + lpSurfaceInfo->offsetY,
					lpSurfaceInfo->offsetX,lpSurfaceInfo->offsetY,
					SpriteInfo[bmpNo].width,SpriteInfo[bmpNo].height,(lpSurfaceInfo->pNext == NULL ? true:false),3);
				}
#endif
#ifdef _NEW_COLOR_
				else{
					extern int NewColor16Flg;
					if(SpriteInfo[ bmpNo ].AlphaFlg){
						DDSURFACEDESC ddsd;	
						DDSURFACEDESC ddsd2;
						short bx, by;
						short x0, y0;
						long w, h;
						int  Dstpitch, Srcpitch;
						RECT rect = { 0, 0, SurfaceSizeX, SurfaceSizeY };
						bx = x0 = pDispInfo->x + lpSurfaceInfo->offsetX;
						by = y0 = pDispInfo->y + lpSurfaceInfo->offsetY;
						w = rect.right - rect.left;
						h = rect.bottom - rect.top;
						if (bx >= lpDraw->xSize || bx + w <= 0 || by >= lpDraw->ySize || by + h <= 0)	continue;
						if (bx < 0){
							rect.left -= bx;
							x0 = 0;
						}
						if (bx + w > lpDraw->xSize)	rect.right -= bx + w - lpDraw->xSize;
						if (by < 0){
							rect.top -= by;
							y0 = 0;
						}
						if (by + h > lpDraw->ySize)	rect.bottom -= by + h - lpDraw->ySize;
						ZeroMemory(&ddsd, sizeof(DDSURFACEDESC));
						ddsd.dwSize = sizeof(DDSURFACEDESC);
						ZeroMemory(&ddsd2, sizeof(DDSURFACEDESC));
						ddsd2.dwSize = sizeof(DDSURFACEDESC);
						if (lpDraw->lpBACKBUFFER->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK)	continue;
						if (lpSurfaceInfo->lpSurface->Lock(NULL, &ddsd2, DDLOCK_WAIT, NULL) != DD_OK)	continue;
						unsigned char *Dst, *Src;
						Dst = (unsigned char *)ddsd.lpSurface;
						Dstpitch = ddsd.lPitch;
						Src = (unsigned char *)ddsd2.lpSurface;
						Srcpitch = ddsd2.lPitch;
						if(displayBpp==16){
							ablend_565(lpSurfaceInfo->lpAlphaData, 64, Src, rect.left, rect.top, Srcpitch, Dst, x0 , y0 , rect.right-rect.left, rect.bottom-rect.top, Dstpitch);
						}else{
							DrawAlpha32((unsigned long *)Dst, x0, y0, Dstpitch, (unsigned long *)Src, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, Srcpitch);
						}
						if (lpSurfaceInfo->lpSurface->Unlock(NULL) != DD_OK)	continue;
						if (lpDraw->lpBACKBUFFER->Unlock(NULL) != DD_OK)	continue;
					}else
#endif
					{
						if( DrawSurfaceFast( 	pDispInfo->x + lpSurfaceInfo->offsetX, 
										pDispInfo->y + lpSurfaceInfo->offsetY, 
										lpSurfaceInfo->lpSurface ) == DDERR_SURFACEBUSY ) SurfaceBusyFlag = TRUE;
					}
					lpSurfaceInfo->date = SurfaceDate;
				}
			}
		}
	}
	/*
#ifdef _LOST_FOREST_FOG
	if(nowFloor==7450||nowFloor==7451
#ifdef _NEW_CLOUD
		//|| nowFloor==61000 || nowFloor==61100
#endif
		){
		extern DWORD sky_island_no;
		if( sky_island_no){
#ifdef _NEW_CLOUD
			extern void SkyIslandDraw(int floor);
	 		SkyIslandDraw(nowFloor);
#else
			extern void SkyIslandDraw();
	 		SkyIslandDraw();
#endif
		}
	}
#endif
	*/
	
#ifdef __SKYISLAND		
#ifdef _NEW_CLOUD
	extern void SkyIslandDraw2(int fl);
	SkyIslandDraw2(nowFloor);	
#else
	extern void SkyIslandDraw2();
	SkyIslandDraw2();	
#endif
#endif
	if( putTextFlag == 0 ){
		// ?????????????????
		PutText( FONT_PRIO_BACK );	
		// ?????
#ifdef _CHANNEL_MODIFY
		PutText(FONT_PRIO_CHATBUFFER);
#endif
		putTextFlag = 1;	
	}
	// ?????( FRONT )
	if( putTextFlag == 1 ){
		PutText( FONT_PRIO_FRONT );	
	}
	// ?????????
	if( drawMapEffectFlag == 0 ) drawMapEffect();

	memset(&DispBuffer,0,sizeof(DispBuffer));


}

//**************************************************************************/
// 	??：	??????????????
// 	??：	UCHAR dispPrio：????????
//		  	int x, int y：坐标
//			int bmpNo：图片号
//			int chr_no：???????
//			int pat_no：?????
//**************************************************************************/
// 储存所有要播放的Image
int StockDispBuffer( int x, int y, UCHAR dispPrio, int bmpNo, BOOL hitFlag )
{	
	short dx,dy;
	int BmpNo;

	DISP_SORT 	*pDispSort = DispBuffer.DispSort + DispBuffer.DispCnt;
	DISP_INFO 	*pDispInfo = DispBuffer.DispInfo + DispBuffer.DispCnt;

	if( DispBuffer.DispCnt >= DISP_BUFFER_SIZE ) return -2;

	if( -1 <= bmpNo && bmpNo <= CG_INVISIBLE ) return -2;

	if( bmpNo > CG_INVISIBLE ){
		realGetNo( bmpNo , (U4 *)&BmpNo );
		realGetPos( BmpNo  , &dx, &dy);
	}else{
		dx = 0;
		dy = 0;
		BmpNo = bmpNo;
	}

	pDispSort->dispPrio = dispPrio;
	pDispSort->no = DispBuffer.DispCnt;
	pDispInfo->hitFlag = hitFlag;
	if(hitFlag >= 20 && hitFlag < 30){
		pDispInfo->DrawEffect = 2;
		pDispInfo->hitFlag = hitFlag - 20;
	}else if(hitFlag >= 30 && hitFlag < 40){
		pDispInfo->DrawEffect = 3;
		pDispInfo->hitFlag = hitFlag - 30;
	}
	else if(hitFlag >= 40 && hitFlag < 50){
		pDispInfo->DrawEffect = 4;
		pDispInfo->hitFlag = hitFlag - 40;
	}
	else pDispInfo->DrawEffect = 0;

	pDispInfo->x = x + dx;
	pDispInfo->y = y + dy;
	pDispInfo->bmpNo = BmpNo;
	pDispInfo->pAct = NULL;
	return DispBuffer.DispCnt++;
}

#ifdef _SFUMATO
int StockDispBuffer2( int x, int y, UCHAR dispPrio, int bmpNo, BOOL hitFlag, int sfumato/* = 0*/)
#else
int StockDispBuffer2( int x, int y, UCHAR dispPrio, int bmpNo, BOOL hitFlag )
#endif
{	
	short dx,dy;
	int BmpNo;
	// ???????????????
	DISP_SORT 	*pDispSort = DispBuffer.DispSort + DispBuffer.DispCnt;
	DISP_INFO 	*pDispInfo = DispBuffer.DispInfo + DispBuffer.DispCnt;
	// ??????????
	if( DispBuffer.DispCnt >= DISP_BUFFER_SIZE ) return -2;
	// ??????????????
	if( -1 <= bmpNo && bmpNo <= CG_INVISIBLE ) return -2;
	// ???
	if( bmpNo > CG_INVISIBLE ){
		BmpNo = bmpNo;
		realGetPos( BmpNo  , &dx, &dy);
	}else{// ????????????
		dx = 0;
		dy = 0;
		BmpNo = bmpNo;
	}
	// ??????
	pDispSort->dispPrio = dispPrio;
	pDispSort->no = DispBuffer.DispCnt;
	// ⿳?? ??????? ?
	pDispInfo->x = x + dx;
	pDispInfo->y = y + dy;
	pDispInfo->bmpNo = BmpNo;
	pDispInfo->hitFlag = hitFlag;
	pDispInfo->pAct = NULL;
#ifdef _READ16BITBMP
	if(pDispInfo->bmpNo >= OLD_GRAPHICS_START && pDispInfo->bmpNo < MAX_GRAPHICS){
		if(adrntruebuff[pDispInfo->bmpNo - OLD_GRAPHICS_START].staturated == 1) pDispInfo->DrawEffect = 2;
	}
#endif
#ifdef _SFUMATO
	pDispInfo->sfumato = sfumato;
#endif
	return DispBuffer.DispCnt++;
}


// 储存所有要播放的Image，依Act的状况来设定
void StockTaskDispBuffer( void )
{
	ACTION *pActLoop; 	/* ???????? */
	DISP_SORT 	*pDispSort = DispBuffer.DispSort + DispBuffer.DispCnt;
	DISP_INFO 	*pDispInfo = DispBuffer.DispInfo + DispBuffer.DispCnt;
	
	/* ?????????? */	
	//while( pActLoop != pActBtm ){
	for(pActLoop = pActTop->pNext ; 
		pActLoop != pActBtm ;
		/* ?????????? */
		pActLoop = pActLoop->pNext ){
		
		// ??????????
		if( DispBuffer.DispCnt >= DISP_BUFFER_SIZE ) break;
		
		// ??????????????
		if( -1 <= pActLoop ->bmpNo && pActLoop ->bmpNo <= CG_INVISIBLE ) continue;
		
		// ????????????
		if( pActLoop ->deathFlag == TRUE ) continue;
		
		/* ???? */
		if( pActLoop->atr & ACT_ATR_HIDE ) continue;

		// ??????
		pDispSort->dispPrio = pActLoop->dispPrio;
		pDispSort->no = DispBuffer.DispCnt;
		// ⿳?? ??????? ?
		pDispInfo->x = pActLoop->x + pActLoop->anim_x;
		pDispInfo->y = pActLoop->y + pActLoop->anim_y;
		pDispInfo->bmpNo = pActLoop->bmpNo;
		pDispInfo->pAct = pActLoop;
		
		// ???????
		// ???


		if( pActLoop->atr & ACT_ATR_HIT_BOX_COL1 ) pDispInfo->hitFlag = 7;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_COL2 ) pDispInfo->hitFlag = 8;
		else
	    if( pActLoop->atr & ACT_ATR_HIT_BOX_COL3 ) pDispInfo->hitFlag = 9;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_COL4 ) pDispInfo->hitFlag = 10;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_COL5 ) pDispInfo->hitFlag = 11;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_COL6 ) pDispInfo->hitFlag = 12;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_COL7 ) pDispInfo->hitFlag = 13;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_COL8 ) pDispInfo->hitFlag = 14;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_ALL4 ) pDispInfo->hitFlag = 6;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_ALL3 ) pDispInfo->hitFlag = 5;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_ALL2 ) pDispInfo->hitFlag = 4;
		else 
		if( pActLoop->atr & ACT_ATR_HIT_BOX_ALL1 ) pDispInfo->hitFlag = 3;
		else // ????
		if( pActLoop->atr & ACT_ATR_HIT_BOX ) pDispInfo->hitFlag = 2;
		else // ??
		if( pActLoop->atr & ACT_ATR_HIT ) pDispInfo->hitFlag = 1;
		else pDispInfo->hitFlag = FALSE;
		// 4 石化
		if(pActLoop->status == 4)	pDispInfo->DrawEffect = 3;
		// 1 毒
		else if(pActLoop->status == 1) pDispInfo->DrawEffect = 4;
		else pDispInfo->DrawEffect = 0;
#ifdef _PETCOLOR_
		extern BOOL EncountFlag,DuelFlag,NoHelpFlag;
		if(!pDispInfo->DrawEffect)
			if(ProcNo==PROC_BATTLE && !EncountFlag && !vsLookFlag&&!eventEnemyFlag&&!DuelFlag&&!NoHelpFlag){
				if(pActLoop->actNo == 0){
					if (pActLoop->level == 1 && (ATR_PLACE_NO(pActLoop) >= 2 * MAX_BATTLE_ROW_CHARS  && ATR_PLACE_NO(pActLoop)< 4 * MAX_BATTLE_ROW_CHARS)){
						pDispInfo->DrawEffect=5;
					}
				}
			}
#endif
#ifdef _READ16BITBMP
		if(pDispInfo->bmpNo >= OLD_GRAPHICS_START && pDispInfo->bmpNo < MAX_GRAPHICS){
			if(adrntruebuff[pDispInfo->bmpNo - OLD_GRAPHICS_START].staturated == 1) pDispInfo->DrawEffect = 2;
		}
#endif
		// ?????????????
		pActLoop->hitDispNo = DispBuffer.DispCnt;
#ifdef _SFUMATO
		pDispInfo->sfumato = pActLoop->sfumato;
#endif
		// ????????
		DispBuffer.DispCnt++;

		// ???????
		pDispSort++;
		pDispInfo++;
	}
	
}

// ??????????????? ///////////////////////////////////////////////////
// ???prio1??prio2???????????
void StockTaskDispBuffer2( int prio1, int prio2 )
{
	ACTION *pActLoop; 	/* ???????? */
	DISP_SORT 	*pDispSort = DispBuffer.DispSort + DispBuffer.DispCnt;
	DISP_INFO 	*pDispInfo = DispBuffer.DispInfo + DispBuffer.DispCnt;
	int tmp;
	
	if( prio1 > prio2 )
	{
		tmp = prio1;
		prio1 = prio2;
		prio2 = tmp;
	}
	
	/* ?????????? */	
	//while( pActLoop != pActBtm ){
	for(pActLoop = pActTop->pNext ; 
		pActLoop != pActBtm ;
		/* ?????????? */
		pActLoop = pActLoop->pNext ){
		
		// ??????????
		if( DispBuffer.DispCnt >= DISP_BUFFER_SIZE ) break;
		
		// prio1??prio2????????
		if( prio1 <= pActLoop->dispPrio && pActLoop->dispPrio <= prio2 )
			continue;
		
		// ??????????????
		if( -1 <= pActLoop ->bmpNo && pActLoop ->bmpNo <= CG_INVISIBLE ) continue;
		
		// ????????????
		if( pActLoop ->deathFlag == TRUE ) continue;
		
		/* ???? */
		if( pActLoop->atr & ACT_ATR_HIDE ) continue;

		// ??????
		pDispSort->dispPrio = pActLoop->dispPrio;
		pDispSort->no = DispBuffer.DispCnt;
		// ⿳?? ??????? ?
		pDispInfo->x = pActLoop->x + pActLoop->anim_x;
		pDispInfo->y = pActLoop->y + pActLoop->anim_y;
		pDispInfo->bmpNo = pActLoop->bmpNo;
		pDispInfo->pAct = pActLoop;
		
		// ???????
		if( pActLoop->atr & ACT_ATR_HIT_BOX ) pDispInfo->hitFlag = 2;
		else 
		if( pActLoop->atr & ACT_ATR_HIT ) pDispInfo->hitFlag = 1;
		else pDispInfo->hitFlag = FALSE;
		// ?????????????
		pActLoop->hitDispNo = DispBuffer.DispCnt;

		// ????????
		DispBuffer.DispCnt++;
		// ???????
		pDispSort++;
		pDispInfo++;
	}
	
}

// ???????????????? ***************************************/
void StockBoxDispBuffer( int x1, int y1, int x2, int y2, UCHAR dispPrio, int color, BOOL fill )
{
	int col;	// ?
	// int ? short ??????
	int x = ( x1 << 16 ) | x2; 	// ????????????????
	int y = ( y1 << 16 ) | y2;	// ????????????????
	
	// ???????
	if( fill == 0 ) col = color | STOCK_DISP_BUFFER_BOX | STOCK_DISP_BUFFER_NO_BMP ;
	// ???????
	else if( fill == 1 ) col = color | STOCK_DISP_BUFFER_BOX_FILL | STOCK_DISP_BUFFER_NO_BMP ;
	// ?????
	else if( fill == 2 ) col = color | STOCK_DISP_BUFFER_LINE | STOCK_DISP_BUFFER_NO_BMP ;
	
	// ?????????????
	StockDispBuffer( x, y, dispPrio, col, 0 );
}

// ????????????????? *********************************/
inline void GetBoxDispBuffer( DISP_INFO *pDispInfo, int bmpNo )
{
	// ??????
	RECT rect;
	int color; // 
	BOOL fill = FALSE; // ???????
	int DispInfoX = pDispInfo->x;
	int DispInfoY = pDispInfo->y;
	
	// ????
	rect.right =	( LONG )( pDispInfo->x & 0xffff );
	rect.bottom =	( LONG )( pDispInfo->y & 0xffff );

	// ??????????
	_asm{
		// rect.left ???
		mov		cl,16				// ??????????
		mov		eax, [ DispInfoX ]
		sar		eax, cl				// ??????
		mov		[ rect.left ], eax
		// rect.top ???
		mov		eax, [ DispInfoY ]
		sar		eax, cl
		mov		[ rect.top ], eax
	}

	
	// ???????
	if( bmpNo & STOCK_DISP_BUFFER_BOX ){ 
		color = bmpNo & 0xff; 			// ??????
		DrawBox( &rect, color, 0 );	// ?????
		
	// ???????
	}else if( bmpNo & STOCK_DISP_BUFFER_BOX_FILL ){ 
		color = color = bmpNo & 0xff;	// 
		DrawBox( &rect, color, 1 );	// ?????
		
	}else if( bmpNo & STOCK_DISP_BUFFER_LINE ){ 
		color = color = bmpNo & 0xff;	// 
		DrawBox( &rect, color, 2 );	// ????
	}
}			

// ?????? //////////////////////////////////////////////////////////////
int SortComp( DISP_SORT *pDisp1, DISP_SORT *pDisp2 )
{
	// pDisp1 ??????????
	if( pDisp1->dispPrio > pDisp2->dispPrio ){
		return 1;
	}
	
	// pDisp2 ????????????
	if( pDisp1->dispPrio < pDisp2->dispPrio ){
		return -1;
	}
	// ?????????
	// pDisp1 ??????????????????
	if( pDisp1->no > pDisp2->no ){
		return -1;
	}
	// ????????????????
	return 1;
}

#ifdef _SURFACE_ANIM
// 画动态地表(优先权为0的动态地表)
void DrawAni()
{
	DISP_INFO 	*pDispInfo;
	int bmpNo;
	SURFACE_INFO *lpSurfaceInfo;
#ifdef _READ16BITBMP
	SURFACE_INFO *lpSurfaceInfoSys;
#endif

	if(ProcNo != PROC_BATTLE){
		pSortTileTail = DispBuffer.DispSort;
		DispBuffer.DispCnt += iProcessAniNum;
		for(int i = 0; i<DispBuffer.DispCnt; i++, pSortTileTail++ ){
			if( pSortTileTail->dispPrio == DISP_PRIO_TILE) continue;
			// 因为有排序过,所以如果目前的显示顺序比地表大的话,表示地表已经处理完了
			if( pSortTileTail->dispPrio > DISP_PRIO_TILE){
				DispBuffer.DispCnt -= i;
				break;
			}
			// 取得第一个要处理的图的 DispInfo 资料
			pDispInfo = DispBuffer.DispInfo + pSortTileTail->no;
			bmpNo = pDispInfo->bmpNo; // ????
			
			if( pDispInfo->pAct != NULL ){
				if( pDispInfo->pAct->atr & ACT_ATR_HIDE2 ) continue;
			}
			if( LoadBmp( bmpNo ) == FALSE ) continue;
#ifdef _READ16BITBMP
			if(g_bUseAlpha){
				for(lpSurfaceInfo = SpriteInfo[ bmpNo ].lpSurfaceInfo,lpSurfaceInfoSys = SpriteInfo[ bmpNo ].lpSurfaceInfoSys
				;lpSurfaceInfo != NULL,lpSurfaceInfoSys != NULL
				;lpSurfaceInfo = lpSurfaceInfo->pNext,lpSurfaceInfoSys = lpSurfaceInfoSys->pNext){
					if(ResoMode == 1){
						if(DrawSurfaceFast((pDispInfo->x >> 1) + lpSurfaceInfo->offsetX,(pDispInfo->y >> 1) + lpSurfaceInfo->offsetY,
							lpSurfaceInfo->lpSurface,lpSurfaceInfoSys->lpSurface) == DDERR_SURFACEBUSY) SurfaceBusyFlag = TRUE;
					}
					else{
						if(DrawSurfaceFast(pDispInfo->x + lpSurfaceInfo->offsetX,
						pDispInfo->y + lpSurfaceInfo->offsetY,
						lpSurfaceInfo->lpSurface,lpSurfaceInfoSys->lpSurface) == DDERR_SURFACEBUSY) SurfaceBusyFlag = TRUE;
					}
					lpSurfaceInfo->date = SurfaceDate;
				}
			}
			else
#endif
			{
				for(lpSurfaceInfo = SpriteInfo[ bmpNo ].lpSurfaceInfo;lpSurfaceInfo != NULL;lpSurfaceInfo = lpSurfaceInfo->pNext ){
					if(ResoMode == 1){
						if(DrawSurfaceFast((pDispInfo->x >> 1) + lpSurfaceInfo->offsetX,
						(pDispInfo->y >> 1) + lpSurfaceInfo->offsetY,
						lpSurfaceInfo->lpSurface ) == DDERR_SURFACEBUSY) SurfaceBusyFlag = TRUE;
					}
					else{
#ifdef _CACHE_SURFACE_
						DrawSurfaceFromPalette(lpSurfaceInfo);
#endif
						if(DrawSurfaceFast(pDispInfo->x + lpSurfaceInfo->offsetX,
						pDispInfo->y + lpSurfaceInfo->offsetY,
						lpSurfaceInfo->lpSurface ) == DDERR_SURFACEBUSY) SurfaceBusyFlag = TRUE;
					}
					lpSurfaceInfo->date = SurfaceDate;
				}
			}
		}
		if(fastDrawTile && lpDraw->lpBACKBUFFER->BltFast( 0, 0, lpBattleSurface, NULL, DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY ) == DDERR_SURFACEBUSY){
			SurfaceBusyFlag = TRUE;
			return;
		}

#ifdef _READ16BITBMP
		if(g_bUseAlpha){
			if(fastDrawTile && lpDraw->lpBACKBUFFERSYS->BltFast( 0, 0, lpBattleSurfaceSys, NULL, DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY ) == DDERR_SURFACEBUSY){
				SurfaceBusyFlag = TRUE;
				return;
			}
		}
#endif
	}
}
#endif
extern int gBitRShift;

void DrawGray(LPDIRECTDRAWSURFACE lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne,int iGrayType){
	static DDSURFACEDESC ddsdSource,ddsdOverlayer;
	int surfacePitch1,surfacePitch2,nColorOverlayer;
	int i,j,w = SURFACE_WIDTH,h = SURFACE_HEIGHT,subx,dx = 0,dy = 0;
	int R,G,B,Gray,g;
	bool odd = false;
	RECT rect;

	if(ox >= lpDraw->xSize || ox + w <= 0 || oy >= lpDraw->ySize || oy + h <= 0 ) return;
	if(bLastOne){
		w = ow - offsetx;
		h = oh - offsety;
	}
	if(ox + w >= lpDraw->xSize){
		w = lpDraw->xSize - ox;
	}
	if(oy + h >= lpDraw->ySize) h = lpDraw->ySize - oy;
	if(ox < 0){
		w = w + ox;
		if(w < 0) return;
		dx = ox*-1;
		ox = 0;
	}
	if(oy < 0){
		h = h + oy;
		if(h < 0) return;
		dy = oy*-1;
		oy = 0;
	}
	subx = ox;
	
	if(iGrayType == 3) g = 6;
	else if(iGrayType == 4)	g = 5;

	ZeroMemory(&ddsdSource,sizeof(DDSURFACEDESC));
	ddsdSource.dwSize = sizeof(DDSURFACEDESC);
	ZeroMemory(&ddsdOverlayer,sizeof(DDSURFACEDESC));
	ddsdOverlayer.dwSize = sizeof(DDSURFACEDESC);
	if(lpDraw->lpBACKBUFFER->Lock(NULL,&ddsdSource,DDLOCK_WAIT,NULL) != DD_OK) return;
	if(lpSurface->Lock(NULL,&ddsdOverlayer,DDLOCK_WAIT,NULL) != DD_OK){
		lpDraw->lpBACKBUFFER->Unlock(NULL);
		return;
	}
#ifdef _HI_COLOR_32
	if( displayBpp == 32 ){
		void *ptSourceDest,*ptOverLayerDest;
		surfacePitch1 = ddsdSource.lPitch >> 2;
		surfacePitch2 = ddsdOverlayer.lPitch >> 2;
		ptSourceDest = (DWORD*)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
		ptOverLayerDest = (DWORD*)(ddsdOverlayer.lpSurface) +dy * surfacePitch2 + dx;

		// 565 显示模式
		if(gBitRShift == 0){
			for(j=0;j<h;j++){
				if(oy >= 0){
					for(i=0;i<w;i++){
						if(subx >= 0){
							nColorOverlayer = *(DWORD*)ptOverLayerDest;
							if(nColorOverlayer != DEF_COLORKEY){
								R = (nColorOverlayer & 0xff0000) >> 16;
								G = (nColorOverlayer & 0x00ff00) >> 8;
								B = (nColorOverlayer & 0x0000ff);
								Gray = (R*3+G*5+B)/10;
								*(DWORD*)ptSourceDest = (DWORD)(Gray << 16 | Gray << 8 | Gray);
							}
						}
						else subx++;
						ptSourceDest = (DWORD*)ptSourceDest + 1;
						ptOverLayerDest = (DWORD*)ptOverLayerDest + 1;
					}
					ptSourceDest = (DWORD*)ptSourceDest + surfacePitch1 - w;
					ptOverLayerDest = (DWORD*)ptOverLayerDest + surfacePitch2 - w;
					subx = ox;
				}
				else{
					ptSourceDest = (DWORD*)ptSourceDest + surfacePitch1;
					ptOverLayerDest = (DWORD*)ptOverLayerDest + surfacePitch2;
					oy++;
				}
			}
		}
	}else
#endif
	if(displayBpp == 16){
		void *ptSourceDest,*ptOverLayerDest;
		surfacePitch1 = ddsdSource.lPitch >> 1;
		surfacePitch2 = ddsdOverlayer.lPitch >> 1;
		ptSourceDest = (WORD*)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
		ptOverLayerDest = (WORD*)(ddsdOverlayer.lpSurface) +dy * surfacePitch2 + dx;
		
		// 565 显示模式
		if(gBitRShift == 2){
			for(j=0;j<h;j++){
				if(oy >= 0){
					for(i=0;i<w;i++){
						if(subx >= 0){
							nColorOverlayer = *(WORD*)ptOverLayerDest;
							if(nColorOverlayer != DEF_COLORKEY){
								R = (nColorOverlayer & 0xf800) >> 11;
								G = (nColorOverlayer & 0x07e0) >> 6;
								B = nColorOverlayer & 0x001f;
								Gray = (R*3+G*6+B)/10;
								*(WORD*)ptSourceDest = (WORD)(Gray << 11 | Gray << g | Gray);
							}
						}
						else subx++;
						ptSourceDest = (WORD*)ptSourceDest + 1;
						ptOverLayerDest = (WORD*)ptOverLayerDest + 1;
					}
					ptSourceDest = (WORD*)ptSourceDest + surfacePitch1 - w;
					ptOverLayerDest = (WORD*)ptOverLayerDest + surfacePitch2 - w;
					subx = ox;
				}
				else{
					ptSourceDest = (WORD*)ptSourceDest + surfacePitch1;
					ptOverLayerDest = (WORD*)ptOverLayerDest + surfacePitch2;
					oy++;
				}
			}
		}
	}
	lpSurface->Unlock(NULL);
	lpDraw->lpBACKBUFFER->Unlock(NULL);
	rect.left = ox;
	rect.top = oy;
	rect.right = ox+w;
	rect.bottom = oy+h;
	lpDraw->lpBACKBUFFER->BltFast(ox,oy,lpDraw->lpBACKBUFFER,&rect,DDBLTFAST_WAIT);
	return;
}

void DrawGrayA(LPDIRECTDRAWSURFACE lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne,int iGrayType){
	static DDSURFACEDESC ddsdSource,ddsdOverlayer;
	int surfacePitch1,surfacePitch2,nColorOverlayer;
	int i,j,w = SURFACE_WIDTH,h = SURFACE_HEIGHT,subx,dx = 0,dy = 0;
	int R,G,B,Gray,g;
	bool odd = false;
	RECT rect;

	if(ox >= lpDraw->xSize || ox + w <= 0 || oy >= lpDraw->ySize || oy + h <= 0 ) return;
	if(bLastOne){
		w = ow - offsetx;
		h = oh - offsety;
	}
	if(ox + w >= lpDraw->xSize){
		w = lpDraw->xSize - ox;
	}
	if(oy + h >= lpDraw->ySize) h = lpDraw->ySize - oy;
	if(ox < 0){
		w = w + ox;
		if(w < 0) return;
		dx = ox*-1;
		ox = 0;
	}
	if(oy < 0){
		h = h + oy;
		if(h < 0) return;
		dy = oy*-1;
		oy = 0;
	}
	subx = ox;
	
	if(iGrayType == 3) g = 6;
	else if(iGrayType == 4)	g = 5;

	ZeroMemory(&ddsdSource,sizeof(DDSURFACEDESC));
	ddsdSource.dwSize = sizeof(DDSURFACEDESC);
	ZeroMemory(&ddsdOverlayer,sizeof(DDSURFACEDESC));
	ddsdOverlayer.dwSize = sizeof(DDSURFACEDESC);
	if(lpDraw->lpBACKBUFFER->Lock(NULL,&ddsdSource,DDLOCK_WAIT,NULL) != DD_OK) return;
	if(lpSurface->Lock(NULL,&ddsdOverlayer,DDLOCK_WAIT,NULL) != DD_OK){
		lpDraw->lpBACKBUFFER->Unlock(NULL);
		return;
	}

	unsigned char ALPHA = 150, RED = 250, GREEN = 0, BLUE = 250;
#ifdef _HI_COLOR_32
	if( displayBpp == 32 ){
		void *ptSourceDest,*ptOverLayerDest;
		surfacePitch1 = ddsdSource.lPitch >> 2;
		surfacePitch2 = ddsdOverlayer.lPitch >> 2;
		ptSourceDest = (DWORD*)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
		ptOverLayerDest = (DWORD*)(ddsdOverlayer.lpSurface) +dy * surfacePitch2 + dx;
		// 565 显示模式
		if(gBitRShift == 0){
			for(j=0;j<h;j++){
				if(oy >= 0){
					for(i=0;i<w;i++){
						if(subx >= 0){
							nColorOverlayer = *(DWORD*)ptOverLayerDest;
							if(nColorOverlayer != DEF_COLORKEY){
								R = (nColorOverlayer & 0xff0000) >> 16;
								G = (nColorOverlayer & 0x00ff00) >> 8;
								B = (nColorOverlayer & 0x0000ff);

								R = RED + (R - RED) * ALPHA / 0xff;
								G = GREEN + (G - GREEN) * ALPHA / 0xff;
								B = BLUE + (B - BLUE) * ALPHA / 0xff;
								Gray = 0xff;
								*(DWORD*)ptSourceDest = (DWORD)(Gray << 24 | R<<16 | G << 8 | B);
							}
						}
						else subx++;
						ptSourceDest = (DWORD*)ptSourceDest + 1;
						ptOverLayerDest = (DWORD*)ptOverLayerDest + 1;
					}
					ptSourceDest = (DWORD*)ptSourceDest + surfacePitch1 - w;
					ptOverLayerDest = (DWORD*)ptOverLayerDest + surfacePitch2 - w;
					subx = ox;
				}
				else{
					ptSourceDest = (DWORD*)ptSourceDest + surfacePitch1;
					ptOverLayerDest = (DWORD*)ptOverLayerDest + surfacePitch2;
					oy++;
				}
			}
		}
	}else
#endif
	if(displayBpp == 16){
		void *ptSourceDest,*ptOverLayerDest;
		surfacePitch1 = ddsdSource.lPitch >> 1;
		surfacePitch2 = ddsdOverlayer.lPitch >> 1;
		ptSourceDest = (WORD*)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
		ptOverLayerDest = (WORD*)(ddsdOverlayer.lpSurface) +dy * surfacePitch2 + dx;
		
		// 565 显示模式
		if(gBitRShift == 2){
			for(j=0;j<h;j++){
				if(oy >= 0){
					for(i=0;i<w;i++){
						if(subx >= 0){
							nColorOverlayer = *(WORD*)ptOverLayerDest;
							if(nColorOverlayer != DEF_COLORKEY){
								R = (nColorOverlayer & 0xf800) >> 8;
								G = (nColorOverlayer & 0x07e0) >> 3;
								B = (nColorOverlayer & 0x001f) << 3;
								R = RED + (R - RED) * ALPHA / 0xff;
								G = GREEN + (G - GREEN) * ALPHA / 0xff;
								B = BLUE + (B - BLUE) * ALPHA / 0xff;
								Gray = 0xff;

								//Gray = (R*3+G*6+B)/10;
								*(WORD*)ptSourceDest = (WORD)((R & 0xf8) << 8 | (G & 0xfc) << 3 | B >> 3);
							}
						}
						else subx++;
						ptSourceDest = (WORD*)ptSourceDest + 1;
						ptOverLayerDest = (WORD*)ptOverLayerDest + 1;
					}
					ptSourceDest = (WORD*)ptSourceDest + surfacePitch1 - w;
					ptOverLayerDest = (WORD*)ptOverLayerDest + surfacePitch2 - w;
					subx = ox;
				}
				else{
					ptSourceDest = (WORD*)ptSourceDest + surfacePitch1;
					ptOverLayerDest = (WORD*)ptOverLayerDest + surfacePitch2;
					oy++;
				}
			}
		}
	}
	lpSurface->Unlock(NULL);
	lpDraw->lpBACKBUFFER->Unlock(NULL);
	rect.left = ox;
	rect.top = oy;
	rect.right = ox+w;
	rect.bottom = oy+h;
	lpDraw->lpBACKBUFFER->BltFast(ox,oy,lpDraw->lpBACKBUFFER,&rect,DDBLTFAST_WAIT);
	return;
}

void DrawStaturated(LPDIRECTDRAWSURFACE lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne){
	static DDSURFACEDESC ddsdSource,ddsdOverlayer;
	int surfacePitch1,surfacePitch2;
	int w = SURFACE_WIDTH,h = SURFACE_HEIGHT,dx = 0,dy = 0;
	DWORD Is4Multiple;
	RECT rect;

	if(ox >= lpDraw->xSize || ox + w <= 0 || oy >= lpDraw->ySize || oy + h <= 0 ) return;
	if(bLastOne){
		w = ow - offsetx;
		h = oh - offsety;
	}
	if(ox + w >= lpDraw->xSize){
		w = lpDraw->xSize - ox;
	}
	if(oy + h >= lpDraw->ySize) h = lpDraw->ySize - oy;
	if(ox < 0){
		w = w + ox;
		if(w < 0) return;
		dx = ox*-1;
		ox = 0;
	}
	if(oy < 0){
		h = h + oy;
		if(h < 0) return;
		dy = oy*-1;
		oy = 0;
	}
	Is4Multiple = w % 4;

	ZeroMemory(&ddsdSource,sizeof(DDSURFACEDESC));
	ddsdSource.dwSize = sizeof(DDSURFACEDESC);
	ZeroMemory(&ddsdOverlayer,sizeof(DDSURFACEDESC));
	ddsdOverlayer.dwSize = sizeof(DDSURFACEDESC);
	if(lpDraw->lpBACKBUFFER->Lock(NULL,&ddsdSource,DDLOCK_WAIT,NULL) != DD_OK) return;
	if(lpSurface->Lock(NULL,&ddsdOverlayer,DDLOCK_WAIT,NULL) != DD_OK){
		lpDraw->lpBACKBUFFER->Unlock(NULL);
		return;
	}

	void *ptSourceDest,*ptOverLayerDest;
#ifdef _HI_COLOR_32
	void *ptSourceDest32,*ptOverLayerDest32;
	if( displayBpp == 32 ){
		surfacePitch1 = ddsdSource.lPitch >> 2;
		surfacePitch2 = ddsdOverlayer.lPitch >> 2;
		ptSourceDest32 = (DWORD *)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
		ptOverLayerDest32 = (DWORD *)(ddsdOverlayer.lpSurface) + dy * surfacePitch2 + dx;
	}else
#endif
	{
		surfacePitch1 = ddsdSource.lPitch >> 1;
		surfacePitch2 = ddsdOverlayer.lPitch >> 1;
		ptSourceDest = (WORD*)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
		ptOverLayerDest = (WORD*)(ddsdOverlayer.lpSurface) + dy * surfacePitch2 + dx;
	}

	{
		int i,j,nColorSource,nColorOverlayer;
		DWORD SR,SG,SB,OR,OG,OB;

		// 565 显示模式
		if(gBitRShift == 2){
#ifdef _HI_COLOR_32
			if( displayBpp == 32 ){
				for(j=0;j<h;j++){
					for(i=0;i<w;i++){
						nColorSource = *(DWORD*)ptSourceDest32;
						nColorOverlayer = *(DWORD*)ptOverLayerDest32;
						if(nColorOverlayer != DEF_COLORKEY){
							SR = (nColorSource & 0xff0000) >> 8; // 原本要右移11左移3,简化成右移8
							SG = (nColorSource & 0x00ff00) >> 3; // 原本要右移5左移2,简化成右移3
							SB = (nColorSource & 0x0000ff) << 3;
							OR = (nColorOverlayer & 0xff0000) >> 8;
							OG = (nColorOverlayer & 0x00ff00) >> 3;
							OB = (nColorOverlayer & 0x0000ff) << 3;
							//								SR = (((SR + OR) > 0x00ff ? 0x00ff:SR+OR) << 8) & 0xf800;
							//								SG = (((SG + OG) > 0x00ff ? 0x00ff:SG+OG) << 3) & 0x07e0;
							//								SB = (((SB + OB) > 0x00ff ? 0x00ff:SB+OB) >> 3) & 0x001f;
							SR = ((OR > SR ? OR:SR) << 8) & 0xff0000;
							SG = ((OG > SG ? OG:SG) << 3) & 0x00ff00;
							SB = ((OB > SB ? OB:SB) >> 3) & 0x0000ff;
							*(DWORD*)ptSourceDest32 = (DWORD)(SB | SG | SR);
						}
						ptSourceDest32 = (DWORD*)ptSourceDest32 + 1;
						ptOverLayerDest32 = (DWORD*)ptOverLayerDest32 + 1;
					}
					ptSourceDest32 = (DWORD*)ptSourceDest32 + surfacePitch1 - w;
					ptOverLayerDest32 = (DWORD*)ptOverLayerDest32 + surfacePitch2 - w;
				}
			}else
#endif
			{
				for(j=0;j<h;j++){
					for(i=0;i<w;i++){
						nColorSource = *(WORD*)ptSourceDest;
						nColorOverlayer = *(WORD*)ptOverLayerDest;
						if(nColorOverlayer != DEF_COLORKEY){
							SR = (nColorSource & 0xf800) >> 8; // 原本要右移11左移3,简化成右移8
							SG = (nColorSource & 0x07e0) >> 3; // 原本要右移5左移2,简化成右移3
							SB = (nColorSource & 0x001f) << 3;
							OR = (nColorOverlayer & 0xf800) >> 8;
							OG = (nColorOverlayer & 0x07e0) >> 3;
							OB = (nColorOverlayer & 0x001f) << 3;
							//								SR = (((SR + OR) > 0x00ff ? 0x00ff:SR+OR) << 8) & 0xf800;
							//								SG = (((SG + OG) > 0x00ff ? 0x00ff:SG+OG) << 3) & 0x07e0;
							//								SB = (((SB + OB) > 0x00ff ? 0x00ff:SB+OB) >> 3) & 0x001f;
							SR = ((OR > SR ? OR:SR) << 8) & 0xf800;
							SG = ((OG > SG ? OG:SG) << 3) & 0x07e0;
							SB = ((OB > SB ? OB:SB) >> 3) & 0x001f;
							*(WORD*)ptSourceDest = (WORD)(SB | SG | SR);
						}
						ptSourceDest = (WORD*)ptSourceDest + 1;
						ptOverLayerDest = (WORD*)ptOverLayerDest + 1;
					}
					ptSourceDest = (WORD*)ptSourceDest + surfacePitch1 - w;
					ptOverLayerDest = (WORD*)ptOverLayerDest + surfacePitch2 - w;
				}
			}
		}
		// 555 显示模式
		else{
#ifdef _HI_COLOR_32
			if( displayBpp == 32 ){
				for(j=0;j<h;j++){
					for(i=0;i<w;i++){
						nColorSource = *(DWORD*)ptSourceDest32;
						nColorOverlayer = *(DWORD*)ptOverLayerDest32;
						if(nColorOverlayer != DEF_COLORKEY){
							SR = (nColorSource & 0xff0000) >> 7;							// 原本要右移10左移3,简化成右移7
							SG = (nColorSource & 0x00ff00) >> 2;	// 原本要右移5左移3,简化成右移2
							SB = (nColorSource & 0x0000ff) << 3;
							OR = (nColorSource & 0xff0000) >> 7;
							OG = (nColorOverlayer & 0x00ff00) >> 2;
							OB = (nColorOverlayer & 0x0000ff) << 3;
							//								SR = ((SR + OR) & 0x00ff)<< 7;
							//								SG = (((SG + OG) & 0x00ff)<< 2) & 0x03e0;
							//								SB = (((SB + OB) & 0x00ff)>> 3) & 0x001f;
							SR = (((OR > SR ? OR:SR) & 0x00ff)<< 7) & 0xff0000;
							SG = (((OG > SG ? OG:SG) & 0x00ff)<< 2) & 0x00ff00;
							SB = (((OB > SB ? OB:SB) & 0x00ff)>> 3) & 0x0000ff;
							*(DWORD*)ptSourceDest32 = (DWORD)(SB | SG | SR);
						}
						ptSourceDest32 = (DWORD*)ptSourceDest32 + 1;
						ptOverLayerDest = (DWORD*)ptOverLayerDest32 + 1;
					}
					ptSourceDest32 = (DWORD*)ptSourceDest32 + surfacePitch1 - w;
					ptOverLayerDest = (DWORD*)ptOverLayerDest32 + surfacePitch2 - w;
				}
			}else
#endif
			{
				for(j=0;j<h;j++){
					for(i=0;i<w;i++){
						nColorSource = *(WORD*)ptSourceDest;
						nColorOverlayer = *(WORD*)ptOverLayerDest;
						if(nColorOverlayer != DEF_COLORKEY){
							SR = nColorSource >> 7;							// 原本要右移10左移3,简化成右移7
							SG = (nColorSource & 0x03e0) >> 2;	// 原本要右移5左移3,简化成右移2
							SB = (nColorSource & 0x001f) << 3;
							OR = nColorOverlayer >> 7;
							OG = (nColorOverlayer & 0x03e0) >> 2;
							OB = (nColorOverlayer & 0x001f) << 3;
							//								SR = ((SR + OR) & 0x00ff)<< 7;
							//								SG = (((SG + OG) & 0x00ff)<< 2) & 0x03e0;
							//								SB = (((SB + OB) & 0x00ff)>> 3) & 0x001f;
							SR = ((OR > SR ? OR:SR) & 0x00ff)<< 7;
							SG = (((OG > SG ? OG:SG) & 0x00ff)<< 2) & 0x03e0;
							SB = (((OB > SB ? OB:SB) & 0x00ff)>> 3) & 0x001f;
							*(WORD*)ptSourceDest = (WORD)(SB | SG | SR);
						}
						ptSourceDest = (WORD*)ptSourceDest + 1;
						ptOverLayerDest = (WORD*)ptOverLayerDest + 1;
					}
					ptSourceDest = (WORD*)ptSourceDest + surfacePitch1 - w;
					ptOverLayerDest = (WORD*)ptOverLayerDest + surfacePitch2 - w;
				}
			}
		}
	}
	lpDraw->lpBACKBUFFER->Unlock(NULL);
	lpSurface->Unlock(NULL);
	rect.left = ox;
	rect.top = oy;
	rect.right = ox+w;
	rect.bottom = oy+h;
	lpDraw->lpBACKBUFFER->BltFast(ox,oy,lpDraw->lpBACKBUFFER,&rect,DDBLTFAST_WAIT);

	return;
}

#ifdef _READ16BITBMP
extern int gBitRShift;


void DrawAlpha(LPDIRECTDRAWSURFACE lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,int Alpha,bool bLastOne){
	static DDSURFACEDESC ddsdSource,ddsdOverlayer;
	int surfacePitch1,surfacePitch2;
	int w = SURFACE_WIDTH,h = SURFACE_HEIGHT,dx = 0,dy = 0;
	DWORD Is4Multiple;
	bool odd = false;
	RECT rect;

	if(ox >= lpDraw->xSize || ox + w <= 0 || oy >= lpDraw->ySize || oy + h <= 0 ) return;
	if(bLastOne){
		w = ow - offsetx;
		if((w % 2) == 1) odd = true;
		h = oh - offsety;
	}
	if(ox + w >= lpDraw->xSize){
		w = lpDraw->xSize - ox;
		if((w % 2) == 1) odd = true;
	}
	if(oy + h >= lpDraw->ySize) h = lpDraw->ySize - oy;
	if(ox < 0){
		w = w + ox;
		if(w < 0) return;
		if((w % 2) == 1) odd = true;
		dx = ox*-1;
		ox = 0;
	}
	if(oy < 0){
		h = h + oy;
		if(h < 0) return;
		dy = oy*-1;
		oy = 0;
	}
	Is4Multiple = w % 4;
	ZeroMemory(&ddsdSource,sizeof(DDSURFACEDESC));
	ddsdSource.dwSize = sizeof(DDSURFACEDESC);
	ZeroMemory(&ddsdOverlayer,sizeof(DDSURFACEDESC));
	ddsdOverlayer.dwSize = sizeof(DDSURFACEDESC);
	if(lpDraw->lpBACKBUFFERSYS->Lock(NULL,&ddsdSource,DDLOCK_WAIT,NULL) != DD_OK) return;
	if(lpSurface->Lock(NULL,&ddsdOverlayer,DDLOCK_WAIT,NULL) != DD_OK){
		lpDraw->lpBACKBUFFERSYS->Unlock(NULL);
		return;
	}

	void *ptSourceDest,*ptOverLayerDest;
#ifdef _HI_COLOR_32
	void *ptSourceDest32,*ptOverLayerDest32;
	if( displayBpp == 32 ){
		surfacePitch1 = ddsdSource.lPitch >> 2;
		surfacePitch2 = ddsdOverlayer.lPitch >> 2;
		ptSourceDest32 = (int *)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
		ptOverLayerDest32 = (int *)(ddsdOverlayer.lpSurface) + dy * surfacePitch2 + dx;
	}else
#endif
	{
		surfacePitch1 = ddsdSource.lPitch >> 1;
		surfacePitch2 = ddsdOverlayer.lPitch >> 1;
		ptSourceDest = (WORD*)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
		ptOverLayerDest = (WORD*)(ddsdOverlayer.lpSurface) + dy * surfacePitch2 + dx;


	}
	
	{
		int i,j,nColorSource,nColorOverlayer;
		DWORD SR,SG,SB,OR,OG,OB;

		w >>= 1;
		// 565 显示模式
		if(gBitRShift == 2){
#ifdef _HI_COLOR_32
			if( displayBpp == 32 ){
				for(j=0;j<h;j++){
					if(odd){
						nColorSource = *(DWORD*)ptSourceDest32;
						nColorOverlayer = *(DWORD*)ptOverLayerDest32;
						if(nColorOverlayer != DEF_COLORKEY){
							SR = (nColorSource & 0xff0000) >> 8; // 原本要右移11左移3,简化成右移8
							SG = (nColorSource & 0x00ff00) >> 3; // 原本要右移5左移2,简化成右移3
							SB = (nColorSource & 0x0000ff) << 3;
							OR = (nColorOverlayer & 0xff0000) >> 8;
							OG = (nColorOverlayer & 0x00ff00) >> 3;
							OB = (nColorOverlayer & 0x0000ff) << 3;
							SR = (((((OR - SR) * Alpha) >> 5) + SR) << 8) & 0xff0000;
							SG = (((((OG - SG) * Alpha) >> 5) + SG) << 3) & 0x00ff00;
							SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x0000ff;
							*(DWORD*)ptSourceDest32 = (DWORD)(SB | SG | SR);
						}
						ptSourceDest32 = (DWORD*)ptSourceDest32 + 1;
						ptOverLayerDest32 = (DWORD*)ptOverLayerDest32 + 1;
					}
					for(i=0;i<w;i++){
						nColorSource = *(DWORD*)ptSourceDest32;
						nColorOverlayer = *(DWORD*)ptOverLayerDest32;
						if(nColorOverlayer != DEF_COLORKEY){
							SR = (nColorSource & 0xff0000) >> 8;
							SG = (nColorSource & 0x00ff00) >> 3;
							SB = (nColorSource & 0x0000ff) << 3;
							OR = (nColorOverlayer & 0xff0000) >> 8;
							OG = (nColorOverlayer & 0x00ff00) >> 3;
							OB = (nColorOverlayer & 0x0000ff) << 3;
							SR = (((((OR - SR) * Alpha) >> 5) + SR) << 8) & 0xff0000;
							SG = (((((OG - SG) * Alpha) >> 5) + SG) << 3) & 0x00ff00;
							SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x0000ff;
							*(DWORD*)ptSourceDest32 = SB | SG | SR;
						}
						ptSourceDest32 = (DWORD*)ptSourceDest + 1;
						ptOverLayerDest32 = (DWORD*)ptOverLayerDest32 + 1;
					}
					ptSourceDest32 = (DWORD*)ptSourceDest32 + (surfacePitch1 >> 2) - w;
					ptOverLayerDest32 = (DWORD*)ptOverLayerDest32 + (surfacePitch2 >> 2) - w;
					if(odd){
						ptSourceDest32 = (WORD*)ptSourceDest - 1;
						ptOverLayerDest32 = (WORD*)ptOverLayerDest32 - 1;
					}
				}
			}else
#endif
			{
				for(j=0;j<h;j++){
					if(odd){
						nColorSource = *(WORD*)ptSourceDest;
						nColorOverlayer = *(WORD*)ptOverLayerDest;
						if(nColorOverlayer != DEF_COLORKEY){
							SR = (nColorSource & 0xf800) >> 8; // 原本要右移11左移3,简化成右移8
							SG = (nColorSource & 0x07e0) >> 3; // 原本要右移5左移2,简化成右移3
							SB = (nColorSource & 0x001f) << 3;
							OR = (nColorOverlayer & 0xf800) >> 8;
							OG = (nColorOverlayer & 0x07e0) >> 3;
							OB = (nColorOverlayer & 0x001f) << 3;
							SR = (((((OR - SR) * Alpha) >> 5) + SR) << 8) & 0xf800;
							SG = (((((OG - SG) * Alpha) >> 5) + SG) << 3) & 0x07e0;
							SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x001f;
							*(WORD*)ptSourceDest = (WORD)(SB | SG | SR);
						}
						ptSourceDest = (WORD*)ptSourceDest + 1;
						ptOverLayerDest = (WORD*)ptOverLayerDest + 1;
					}
					for(i=0;i<w;i++){
						nColorSource = *(DWORD*)ptSourceDest;
						nColorOverlayer = *(DWORD*)ptOverLayerDest;
						if(nColorOverlayer != DEF_COLORKEY){
							SR = (nColorSource & 0xf800f800) >> 8;
							SG = (nColorSource & 0x07e007e0) >> 3;
							SB = (nColorSource & 0x001f001f) << 3;
							OR = (nColorOverlayer & 0xf800f800) >> 8;
							OG = (nColorOverlayer & 0x07e007e0) >> 3;
							OB = (nColorOverlayer & 0x001f001f) << 3;
							SR = (((((OR - SR) * Alpha) >> 5) + SR) << 8) & 0xf800f800;
							SG = (((((OG - SG) * Alpha) >> 5) + SG) << 3) & 0x07e007e0;
							SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x001f001f;
							*(DWORD*)ptSourceDest = SB | SG | SR;
						}
						ptSourceDest = (DWORD*)ptSourceDest + 1;
						ptOverLayerDest = (DWORD*)ptOverLayerDest + 1;
					}
					ptSourceDest = (DWORD*)ptSourceDest + (surfacePitch1 >> 1) - w;
					ptOverLayerDest = (DWORD*)ptOverLayerDest + (surfacePitch2 >> 1) - w;
					if(odd){
						ptSourceDest = (WORD*)ptSourceDest - 1;
						ptOverLayerDest = (WORD*)ptOverLayerDest - 1;
					}
				}
			}
			
		}
		// 555 显示模式
		else{
#ifdef _HI_COLOR_32
			if( displayBpp == 32 ){
				for(j=0;j<h;j++){
					if(odd){
						nColorSource = *(DWORD*)ptSourceDest32;
						nColorOverlayer = *(DWORD*)ptOverLayerDest32;
						if(nColorOverlayer != DEF_COLORKEY){
							SR = (nColorSource & 0xff0000) >> 7;							// 原本要右移10左移3,简化成右移7
							SG = (nColorSource & 0x00ff00) >> 2;	// 原本要右移5左移3,简化成右移2
							SB = (nColorSource & 0x0000ff) << 3;
							OR = (nColorOverlayer & 0xff0000) >> 7;
							OG = (nColorOverlayer & 0x00ff00) >> 2;
							OB = (nColorOverlayer & 0x0000ff) << 3;
							SR = (((((OR - SR) * Alpha) >> 5) + SR) << 7) & 0xff0000;
							SG = (((((OG - SG) * Alpha) >> 5) + SG) << 2) & 0x00ff00;
							SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x0000ff;
							*(DWORD*)ptSourceDest32 = (DWORD)(SB | SG | SR);
						}
						ptSourceDest32 = (DWORD*)ptSourceDest32 + 1;
						ptOverLayerDest32 = (DWORD*)ptOverLayerDest32 + 1;
					}
					for(i=0;i<w;i++){
						nColorSource = *(DWORD*)ptSourceDest32;
						nColorOverlayer = *(DWORD*)ptOverLayerDest32;
						if(nColorOverlayer != DEF_COLORKEY){
							SR = (nColorSource & 0xff0000) >> 7;
							SG = (nColorSource & 0x00ff00) >> 2;
							SB = (nColorSource & 0x0000ff) << 3;
							OR = (nColorOverlayer & 0xff0000) >> 7;
							OG = (nColorOverlayer & 0x00ff00) >> 2;
							OB = (nColorOverlayer & 0x0000ff) << 3;
							SR = (((((OR - SR) * Alpha) >> 5) + SR) << 7) & 0xff0000;
							SG = (((((OG - SG) * Alpha) >> 5) + SG) << 2) & 0x00ff00;
							SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x0000ff;
							*(DWORD*)ptSourceDest32 = SB | SG | SR;
							ptSourceDest32 = (DWORD*)ptSourceDest32 + 1;
							ptOverLayerDest32 = (DWORD*)ptOverLayerDest32 + 1;
						}
						ptSourceDest32 = (DWORD*)ptSourceDest32 + 1;
						ptOverLayerDest32 = (DWORD*)ptOverLayerDest32 + 1;
					}
					ptSourceDest32 = (DWORD*)ptSourceDest32 + (surfacePitch1 >> 2) - w;
					ptOverLayerDest32 = (DWORD*)ptOverLayerDest32 + (surfacePitch2 >> 2) - w;
					if(odd){
						ptSourceDest32 = (WORD*)ptSourceDest32 - 1;
						ptOverLayerDest32 = (WORD*)ptOverLayerDest32 - 1;
					}
				}
			}else
#endif
			{
				for(j=0;j<h;j++){
					if(odd){
						nColorSource = *(WORD*)ptSourceDest;
						nColorOverlayer = *(WORD*)ptOverLayerDest;
						if(nColorOverlayer != DEF_COLORKEY){
							SR = nColorSource >> 7;							// 原本要右移10左移3,简化成右移7
							SG = (nColorSource & 0x03e0) >> 2;	// 原本要右移5左移3,简化成右移2
							SB = (nColorSource & 0x001f) << 3;
							OR = nColorOverlayer >> 7;
							OG = (nColorOverlayer & 0x03e0) >> 2;
							OB = (nColorOverlayer & 0x001f) << 3;
							SR = ((((OR - SR) * Alpha) >> 5) + SR) << 7;
							SG = (((((OG - SG) * Alpha) >> 5) + SG) << 2) & 0x03e0;
							SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x001f;
							*(WORD*)ptSourceDest = (WORD)(SB | SG | SR);
						}
						ptSourceDest = (WORD*)ptSourceDest + 1;
						ptOverLayerDest = (WORD*)ptOverLayerDest + 1;
					}
					for(i=0;i<w;i++){
						nColorSource = *(DWORD*)ptSourceDest;
						nColorOverlayer = *(DWORD*)ptOverLayerDest;
						if(nColorOverlayer != DEF_COLORKEY){
							SR = (nColorSource & 0x7c007c00) >> 7;
							SG = (nColorSource & 0x03e003e0) >> 2;
							SB = (nColorSource & 0x001f001f) << 3;
							OR = (nColorOverlayer & 0x7c007c00) >> 7;
							OG = (nColorOverlayer & 0x03e003e0) >> 2;
							OB = (nColorOverlayer & 0x001f001f) << 3;
							SR = (((((OR - SR) * Alpha) >> 5) + SR) << 7) & 0x7c007c00;
							SG = (((((OG - SG) * Alpha) >> 5) + SG) << 2) & 0x03e003e0;
							SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x001f001f;
							*(DWORD*)ptSourceDest = SB | SG | SR;
							ptSourceDest = (DWORD*)ptSourceDest + 1;
							ptOverLayerDest = (DWORD*)ptOverLayerDest + 1;
						}
						ptSourceDest = (DWORD*)ptSourceDest + 1;
						ptOverLayerDest = (DWORD*)ptOverLayerDest + 1;
					}
					ptSourceDest = (DWORD*)ptSourceDest + (surfacePitch1 >> 1) - w;
					ptOverLayerDest = (DWORD*)ptOverLayerDest + (surfacePitch2 >> 1) - w;
					if(odd){
						ptSourceDest = (WORD*)ptSourceDest - 1;
						ptOverLayerDest = (WORD*)ptOverLayerDest - 1;
					}
				}
			}
		}
	}
	lpDraw->lpBACKBUFFERSYS->Unlock(NULL);
	lpSurface->Unlock(NULL);
	rect.left = ox;
	rect.top = oy;
	rect.right = ox+w;
	rect.bottom = oy+h;
	lpDraw->lpBACKBUFFER->BltFast(ox,oy,lpDraw->lpBACKBUFFERSYS,&rect,DDBLTFAST_WAIT);

	return;
}

void DrawStaturated(LPDIRECTDRAWSURFACE lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne){
	static DDSURFACEDESC ddsdSource,ddsdOverlayer;
	int surfacePitch1,surfacePitch2;
	int w = SURFACE_WIDTH,h = SURFACE_HEIGHT,dx = 0,dy = 0;
	DWORD Is4Multiple;
	RECT rect;

	if(ox >= lpDraw->xSize || ox + w <= 0 || oy >= lpDraw->ySize || oy + h <= 0 ) return;
	if(bLastOne){
		w = ow - offsetx;
		h = oh - offsety;
	}
	if(ox + w >= lpDraw->xSize){
		w = lpDraw->xSize - ox;
	}
	if(oy + h >= lpDraw->ySize) h = lpDraw->ySize - oy;
	if(ox < 0){
		w = w + ox;
		if(w < 0) return;
		dx = ox*-1;
		ox = 0;
	}
	if(oy < 0){
		h = h + oy;
		if(h < 0) return;
		dy = oy*-1;
		oy = 0;
	}
	Is4Multiple = w % 4;

	ZeroMemory(&ddsdSource,sizeof(DDSURFACEDESC));
	ddsdSource.dwSize = sizeof(DDSURFACEDESC);
	ZeroMemory(&ddsdOverlayer,sizeof(DDSURFACEDESC));
	ddsdOverlayer.dwSize = sizeof(DDSURFACEDESC);
	if(lpDraw->lpBACKBUFFERSYS->Lock(NULL,&ddsdSource,DDLOCK_WAIT,NULL) != DD_OK) return;
	if(lpSurface->Lock(NULL,&ddsdOverlayer,DDLOCK_WAIT,NULL) != DD_OK){
		lpDraw->lpBACKBUFFERSYS->Unlock(NULL);
		return;
	}

	void *ptSourceDest,*ptOverLayerDest;
#ifdef _HI_COLOR_32
	void *ptSourceDest32,*ptOverLayerDest32;
	if( displayBpp == 32 ){
		surfacePitch1 = ddsdSource.lPitch >> 2;
		surfacePitch2 = ddsdOverlayer.lPitch >> 2;
		ptSourceDest32 = (DWORD *)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
		ptOverLayerDest32 = (DWORD *)(ddsdOverlayer.lpSurface) + dy * surfacePitch2 + dx;
	}else
#endif
	{
		surfacePitch1 = ddsdSource.lPitch >> 1;
		surfacePitch2 = ddsdOverlayer.lPitch >> 1;
		ptSourceDest = (WORD*)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
		ptOverLayerDest = (WORD*)(ddsdOverlayer.lpSurface) + dy * surfacePitch2 + dx;
	}

	{
		int i,j,nColorSource,nColorOverlayer;
		DWORD SR,SG,SB,OR,OG,OB;

		// 565 显示模式
		if(gBitRShift == 2){
#ifdef _HI_COLOR_32
			if( displayBpp == 32 ){
				for(j=0;j<h;j++){
					for(i=0;i<w;i++){
						nColorSource = *(DWORD*)ptSourceDest32;
						nColorOverlayer = *(DWORD*)ptOverLayerDest32;
						if(nColorOverlayer != DEF_COLORKEY){
							SR = (nColorSource & 0xff0000) >> 8; // 原本要右移11左移3,简化成右移8
							SG = (nColorSource & 0x00ff00) >> 3; // 原本要右移5左移2,简化成右移3
							SB = (nColorSource & 0x0000ff) << 3;
							OR = (nColorOverlayer & 0xff0000) >> 8;
							OG = (nColorOverlayer & 0x00ff00) >> 3;
							OB = (nColorOverlayer & 0x0000ff) << 3;
							//								SR = (((SR + OR) > 0x00ff ? 0x00ff:SR+OR) << 8) & 0xf800;
							//								SG = (((SG + OG) > 0x00ff ? 0x00ff:SG+OG) << 3) & 0x07e0;
							//								SB = (((SB + OB) > 0x00ff ? 0x00ff:SB+OB) >> 3) & 0x001f;
							SR = ((OR > SR ? OR:SR) << 8) & 0xff0000;
							SG = ((OG > SG ? OG:SG) << 3) & 0x00ff00;
							SB = ((OB > SB ? OB:SB) >> 3) & 0x0000ff;
							*(DWORD*)ptSourceDest32 = (DWORD)(SB | SG | SR);
						}
						ptSourceDest32 = (DWORD*)ptSourceDest32 + 1;
						ptOverLayerDest32 = (DWORD*)ptOverLayerDest32 + 1;
					}
					ptSourceDest32 = (DWORD*)ptSourceDest32 + surfacePitch1 - w;
					ptOverLayerDest32 = (DWORD*)ptOverLayerDest32 + surfacePitch2 - w;
				}
			}else
#endif
			{
				for(j=0;j<h;j++){
					for(i=0;i<w;i++){
						nColorSource = *(WORD*)ptSourceDest;
						nColorOverlayer = *(WORD*)ptOverLayerDest;
						if(nColorOverlayer != DEF_COLORKEY){
							SR = (nColorSource & 0xf800) >> 8; // 原本要右移11左移3,简化成右移8
							SG = (nColorSource & 0x07e0) >> 3; // 原本要右移5左移2,简化成右移3
							SB = (nColorSource & 0x001f) << 3;
							OR = (nColorOverlayer & 0xf800) >> 8;
							OG = (nColorOverlayer & 0x07e0) >> 3;
							OB = (nColorOverlayer & 0x001f) << 3;
							//								SR = (((SR + OR) > 0x00ff ? 0x00ff:SR+OR) << 8) & 0xf800;
							//								SG = (((SG + OG) > 0x00ff ? 0x00ff:SG+OG) << 3) & 0x07e0;
							//								SB = (((SB + OB) > 0x00ff ? 0x00ff:SB+OB) >> 3) & 0x001f;
							SR = ((OR > SR ? OR:SR) << 8) & 0xf800;
							SG = ((OG > SG ? OG:SG) << 3) & 0x07e0;
							SB = ((OB > SB ? OB:SB) >> 3) & 0x001f;
							*(WORD*)ptSourceDest = (WORD)(SB | SG | SR);
						}
						ptSourceDest = (WORD*)ptSourceDest + 1;
						ptOverLayerDest = (WORD*)ptOverLayerDest + 1;
					}
					ptSourceDest = (WORD*)ptSourceDest + surfacePitch1 - w;
					ptOverLayerDest = (WORD*)ptOverLayerDest + surfacePitch2 - w;
				}
			}
		}
		// 555 显示模式
		else{
#ifdef _HI_COLOR_32
			if( displayBpp == 32 ){
				for(j=0;j<h;j++){
					for(i=0;i<w;i++){
						nColorSource = *(DWORD*)ptSourceDest32;
						nColorOverlayer = *(DWORD*)ptOverLayerDest32;
						if(nColorOverlayer != DEF_COLORKEY){
							SR = (nColorSource & 0xff0000) >> 7;							// 原本要右移10左移3,简化成右移7
							SG = (nColorSource & 0x00ff00) >> 2;	// 原本要右移5左移3,简化成右移2
							SB = (nColorSource & 0x0000ff) << 3;
							OR = (nColorSource & 0xff0000) >> 7;
							OG = (nColorOverlayer & 0x00ff00) >> 2;
							OB = (nColorOverlayer & 0x0000ff) << 3;
							//								SR = ((SR + OR) & 0x00ff)<< 7;
							//								SG = (((SG + OG) & 0x00ff)<< 2) & 0x03e0;
							//								SB = (((SB + OB) & 0x00ff)>> 3) & 0x001f;
							SR = (((OR > SR ? OR:SR) & 0x00ff)<< 7) & 0xff0000;
							SG = (((OG > SG ? OG:SG) & 0x00ff)<< 2) & 0x00ff00;
							SB = (((OB > SB ? OB:SB) & 0x00ff)>> 3) & 0x0000ff;
							*(DWORD*)ptSourceDest32 = (DWORD)(SB | SG | SR);
						}
						ptSourceDest32 = (DWORD*)ptSourceDest32 + 1;
						ptOverLayerDest = (DWORD*)ptOverLayerDest32 + 1;
					}
					ptSourceDest32 = (DWORD*)ptSourceDest32 + surfacePitch1 - w;
					ptOverLayerDest = (DWORD*)ptOverLayerDest32 + surfacePitch2 - w;
				}
			}else
#endif
			{
				for(j=0;j<h;j++){
					for(i=0;i<w;i++){
						nColorSource = *(WORD*)ptSourceDest;
						nColorOverlayer = *(WORD*)ptOverLayerDest;
						if(nColorOverlayer != DEF_COLORKEY){
							SR = nColorSource >> 7;							// 原本要右移10左移3,简化成右移7
							SG = (nColorSource & 0x03e0) >> 2;	// 原本要右移5左移3,简化成右移2
							SB = (nColorSource & 0x001f) << 3;
							OR = nColorOverlayer >> 7;
							OG = (nColorOverlayer & 0x03e0) >> 2;
							OB = (nColorOverlayer & 0x001f) << 3;
							//								SR = ((SR + OR) & 0x00ff)<< 7;
							//								SG = (((SG + OG) & 0x00ff)<< 2) & 0x03e0;
							//								SB = (((SB + OB) & 0x00ff)>> 3) & 0x001f;
							SR = ((OR > SR ? OR:SR) & 0x00ff)<< 7;
							SG = (((OG > SG ? OG:SG) & 0x00ff)<< 2) & 0x03e0;
							SB = (((OB > SB ? OB:SB) & 0x00ff)>> 3) & 0x001f;
							*(WORD*)ptSourceDest = (WORD)(SB | SG | SR);
						}
						ptSourceDest = (WORD*)ptSourceDest + 1;
						ptOverLayerDest = (WORD*)ptOverLayerDest + 1;
					}
					ptSourceDest = (WORD*)ptSourceDest + surfacePitch1 - w;
					ptOverLayerDest = (WORD*)ptOverLayerDest + surfacePitch2 - w;
				}
			}
		}
	}
	lpDraw->lpBACKBUFFERSYS->Unlock(NULL);
	lpSurface->Unlock(NULL);
	rect.left = ox;
	rect.top = oy;
	rect.right = ox+w;
	rect.bottom = oy+h;
	lpDraw->lpBACKBUFFER->BltFast(ox,oy,lpDraw->lpBACKBUFFERSYS,&rect,DDBLTFAST_WAIT);

	return;
}

void DrawGray(LPDIRECTDRAWSURFACE lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne,int iGrayType){
	static DDSURFACEDESC ddsdSource,ddsdOverlayer;
	int surfacePitch1,surfacePitch2,nColorOverlayer;
	int i,j,w = SURFACE_WIDTH,h = SURFACE_HEIGHT,subx,dx = 0,dy = 0;
	int R,G,B,Gray,g;
	bool odd = false;
	RECT rect;

	if(ox >= lpDraw->xSize || ox + w <= 0 || oy >= lpDraw->ySize || oy + h <= 0 ) return;
	if(bLastOne){
		w = ow - offsetx;
		h = oh - offsety;
	}
	if(ox + w >= lpDraw->xSize){
		w = lpDraw->xSize - ox;
	}
	if(oy + h >= lpDraw->ySize) h = lpDraw->ySize - oy;
	if(ox < 0){
		w = w + ox;
		if(w < 0) return;
		dx = ox*-1;
		ox = 0;
	}
	if(oy < 0){
		h = h + oy;
		if(h < 0) return;
		dy = oy*-1;
		oy = 0;
	}
	subx = ox;
	
	if(iGrayType == 3) g = 6;
	else if(iGrayType == 4)	g = 5;

	ZeroMemory(&ddsdSource,sizeof(DDSURFACEDESC));
	ddsdSource.dwSize = sizeof(DDSURFACEDESC);
	ZeroMemory(&ddsdOverlayer,sizeof(DDSURFACEDESC));
	ddsdOverlayer.dwSize = sizeof(DDSURFACEDESC);
	if(lpDraw->lpBACKBUFFERSYS->Lock(NULL,&ddsdSource,DDLOCK_WAIT,NULL) != DD_OK) return;
	if(lpSurface->Lock(NULL,&ddsdOverlayer,DDLOCK_WAIT,NULL) != DD_OK){
		lpDraw->lpBACKBUFFERSYS->Unlock(NULL);
		return;
	}
#ifdef _HI_COLOR_32
	if( displayBpp == 32 ){
		void *ptSourceDest,*ptOverLayerDest;
		surfacePitch1 = ddsdSource.lPitch >> 2;
		surfacePitch2 = ddsdOverlayer.lPitch >> 2;
		ptSourceDest = (DWORD*)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
		ptOverLayerDest = (DWORD*)(ddsdOverlayer.lpSurface) +dy * surfacePitch2 + dx;

		// 565 显示模式
		if(gBitRShift == 2){
			for(j=0;j<h;j++){
				if(oy >= 0){
					for(i=0;i<w;i++){
						if(subx >= 0){
							nColorOverlayer = *(DWORD*)ptOverLayerDest;
							if(nColorOverlayer != DEF_COLORKEY){
								R = (nColorOverlayer & 0xff0000) >> 16;
								G = (nColorOverlayer & 0x00ff00) >> 8;
								B = (nColorOverlayer & 0x0000ff);
								Gray = (R*3+G*6+B)/10;
								*(DWORD*)ptSourceDest = (DWORD)(Gray << 16 | Gray << 8 | Gray);
							}
						}
						else subx++;
						ptSourceDest = (DWORD*)ptSourceDest + 1;
						ptOverLayerDest = (DWORD*)ptOverLayerDest + 1;
					}
					ptSourceDest = (DWORD*)ptSourceDest + surfacePitch1 - w;
					ptOverLayerDest = (DWORD*)ptOverLayerDest + surfacePitch2 - w;
					subx = ox;
				}
				else{
					ptSourceDest = (DWORD*)ptSourceDest + surfacePitch1;
					ptOverLayerDest = (DWORD*)ptOverLayerDest + surfacePitch2;
					oy++;
				}
			}
		}
		// 555 显示模式
		else{
			for(j=0;j<h;j++){
				if(oy >= 0){
					for(i=0;i<w;i++){
						if(subx >= 0){
							nColorOverlayer = *(DWORD*)ptOverLayerDest;
							if(nColorOverlayer != DEF_COLORKEY){
								R = (nColorOverlayer & 0xff0000) >> 16;
								G = (nColorOverlayer & 0x00ff00) >> 8;
								B = (nColorOverlayer & 0x0000ff);

								Gray = (R*3+G*5+B)/10;
								*(DWORD*)ptSourceDest = (DWORD)(Gray << 16 | Gray << 8 | Gray);
							}
						}
						else subx++;
						ptSourceDest = (DWORD*)ptSourceDest + 1;
						ptOverLayerDest = (DWORD*)ptOverLayerDest + 1;
					}
					ptSourceDest = (DWORD*)ptSourceDest + surfacePitch1 - w;
					ptOverLayerDest = (DWORD*)ptOverLayerDest + surfacePitch2 - w;
					subx = ox;
				}
				else{
					ptSourceDest = (DWORD*)ptSourceDest + surfacePitch1;
					ptOverLayerDest = (DWORD*)ptOverLayerDest + surfacePitch2;
					oy++;
				}
			}
		}
	}else
#endif
	if(displayBpp == 16){
		void *ptSourceDest,*ptOverLayerDest;
		surfacePitch1 = ddsdSource.lPitch >> 1;
		surfacePitch2 = ddsdOverlayer.lPitch >> 1;
		ptSourceDest = (WORD*)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
		ptOverLayerDest = (WORD*)(ddsdOverlayer.lpSurface) +dy * surfacePitch2 + dx;
		
		// 565 显示模式
		if(gBitRShift == 2){
			for(j=0;j<h;j++){
				if(oy >= 0){
					for(i=0;i<w;i++){
						if(subx >= 0){
							nColorOverlayer = *(WORD*)ptOverLayerDest;
							if(nColorOverlayer != DEF_COLORKEY){
								R = (nColorOverlayer & 0xf800) >> 11;
								G = (nColorOverlayer & 0x07e0) >> 6;
								B = nColorOverlayer & 0x001f;
								Gray = (R*3+G*6+B)/10;
								*(WORD*)ptSourceDest = (WORD)(Gray << 11 | Gray << g | Gray);
							}
						}
						else subx++;
						ptSourceDest = (WORD*)ptSourceDest + 1;
						ptOverLayerDest = (WORD*)ptOverLayerDest + 1;
					}
					ptSourceDest = (WORD*)ptSourceDest + surfacePitch1 - w;
					ptOverLayerDest = (WORD*)ptOverLayerDest + surfacePitch2 - w;
					subx = ox;
				}
				else{
					ptSourceDest = (WORD*)ptSourceDest + surfacePitch1;
					ptOverLayerDest = (WORD*)ptOverLayerDest + surfacePitch2;
					oy++;
				}
			}
		}
		// 555 显示模式
		else{
			for(j=0;j<h;j++){
				if(oy >= 0){
					for(i=0;i<w;i++){
						if(subx >= 0){
							nColorOverlayer = *(WORD*)ptOverLayerDest;
							if(nColorOverlayer != DEF_COLORKEY){
								R = nColorOverlayer >> 10;
								G = (nColorOverlayer & 0x03e0) >> 5;
								B = nColorOverlayer & 0x001f;
								Gray = (R*3+G*6+B)/10;
								*(WORD*)ptSourceDest = (WORD)(Gray << 10 | Gray << g | Gray);
							}
						}
						else subx++;
						ptSourceDest = (WORD*)ptSourceDest + 1;
						ptOverLayerDest = (WORD*)ptOverLayerDest + 1;
					}
					ptSourceDest = (WORD*)ptSourceDest + surfacePitch1 - w;
					ptOverLayerDest = (WORD*)ptOverLayerDest + surfacePitch2 - w;
					subx = ox;
				}
				else{
					ptSourceDest = (WORD*)ptSourceDest + surfacePitch1;
					ptOverLayerDest = (WORD*)ptOverLayerDest + surfacePitch2;
					oy++;
				}
			}
		}
	}
	lpSurface->Unlock(NULL);
	lpDraw->lpBACKBUFFERSYS->Unlock(NULL);
	rect.left = ox;
	rect.top = oy;
	rect.right = ox+w;
	rect.bottom = oy+h;
	lpDraw->lpBACKBUFFER->BltFast(ox,oy,lpDraw->lpBACKBUFFERSYS,&rect,DDBLTFAST_WAIT);
	
	return;
}

void DrawAlphaChannel(SURFACE_INFO *surface_info,BYTE *AlphaData,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne){
	static DDSURFACEDESC ddsdSource,ddsdOverlayer;
	int surfacePitch1,surfacePitch2;
	int w = SURFACE_WIDTH,h = SURFACE_HEIGHT,dx = 0,dy = 0;
	DWORD Is4Multiple;
	RECT rect;
	if(ox >= lpDraw->xSize || ox + w <= 0 || oy >= lpDraw->ySize || oy + h <= 0 ) return;
	if(bLastOne){
		w = ow - offsetx;
		h = oh - offsety;
	}
	if(ox + w >= lpDraw->xSize){
		w = lpDraw->xSize - ox;
	}
	if(oy + h >= lpDraw->ySize) h = lpDraw->ySize - oy;
	if(ox < 0){
		w = w + ox;
		if(w < 0) return;
		dx = ox*-1;
		ox = 0;
	}
	if(oy < 0){
		h = h + oy;
		if(h < 0) return;
		dy = oy*-1;
		oy = 0;
	}
	Is4Multiple = w % 4;
	ZeroMemory(&ddsdSource,sizeof(DDSURFACEDESC));
	ddsdSource.dwSize = sizeof(DDSURFACEDESC);
	ZeroMemory(&ddsdOverlayer,sizeof(DDSURFACEDESC));
	ddsdOverlayer.dwSize = sizeof(DDSURFACEDESC);
	if(lpDraw->lpBACKBUFFERSYS->Lock(NULL,&ddsdSource,DDLOCK_WAIT,NULL) != DD_OK) return;
	if(surface_info->lpSurface->Lock(NULL,&ddsdOverlayer,DDLOCK_WAIT | DDLOCK_READONLY ,NULL) != DD_OK){
		lpDraw->lpBACKBUFFERSYS->Unlock(&ddsdSource);
		return;
	}
	void *ptSourceDest,*ptOverLayerDest;
#ifdef _HI_COLOR_32
	void *ptSourceDest32,*ptOverLayerDest32;
	if( displayBpp == 32){
		surfacePitch1 = ddsdSource.lPitch >> 2;
		surfacePitch2 = ddsdOverlayer.lPitch >> 2;
		ptSourceDest32 = (int *)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
		ptOverLayerDest32 = (int *)(ddsdOverlayer.lpSurface) + dy * surfacePitch2 + dx;
	}else
#endif
	{
		surfacePitch1 = ddsdSource.lPitch >> 1;
		surfacePitch2 = ddsdOverlayer.lPitch >> 1;
		ptSourceDest = (WORD*)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
		ptOverLayerDest = (WORD*)(ddsdOverlayer.lpSurface) + dy * surfacePitch2 + dx;
	}

	int i,j,nColorSource,nColorOverlayer;
	DWORD SR,SG,SB,OR,OG,OB,Alpha;
	// 565 显示模式
	if(gBitRShift == 2){
#ifdef _HI_COLOR_32
		if( displayBpp == 32 ){
			for(j=0;j<h;j++){
				for(i=0;i<w;i++){
					nColorSource = *(DWORD*)ptSourceDest32;
					nColorOverlayer = *(DWORD*)ptOverLayerDest32;
					Alpha = (DWORD)(*AlphaData);
					if(nColorOverlayer != DEF_COLORKEY){
						SR = (nColorSource & 0xff0000) >> 8; // 原本要右移11左移3,简化成右移8
						SG = (nColorSource & 0x00ff00) >> 3; // 原本要右移5左移2,简化成右移3
						SB = (nColorSource & 0x0000ff) << 3;
						OR = (nColorOverlayer & 0xff0000) >> 8;
						OG = (nColorOverlayer & 0x00ff00) >> 3;
						OB = (nColorOverlayer & 0x0000ff) << 3;
						SR = (((((OR - SR) * Alpha) >> 5) + SR) << 8) & 0xff0000;
						SG = (((((OG - SG) * Alpha) >> 5) + SG) << 3) & 0x00ff00;
						SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x0000ff;
						*(DWORD*)ptSourceDest32 = (DWORD)(SB | SG | SR);
					}
					ptSourceDest32 = (DWORD*)ptSourceDest32 + 1;
					ptOverLayerDest32 = (DWORD*)ptOverLayerDest32 + 1;
					AlphaData++;
				}
				ptSourceDest32 = (DWORD*)ptSourceDest32 + surfacePitch1 - w;
				ptOverLayerDest32 = (DWORD*)ptOverLayerDest32 + surfacePitch2 - w;
				AlphaData += SURFACE_WIDTH - w;
			}
		}else
#endif
			for(j=0;j<h;j++){
				for(i=0;i<w;i++){
					nColorSource = *(WORD*)ptSourceDest;
					nColorOverlayer = *(WORD*)ptOverLayerDest;
					Alpha = (WORD)(*AlphaData);
					if(nColorOverlayer != DEF_COLORKEY){
						SR = (nColorSource & 0xf800) >> 8; // 原本要右移11左移3,简化成右移8
						SG = (nColorSource & 0x07e0) >> 3; // 原本要右移5左移2,简化成右移3
						SB = (nColorSource & 0x001f) << 3;
						OR = (nColorOverlayer & 0xf800) >> 8;
						OG = (nColorOverlayer & 0x07e0) >> 3;
						OB = (nColorOverlayer & 0x001f) << 3;
						SR = (((((OR - SR) * Alpha) >> 5) + SR) << 8) & 0xf800;
						SG = (((((OG - SG) * Alpha) >> 5) + SG) << 3) & 0x07e0;
						SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x001f;
						*(WORD*)ptSourceDest = (WORD)(SB | SG | SR);
					}
					ptSourceDest = (WORD*)ptSourceDest + 1;
					ptOverLayerDest = (WORD*)ptOverLayerDest + 1;
					AlphaData++;
				}
				ptSourceDest = (WORD*)ptSourceDest + surfacePitch1 - w;
				ptOverLayerDest = (WORD*)ptOverLayerDest + surfacePitch2 - w;
				AlphaData += SURFACE_WIDTH - w;
			}
	}
	// 555 显示模式
	else{
#ifdef _HI_COLOR_32
		if( displayBpp == 32 ){
			for(j=0;j<h;j++){
				for(i=0;i<w;i++){
					nColorSource = *(DWORD*)ptSourceDest32;
					nColorOverlayer = *(DWORD*)ptOverLayerDest32;
					Alpha = (DWORD)(*AlphaData);
					if(nColorOverlayer != DEF_COLORKEY){
						SR = (nColorSource & 0xff0000) >> 8; // 原本要右移11左移3,简化成右移8
						SG = (nColorSource & 0x00ff00) >> 3; // 原本要右移5左移2,简化成右移3
						SB = (nColorSource & 0x0000ff) << 3;
						OR = (nColorOverlayer & 0xff0000) >> 8;
						OG = (nColorOverlayer & 0x00ff00) >> 3;
						OB = (nColorOverlayer & 0x0000ff) << 3;
						SR = (((((OR - SR) * Alpha) >> 5) + SR) << 8) & 0xff0000;
						SG = (((((OG - SG) * Alpha) >> 5) + SG) << 3) & 0x00ff00;
						SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x0000ff;
						*(DWORD*)ptSourceDest32 = (DWORD)(SB | SG | SR);
					}
					ptSourceDest32 = (DWORD*)ptSourceDest32 + 1;
					ptOverLayerDest32 = (DWORD*)ptOverLayerDest32 + 1;
					AlphaData++;
				}
				ptSourceDest32 = (DWORD*)ptSourceDest32 + surfacePitch1 - w;
				ptOverLayerDest32 = (DWORD*)ptOverLayerDest32 + surfacePitch2 - w;
				AlphaData += SURFACE_WIDTH - w;
			}
		}else
#endif

		{
			for(j=0;j<h;j++){
				for(i=0;i<w;i++){
					nColorSource = *(WORD*)ptSourceDest;
					nColorOverlayer = *(WORD*)ptOverLayerDest;
					Alpha = (WORD)(*AlphaData);
					if(nColorOverlayer != DEF_COLORKEY){
						SR = nColorSource >> 7;							// 原本要右移10左移3,简化成右移7
						SG = (nColorSource & 0x03e0) >> 2;	// 原本要右移5左移3,简化成右移2
						SB = (nColorSource & 0x001f) << 3;
						OR = nColorOverlayer >> 7;
						OG = (nColorOverlayer & 0x03e0) >> 2;
						OB = (nColorOverlayer & 0x001f) << 3;
						SR = ((((OR - SR) * Alpha) >> 5) + SR) << 7;
						SG = (((((OG - SG) * Alpha) >> 5) + SG) << 2) & 0x03e0;
						SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x001f;
						*(WORD*)ptSourceDest = (WORD)(SB | SG | SR);
					}
					ptSourceDest = (WORD*)ptSourceDest + 1;
					ptOverLayerDest = (WORD*)ptOverLayerDest + 1;
					AlphaData++;
				}
				ptSourceDest = (DWORD*)ptSourceDest + surfacePitch1 - w;
				ptOverLayerDest = (DWORD*)ptOverLayerDest + surfacePitch2 - w;
				AlphaData += SURFACE_WIDTH - w;
			}
		}

	}
	lpDraw->lpBACKBUFFERSYS->Unlock(&ddsdSource);
	surface_info->lpSurface->Unlock(&ddsdOverlayer);
	rect.left = ox;
	rect.top = oy;
	rect.right = ox+w;
	rect.bottom = oy+h;
	lpDraw->lpBACKBUFFER->BltFast(ox,oy,lpDraw->lpBACKBUFFERSYS,&rect,DDBLTFAST_WAIT);
	return;
}
#endif

#ifdef _SFUMATO
void DrawSfumato(LPDIRECTDRAWSURFACE lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne,int iGrayType, int sfumato){
	static DDSURFACEDESC ddsdSource,ddsdOverlayer;
	int surfacePitch1,surfacePitch2,nColorOverlayer;
	int i,j,w = SURFACE_WIDTH,h = SURFACE_HEIGHT,subx,dx = 0,dy = 0;
	int R,G,B,Gray,g;
	bool odd = false;
	RECT rect;

	if(ox >= lpDraw->xSize || ox + w <= 0 || oy >= lpDraw->ySize || oy + h <= 0 ) return;
	if(bLastOne){
		w = ow - offsetx;
		h = oh - offsety;
	}
	if(ox + w >= lpDraw->xSize){
		w = lpDraw->xSize - ox;
	}
	if(oy + h >= lpDraw->ySize) h = lpDraw->ySize - oy;
	if(ox < 0){
		w = w + ox;
		if(w < 0) return;
		dx = ox*-1;
		ox = 0;
	}
	if(oy < 0){
		h = h + oy;
		if(h < 0) return;
		dy = oy*-1;
		oy = 0;
	}
	subx = ox;

	if(iGrayType == 3) g = 6;
	else if(iGrayType == 4)	g = 5;

	ZeroMemory(&ddsdSource,sizeof(DDSURFACEDESC));
	ddsdSource.dwSize = sizeof(DDSURFACEDESC);
	ZeroMemory(&ddsdOverlayer,sizeof(DDSURFACEDESC));
	ddsdOverlayer.dwSize = sizeof(DDSURFACEDESC);
	if(lpDraw->lpBACKBUFFERSYS->Lock(NULL,&ddsdSource,DDLOCK_WAIT,NULL) != DD_OK) return;
	if(lpSurface->Lock(NULL,&ddsdOverlayer,DDLOCK_WAIT,NULL) != DD_OK){
		lpDraw->lpBACKBUFFERSYS->Unlock(NULL);
		return;
	}
#ifdef _HI_COLOR_32
	if( displayBpp == 32 ){
		void *ptSourceDest,*ptOverLayerDest;
		surfacePitch1 = ddsdSource.lPitch >> 2;
		surfacePitch2 = ddsdOverlayer.lPitch >> 2;
		ptSourceDest = (DWORD*)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
		ptOverLayerDest = (DWORD*)(ddsdOverlayer.lpSurface) +dy * surfacePitch2 + dx;

		// 565 显示模式
		if(gBitRShift == 2){
			for(j=0;j<h;j++){
				if(oy >= 0){
					for(i=0;i<w;i++){
						if(subx >= 0){
							nColorOverlayer = *(DWORD*)ptOverLayerDest;
							if(nColorOverlayer != DEF_COLORKEY){
								R = (nColorOverlayer & 0xff0000) >> 16;
								G = (nColorOverlayer & 0x00ff00) >> 8;
								B = (nColorOverlayer & 0x0000ff);
								Gray = (R*3+G*6+B)/10;
								*(DWORD*)ptSourceDest = (DWORD)(Gray << 16 | Gray << 8 | Gray);
							}
						}
						else subx++;
						ptSourceDest = (DWORD*)ptSourceDest + 1;
						ptOverLayerDest = (DWORD*)ptOverLayerDest + 1;
					}
					ptSourceDest = (DWORD*)ptSourceDest + surfacePitch1 - w;
					ptOverLayerDest = (DWORD*)ptOverLayerDest + surfacePitch2 - w;
					subx = ox;
				}
				else{
					ptSourceDest = (DWORD*)ptSourceDest + surfacePitch1;
					ptOverLayerDest = (DWORD*)ptOverLayerDest + surfacePitch2;
					oy++;
				}
			}
		}
		// 555 显示模式
		else{
			for(j=0;j<h;j++){
				if(oy >= 0){
					for(i=0;i<w;i++){
						if(subx >= 0){
							nColorOverlayer = *(DWORD*)ptOverLayerDest;
							if(nColorOverlayer != DEF_COLORKEY){
								R = (nColorOverlayer & 0xff0000) >> 16;
								G = (nColorOverlayer & 0x00ff00) >> 8;
								B = (nColorOverlayer & 0x0000ff);


								if(sfumato > 0){
									int x = G - R;
									int y = G - B;
									if((R > 0x30 && G > 0x30 && B > 0x30)/* &&(R < 0xf0 && G < 0xf0 && B < 0xf0) */&& (x > -40 && x < 40) && (y > -40 && y < 40)){
										R = (nColorOverlayer & 0xff0000) >> 8; // 原本要右移11左移3,简化成右移8
										G = (nColorOverlayer & 0x00ff00) >> 3; // 原本要右移5左移2,简化成右移3
										B = (nColorOverlayer & 0x0000ff) << 3;

										int OR = (sfumato & 0xff0000) >> 8;
										int OG = (sfumato & 0x00ff00) >> 3;
										int OB = (sfumato & 0x0000ff) << 3;

										R = (((((OR - R) * 12) >> 5) + R) << 8) & 0xff0000;
										G = (((((OG - G) * 12) >> 5) + G) << 3) & 0x00ff00;
										B = (((((OB - B) * 12) >> 5) + B) >> 3) & 0x0000ff;
										*(DWORD*)ptSourceDest = (DWORD)(B | G | R);

									}else{
										*(DWORD*)ptSourceDest = nColorOverlayer;
									}
								}else{
									*(DWORD*)ptSourceDest = nColorOverlayer;
								}
							}
						}
						else subx++;
						ptSourceDest = (DWORD*)ptSourceDest + 1;
						ptOverLayerDest = (DWORD*)ptOverLayerDest + 1;
					}
					ptSourceDest = (DWORD*)ptSourceDest + surfacePitch1 - w;
					ptOverLayerDest = (DWORD*)ptOverLayerDest + surfacePitch2 - w;
					subx = ox;
				}
				else{
					ptSourceDest = (DWORD*)ptSourceDest + surfacePitch1;
					ptOverLayerDest = (DWORD*)ptOverLayerDest + surfacePitch2;
					oy++;
				}
			}
		}
	}else
#endif
		if(displayBpp == 16){
			void *ptSourceDest,*ptOverLayerDest;
			surfacePitch1 = ddsdSource.lPitch >> 1;
			surfacePitch2 = ddsdOverlayer.lPitch >> 1;
			ptSourceDest = (WORD*)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
			ptOverLayerDest = (WORD*)(ddsdOverlayer.lpSurface) +dy * surfacePitch2 + dx;

			// 565 显示模式
			if(gBitRShift == 2){
				for(j=0;j<h;j++){
					if(oy >= 0){
						for(i=0;i<w;i++){
							if(subx >= 0){
								nColorOverlayer = *(WORD*)ptOverLayerDest;
								if(nColorOverlayer != DEF_COLORKEY){
									R = (nColorOverlayer & 0xf800) >> 11;
									G = (nColorOverlayer & 0x07e0) >> 6;
									B = nColorOverlayer & 0x001f;
									Gray = (R*3+G*6+B)/10;
									*(WORD*)ptSourceDest = (WORD)(Gray << 11 | Gray << g | Gray);
								}
							}
							else subx++;
							ptSourceDest = (WORD*)ptSourceDest + 1;
							ptOverLayerDest = (WORD*)ptOverLayerDest + 1;
						}
						ptSourceDest = (WORD*)ptSourceDest + surfacePitch1 - w;
						ptOverLayerDest = (WORD*)ptOverLayerDest + surfacePitch2 - w;
						subx = ox;
					}
					else{
						ptSourceDest = (WORD*)ptSourceDest + surfacePitch1;
						ptOverLayerDest = (WORD*)ptOverLayerDest + surfacePitch2;
						oy++;
					}
				}
			}
			// 555 显示模式
			else{
				for(j=0;j<h;j++){
					if(oy >= 0){
						for(i=0;i<w;i++){
							if(subx >= 0){
								nColorOverlayer = *(WORD*)ptOverLayerDest;
								if(nColorOverlayer != DEF_COLORKEY){
									R = nColorOverlayer >> 10;
									G = (nColorOverlayer & 0x03e0) >> 5;
									B = nColorOverlayer & 0x001f;
									Gray = (R*3+G*6+B)/10;
									*(WORD*)ptSourceDest = (WORD)(Gray << 10 | Gray << g | Gray);
								}
							}
							else subx++;
							ptSourceDest = (WORD*)ptSourceDest + 1;
							ptOverLayerDest = (WORD*)ptOverLayerDest + 1;
						}
						ptSourceDest = (WORD*)ptSourceDest + surfacePitch1 - w;
						ptOverLayerDest = (WORD*)ptOverLayerDest + surfacePitch2 - w;
						subx = ox;
					}
					else{
						ptSourceDest = (WORD*)ptSourceDest + surfacePitch1;
						ptOverLayerDest = (WORD*)ptOverLayerDest + surfacePitch2;
						oy++;
					}
				}
			}
		}
		lpSurface->Unlock(NULL);
		lpDraw->lpBACKBUFFERSYS->Unlock(NULL);
		rect.left = ox;
		rect.top = oy;
		rect.right = ox+w;
		rect.bottom = oy+h;
		lpDraw->lpBACKBUFFER->BltFast(ox,oy,lpDraw->lpBACKBUFFERSYS,&rect,DDBLTFAST_WAIT);

		return;
}
#endif

void ablend_565(unsigned char *lpAlpha, unsigned int iAlpPitch,
	unsigned char *lpSrc, unsigned int iSrcX, unsigned int iSrcY,
	unsigned int iSrcPitch, unsigned char *lpDst,
	unsigned int iDstX, unsigned int iDstY,
	unsigned int iDstW, unsigned int iDstH,
	unsigned int iDstPitch)
{
	//Mask for isolating the red,green, and blue components
	static __int64 MASKB = 0x001F001F001F001F;
	static __int64 MASKG = 0x07E007E007E007E0;
	static __int64 MASKSHIFTG = 0x03F003F003F003F0;
	static __int64 MASKR = 0xF800F800F800F800;
	static __int64 SIXTEEN = 0x0010001000100010;
	static __int64 FIVETWELVE = 0x0200020002000200;
	static __int64 SIXONES = 0x003F003F003F003F;
	unsigned char *lpLinearDstBp = (iDstX << 1) + (iDstY*iDstPitch) + lpDst; 
	unsigned char *lpLinearSrcBp = (iSrcX << 1) + (iSrcY*iSrcPitch) + lpSrc;
	unsigned char *lpLinearAlpBp = iSrcX + (iSrcY*iAlpPitch) + lpAlpha;
	_asm{
		mov esi, lpLinearSrcBp;
		mov edi, lpLinearDstBp;
		mov eax, lpLinearAlpBp;
		mov ecx, iDstH; 
		mov ebx, iDstW;
		test esi, 6; 
		jnz done; 
	primeloop:
		movd mm1, [eax];
		pxor mm2, mm2;
		movq mm4, [esi];
		punpcklbw mm1, mm2;
	loopqword:
		mov edx, [eax];
		test ebx, 0xFFFFFFFC;
		jz checkback;
		cmp edx, 0xffffffff;
		je copyback;
		test edx, 0xffffffff;
		jz leavefront; 
		movq mm5, [edi];
		psrlw mm1, 2;
		movq mm7, MASKSHIFTG;
		psrlw mm4, 1;
		movq mm0, mm1;
		psrlw mm5, 1;
		psrlw mm1, 1;
		pand mm4, mm7;
		movq mm2, SIXONES;
		pand mm5, mm7;
		movq mm3, [esi];
		psubsb mm2, mm0;
		movq mm7, MASKB;
		pmullw mm4, mm0;
		movq mm0, [edi];
		pmullw mm5, mm2;
		movq mm2, mm7;
		pand mm3, mm7;
		pmullw mm3, mm1;
		pand mm0, mm7;
		movq mm7, [esi];
		paddw mm4, mm5;
		pand mm7, MASKR;
		psubsb mm2, mm1;
		paddw mm4, FIVETWELVE;
		pmullw mm0, mm2;
		movq mm5, mm4;
		psrlw mm7, 11;
		psrlw mm4, 6;
		paddw mm4, mm5;
		paddw mm0, mm3;
		movq mm5, [edi];
		paddw mm0, SIXTEEN;
		pand mm5, MASKR;
		psrlw mm4, 5;
		movq mm3, mm0;
		psrlw mm0, 5;
		psrlw mm5, 11;
		paddw mm0, mm3;
		psrlw mm0, 5;
		pmullw mm7, mm1; //mm7=sr?*a?

		pand mm4, MASKG; //g16: mm4=00g0 00g0 00g0 00g0 green

		pmullw mm5, mm2; //r7: mm5=dr?*(31-a?)

		por mm0, mm4; //mm0=00gb 00gb 00gb 00gb

		add eax, 4; //move to next 4 alphas

		add esi, 8; //move to next 4 pixels in src

		add edi, 8; //move to next 4 pixels in dst

		movd mm1, [eax]; //mm1=00 00 00 00 a3 a2 a1 a0

		paddw mm5, mm7; //r8: mm5=sr?*a?+dr?*(31-a?)

		paddw mm5, SIXTEEN; //r9: mm5=(mm5+16) red

		pxor mm2, mm2; //mm2=0;

		movq mm7, mm5; //r10: mm7=mm5 red

		psrlw mm5, 5; //r11: mm5=mm5>>5 red

		movq mm4, [esi]; //g1: mm4=src3 src2 src1 src0

		paddw mm5, mm7; //r12: mm5=mm7+mm5 red

		punpcklbw mm1, mm2; //mm1=00a3 00a2 00a1 00a0

		psrlw mm5, 5; //r13: mm5=mm5>>5 red

		psllw mm5, 11; //r14: mm5=mm5<<10 red

		por mm0, mm5; //mm0=0rgb 0rgb 0rgb 0rgb

		sub ebx, 4; //polished off 4 pixels

		movq[edi - 8], mm0; //dst=0rgb 0rgb 0rgb 0rgb

		jmp loopqword; //go back to start

	copyback:

		movq[edi], mm4; //copy source to destination

	leavefront:

		add edi, 8; //advance destination by 4 pixels

		add eax, 4; //advance alpha by 4

		add esi, 8; //advance source by 4 pixels

		sub ebx, 4; //decrease pixel count by 4

		jmp primeloop;

	checkback:

		test ebx, 0xFF; //check if 0 pixels left

		jz nextline; //done with this span

		//backalign: //work out back end pixels

		movq mm5, [edi]; //g2: mm5=dst3 dst2 dst1 dst0

		psrlw mm1, 2; //mm1=a?>>2 nuke out lower 2 bits

		movq mm7, MASKSHIFTG; //g3: mm7=shift 1 bit green mask

		psrlw mm4, 1; //g3a: move src green down by 1 so that we won't overflow

		movq mm0, mm1; //mm0=00a3 00a2 00a1 00a0

		psrlw mm5, 1; //g3b: move dst green down by 1 so that we won't overflow

		psrlw mm1, 1; //mm1=a?>>1 nuke out lower 1 bits

		pand mm4, mm7; //g5: mm4=sg3 sg2 sg1 sg0

		movq mm2, SIXONES;//g4: mm2=63

		pand mm5, mm7; //g7: mm5=dg3 dg2 dg1 dg0

		movq mm3, [esi]; //b1: mm3=src3 src2 src1 src0

		psubsb mm2, mm0; //g6: mm2=63-a3 63-a2 63-a1 63-a0

		movq mm7, MASKB; //b2: mm7=BLUE MASK

		pmullw mm4, mm0; //g8: mm4=sg?*a?

		movq mm0, [edi]; //b3: mm0=dst3 dst2 dst1 dst0

		pmullw mm5, mm2; //g9: mm5=dg?*(1-a?)

		movq mm2, mm7; //b4: mm2=fiveones

		pand mm3, mm7; //b4: mm3=sr3 sr2 sr1 sr0

		pmullw mm3, mm1; //b6: mm3=sb?*a?

		pand mm0, mm7; //b5: mm0=db3 db2 db1 db0

		movq mm7, [esi]; //r1: mm7=src3 src2 src1 src0

		paddw mm4, mm5; //g10: mm4=sg?*a?+dg?*(1-a?)

		pand mm7, MASKR; //r2: mm7=sr3 sr2 sr1 sr0

		psubsb mm2, mm1; //b5a: mm2=31-a3 31-a2 31-a1 31-a0

		paddw mm4, FIVETWELVE; //g11: mm4=(i+512) green

		pmullw mm0, mm2; //b7: mm0=db?*(1-a?)

		movq mm5, mm4; //g12: mm5=(i+512) green

		psrlw mm7, 11; //r4: shift src red down to position 0

		psrlw mm4, 6; //g13: mm4=(i+512)>>6

		paddw mm4, mm5; //g14: mm4=(i+512)+((i+512)>>6) green

		paddw mm0, mm3; //b8: mm0=sb?*a?+db?*(1-a?)

		movq mm5, [edi]; //r3: mm5=dst3 dst2 dst1 dst0

		paddw mm0, SIXTEEN; //b9: mm0=(i+16) blue

		pand mm5, MASKR; //r5: mm5=dr3 dr2 dr1 dr0

		psrlw mm4, 5; //g15: mm4=0?g0 0?g0 0?g0 0?g0 green

		movq mm3, mm0; //b10: mm3=(i+16) blue

		psrlw mm0, 5; //b11: mm0=(i+16)>>5 blue

		psrlw mm5, 11; //r6: shift dst red down to position 0

		paddw mm0, mm3; //b12: mm0=(i+16)+(i+16)>>5 blue

		psrlw mm0, 5; //b13: mm0=000r 000r 000r 000r blue

		pmullw mm7, mm1; //mm7=sr?*a?

		pand mm4, MASKG; //g16: mm4=00g0 00g0 00g0 00g0 green

		pmullw mm5, mm2; //r7: mm5=dr?*(31-a?)

		por mm0, mm4; //mm0=00gb 00gb 00gb 00gb

		add eax, 4; //move to next 4 alphas

		//stall

		paddw mm5, mm7; //r8: mm5=sr?*a?+dr?*(31-a?)

		paddw mm5, SIXTEEN; //r9: mm5=(i+16) red

		movq mm7, mm5; //r10: mm7=(i+16) red

		psrlw mm5, 5; //r11: mm5=(i+16)>>5 red

		paddw mm5, mm7; //r12: mm5=(i+16)+((i+16)>>5) red

		psrlw mm5, 5; //r13: mm5=(i+16)+((i+16)>>5)>>5 red

		psllw mm5, 11; //r14: mm5=mm5<<10 red

		por mm0, mm5; //mm0=0rgb 0rgb 0rgb 0rgb

		test ebx, 2; //check if there are 2 pixels

		jz oneendpixel; //goto one pixel if that's it

		movd[edi], mm0; //dst=0000 0000 0rgb 0rgb

		psrlq mm0, 32; //mm0>>32

		add edi, 4; //edi=edi+4

		sub ebx, 2; //saved 2 pixels

		jz nextline; //all done goto next line

	oneendpixel: //work on last pixel

		movd edx, mm0; //edx=0rgb

		mov[edi], dx; //dst=0rgb

	nextline: //goto next line

		dec ecx; //nuke one line

		jz done; //all done

		mov eax, lpLinearAlpBp; //alpha

		mov esi, lpLinearSrcBp; //src

		mov edi, lpLinearDstBp; //dst

		add eax, iAlpPitch; //inc alpha ptr by 1 line

		add esi, iSrcPitch; //inc src ptr by 1 line

		add edi, iDstPitch; //inc dst ptr by 1 line

		mov lpLinearAlpBp, eax; //save new alpha base ptr

		mov ebx, iDstW; //ebx=span width to copy

		mov lpLinearSrcBp, esi; //save new src base ptr

		mov lpLinearDstBp, edi; //save new dst base ptr

		jmp primeloop; //start the next span

	done:
		emms
	}

}


//mmx ARGB混合
void DrawAlpha32(
	unsigned long*   lpDst,			// 目标缓冲
	unsigned long    iDstX,			// 目标位置
	unsigned long    iDstY,			// 目标位置
	unsigned long    iDstPitch,		// 目标缓冲的pitch
	unsigned long*   lpSrc,			// 原色彩缓冲
	unsigned long    iSrcX,			// 原色彩位置
	unsigned long    iSrcY,			// 原色彩位置
	unsigned long    iSrcW,			// 原缓冲的尺寸
	unsigned long    iSrcH,			// 原缓冲的尺寸
	unsigned long    iSrcPitch		// 原色彩pitch
	)
{
	unsigned long *lpLinearDstBp = (iDstX)+(iDstY*iDstPitch / sizeof(DWORD)) + lpDst; //base pointer for linear destination
	unsigned long *lpLinearSrcBp = (iSrcX)+(iSrcY*iSrcPitch / sizeof(DWORD)) + lpSrc; //base pointer for linear source

_asm{
	mov edx, 0xFFFFFFFF; //255-Alpha mask 
	pxor mm7,mm7;	//MM7清0
	movd mm6,edx;	//MM6=FFFFFFFF

	mov esi, lpLinearSrcBp;                    // 移入源像素缓冲地址
	mov edi, lpLinearDstBp;                    // 移入目标像素缓冲地址
	mov ecx, iSrcH;                            // 下面两步操作是移入原缓冲的高度和宽度
	mov ebx, iSrcW;

MainLoop:
	//目标：0xFF585C58     源：0x71000008
	movd mm0,[esi];	//mm0=SRC        MM0=0000 0000 7100 0008
	punpcklbw mm0,mm7;	//SRC:32位Bit到64位Bit   MM0=0071 0000 0000 0008
	movq mm2,mm0;	//mm2=SRC MM2=0071 0000 0000 0008
	punpckhwd mm0,mm0;	//高位是ALPHA  原0071 0000 0000 0008 移 0071 0071 0000 0000
	punpckhdq mm0,mm0;	//双字移动到四字,现在有八个像素的Alpha了!  0071 0071 0071 0071

	movd edx,mm0;
	cmp edx,0x00ff00ff;
	je CopySrc;
	test edx, 0xffffffff;						// 如果alpha为0 ，那么会影响寄存器的标志位,ZF=1
	jz BeginPixel;

	movq mm1,mm6;       //MM1=0000 0000 FFFF FFFF
	punpckhdq mm1,mm1;
	movd mm3,[edi];       //mm3=DST      MM3=0000 0000 FF58 5C58
	punpcklbw mm3,mm7;  //mm3=32位Bit到64位Bit    MM3=00FF 0058 005C 0058
	psubb mm1,mm0;      //mm1=255-src alpha bit   MM1=00FF 00FF 00FF 00FF     MM0=0071 0071 0071 0071   MM1=008E 008E 008E 008E

	pmullw mm2,mm0;     //mm2=src*srcAlpha      MM2= 0071 0000 0000 0008   MM0=0071 0071 FF71 0071    MM2= 31E1 0000 0000 0388
	pmullw mm3,mm1;     //mm3=dst*(255-srcAlpha)  MM3= 00FF 0058 005C 0058  MM1=008F 008F FF8E FF8E   MM3=8E71 3128 D708 D8D0
	paddusw mm3,mm2;    //mm3=src*srcAlpha+dst*(255-srcAlpha)  MM3=8E71 3128 D708 D8D0    MM2=31E1 0000 0000 0388  MM3=C052 3128 D708 DC58
	psrlw mm3, 8;        //mm3=src*srcAlpha/256+dst*(255-srcAlpha)/256   MM3=C052 3128 D708 DC58   MM3=00C0 0031 00D7 00DC
	packuswb mm3,mm7;   //mm3=packed dst      MM3=00C0 0031 00D7 00DC     MM3=0000 0000 C031 D7DC
	movd [edi],mm3;
	jmp BeginPixel;
CopySrc:
	packuswb  mm2, mm2;							// 紧缩到低
	movd[edi], mm2;			
BeginPixel:
	add edi, 4;									// 目标像素向前移动4个像素
	add esi, 4;                                 // 源像素向前移动4个像素    
	sub ebx, 1;                                 // 宽度减4
	test ebx, 0xffffffff;						// check if only 0 pixels left
	jz NextLine;								// 如果只有0个像素，跳转到NextLine处理
	jmp MainLoop;								// 跳转到开始处，重新计算
NextLine:
	dec ecx;
	jz  Done;									// 处理完成
	mov esi, lpLinearSrcBp;						// src
	mov edi, lpLinearDstBp;						// dst
	add esi, iSrcPitch;							// inc src ptr by 1 line
	add edi, iDstPitch;							// inc dst ptr by 1 line
	mov ebx, iSrcW;								// ebx=span width to copy
	mov lpLinearSrcBp, esi;						// save new src base ptr
	mov lpLinearDstBp, edi;						// save new dst base ptr
	jmp MainLoop;								// start the next span
Done:
	emms;
	}
}



BOOL 获取动画尺寸(ACTION* a0,S2 *wx,S2* wy)
{
	int chrNo = ATR_CHR_NO(a0) - SPRSTART;
	if(chrNo < 0){
		int BmpNo;
		realGetNo( ATR_CHR_NO(a0) , (U4 *)&BmpNo );
			if(realGetWH(BmpNo, wx, wy)) return TRUE;
			else return FALSE;
	}else{
		if(chrNo >= mxSPRITE) return FALSE;
		ANIMLIST * ptAnimlist = SpriteData[chrNo].ptAnimlist;
		if(ptAnimlist){
			FRAMELIST * ptFramelist = ptAnimlist[0].ptFramelist;
			if(realGetWH(ptFramelist[0].BmpNo, wx, wy)) return TRUE;
			else return FALSE;
		}
	}
	return FALSE;
}

