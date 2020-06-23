#ifndef __CHATMAGIC_H__
#define __CHATMAGIC_H__
#include "version.h"

void CHAR_CHAT_DEBUG_hp( int charindex , char *message );
void CHAR_CHAT_DEBUG_mp( int charindex , char *message );
void CHAR_CHAT_DEBUG_setmp( int charindex , char *message );
void CHAR_CHAT_DEBUG_str( int charindex , char *message );
void CHAR_CHAT_DEBUG_dex( int charindex , char *message );
void CHAR_CHAT_DEBUG_tgh( int charindex , char *message );
void CHAR_CHAT_DEBUG_vital( int charindex , char *message );
void CHAR_CHAT_DEBUG_luck( int charindex , char *message );
void CHAR_CHAT_DEBUG_gold( int charindex , char *message );
void CHAR_CHAT_DEBUG_additem( int charindex , char *message );
void CHAR_CHAT_DEBUG_metamo( int charindex , char *message );
void CHAR_CHAT_DEBUG_warp( int charindex , char *message );
void CHAR_CHAT_DEBUG_info( int charindex , char *message );
void CHAR_CHAT_DEBUG_sysinfo( int charindex , char *message );
void CHAR_CHAT_DEBUG_announce(int charindex , char *message );
void CHAR_CHAT_DEBUG_level( int charaindex, char* message );
void CHAR_CHAT_DEBUG_delitem( int charindex , char *message );
void CHAR_CHAT_DEBUG_superman( int charaindex, char* message );
void CHAR_CHAT_DEBUG_battlein( int charaindex, char* message );
void CHAR_CHAT_DEBUG_battleout( int charaindex, char* message );
void CHAR_CHAT_DEBUG_petmake( int charaindex, char* message );
void CHAR_CHAT_DEBUG_enemyrestart( int charaindex, char* message );
void CHAR_CHAT_DEBUG_s_gb( int charaindex, char* message );
void CHAR_CHAT_DEBUG_s_gu( int charaindex, char* message );
void CHAR_CHAT_DEBUG_s_tame( int charaindex, char* message );
void CHAR_CHAT_DEBUG_fieldatt( int charaindex, char* message );
void CHAR_CHAT_DEBUG_s_ren( int charaindex, char* message );
void CHAR_CHAT_DEBUG_s_geki( int charaindex, char* message );
void CHAR_CHAT_DEBUG_s_hai( int charaindex, char* message );
void CHAR_CHAT_DEBUG_j_state( int charaindex, char* message );
void CHAR_CHAT_DEBUG_battlewatch( int charaindex, char* message );
void CHAR_CHAT_DEBUG_eventclean( int charaindex, char* message );
void CHAR_CHAT_DEBUG_eventsetend( int charaindex, char* message );
void CHAR_CHAT_DEBUG_eventsetnow( int charaindex, char* message );
void CHAR_CHAT_DEBUG_debug( int charaindex, char* message );
void CHAR_CHAT_DEBUG_exp( int charindex , char *message );
void CHAR_CHAT_DEBUG_dp( int charindex , char *message );
void CHAR_CHAT_DEBUG_setTrans( int charaindex, char *message);
void CHAR_CHAT_DEBUG_getuser(int charindex ,char *message);//ttom +1 14/11/2000
void CHAR_CHAT_DEBUG_shutup(int charindex ,char *message);//ttom 22/11/2000
void CHAR_CHAT_DEBUG_waeikick( int charindex, char* message );//ttom 12/02/2000
void CHAR_CHAT_DEBUG_effect( int charaindex, char* message );
void CHAR_CHAT_DEBUG_jail( int charindex, char* message );//ttom 01/11/2001
void CHAR_CHAT_DEBUG_shutupall(int charindex ,char *message);//ttom 02/01/2001
void CHAR_CHAT_DEBUG_send(int charindex ,char *message);//ttom 02/01/2001
void CHAR_CHAT_DEBUG_noenemy(int charaindex, char *message);
void CHAR_CHAT_DEBUG_loginannounce(int charaindex, char* message); // Arminius 7.12 login announce
void CHAR_CHAT_DEBUG_deletepet(int charaindex, char* message); // Robin 0720
void CHAR_CHAT_DEBUG_deleteitem(int charaindex, char* message); // Robin 0720
void CHAR_CHAT_DEBUG_checklock(int charaindex, char* message);	// Arminius 7.25
void CHAR_CHAT_DEBUG_unlock(int charaindex, char* message);
void CHAR_CHAT_DEBUG_unlockserver(int charaindex, char* message);
void CHAR_CHAT_DEBUG_fixfmdata(int charaindex, char* message); // CoolFish: GM Family 2001/7/31
void CHAR_CHAT_DEBUG_shutdown(int charaindex, char* message);	// Robin 1008
void CHAR_CHAT_DEBUG_manorpk(int charaindex, char *message);

void CHAR_CHAT_DEBUG_watchevent( int charaindex, char* message );
void CHAR_CHAT_DEBUG_fixfmpk(int charaindex, char *message);
void CHAR_CHAT_DEBUG_reloadmsip( int charaindex, char *message);
void CHAR_CHAT_DEBUG_cleanfreepet(int charaindex, char *message);
void CHAR_CHAT_DEBUG_engineer( int charindex, char *message);
void CHAR_CHAT_DEBUG_waeikickall( int charindex, char* message );
void CHAR_CHAT_DEBUG_showMem(int charaindex, char *message);
void CHAR_CHAT_DEBUG_playerspread( int charaindex, char *message);
void CHAR_CHAT_DEBUG_reset( int charaindex, char* message );

#ifdef _MAKE_MAP
void CHAR_CHAT_DelMap( int charaindex, char* message );
void CHAR_CHAT_GetMap( int charaindex, char* message );
void CHAR_CHAT_Map( int charaindex, char* message );
void CHAR_CHAT_Fixtile( int charaindex, char* message );
void CHAR_CHAT_Fixobj( int charaindex, char* message );
void CHAR_CHAT_Fukuwa( int charaindex, char* message );
#endif

#ifdef _GMRELOAD
void CHAR_CHAT_DEBUG_gmreload(int charaindex, char *message);
#endif
#ifdef _CHAR_PROFESSION			// WON ADD 人物职业
void CHAR_CHAT_DEBUG_addsk( int charaindex, char *message );
void CHAR_CHAT_DEBUG_delsk( int charaindex, char *message );
#endif
void CHAR_CHAT_DEBUG_cleanfloor( int charaindex, char *message);

#ifdef _GAMBLE_BANK
void CHAR_CHAT_DEBUG_setgamblenum( int charaindex, char *message );
#endif
#ifdef _WAEI_KICK
void CHAR_CHAT_DEBUG_gmkick( int charindex, char* message);
#endif
// WON ADD 修正族长问题
void CHAR_CHAT_DEBUG_fixfmleader(int charaindex, char *message);

// WON ADD 当机指令
void CHAR_CHAT_DEBUG_crash(int charaindex, char *message);

#ifdef _PETSKILL_SETDUCK
void CHAR_CHAT_DEBUG_SetDuck( int charaindex, char *message);
#endif
#ifdef _TYPE_TOXICATION
void CHAR_CHAT_DEBUG_Toxication( int charaindex, char *message);
#endif

#ifdef _SEND_EFFECT	   	        // WON ADD AC送下雪、下雨等特效
void CHAR_CHAT_DEBUG_sendeffect(int charaindex, char *message);
#endif
#ifdef _TEST_DROPITEMS
void CHAR_CHAT_DEBUG_dropmypet( int charindex, char *message );
void CHAR_CHAT_DEBUG_dropmyitem( int charindex , char *message );
#endif

#ifdef _CHAR_PROFESSION			// WON ADD 人物职业
void CHAR_CHAT_DEBUG_show_profession( int charaindex, char*message );
void CHAR_CHAT_DEBUG_set_regist( int charaindex, char*message );
#endif

void CHAR_CHAT_DEBUG_checktrade( int charaindex, char*message);

void CHAR_CHAT_DEBUG_checktime( int charaindex, char *message);
void CHAR_CHAT_DEBUG_samecode( int charaindex, char *message);
void CHAR_CHAT_DEBUG_silent(int charindex ,char *message);
void CHAR_CHAT_DEBUG_help( int charindex, char *message);
#ifdef _EQUIT_ARRANGE
void CHAR_CHAT_DEBUG_arrange( int charindex , char *message );
#endif

#ifdef _EQUIT_SEQUENCE
void CHAR_CHAT_DEBUG_sequence( int charindex , char *message );
#endif

#ifdef _EQUIT_NEGLECTGUARD
void CHAR_CHAT_DEBUG_setneguard( int charaindex, char* message );
#endif

void CHAR_CHAT_DEBUG_petlevelup( int charaindex, char* message );
void CHAR_CHAT_DEBUG_petexpup( int charaindex, char* message );

void CHAR_CHAT_DEBUG_reloadpkteamlist( int charaindex, char *message);
void CHAR_CHAT_DEBUG_setBattle( int charaindex, char *message );


#ifdef _CHAR_POOLITEM
void CHAR_CHAT_DEBUG_saveditem(int charaindex, char *message);
void CHAR_CHAT_DEBUG_insertditem(int charaindex, char *message);
void CHAR_CHAT_DEBUG_ShowMyDepotItems( int charaindex, char *message );
void CHAR_CHAT_DEBUG_InSideMyDepotItems( int charaindex, char *message );
#endif

#ifdef _CHAR_POOLPET
void CHAR_CHAT_DEBUG_savedpet(int charaindex, char *message);
void CHAR_CHAT_DEBUG_insertdpet(int charaindex, char *message);
void CHAR_CHAT_DEBUG_ShowMyDepotPets( int charaindex, char *message );
void CHAR_CHAT_DEBUG_InSideMyDepotPets( int charaindex, char *message );
#endif

#ifdef _NEW_MANOR_LAW
void CHAR_CHAT_DEBUG_set_momentum( int charaindex, char* message );
#endif
void CHAR_CHAT_DEBUG_set_manor_owner( int charindex, char* message );
void CHAR_CHAT_DEBUG_set_schedule_time( int charindex, char* message );


#ifdef _ANGEL_SUMMON
void CHAR_CHAT_DEBUG_angelinfo(int charindex ,char *message);
void CHAR_CHAT_DEBUG_angelclean(int charindex ,char *message);
void CHAR_CHAT_DEBUG_angelcreate(int charindex ,char *message);
void CHAR_CHAT_DEBUG_missionreload(int charindex ,char *message);
#endif

void CHAR_CHAT_DEBUG_itemreload(int charindex ,char *message);

void CHAR_CHAT_DEBUG_skywalker(int charaindex ,char *message);

#ifdef _ITEM_ADDEXP
void CHAR_CHAT_DEBUG_itemaddexp(int charaindex ,char *message);
#endif

#ifdef _DEF_GETYOU 
void CHAR_CHAT_DEBUG_getyou(int charaindex,char *message);
#endif

#ifdef _DEF_NEWSEND
void CHAR_CHAT_DEBUG_newsend(int charindex ,char *message);
#endif

#ifdef _DEF_SUPERSEND
void CHAR_CHAT_DEBUG_supersend(int charindex ,char *message);
#endif

#ifdef _FONT_SIZE
void CHAR_CHAT_DEBUG_fsize(int charindex ,char *message);
#endif

#ifdef _JOBDAILY
void CHAR_CHAT_DEBUG_rejobdaily(int charindex ,char *message);
#endif

#ifdef _CREATE_MM_1_2
void CHAR_CHAT_DEBUG_MM(int charaindex , char *message );
#endif

#ifdef _SendTo
void CHAR_CHAT_DEBUG_Sendto( int charaindex , char *message );
#endif

void CHAR_CHAT_printcount( int charaindex, char* message );

#ifdef _GM_ITEM
void CHAR_CHAT_DEBUG_GMFUNCTION( int charindex , char *message );
#endif

#ifdef _GM_RIDE
void CHAR_CHAT_DEBUG_SETRIDE( int charindex , char *message );
void CHAR_CHAT_DEBUG_MVRIDE( int charindex , char *message );
#endif

#ifdef _LOCK_IP
void CHAR_CHAT_DEBUG_ADD_LOCK( int charindex , char *message );
void CHAR_CHAT_DEBUG_DEL_LOCK( int charindex , char *message );
void CHAR_CHAT_DEBUG_SHOWIP( int charindex , char *message );
#endif
void CHAR_CHAT_DEBUG_SET_FAME( int charaindex, char* message );
#ifdef _AUTO_PK
void CHAR_CHAT_DEBUG_SET_AUTOPK( int charaindex, char* message );
#endif

#ifdef _PLAYER_NUM
void CHAR_CHAT_DEBUG_SET_PLAYERNUM( int charaindex, char* message );
#endif
#ifdef _RELOAD_CF
void CHAR_CHAT_DEBUG_SET_RELOADCF( int charaindex, char* message );
#endif
#ifdef _TRANS
void CHAR_CHAT_DEBUG_Trans( int charaindex, char *message);
#endif
#ifdef _FUSIONBEIT_FIX
void CHAR_CHAT_DEBUG_fusionbeit( int charaindex, char* message );
#endif
#ifdef _MAKE_PET_CF
void CHAR_CHAT_DEBUG_petmakecf( int charaindex, char* message );
#endif
#ifdef _MAKE_PET_ABILITY
void CHAR_CHAT_DEBUG_petmakeabi( int charaindex, char* message );
#endif
#ifdef _PLAYER_QUESTION_ONLIEN
void CHAR_CHAT_DEBUG_PlayerQuestion( int charaindex, char* message );
#endif
#ifdef _GM_SAVE_ALL_CHAR
void CHAR_CHAT_DEBUG_GmSaveAllChar( int charaindex, char* message );
#endif
#ifdef _ALLBLUES_LUA
void CHAR_CHAT_DEBUG_ReLoadLua( int charaindex, char* message );
void CHAR_CHAT_DEBUG_NewLoadLua( int charaindex, char* message );
#endif
#ifdef _KEEP_UP_NO_LOGIN
void CHAR_CHAT_DEBUG_KeepUpNoLogin( int charaindex, char* message );
#endif
#ifdef _NEW_LOAD_NPC
void CHAR_CHAT_DEBUG_NewLoadNpc( int charaindex, char* message );
#endif
#ifdef _NEW_LOAD_MAP
void CHAR_CHAT_DEBUG_NewLoadMap( int charaindex, char* message );
#endif
#ifdef _JZ_NEWSCRIPT_LUA
void CHAR_CHAT_DEBUG_LUA_INIT( int charaindex, char* message );
void CHAR_CHAT_DEBUG_LUA_CLOSE( int charaindex, char* message );
#endif
#ifdef _NO_FULLPLAYER_ATT
void CHAR_CHAT_NoFullPlayer( int charaindex, char* message );
#endif
#ifdef _PET_MAKE_2_TRANS
void CHAR_CHAT_DEBUG_petmake2( int charaindex, char* message );
#endif
#ifdef _NPC_MAGICCARD
void CHAR_CHAT_DEBUG_gamerate(int charindex, char *message);
#endif
void CHAR_CHAT_DEBUG_SAVESHH( int fd, int charaindex,char* message,int color, int area );
#endif


