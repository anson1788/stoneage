#include <stdio.h>
#include <string.h>

#include "version.h"

#include "common.h"
#include "npctemplate.h"
#include "npccreate.h"
#ifdef _MO_RELOAD_NPC
#include "char.h"
#include "battle.h"
#include "configfile.h"
#endif
BOOL NPC_readNPCSettingFiles( char* topdirectory ,int templatesize, int createsize)
{
    print("\n");
    if( NPC_readNPCTemplateFiles( topdirectory ,templatesize) == FALSE )
        return FALSE;

    if( NPC_readNPCCreateFiles( topdirectory ,createsize) == FALSE )
        return FALSE;

    return TRUE;
}
#ifdef _MO_RELOAD_NPC
BOOL NPC_reloadNPC(void)
{
	int i;
	int charnum = getFdnum()+ getPetcharnum()+getOtherscharnum();
	for(i=0;i<charnum;i++){
		if(!CHAR_CHECKINDEX(i)) continue;
		if(CHAR_getWorkInt(i,CHAR_WORKNPCTYPE)==1){
			NPC_Util_NPCDelete(i);
		}
	}
	if(NPC_readNPCSettingFiles( getNpcdir(), getNpctemplatenum(),
                                  getNpccreatenum() )==TRUE)
  	NPC_generateLoop( 1 );
  if(getReloadNpcType()==1){
  	int j;
  	int playernum = CHAR_getPlayerMaxNum();
  	for(j=0;j<playernum;j++){
  		if( CHAR_getCharUse(j) != FALSE ) {
  			if(CHAR_getWorkInt(j,CHAR_WORK_OFFLINE)==0 && CHAR_getWorkInt(j,CHAR_WORKBATTLEMODE)==BATTLE_CHARMODE_NONE){
  				CHAR_warpToSpecificPoint(j,CHAR_getInt(j,CHAR_FLOOR),CHAR_getInt(j,CHAR_X),CHAR_getInt(j,CHAR_Y));
  			}
  		}
  	}
  }
  return TRUE;
}
#endif
