#include <string.h>
#include "version.h"
#include "common.h"
#include "longzoro/characters.h"
#include "readmap.h"
#include "saacproto_cli.h"
#include "char_base.h"
#ifdef _NEW_ITEM_
extern int CheckCharMaxItem(int charindex);
#endif
extern int CheckCharMaxItemChar(Char* ch);
#ifdef _CHARADATA_SAVE_SQL
#include "item.h"
char CHAR_list_info1_int[][64]={
	"CHAR_DATAPLACENUMBER",
  "CHAR_BASEIMAGENUMBER", 
  "CHAR_BASEBASEIMAGENUMBER",
	"CHAR_FACEIMAGENUMBER",
  "CHAR_FLOOR",
  "CHAR_X",  
  "CHAR_Y", 
  "CHAR_DIR",
	"CHAR_LV",
  "CHAR_GOLD",

  "CHAR_HP",
  "CHAR_MP",
  "CHAR_MAXMP", 

	"CHAR_VITAL",
  "CHAR_STR",
  "CHAR_TOUGH",
	"CHAR_DEX",

	"CHAR_CHARM",
	"CHAR_LUCK",
	"CHAR_EARTHAT",	
	"CHAR_WATERAT",
	"CHAR_FIREAT",
	"CHAR_WINDAT"
};

char CHAR_list_info2_int[][64]={
	"CHAR_SLOT",
	"CHAR_CRITIAL",
	"CHAR_COUNTER",
	"CHAR_RARE",
  "CHAR_RADARSTRLENGTH",
  "CHAR_CHATVOLUME",
  "CHAR_MERCHANTLEVEL", 
  "CHAR_HEALERLEVEL", 
  "CHAR_DETERMINEITEM",
  "CHAR_INDEXOFEQTITLE",             
  "CHAR_POISON",              
  "CHAR_PARALYSIS",              
  "CHAR_SILENCE",
  "CHAR_STONE",              
  "CHAR_DARKNESS", 
  "CHAR_CONFUSION"
};

char CHAR_list_count_int[][64]={
  "CHAR_LOGINCOUNT",
  "CHAR_DEADCOUNT",
  "CHAR_WALKCOUNT",
  "CHAR_TALKCOUNT",
  "CHAR_DAMAGECOUNT",
  "CHAR_GETPETCOUNT",
  "CHAR_KILLPETCOUNT",
  "CHAR_DEADPETCOUNT",
	"CHAR_SENDMAILCOUNT",
	"CHAR_MERGEITEMCOUNT",
	"CHAR_DUELBATTLECOUNT",
	"CHAR_DUELWINCOUNT",
	"CHAR_DUELLOSECOUNT",
  "CHAR_DUELSTWINCOUNT",
  "CHAR_DUELMAXSTWINCOUNT"
};

char CHAR_list_info3_int[][64]={
  "CHAR_WHICHTYPE",              
  "CHAR_WALKNTERVAL",             
  "CHAR_LOOPINTERVAL",             
#ifdef _NEWOPEN_MAXEXP
	"CHAR_OLDEXP",              
	"CHAR_EXP",				
#else
  "CHAR_EXP",              
#endif
  "CHAR_LASTTALKELDER",              
  "CHAR_SKILLUPPOINT",
	"CHAR_LEVELUPPOINT",			
  "CHAR_IMAGETYPE", 
	"CHAR_NAMECOLOR",
	"CHAR_POPUPNAMECOLOR",
	"CHAR_LASTTIMESETLUCK",			
	"CHAR_DUELPOINT"
};

char CHAR_list_event_int[][64]={
	"CHAR_EVENT1",			
	"CHAR_EVENT2",			
	"CHAR_EVENT3",			
#ifdef _NEWEVENT
	"CHAR_EVENT4",			
	"CHAR_EVENT5",			
	"CHAR_EVENT6",			
#endif
#ifdef _ADD_NEWEVENT 
	"CHAR_EVENT7",			
	"CHAR_ENDEVENT8",
#endif

	"CHAR_NOWEVENT1",			
	"CHAR_NOWEVENT2",			
	"CHAR_NOWEVENT3",			
#ifdef _NEWEVENT
	"CHAR_NOWEVENT4",			
	"CHAR_NOWEVENT5",			
	"CHAR_NOWEVENT6",			
#endif
#ifdef _ADD_NEWEVENT
	"CHAR_NOWEVENT7",			
	"CHAR_NOWEVENT8"
#endif
};

char CHAR_list_info4_int[][64]={
	"CHAR_TRANSMIGRATION",
  "CHAR_TRANSEQUATION",
    
  "CHAR_INITDATA", 
  "CHAR_SILENT",     
  "CHAR_FMINDEX",
  "CHAR_FMLEADERFLAG",
  "CHAR_FMSPRITE",
  "CHAR_BANKGOLD",		
  "CHAR_RIDEPET",		
  "CHAR_LEARNRIDE",	
#ifdef _NEW_RIDEPETS
	"CHAR_LOWRIDEPETS",
#endif
  "CHAR_LIMITLEVEL",
#ifdef _PET_FUSION
	"CHAR_FUSIONCODE",
	"CHAR_FUSIONINDEX",
	"CHAR_FUSIONRAISE",
	"CHAR_FUSIONBEIT",	
	"CHAR_FUSIONTIMELIMIT",
#endif

#ifdef _PERSONAL_FAME
  "CHAR_FAME",
#endif

#ifdef _NEWSAVE
  "CHAR_SAVEINDEXNUMBER"	
#endif
};

char CHAR_list_attackmagic_int[][64]={
#ifdef _ATTACK_MAGIC
	"CHAR_EARTH_EXP",	
	"CHAR_WATER_EXP",	
	"CHAR_FIRE_EXP",	
	"CHAR_WIND_EXP", 	
	"CHAR_EARTH_RESIST", 				
	"CHAR_WATER_RESIST", 				
	"CHAR_FIRE_RESIST",					
	"CHAR_WIND_RESIST",					
	"CHAR_EARTH_ATTMAGIC_EXP",	
	"CHAR_WATER_ATTMAGIC_EXP",	
	"CHAR_FIRE_ATTMAGIC_EXP",		
	"CHAR_WIND_ATTMAGIC_EXP",		
	"CHAR_EARTH_DEFMAGIC_EXP",  
	"CHAR_WATER_DEFMAGIC_EXP",  
	"CHAR_FIRE_DEFMAGIC_EXP",   
	"CHAR_WIND_DEFMAGIC_EXP" 
#endif
};

char CHAR_list_info5_int[][64]={
#ifdef _GAMBLE_BANK 
	"CHAR_PERSONAGOLD",
#endif
#ifdef _DROPSTAKENEW
	"CHAR_GAMBLENUM",	
#endif
#ifdef _ADD_ACTION   
	"CHAR_ACTIONSTYLE",
#endif
#ifdef _PET_EVOLUTION
	"CHAR_EVOLUTIONBASE1",
	"CHAR_EVOLUTIONBASE2",
	"CHAR_EVOLUTIONBASE3",
	"CHAR_EVOLUTIONBASE4",
#endif

#ifdef _FM_JOINLIMIT
	"CHAR_FMTIMELIMIT"
#endif
};

char CHAR_list_profession_int[][64]={
#ifdef _CHAR_PROFESSION
	"PROFESSION_CLASS",
	"PROFESSION_LEVEL",
//	"PROFESSION_EXP",
	"PROFESSION_SKILL_POINT",
	"ATTACHPILE",
	"PROFESSION_FIRE_P",
	"PROFESSION_ICE_P",
	"PROFESSION_THUNDER_P",
	"PROFESSION_FIRE_R",
	"PROFESSION_ICE_R",
	"PROFESSION_THUNDER_R"
#endif
};

char CHAR_list_info6_int[][64]={
#ifdef _ALLDOMAN 
	"CHAR_HEROFLOOR",
#endif
#ifdef _PETSKILL_BECOMEPIG
  "CHAR_BECOMEPIG",
	"CHAR_BECOMEPIG_BBI",
#endif
	"CHAR_LASTLEAVETIME",
#ifdef _NEW_MANOR_LAW
	"CHAR_MOMENTUM",	
#endif

#ifdef _ITEM_ADDEXP2
	"CHAR_ADDEXPPOWER",		
	"CHAR_ADDEXPTIME",		
#endif

#ifdef _ANGEL_SUMMON
	"CHAR_HEROCNT",	
#endif
#ifdef _TEACHER_SYSTEM
	"CHAR_TEACHER_FAME",	
#endif

#ifdef _GM_ITEM
	"CHAR_GMTIME", 
#endif
#ifdef _VIP_SERVER
	"CHAR_AMPOINT", 
#endif
#ifdef _VIP_RIDE
	"CHAR_VIPRIDE", 
#endif

	"CHAR_LISTPAGE",

#ifdef _AUTO_PK
	"CHAR_AUTOPK", 
#endif
#ifdef _BATTLE_PK
	"CHAR_BATTLEPK",
#endif

#ifdef _PET_BEATITUDE
	"CHAR_BEATITUDE",
#endif

#ifdef _RIDE_CF
	"CHAR_LOWRIDEPETS1",
#endif

#ifdef _ITEM_PET_LOCKED
	"CHAR_LOCKED",
#endif

#ifdef _BOUND_TIME
	"CHAR_BOUNDTIME"
#endif
};

char CHAR_list_info_char[][64]={
  "CHAR_NAME" ,          
  "CHAR_OWNTITLE" ,      
  "CHAR_NPCARGUMENT",   
  "CHAR_OWNERCDKEY",
  "CHAR_OWNERCHARANAME",
  "CHAR_FMNAME",
#ifdef _UNIQUE_P_I    
  "CHAR_UNIQUECODE",
#endif

#ifdef _TEACHER_SYSTEM
	"CHAR_TEACHER_ID",
	"CHAR_TEACHER_NAME",
#endif

#ifdef _ITEM_SETLOVER
	"CHAR_LOVE",      
	"CHAR_LOVERID",   
	"CHAR_LOVERNAME",	
#endif

#ifdef _GM_ITEM
	"CHAR_GMFUNCTION",
#endif
#ifdef _NEW_NAME
	"CHAR_NEWNAME"
#endif
};


char CHAR_list[][64]={
  "CHAR_DATAPLACENUMBER" ,          
  "CHAR_FACEIMAGENUMBER" ,      
  "CHAR_LV",   
  "CHAR_WORKMAXHP",
  "CHAR_WORKATTACKPOWER",
  "CHAR_WORKDEFENCEPOWER",
  "CHAR_WORKQUICK",
	"CHAR_WORKFIXCHARM",
	"CHAR_DUELPOINT",
	"CHAR_WORKFIXEARTHAT",      
	"CHAR_WORKFIXWATERAT",   
	"CHAR_WORKFIXFIREAT",	
	"CHAR_WORKFIXWINDAT",
	"CHAR_LOGINCOUNT",   
	"CHAR_NAME",   
	"MAP_NAME"
};


char CHAR_list_item_int[][64]={
	"ITEM_ID",
	"ITEM_BASEIMAGENUMBER",
//	"ITEM_COST",
//	"ITEM_TYPE",
//	"ITEM_ABLEUSEFIELD"
	"ITEM_TARGET",
//	"ITEM_LEVEL",
#ifdef _ITEM_MAXUSERNUM
	"ITEM_DAMAGEBREAK",
#endif
#ifdef _ITEMSET4_TXT
	"ITEM_USEPILENUMS",
	"ITEM_CANBEPILE",
//	"ITEM_NEEDSTR",
//	"ITEM_NEEDDEX",
//	"ITEM_NEEDTRANS",
//	"ITEM_NEEDPROFESSION",
#endif
#ifdef _TAKE_ITEMDAMAGE
	"ITEM_DAMAGECRUSHE",
	"ITEM_MAXDAMAGECRUSHE",
#endif
#ifdef _ADD_DEAMGEDEFC
//	"ITEM_OTHERDAMAGE",
//	"ITEM_OTHERDEFC",
#endif
#ifdef _SUIT_ITEM
//	"ITEM_SUITCODE",
#endif
//	"ITEM_ATTACKNUM_MIN",
//	"ITEM_ATTACKNUM_MAX",
//	"ITEM_MODIFYATTACK",
//	"ITEM_MODIFYDEFENCE",

	"ITEM_MODIFYQUICK",
//	"ITEM_MODIFYHP",
//	"ITEM_MODIFYMP",
//	"ITEM_MODIFYLUCK",
	"ITEM_MODIFYCHARM",
	"ITEM_MODIFYAVOID",
//	"ITEM_MODIFYATTRIB",
//	"ITEM_MODIFYATTRIBVALUE",
	"ITEM_MAGICID",
//	"ITEM_MAGICPROB",
	"ITEM_MAGICUSEMP",
#ifdef _ITEMSET5_TXT
//	"ITEM_MODIFYARRANGE",
//	"ITEM_MODIFYSEQUENCE",
//	"ITEM_ATTACHPILE",
//	"ITEM_HITRIGHT",
#endif

#ifdef _ITEMSET6_TXT
//	"ITEM_NEGLECTGUARD",
#endif
	"ITEM_MERGEFLG",
	
#ifdef _ITEM_COLOER
	"ITEM_COLOER"
#endif
};

char CHAR_list_item_char[][64]={
	"ITEM_NAME",
	"ITEM_SECRETNAME",
	"ITEM_EFFECTSTRING",
	"ITEM_ARGUMENT",
	"ITEM_TYPECODE"
	"ITEM_INLAYCODE",
	"ITEM_CDKEY",
	"ITEM_FORUSERNAME",
	"ITEM_FORUSERCDKEY",
	"ITEM_ANGELMISSION",
	"ITEM_ANGELINFO",
	"ITEM_HEROINFO",
	"ITEM_UNIQUECODE"
};

void CHAR_make_list_String( Char* ch, char *buf )
{
	
	char  escapeshowstring[256]="";
	char *showstr = MAP_getfloorShowstring(ch->data[CHAR_FLOOR]);
  sprintf( buf, "('%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s')",
  					ch->string[CHAR_CDKEY].string, 
  					ch->data[CHAR_SAVEINDEXNUMBER],
            ch->data[CHAR_DATAPLACENUMBER],
            ch->data[CHAR_FACEIMAGENUMBER],
            ch->data[CHAR_LV],
            ch->workint[CHAR_WORKMAXHP],
            ch->workint[CHAR_WORKATTACKPOWER],
            ch->workint[CHAR_WORKDEFENCEPOWER],
            ch->workint[CHAR_WORKQUICK],
            ch->workint[CHAR_WORKFIXCHARM],
            ch->data[CHAR_DUELPOINT],
            ch->workint[CHAR_WORKFIXEARTHAT],
            ch->workint[CHAR_WORKFIXWATERAT],
            ch->workint[CHAR_WORKFIXFIREAT],
            ch->workint[CHAR_WORKFIXWINDAT],
            ch->data[CHAR_AMPOINT],
            ch->string[CHAR_NAME].string,
            makeEscapeString( showstr ,
                              escapeshowstring,
                              sizeof(escapeshowstring) )
  );
}

Charadata charadata;


int list_info1 = arraysizeof(CHAR_list_info1_int);
int list_info2 = arraysizeof(CHAR_list_info2_int);
int list_count = arraysizeof(CHAR_list_count_int);
int list_info3 = arraysizeof(CHAR_list_info3_int);
int list_event = arraysizeof(CHAR_list_event_int);
int list_info4 = arraysizeof(CHAR_list_info4_int);
int list_attackmagic = arraysizeof(CHAR_list_attackmagic_int);
int list_info5 = arraysizeof(CHAR_list_info5_int);
int list_profession = arraysizeof(CHAR_list_profession_int);
int list_info6 = arraysizeof(CHAR_list_info6_int);

int list_info = arraysizeof(CHAR_list_info_char);

void CHAR_make_list_int_String( Char* ch, char (*data)[64], char *buf, int start, int end )
{
	if (!ch) return;
	int i;
	char token[1024]="";
	for( i = start ; i < end ; i ++ ){
    char linedata[256]="";
    sprintf( linedata, "%d",
              ch->data[i] );
    if(i < end - 1){
	    strcat(linedata, ",");
	  }
    strcat( token, linedata );
  }
  sprintf(buf, "('%s', %d, %s)", ch->string[CHAR_CDKEY].string, ch->data[CHAR_SAVEINDEXNUMBER], token);
}

void CHAR_make_list_char_String( Char* ch, char (*data)[64], char *buf, int start, int end )
{
	if (!ch) return;
	int i;
	char token[1024]="";
	for( i = start ; i < end ; i ++ ){
    char    linedata[256]="";
    sprintf( linedata, "'%s'",
              ch->string[i].string );
    if(i < end - 1){
	    strcat(linedata, ",");
	  }
    strcat( token, linedata );
  }
  sprintf(buf, "('%s',%d, %s)", ch->string[CHAR_CDKEY].string, ch->data[CHAR_SAVEINDEXNUMBER], token);
}

void CHAR_make_list_flg_String( Char* ch, char *buf)
{
	if (!ch) return;
	int i;
	int len = arraysizeof( ch->flg );
	char token[1024]="";
	for( i = 0 ; i < len ; i ++ ){
 	  char    linedata[128]="";
 	  sprintf( linedata, "%d", ch->flg[i]);
 	  
		if(i < len - 1){
	    strcat(linedata, ",");
	  }
	  
 	  strcat( token, linedata );
 	}
 	sprintf( buf, "('%s', %d, '%s')", ch->string[CHAR_CDKEY].string, ch->data[CHAR_SAVEINDEXNUMBER], token);
}

void CHAR_make_list_skill_String( Char* ch, char *buf)
{
	if (!ch) return;
	int i;
	char lv[1024]="";
	char id[1024]="";
	for( i = 0 ; i < CHAR_SKILLMAXHAVE ; i ++ ){
		char lvtmp[128]="";
		char idtmp[128]="";
		sprintf( lvtmp, "%d", ch->haveSkill[i].skill.data[SKILL_LEVEL]);

		sprintf( idtmp, "%d", ch->haveSkill[i].skill.data[SKILL_IDENTITY]);
		
		if(i < CHAR_SKILLMAXHAVE - 1){
	    strcat(lvtmp, ",");
	    strcat(idtmp, ",");
	  }
	  strcat( lv, lvtmp );
		strcat( id, idtmp );
	}
	sprintf( buf, "('%s', %d, '%s', '%s')", ch->string[CHAR_CDKEY].string, ch->data[CHAR_SAVEINDEXNUMBER], lv, id);
}
extern ITEM_intDataSetting ITEM_setintdata[26];
extern ITEM_intDataSetting ITEM_setchardata[13];
extern ITEM_exists* ITEM_item;

void CHAR_make_list_item_String( Char* ch, char *buf)
{
	if (!ch) return;
	int i, j;
	char token[STRINGBUFSIZ]="";
	int lenint=arraysizeof( ITEM_setintdata);
	int lenchar=arraysizeof( ITEM_setchardata);
  for( i = 0 ; i <  CheckCharMaxItemChar(ch) && i < CHAR_MAXPOOLITEMHAVE ; i ++ ){
    if(i <  CheckCharMaxItemChar(ch)){
	    if( ch->indexOfExistItems[i] != -1 ){
	    	char linedata[256]="";
		    char itemint[STRINGBUFSIZ]="";
		    char itemchar[STRINGBUFSIZ]="";
	    	for(j = 0; j < lenint ; j ++ ){
	    		sprintf(linedata, "%d",
	    								ITEM_item[ch->indexOfExistItems[i]].itm.data[ITEM_setintdata[j].table]);
	    		if(j < lenint - 1){
				    strcat(linedata, ",");
				  }
		    	strcat(itemint, linedata);
	    	}
	    	for(j = 0; j < lenchar ; j ++ ){
	    		sprintf(linedata, "'%s'",
	    								ITEM_item[ch->indexOfExistItems[i]].itm.string[ITEM_setchardata[j].table].string);		
	    		if(j < lenchar - 1){
				    strcat(linedata, ",");
				  }
		    	strcat(itemchar, linedata);
	    	}
	    	sprintf(token, "('%s', %d, %d, 0, %s, %s),", ch->string[CHAR_CDKEY].string, ch->data[CHAR_SAVEINDEXNUMBER], i, itemint, itemchar);
				strcat(buf, token);
	    }
	  }
    if(i < CHAR_MAXPOOLITEMHAVE){
	    if( ch->indexOfExistPoolItems[i] != -1 ){
	    	char linedata[256]="";
		    char itemint[STRINGBUFSIZ]="";
		    char itemchar[STRINGBUFSIZ]="";
	    	for(j = 0; j < lenint ; j ++ ){
	    		sprintf(linedata, "%d", 
	    								ITEM_item[ch->indexOfExistPoolItems[i]].itm.data[ITEM_setintdata[j].table]);
	    		if(j < lenint - 1){
				    strcat(linedata, ",");
				  }
		    	strcat(itemint, linedata);
	    	}
	    	for(j = 0; j < lenchar ; j ++ ){
	    		sprintf(linedata, "'%s'", 
	    								ITEM_item[ch->indexOfExistPoolItems[i]].itm.string[ITEM_setchardata[j].table].string);		
	    		if(j < lenchar - 1){
				    strcat(linedata, ",");
				  }
		    	strcat(itemchar, linedata);
	    	}
	    	sprintf(token, "('%s', %d, %d, 1, %s, %s),", ch->string[CHAR_CDKEY].string, ch->data[CHAR_SAVEINDEXNUMBER], i, itemint, itemchar);
				strcat(buf, token);
	    }
	  }
  }
  buf[strlen(buf)-1]='\0';
}

void CHAR_make_list_title_String( Char* ch, char *buf)
{
	if (!ch) return;
	int i;
	int len = arraysizeof( ch->flg );
	char token[1024]="";
	for( i = 0 ; i < len ; i ++ ){
 	  char    linedata[128]="";
 	  sprintf( linedata, "%d", ch->indexOfHaveTitle[i]);
 	  
		if(i < len - 1){
	    strcat(linedata, ",");
	  }
	  
 	  strcat( token, linedata );
 	}
 	sprintf( buf, "('%s', %d, '%s')", ch->string[CHAR_CDKEY].string, ch->data[CHAR_SAVEINDEXNUMBER], token);
}

void CHAR_make_list_mail_String( Char* ch, char *buf)
{
	if (!ch) return;
	int i;
	for( i = 0 ; i < ADDRESSBOOK_MAX ; i ++ ){
		char token[1024]="";
		ADDRESSBOOK_entry *mail = &ch->addressBook[i];
		if( mail->use == 0 )continue;
	  sprintf( token,
			  "('%s',%d,'%s','%s',%d,%d,%d,%d),",
			  ch->string[CHAR_CDKEY].string, ch->data[CHAR_SAVEINDEXNUMBER], mail->cdkey, mail->charname, mail->level, mail->duelpoint, mail->graphicsno,mail->transmigration);
		strcat( buf, token );
 	}
 	buf[strlen(buf)-1]='\0';
}

void CHAR_CharadataSaveSQL(Char* ch)
{
	if (!ch) return;
	int start=0, end=0;
	memset(&charadata, 0, sizeof(charadata));
	CHAR_make_list_String(ch, charadata.CHAR_list_String);
	start=end;
	end=list_info1;
	CHAR_make_list_int_String(ch, CHAR_list_info1_int, charadata.CHAR_list_info1_String, start, end);
	start=end;
	end+=list_info2;
	CHAR_make_list_int_String(ch, CHAR_list_info2_int, charadata.CHAR_list_info2_String, start, end);
	start=end;
	end+=list_count;
	CHAR_make_list_int_String(ch, CHAR_list_count_int, charadata.CHAR_list_count_String, start, end);
	start=end;
	end+=list_info3;
	CHAR_make_list_int_String(ch, CHAR_list_info3_int, charadata.CHAR_list_info3_String, start, end);
	start=end;
	end+=list_event;
	CHAR_make_list_int_String(ch, CHAR_list_event_int, charadata.CHAR_list_event_String, start, end);
	start=end;
	end+=list_info4;
	CHAR_make_list_int_String(ch, CHAR_list_info4_int, charadata.CHAR_list_info4_String, start, end);
	start=end;
	end+=list_attackmagic;
	CHAR_make_list_int_String(ch, CHAR_list_attackmagic_int, charadata.CHAR_list_attackmagic_String, start, end);
	start=end;
	end+=list_info5;
	CHAR_make_list_int_String(ch, CHAR_list_info5_int, charadata.CHAR_list_info5_String, start, end);
	start=end;
	end+=list_profession;
	CHAR_make_list_int_String(ch, CHAR_list_profession_int, charadata.CHAR_list_profession_String, start, end);
	start=end;
	end+=list_info6;
	CHAR_make_list_int_String(ch, CHAR_list_info6_int, charadata.CHAR_list_info6_String, start, end);
	CHAR_make_list_char_String(ch, CHAR_list_info_char, charadata.CHAR_list_info_String, 0, list_info);

	CHAR_make_list_flg_String(ch, charadata.CHAR_list_flg_String);
	CHAR_make_list_skill_String(ch, charadata.CHAR_list_skill_String);
	CHAR_make_list_item_String(ch, charadata.CHAR_list_item_String);
	CHAR_make_list_title_String(ch, charadata.CHAR_list_title_String);
	CHAR_make_list_mail_String(ch, charadata.CHAR_list_mail_String);

	saacproto_CharadataSaveSQL_send(acfd, ch->string[CHAR_CDKEY].string, charadata, ch->data[CHAR_SAVEINDEXNUMBER], 0);
}

/*
char* CHAR_makeStringFromCharSQL( Char* one )
{
  int     i;
	int		petnum;
	int     strlength=0;

  if (!one) return "\0";

	memset( CHAR_dataString, 0, sizeof(CHAR_dataString));
    for( i = 0 ; i < CHAR_DATAINTNUM ; i ++ ){
        char    linedata[256];
        snprintf( linedata , sizeof(linedata),
                  "%s=%d",
                  CHAR_setintdata[i],
                  one->data[i] );
                  
        strcat(linedata, ",");
            
        strcpysafe( &CHAR_dataString[strlength],
                    sizeof( CHAR_dataString ) - strlength,
                    linedata );
        strlength += strlen( linedata );
        
        if( strlength > sizeof( CHAR_dataString ) ) {
					fprint( "err charadata buffer over\n");
		      goto RETURN;
				}
    }
    for( i = 0 ; i < CHAR_DATACHARNUM ; i ++ ){
        char    linedata[256];
        char    escapebuffer[256];
        snprintf( linedata , sizeof(linedata),
                  "%s='%s'",
                  CHAR_setchardata[i],
                  makeEscapeString( one->string[i].string, escapebuffer,
                                    sizeof( escapebuffer) ));

				if(i < CHAR_DATACHARNUM - 1){
        	strcat(linedata, ",");
        }
        
        strcpysafe( &CHAR_dataString[strlength],
                    sizeof( CHAR_dataString ) - strlength,
                    linedata );
        strlength += strlen( linedata );
        if( strlength > sizeof( CHAR_dataString ) ) {
					fprint( "err charadata buffer over\n");
		      goto RETURN;
		    }
    }

    for( i = 0 ; i < arraysizeof( one->flg ) ; i ++ ){
        char    linedata[128];
        snprintf( linedata , sizeof(linedata),
                  FLGRESERVESTRING "%d=%d" CHAR_DELIMITER,
                  i, one->flg[i]);

        strcpysafe( &CHAR_dataString[strlength],
                    sizeof( CHAR_dataString ) - strlength,
                    linedata );
        strlength += strlen( linedata );
        if( strlength > sizeof( CHAR_dataString ) ) {
					fprint( "err charadata buffer over\n");
        	goto RETURN;
    	}
    }

    for( i = 0 ; i < CHAR_SKILLMAXHAVE ; i ++ ){
        char    linedata[128];
        char    *skillstring="\0";
        if( one->haveSkill[i].use == TRUE)
            skillstring = SKILL_makeStringFromSkillData(
                &one->haveSkill[i].skill );
        if( skillstring == "\0" ) continue;
        snprintf( linedata , sizeof( linedata ),
                  SKILLRESERVESTRING "%d=%s" CHAR_DELIMITER,
                  i,skillstring );
        strcpysafe( &CHAR_dataString[strlength],
                    sizeof( CHAR_dataString ) - strlength,
                    linedata );
        strlength += strlen( linedata );
        if( strlength > sizeof( CHAR_dataString ) ) {
			fprint( "err charadata buffer over\n");
        	goto RETURN;
    	}
    }

    for( i = 0 ; i <  CheckCharMaxItemChar(one) ; i ++ ){
        char    linedata[2048];
        char    *itemstring="\0";
        if( one->indexOfExistItems[i] != -1 )
            itemstring = ITEM_makeStringFromItemIndex(
                							one->indexOfExistItems[i], 0);
        if( itemstring == "\0" )continue;
        snprintf( linedata , sizeof( linedata ),
                  ITEMRESERVESTRING "%d=%s" CHAR_DELIMITER,
                  i, itemstring);
        strcpysafe( &CHAR_dataString[strlength],
                    sizeof( CHAR_dataString ) - strlength,
                    linedata );

        strlength += strlen( linedata );
        if( strlength > sizeof( CHAR_dataString ) ) {
			fprint( "err charadata buffer over\n");
        	goto RETURN;
    	}
    }

    for( i = 0 ; i < CHAR_MAXPOOLITEMHAVE ; i ++ ){
        char    linedata[2048];
        char    *itemstring="\0";
        if( one->indexOfExistPoolItems[i] != -1 )
            itemstring = ITEM_makeStringFromItemIndex(
                							one->indexOfExistPoolItems[i], 0);
        if( itemstring == "\0" )continue;
        snprintf( linedata , sizeof( linedata ),
                  POOLITEMRESERVESTRING "%d=%s" CHAR_DELIMITER,
                  i, itemstring);
        strcpysafe( &CHAR_dataString[strlength],
                    sizeof( CHAR_dataString ) - strlength,
                    linedata );
        strlength += strlen( linedata );
        if( strlength > sizeof( CHAR_dataString ) ) {
			fprint( "err charadata buffer over\n");
        	goto RETURN;
    	}
    }

    for( i = 0 ; i < CHAR_TITLEMAXHAVE ; i ++ ){
        char    linedata[128];
		if( one->indexOfHaveTitle[i]< 0 || one->indexOfHaveTitle[i] >= CHAR_TITLEMAXHAVE ) {
			continue;
		}
        snprintf( linedata , sizeof( linedata ),
                  TITLERESERVESTRING "%d=%d" CHAR_DELIMITER,
                  i, one->indexOfHaveTitle[i] );
        strcpysafe( &CHAR_dataString[strlength],
                    sizeof( CHAR_dataString ) - strlength,
                    linedata );
        strlength += strlen( linedata );
        if( strlength > sizeof( CHAR_dataString ) ) {
			fprint( "err charadata buffer over\n");
        	goto RETURN;
    	}
    }
    for( i=0 ; i < ADDRESSBOOK_MAX ; i++ ){
        char    linedata[512];
		char	*p;
		p = ADDRESSBOOK_makeAddressbookString(&one->addressBook[i]);
		if( strlen( p) == 0 ) {
			continue;
		}
        snprintf( linedata ,sizeof(linedata),
                  ADDRESSBOOKRESERVESTRING "%d=%s" CHAR_DELIMITER,
                  i,p
            );
        strcpysafe( &CHAR_dataString[strlength],
                    sizeof( CHAR_dataString ) - strlength,
                    linedata );
        strlength += strlen( linedata );
        if( strlength > sizeof( CHAR_dataString ) ) {
			fprint( "err charadata buffer over\n");
        	goto RETURN;
    	}
    }
    for( petnum = 0; petnum < CHAR_MAXPETHAVE; petnum ++ ) {
		int	petindex;
		char	linedata[4096*2];
		char	*petstring="\0";
		petindex = one->unionTable.indexOfPet[petnum];
	    if( !CHAR_CHECKINDEX(petindex))continue;
		petstring = CHAR_makePetStringFromPetIndex( petindex);
		if( petstring == "\0" ) continue;
        snprintf( linedata , sizeof(linedata),
                  PETSERVERSTRING "%d=%s" CHAR_DELIMITER,petnum,petstring );
        strcpysafe( &CHAR_dataString[strlength],
                    sizeof( CHAR_dataString ) - strlength,
                    linedata );
        strlength += strlen( linedata );
        if( strlength > sizeof( CHAR_dataString ) ) {
			fprint( "err charadata buffer over\n");
        	goto RETURN;
		}
	}

    for( petnum = 0; petnum < CHAR_MAXPOOLPETHAVE; petnum ++ ) {
		int	petindex;
		char	linedata[4096*2];
		char	*petstring="\0";
		petindex = one->indexOfPoolPet[petnum];
	    if( !CHAR_CHECKINDEX(petindex))continue;
		petstring = CHAR_makePetStringFromPetIndex( petindex);
		if( petstring == "\0" ) continue;

		//andy_log
        if( strstr( petstring, "name:") == NULL ||
			strstr( petstring, "ownt:") == NULL ) {
			LodBadPetString( "petstring", "poolpet string buffer err:", petnum);
			fprint( "ANDY err poolpet string buffer err:\n%s\n", petstring);
        	goto RETURN;
		}

        snprintf( linedata , sizeof(linedata),
                  POOLPETSERVERSTRING "%d=%s" CHAR_DELIMITER,petnum,petstring );

        strcpysafe( &CHAR_dataString[strlength],
                    sizeof( CHAR_dataString ) - strlength,
                    linedata );
        strlength += strlen( linedata );
        if( strlength > sizeof( CHAR_dataString ) ) {
			fprint( "err charadata buffer over\n");
        	goto RETURN;
		}
	}
#ifdef _CHAR_FIXDATADEF
	{
		char linedata[1024];
		memset( linedata, 0, sizeof(linedata));
        snprintf( linedata , sizeof(linedata),
                  DATAENDCHECKPOINT "=%d" CHAR_DELIMITER, (int)time(NULL)/1000);

        strcpysafe( &CHAR_dataString[strlength],
                    sizeof( CHAR_dataString ) - strlength, linedata );

		if( strlength > sizeof( CHAR_dataString ) ) {
			LodBadPetString( "DATAENDCHECKPOINT", "err charadata buffer over", -1);
			fprint( "err charadata buffer over:DATAENDCHECKPOINT !\n");
        	goto MAKESTRINGERR;
		}
	}
#endif

RETURN:
	if( strstr( CHAR_dataString, DATAENDCHECKPOINT) == NULL ){
		LodBadPetString( CHAR_dataString, "err mk dataString no end", -1);
	}

    return CHAR_dataString;
MAKESTRINGERR:
	return "\0";
}
*/




#endif

