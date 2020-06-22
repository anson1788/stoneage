#include "version.h"
#include <stdio.h>
#include <ctype.h>
#include <time.h>
//???
#include "common.h"
#include "handletime.h"
#include "object.h"
#include "char_base.h"
#include "char.h"
#include "char_event.h"
#include "lssproto_serv.h"
#include "readmap.h"
#include "map_deal.h"
#include "npccreate.h"
#include "encount.h"
#include "npcutil.h"
#include "battle.h"
#include "net.h"
#include "configfile.h"
#include "npc_npcenemy.h"
#ifdef _ALLBLUES_LUA
#include "mylua/function.h"
#endif
void CHAR_sendCharaAtWalk( int charaindex, int of, int ox, int oy, int xflg, int yflg );
static void CHAR_sendCDCharaAtWalk( int charaindex, int of, int ox, int oy,	int xflg, int yflg );

static CHAR_WALKRET CHAR_walk_turn( int index, int dir )
{
    int     fx,fy,ff;
    OBJECT  object;

    if(!CHAR_CHECKINDEX( index ))return CHAR_WALKSYSTEMERROR;
    CHAR_setInt(index,CHAR_DIR,dir);
    fx = CHAR_getInt(index,CHAR_X);
    fy = CHAR_getInt(index,CHAR_Y);
    ff = CHAR_getInt(index,CHAR_FLOOR);
    CHAR_sendWatchEvent( CHAR_getWorkInt(index, CHAR_WORKOBJINDEX),
                         CHAR_ACTTURN,NULL,0,FALSE);
		CHAR_setWorkInt( index, CHAR_WORKACTION, CHAR_ACTTURN);
    for( object = MAP_getTopObj(ff,fx,fy) ; object ;
         object = NEXT_OBJECT(object ) ){
        typedef void (*POSTOFUNC)(int,int);
        POSTOFUNC   pfunc=NULL;
        int objindex = GET_OBJINDEX(object);

        switch( OBJECT_getType(objindex)  ){
        case OBJTYPE_CHARA:
            pfunc = (POSTOFUNC)CHAR_getFunctionPointer(
                OBJECT_getIndex(objindex),
                CHAR_POSTOVERFUNC);
            break;
        case OBJTYPE_ITEM:
            pfunc = (POSTOFUNC)ITEM_getFunctionPointer(
                OBJECT_getIndex(objindex),
                ITEM_POSTOVERFUNC);
#ifdef _ALLBLUES_LUA_1_2 
					if( !pfunc ){
						RunItemPostOverEvent(OBJECT_getIndex(objindex), index );
					}
#endif
            break;
        case OBJTYPE_GOLD:
            break;
        default:
            break;
        }
        if( pfunc )pfunc( OBJECT_getIndex(objindex),index );
    }
    return CHAR_WALKSUCCESSED;
}


/*------------------------------------------------------------
 * n 汹 汹中凶map毛霜月
 * 娄醒
 *  index       int     平乓仿及奶件犯永弁旦
 *  fl          int     白夫失ID
 *  ox          int     蟆及橇谪及x
 *  oy          int     蟆及橇谪及y
 *  fx          int     汹中凶  及x
 *  fy          int     汹中凶  及y
 * 忒曰袄
 *  卅仄
 ------------------------------------------------------------*/
static void CHAR_sendMapAtWalk( int index, int fl ,int ox ,int oy,
								int fx, int fy )
{
    /*
     *  蜇箕及袄午｝蟆引匹及袄及犒毛苇月
     *  仇丹允月午｝伐□皿NPC手棵签分［
     */
    int     vx,vy;
    const int     seesiz = MAP_CHAR_DEFAULTSEESIZ;

    if( !CHAR_CHECKINDEX( index ) )return;

    vx = fx - ox;
    vy = fy - oy;

    if( ABS(vx) >= seesiz/2 ||       /*  犒互  五中  */
        ABS(vy) >= seesiz/2 ){       /*  犒互  五中  */
        return;

    }else{
        RECT    send,get;
        char*   mapdata;
        int oldlux = ox - (int)(seesiz/2);
        int oldluy = oy - (int)(seesiz/2);
        int absx = ABS(vx);
        int absy = ABS(vy);
        /*
         * 夫午勾聂户卞旦正□玄毛潸月及午｝vx , vy 井日1夫仁及互禾奶件玄
         * 蟆午井今卅日卅中方丹卞｝
         * clipRect 反 w,h 0 毛心午户化中月
         */

        /*  == 0 及凛反霜日卅仁化方中   */
        if( vx != 0 ){
            if( vx > 0 ){
                send.x     = oldlux + seesiz;
                send.width = absx;
            }else if( vx < 0 ){
                send.x     = oldlux - absx;
                send.width = absx;
            }

            if( vy >= 0 ){
                send.y      = oldluy;
                send.height = seesiz + absy;
            }else{
                send.y      = oldluy - absy;
                send.height = seesiz + absy;
            }
{
            int     tilesum, objsum,eventsum;
            mapdata = MAP_getChecksumFromRECT(fl,&send,&get,&tilesum, &objsum,&eventsum );
            if( mapdata != NULL
                && !( get.width == 0 && get.height == 0 ) ){
                int fd = getfdFromCharaIndex( index );
                if( fd !=  -1 )
                    lssproto_MC_send( fd,fl,get.x,get.y,
                                     get.x+get.width,
                                     get.y+get.height,
                                     tilesum,
                                     objsum,
                                     eventsum,
                                     mapdata );
            }
}
        }

        /*  == 0 及凛反霜日卅仁化方中   */
        if( vy != 0 ){
            if( vy > 0 ){
                send.y      = oldluy + seesiz;
                send.height = absy;
            }else{
                send.y      = oldluy - absy;
                send.height = absy;
            }
            if( vx >= 0 ){
                send.x     = oldlux;
                send.width = seesiz + absx;
            }else if( vx < 0 ){
                send.x     = oldlux - absx;
                send.width = seesiz + absx;
            }
{
            int     tilesum, objsum,eventsum;
            mapdata = MAP_getChecksumFromRECT(fl,&send,&get, &tilesum, &objsum,&eventsum);
            if( mapdata != NULL
                && !( get.width == 0 && get.height == 0 ) ){
                int fd = getfdFromCharaIndex( index );
                if( fd !=  -1 )
                    lssproto_MC_send( fd,fl,get.x,get.y,
                                     get.x+get.width,
                                     get.y+get.height,
                                     tilesum,
                                     objsum,
                                     eventsum,
                                     mapdata );

            }
}
        }
    }

}
static CHAR_WALKRET CHAR_walk_move( int charaindex, int dir );
/*------------------------------------------------------------
 * 啖  允月［
 * 娄醒
 *  index       int     平乓仿及奶件犯永弁旦
 *  dir         int       轾
 * 忒曰袄
 *  CHAR_WALKRET
 ------------------------------------------------------------*/
static CHAR_WALKRET CHAR_walk_move( int charaindex, int dir )
{
    int     i;
    int     fx,fy,ff;
    int     ox,oy,of;
    int     objbuf[128];
    int     objbufindex=0;
    int     notover=FALSE;
    int     retvalue=CHAR_WALKSUCCESSED;

    if( !CHAR_CHECKINDEX( charaindex )) return CHAR_WALKSYSTEMERROR;
    ox = CHAR_getInt(charaindex,CHAR_X);
    oy = CHAR_getInt(charaindex,CHAR_Y);
    of = CHAR_getInt(charaindex,CHAR_FLOOR);
    CHAR_getCoordinationDir( dir,ox,oy,1,&fx,&fy);
    ff = of;

    if( CHAR_getInt( charaindex,CHAR_WHICHTYPE ) == CHAR_TYPEENEMY ){
        RECT    walkr;
        POINT   nextp;
        int npccreateindex;
        npccreateindex = CHAR_getInt( charaindex,CHAR_NPCCREATEINDEX );
        if( CHAR_isInvincibleArea( ff,fx,fy)) {
            CHAR_setInt(charaindex,CHAR_DIR,dir);
            retvalue = CHAR_WALKEXTEND;
            goto CHAR_AFTERWALK;
        }
        if( NPC_isBoundarySet( npccreateindex ) &&
            NPC_createGetRECT( npccreateindex,&walkr ) == TRUE )
        {
            nextp.x = fx;
            nextp.y = fy;
            if( PointInRect( &walkr,&nextp ) == FALSE ){
                CHAR_setInt(charaindex,CHAR_DIR,dir);
                retvalue = CHAR_WALKEXTEND;
                goto CHAR_AFTERWALK;
            }
        }else{
            ;
        }
    }


    if( CHAR_getFlg( charaindex, CHAR_ISBIG ) == 1 ){
        static POINT offset[5]={
            { 0,-1},
            {-1, 0},
            { 0, 0},
            { 1, 0},
            { 0, 1},
        };
        int     k;
        for( k=0 ; k<5; k++ ){
            if( !MAP_walkAble( charaindex, ff, fx+offset[k].x,
                               fy+offset[k].y )){
                CHAR_setInt(charaindex,CHAR_DIR,dir);
                retvalue = CHAR_WALK1357;
                goto CHAR_AFTERWALK;
            }
        }

    }else{
        if( CHAR_getDX(dir)*CHAR_getDY(dir) == 0 ){
            if( !MAP_walkAble( charaindex,ff, fx, fy ) ){
                CHAR_setInt(charaindex,CHAR_DIR,dir);
                retvalue = CHAR_WALK1357;
                goto CHAR_AFTERWALK;
            }
        }else{
            int     xflg,yflg;
            if( !MAP_walkAble( charaindex,ff, fx, fy ) ){
                CHAR_setInt(charaindex,CHAR_DIR,dir);
                retvalue = CHAR_WALK1357;
                goto CHAR_AFTERWALK;
            }

            xflg = MAP_walkAble( charaindex,of, ox+CHAR_getDX(dir), oy );
            yflg = MAP_walkAble( charaindex,of, ox, oy+CHAR_getDY(dir) );

            if( !xflg || !yflg ) {
                CHAR_setInt(charaindex,CHAR_DIR,dir);
                retvalue = CHAR_WALK1357;
                goto CHAR_AFTERWALK;
            }
        }
    }
    objbufindex = CHAR_getSameCoordinateObjects( objbuf, arraysizeof( objbuf ), ff,fx,fy );
    for( i = 0 ; i < objbufindex ; i ++ ){
        int     objindex= objbuf[i];
        switch( OBJECT_getType(objindex) ){
        case OBJTYPE_CHARA:
        	if(CHAR_CHECKINDEX(OBJECT_getIndex(objindex))==TRUE){
#ifdef _ALLBLUES_LUA
						int meindex=-1, toindex=-1;
						if(CHAR_getInt(charaindex, CHAR_WHICHTYPE)==CHAR_TYPEPLAYER){
								if(CHAR_getInt(OBJECT_getIndex(objindex), CHAR_WHICHTYPE)==CHAR_TYPELUANPC){
									meindex = OBJECT_getIndex(objindex);
									toindex = charaindex;
								}
						}else if(CHAR_getInt(charaindex, CHAR_WHICHTYPE)==CHAR_TYPELUANPC){
								if(CHAR_getInt(OBJECT_getIndex(objindex), CHAR_WHICHTYPE)==CHAR_TYPEPLAYER){
									meindex = charaindex;
									toindex = OBJECT_getIndex(objindex);
								}
						}
						if(CHAR_CHECKINDEX(meindex) == TRUE || CHAR_CHECKINDEX(toindex) == TRUE){
							if( CHAR_getWorkInt( meindex, CHAR_WORKBATTLEMODE ) == BATTLE_CHARMODE_NONE
									&& CHAR_getWorkInt( toindex, CHAR_WORKBATTLEMODE ) == BATTLE_CHARMODE_NONE ){
								if(CHAR_getFlg(meindex, CHAR_ISVISIBLE)==TRUE && CHAR_getFlg(meindex, CHAR_ISDIE) == FALSE){
									if( CHAR_getWorkInt( meindex, CHAR_WORKPARTYMODE )!= CHAR_PARTY_CLIENT && CHAR_getWorkInt( toindex, CHAR_WORKPARTYMODE )!= CHAR_PARTY_CLIENT ){
										RunCharOverlapEvent(meindex, toindex);
									}
								}
							}
						}
#endif
#ifdef _PLAYER_OVERLAP_PK
						if(CHAR_getInt(charaindex, CHAR_WHICHTYPE)==CHAR_TYPEPLAYER
							&& CHAR_getInt(OBJECT_getIndex(objindex), CHAR_WHICHTYPE)==CHAR_TYPEPLAYER ){
								if( CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE )!= CHAR_PARTY_NONE ){
									int i;
									for( i = 0; i < getPartyNum(charaindex); i ++ ){
										int subindex = CHAR_getWorkInt( charaindex, CHAR_WORKPARTYINDEX1+i );
										if( CHAR_CHECKINDEX( subindex ) == FALSE ) continue;
										if(OBJECT_getIndex(objindex) == subindex)break;
									}

									if(i == getPartyNum(charaindex)){
										int j;
										for(j=0;j<32;j++){
											if(CHAR_getInt(charaindex, CHAR_FLOOR) == getPlayerOverlapPk(j)){
												BATTLE_CreateVsPlayer(charaindex, OBJECT_getIndex(objindex));
												break;
											}
										}
									}
								}
						}
#endif
					}
            if( !CHAR_getFlg( OBJECT_getIndex(objindex),CHAR_ISOVERED) ){
            	if(!CHAR_CHECKINDEX(OBJECT_getIndex(objindex))){
	            	printf("自动删除一个问题对象！\n");
	        			endObjectOne(objindex);
	        			break;
	        		}
              notover= TRUE;
            }
            break;
        case OBJTYPE_ITEM:
            if( !ITEM_getInt( OBJECT_getIndex(objindex), ITEM_ISOVERED ) )
                notover= TRUE;
            break;
        case OBJTYPE_GOLD:
            break;
        default:
            break;
        }
    }
    if( notover == TRUE ){
        CHAR_setInt(charaindex,CHAR_DIR,dir);
        retvalue = CHAR_WALKHITOBJECT;
    }else{
        for( i = 0 ; i < objbufindex ; i ++ ){
            typedef void (*PREOFUNC)(int,int);
            PREOFUNC   pfunc=NULL;
            int     objindex=objbuf[i];
            switch( OBJECT_getType(objindex) ){
            case OBJTYPE_CHARA:
                pfunc = (PREOFUNC)CHAR_getFunctionPointer(
                    OBJECT_getIndex(objindex),
                    CHAR_PREOVERFUNC);
                break;
            case OBJTYPE_ITEM:
                pfunc = (PREOFUNC)ITEM_getFunctionPointer(
                    OBJECT_getIndex(objindex),
                    ITEM_PREOVERFUNC);
#ifdef _ALLBLUES_LUA_1_2 
								if( !pfunc ){
									RunItemPreOverEvent(OBJECT_getIndex(objindex), charaindex );
								}
#endif
                break;
            case OBJTYPE_GOLD:
                break;
            default:
                break;
            }
            if( pfunc )pfunc( OBJECT_getIndex(objindex),charaindex );
        }

        CHAR_setInt(charaindex,CHAR_X,fx);
        CHAR_setInt(charaindex,CHAR_Y,fy);
        CHAR_setInt(charaindex,CHAR_FLOOR,ff);
        CHAR_setInt(charaindex,CHAR_DIR,dir);
        {
            int objindex;
            int ox,oy,of;
            objindex = CHAR_getWorkInt( charaindex,CHAR_WORKOBJINDEX );
            of = OBJECT_setFloor(objindex,
                                 CHAR_getInt(charaindex,CHAR_FLOOR));
            ox = OBJECT_setX(objindex,CHAR_getInt(charaindex,CHAR_X));
            oy = OBJECT_setY(objindex,CHAR_getInt(charaindex,CHAR_Y));
            if( !MAP_objmove( objindex, of,ox,oy,ff,fx,fy ) ){
                /*  仇氏卅氏升丹仄方丹手卅中    */
   //             fprint( "ERROR MAP_OBJMOVE objindex=%d(%s)\n",objindex, 	CHAR_getUseName( charaindex ) );
            }
        }

        CHAR_setInt(charaindex,CHAR_WALKCOUNT,
                    CHAR_getInt(charaindex,CHAR_WALKCOUNT) + 1 );


        for( i = 0 ; i < objbufindex ; i ++ ){
            typedef void (*POSTOFUNC)(int,int);
            POSTOFUNC   pfunc=NULL;
            int     objindex=objbuf[i];

            switch( OBJECT_getType(objindex) ){
            case OBJTYPE_CHARA:
                pfunc = (POSTOFUNC)CHAR_getFunctionPointer(
                    OBJECT_getIndex(objindex),
                    CHAR_POSTOVERFUNC);
                break;
            case OBJTYPE_ITEM:
                pfunc = (POSTOFUNC)ITEM_getFunctionPointer( OBJECT_getIndex(objindex), ITEM_POSTOVERFUNC);
#ifdef _ALLBLUES_LUA_1_2 
								if( !pfunc ){
									RunItemPostOverEvent(OBJECT_getIndex(objindex), charaindex );
								}
#endif
                break;
            case OBJTYPE_GOLD:
                break;
            default:
                break;
            }
            if( pfunc )pfunc( OBJECT_getIndex(objindex),charaindex );
        }
        objbufindex = CHAR_getSameCoordinateObjects(objbuf, arraysizeof(objbuf), of, ox, oy );
        for( i=0 ;i<objbufindex;i++){
            typedef void (*OFFFUNC)(int,int);
            OFFFUNC   ofunc=NULL;
            int     objindex = objbuf[i];

            switch( OBJECT_getType( objindex ) ){
            case OBJTYPE_CHARA:
                ofunc = (OFFFUNC)CHAR_getFunctionPointer( OBJECT_getIndex(objindex), CHAR_OFFFUNC);
                break;
            case OBJTYPE_ITEM:
                /*    卞窒手仄卅中  */
                break;
            case OBJTYPE_GOLD:
                /*    卞窒手仄卅中  */
                break;
            default:
                break;
            }
            if( ofunc )ofunc( OBJECT_getIndex(objindex), charaindex );

        }
    }
CHAR_AFTERWALK:
    if( retvalue == CHAR_WALK1357 || retvalue == CHAR_WALKHITOBJECT ){
	    {
	        int     opt[2] = { ox, oy};
	        CHAR_sendWatchEvent( CHAR_getWorkInt(charaindex,
	                                             CHAR_WORKOBJINDEX),
	                             CHAR_ACTWALK,opt,2,TRUE );
	    }
      CHAR_setWorkChar( charaindex, CHAR_WORKWALKARRAY, "");
			if( CHAR_getInt( charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER) {
			    CHAR_sendWatchEvent( CHAR_getWorkInt(charaindex, CHAR_WORKOBJINDEX),
	    		                     CHAR_ACTWARP,NULL,0,TRUE);
			}
    }else if( CHAR_getInt( charaindex, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER ){

			BOOL	flg = FALSE;
			int		par;
			int		count;
			CHAR_setWorkInt( charaindex, CHAR_WORKACTION, -1 );
			if( CHAR_getWorkInt( charaindex, CHAR_WORKBATTLEMODE ) == BATTLE_CHARMODE_NONE ){
				CHAR_sendCharaAtWalk( charaindex, of,ox,oy,CHAR_getDX(dir), CHAR_getDY(dir));
			}

	    {
	        int     opt[2] = { ox, oy};
	        CHAR_sendWatchEvent( CHAR_getWorkInt(charaindex,
	                                             CHAR_WORKOBJINDEX),
	                             CHAR_ACTWALK,opt,2,TRUE );
	    }
			//if( CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_CLIENT ) {
#ifdef _MO_MAP_AUTO_UPDATE
				CHAR_sendMapAtWalk( charaindex, of,
									ox,oy,
									CHAR_getInt( charaindex, CHAR_X),
									CHAR_getInt( charaindex, CHAR_Y));
#endif
			//}
			count = CHAR_getWorkInt( charaindex, CHAR_WORK_TOHELOS_COUNT);
			if( count > 0 ) {
				CHAR_setWorkInt( charaindex, CHAR_WORK_TOHELOS_COUNT, count -1);
				if( count -1 == 0 ) {
					CHAR_talkToCli( charaindex, -1, "道具的效力已到。", CHAR_COLORWHITE);
				}
			}
			par = ENCOUNT_getEncountPercentMin( charaindex, of,ox,oy);
			if( par != -1 ) {
				if( CHAR_getWorkInt( charaindex, CHAR_WORKENCOUNTPROBABILITY_MIN) != par ){
					flg = TRUE;
					CHAR_setWorkInt( charaindex, CHAR_WORKENCOUNTPROBABILITY_MIN, par);
				}
			}

			par = ENCOUNT_getEncountPercentMax( charaindex, of,ox,oy);
			if( par != -1 ) {
				if( CHAR_getWorkInt( charaindex, CHAR_WORKENCOUNTPROBABILITY_MAX) != par ){
					flg = TRUE;
					CHAR_setWorkInt( charaindex, CHAR_WORKENCOUNTPROBABILITY_MAX, par);
				}
			}
#ifdef _ALLBLUES_LUA_1_9
//			if(WalkFunction( charaindex ) == FALSE)
#endif
			{
			  int enfd = getfdFromCharaIndex( charaindex );
			  int eqen = getEqNoenemy( enfd );	// Arminius 7.2: Ra's amulet
			  int noen = getNoenemy(enfd);

	          // Arminius 7.31 cursed stone
	      if (getStayEncount(enfd)>0) {
				  clearStayEncount(enfd);
			  }
			  //print("\n enfd=%d,eqen=%d,noen=%d", enfd, eqen, noen);

			  // Arminius 7.2 Ra's amulet
			  if (eqen>=200) {
			    noen=1;
			  } else if (eqen>=120) {
			    if ((ff==100)||(ff==200)||(ff==300)||(ff==400)||(ff==500)) noen=1;
			  } else if (eqen>=80) {
			    if ((ff==100)||(ff==200)||(ff==300)||(ff==400)) noen=1;
			  } else if (eqen>=40) {
			    if ((ff==100)||(ff==200)) noen=1;
			  }

				if (noen>0) {
					if ((ff==8252)||(ff==8253)||(ff==8254)||(ff==8256)){
						noen=0;
					}
				}
			  //print("\n noen=%d", noen);
			  if (noen==0) {
					int maxep = CHAR_getWorkInt(charaindex, CHAR_WORKENCOUNTPROBABILITY_MAX);
			    int minep = CHAR_getWorkInt(charaindex, CHAR_WORKENCOUNTPROBABILITY_MIN);
			    int cep = CONNECT_get_CEP(enfd);

#ifdef _PROFESSION_SKILL			// WON ADD 人物职业技能
					int temp=0;
					int p_cep = CHAR_getWorkInt(charaindex, CHAR_ENCOUNT_FIX);
					if( p_cep != 0 ){
						if( CHAR_getWorkInt( charaindex, CHAR_ENCOUNT_NUM) < (int)time(NULL) ){
							CHAR_talkToCli( charaindex, -1, "技能效用结束。", CHAR_COLORYELLOW);
							CHAR_setWorkInt( charaindex, CHAR_ENCOUNT_FIX, 0);
							CHAR_setWorkInt( charaindex, CHAR_ENCOUNT_NUM, 0);
						}
						temp = cep * (100 + p_cep ) / 100;
					}else{
						temp = cep;
					}
#endif

			    if (cep<minep) cep=minep;
			    if (cep>maxep) cep=maxep;
			    if (CHAR_getWorkInt(charaindex,CHAR_WORKBATTLEMODE)==BATTLE_CHARMODE_NONE) {
						int entflag=1;
						{
							int objindex,index;
							OBJECT obj;
							for ( obj = MAP_getTopObj( ff, fx, fy); obj; obj = NEXT_OBJECT( obj)) {
								objindex = GET_OBJINDEX( obj);
								if (OBJECT_getType( objindex) == OBJTYPE_CHARA) {
									int etype;
									index = OBJECT_getIndex( objindex);
									if (!CHAR_CHECKINDEX( index)) continue;
									if( CHAR_getInt( index, CHAR_WHICHTYPE) == CHAR_TYPENPCENEMY &&
										CHAR_getWorkInt( index, CHAR_WORKEVENTTYPE) == CHAR_EVENT_ENEMY )	{
										CHAR_setInt( charaindex, CHAR_X, ox);
										CHAR_setInt( charaindex, CHAR_Y, oy);
										lssproto_XYD_send( getfdFromCharaIndex(charaindex),
										CHAR_getInt( charaindex, CHAR_X ),
										CHAR_getInt( charaindex, CHAR_Y ),
										CHAR_getInt( charaindex, CHAR_DIR ) );
										break;
									}
									etype = CHAR_getWorkInt( index, CHAR_WORKEVENTTYPE);
									if (etype!=CHAR_EVENT_NONE) {
										if (etype==CHAR_EVENT_WARP) entflag=0;
									}
								}
							}
						}


#ifdef _PROFESSION_SKILL			// WON ADD 人物职业技能
#ifdef _ENEMY_ACTION
						if( rand()%(120*getEnemyAction()) < temp )
#else
			  		if( rand()%120 < temp )
#endif
#else			
#ifdef _ENEMY_ACTION
						if( rand()%(120*getEnemyAction()) < cep )
#else
		    		if (rand()%120<cep)	// Arminius 6.28 lower encounter prob.
#endif
#endif
						{
							if (entflag) {
#ifdef _Item_MoonAct
								if( getEqRandenemy( enfd) > 0 )	{
									int Rnum=0;
									int RandEnemy = getEqRandenemy( enfd);
									Rnum = RAND(0,100);
									if( Rnum > RandEnemy )	{
#endif
										//encounter!!
										cep = minep;
										lssproto_EN_recv(enfd,
										CHAR_getInt(charaindex,CHAR_X),
										CHAR_getInt(charaindex,CHAR_Y));
#ifdef _Item_MoonAct
									}
								}else{
										cep = minep;
										lssproto_EN_recv(enfd,
										CHAR_getInt(charaindex,CHAR_X),
										CHAR_getInt(charaindex,CHAR_Y));
								}
#endif
							}
					  } else {
					    if (cep<maxep) cep++;
					  }
					}
			 		CONNECT_set_CEP(enfd, cep);
				}
			}
			if( CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE )!= CHAR_PARTY_CLIENT ){
				CHAR_setFlg( charaindex, CHAR_ISWARP, 0);
			}
    }else if( CHAR_getInt( charaindex, CHAR_WHICHTYPE ) == CHAR_TYPEPET ||
    		CHAR_getInt( charaindex, CHAR_WHICHTYPE) == CHAR_TYPEBUS
#ifdef _GAMBLE_ROULETTE	
			|| CHAR_getInt( charaindex, CHAR_WHICHTYPE) == CHAR_GAMBLEROULETTE
#endif
#ifdef _PETRACE
			|| CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_PETRACEPET
#endif
#ifdef _ALLBLUES_LUA 
			|| CHAR_getInt(charaindex, CHAR_WHICHTYPE) >= CHAR_TYPELUANPC
#endif
	)	{
		CHAR_setWorkInt( charaindex, CHAR_WORKACTION, -1 );
		if( CHAR_getWorkInt( charaindex, CHAR_WORKBATTLEMODE ) == BATTLE_CHARMODE_NONE ){
			CHAR_sendCharaAtWalk( charaindex, of,ox,oy,CHAR_getDX(dir), CHAR_getDY(dir));
		}
	    {
	        int     opt[2] = { ox, oy};
	        CHAR_sendWatchEvent( CHAR_getWorkInt(charaindex,
	                                             CHAR_WORKOBJINDEX),
	                             CHAR_ACTWALK,opt,2,TRUE );
	    }
    }
    if( retvalue == CHAR_WALKSUCCESSED ) {
			if( CHAR_getWorkInt( charaindex, CHAR_WORKBATTLEMODE )	== BATTLE_CHARMODE_NONE ){
				CHAR_sendCDCharaAtWalk( charaindex, of,ox,oy,
										CHAR_getDX(dir), CHAR_getDY(dir));
			}
		}
    return retvalue;
}

/*------------------------------------------------------------
 * 平乓仿互汹仁［
 * 娄醒
 *  index   int     Char匹及index
 *  dir     int       轾
 *  mode    int     0卅日汹仁［1卅日  轾鳖晶［
 * 忒曰袄
 *  CHAR_WALKRET
 ------------------------------------------------------------*/
CHAR_WALKRET CHAR_walk( int index, int dir, int mode)
{
    CHAR_WALKRET ret;
    BOOL (*prewalk)(int , int* , int* );
    void (*postwalk)(int);
    if( !CHAR_CHECKINDEX( index ) )return CHAR_WALKSYSTEMERROR;

    if( CHAR_getFlg(index,CHAR_ISDIE) ) return CHAR_WALKDIE;

#ifdef _MO_NO_WALK
    if( CHAR_getInt(index,CHAR_NO_WALK) > 0 ) return CHAR_WALKSYSTEMERROR;
#endif
    VALIDATEDIR(dir);
    prewalk = (BOOL(*)(int,int*,int*))
        CHAR_getFunctionPointer(index, CHAR_WALKPREFUNC);

    if( prewalk != NULL )
        if( prewalk(index,&dir,&mode) == FALSE )	{
            return CHAR_WALKPREWALK;
		}

    if( mode == 0 )
        ret = CHAR_walk_move( index, dir );
    else
        ret = CHAR_walk_turn( index, dir );
    postwalk = (void(*)(int))
        CHAR_getFunctionPointer(index,CHAR_WALKPOSTFUNC);

    if( postwalk != NULL )
        postwalk(index);
    return ret;
}

#ifdef _MO_LNS_NLGSUOXU
CHAR_WALKRET CHAR_walk_jjc( int index, int floor, int ox, int oy, int dir)
{
	CHAR_WALKRET ret;
	int mode;
	BOOL (*prewalk)(int , int* , int* );
	void (*postwalk)(int);
	if( !CHAR_CHECKINDEX( index ) )return CHAR_WALKSYSTEMERROR;

	if( CHAR_getFlg(index,CHAR_ISDIE) ) return CHAR_WALKDIE;

	VALIDATEDIR(dir);
	prewalk = (BOOL(*)(int,int*,int*))
		CHAR_getFunctionPointer(index, CHAR_WALKPREFUNC);

	if( prewalk != NULL )
		if( prewalk(index,&dir,&mode) == FALSE )	{
			return CHAR_WALKPREWALK;
		}

		ret = CHAR_walk_jc( index, floor, ox, oy, dir );

		postwalk = (void(*)(int))
			CHAR_getFunctionPointer(index,CHAR_WALKPOSTFUNC);

		if( postwalk != NULL )
			postwalk(index);

		return ret;
}
#endif

/*------------------------------------------------------------
 * 汹井六月蟆赏  ［
 * 娄醒
 *  index       int         平乓仿奶件犯永弁旦
 * 忒曰袄
 *  卅仄
 ------------------------------------------------------------*/
void CHAR_walkcall( int index )
{
    char    tmp[128];
    int     dir,mode;
	int		ret;
	POINT	start,end,end1;

    if( !CHAR_CHECKINDEX( index ) ) return;

    strcpysafe( tmp ,sizeof(tmp),
                CHAR_getWorkChar(index,CHAR_WORKWALKARRAY));

    CHAR_ctodirmode( tmp[0], &dir ,&mode );
	if( CHAR_getInt( index, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER ) {
		if( CHAR_getWorkInt( index, CHAR_WORKPARTYMODE ) == CHAR_PARTY_LEADER ) {
			end.x = CHAR_getInt( index, CHAR_X);
			end.y = CHAR_getInt( index, CHAR_Y);
		}
	}

    ret = CHAR_walk( index , dir ,mode );

	if( mode == 0 && ret == CHAR_WALKSUCCESSED ) {
		/* 愤坌互褪卅日醮棉毛汹井六月 */
		if( CHAR_getInt( index, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER ){
			if( CHAR_getWorkInt( index, CHAR_WORKPARTYMODE) == CHAR_PARTY_LEADER ) {
				int	i;
				for( i = 1; i < getPartyNum(index); i ++ ) {
					int toindex = CHAR_getWorkInt( index, i + CHAR_WORKPARTYINDEX1);
					if( CHAR_CHECKINDEX(toindex) ) {
						int		parent_dir;
						/* 阂及匏  午］褪及汹五蟆及匏  井日  轾毛菲户月 */
						/* 汹仁 */
						start.x = CHAR_getInt( toindex, CHAR_X);
						start.y = CHAR_getInt( toindex, CHAR_Y);
						parent_dir = NPC_Util_getDirFromTwoPoint( &start,&end );
						/* 弘仿犯奴它旦左皿扑亦件汹五毛  蜇允月啃卞］
						 * 戚及阂反蟆及阂及  毛馨丹方丹卞允月
						 */
						end = start;
						if( parent_dir != -1 ) {
							CHAR_walk( toindex, parent_dir, 0);
#ifdef _PETFOLLOW_NEW_
							end1  = start;
							int flg=FALSE;
							int petnum;
							petnum=0;
							for(;petnum<5;petnum++)
							if( CHAR_getWorkInt( toindex, CHAR_WORKPETFOLLOW+petnum) != -1 ) {
								int petindex = CHAR_getWorkInt( toindex, CHAR_WORKPETFOLLOW+petnum);
								if( CHAR_CHECKINDEX(petindex) ) {
									int		parent_dir;
									start.x = CHAR_getInt( petindex, CHAR_X);
									start.y = CHAR_getInt( petindex, CHAR_Y);
									if(!flg){
										flg=TRUE;
										if( end1.x > start.x )    end1.x --;
										else if( end1.x < start.x )	end1.x ++;
										if( end1.y > start.y )    end1.y --;
										else if( end1.y < start.y )	end1.y ++;
									}
									parent_dir = NPC_Util_getDirFromTwoPoint( &start,&end1 );
									end1 = start;
									if( parent_dir != -1 ) {
										if( CHAR_walk( petindex, parent_dir, 0) == CHAR_WALKSUCCESSED )
										{
											CHAR_setWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT,
												1 + CHAR_getWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT) );
											if( CHAR_getWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT) > 1000 ){
												CHAR_setWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT, 0);
												CHAR_pickupFollowPet( toindex, petindex );
											}

										}
									}
								}else {
									CHAR_setWorkInt( toindex, CHAR_WORKPETFOLLOW+petnum, -1);
								}

							}
#else

							// Robin 0702 petFollow
							if( CHAR_getWorkInt( toindex, CHAR_WORKPETFOLLOW) != -1 ) {
									int petindex = CHAR_getWorkInt( toindex, CHAR_WORKPETFOLLOW);
									if( CHAR_CHECKINDEX(petindex) ) {
										int		parent_dir;
										/* 阂及匏  午］褪及汹五蟆及匏  井日  轾毛菲户月 */
										/* 汹仁 */
										end.x = CHAR_getInt( toindex, CHAR_X);
										end.y = CHAR_getInt( toindex, CHAR_Y);
										start.x = CHAR_getInt( petindex, CHAR_X);
										start.y = CHAR_getInt( petindex, CHAR_Y);

						if( end.x > start.x )    end.x --;
						else if( end.x < start.x )	end.x ++;
						if( end.y > start.y )    end.y --;
						else if( end.y < start.y )	end.y ++;

						parent_dir = NPC_Util_getDirFromTwoPoint( &start,&end );
						if( parent_dir != -1 ) {
							if( CHAR_walk( petindex, parent_dir, 0) == CHAR_WALKSUCCESSED )
							{
								CHAR_setWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT,
									1 + CHAR_getWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT) );
								if( CHAR_getWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT) > 1000 ){
									CHAR_setWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT, 0);
									CHAR_pickupFollowPet( toindex, petindex );
								}

							}
						}
					}else {
						CHAR_setWorkInt( toindex, CHAR_WORKPETFOLLOW, -1);
					}

			}
#endif
						}
					}
					else {
						/* 赓渝祭仄卅云允 */
						CHAR_setWorkInt( index, i+CHAR_WORKPARTYINDEX1, -1);
					}
				}
			}
#ifdef _PETFOLLOW_NEW_
			int petnum,petindex;
			int flg=FALSE;
			end.x = CHAR_getInt( index, CHAR_X);
			end.y = CHAR_getInt( index, CHAR_Y);
			for(petnum=0;petnum<5;petnum++){
				petindex = CHAR_getWorkInt( index, CHAR_WORKPETFOLLOW+petnum);
				if(CHAR_CHECKINDEX(petindex)){
					int		parent_dir;
					start.x = CHAR_getInt( petindex, CHAR_X);
					start.y = CHAR_getInt( petindex, CHAR_Y);
					if(!flg){
						flg=TRUE;
						if( end.x > start.x )    end.x --;
						else if( end.x < start.x )	end.x ++;
						if( end.y > start.y )    end.y --;
						else if( end.y < start.y )	end.y ++;
					}
					parent_dir = NPC_Util_getDirFromTwoPoint( &start,&end );
					end = start;
					if( parent_dir != -1 ) {
						if( CHAR_walk( petindex, parent_dir, 0) == CHAR_WALKSUCCESSED ){
							CHAR_setWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT,
								1 + CHAR_getWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT) );
#ifdef _SYSTEM_SPEAD
							if( CHAR_getWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT) > 200 )
#else
							if( CHAR_getWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT) > 1000 )
#endif
							{
								CHAR_setWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT, 0);
								CHAR_pickupFollowPet( index, petindex );
							}
						}
					}
				}else{
					CHAR_setWorkInt( index, CHAR_WORKPETFOLLOW+petnum, -1);
				}
			}
#else
			if( CHAR_getWorkInt( index, CHAR_WORKPETFOLLOW) != -1 ) {
					int petindex = CHAR_getWorkInt( index, CHAR_WORKPETFOLLOW);
					if( CHAR_CHECKINDEX(petindex) ) {
						int		parent_dir;
						/* 阂及匏  午］褪及汹五蟆及匏  井日  轾毛菲户月 */
						/* 汹仁 */
						end.x = CHAR_getInt( index, CHAR_X);
						end.y = CHAR_getInt( index, CHAR_Y);
						start.x = CHAR_getInt( petindex, CHAR_X);
						start.y = CHAR_getInt( petindex, CHAR_Y);

						if( end.x > start.x )    end.x --;
						else if( end.x < start.x )	end.x ++;
						if( end.y > start.y )    end.y --;
						else if( end.y < start.y )	end.y ++;
						parent_dir = NPC_Util_getDirFromTwoPoint( &start,&end );
						if( parent_dir != -1 ) {
							if( CHAR_walk( petindex, parent_dir, 0) == CHAR_WALKSUCCESSED )
							{
								CHAR_setWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT,
									1 + CHAR_getWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT) );
#ifdef _SYSTEM_SPEAD
								if( CHAR_getWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT) > 200 )
#else
								if( CHAR_getWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT) > 1000 )
#endif
								{
									/*
									CHAR_setInt( petindex, CHAR_VARIABLEAI,
										200 + CHAR_getInt( petindex, CHAR_VARIABLEAI) );
									*/
									CHAR_setWorkInt( petindex, CHAR_WORKPETFOLLOWCOUNT, 0);
										CHAR_pickupFollowPet( index, petindex );
									}
								}
							}
						}
						else {
							/* 赓渝祭仄卅云允 */
							CHAR_setWorkInt( index, CHAR_WORKPETFOLLOW, -1);
						}
			}
#endif
		 }
  }

  /* 99/03/09 By Kawata */
  /* walk及瑛绊｝WALKARRAY互弁伉失今木化中月桦宁互丐月及匹
   * 潸曰  仄化戊疋月
   */
  strcpysafe( tmp ,sizeof(tmp),
              CHAR_getWorkChar(index,CHAR_WORKWALKARRAY));
  if( strlen( tmp) > 0 ) {
      CHAR_setWorkChar(index,CHAR_WORKWALKARRAY,&tmp[1]);
  }

  /*  凛棉毛褡户月    */
  CHAR_setWorkInt( index, CHAR_WORKWALKSTARTSEC , NowTime.tv_sec );
  CHAR_setWorkInt( index, CHAR_WORKWALKSTARTMSEC , NowTime.tv_usec );
}



/*------------------------------------------------------------
 * 汹五铵户毛瓒  允月［
 * 娄醒
 *  index           int         平乓仿    匹及奶件犯永弁旦
 *  dir             char*         轾  侬
 *  id              int         ID
 * 忒曰袄
 *  卅仄
 ------------------------------------------------------------*/
void CHAR_walk_start(int index, int x, int y, char* dir, BOOL mapsendmode )
{

    if( !CHAR_CHECKINDEX( index ) )return;

    // Nuke 08/27: Stop wap
    /*
    {
       int ox, oy;
       const int     seesiz = MAP_CHAR_DEFAULTSEESIZ;
       ox = CHAR_getInt(index,CHAR_X);
       oy = CHAR_getInt(index,CHAR_Y);
       if ( ABS(x - ox) > seesiz || ABS(y - oy) > seesiz ) {
          CHAR_talkToCli(index, -1, "因座标错误而断线。", CHAR_COLORYELLOW);
          CONNECT_setCloseRequest_debug(getfdFromCharaIndex(index) , 1);
          return;
       }
    }
    */



    if( CHAR_getFlg( index,CHAR_ISDIE ) )return;
    CHAR_setWorkChar( index,CHAR_WORKWALKARRAY, dir );
    CHAR_setWorkInt( index, CHAR_WORKWALKSTARTSEC , NowTime.tv_sec );
    CHAR_setWorkInt( index, CHAR_WORKWALKSTARTMSEC , NowTime.tv_usec );

    CHAR_setMyPosition( index, x, y, FALSE);
	if( mapsendmode ) {
		int ox,oy;
		char	buf[64];
		int	len;
		int	i;
		strcpysafe( buf, sizeof(buf),
				CHAR_getWorkChar( index, CHAR_WORKWALKARRAY));

		len = strlen( buf );
	    ox = CHAR_getInt(index,CHAR_X);
	    oy = CHAR_getInt(index,CHAR_Y);

		for( i = 0; i < len; i ++ ) {
		    int	dir,mode;
			int	fx,fy;
		    CHAR_ctodirmode( buf[i], &dir ,&mode );
			if( mode == 0 ) {
			    CHAR_getCoordinationDir( dir,ox,oy,1,&fx,&fy);
				ox = fx;
				oy = fy;
			}
		}
#ifdef _MO_MAP_AUTO_UPDATE
		CHAR_sendMapAtWalk(index,
							CHAR_getInt( index, CHAR_FLOOR),
							CHAR_getInt( index, CHAR_X),
							CHAR_getInt( index, CHAR_Y),
							ox,oy);

#endif
	}

    if( strlen( dir ) >= 1 ) CHAR_walkcall( index );
}
/*------------------------------------------------------------
 * 汹五及  赓
 * 娄醒
 *  index           int         平乓仿    匹及奶件犯永弁旦
 *  dir             char*         轾  侬
 *  id              int         ID
 * 忒曰袄
 *  卅仄
 ------------------------------------------------------------*/
void CHAR_walk_init( int fd, int x, int y, char *direction, BOOL mapsendmode)
{
    if( strlen( direction) > 32) {
        print( "walkarray length over\n");
        return;
    }

	if( CHAR_getWorkInt( CONNECT_getCharaindex(fd), CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE){
		return ;
	}
	if( CHAR_getWorkInt( CONNECT_getCharaindex(fd), CHAR_WORKPARTYMODE ) != CHAR_PARTY_CLIENT ){
    	CHAR_walk_start( CONNECT_getCharaindex(fd), x, y, direction, mapsendmode );
	}else {
		int		dir, mode;
	  CHAR_ctodirmode( direction[0], &dir ,&mode );
		if( mode == 0 ) {
			print( "玩家并非领队也非单人玩家。\n");
		}else {
	    	CHAR_walk_start( CONNECT_getCharaindex(fd),
                             x,y,direction, FALSE );
		}
	}
}

void CHAR_sendCharaAtWalk( int charaindex, int of,int ox,int  oy,int xflg, int yflg )
{
	int 	fd = -1;
	int     i,j,fl, x, y;
	char    introduction[512];
	char    c_msg[1024*64];
	int     strpos = 0;
	int     seesiz = CHAR_DEFAULTSEESIZ/2;
	char	cabuf[128];
	char	myintroduction[1024*4];
	int		whichtype;

	whichtype = CHAR_getInt( charaindex, CHAR_WHICHTYPE);
	if( whichtype == CHAR_TYPEPLAYER) {
		fd = getfdFromCharaIndex( charaindex );
		if( fd == -1 )return;
	}
	if( xflg > 1 || xflg < -1 ) {
		print( "%s:%d: arg err[xflg]\n",__FILE__, __LINE__);
		return;
	}
	if( yflg > 1 || yflg < -1 ) {
		print( "%s:%d: arg err[yflg]\n",__FILE__, __LINE__);
		return;
	}
	fl = CHAR_getInt( charaindex, CHAR_FLOOR);
	x = CHAR_getInt( charaindex, CHAR_X);
	y = CHAR_getInt( charaindex, CHAR_Y);

	if( of !=  fl || ABS(x - ox) > seesiz/2 || ABS(y - oy) > seesiz/2 )
		return;

	if( CHAR_getInt( charaindex, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER ) {
		return;
	}
	c_msg[0] = '\0';
#ifndef _NPC_EVENT_NOTICE
	if( !CHAR_makeObjectCString( CHAR_getWorkInt( charaindex, CHAR_WORKOBJINDEX), myintroduction, sizeof( myintroduction))){
		myintroduction[0] = '\0';
	}
#endif
	if( xflg != 0) {
		i = x + seesiz * xflg;
		for( j = y-seesiz ; j <= y+seesiz ;j ++ ) {
			OBJECT  object;
			for( object = MAP_getTopObj(fl,i,j) ; object ; object = NEXT_OBJECT(object ) ){
				int introlen;
				int objindex = GET_OBJINDEX(object);
				int	c_index = OBJECT_getIndex( objindex);

				if( OBJECT_getType(objindex) == OBJTYPE_NOUSE ) continue;
				if( OBJECT_getType(objindex) == OBJTYPE_CHARA && !CHAR_getFlg(OBJECT_getIndex(objindex), CHAR_ISVISIBLE) ){
					continue;
				}
				if( strlen( myintroduction) != 0 ) {
					if( OBJECT_getType( objindex) == OBJTYPE_CHARA ) {
						if( CHAR_getInt( c_index, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER ) {
							int		tofd = getfdFromCharaIndex( c_index );
							if(tofd != -1){
								CDsend( tofd);
#ifdef _NPC_EVENT_NOTICE
										if( !CHAR_makeObjectCStringNew( CHAR_getWorkInt( charaindex, CHAR_WORKOBJINDEX),c_index, myintroduction, sizeof( myintroduction))){
											myintroduction[0] = '\0';
										}
#endif
								lssproto_C_send( tofd, myintroduction);
							}
						}
					}
				}
				if( whichtype == CHAR_TYPEPLAYER) {
					/* Make C*/
#ifdef _NPC_EVENT_NOTICE
					if( CHAR_makeObjectCStringNew( objindex, charaindex,introduction,	sizeof( introduction))){
#else
					if( CHAR_makeObjectCString( objindex, introduction,	sizeof( introduction))){
#endif
						introlen = strlen( introduction);
						introduction[introlen] = ',';
						introduction[introlen+1] = '\0';
						strcpysafe( &c_msg[strpos], sizeof( c_msg) - strpos, introduction);
						strpos += strlen( introduction);
					}
					if( OBJECT_getType(objindex) == OBJTYPE_CHARA ) {
						if( CHAR_getInt( c_index, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER &&
							CHAR_getWorkInt( c_index, CHAR_WORKPARTYMODE ) == CHAR_PARTY_LEADER ){
							if( CHAR_makeCAOPT1String( objindex, cabuf, sizeof( cabuf),	CHAR_ACTLEADER,1 )){
								CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
							}
						}
						if( (CHAR_getInt( c_index, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER
							) && CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE ) == CHAR_PARTY_LEADER &&
							CHAR_getWorkInt( c_index, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE){
							int		tofd = getfdFromCharaIndex( c_index );
							if( tofd != -1 ) {
								if( CHAR_makeCAOPT1String( CHAR_getWorkInt( charaindex, CHAR_WORKOBJINDEX),
															cabuf, sizeof( cabuf), CHAR_ACTLEADER,1 )){
									CONNECT_appendCAbuf( tofd,cabuf,strlen(cabuf));
								}
							}
						}
						if( CHAR_getInt( c_index, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER &&
							CHAR_getWorkInt( c_index, CHAR_WORKBATTLEMODE )	!= BATTLE_CHARMODE_NONE ){
							if( CHAR_getWorkInt( c_index, CHAR_WORKBATTLEWATCH) == TRUE ) {
								if( CHAR_makeCAOPT1String( objindex, cabuf, sizeof( cabuf),
															CHAR_ACTBATTLEWATCH, 1)){
									CONNECT_appendCAbuf( fd, cabuf, strlen( cabuf));
								}
							}else {
								int	battleno = CHAR_getWorkInt( c_index, CHAR_WORKBATTLEINDEX);
								int	sideno   = CHAR_getWorkInt( c_index,CHAR_WORKBATTLESIDE);
								int	helpno;
								if( !BATTLE_CHECKINDEX( battleno) )	{
									//print(" %s[%d]: BATTLE_CHECKINDEX( %d) err!!\n", __FILE__, __LINE__, battleno);
								}else	{
									helpno = (BattleArray[ battleno].Side[ sideno].flg & BSIDE_FLG_HELP_OK) ? TRUE:FALSE;
									/* HelpNo = 1 反移 */
									if( CHAR_makeCAOPT3String( objindex, cabuf, sizeof( cabuf),
																CHAR_ACTBATTLE,	battleno,sideno,helpno )){
										CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
									}
								}
							}
						}
						if( ( CHAR_getInt( c_index, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER ||
							  CHAR_getInt( c_index, CHAR_WHICHTYPE ) == CHAR_TYPEPET )  &&
							CHAR_getWorkInt( c_index, CHAR_WORKACTION ) != -1 ) {
							if( CHAR_makeCADefaultString( objindex, cabuf, sizeof( cabuf),
											CHAR_getWorkInt( c_index, CHAR_WORKACTION))){
								CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
							}
						}
#ifdef _ADD_ACTION
						else if( CHAR_getWorkInt( c_index, CHAR_WORKACTION ) > 0 ) {
							if( CHAR_makeCADefaultString( objindex, cabuf, sizeof( cabuf),
											CHAR_getWorkInt( c_index, CHAR_WORKACTION))){
								CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
							}
						}
#endif
#ifdef _STREET_VENDOR
						if(CHAR_getInt(c_index,CHAR_WHICHTYPE) == CHAR_TYPEPLAYER &&
							 CHAR_getWorkInt(c_index,CHAR_WORKSTREETVENDOR) == 1 &&
							 CHAR_getWorkInt(c_index,CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE){
							if(CHAR_makeCAOPTString(objindex,cabuf,sizeof(cabuf),
								 CHAR_STREETVENDOR_OPEN,CHAR_getWorkChar(c_index,CHAR_STREETVENDOR_NAME))){
								CONNECT_appendCAbuf(fd,cabuf,strlen(cabuf));
							}
						}
#endif

#ifdef _ANGEL_SUMMON
						if( CHAR_getInt( c_index, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER &&
								CHAR_getWorkInt( c_index, CHAR_WORKANGELMODE ) == TRUE ){
							if( CHAR_makeCAOPT1String( objindex, cabuf, sizeof( cabuf),	CHAR_ACTANGEL,1 )){
								CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
							}
						}
#endif
#ifdef _PLAYER_EFFECT
						if( CHAR_getInt( c_index, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER &&
								CHAR_getWorkInt( c_index, CHAR_PLAYER_EFFECT )  > 0 ){
							if( CHAR_makeCAOPT1String( objindex, cabuf, sizeof( cabuf),	CHAR_MIND, CHAR_getWorkInt( c_index, CHAR_PLAYER_EFFECT ))){
								CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
							}
						}
#endif
#ifdef FAMILY_MANOR_
						if( CHAR_getInt( OBJECT_getIndex( objindex), CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER &&
								CHAR_getWorkInt( OBJECT_getIndex( objindex), CHAR_PLAYER_EFFECT_MANOR )  > 0 ){
							if( CHAR_makeCAOPT1String( objindex, cabuf, sizeof( cabuf),	44, CHAR_getWorkInt( OBJECT_getIndex( objindex), CHAR_PLAYER_EFFECT_MANOR ))){
								CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
							}
						}
#endif
#ifdef _TRUMP_EQUIPMENT
						if( CHAR_getInt( OBJECT_getIndex( objindex), CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER &&
								CHAR_getInt( OBJECT_getIndex( objindex), CHAR_TRUMP_EFFECT )  > 0 ){
							if( CHAR_makeCAOPT1String( objindex, cabuf, sizeof( cabuf),	46, CHAR_getInt( OBJECT_getIndex( objindex), CHAR_TRUMP_EFFECT ))){
								CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
							}
						}
#endif
#ifdef _NEW_UPDATETITLE
						if( CHAR_getInt( OBJECT_getIndex( objindex), CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER &&
								CHAR_getInt( OBJECT_getIndex( objindex), CHAR_TITLE_DEFAULT )  > 0 ){
							if( CHAR_makeCAOPT1String( objindex, cabuf, sizeof( cabuf),	45, CHAR_getInt( OBJECT_getIndex( objindex), CHAR_TITLE_DEFAULT ))){
								CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
							}
						}
#endif

#ifdef _FAMILYBADGE_
						extern int getFamilyBadge(int index);
						if( CHAR_getInt( OBJECT_getIndex( objindex), CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER &&
							getFamilyBadge( OBJECT_getIndex( objindex))){
								if( CHAR_makeCAOPT1String( objindex, cabuf, sizeof( cabuf),	CHAR_FAMILYBADGE, getFamilyBadge( OBJECT_getIndex( objindex)))){
									CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
								}
						}
#endif
					}
				}
			}
		}
	}
	if( yflg != 0) {
		j = y + seesiz * yflg;
		for( i = x-seesiz ; i <= x+seesiz  ;i ++ ) {
			OBJECT  object;
			for( object = MAP_getTopObj(fl,i,j) ; object ; object = NEXT_OBJECT(object ) ){
				int introlen;
				int objindex = GET_OBJINDEX(object);
				int	c_index = OBJECT_getIndex( objindex);
				if( OBJECT_getType(objindex) == OBJTYPE_NOUSE ) continue;
				if( OBJECT_getType(objindex) == OBJTYPE_CHARA &&
					!CHAR_getFlg(OBJECT_getIndex(objindex),	CHAR_ISVISIBLE) ){
					if(!CHAR_CHECKINDEX(OBJECT_getIndex(objindex))){
	            printf("自动删除一个问题对象！");
	        		endObjectOne(objindex);
	        }
					continue;
				}
				if( strlen( myintroduction) != 0 ) {
					if( OBJECT_getType( objindex) == OBJTYPE_CHARA ) {
						if( CHAR_getInt( c_index, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER ) {
							int		tofd = getfdFromCharaIndex( c_index );
							if(tofd != -1){
								CDsend( tofd);
								lssproto_C_send( tofd, myintroduction);
							}
						}
					}
				}
				if( whichtype == CHAR_TYPEPLAYER ) {
					/* Make C*/
#ifdef _NPC_EVENT_NOTICE
					if( CHAR_makeObjectCStringNew( objindex, charaindex,introduction,	sizeof( introduction))){
#else
					if( CHAR_makeObjectCString( objindex, introduction,	sizeof( introduction))){
#endif
						introlen = strlen( introduction);
						introduction[introlen] = ',';
						introduction[introlen+1] = '\0';
						strcpysafe( &c_msg[strpos], sizeof( c_msg) - strpos, introduction);
						strpos += strlen( introduction);
					}
					if( OBJECT_getType(objindex) == OBJTYPE_CHARA ) {
						if( CHAR_getInt( c_index, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER &&
							CHAR_getWorkInt( c_index, CHAR_WORKPARTYMODE ) == CHAR_PARTY_LEADER ){
							if( CHAR_makeCAOPT1String( objindex, cabuf, sizeof( cabuf),
														CHAR_ACTLEADER,1 )){
								CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
							}
						}
						if( (CHAR_getInt( c_index, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER
							) && CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE ) == CHAR_PARTY_LEADER &&
							CHAR_getWorkInt( c_index, CHAR_WORKBATTLEMODE)== BATTLE_CHARMODE_NONE){
							int		tofd = getfdFromCharaIndex( c_index );
							if( tofd != -1 ) {
								if( CHAR_makeCAOPT1String( CHAR_getWorkInt( charaindex, CHAR_WORKOBJINDEX),
															cabuf, sizeof( cabuf), CHAR_ACTLEADER,1 )){
									CONNECT_appendCAbuf( tofd,cabuf,strlen(cabuf));
								}
							}
						}
						if( CHAR_getInt( c_index, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER &&
							CHAR_getWorkInt( c_index, CHAR_WORKBATTLEMODE )	!= BATTLE_CHARMODE_NONE ){
							if( CHAR_getWorkInt( c_index, CHAR_WORKBATTLEWATCH) == TRUE ) {
								if( CHAR_makeCAOPT1String( objindex, cabuf,	sizeof( cabuf),
															CHAR_ACTBATTLEWATCH,1)){
									CONNECT_appendCAbuf( fd, cabuf, strlen( cabuf));
								}
							}else {
								int	battleno = CHAR_getWorkInt( c_index, CHAR_WORKBATTLEINDEX);
								int	sideno   = CHAR_getWorkInt( c_index, CHAR_WORKBATTLESIDE);
								int	helpno = (BattleArray[CHAR_getWorkInt( c_index,
										CHAR_WORKBATTLEINDEX)].Side[
										CHAR_getWorkInt( c_index,
										CHAR_WORKBATTLESIDE)].flg & BSIDE_FLG_HELP_OK) ? TRUE:FALSE;

								if( CHAR_makeCAOPT3String( objindex, cabuf, sizeof( cabuf),
															CHAR_ACTBATTLE,	battleno,sideno,helpno	)){
									CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
								}
							}
						}
						if( ( CHAR_getInt( c_index, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER ||
							  CHAR_getInt( c_index, CHAR_WHICHTYPE ) == CHAR_TYPEPET )  &&
							CHAR_getWorkInt( c_index, CHAR_WORKACTION ) != -1 )	{
							if( CHAR_makeCADefaultString( objindex, cabuf, sizeof( cabuf),
											CHAR_getWorkInt( c_index, CHAR_WORKACTION))){
								CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
							}
						}
#ifdef _ADD_ACTION
						else if( CHAR_getWorkInt( c_index, CHAR_WORKACTION ) > 0 ) {
							if( CHAR_makeCADefaultString( objindex, cabuf, sizeof( cabuf),
											CHAR_getWorkInt( c_index, CHAR_WORKACTION))){
								CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
							}
						}
#endif
#ifdef _STREET_VENDOR
						if(CHAR_getInt(c_index,CHAR_WHICHTYPE) == CHAR_TYPEPLAYER &&
							 CHAR_getWorkInt(c_index,CHAR_WORKSTREETVENDOR) == 1 &&
							 CHAR_getWorkInt(c_index,CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE){
							if(CHAR_makeCAOPTString(objindex,cabuf,sizeof(cabuf),
								 CHAR_STREETVENDOR_OPEN,CHAR_getWorkChar(c_index,CHAR_STREETVENDOR_NAME))){
								CONNECT_appendCAbuf(fd,cabuf,strlen(cabuf));
							}
						}
#endif

#ifdef _ANGEL_SUMMON
						if( CHAR_getInt( c_index, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER &&
							CHAR_getWorkInt( c_index, CHAR_WORKANGELMODE ) == TRUE ){
							if( CHAR_makeCAOPT1String( objindex, cabuf, sizeof( cabuf),
														CHAR_ACTANGEL,1 )){
								CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
							}
						}
#endif

#ifdef _PLAYER_EFFECT
						if( CHAR_getInt( c_index, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER &&
								CHAR_getWorkInt( c_index, CHAR_PLAYER_EFFECT )  > 0 ){
							if( CHAR_makeCAOPT1String( objindex, cabuf, sizeof( cabuf),	CHAR_MIND, CHAR_getWorkInt( c_index, CHAR_PLAYER_EFFECT ))){
								CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
							}
						}
#endif

#ifdef FAMILY_MANOR_
						if( CHAR_getInt( OBJECT_getIndex( objindex), CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER &&
								CHAR_getWorkInt( OBJECT_getIndex( objindex), CHAR_PLAYER_EFFECT_MANOR )  > 0 ){
							if( CHAR_makeCAOPT1String( objindex, cabuf, sizeof( cabuf),	44, CHAR_getWorkInt( OBJECT_getIndex( objindex), CHAR_PLAYER_EFFECT_MANOR ))){
								CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
							}
						}
#endif
#ifdef _TRUMP_EQUIPMENT
						if( CHAR_getInt( OBJECT_getIndex( objindex), CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER &&
								CHAR_getInt( OBJECT_getIndex( objindex), CHAR_TRUMP_EFFECT )  > 0 ){
							if( CHAR_makeCAOPT1String( objindex, cabuf, sizeof( cabuf),	46, CHAR_getInt( OBJECT_getIndex( objindex), CHAR_TRUMP_EFFECT ))){
								CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
							}
						}
#endif
#ifdef _NEW_UPDATETITLE
						if( CHAR_getInt( OBJECT_getIndex( objindex), CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER &&
								CHAR_getInt( OBJECT_getIndex( objindex), CHAR_TITLE_DEFAULT )  > 0 ){
							if( CHAR_makeCAOPT1String( objindex, cabuf, sizeof( cabuf),	45, CHAR_getInt( OBJECT_getIndex( objindex), CHAR_TITLE_DEFAULT ))){
								CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
							}
						}
#endif



#ifdef _FAMILYBADGE_
						extern int getFamilyBadge(int index);
						if( CHAR_getInt( OBJECT_getIndex( objindex), CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER &&
							getFamilyBadge(OBJECT_getIndex( objindex))){
								if( CHAR_makeCAOPT1String( objindex, cabuf, sizeof( cabuf),	CHAR_FAMILYBADGE, getFamilyBadge(OBJECT_getIndex( objindex)))){
									CONNECT_appendCAbuf( fd,cabuf,strlen(cabuf));
								}
						}
#endif

					}
				}
			}
		}
	}
	dchop( c_msg, "," );
	if( strlen( c_msg) != 0 ) {
		CDflush( charaindex);
		lssproto_C_send( fd, c_msg);
	}
}

static void CHAR_sendCDCharaAtWalk( int charaindex, int of, int ox, int oy,	 int xflg, int yflg ){
	int     i,j,fl, x, y;
	int     seesiz = CHAR_DEFAULTSEESIZ/2;
	int     fd, tofd;
	if( xflg > 1 || xflg < -1 ) {
		print( "%s:%d: arg err[xflg]\n",__FILE__, __LINE__);
		return;
	}
	if( yflg > 1 || yflg < -1 ) {
		print( "%s:%d: arg err[yflg]\n",__FILE__, __LINE__);
		return;
	}
	fl = CHAR_getInt( charaindex, CHAR_FLOOR);
	x = CHAR_getInt( charaindex, CHAR_X);
	y = CHAR_getInt( charaindex, CHAR_Y);

	if( of !=  fl ||  /*  白夫失互啜丹    */
		ABS(x - ox) > seesiz/2 ||       /*  犒互  五中  */
		ABS(y - oy) > seesiz/2 )        /*  犒互  五中  */
		return;

	fd = getfdFromCharaIndex( charaindex );
	if( xflg != 0) {
		i = x + (seesiz * xflg + xflg)*-1;
		for( j = y-seesiz +(yflg*-1) ; j <= y+seesiz +(yflg*-1) ;j ++ ) {
			OBJECT  object;
			for( object = MAP_getTopObj(fl,i,j) ;
				 object ;
				 object = NEXT_OBJECT(object ) )
			{
				int objindex = GET_OBJINDEX(object);
				char    buf[64];
				if( OBJECT_getType(objindex) == OBJTYPE_NOUSE ) continue;
				if( OBJECT_getType(objindex) == OBJTYPE_CHARA &&
					!CHAR_getFlg(OBJECT_getIndex(objindex),	CHAR_ISVISIBLE) ){
					if(!CHAR_CHECKINDEX(OBJECT_getIndex(objindex))){
	            printf("自动删除一个问题对象！");
	        		endObjectOne(objindex);
	        }
					continue;
				}
				if( fd != -1) {
					cnv10to62( objindex, buf, sizeof( buf));
					CONNECT_appendCDbuf( fd, buf, strlen( buf));
				}
				if( OBJECT_getType( objindex) == OBJTYPE_CHARA) {
					int tocharaindex = OBJECT_getIndex( objindex);
					if( CHAR_getInt( tocharaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER )	{
						if( CHAR_getWorkInt( tocharaindex, CHAR_WORKBATTLEMODE)
							== BATTLE_CHARMODE_NONE ) {
							tofd = getfdFromCharaIndex( tocharaindex );
							if( tofd != -1 ) {
								cnv10to62( CHAR_getWorkInt( charaindex, CHAR_WORKOBJINDEX),
											buf, sizeof( buf));
								CONNECT_appendCDbuf( tofd, buf, strlen( buf));
								CAsend( tofd);
								CDsend( tofd);
							}
						}
					}
				}
			}
		}
	}
	if( yflg != 0) {
		j = y + (seesiz * yflg + yflg)*-1;
		for( i = x-seesiz + (xflg*-1) ; i <= x+seesiz + (xflg*-1) ;i ++ ) {
			OBJECT  object;
			for( object = MAP_getTopObj(fl,i,j) ; object ; object = NEXT_OBJECT(object ) ){
				int objindex = GET_OBJINDEX(object);
				char    buf[64];
				if( OBJECT_getType(objindex) == OBJTYPE_NOUSE ) continue;
				if( OBJECT_getType(objindex) == OBJTYPE_CHARA &&
					!CHAR_getFlg(OBJECT_getIndex(objindex),	CHAR_ISVISIBLE) ){
					continue;
				}
				if( fd != -1 ) {
					cnv10to62( objindex, buf, sizeof( buf));
					CONNECT_appendCDbuf( fd, buf, strlen( buf));
				}
				if( OBJECT_getType( objindex) == OBJTYPE_CHARA) {
					int tocharaindex = OBJECT_getIndex( objindex);
					if( CHAR_getInt( tocharaindex, CHAR_WHICHTYPE)	== CHAR_TYPEPLAYER ){
						if( CHAR_getWorkInt( tocharaindex, CHAR_WORKBATTLEMODE)	== BATTLE_CHARMODE_NONE ){
							tofd = getfdFromCharaIndex( tocharaindex );
							if( tofd != -1 ) {
								cnv10to62( CHAR_getWorkInt( charaindex, CHAR_WORKOBJINDEX),	buf, sizeof( buf));
								CONNECT_appendCDbuf( tofd, buf, strlen( buf));
								CAsend( tofd);
								CDsend( tofd);
							}
						}
					}
				}
			}
		}
	}
	if( fd != -1) {
		CDsend( fd);
	}
}

void CHAR_ctodirmode(char moji , int* dir , int* mode)
{
	*mode = 1;
	if( 'a' <= moji && moji <= 'h' )*mode = 0;

	*dir = tolower(moji) - 'a';
}
#ifdef _MO_LNS_NLGSUOXU
CHAR_WALKRET CHAR_walk_jc( int charaindex, int of, int ox, int oy, int dir )
{
    int     i;
    int     fx,fy,ff;
    int     objbuf[128];
    int     objbufindex=0;
    int     notover=FALSE;
    int     retvalue=CHAR_WALKSUCCESSED;

    if( !CHAR_CHECKINDEX( charaindex )) return CHAR_WALKSYSTEMERROR;

    CHAR_getCoordinationDir( dir,ox,oy,1,&fx,&fy);
    ff = of;

    if( CHAR_getInt( charaindex,CHAR_WHICHTYPE ) == CHAR_TYPEENEMY ){
        RECT    walkr;
        POINT   nextp;
        int npccreateindex;
        npccreateindex = CHAR_getInt( charaindex,CHAR_NPCCREATEINDEX );
        if( CHAR_isInvincibleArea( ff,fx,fy)) {
            CHAR_setInt(charaindex,CHAR_DIR,dir);
            retvalue = CHAR_WALKEXTEND;
            return retvalue;
        }
        if( NPC_isBoundarySet( npccreateindex ) &&
            NPC_createGetRECT( npccreateindex,&walkr ) == TRUE )
        {
            nextp.x = fx;
            nextp.y = fy;
            if( PointInRect( &walkr,&nextp ) == FALSE ){
                CHAR_setInt(charaindex,CHAR_DIR,dir);
                retvalue = CHAR_WALKEXTEND;
            return retvalue;
            }
        }else{
            ;
        }
    }


    if( CHAR_getFlg( charaindex, CHAR_ISBIG ) == 1 ){
        static POINT offset[5]={
            { 0,-1},
            {-1, 0},
            { 0, 0},
            { 1, 0},
            { 0, 1},
        };
        int     k;
        for( k=0 ; k<5; k++ ){
            if( !MAP_walkAble( charaindex, ff, fx+offset[k].x,
                               fy+offset[k].y )){
                CHAR_setInt(charaindex,CHAR_DIR,dir);
                retvalue = CHAR_WALK1357;
            return retvalue;
            }
        }

    }else{
        if( CHAR_getDX(dir)*CHAR_getDY(dir) == 0 ){
            if( !MAP_walkAble( charaindex,ff, fx, fy ) ){
                CHAR_setInt(charaindex,CHAR_DIR,dir);
                retvalue = CHAR_WALK1357;
            return retvalue;
            }
        }else{
            int     xflg,yflg;
            if( !MAP_walkAble( charaindex,ff, fx, fy ) ){
                CHAR_setInt(charaindex,CHAR_DIR,dir);
                retvalue = CHAR_WALK1357;
            return retvalue;
            }

            xflg = MAP_walkAble( charaindex,of, ox+CHAR_getDX(dir), oy );
            yflg = MAP_walkAble( charaindex,of, ox, oy+CHAR_getDY(dir) );

            if( !xflg || !yflg ) {
                CHAR_setInt(charaindex,CHAR_DIR,dir);
                retvalue = CHAR_WALK1357;
            return retvalue;
            }
        }
    }
    objbufindex = CHAR_getSameCoordinateObjects( objbuf, arraysizeof( objbuf ), ff,fx,fy );
    for( i = 0 ; i < objbufindex ; i ++ ){
        int     objindex= objbuf[i];
        switch( OBJECT_getType(objindex) ){
        case OBJTYPE_CHARA:
            if( !CHAR_getFlg( OBJECT_getIndex(objindex),CHAR_ISOVERED) ){
            	if(!CHAR_CHECKINDEX(OBJECT_getIndex(objindex))){
	            	printf("自动删除一个问题对象！");
	        			endObjectOne(objindex);
	        			break;
	        		}
              notover= TRUE;
            }
            break;
        case OBJTYPE_ITEM:
            if( !ITEM_getInt( OBJECT_getIndex(objindex), ITEM_ISOVERED ) )
                notover= TRUE;
            break;
        case OBJTYPE_GOLD:
            break;
        default:
            break;
        }
    }
    if( notover == TRUE ){
        CHAR_setInt(charaindex,CHAR_DIR,dir);
        retvalue = CHAR_WALKHITOBJECT;
            return retvalue;
    }else{
        for( i = 0 ; i < objbufindex ; i ++ ){
            typedef void (*PREOFUNC)(int,int);
            PREOFUNC   pfunc=NULL;
            int     objindex=objbuf[i];
            switch( OBJECT_getType(objindex) ){
            case OBJTYPE_CHARA:
                pfunc = (PREOFUNC)CHAR_getFunctionPointer(
                    OBJECT_getIndex(objindex),
                    CHAR_PREOVERFUNC);
                break;
            case OBJTYPE_ITEM:
                pfunc = (PREOFUNC)ITEM_getFunctionPointer(
                    OBJECT_getIndex(objindex),
                    ITEM_PREOVERFUNC);
                break;
            case OBJTYPE_GOLD:
                break;
            default:
                break;
            }
            if( pfunc )pfunc( OBJECT_getIndex(objindex),charaindex );
        }

        CHAR_setInt(charaindex,CHAR_X,fx);
        CHAR_setInt(charaindex,CHAR_Y,fy);
        CHAR_setInt(charaindex,CHAR_FLOOR,ff);
        CHAR_setInt(charaindex,CHAR_DIR,dir);

        {
            int objindex;
            int ox,oy,of;
            objindex = CHAR_getWorkInt( charaindex,CHAR_WORKOBJINDEX );
            of = OBJECT_setFloor(objindex,
                                 CHAR_getInt(charaindex,CHAR_FLOOR));
            ox = OBJECT_setX(objindex,CHAR_getInt(charaindex,CHAR_X));
            oy = OBJECT_setY(objindex,CHAR_getInt(charaindex,CHAR_Y));
            if( !MAP_objmove( objindex, of,ox,oy,ff,fx,fy ) ){
                /*  仇氏卅氏升丹仄方丹手卅中    */
                fprint( "ERROR MAP_OBJMOVE objindex=%d(%s)\n",objindex,
                	CHAR_getUseName( charaindex ) );
            }
        }

        CHAR_setInt(charaindex,CHAR_WALKCOUNT,
                    CHAR_getInt(charaindex,CHAR_WALKCOUNT) + 1 );


        for( i = 0 ; i < objbufindex ; i ++ ){
            typedef void (*POSTOFUNC)(int,int);
            POSTOFUNC   pfunc=NULL;
            int     objindex=objbuf[i];

            switch( OBJECT_getType(objindex) ){
            case OBJTYPE_CHARA:
                pfunc = (POSTOFUNC)CHAR_getFunctionPointer(
                    OBJECT_getIndex(objindex),
                    CHAR_POSTOVERFUNC);
                break;
            case OBJTYPE_ITEM:
                pfunc = (POSTOFUNC)ITEM_getFunctionPointer( OBJECT_getIndex(objindex), ITEM_POSTOVERFUNC);
                break;
            case OBJTYPE_GOLD:
                break;
            default:
                break;
            }
            if( pfunc )pfunc( OBJECT_getIndex(objindex),charaindex );
        }
        objbufindex = CHAR_getSameCoordinateObjects(objbuf, arraysizeof(objbuf), of, ox, oy );
        for( i=0 ;i<objbufindex;i++){
            typedef void (*OFFFUNC)(int,int);
            OFFFUNC   ofunc=NULL;
            int     objindex = objbuf[i];

            switch( OBJECT_getType( objindex ) ){
            case OBJTYPE_CHARA:
                ofunc = (OFFFUNC)CHAR_getFunctionPointer( OBJECT_getIndex(objindex), CHAR_OFFFUNC);
                break;
            case OBJTYPE_ITEM:
                /*    卞窒手仄卅中  */
                break;
            case OBJTYPE_GOLD:
                /*    卞窒手仄卅中  */
                break;
            default:
                break;
            }
            if( ofunc )ofunc( OBJECT_getIndex(objindex), charaindex );

        }
    }
CHAR_AFTERWALK:
    if( retvalue == CHAR_WALK1357 || retvalue == CHAR_WALKHITOBJECT ){
	    {
	        int     opt[2] = { ox, oy};
	        CHAR_sendWatchEvent( CHAR_getWorkInt(charaindex,
	                                             CHAR_WORKOBJINDEX),
	                             CHAR_ACTWALK,opt,2,TRUE );
	    }
        CHAR_setWorkChar( charaindex, CHAR_WORKWALKARRAY, "");
		if( CHAR_getInt( charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER) {
		    CHAR_sendWatchEvent( CHAR_getWorkInt(charaindex, CHAR_WORKOBJINDEX),
    		                     CHAR_ACTWARP,NULL,0,TRUE);
		}
    }else if( CHAR_getInt( charaindex, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER ){

		BOOL	flg = FALSE;
		int		par;
		int		count;
		CHAR_setWorkInt( charaindex, CHAR_WORKACTION, -1 );
		if( CHAR_getWorkInt( charaindex, CHAR_WORKBATTLEMODE ) == BATTLE_CHARMODE_NONE ){
			CHAR_sendCharaAtWalk( charaindex, of,ox,oy,CHAR_getDX(dir), CHAR_getDY(dir));
		}

	    {
	        int     opt[2] = { ox, oy};
	        CHAR_sendWatchEvent( CHAR_getWorkInt(charaindex,
	                                             CHAR_WORKOBJINDEX),
	                             CHAR_ACTWALK,opt,2,TRUE );
	    }
		if( CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_CLIENT ) {
			CHAR_sendMapAtWalk( charaindex, of,
								ox,oy,
								CHAR_getInt( charaindex, CHAR_X),
								CHAR_getInt( charaindex, CHAR_Y));
		}
		count = CHAR_getWorkInt( charaindex, CHAR_WORK_TOHELOS_COUNT);//什么道具的效力
		if( count > 0 ) {
			CHAR_setWorkInt( charaindex, CHAR_WORK_TOHELOS_COUNT, count -1);
			if( count -1 == 0 ) {
				//CHAR_talkToCli( charaindex, -1, "道具的效力已到。", CHAR_COLORWHITE);
			}
		}
		par = ENCOUNT_getEncountPercentMin( charaindex, of,ox,oy);
		if( par != -1 ) {
			if( CHAR_getWorkInt( charaindex, CHAR_WORKENCOUNTPROBABILITY_MIN) != par ){
				flg = TRUE;
				CHAR_setWorkInt( charaindex, CHAR_WORKENCOUNTPROBABILITY_MIN, par);
			}
		}

		par = ENCOUNT_getEncountPercentMax( charaindex, of,ox,oy);
		if( par != -1 ) {
			if( CHAR_getWorkInt( charaindex, CHAR_WORKENCOUNTPROBABILITY_MAX) != par ){
				flg = TRUE;
				CHAR_setWorkInt( charaindex, CHAR_WORKENCOUNTPROBABILITY_MAX, par);
			}
		}
		// Arminius 7.12 login announce
		/*
		{
		  int enfd = getfdFromCharaIndex( charaindex );
		  if (CONNECT_get_announced(enfd)==0) {
		    // Robin 0720
		    //AnnounceToPlayer(charaindex);
		    AnnounceToPlayerWN( enfd );
		    CONNECT_set_announced(enfd,1);
		  }
		}
		*/
		// Arminius 6.22 check Encounter
		// Nuke 0622: Provide No Enemy function
		{
		  int enfd = getfdFromCharaIndex( charaindex );
		  int eqen = getEqNoenemy( enfd );	// Arminius 7.2: Ra's amulet
		  int noen = getNoenemy(enfd);

          // Arminius 7.31 cursed stone
      if (getStayEncount(enfd)>0) {
			  clearStayEncount(enfd);
		  }
		  //print("\n enfd=%d,eqen=%d,noen=%d", enfd, eqen, noen);

		  // Arminius 7.2 Ra's amulet
		  if (eqen>=200) {
		    noen=1;
		  } else if (eqen>=120) {
		    if ((ff==100)||(ff==200)||(ff==300)||(ff==400)||(ff==500)) noen=1;
		  } else if (eqen>=80) {
		    if ((ff==100)||(ff==200)||(ff==300)||(ff==400)) noen=1;
		  } else if (eqen>=40) {
		    if ((ff==100)||(ff==200)) noen=1;
		  }


		  //print("\n noen=%d", noen);
		  if (noen==0) {
				int maxep = CHAR_getWorkInt(charaindex, CHAR_WORKENCOUNTPROBABILITY_MAX);
		    int minep = CHAR_getWorkInt(charaindex, CHAR_WORKENCOUNTPROBABILITY_MIN);
		    int cep = CONNECT_get_CEP(enfd);

#ifdef _PROFESSION_SKILL			// WON ADD 人物职业技能
			int temp=0;
			int p_cep = CHAR_getWorkInt(charaindex, CHAR_ENCOUNT_FIX);
			if( p_cep != 0 ){
				if( CHAR_getWorkInt( charaindex, CHAR_ENCOUNT_NUM) < (int)time(NULL) ){
					CHAR_talkToCli( charaindex, -1, "技能效用结束。", CHAR_COLORYELLOW);
					CHAR_setWorkInt( charaindex, CHAR_ENCOUNT_FIX, 0);
					CHAR_setWorkInt( charaindex, CHAR_ENCOUNT_NUM, 0);
				}
				temp = cep * (100 + p_cep ) / 100;
			}else{
				temp = cep;
			}
#endif

		    if (cep<minep) cep=minep;
		    if (cep>maxep) cep=maxep;
		    if (CHAR_getWorkInt(charaindex,CHAR_WORKBATTLEMODE)==BATTLE_CHARMODE_NONE) {
				int entflag=1;
				{
					int objindex,index;
					OBJECT obj;
					for ( obj = MAP_getTopObj( ff, fx, fy); obj; obj = NEXT_OBJECT( obj)) {
						objindex = GET_OBJINDEX( obj);
						if (OBJECT_getType( objindex) == OBJTYPE_CHARA) {
							int etype;
							index = OBJECT_getIndex( objindex);
							if (!CHAR_CHECKINDEX( index)) continue;
							if( CHAR_getInt( index, CHAR_WHICHTYPE) == CHAR_TYPENPCENEMY &&
								CHAR_getWorkInt( index, CHAR_WORKEVENTTYPE) == CHAR_EVENT_ENEMY )	{
								CHAR_setInt( charaindex, CHAR_X, ox);
								CHAR_setInt( charaindex, CHAR_Y, oy);
								lssproto_XYD_send( getfdFromCharaIndex(charaindex),
								CHAR_getInt( charaindex, CHAR_X ),
								CHAR_getInt( charaindex, CHAR_Y ),
								CHAR_getInt( charaindex, CHAR_DIR ) );
								break;
							}
							etype = CHAR_getWorkInt( index, CHAR_WORKEVENTTYPE);
							if (etype!=CHAR_EVENT_NONE) {
								if (etype==CHAR_EVENT_WARP) entflag=0;
							}
						}
					}
				}


#ifdef _PROFESSION_SKILL			// WON ADD 人物职业技能
#ifdef _ENEMY_ACTION
				if( rand()%(120*getEnemyAction()) < temp ){
#else
			  if( rand()%120 < temp ){
#endif
#else
#ifdef _ENEMY_ACTION
				if( rand()%(120*getEnemyAction()) < cep ){
#else
		      if (rand()%120<cep){	// Arminius 6.28 lower encounter prob.
#endif
#endif
					if (entflag) {
#ifdef _Item_MoonAct
						if( getEqRandenemy( enfd) > 0 )	{
							int Rnum=0;
							int RandEnemy = getEqRandenemy( enfd);
							Rnum = RAND(0,100);
							if( Rnum > RandEnemy )	{
#endif
								//encounter!!
								cep = minep;
								lssproto_EN_recv(enfd,
								CHAR_getInt(charaindex,CHAR_X),
								CHAR_getInt(charaindex,CHAR_Y));
#ifdef _Item_MoonAct
							}
						}else{
								cep = minep;
								lssproto_EN_recv(enfd,
								CHAR_getInt(charaindex,CHAR_X),
								CHAR_getInt(charaindex,CHAR_Y));
						}
#endif
					}
		      } else {
		        if (cep<maxep) cep++;
		      }
		    }
		    CONNECT_set_CEP(enfd, cep);
		  }
		}
		if( CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE )!= CHAR_PARTY_CLIENT ){
			CHAR_setFlg( charaindex, CHAR_ISWARP, 0);
		}
    }
    else if( CHAR_getInt( charaindex, CHAR_WHICHTYPE ) == CHAR_TYPEPET ||
    		CHAR_getInt( charaindex, CHAR_WHICHTYPE) == CHAR_TYPEBUS
#ifdef _GAMBLE_ROULETTE
			|| CHAR_getInt( charaindex, CHAR_WHICHTYPE) == CHAR_GAMBLEROULETTE
#endif
#ifdef _PETRACE
			|| CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_PETRACEPET
#endif
	)	{
		CHAR_setWorkInt( charaindex, CHAR_WORKACTION, -1 );
		if( CHAR_getWorkInt( charaindex, CHAR_WORKBATTLEMODE ) == BATTLE_CHARMODE_NONE ){
			CHAR_sendCharaAtWalk( charaindex, of,ox,oy,CHAR_getDX(dir), CHAR_getDY(dir));
		}
	    {
	        int     opt[2] = { ox, oy};
	        CHAR_sendWatchEvent( CHAR_getWorkInt(charaindex,
	                                             CHAR_WORKOBJINDEX),
	                             CHAR_ACTWALK,opt,2,TRUE );
	    }
    }
		  if( retvalue == CHAR_WALKSUCCESSED ) {
					if( CHAR_getWorkInt( charaindex, CHAR_WORKBATTLEMODE )	== BATTLE_CHARMODE_NONE ){
						CHAR_sendCDCharaAtWalk( charaindex, of,ox,oy,
												CHAR_getDX(dir), CHAR_getDY(dir));
					}
				}
    return retvalue;
}
#endif

