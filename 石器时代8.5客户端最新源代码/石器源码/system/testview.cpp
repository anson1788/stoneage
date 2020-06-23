/************************/
/*	testView.cpp		*/
/************************/
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/loadsprbin.h"
#include "../systeminc/anim_tbl.h"
#include "../oft/vg410.h"
#include "../systeminc/t_music.h"

#ifdef _STONDEBUG_		

// ???????????
extern void hit_mark_critical( ACTION *a0 );

// 牙??????????????????
void StockDispBufferSprView( int x, int y, UCHAR dispPrio, int bmpNo )
{	
	// ??卒㎏?????????????
	DISP_SORT 	*pDispSort = DispBuffer.DispSort + DispBuffer.DispCnt;
	DISP_INFO 	*pDispInfo = DispBuffer.DispInfo + DispBuffer.DispCnt;
	
	// ??????????
	if( DispBuffer.DispCnt >= DISP_BUFFER_SIZE ) return;
	
	// 牙?！?????﹨▍
	pDispSort->dispPrio = dispPrio;
	pDispSort->no = DispBuffer.DispCnt;
	// 牙??﹨▍? ???????? ?
	pDispInfo->x = x;
	pDispInfo->y = y;
	pDispInfo->bmpNo = bmpNo;
	pDispInfo->hitFlag = 2;

	// 牙????????
	DispBuffer.DispCnt++;
}

/* ??????囝???? ********************************************************************/
void SprViewProc( void )
{
	char szMoji[ 256 ];
	int bmpX;
	int bmpY;
	static int bmpNo = 0;
	int bmpNoBak;
	static int no = 0; // ?希吻?
	static BOOL fontFlag = TRUE; // ????牙???
	// ?希????
	int zoubunTbl[] ={   1,  5,  25,  100, 	500,  1000,
						-1, -5, -25, -100, -500, -1000 };
	static int palNo = 0; // ????吻?
	static int time = 600; // ????吻?
	
	/* ??????吻??希? */
	switch( SubProcNo ){
	
		case 0:
			// ????????丙????希?
			//BackBufferDrawType = DRAW_BACK_NON
			// ???????????
			// ???
			if( joy_auto[ 0 ] & JOY_RIGHT ) bmpNo += zoubunTbl[ no ];
			// ??
			if( joy_auto[ 0 ] & JOY_LEFT ) bmpNo -= zoubunTbl[ no ];
			// ?希???
			if( joy_trg[ 0 ] & JOY_UP ){ 
				no++;
				if( no >= 6 ) no = 0; // ????????
			}
			// ?希????
			if( joy_trg[ 0 ] & JOY_DOWN ){
				no--;
				if( no <= -1 ) no = 5; // ????????
			}
#if 0
			// ?????????
			// ???????
			if( mouse.onceState & MOUSE_LEFT_CRICK ){
				bmpNo += zoubunTbl[ no ];
				// ?????
				play_se( 201, 320, 240 );
			}
			// ?????????
			if( GetAsyncKeyState( VK_MBUTTON ) ){
				bmpNo += zoubunTbl[ no ];
			}
			
			// ???????希阪?
			if( mouse.onceState & MOUSE_RIGHT_CRICK ){
			//if( mouse.state & MOUSE_RIGHT_CRICK ){
				no++;
				if( no == 6 ) no = 0; // ????????
				// ?????
				play_se( 201, 320, 240 );
			}
#endif			
			// ????????
			if( bmpNo < 0 ) bmpNo = 0;
			if( bmpNo >= MAX_GRAPHICS - 25 ) bmpNo = MAX_GRAPHICS -25;
			
			// ????阪?
			if( joy_trg[ 0 ] & JOY_A ){	/* ??? */
				palNo--;
				if( palNo < 0 ) palNo = MAX_PAL - 1;
				// ????????
				PaletteChange( palNo, 10 );
				// ????????
				play_se( 202, 320, 240 );
			}
			if( joy_trg[ 0 ] & JOY_B ){	/* ???? */
				palNo++;
				if( palNo >= MAX_PAL ) palNo = 0;
				// ????????
				PaletteChange( palNo, 10 );
				// ????????
				play_se( 202, 320, 240 );
			}
			// ???????阪?
			if( joy_trg[ 0 ] & JOY_DEL ){	/* ??? */
				if( fontFlag ) fontFlag = 0;
				else fontFlag = 1;
				// ??????
				play_se( 212, 320, 240 );
			}
			
			// ?????
			if( joy_trg[ 0 ] & JOY_ESC ){	/* ???? */
				// ???????? WM_CLOSE ??????????
				PostMessage( hWnd, WM_CLOSE, 0, 0L );
			}
			
			// ???吻???????
			bmpNoBak = bmpNo;
			
			// ???希???????希?
			for( bmpY = 0 ; bmpY < lpDraw->ySize ; bmpY += 96 ){
				for( bmpX = 0 ; bmpX < lpDraw->xSize ; bmpX += 128 ){
					// ???????????
					if( fontFlag == TRUE ){
						// ???吻?
						wsprintf( szMoji,"%7d", bmpNoBak );
						StockFontBuffer( bmpX, bmpY, FONT_PRIO_BACK, 0, szMoji, 0 );
					}
					// ???牙????????
					StockDispBufferSprView( bmpX, bmpY, 0, bmpNoBak++ );
				}
			}
			// ???????????
			if( fontFlag == TRUE ){
				// ?希牙
				wsprintf( szMoji, "PAL:%2d  ?希:%6d", palNo, zoubunTbl[ no ] );
				StockFontBuffer( 640 - 16 * 12, 462, FONT_PRIO_FRONT, 0, szMoji, 0 );
					
				// ?牙
				StockFontBuffer( 4, 462, FONT_PRIO_FRONT, 0, "ESC:賦旰  X or Y:PAL曹載  DEL:恅趼 ON/OFF", 0 );
			}
			/* ????????? */
			RunAction();
			// ???牙????????????
			StockTaskDispBuffer();
			
			// ??丙????
			StockBoxDispBuffer( 0, 0, lpDraw->xSize, lpDraw->ySize, DISP_PRIO_BG, SYSTEM_PAL_PURPLE , 1 );
			
			// ????????
			NowTime = TimeGetTime();

			break;
	}
}

/* ?????????????? ***********************************************/
void AnimSpr( ACTION *pAct )
{
	char szMoji[ 256 ];
	int x = 32, y = 32;
	int c = 0,d0;
	ACTION *a1;
	static int palNo = 0;
	static int anim_spd = 0;
	static bool slowFlag = FALSE;
	static int hitNo = 0;
	
	/* ┤?希? */
	switch( pAct->state ){
		/* 二? */
		case ACT_STATE_ACT:
			// ???????
			if( slowFlag == FALSE ){
				// ?????吻?阪?????
				if( joy_auto[ 0 ] & JOY_RIGHT ){	/* ??? */
					while( 1 ){
						pAct->anim_chr_no++;
						if( SpriteData[pAct->anim_chr_no - SPRSTART].ptAnimlist !=NULL ) break;
						if( pAct->anim_chr_no > 1000000 ){
							 pAct->anim_chr_no = 1000000;
							 break;
						}
					}
				}
				if( joy_auto[ 0 ] & JOY_LEFT ){		/* ???? */
					while( 1 ){
						pAct->anim_chr_no--;
						if( SpriteData[pAct->anim_chr_no - SPRSTART].ptAnimlist !=NULL ) break;
						if( pAct->anim_chr_no <  SPRSTART ){ 
							pAct->anim_chr_no =  SPRSTART;
							break;
						}
					}
				}
				// ?????吻?阪????????
				if( joy_trg[ 0 ] & JOY_INS ){	/* ??? */
					pAct->anim_chr_no += 1000;
					while( 1 ){
						pAct->anim_chr_no++;
						if( SpriteData[pAct->anim_chr_no - SPRSTART].ptAnimlist !=NULL ) break;
						if( pAct->anim_chr_no > 1000000 ){ 
							pAct->anim_chr_no = 1000000;
							break;
						}
					}
				}
				if( joy_trg[ 0 ] & JOY_DEL ){	/* ???? */
					pAct->anim_chr_no -= 1000;
					while( 1 ){
						pAct->anim_chr_no--;
						if( SpriteData[pAct->anim_chr_no - SPRSTART].ptAnimlist !=NULL ) break;
						if( pAct->anim_chr_no <  SPRSTART ){ 
							pAct->anim_chr_no =  SPRSTART;
							break;
						}
					}
				}
				// ???????????????
				//while( 1 ){
				//if( SpriteData[ pAct->anim_chr_no ].ptAnimlist == NULL )
				//{
					
				
				// ??阪?
				if( joy_auto[ 0 ] & JOY_UP ){		/* ??? */
					pAct->anim_ang++;
					if( pAct->anim_ang >= 8 ) pAct->anim_ang = 0;
				}
				if( joy_auto[ 0 ] & JOY_DOWN ){		/* ???? */
					pAct->anim_ang--;
					if( pAct->anim_ang < 0 ) pAct->anim_ang =  7;
				}
				
				// ???????吻?牙
				if( joy_trg[ 0 ] & JOY_HOME ){		/* ??? */
					pAct->anim_no++;
					if( pAct->anim_no > 12 ) pAct->anim_no = 12;
				}
				if( joy_trg[ 0 ] & JOY_END ){	/* ???? */
					pAct->anim_no--;
					if( pAct->anim_no < 0 ) pAct->anim_no =  0;
				}
				
				// ????阪?
				if( joy_trg[ 0 ] & JOY_A ){	/* ??? */
					anim_spd--;
					if( anim_spd < 0 ) anim_spd = 0;
				}
				if( joy_trg[ 0 ] & JOY_B ){	/* ???? */
					anim_spd++;
					if( anim_spd >= 255 ) anim_spd = 255;
				}
			}
			// ?????
			if( joy_trg[ 0 ] & JOY_ESC ){	/* ???? */
				// ???????? WM_CLOSE ??????????
				PostMessage( hWnd, WM_CLOSE, 0, 0L );
			}
			
			
			// ?????????
			if( joy_trg[ 1 ] & JOY_F12 ){ 
				pAct->anim_frame_cnt = 0;
				slowFlag = TRUE;
			}
			// ?????????
			if( joy_trg[ 1 ] & JOY_F11 ){ 
				pAct->anim_cnt -= 2;
				// ????????
				if( pAct->anim_cnt <= 0 ) pAct->anim_cnt = 0;
				pAct->anim_frame_cnt = 0;
				slowFlag = TRUE;
			}
			// ??????????
			if( joy_trg[ 1 ] & JOY_F10 )
				slowFlag = FALSE;
			
			if( slowFlag == FALSE || joy_trg[ 1 ] & JOY_F11 || joy_trg[ 1 ] & JOY_F12 ){
				// ???????
				pattern( pAct, anim_spd, ANM_LOOP );
			}
			
			// ???刪叉??
			if( pAct->anim_hit >= 10000 ){
				hitNo = pAct->anim_hit; // ???刪叉吻???
				pAct->anim_hit = 0; // ???
				// ???????
				for( d0=5 ;d0>0 ;d0--){
					//?????????
					a1 = GetAction( PRIO_JIKI, sizeof( ATR_EQU ) );
					if( a1 == NULL ) return;
					/* 牙?！冉 */
					ATR_DISP_PRIO(a1) = DISP_PRIO_CHAR + 1;
					/* ?????吻? */
					ATR_CHR_NO(a1) = CG_HIT_MARK_22;
					ATR_H_POS(a1) = 320;
					ATR_V_POS(a1) = 240;
					ATR_SPD(a1) = Rnd( 4, 7 );
					ATR_CRS(a1) = d0 * 6;
					ATR_FIRST_FLG(a1) = ATR_STIMER(a1) = 32;		//牙?????
					ATR_KAISHIN(a1) = 1;
					ATR_NAME(a1) = hit_mark_critical;
					ATR_CHR_NO(a1) = CG_HIT_MARK_01;
				}
			}
			// ?????吻?牙
			wsprintf( szMoji, "雄  賒  晤  瘍 =  %8d ㄩ Left or Right ( ㊣1 )", pAct->anim_chr_no );
			StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, szMoji, 0 ); y += 20;
			StockFontBuffer( x + 236, y, FONT_PRIO_FRONT, 0, "ㄩ DEL  or INS ( ?1000 )", 0 ); y += 20;
			// ??牙
			wsprintf( szMoji, "源          砃 =  %8d ㄩ Down or UP", pAct->anim_ang );
			StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, szMoji, 0 ); y += 20;
			// ???????吻?牙
			wsprintf( szMoji, "雄  釬  晤  瘍 =  %8d ㄩ END  or HOME", pAct->anim_no );
			StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, szMoji, 0 ); y += 20;
			// ?希牙
			wsprintf( szMoji, "厒         僅 =  %8d ㄩ Z    or X", anim_spd );
			StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, szMoji, 0 ); y += 20;
			
			// ???吻?
			wsprintf( szMoji, "秞  虴  晤  瘍 =  %8d", t_music_se_no );
			StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, szMoji, 0 ); y += 20;
			// ??????
			if( t_music_se_no != -1 ){
				// ??????
				if( joy_trg[ 1 ] & JOY_F8 ){		/* ??? */
					tone_tbl[ t_music_se_no ].voice_volume++;
					if( tone_tbl[ t_music_se_no ].voice_volume >= 128 ) tone_tbl[ t_music_se_no ].voice_volume = 127;
				}
				if( joy_trg[ 1 ] & JOY_F7 ){	/* ???? */
					tone_tbl[ t_music_se_no ].voice_volume--;
					if( tone_tbl[ t_music_se_no ].voice_volume <= -1 ) tone_tbl[ t_music_se_no ].voice_volume = 0;
				}
				
				// ???????
				if( joy_trg[ 1 ] & JOY_F6 ){	/* ??? */
					tone_tbl[ t_music_se_no ].voice_note++;
					if( tone_tbl[ t_music_se_no ].voice_note + tone_tbl[ t_music_se_no ].voice_rate >= 63 ) tone_tbl[ t_music_se_no ].voice_note = 62 - tone_tbl[ t_music_se_no ].voice_rate;
				}
				if( joy_trg[ 1 ] & JOY_F5 ){	/* ???? */
					tone_tbl[ t_music_se_no ].voice_note--;
					if( tone_tbl[ t_music_se_no ].voice_note + tone_tbl[ t_music_se_no ].voice_rate <= 0 ) tone_tbl[ t_music_se_no ].voice_note = -tone_tbl[ t_music_se_no ].voice_rate + 1;
				}
				// ?????
				wsprintf( szMoji, "秞  虴  秞  講 =  %8d ㄩ F7 or F8", tone_tbl[ t_music_se_no ].voice_volume );
				StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, szMoji, 0 ); y += 20;
				// ??????
				wsprintf( szMoji, "秞  虴  詢  腴 =  %8d ㄩ F5 or F6", tone_tbl[ t_music_se_no ].voice_note );
				StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, szMoji, 0 ); y += 20;
			}else{
				// ?????
				wsprintf( szMoji, "秞  虴  秞  講 =  %8d ㄩ F7 or F8", 0 );
				StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, szMoji, 0 ); y += 20;
				// ??????
				wsprintf( szMoji, "秞  虴  詢  腴 =  %8d ㄩ F5 or F6", 0 );
				StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, szMoji, 0 ); y += 20;
			}
			
			// ???吻?
			//wsprintf( szMoji, "???  吻  ? =  %8d", pAct->bmpNo );
			//StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, szMoji, 0 ); y += 20;
			// ???牙
			//wsprintf( szMoji, "???  ??? = %4d,%4d", SpriteInfo[ pAct->bmpNo ].width, SpriteInfo[ pAct->bmpNo ].height );
			//StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, szMoji, 0 ); y += 20;
						
			// ???刪叉吻?
			wsprintf( szMoji, "???刪叉吻? =  %8d", hitNo );
			StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, szMoji, 0 ); y += 20;
			
			// ?????。?
			wsprintf( szMoji, "F12ㄩ????  F11ㄩ????  F10ㄩ???" );
			StockFontBuffer( 8, 460, FONT_PRIO_FRONT, 0, szMoji, 0 ); y += 20;
			
			// ?牙
			StockFontBuffer( 530, 460, FONT_PRIO_FRONT, 0, "賦旰ㄩESC", 0 );
			
			break;
		
		/* ?昀? */	
		case ACT_STATE_DEAD:
		
			DeathAction( pAct );
			
			break;
	}
}

/* ????????????? ***********************************************/
ACTION *MakeAnimSpr( void )
{
	ACTION *pAct;
	
	/* ?????????仕? */
	pAct = GetAction( PRIO_JIKI, 0 );
	if( pAct == NULL ) return NULL;
	
	/* ??? */
	pAct->func = AnimSpr;
	// anim_tbl.h ?吻?
	pAct->anim_chr_no = SPRSTART; 
	// 去?吻?
	pAct->anim_no = 0;//ANIM_ATTACK;
	// ?????????( ??? )( ??????? )
	pAct->anim_ang = 0;//Rnd( 0, 7 );
	// ????牙
	pAct->atr |= ACT_ATR_HIT_BOX;
	/* 牙?！冉 */
	pAct->dispPrio = DISP_PRIO_CHAR;
	/* ???ㄅ */
	pAct->x = 320;
	pAct->y = 360;
	
	// ???????
	pattern( pAct, ANM_NOMAL_SPD, ANM_LOOP );
	
	return pAct;
}

/* ????????囝???? ********************************************************************/
void AnimViewProc( void )
{
	/* ??????吻??希? */
	switch( SubProcNo ){
	
		case 0:
			
			// ?????????????
			MakeAnimSpr();
			// ·卯??????
			play_bgm( 1 );
			// ??????吻????
			SubProcNo++;
			
			break;
			
		case 1:
		
			/* ????????? */
			RunAction();
			// ???牙????????????
			StockTaskDispBuffer();
			
			// ??丙????
			StockBoxDispBuffer( 0, 0, lpDraw->xSize, lpDraw->ySize, DISP_PRIO_BG, SYSTEM_PAL_PURPLE , 1 );
			
			break;
	}
}

/* ???囝???? ********************************************************************/
void SeTestProc( void )
{
	char szMoji[ 256 ];
	int x = 128, y = 196;
	static int seNo = 1, bgmFlag;
	
	/* ??????吻??希? */
	switch( SubProcNo ){
	
		case 0:
			
			// ??????吻????
			SubProcNo++;
			
			break;
			
		case 1:
			
			// ??????
			if( joy_trg[ 0 ] & JOY_A ) play_se( seNo, 320, 240 );
			// ???????????
			if( joy_trg[ 0 ] & JOY_B ){ 
				if( bgmFlag == FALSE ){
					bgmFlag = TRUE;
					play_bgm( 0 );
				}else{
					bgmFlag = FALSE;
					stop_bgm();
				}
			}
			
			// ???吻?阪?????
			if( joy_auto[ 0 ] & JOY_RIGHT ){	/* ??? */
				while( 1 ){
					seNo++;
					if( seNo >= TONE_MAX ) seNo = 1;
					if( tone_tbl[ seNo ].voice_volume != 0 ) break;
				}
			}
			if( joy_auto[ 0 ] & JOY_LEFT ){		/* ???? */
				while( 1 ){
					seNo--;
					if( seNo < 0 ) seNo = TONE_MAX - 1;
					if( tone_tbl[ seNo ].voice_volume != 0 ) break;
				}
			}
			// ??????
			if( joy_auto[ 1 ] & JOY_F8 ){		/* ??? */
				tone_tbl[ seNo ].voice_volume++;
				if( tone_tbl[ seNo ].voice_volume >= 128 ) tone_tbl[ seNo ].voice_volume = 127;
			}
			if( joy_auto[ 1 ] & JOY_F7 ){	/* ???? */
				tone_tbl[ seNo ].voice_volume--;
				if( tone_tbl[ seNo ].voice_volume <= -1 ) tone_tbl[ seNo ].voice_volume = 0;
			}
			
			// ???????
			if( joy_auto[ 1 ] & JOY_F6 ){	/* ??? */
				tone_tbl[ seNo ].voice_note++;
				if( tone_tbl[ seNo ].voice_note + tone_tbl[ seNo ].voice_rate >= 63 ) tone_tbl[ seNo ].voice_note = 62 - tone_tbl[ seNo ].voice_rate;
			}
			if( joy_auto[ 1 ] & JOY_F5 ){	/* ???? */
				tone_tbl[ seNo ].voice_note--;
				if( tone_tbl[ seNo ].voice_note + tone_tbl[ seNo ].voice_rate <= 0 ) tone_tbl[ seNo ].voice_note = -tone_tbl[ seNo ].voice_rate + 1;
			}
			
			// ?????
			if( joy_trg[ 0 ] & JOY_ESC ){	/* ???? */
				// ???????? WM_CLOSE ??????????
				PostMessage( hWnd, WM_CLOSE, 0, 0L );
			}
			// ???吻?
			wsprintf( szMoji, "秞  虴  晤  瘍 =  %4d ㄩ Left or Right", seNo );
			StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, szMoji, 0 ); y += 20;
			// ?????
			wsprintf( szMoji, "秞  虴  秞  講 =  %4d ㄩ F7   or F8", tone_tbl[ seNo ].voice_volume );
			StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, szMoji, 0 ); y += 20;
			// ??????
			wsprintf( szMoji, "秞  虴  詢  腴 =  %4d ㄩ F5   or F6", tone_tbl[ seNo ].voice_note );
			StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, szMoji, 0 ); y += 20;
			// ?????????
			wsprintf( szMoji, "Zㄩ秞  虴  畦  溫  Xㄩ掖劓秞氈畦溫" );
			StockFontBuffer( 16, 460, FONT_PRIO_FRONT, 0, szMoji, 0 ); y += 20;
			
			// ?牙
			StockFontBuffer( 530, 460, FONT_PRIO_FRONT, 0, "賦旰ㄩESC", 0 );
			
			break;
	}
}

#endif
