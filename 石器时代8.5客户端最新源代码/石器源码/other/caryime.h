/******************************************************************************
	caryIme.h:	cary's Ime的DllMain。
												(c) 許百勝Cary Hsu,	2000.7.10
******************************************************************************/
#ifndef __CARYIME
#define __CARYIME

typedef void(*RESULTCALLBACK)(LPSTR);

/*	IME的初始化
parameter:	hwnd:			所使用的HWND
			ResultCallback:	中文輸入法的結果回報
return:		TRUE:			嚴重的錯誤，請結束程式，否則無法使用中文輸入法*/
BOOL InitIme(HWND hwnd,RESULTCALLBACK ResultCallback);

/*	IME結束*/
void EndIme();

/*	改變顯示的模式，window mode或fullscreen mode
parameter:	bWindowMode:	使用的視窗模式		*/
void ChangeMode(BOOL bWindowMode);

/*	處理IME的message
parameter:	Message:	視窗訊息
			wParam,lParam:視窗訊息的參數
return:	TRUE:	已經有處理IME的message
		FALSE:	還有其也的message需要處理	*/
BOOL ImeMessage(UINT Message,WPARAM wParam,LPARAM lParam);

/*	clear IME buffer	*/
void ClearIme();

/*	IME的ON,OFF狀態確認
return:	ON:	TRUE
		OFF:FALSE		*/
BOOL ImeOpenState();

/*	取得輸入法的字串，可能candiate或compsition	
return:	NULL:	沒有字串，請自行處理
		非NULL:	有輸入法的字串				*/
LPSTR GetImeString();

/*	取得輸入法的描述字串
return:	輸入法的描述字串	*/
LPSTR GetImeDescString();

///////////////////////////////////////////////////////////////////////////////

/*	取得lpstr的上一個字的位址
parameter:	lpstrHead:	字串的頭
			lpstr:		要取回上一個字的字串
return:		上一個字的位址					*/
char *GetCharPrev(char *lpstrHead,char *lpstr);

#endif