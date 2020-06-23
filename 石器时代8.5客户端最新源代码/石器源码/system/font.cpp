#include "../systeminc/version.h"
#include "../systeminc/system.h"

#define ASCII(a) a-'A'+10
#define ASCII_DEC(a) a-'0'+ 35

#define FONT_BUFFER_SIZE 1024 	// ???????????

// ?????????
FONT_BUFFER FontBuffer[ FONT_BUFFER_SIZE ];

int FontCnt = 0;

int FontZenkauWidth;

int	FontHankakuWidth;
int MessageBoxNew(HWND hWnd,LPCSTR lpText,LPCSTR lpCaption,UINT uType);
#ifdef _SUNDAY_STR_SEARCH
char* sunday( char* str,  char* subStr)
{
	const int maxSize=256;
	int next[maxSize];
	int strLen = strlen(str);
	int subLen = strlen(subStr);
	int i,j,pos;
	for(i=0;i<maxSize;i++)
	{
		next[i] = subLen+1;
	}
	for(i=0;i<subLen;i++)
	{
		next[ (unsigned char)subStr[i] ] = subLen-i;//计算子串中的字符到字符串结尾的\0之间的距离
	}
	pos=0;
	while(pos<=(strLen-subLen))
	{
		i=pos;
		for(j=0;j<subLen;j++,i++)
		{
			
			if(str[i] != subStr[j])
			{
				pos += next[ (unsigned char)str[pos+subLen] ];//向后移动
				break;
			}
			
		}
		if(j==subLen)//找到字串，返回
		{
			return str+pos;
		}
	}
	return NULL;
}
#endif

#ifdef _FONT_SIZE
int StockFontBufferExt( int x, int y, char fontPrio, int color, char *str, BOOL hitFlag, int size )
{
	if( FontCnt >= FONT_BUFFER_SIZE ) return -2;
	FontBuffer[ FontCnt ].x = x;
	FontBuffer[ FontCnt ].y = y;
	FontBuffer[ FontCnt ].fontPrio = fontPrio;
	FontBuffer[ FontCnt ].color = color;
	FontBuffer[ FontCnt ].hitFlag = hitFlag;
	extern int 编码;
	extern int 繁体开关;
	if(繁体开关){
		char 繁体[1024]={0};
		LCMapString (0x804,0x4000000,str, strlen(str),繁体,1024);
		if(编码==950){
			extern char* GB2312ToBIG5(const char* szBIG5String);
			strcpy( FontBuffer[ FontCnt ].str, GB2312ToBIG5((const char *)繁体) );
		}else
			strcpy( FontBuffer[ FontCnt ].str, 繁体 );
	}else{
		if(编码==950){
			extern char* GB2312ToBIG5(const char* szBIG5String);
			strcpy( FontBuffer[ FontCnt ].str, GB2312ToBIG5((const char *)str) );
		}else
		strcpy( FontBuffer[ FontCnt ].str, str );
	}
	FontBuffer[ FontCnt ].size = size;
	return FontCnt++;
}
int StockFontBuffer( int x, int y, char fontPrio, int color, char *str, BOOL hitFlag )
{
	return StockFontBufferExt( x, y, fontPrio, color, str, hitFlag, 0 );
}
#else

int StockFontBuffer( int x, int y, char fontPrio, int color, char *str, BOOL hitFlag )
{
	if( FontCnt >= FONT_BUFFER_SIZE ) return -2;
	FontBuffer[ FontCnt ].x = x;
	FontBuffer[ FontCnt ].y = y;
	FontBuffer[ FontCnt ].fontPrio = fontPrio;
	FontBuffer[ FontCnt ].color = color;
	FontBuffer[ FontCnt ].hitFlag = hitFlag;
	extern int 编码;
	extern int 繁体开关;
	if(繁体开关){
		char 繁体[1024]={0};
		LCMapString (0x804,0x4000000,str, strlen(str),繁体,1024);
		strcpy( FontBuffer[ FontCnt ].str, 繁体 );
	}else{
		strcpy( FontBuffer[ FontCnt ].str, str );
	}
	return FontCnt++;
}
#endif
HDC FontSizeHdc=NULL;
HDC hScrDC=NULL;
void CreatFontHdc()
{
	hScrDC = CreateDC("FontHdc", NULL, NULL, NULL);
	FontSizeHdc = CreateCompatibleDC(hScrDC);
	if(FontSizeHdc){
#ifdef _NEWFONT_
		extern int 编码;
		char strfame[128];
		if(编码==950){
			sprintf(strfame,"Microsoft JhengHei");
		}else{
			sprintf(strfame,"Microsoft JhengHei");
		}
		HFONT font=CreateFont(FONT_SIZE1,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,1,
			0,0,0,17,(LPCTSTR)strfame);
#else
				extern int 编码;
		char strfame[128];
		HFONT font;
		if(编码==950){
			sprintf(strfame,"Microsoft JhengHei");
			font=CreateFont(FONT_SIZE2,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,1,
			0,0,0,17,(LPCTSTR)strfame);
		}else{
			font=CreateFont(FONT_SIZE1,0,0,0,400,FALSE,FALSE,FALSE,134,
			OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH|FF_ROMAN,(LPCTSTR)"宋体");
		}


#endif
		SelectObject(FontSizeHdc, font);
	}else{
		MessageBoxNew(NULL,"创建HDC错误！","ＳｔｏｎｅＡｇｅ",NULL);
		exit(0);
	}
}
int getTextLength(char * str)
{
	SIZE fontsize;
	GetTextExtentPoint32(FontSizeHdc,(LPCSTR)str,strlen(str),(LPSIZE)&fontsize);
	return fontsize.cx;
}


#ifdef _MO_CHAT_EXPRESSION
void delFontBuffer(CHAT_BUFFER *chatbuffer)
{

	CHAT_BUFFER *pHAIH = chatbuffer->NextChatBuffer;
	while(pHAIH)	//如果链中还存在结点
	{
		CHAT_BUFFER* pTemp;
		pTemp = pHAIH;
		pHAIH = pHAIH->NextChatBuffer;
		free(pTemp);
	}
	chatbuffer->NextChatBuffer=NULL;
}



void NewStockFontBuffer(CHAT_BUFFER *chatbuffer,int x,unsigned char color,char *str,int size)
{
	if(!str[0]){
		return;
	}
	memset(chatbuffer,0,sizeof(CHAT_BUFFER));
	chatbuffer->fontsize = size;
	char outText[512];
	char *temp = sunday(str, "#");
	if (temp){
		if(temp!=str){
			int strl = temp-str;
			memcpy(outText,str,strl);
			outText[strl]=0x0;
			SIZE fontsize;
			GetTextExtentPoint32(FontSizeHdc,(LPCSTR)outText,strl,(LPSIZE)&fontsize);
			chatbuffer->color = color;
			chatbuffer->x = x;
			strcpy(chatbuffer->buffer,outText);
			chatbuffer->NextChatBuffer =  (CHAT_BUFFER *)calloc(1,sizeof(CHAT_BUFFER));
			NewStockFontBuffer(chatbuffer->NextChatBuffer,x+fontsize.cx,color,temp,size);
		}else{
			int cnt_int = 0;
			int i=1;
			for(i;i<4;i++){
				if(temp[i]>='0'&& temp[i]<='9'){
					cnt_int*=10;
					cnt_int+=temp[i]-'0'; 
				}else{
					break;
				}
			}
			if(cnt_int>0&&cnt_int<=EXPRESSION_NOID_NUM+1){
				chatbuffer->x = x;
				chatbuffer->BmpNo = EXPRESSION_NOID_START+cnt_int-1;
				temp+=i; 
				x+=26;
			}else{
				memcpy(outText,temp,i);
				outText[i]=0x0;
				SIZE fontsize;
				GetTextExtentPoint32(FontSizeHdc,(LPCSTR)outText,i,(LPSIZE)&fontsize);
				chatbuffer->color = color;
				chatbuffer->x = x;
				strcpy(chatbuffer->buffer,outText);
				x+=fontsize.cx;
				temp+=i;
			}
			chatbuffer->NextChatBuffer =  (CHAT_BUFFER *)calloc(1,sizeof(CHAT_BUFFER));
			NewStockFontBuffer(chatbuffer->NextChatBuffer,x,color,temp,size);
		}
	}else{
		chatbuffer->color = color;
		chatbuffer->x = x;
		strcpy(chatbuffer->buffer,str);
	}
}
#endif

void StockFontBuffer2( STR_BUFFER *strBuffer )
{
	int lineDist = 0; //行距
	int splitPoint = 0;
	int bakSplitPoint,cursor;
	BOOL SetCursor=FALSE;
	char splitStr[ 256 ];
	if( FontCnt >= FONT_BUFFER_SIZE ){ 
		strBuffer->hitFontNo = -2;
		return;
	}
	//需要分行时
	if( strBuffer->lineLen != 0 ){
		cursor=strBuffer->cursor;
		while( strlen( strBuffer->buffer + splitPoint ) >= strBuffer->lineLen ){
			bakSplitPoint=splitPoint;
			//copy一行的字串
			strncpy_s( splitStr, strBuffer->buffer + splitPoint, strBuffer->lineLen );
			*( splitStr + strBuffer->lineLen ) = NULL;
			if( GetStrLastByte( splitStr ) == 3 ){
				//分割到中DBCS时，退回一个byte
				splitPoint = strBuffer->lineLen - 1 + splitPoint;
				*( splitStr + strBuffer->lineLen - 1 ) = NULL; 
			}else
				splitPoint = strBuffer->lineLen + splitPoint;
			StockFontBuffer( strBuffer->x, strBuffer->y + lineDist, strBuffer->fontPrio, 0, splitStr, 0 );
			if(cursor>=bakSplitPoint && cursor<splitPoint){
				strBuffer->imeX = strBuffer->x + (cursor-bakSplitPoint)*(FONT_SIZE>>1);
				strBuffer->imeY = strBuffer->y + lineDist;
				SetCursor=TRUE;
			}
			lineDist += strBuffer->lineDist;
		}
		if(!SetCursor){
			strBuffer->imeX = strBuffer->x + (strBuffer->cursor-splitPoint)*(FONT_SIZE>>1);
			strBuffer->imeY = strBuffer->y + lineDist;
		}
		StockFontBuffer( strBuffer->x, strBuffer->y + lineDist, strBuffer->fontPrio, 0, strBuffer->buffer + splitPoint, 0 );
	}else{
		FontBuffer[ FontCnt ].x = strBuffer->x;
		FontBuffer[ FontCnt ].y = strBuffer->y;
		FontBuffer[ FontCnt ].fontPrio = strBuffer->fontPrio;
		FontBuffer[ FontCnt ].color = strBuffer->color;
		FontBuffer[ FontCnt ].hitFlag = 0;
#ifdef _FONT_SIZE
		FontBuffer[ FontCnt ].size = 0;
#endif
//定义Hook_type用来处理星号产生
#ifdef _SAHOOK //Syu ADD Hook程式
		if( strBuffer->filterFlag == HOOK_TYPE){
			extern int HOOK_PASSWD_NUM;
			for(int i = 0 ; i < HOOK_PASSWD_NUM ; i++)
				FontBuffer[ FontCnt ].str[ i ] = '*';
			FontBuffer[ FontCnt].str[i]=NULL;
			strBuffer->cursor = HOOK_PASSWD_NUM;
		}
		else if( strBuffer->filterFlag == BLIND_TYPE ){
#else
		if( strBuffer->filterFlag == BLIND_TYPE ){
#endif
			int i;
			for(i = 0 ; i < strBuffer->cnt ; i++ )
				FontBuffer[ FontCnt ].str[ i ] = '*';
			FontBuffer[ FontCnt ].str[ i ] = NULL;
		}else{
			extern int 编码;
			if(编码==950){
				extern char* GB2312ToBIG5(const char* szBIG5String);
				strcpy( FontBuffer[ FontCnt ].str, GB2312ToBIG5((const char *)strBuffer->buffer));
			}else
				strcpy( FontBuffer[ FontCnt ].str, strBuffer->buffer);
		}
#ifdef _NEWFONT_
		char strtemp[512];
		char *ptempstr;
		if( strBuffer->filterFlag == BLIND_TYPE ){
			ptempstr = FontBuffer[ FontCnt ].str;
		}else{
			ptempstr = strBuffer->buffer;
		}
		if(strBuffer->cursor==0){
			strBuffer->imeX = strBuffer->x;
		}else{
			memcpy(strtemp,ptempstr,strBuffer->cursor);
			strtemp[strBuffer->cursor]=0;
			strBuffer->imeX = strBuffer->x + GetStrWidth(strtemp);		
		
		}
#else
		strBuffer->imeX = strBuffer->x + strBuffer->cursor*(FONT_SIZE>>1);
#endif
		strBuffer->imeY = strBuffer->y;
		strBuffer->hitFontNo = FontCnt++;
	}
}
//ONLINEGM USE
int StockFontBuffer3( STR_BUFFER *strBuffer )
{
	int lineDist = 0; //行距
	int splitPoint = 0;
	int bakSplitPoint,cursor;
	BOOL SetCursor=FALSE;
	char splitStr[ 256 ];
	
	cursor=strBuffer->cursor;
	//需要分行时
	while( strlen( strBuffer->buffer + splitPoint ) >= (unsigned)strBuffer->lineLen-1 ){
		bakSplitPoint=splitPoint;
		strcpy( splitStr , strBuffer->buffer + splitPoint );  //一个字
		*( splitStr + strBuffer->lineLen ) = NULL;  //指向最后
		splitPoint = strBuffer->lineLen + splitPoint;
		StockFontBuffer( strBuffer->x, strBuffer->y , strBuffer->fontPrio, 0 , splitStr, 0 );

		if(cursor>=bakSplitPoint && cursor<splitPoint){
			strBuffer->imeX = strBuffer->x + (cursor-bakSplitPoint)*(FONT_SIZE>>1);
			strBuffer->imeY = strBuffer->y + lineDist;
			SetCursor=TRUE;
		}
		lineDist += strBuffer->lineDist;	
		return 1;
	}
	if(!SetCursor){  // 游标不需换行
		strBuffer->imeX = strBuffer->x + (strBuffer->cursor-splitPoint)*(FONT_SIZE>>1);
		strBuffer->imeY = strBuffer->y + lineDist;
	}
	StockFontBuffer( strBuffer->x, strBuffer->y + lineDist, strBuffer->fontPrio, 0 , strBuffer->buffer + splitPoint, 0 );
	return 0;
}


void StockFontBufferFamily( STR_BUFFER *strBuffer )
{
	int lineDist = 0; //行距
	int splitPoint = 0;
	int bakSplitPoint,cursor;
	BOOL SetCursor=FALSE;
	char splitStr[ 256 ];
	if( FontCnt >= FONT_BUFFER_SIZE ){ 
		strBuffer->hitFontNo = -2;
		return;
	}
	int 行字数;
	if( strBuffer->lineLen != 0 ){
		cursor=strBuffer->cursor;
		if(lineDist) 行字数 = strBuffer->lineLen;
		else 行字数 = 38;
		while( strlen( strBuffer->buffer + splitPoint ) >=行字数 ){
			bakSplitPoint=splitPoint;
			//copy一行的字串
			strncpy_s( splitStr, strBuffer->buffer + splitPoint, 行字数 );
			*( splitStr +行字数 ) = NULL;
			if( GetStrLastByte( splitStr ) == 3 ){
				//分割到中DBCS时，退回一个byte
				splitPoint = 行字数 - 1 + splitPoint;
				*( splitStr + 行字数 - 1 ) = NULL; 
			}else
				splitPoint =行字数 + splitPoint;
			if(lineDist)
				StockFontBuffer( strBuffer->x-60, strBuffer->y + lineDist, strBuffer->fontPrio, 0, splitStr, 0 );
			else
				StockFontBuffer( strBuffer->x, strBuffer->y + lineDist, strBuffer->fontPrio, 0, splitStr, 0 );
			if(cursor>=bakSplitPoint && cursor<splitPoint){
				if(lineDist)
					strBuffer->imeX = strBuffer->x-60 + (cursor-bakSplitPoint)*(FONT_SIZE>>1);
				else
					strBuffer->imeX = strBuffer->x +(cursor-bakSplitPoint)*(FONT_SIZE>>1);
				strBuffer->imeY = strBuffer->y + lineDist;
				SetCursor=TRUE;
			}
			lineDist += strBuffer->lineDist;
			if(lineDist) 行字数 = strBuffer->lineLen;
			else 行字数 = 38;
		}
		if(!SetCursor){
			if(lineDist)
				strBuffer->imeX = strBuffer->x -60+ (strBuffer->cursor-splitPoint)*(FONT_SIZE>>1);
			else
				strBuffer->imeX = strBuffer->x + (strBuffer->cursor-splitPoint)*(FONT_SIZE>>1);
			strBuffer->imeY = strBuffer->y + lineDist;
		}
		if(lineDist)
			StockFontBuffer( strBuffer->x-60, strBuffer->y + lineDist, strBuffer->fontPrio, 0, strBuffer->buffer + splitPoint, 0 );
		else
			StockFontBuffer( strBuffer->x, strBuffer->y + lineDist, strBuffer->fontPrio, 0, strBuffer->buffer + splitPoint, 0 );
	}else{
		FontBuffer[ FontCnt ].x = strBuffer->x;
		FontBuffer[ FontCnt ].y = strBuffer->y;
		FontBuffer[ FontCnt ].fontPrio = strBuffer->fontPrio;
		FontBuffer[ FontCnt ].color = strBuffer->color;
		FontBuffer[ FontCnt ].hitFlag = 0;
#ifdef _FONT_SIZE
		FontBuffer[ FontCnt ].size = 0;
#endif
		//定义Hook_type用来处理星号产生
#ifdef _SAHOOK //Syu ADD Hook程式
		if( strBuffer->filterFlag == HOOK_TYPE){
			extern int HOOK_PASSWD_NUM;
			for(int i = 0 ; i < HOOK_PASSWD_NUM ; i++)
				FontBuffer[ FontCnt ].str[ i ] = '*';
			FontBuffer[ FontCnt].str[i]=NULL;
			strBuffer->cursor = HOOK_PASSWD_NUM;
		}
		else if( strBuffer->filterFlag == BLIND_TYPE ){
#else
		if( strBuffer->filterFlag == BLIND_TYPE ){
#endif
			int i;
			for(i = 0 ; i < strBuffer->cnt ; i++ )
				FontBuffer[ FontCnt ].str[ i ] = '*';
			FontBuffer[ FontCnt ].str[ i ] = NULL;
		}else {
		extern int 编码;
		if(编码==950){
			extern char* GB2312ToBIG5(const char* szBIG5String);
			strcpy( FontBuffer[ FontCnt ].str, GB2312ToBIG5((const char *)strBuffer->buffer) );
		}else
			strcpy( FontBuffer[ FontCnt ].str, strBuffer->buffer );		
		}
		strBuffer->imeX = strBuffer->x - 60+strBuffer->cursor*(FONT_SIZE>>1);
		strBuffer->imeY = strBuffer->y;
		strBuffer->hitFontNo = FontCnt++;
	}
}