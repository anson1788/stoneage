#pragma once

#include "../systeminc/version.h"

#ifdef BIN_LOAD
class BinLoad
{
public:
	BinLoad(void);
	~BinLoad(void);

public:
	static void LoadBinData();
	
private:
	static void EncryptSimple( const char* szSrc, char* szDes );
	static void LoadOneBin( char* pszFilePath );

	static void SetBinData( char* pszFileName, char* pszData, int iLine );
#ifdef QUEST_EXTERN
	static void SetQuestData( char* pszData, int iLine );
	static void SetActData( char* pszData, int iLine );
#endif
#ifdef PET_EXTERN
	static void SetPetLifeType( char* pszData );
#endif
	static int GetStringData( char* pszData, int iBegin, char cEnd, char* pszToken );
};

#endif
