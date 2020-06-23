/************************/
/*	battleMap.c			*/
/************************/
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../other/caryIme.h"
#include "../systeminc/battleMap.h"
#include "../mylua/winlua.h"

// ??????????
#define BATTLE_MAP_SIZE 400

#ifdef _NEWDRAWBATTLEMAP		   // (不可开放) Syu ADD 自动产生BattleMap
#define RAND(x,y)   ((x-1)+1+ (int)( (double)(y-(x-1))*rand()/(RAND_MAX+1.0)) )
int RandBattleBg = 0;
#endif

#ifdef __ATTACK_MAGIC

// Global vars
extern int			g_iRunEarthQuake;			// 地震的状况: 0 --> 没地震 , 1 --> 初始化地震 , 2 --> 地震中
extern int			g_iCurRunEarthQuake;		// 目前地震的位置线
extern int			g_iNumRunEarthQuake;		// 地震的总次数

#endif

#ifdef _READ16BITBMP
extern BOOL g_bUseAlpha;
#endif
int MessageBoxNew(HWND hWnd,LPCSTR lpText,LPCSTR lpCaption,UINT uType);
// ??????????
int BattleMapNo = 0;

#ifdef _NEWDRAWBATTLEMAP_FIX		   // (不可开)ROG ADD 战斗地图修改
int BattleMapAdvance = 0;			   // 战斗地图效能改进
#endif
// ?????? ????䦶
char *BattleMapFile[] = {	
	#include "../systeminc/battleMapName.h"
};
						
// ?????????????????????? ***************************/
// 水晶地表战斗地图编号
static int iCrystalBattleMap[4][3] =
{	// 绿								// 蓝								// 红								// 黄
	{29060,29061,29062},{29058,29059,29066},{29063,29064,29065},{29057,29067,29068}
};

BOOL ReadBattleMap( int no )
{
#ifdef _NEWDRAWBATTLEMAP_FIX		   // (不可开)ROG ADD 战斗地图修改
	extern void ddrawBattleMap();
	ddrawBattleMap();
#else
	char msg[ 256 ];
	FILE *fp;
	char header[ 5 ];
	unsigned short tile[ 1089 ];
	int cnt = 0;
	int i, j, x = 0, y = 0, posX, posY;
	unsigned short c1, c2;
	// ????????
	if( no >= BATTLE_MAP_FILES )
		no = 0;
	// ?????????
	BattleMapNo = no;
	// ??????????????
	if( ( fp = fopen( BattleMapFile[ no ], "rb" ) ) == NULL ){
		sprintf_s( msg, "%s读取失败。", BattleMapFile[ no ] );
		MessageBoxNew( hWnd, msg, "战场地图错误", MB_OK | MB_ICONSTOP );
		return FALSE;
	}
	// ????????
	fread( header, sizeof( char ), 4, fp );
	header[ 4 ] = '\0';
	// ????????
	if( !( strstr( header, "SAB" )) )
		MessageBoxNew( hWnd, "SAB 档案不存在", "战场地图错误", MB_OK | MB_ICONSTOP );
	// ??????
	for( i = 0 ; i < 1089 ; i++ ){
		// ?????????? ????????? ?
		c1 = fgetc( fp );
		c2 = fgetc( fp );
		// ???????
		tile[ i ] = ( c1 << 8 ) | c2;
	}
	// ??????
	fclose( fp );
	//ROG FIX 战斗地图绘图位址调整
	//posX = 32 * ( -9 ) + (400 >> 1);
	//posY = 24 * 10 + (300 >> 1);
	posX = -416;
	posY=272;
#ifdef _NEWDRAWBATTLEMAP		   // Syu ADD 自动产生BattleMap
	extern int nowFloor ; 

	if(((nowFloor == 817) || (nowFloor == 8007) || (nowFloor == 8101) || (nowFloor == 8100) ||
		  (nowFloor == 8027) || (nowFloor == 8028) || (nowFloor == 8029) || (nowFloor == 8015) ||
			(nowFloor == 151) || (nowFloor == 160) || (nowFloor == 161) || (nowFloor >= 30022 && nowFloor <= 30025) || nowFloor == 8113 || nowFloor == 8114
#ifdef _8BATTLEMAP
		|| (nowFloor >= 78 && nowFloor <= 99) || nowFloor == 8107 || (nowFloor >= 8108 && nowFloor <= 8112) || (nowFloor >= 8116 && nowFloor <= 8120) || (nowFloor >= 8124 && nowFloor <= 8126) || nowFloor == 7500 || nowFloor == 7530 || nowFloor == 7600
#endif
			)
		&& ( RandBattleBg == 0 )  )
	{
		RandBattleBg = RAND ( 0 , 4 );
		if ( RandBattleBg == 0 ) RandBattleBg = 29001 ;
		else if ( RandBattleBg == 1 ) RandBattleBg = 29020 ;
		else if ( RandBattleBg == 2 ) RandBattleBg = 29019 ;
		else if ( RandBattleBg == 3 ) RandBattleBg = 29024 ;
		else if ( RandBattleBg == 4 ) RandBattleBg = 29025 ;
		
		RandBattleBg = RAND ( 29047 , 29049 );
		// 水晶地表战斗地图
		if(nowFloor == 30022) RandBattleBg = iCrystalBattleMap[0][RAND(0,2)];
		else if(nowFloor == 30023) RandBattleBg = iCrystalBattleMap[1][RAND(0,2)];
		else if(nowFloor == 30024) RandBattleBg = iCrystalBattleMap[2][RAND(0,2)];
		else if(nowFloor == 30025) RandBattleBg = iCrystalBattleMap[3][RAND(0,2)];
#ifdef _8BATTLEMAP
		else if((nowFloor >= 78 && nowFloor <= 99) || nowFloor == 8107 ) RandBattleBg = 29074+RAND(0,2);
		else if((nowFloor >= 8108 && nowFloor <= 8112) || (nowFloor >= 8116 && nowFloor <= 8120) || (nowFloor >= 8124 && nowFloor <= 8126) || nowFloor == 7500 || nowFloor == 7530 || nowFloor == 7600 ) RandBattleBg = 29070+RAND(0,3);
#endif
		StockDispBuffer ( 400 , 300 , DISP_PRIO_TILE , RandBattleBg , 0 ) ;
	}
	else if ( ((nowFloor == 817) || (nowFloor == 8007) || (nowFloor == 8101) || (nowFloor == 8100)
		|| (nowFloor == 8027) || (nowFloor == 8028) || (nowFloor == 8029) || (nowFloor == 8015) || ( nowFloor == 151 )
		|| (nowFloor == 160) || (nowFloor == 161)	|| (nowFloor >= 30022 && nowFloor <= 30025) || nowFloor == 8113 || nowFloor == 8114
#ifdef _8BATTLEMAP
		|| (nowFloor >= 78 && nowFloor <= 99) || nowFloor == 8107 || (nowFloor >= 8108 && nowFloor <= 8112) || (nowFloor >= 8116 && nowFloor <= 8120) || (nowFloor >= 8124 && nowFloor <= 8126) || nowFloor == 7500 || nowFloor == 7530 || nowFloor == 7600
#endif
		)
		&& (RandBattleBg != 0) )
		StockDispBuffer (400 , 300 , DISP_PRIO_TILE , RandBattleBg , 0 ) ;
#ifdef _SYUTEST2
	else { 
		RandBattleBg = 29050;
		StockDispBuffer ( 320 , 240 , DISP_PRIO_TILE , RandBattleBg , 0 ) ;
	}
#else
	else if(nowFloor == 8519){
		RandBattleBg = 29069;
		StockDispBuffer ( 400 , 300 , DISP_PRIO_TILE , RandBattleBg , 0 ) ;
	}else
	{
		if((RandBattleBg = FreeGetBattleMap(nowFloor))){
			StockDispBuffer ( 400 , 300 , DISP_PRIO_TILE , RandBattleBg , 0 ) ;	
		}else{
			for( i = 0 ; i < 33 ; i++ )
			{
				x = 0;
				y = 0;
				for( j = 0 ; j < 33 ; j++ )
				{
					StockDispBuffer( posX + x, posY + y, DISP_PRIO_TILE, tile[ cnt++ ], 0 );
					x += 32;
					y -= 24;
				}
				posX += 32;
				posY += 24;
			}
		}
	}
#endif

//	extern void ddrawBattleMap();
//	ddrawBattleMap();

#else
	// ??????
	
	for( i = 0 ; i < 20 ; i++ ){
		x = 0;
		y = 0;
		for( j = 0 ; j < 20 ; j++ ){
			// ???????????
			StockDispBuffer( posX + x, posY + y, DISP_PRIO_TILE, tile[ cnt++ ], 0 );
			x += 32;
			y -= 24;
		}
		posX += 32;
		posY += 24;
	}
#endif
#endif
	return TRUE;
}	

// ??????? ***********************************************************/
#ifdef _NEWDRAWBATTLEMAP_FIX		   // (不可开)ROG ADD 战斗地图修改
#define	RASTER_CLEARANCE	24		   //增加效能
#else
#define	RASTER_CLEARANCE	8
#endif
int piyo_tbl[]={
	0,1,2,3,4,5,6,7,8,9,10,10,11,11,11,12,
	12,12,11,11,11,10,9,9,8,7,6,5,4,3,2,1,
	0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-11,-11,-11,-12,
	-12,-12,-11,-11,-11,-10,-9,-9,-8,-7,-6,-5,-4,-3,-2,-1,
};
int piyo_point = 0;

void DrawBattleMap( void )
{

#ifdef D3D_ENGINE
	RECT destrect = { 0 };
#else
	RECT rect = { 0, 0, DEF_APPSIZEX, DEF_APPSIZEY };
#endif

	int d0,d1,d6,d7,value;
	short bx , by;
#ifdef __ATTACK_MAGIC

	// 特殊地图编号或地震中

#ifdef _NEWDRAWBATTLEMAP		   // Syu ADD 自动产生BattleMap
	extern int nowFloor;
	if( ( BattleMapNo >= 148 && BattleMapNo <= 150 ) || 2 == g_iRunEarthQuake || 
		nowFloor == 817 || nowFloor == 8007 || nowFloor == 8101 || nowFloor == 8100 || 
		nowFloor == 8027 || nowFloor == 8028 || nowFloor == 8029 || nowFloor == 8015 || nowFloor == 8113 || nowFloor == 8114 ){
#else
	if( ( BattleMapNo >= 148 && BattleMapNo <= 150 ) || 2 == g_iRunEarthQuake ){
#endif
#else
	if(BattleMapNo >= 148 && BattleMapNo <= 150){		//???????????
#endif
		d1 = 1;
	} else {
		d1 = 0;
	}
	switch( d1 ){
	case 0:
		bx = 0;
		by = 0;
		short x0, y0;
		long w, h;
		// ???????
		x0 = bx;
		y0 = by;
		w = rect.right - rect.left;
		h = rect.bottom - rect.top;
		if( bx >= lpDraw->xSize || bx + w <= 0 || by >= lpDraw->ySize || by + h <= 0 ){
			return;
		}
		if( bx < 0 ){
			rect.left -= bx;
			x0 = 0;
		}
		if( bx + w > lpDraw->xSize ){
			rect.right -= bx + w - lpDraw->xSize;
		}
		if( by < 0 ){
			rect.top -= by;
			y0 = 0;
		}
		if( by + h > lpDraw->ySize ){
			rect.bottom -= by + h - lpDraw->ySize;
		}
#ifdef _STONDEBUG_
		SurfaceDispCnt++;
#endif
#ifdef D3D_ENGINE
		destrect.left = x0;
		destrect.top = y0;
		destrect.right = x0 + rect.right - rect.left;
		destrect.bottom = y0 + rect.bottom - rect.top;
		lpDraw->lpd3dDevice->StretchRect(lpBattleSurface, &rect, lpDraw->lpOffScrSurface, &destrect, D3DTEXF_NONE);
#else
		lpDraw->lpBACKBUFFER->BltFast( x0, y0, lpBattleSurface, &rect, DDBLTFAST_WAIT );
#endif
#ifdef _READ16BITBMP
		if(g_bUseAlpha) lpDraw->lpBACKBUFFERSYS->BltFast(x0,y0,lpBattleSurfaceSys,&rect,DDBLTFAST_WAIT);
#endif
		break;
	case 1:

		value		 = piyo_point;
		d6			 = 0;

		#ifdef __ATTACK_MAGIC

		value		+= g_iCurRunEarthQuake;

		#endif

		rect.left = 0;		//?
		rect.right = 640 + DISPLACEMENT_X;		//?
	/*	if(BattleMapAdvance < 10){
			BattleMapAdvance ++;
			return;
		}
		else
			BattleMapAdvance =0;*/

		for(d7=0; d7 < 480 + DISPLACEMENT_Y; d7+=RASTER_CLEARANCE){
			rect.top = d7 + piyo_tbl[(d6+value) & 63] + 12;		//
			rect.bottom = rect.top + RASTER_CLEARANCE;		//?


#ifdef D3D_ENGINE
			destrect.left = 0;
			destrect.top = d7;
			destrect.right = rect.right - rect.left;
			destrect.bottom = d7 + rect.bottom - rect.top;
			lpDraw->lpd3dDevice->StretchRect(lpBattleSurface, &rect, lpDraw->lpOffScrSurface, &destrect, D3DTEXF_NONE);
#else
			lpDraw->lpBACKBUFFER->BltFast( 0, d7, lpBattleSurface, &rect, DDBLTFAST_WAIT );
#endif
#ifdef _READ16BITBMP
			if(g_bUseAlpha) lpDraw->lpBACKBUFFERSYS->BltFast(0,d7,lpBattleSurfaceSys,&rect,DDBLTFAST_WAIT);
#endif
			d6++;
		}
		break;
	case 2:
		d6 = 0;
		for(d7=0; d7<480 + DISPLACEMENT_Y; d7+=RASTER_CLEARANCE/2){

			rect.top = d7;		//
			rect.bottom = rect.top + RASTER_CLEARANCE;		//?

			d0 = piyo_tbl[(d6+piyo_point) & 63] / 2;		//?
			if(d0 < 0){		//??????
				rect.left = 0 - d0;
				rect.right = 640 + DISPLACEMENT_X+ d0;		//?
				d0 = 0;
			} else {
				rect.left = 0;
				rect.right = 640 + DISPLACEMENT_X- d0;		//?
			}
#ifdef D3D_ENGINE
			destrect.left = d0;
			destrect.top = d7;
			destrect.right = d0 + rect.right - rect.left;
			destrect.bottom = d7 + rect.bottom - rect.top;
			lpDraw->lpd3dDevice->StretchRect(lpBattleSurface, &rect, lpDraw->lpOffScrSurface, &destrect, D3DTEXF_NONE);
#else
			lpDraw->lpBACKBUFFER->BltFast( d0, d7, lpBattleSurface, &rect, DDBLTFAST_WAIT );
#endif
#ifdef _READ16BITBMP
			if(g_bUseAlpha) lpDraw->lpBACKBUFFERSYS->BltFast(d0,d7,lpBattleSurfaceSys,&rect,DDBLTFAST_WAIT);
#endif
			d6++;
		}
		break;
	}
}
