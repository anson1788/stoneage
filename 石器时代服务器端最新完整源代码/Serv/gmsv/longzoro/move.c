#include <string.h>
#include "version.h"
#include "longzoro/move.h"
#include "char_base.h"
#include "char.h"
#include "autil.h"

#ifdef _TALK_MOVE_FLOOR

Talk_Move_Floor_t talkmovefloor[MAXTALKMOVEFLOOR];

BOOL talk_move_map(int charaindex, char *messageeraseescape)
{
	if(messageeraseescape[0] == '/' && messageeraseescape[1] == 'm' && messageeraseescape[2] == 'o' && messageeraseescape[3] == 'v' && messageeraseescape[4] == 'e'){
			char token[32];
			char buf[256]="";
			int i;
			easyGetTokenFromString( messageeraseescape , 2 , token, sizeof(token));

			if(strcmp( token, "?") == 0){
				strcpy( buf, "您可顺移地方:");
				for(i=0;i<MAXTALKMOVEFLOOR;i++){
					if( strlen( buf ) >= 220 ){
						CHAR_talkToCli( charaindex, -1, buf, CHAR_COLORYELLOW);	
						strcpy( buf, "您可顺移地方:");
					}
					if( CHAR_getInt(charaindex, CHAR_TRANSMIGRATION) > talkmovefloor[i].trans 
							|| ( CHAR_getInt(charaindex, CHAR_TRANSMIGRATION) == talkmovefloor[i].trans
								 && CHAR_getInt(charaindex, CHAR_LV) >= talkmovefloor[i].level ) ) {
						strcat( buf, talkmovefloor[i].name);
						strcat( buf, " ");
					}
				}
				CHAR_talkToCli( charaindex, -1, buf, CHAR_COLORYELLOW);	
				return TRUE;
			}

			BOOL isMove = FALSE;
			for(i=0;i<MAXTALKMOVEFLOOR;i++){
				if(strcmp(talkmovefloor[i].name, token) == 0){
					isMove = TRUE;
					break;
				}
			}
			if( isMove == TRUE ){
				if( CHAR_getInt(charaindex, CHAR_TRANSMIGRATION) > talkmovefloor[i].trans 
					|| ( CHAR_getInt(charaindex, CHAR_TRANSMIGRATION) == talkmovefloor[i].trans
								 && CHAR_getInt(charaindex, CHAR_LV) >= talkmovefloor[i].level ) ) {
							CHAR_warpToSpecificPoint(charaindex, talkmovefloor[i].id, talkmovefloor[i].x, talkmovefloor[i].y );
							return TRUE;
				}
			}
	}
	return FALSE;
}

BOOL MoveMap_init()
{
	FILE* fp;
	int i;
#ifdef _CRYPTO_DATA		
	BOOL crypto = FALSE;
	fp = fopen( "data/talkmovefloor.txt.allblues", "r");
	if( fp != NULL ){
		crypto = TRUE;
	}else
#endif
{
	fp = fopen("data/talkmovefloor.txt", "r");
}
	if (fp == NULL)
	{
		print("无法打开文件\n");
		return FALSE;
	}
	for(i=0;i<MAXTALKMOVEFLOOR;i++){
		char line[64], buf[16];
		if (fgets(line, sizeof(line), fp) == NULL){
#ifdef _CRYPTO_DATA		
			if(crypto==TRUE){
				DecryptKey(line);
			}
#endif
			memset( &talkmovefloor[i], 0, sizeof( talkmovefloor[i] ) );
			continue;
		}
		chop(line);

		getStringFromIndexWithDelim(line,",", 1, talkmovefloor[i].name, sizeof(talkmovefloor[i].name));
		
		getStringFromIndexWithDelim(line,",", 2, buf, sizeof(buf));
		talkmovefloor[i].id=atoi(buf);
		
		getStringFromIndexWithDelim(line,",", 3, buf, sizeof(buf));
		talkmovefloor[i].x=atoi(buf);
		
		getStringFromIndexWithDelim(line,",", 4, buf, sizeof(buf));
		talkmovefloor[i].y=atoi(buf);
		
		getStringFromIndexWithDelim(line,",", 5, buf, sizeof(buf));
		talkmovefloor[i].trans=atoi(buf);
		
		getStringFromIndexWithDelim(line,",", 6, buf, sizeof(buf));
		talkmovefloor[i].level=atoi(buf);
		
	}
	fclose(fp);
	return TRUE;
}
#endif
