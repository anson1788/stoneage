#ifndef __READNPC_H__
#define __READNPC_H__
BOOL NPC_readNPCSettingFiles( char* topdirectory ,int templatesize,int createsize);
#ifdef _MO_RELOAD_NPC
BOOL NPC_reloadNPC(void);
#endif
#endif 
 /*__READNPC_H__*/
