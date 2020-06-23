#ifndef _LOADREALBIN_H_
#define _LOADREALBIN_H_
#include "directdraw.h"
void initAutoMapColor( char * );
void makeAutoMapColor( void );
int writeAutoMapColor( char *, char * );
int readAutoMapColor( char *, char * );

int initRealbinFileOpen(char *realbinfilename, char *addrbinfilename);
#ifdef _READ16BITBMP
int InitRealTruebinFileOpen(char *szRealTrueBinFileName,char *szAdrnTruebinFileName);
#endif
void cleanupRealbin(void);

extern unsigned char autoMapColorTbl[];

#include <stdio.h>
typedef unsigned char MOJI;
typedef unsigned char U1;
typedef          char S1;
typedef unsigned short U2;
typedef          short S2;
typedef unsigned long U4;
typedef          long S4;
typedef float  F4;
typedef double F8;
#ifdef _READ16BITBMP
BOOL Read16BMP(int BmpNo,unsigned char **BmpData,int *width,int *height,BYTE **AlphaData,BOOL *useAlpha);
#endif
int realGetImage(int graphicNo, unsigned char **bmpdata, int *width, int *height);
int realGetPos(U4 GraphicNo , S2 *x , S2 *y);
int realGetWH(U4 GraphicNo , S2 *w , S2 *h);
int realGetHitPoints(U4 GraphicNo , S2 *HitX , S2 *HitY);
int realGetHitFlag(U4 GraphicNo , S2 *Hit);
BOOL realGetPrioType(U4 GraphicNo , S2 *prioType);
int realGetHeightFlag(U4 GraphicNo , S2 *Height);
int realGetNo( U4 CharAction , U4 *GraphicNo );
int realGetSoundEffect(U4 GraphicNo);
int realGetWalkSoundEffect(U4 GraphicNo);
int realGetBitmapNo( int num );
typedef struct {
	unsigned char atari_x,atari_y;	//??
	unsigned short hit;				// 可否行走 //???
	short height;				//?????
	short broken;				//????
	short indamage;				//HP????
	short outdamage;			//????
	short inpoison;				//
	short innumb;				//???
	short inquiet;				//?
	short instone;				//?
	short indark;				//??
	short inconfuse;			//??
	short outpoison;			//
	short outnumb;				//??
	short outquiet;				//?
	short outstone;				//?
	short outdark;				//??
	short outconfuse;			//??
	short effect1;				//?????1???????????????
	short effect2;				//?????2?????????????
	unsigned short damy_a;
	unsigned short damy_b;
	unsigned short damy_c;
	unsigned int bmpnumber;		//???
} MAP_ATTR;

struct ADRNBIN{
	unsigned long	bitmapno;
	unsigned long	adder;
	unsigned long	size;
	int	xoffset;
	int	yoffset;
	unsigned int width;
	unsigned int height;
	MAP_ATTR attr;
};

#ifdef _READ16BITBMP
struct AddressBin_s{
	unsigned int bitmapno;			// 图号
	unsigned int adder;					// 资料在图素档里的起始位址
	unsigned int palSize;				// 色盘资料大小
	unsigned int size;					// 一笔资料的大小
	unsigned int alpha_size;		// alpha_size不为0表示在图素资料后连着是alpha资料,alpha_size就是alpha资料的大小
															// 如果alpha_size为0表示没有带alpha资料
	int	xoffset;
	int	yoffset;
	unsigned int width;					// 图宽
	unsigned int height;				// 图高
	unsigned int staturated;		// 是否用饱和效果
};
#endif

// ????????
static DWORD FontPal[FONT_PAL_NUM]={
#ifdef _SA_VERSION_25
	RGB(0xff ,0xff, 0xff ), // 0:FONT_PAL_WHITE
	RGB(0x00 ,0xff, 0xff ), // 1:FONT_PAL_AQUA
	RGB(0xff ,0x00, 0xff ), // 2:FONT_PAL_PURPLE
	RGB(0x48 ,0x48, 0xfa ), // 3:FONT_PAL_BLUE
	RGB(0xff ,0xea, 0x39 ), // 4:FONT_PAL_YELLOW
	RGB(0xa0 ,0xf4, 0x3a ), // 5:FONT_PAL_GREEN  a0f43a
	RGB(0xB5 ,0x38, 0x17 ), // 6:FONT_PAL_RED
	RGB(0xa0 ,0xa0, 0xa4 ), // 7:FONT_PAL_GRAY
	RGB(0xa6 ,0xca, 0xf0 ), // 8:FONT_PAL_BLUE2
	RGB(0xc0 ,0xdc, 0xc0 ), // 9:FONT_PAL_GREEN
#endif


#ifdef _EXTEND_FONT_PAL
	RGB(0xff ,0x80, 0x00 ), // 10
	RGB(0x6f ,0xc6, 0x6f ), // 11
	RGB(0x98 ,0x3d, 0xf3 ), // 12
	RGB(0xff ,0x80, 0xff ), // 13
	RGB(0x4a ,0x9f, 0xf4 ), // 14
	RGB(0xb4 ,0x0e, 0xb4 ), // 15
	RGB(0xbf ,0xbf, 0x19 ), // 16
	RGB(0xff ,0x00, 0x80 ), // 17
	RGB(0xff ,0x80, 0x80 ), // 18
	RGB(0x80 ,0x80, 0x80 ), // 19

	RGB(0xff ,0xff, 0x80 ), // 20
	RGB(0x80 ,0xff, 0xff ), // 21
	RGB(0x80 ,0xff, 0x00 ), // 22
	RGB(0x00 ,0x80, 0xff ), // 23
	RGB(0xd0 ,0x2a, 0x7d ), // 24
	RGB(0x8d ,0x56, 0x8d ), // 25
#endif
};

/*
BOOL InitRealbinFileOpen( char *realbinfilename, char *addrbinfilename);	//初始化realbin的资料并配置相关的记忆体
//void SetCurFrame();									//累加CurFrame的counter，请每个frame执行一次
void CacheRLU( int howlong);							//释放太久没使用的图档记忆体
BOOL realGetNo( DWORD CharAction, DWORD *GraphicNo);	//从char action转成图档编号
DWORD realGetNo2( DWORD bnum);							//从bnum转成图档编号
BOOL realIsValid( DWORD CharAction);					//检查图号是否有效
DWORD realGetBitmapNo( DWORD num);						//从图档编号转回bnum
BOOL realGetPos( DWORD GraphicNo, short *x, short *y);	//取回GraphicNo的x和y offset
BOOL realGetWH( DWORD GraphicNo, short *w, short *h);	//取回GraphicNo的宽和高
BOOL realGetHitPoints( DWORD GraphicNo, short *HitX, short *HitY);	//取回GraphicNo的实际宽和高
BOOL realGetPrioType( DWORD GraphicNo , short *prioType);	//取回GraphicNo的优先权种类
BOOL realGetHitFlag( DWORD GraphicNo, short *Hit);			//取回GraphicNo的Hit
BOOL realGetHeightFlag( DWORD GraphicNo, short *Height);	//取回GraphicNo是否有高度
int realGetSoundEffect( DWORD GraphicNo);					//取回GraphicNo的环境音
int realGetWalkSoundEffect( DWORD GraphicNo);				//取回GraphicNo的脚步声
LPBYTE ReadRealBin( DWORD GraphicNo);
//画出realbin
BOOL DrawScaleImage( DWORD GraphicNo, int screenx, int screeny, int screenw, int screenh, LPWORD dest,
					int destw, int desth, int pitch, LPWORD pal);
*/
#endif /*_LOADREALBIN_H_*/
