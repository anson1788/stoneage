/*-----------------------------------------------------------------------------
    caryMemory_lib.h -- cary's Virtual memory 管理。
    Waei(c) All rights reserved
    author: 許百勝Cary Hsu, 2000.10.8
-----------------------------------------------------------------------------*/
#ifndef __MEMDLL
#define __MEMDLL

#define _MEM_LIB

#ifdef _MEM_LIB
	#define MEMDLL_API
#else
	#ifdef MEMDLL_EXPORTS
		#define MEMDLL_API __declspec(dllexport)
	#else
		#define MEMDLL_API __declspec(dllimport)
	#endif
#endif

//回傳的錯誤***************************************************************************************
#define ERROR_MEMORY_OK				0x00000000
#define ERROR_MEMORY_INITFAILED		0x00000001	//配置Handle陣列錯誤

#define ERROR_HEAP_NOT_ENOUGH		0x00000010
#define ERROR_HEAP_LIST_NOT_ENOUGH	0x00000020

//*************************************************************************************************
/*	初始化記憶體函式庫
return:	ERROR_MEMORY_OK:		成功
		ERROR_MEMORY_INITFAILED:初始化失敗	*/
DWORD MemoryInit();

/*	釋放記憶體函式庫的記憶體	*/
void MemoryRelease();

//*************************************************************************************************
/*	保留一大範圍的Virtual Memory，並傳回Handle，建議以page為單位(先配置1個commit page的記憶體)
parameter:	lpdwHandle:	傳回Handle
			iSize:		要配置多少記憶體，並回傳建立的大小
return:		非NULL:		回傳配置記憶體的位址
			NULL:		失敗					*/
MEMDLL_API LPVOID VirtualReserve( LPDWORD lpdwHandle, int &iSize);

/*	釋放此Handle配置的記憶體
parameter:	lpdwHandle:	Handle值	*/
MEMDLL_API void VirtualRelease( LPDWORD lpdwHandle);

/*	發生Exception的處理函式
parameter:	dwHandle:						Handle值
			lpEP:							EXCEPTION_POINTERS的structure
return:		EXCEPTION_CONTINUE_EXECUTION:	繼續執行
			EXCEPTION_CONTINUE_SEARCH:		繼續尋找可處理exception handle	*/
MEMDLL_API int ExpFilter( DWORD dwHandle, LPEXCEPTION_POINTERS lpEP);

//*************************************************************************************************
//*************************************************************************************************
/*	配置iSize個記憶體當Heap
parameter:	lpdwHandle:		傳回Handle
			iSize:			要配置多少的記憶體
return:		TRUE:			配置成功
			FALSE:			失敗				*/
MEMDLL_API BOOL HeapCreate( LPDWORD lpdwHandle, int &iSize);

/*	釋放Heap的所有記憶體
parameter:	lpdwHandle:	Handle值	*/
MEMDLL_API void HeapRelease( LPDWORD lpdwHandle);

/*	清除Heap的List，再使用
parameter:	dwHandle:	Handle值	*/
MEMDLL_API void HeapReset( DWORD dwHandle);

/*	從lpheap中，配置dwSize大小的記憶體
parameter:	dwHandle:	Handle值
			dwSize:		要配置多少記憶體
return:		非NULL:		傳回配置的記憶體
			NULL:		失敗				*/
MEMDLL_API LPVOID HeapAlloc( DWORD dwHandle, DWORD dwSize);

/*	從lpheap中釋放掉Heap的memlt
parameter:	dwHandle:	Handle值
			lpAddress:	要釋放的記憶體位址的位址				*/
MEMDLL_API void HeapFree( DWORD dwHandle, LPVOID *lpAddress);

//*************************************************************************************************
//*************************************************************************************************
typedef struct USEDMEMORY{
	LPBYTE			lpbyStart;
	DWORD			dwSize;
}*LPUSEDMEMORY;

/*	取回lpheap的第一個Heap list
parameter:	dwHandle:	Handle值
return:		傳回使用中的address，NULL為最後一個	*/
MEMDLL_API LPUSEDMEMORY GetMemoryListHead( DWORD dwHandle);

/*	取回lpheap的下一個Heap list
parameter:	dwHandle:	Handle值
return:		傳回使用中的address，NULL為最後一個	*/
MEMDLL_API LPUSEDMEMORY GetMemoryListNext( DWORD dwHandle);

/*	取回錯誤值
parameter:	dwHandle:	Handle值
return:		回傳錯誤值			*/
MEMDLL_API DWORD GetCaryError( DWORD dwHandle);

//*************************************************************************************************
//*************************************************************************************************
/*	搬移記憶體，使用MMX做針對pentum II最佳化的處理
parameter:	dest:	目的位址
			src:	來源位址
			cnt:	搬移的byte數
return:		TRUE:	成功執行
			FALSE:	位址不合法						*/
MEMDLL_API BOOL mmx_CopyMemory( LPVOID dest, LPVOID src, int cnt);

/*	搬移記憶體，使用MMX做針對pentum II最佳化的處理
parameter:	dest:	目的位址
			src:	來源位址
			cnt:	搬移的byte數
return:		TRUE:	成功執行
			FALSE:	位址不合法						*/
MEMDLL_API BOOL mmx_CopyMemory_test( LPVOID dest, LPVOID src, int cnt);

#endif
