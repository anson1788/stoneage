#define UNPACK2	0
#define WIN32_LEAN_AND_MEAN
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/loadrealbin.h"
#include "../systeminc/unpack.h"

#include <io.h>
#include <time.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char autoMapColorTbl[MAX_GRAPHICS];	// ?????????????????

#ifdef _STONDEBUG_
extern int g_iMallocCount;
#endif

unsigned long bitmapnumbertable[MAX_GRAPHICS];

#ifdef _PTTERN_SEPARATION_BIN
extern int MaxAdrnID;
FILE *Realbinfp[MAX_GRAPHICS] = {NULL};
#else
FILE *Realbinfp;
#endif

FILE *Addrbinfp;
ADRNBIN adrnbuff[MAX_GRAPHICS];
#ifdef _READ16BITBMP
AddressBin_s adrntruebuff[MAX_GRAPHICS_24];
HANDLE hAdrntrueFile;
HANDLE hRealtrueFile;
RGBQUAD g_rgbPal[256];
#endif
void initAutoMapColor( char *addrbinfilename )
{
	char *filename = "data\\auto.dat";
	if( readAutoMapColor( filename, addrbinfilename ) == 0 ){
		makeAutoMapColor();
		writeAutoMapColor( filename, addrbinfilename );
	}
}

void makeAutoMapColor( void )
{
	unsigned int i;
	int chgTbl[20] = {
		166,  61,  80, 112,  96, 189, 112,  60, 164, 164,
		111,   0,   0,   0,   0,   0,   0,   0,   0,   0	};
	for( i = 0; i < MAX_GRAPHICS; i++ ){
		if( adrnbuff[i].attr.bmpnumber != 0 ){
			if( 100 <= adrnbuff[i].attr.bmpnumber && adrnbuff[i].attr.bmpnumber <= 19999 ){
				autoMapColorTbl[adrnbuff[i].attr.bmpnumber] = getAutoMapColor( adrnbuff[i].bitmapno );
			}else if( 60 <= adrnbuff[i].attr.bmpnumber && adrnbuff[i].attr.bmpnumber <= 79 ){
				autoMapColorTbl[adrnbuff[i].attr.bmpnumber] = (unsigned char)chgTbl[adrnbuff[i].attr.bmpnumber-60];
			}else{
				autoMapColorTbl[adrnbuff[i].attr.bmpnumber] = 0;
			}
		}else{
			autoMapColorTbl[adrnbuff[i].attr.bmpnumber] = 0;
		}
	}
}

int writeAutoMapColor( char *wFName, char *addrbinfilename )
{
	FILE *wfp, *rfp;
	int rfh;
	struct _stat statBuf;
	int adrnNo;
	unsigned int adrnTime;
	unsigned short autoMapColorVersion = 4;	
	char *tmpStr;

	// adrn.bin???????????
	tmpStr = strstr( addrbinfilename, "adrn" );
	if( tmpStr == NULL )
		return 0;	// ??䥺??????
	if( tmpStr[4] == '.' )
		adrnNo = 0;	// ?????????
	else{
		adrnNo = -1;
		sscanf_s( tmpStr, "adrn_%d.bin", &adrnNo );
		if( adrnNo < 0 )
			return 0;
	}
	// adrn.bin??????
	if( (rfp = fopen( addrbinfilename, "rb" )) == NULL )
		return 0;
	rfh = _fileno( rfp );
	if( _fstat( rfh, &statBuf ) < 0 ){
		fclose( rfp );
		return 0;
	}
	adrnTime = (UINT)statBuf.st_ctime;
	fclose( rfp );
	// ???????
	if( (wfp = fopen( wFName, "wb" )) == NULL )
		return 0;
	fwrite( &autoMapColorVersion, sizeof( autoMapColorVersion ), 1, wfp );
	fwrite( &adrnNo, sizeof( adrnNo ), 1, wfp );
	fwrite( &adrnTime, sizeof( adrnTime ), 1, wfp );
	fwrite( &autoMapColorTbl, sizeof( autoMapColorTbl ), 1, wfp );
	fclose( wfp );
	return 1;
}

int readAutoMapColor( char *wFName, char *addrbinfilename )
{
	FILE *rfp;
	int rfh;
	struct _stat statBuf;
	int adrnNo, rAdrnNo;
	unsigned int adrnTime, rAdrnTime;
	unsigned short autoMapColorVersion = 4, rAutoMapColorVersion;
	char *tmpStr;
	// adrn.bin???????
	tmpStr = strstr( addrbinfilename, "adrn" );
	if( tmpStr == NULL )
		return 0;	// ??䥺??????
	if( tmpStr[4] == '.' )
		adrnNo = 0;	// ?????????
	else{
		adrnNo = -1;
		sscanf_s( tmpStr, "adrn_%d.bin", &adrnNo );
		if( adrnNo < 0 )
			return 0;
	}
	// adrn.bin??
	if( (rfp = fopen( addrbinfilename, "rb" )) == NULL )
		return 0;
	rfh = _fileno( rfp );
	if( _fstat( rfh, &statBuf ) < 0 ){
		fclose( rfp );
		return 0;
	}
	adrnTime = (UINT)statBuf.st_ctime;
	fclose( rfp );
	if( (rfp = fopen( wFName, "rb" )) == NULL )
		return 0;

	if( fread( &rAutoMapColorVersion, sizeof( rAutoMapColorVersion ), 1, rfp ) != 1 ){
		fclose( rfp );
		return 0;
	}
	if( autoMapColorVersion != rAutoMapColorVersion ){
		fclose( rfp );
		return 0;
	}
	// adrn.bin????????
	if( fread( &rAdrnNo, sizeof( rAdrnNo ), 1, rfp ) != 1 ){
		fclose( rfp );
		return 0;
	}
	if( adrnNo != rAdrnNo ){
		fclose( rfp );
		return 0;
	}
	// adrn.bin?????
	if( fread( &rAdrnTime, sizeof( rAdrnTime ), 1, rfp ) != 1 ){
		fclose( rfp );
		return 0;
	}
	if( adrnTime != rAdrnTime ){
		fclose( rfp );
		return 0;
	}
	if( fread( &autoMapColorTbl, sizeof( autoMapColorTbl ), 1, rfp ) != 1 ){
		fclose( rfp );
		return 0;
	}
	fclose( rfp );
	return TRUE;
}

BOOL initRealbinFileOpen(char *realbinfilename, char *addrbinfilename)
{
	ADRNBIN tmpadrnbuff;
	if ((Addrbinfp = fopen(addrbinfilename, "rb"))==NULL)
		return FALSE;
#ifdef _PTTERN_SEPARATION_BIN
	FILE *Realfp;
	if ((Realfp = fopen(realbinfilename, "rb"))==NULL)
		return FALSE;
#else
	if ((fopen_s(&Realbinfp,realbinfilename, "rb"))!=NULL)
		return FALSE;
#endif
	//adrn.bin
	while(!feof(Addrbinfp)){
		fread(&tmpadrnbuff, sizeof(tmpadrnbuff), 1, Addrbinfp);
		adrnbuff[tmpadrnbuff.bitmapno] = tmpadrnbuff;

#ifdef _PTTERN_SEPARATION_BIN
		Realbinfp[tmpadrnbuff.bitmapno] = Realfp;
		if((ULONG)MaxAdrnID < tmpadrnbuff.bitmapno){
			MaxAdrnID = tmpadrnbuff.bitmapno;
		}
#endif
		if( tmpadrnbuff.attr.bmpnumber != 0 ){
			if( (12802 <= tmpadrnbuff.attr.bmpnumber && tmpadrnbuff.attr.bmpnumber <= 12811)
			 || (10132 <= tmpadrnbuff.attr.bmpnumber && tmpadrnbuff.attr.bmpnumber <= 10136) ){
				adrnbuff[tmpadrnbuff.bitmapno].attr.hit =
					300 + (adrnbuff[tmpadrnbuff.bitmapno].attr.hit % 100);
			}
			if( tmpadrnbuff.attr.bmpnumber<=33 && tmpadrnbuff.bitmapno>230000){//防堵魔法图号覆盖声音的bug
				continue;
			}
			bitmapnumbertable[tmpadrnbuff.attr.bmpnumber] = tmpadrnbuff.bitmapno;
		}else
			bitmapnumbertable[tmpadrnbuff.attr.bmpnumber] = 0;
	}
	fclose(Addrbinfp);
	return TRUE;
}

void cleanupRealbin(void)
{
#ifdef _PTTERN_SEPARATION_BIN
	for(int i = 0; i < MAX_GRAPHICS; i ++){
		if(Realbinfp[i] != NULL){
			fclose(Realbinfp[i]);
			Realbinfp[i] = NULL;
		}
	}
#else
	fclose(Realbinfp);
#endif

#ifdef _READ16BITBMP
	CloseHandle(hRealtrueFile);
#endif
}

BOOL realGetPos(U4 GraphicNo , S2 *x , S2 *y)
{
#ifndef _READ16BITBMP
	if(GraphicNo<0 || GraphicNo>=MAX_GRAPHICS){*x=0;*y=0;return FALSE;}
	*x = adrnbuff[GraphicNo].xoffset;
	*y = adrnbuff[GraphicNo].yoffset;
#else
	if(GraphicNo < 0) {*x=0;*y=0;return FALSE;}
	if(GraphicNo >= OLD_GRAPHICS_START){
		if(GraphicNo > MAX_GRAPHICS) {*x=0;*y=0;return FALSE;}
		*x = adrntruebuff[GraphicNo - OLD_GRAPHICS_START].xoffset;
		*y = adrntruebuff[GraphicNo - OLD_GRAPHICS_START].yoffset;
	}
	else{
		*x = adrnbuff[GraphicNo].xoffset;
		*y = adrnbuff[GraphicNo].yoffset;
	}
#endif
	return TRUE;
}

BOOL realGetWH(U4 GraphicNo , S2 *w , S2 *h)
{
#ifndef _READ16BITBMP
	if(GraphicNo<0 || GraphicNo>=MAX_GRAPHICS){*w=0;*h=0;return FALSE;}
	*w = adrnbuff[GraphicNo].width;
	*h = adrnbuff[GraphicNo].height;
#else
	if(GraphicNo < 0){*w=0;*h=0;return FALSE;}
	if(GraphicNo >= OLD_GRAPHICS_START){
		if(GraphicNo > MAX_GRAPHICS) {*w=0;*h=0;return FALSE;}
		*w = adrntruebuff[GraphicNo - OLD_GRAPHICS_START].width;
		*h = adrntruebuff[GraphicNo - OLD_GRAPHICS_START].height;
	}
	else{
		*w = adrnbuff[GraphicNo].width;
		*h = adrnbuff[GraphicNo].height;
	}
#endif

	return TRUE;
}

BOOL realGetHitPoints(U4 GraphicNo , S2 *HitX , S2 *HitY)
{
	if(GraphicNo<0 || GraphicNo>=MAX_GRAPHICS){*HitX=0;*HitY=0;return FALSE;}

	*HitX = adrnbuff[GraphicNo].attr.atari_x;
	*HitY = adrnbuff[GraphicNo].attr.atari_y;

	return TRUE;
}

BOOL realGetHitFlag(U4 GraphicNo , S2 *Hit)
{
	if(GraphicNo<0 || GraphicNo>=MAX_GRAPHICS){
		*Hit=0;
		return FALSE;
	}

	if ((GraphicNo >= 369715 && GraphicNo <= 369847) || GraphicNo == 369941)//强制地表可走
		*Hit = 1;
	else if (GraphicNo >= 369641 && GraphicNo <= 369654)
		*Hit = 1;
	else
		*Hit = (adrnbuff[GraphicNo].attr.hit % 100);

	return TRUE;
}

BOOL realGetPrioType(U4 GraphicNo , S2 *prioType)
{
	if(GraphicNo<0 || GraphicNo>=MAX_GRAPHICS){
		*prioType=0;
		return FALSE;
	}

	*prioType = (adrnbuff[GraphicNo].attr.hit / 100);
	return TRUE;
}

BOOL realGetHeightFlag(U4 GraphicNo , S2 *Height)
{
	if(GraphicNo<0 || GraphicNo>=MAX_GRAPHICS){*Height=0;return FALSE;}
	*Height = adrnbuff[GraphicNo].attr.height;

	return TRUE;
}

int realGetSoundEffect(U4 GraphicNo)
{
	if(GraphicNo<0 || GraphicNo>=MAX_GRAPHICS)return FALSE;
	return adrnbuff[bitmapnumbertable[GraphicNo]].attr.effect1;

}

BOOL realGetWalkSoundEffect(U4 GraphicNo )
{
	if(GraphicNo<0 || GraphicNo>=MAX_GRAPHICS){return FALSE;}
	return adrnbuff[GraphicNo].attr.effect2;

}

BOOL realGetNo( U4 CharAction , U4 *GraphicNo )
{
#ifndef _READ16BITBMP
	if(CharAction<0 || CharAction>=MAX_GRAPHICS){*GraphicNo=0;return FALSE;}
	*GraphicNo = bitmapnumbertable[CharAction];
	return TRUE;
#else
	if(CharAction < 0){
		*GraphicNo = 0;
		return FALSE;
	}
	if(CharAction >= OLD_GRAPHICS_START){
		if(CharAction >= MAX_GRAPHICS){
			*GraphicNo = 0;
			return FALSE;
		}
		*GraphicNo = CharAction;
		return TRUE;
	}
	else *GraphicNo = bitmapnumbertable[CharAction];
	return TRUE;
#endif
}

int realGetBitmapNo( int num )
{
#ifndef _READ16BITBMP
	if( num < 0 || num >= MAX_GRAPHICS ) return -1;
	return adrnbuff[num].attr.bmpnumber;
#else
	if(num < 0) return -1;
	if(num >= OLD_GRAPHICS_START){
		if(num >= MAX_GRAPHICS) return -1;
		return adrntruebuff[num - OLD_GRAPHICS_START].bitmapno;
	}
	else return adrnbuff[num].attr.bmpnumber;
#endif
}

///////////////////////////////////////////////////////////////////??????????
#define REALGETIMAGEMAXSIZE 1600*1600
BYTE g_realgetimagebuf[REALGETIMAGEMAXSIZE];
BYTE g_realgetimagebuf2[REALGETIMAGEMAXSIZE];
#ifdef _NEW_COLOR_
int NewColor16Flg;
#endif
BOOL realGetImage( int graphicNo, unsigned char **bmpdata, int *width, int *height)
{
#ifdef _NEW_COLOR_
	NewColor16Flg=0;
#endif
	ADRNBIN adrdata;
	if(graphicNo<0 || graphicNo>=MAX_GRAPHICS)return FALSE;
	adrdata=adrnbuff[graphicNo];
#ifdef _PTTERN_SEPARATION_BIN
	if (Realbinfp[graphicNo]==NULL) return FALSE;
	fseek(Realbinfp[graphicNo], adrdata.adder, SEEK_SET);//real.bin??????????? 
	if( fread(&g_realgetimagebuf, adrdata.size, 1, Realbinfp[graphicNo] ) != 1 )
		return FALSE;
#else
	fseek(Realbinfp, adrdata.adder, SEEK_SET);//real.bin??????????? 
	if( fread(&g_realgetimagebuf, adrdata.size, 1, Realbinfp ) != 1 )
		return FALSE;
#endif
	unsigned int len;
	*bmpdata = g_realgetimagebuf2;
	if( decoder( g_realgetimagebuf, bmpdata,
			(unsigned int*)width, (unsigned int*)height, &len ) == NULL ){
		return FALSE;
	}
	return TRUE;
}

#ifdef _READ16BITBMP
int InitRealTruebinFileOpen(char *szRealTrueBinFileName,char *szAdrnTruebinFileName)
{
	BOOL bReadReturn;
	AddressBin_s Addr;
	DWORD dwReadByte;

	ZeroMemory(adrntruebuff,sizeof(adrntruebuff));
	// 开档
	hAdrntrueFile = CreateFile(szAdrnTruebinFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hAdrntrueFile == INVALID_HANDLE_VALUE) return -1;
	hRealtrueFile = CreateFile(szRealTrueBinFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hRealtrueFile == INVALID_HANDLE_VALUE){
		CloseHandle(hAdrntrueFile);
		return -2;
	}
	
	while(1){
		// 把档案内的资料读入
		bReadReturn = ReadFile(hAdrntrueFile,&Addr,sizeof(AddressBin_s),&dwReadByte,NULL);
		// 资料读完了
		if(bReadReturn && dwReadByte == 0) break;
		// 错误
		if(dwReadByte == 0) break;
		memcpy(&adrntruebuff[Addr.bitmapno],&Addr,sizeof(AddressBin_s));
	}
	CloseHandle(hAdrntrueFile);
	return 0;
}

// 传入的BmpNo 值必须是减去 OLD_GRAPHICS_START 后的值,才能在全彩的图档里找到图
BOOL Read16BMP(int BmpNo,unsigned char **BmpData,int *width,int *height,BYTE **AlphaData,BOOL *useAlpha)
{
	BOOL bRet = TRUE,bReadReturn;
	AddressBin_s *pAddr;
	unsigned char *pBmpData;
	unsigned int len,iw = 0,ih = 0;
	DWORD dwReadByte;

	if(BmpNo > MAX_GRAPHICS_24) return FALSE;
	pAddr = &adrntruebuff[BmpNo];
	// 移到要读取的图档资料位置上
	SetFilePointer(hRealtrueFile,pAddr->adder,NULL,FILE_BEGIN);
	pBmpData = (unsigned char*)MALLOC(pAddr->size);
#ifdef _STONDEBUG_
	g_iMallocCount++;
#endif
	if(pBmpData == NULL) return FALSE;
	else{
		memset(g_rgbPal,0,sizeof(g_rgbPal));
		// 先读入色盘资料
		bReadReturn = ReadFile(hRealtrueFile,g_rgbPal,pAddr->palSize+sizeof(RGBQUAD),&dwReadByte,NULL);
		// 读档失败
		if(bReadReturn && dwReadByte == 0) bRet = FALSE;
		// 把图档资料读入
		bReadReturn = ReadFile(hRealtrueFile,pBmpData,pAddr->size,&dwReadByte,NULL);
		// 读档失败
		if(bReadReturn && dwReadByte == 0) bRet = FALSE;
		else{
			*BmpData = g_realgetimagebuf2;
			if(decoder(pBmpData,BmpData,(unsigned int*)width,(unsigned int*)height,&len) == NULL) bRet = FALSE;
		}
		FREE(pBmpData);
#ifdef _STONDEBUG_
		g_iMallocCount--;
#endif
		// 把alpha资料读入
		*useAlpha = FALSE;
		if(pAddr->alpha_size > 0){
			pBmpData = (unsigned char*)MALLOC(pAddr->alpha_size);
#ifdef _STONDEBUG_
			g_iMallocCount++;
#endif
			if(pBmpData == NULL) return FALSE;
			bReadReturn = ReadFile(hRealtrueFile,pBmpData,pAddr->alpha_size,&dwReadByte,NULL);
			// 读档失败
			if(bReadReturn && dwReadByte == 0) bRet = FALSE;
			else{
				*AlphaData = g_realgetimagebuf;
				if(decoder(pBmpData,AlphaData,&iw,&ih,&len) == NULL) bRet = FALSE;
			}
			FREE(pBmpData);
#ifdef _STONDEBUG_
			g_iMallocCount--;
#endif
			*useAlpha = TRUE;
		}
	}

	return bRet;
}
#endif
