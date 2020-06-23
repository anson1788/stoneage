/*-----------------------------------------------
   Message.cpp -- Message format definition
               (c) 许百胜Cary Hsu, 1999.8.18
-----------------------------------------------*/
#include <stdio.h>
#include "../systeminc/version.h"
#include "Message.h"
#include "descrypt.h"

//DWORD dwCafeKey=0;

//for AnalyzeMessage()
enum
{
	ZERO,
	READHEAD,
	READLEN,
	READPARAM,
	GETHEAD
};

int	iEncrypt=E_INIT;
Parameter	*messNewTailParam;
MessNode	*messNew;
MessNode	*messHead;
MessNode	*messTail;
//制作讯息格式时使用
int			iFormatLen;				//此讯息格式的长度,以DWORD为单位
int			iNumStart=0;
char		szFormatBuf[1024];
//分析时使用
char		szAnalyzeBuf[1024];
int			nState;
BOOL		backslash;
BOOL		bAnd;
int			indexBuf;
int			nParamCounter;

extern TCHAR	P_Key[];
extern TCHAR	szPid[];

const DWORD RET_OK=1;
const DWORD RET_MEMORY=2;
const DWORD RET_PARAM=3;

#ifdef _OMIT_WGS
LPTSTR O_WGS_Format()
{
	szFormatBuf[0]='&';
	szFormatBuf[1]=';';
	szFormatBuf[2]='0';
	szFormatBuf[3]=';';
	szFormatBuf[4]='Q';
	szFormatBuf[5]=';';
	iFormatLen = 6;
	return szFormatBuf;
}
#endif

inline void StartFormat()
{
	szFormatBuf[0]='&';
	szFormatBuf[1]=';';
	lstrcpy(szFormatBuf+2,szPid);
	iNumStart=lstrlen(szPid)+2;
	szFormatBuf[iNumStart++]=';';
	szFormatBuf[iNumStart++]='0';
	szFormatBuf[iNumStart++]='0';
	szFormatBuf[iNumStart++]='0';
	szFormatBuf[iNumStart]='1';
	szFormatBuf[iNumStart+1]=';';
	iFormatLen=iNumStart+2;
}

inline void FinishFormat()
{
	int j;
	if((j=iFormatLen & 0x07)>0){
		for(;j<8;j++)
			szFormatBuf[iFormatLen++]='&';
	}
}

inline void FormatAdd()
{
	char *ch=&szFormatBuf[iNumStart];
	(*ch)++;
	if(*ch>'9'){
		*ch='0';
		if((*(ch-=1)+=1)>'9'){
			*ch='0';
			if((*(ch-=1)+=1)>'9'){
				*ch='0';
				if((*(ch-=1)+=1)>'9')
					*ch=szFormatBuf[3]=szFormatBuf[4]=szFormatBuf[5]='9';
			}
		}
	}
}

void AddParamForFormat(LPSTR lpstr,int iLen,DWORD flag)
{
	if(flag & FORMAT_START)
		StartFormat();
	for(int j=0;j<iLen;j++){
		if(('\\'==lpstr[j])||(';'==lpstr[j])||('&'==lpstr[j]))
			szFormatBuf[iFormatLen++]='\\';
		szFormatBuf[iFormatLen++]=lpstr[j];
	}
	szFormatBuf[iFormatLen++]=';';
	FormatAdd();
	if(flag & FORMAT_FINISH)
		FinishFormat();
}

void AddParamForFormat(char ch,DWORD flag)
{
	if(flag & FORMAT_START)
		StartFormat();
	if(('\\'==ch)||(';'==ch)||('&'==ch))
		szFormatBuf[iFormatLen++]='\\';
	szFormatBuf[iFormatLen++]=ch;
	szFormatBuf[iFormatLen++]=';';
	FormatAdd();
	if(flag & FORMAT_FINISH)
		FinishFormat();
}

LPTSTR GetString(int &iLen,LPTSTR lpstrEncrypt)
{
	ecb_crypt(lpstrEncrypt,szFormatBuf,iFormatLen,DES_ENCRYPT);
	iLen=iFormatLen;
	return szFormatBuf;
}

inline void ReleaseParam(Parameter *param)
{
	if(param->lpstrParam)
		delete[] param->lpstrParam;
	delete param;
}

inline void LinkParamToMessNew(Parameter *paramNew)
{
	if(messNewTailParam)
	{
		messNewTailParam->next=paramNew;
		messNewTailParam=paramNew;
	}
	else
		messNew->param=messNewTailParam=paramNew;
}

inline void LinkMessNewToMessTail()
{
	if(messTail)
	{
		messTail->next=messNew;
		messTail=messNew;
	}
	else
		messTail=messHead=messNew;
	messNew=NULL;
	messNewTailParam=NULL;
}

inline void ReleaseMessNew()
{
	register Parameter *paramTemp;
	if(messNew)
	{
		while(messNew->param)
		{
			paramTemp=messNew->param->next;
			ReleaseParam(messNew->param);
			messNew->param=paramTemp;
		}
		delete messNew;
		messNew=NULL;
		messNewTailParam=NULL;
	}
}

inline DWORD HandleZero(LPSTR &lpstr,int &iStrLen)
{
	indexBuf=0;
	nParamCounter=0;
	backslash=FALSE;
	bAnd=FALSE;
	nState=READHEAD;
	
	if(messNew=new MessNode)
		return RET_OK;
	return RET_MEMORY;
}

inline void HandleHead(LPSTR &lpstr,int &iStrLen)
{
	char ch;
	while(iStrLen-- > 0)
	{
		if(bAnd)
		{
			if(';'==(ch=*lpstr++))
			{
				bAnd=FALSE;
				nState=READLEN;
				return;
			}
			else if('&'!=ch)
				bAnd=FALSE;
		}
		else
		{
			if('&'==*lpstr++)
				bAnd=TRUE;
		}
	}
}

inline DWORD HandleLen(LPSTR &lpstr,int &iStrLen)
{
	char	ch;
	while(iStrLen-- > 0)
	{
		if(';'==(ch=*lpstr++))
		{
			szAnalyzeBuf[indexBuf]='\0';
			nParamCounter=atoi(szAnalyzeBuf);
			if(!nParamCounter)
			{
				nState=ZERO;
				return RET_PARAM;
			}
			messNew->nParam=(nParamCounter-=1);
			indexBuf=0;
			nState=READPARAM;
			return RET_OK;
		}
		szAnalyzeBuf[indexBuf++]=ch;
		if(indexBuf>5)
		{
			nState=ZERO;
			return RET_PARAM;
		}
	}
	return RET_OK;
}

inline DWORD HandleParam(LPSTR &lpstr,int &iStrLen)
{
	char	ch;
	while(iStrLen-- > 0)
	{
		if(backslash)
		{
			backslash=FALSE;
			szAnalyzeBuf[indexBuf++]=*lpstr++;
		}
		else
		{
			if(';'==(ch=*lpstr++))
			{
				Parameter *paramNew=NULL;
				szAnalyzeBuf[indexBuf++]='\0';
				if((paramNew=new Parameter) && (paramNew->lpstrParam=new char[indexBuf+1]))
				{
					paramNew->next=NULL;
					CopyMemory(paramNew->lpstrParam,szAnalyzeBuf,indexBuf);
					paramNew->lpstrParam[indexBuf]='\0';
					paramNew->iLen=indexBuf-1;
					LinkParamToMessNew(paramNew);
				}
				else
				{
					if(paramNew)
						delete paramNew;
					ReleaseMessNew();
					return RET_MEMORY;
				}
				if(--nParamCounter<=0)
				{
					LinkMessNewToMessTail();
					nState=ZERO;
					return RET_OK;
				}
				indexBuf=0;
			}
			else
			{
				if('\\'==ch)
					backslash=TRUE;
				else if('&'==ch)
				{
					ReleaseMessNew();
					nState=ZERO;
					return RET_PARAM;
				}
				else
					szAnalyzeBuf[indexBuf++]=ch;
			}
		}
	}
	return RET_OK;
}

DWORD AnalyzeMessage(LPSTR lpstr,int iStrLen)
{
	if(0<iStrLen){
		switch(iEncrypt){
		case E_INIT:
			ecb_crypt(SZ_ECB,lpstr,iStrLen,DES_DECRYPT);
			break;
		case E_ECB:
			ecb_crypt(P_Key,lpstr,iStrLen,DES_DECRYPT);
			break;
		case E_NO:
			break;
		}
		lpstr[iStrLen]='\0';
#ifdef _STONDEBUG_
		FILE *fp;
		fopen_s(&fp, "recvtime.txt", "a+" );
		if( fp ) fprintf( fp, "%u RECV %d  %s\n", TimeGetTime(), iStrLen, lpstr );
		if( fp ) fclose( fp );
#endif	
		DWORD dw;
		while(0<iStrLen)
		{
			switch(nState)
			{
			case READPARAM:
				if(RET_OK!=(dw=HandleParam(lpstr,iStrLen)))
					return dw;
				break;
			case READLEN:
				if(RET_PARAM==HandleLen(lpstr,iStrLen))
					return RET_PARAM;
				break;
			case READHEAD:
				HandleHead(lpstr,iStrLen);
				break;
			case ZERO:
				if(RET_MEMORY==HandleZero(lpstr,iStrLen))
					return RET_MEMORY;
			}
		}
	}
	return RET_OK;
}

Parameter *ReleaseMessHeadParam()
{
	Parameter *paramTemp=NULL;
	if(messHead&&(messHead->param)){
		paramTemp=messHead->param->next;
		ReleaseParam(messHead->param);
		messHead->param=paramTemp;
	}
	return paramTemp;
}

void RemoveMessHead()
{
	register Parameter *paramTemp;
	MessNode	*messTemp;
	if(messHead)
	{
		messTemp=messHead->next;
		while(messHead->param)
		{
			paramTemp=messHead->param->next;
			ReleaseParam(messHead->param);
			messHead->param=paramTemp;
		}
		delete messHead;
		if(!messTemp)
			messTail=NULL;
		messHead=messTemp;
	}
}

void ReleaseMessList()
{
	ReleaseMessNew();
	nState=ZERO;
	MessNode *messTemp;
	register Parameter *paramTemp;
	while(messHead)
	{
		messTemp=messHead->next;
		while(messHead->param)
		{
			paramTemp=messHead->param->next;
			ReleaseParam(messHead->param);
			messHead->param=paramTemp;
		}
		delete messHead;
		messHead=messTemp;
	}
	messTail=NULL;
}

DWORD GetCafeNumber()
{
	HANDLE hFileMapping=NULL;
	LPSTR lpstr=NULL;
	DWORD Id=0;
#ifdef _NEW_EARTH
	if (hFileMapping = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, "vup2u4fu.6"))
	{
		int *pFlag;

		if (pFlag = (int*)MapViewOfFile(hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0))
			Id = *pFlag;
		CloseHandle(hFileMapping);
	}
#else
	char szTemp[64];
	if(hFileMapping=OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE,FALSE,"f; enc ")){
		if(lpstr=(LPSTR)MapViewOfFile(hFileMapping,FILE_MAP_READ | FILE_MAP_WRITE,0,0,0)){
			strcpy(szTemp,lpstr);
			ecb_crypt("lbgorj/",szTemp,8,DES_DECRYPT);
			UnmapViewOfFile((LPVOID)lpstr);
			Id=atoi(szTemp);
		}
		CloseHandle(hFileMapping);
	}
#endif
	return Id;
}
