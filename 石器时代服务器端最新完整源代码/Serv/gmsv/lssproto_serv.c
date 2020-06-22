#include "version.h"
#include "autil.h"
#include "lssproto_serv.h"
#include "common.h"	// for StoneAge
#include "log.h"	// for StoneAge
#include "char.h"
#include "battle.h"
#include "descrypt.h"
#include "configfile.h"
#include <stdio.h>
#include <time.h>
#include <net.h>
#include <zutil.h>
#include "shop.h"
#include "npc_lua_interface.h"
#ifdef _ABSOLUTE_DEBUG
extern char charId[32];
extern char errordata[256];
extern int lastfunctime;
#endif

void lssproto_SetServerLogFiles( char *r , char *w )
{
    lssproto_strcpysafe( lssproto_writelogfilename , w , sizeof(lssproto_writelogfilename ));
    lssproto_strcpysafe( lssproto_readlogfilename , r , sizeof(lssproto_readlogfilename ));
}
int lssproto_InitServer(	int (*writefunc)(int,char*,int) ,	int worksiz )
{
    int i;
    if( (void*)writefunc == NULL) {
        lssproto.write_func = lssproto_default_write_wrap;
    } else {
        lssproto.write_func = writefunc;
    }
    lssproto_AllocateCommonWork(worksiz);
    lssproto_stringwrapper = (char**)calloc( 1,sizeof(char*) * MAXLSRPCARGS);
    if(lssproto_stringwrapper ==NULL)
        return -1;
    memset( lssproto_stringwrapper , 0, sizeof(char*)*MAXLSRPCARGS);
    for(i=0;i<MAXLSRPCARGS;i++) {
        lssproto_stringwrapper[i] = (char*)calloc( 1, worksiz );
        if( lssproto_stringwrapper[i] == NULL) {
            for(i=0;i<MAXLSRPCARGS;i++) {
                free( lssproto_stringwrapper[i]);
                return -1;
            }
        }
    }
    lssproto.ret_work = (char*)calloc( 1,sizeof( worksiz ));
    if( lssproto.ret_work == NULL ) {
        return -1;
    }
    return 0;
}

void lssproto_CleanupServer( void )
{
    int i;
    free( lssproto.work );
    free( lssproto.arraywork);
    free( lssproto.escapework );
    free( lssproto.val_str);
    free( lssproto.token_list );
    for(i=0;i<MAXLSRPCARGS;i++) {
        free( lssproto_stringwrapper[i]);
    }
    free( lssproto_stringwrapper );
    free( lssproto.ret_work );
}

#define DME() print("<DME(%d)%d:%d>",fd,__LINE__,func)
extern int cliretfunc;
int lssproto_ServerDispatchMessage(int fd, char *encoded)
{
#ifdef _DEFEND_BIGBAO
	if(CONNECT_getState(fd) == NOTLOGIN){
		if (strlen(encoded)>getBigBao()){
			close(fd);
			return -1;
		}
	}else{
		if(strlen(encoded)>getBigBao2()){
			 close(fd);
			return -1;
		}
	}
#endif

	int func,fieldcount;
	char raw[1024 * 64];
#ifdef _SERVER_DEF
    util_DiscardMessage();
#endif
	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);
#ifdef _NEW_FUNC_DECRYPT
	if(!util_DecodeMessageTea(raw,encoded)) {
		return -2;
	}
#else
	util_DecodeMessage(raw,encoded);
#endif
#ifdef _DEBUG_RET	
	print("\nraw=%s\n",raw);
#endif
	if( !util_SplitMessage(raw,SEPARATOR) ){
		//print("\nDME1:package=%s\n",raw);
		DME(); return -2;
	}
	if (!util_GetFunctionFromSlice(&func, &fieldcount)) {
		logHack(fd,HACK_GETFUNCFAIL);
		DME(); return -1;
	}

#ifdef _MO_SHOW_DEBUG
	if (isDebug()==1) {
		printf("\n客户端接口=%d\n",func);
	}
	if (func<LSSPROTO_W_RECV) {
		if (isDebug()==1) {
			printf("\n客户端接口异常，已屏蔽！\n");
		}
		return -1;
	}
#endif

#ifdef _SERVER_DEF
	/*
if (func != 71 && func != 79 && func != 87 && func != 77 && func != 81 && func != 73 && func != 75 && func != 203)//登录前封包 全部可执行，其他封包检查是否已经有玩家在线！
		{
			if( !CHAR_CHECKINDEX( CONNECT_getCharaindex(fd) ) )//并没有登入人物，仅仅是一个连接！
			{
        close(fd);//断开玩家之间的连接
				return -1;//你要做的事!
			}
		}
		*/
#endif
	cliretfunc=func;
#ifdef _ABSOLUTE_DEBUG
	CONNECT_getCdkey( fd, charId, sizeof(charId));
	lastfunctime=3;
#endif
	
#ifdef _DEBUG_RET	
	printf("\n客户端接口=%d\n",func);
#endif
	if (func==LSSPROTO_W_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		char direction[1024 * 64];

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_destring(4, direction);
		util_deint(5, &checksumrecv);
		
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_W_RECV-x:%d,y:%d,direction:%s\n", x, y, direction);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%s", x, y, direction);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_W_recv(fd, x, y, direction);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_W2_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		char direction[1024 * 64];

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_destring(4, direction);
		util_deint(5, &checksumrecv);

		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}

#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_W2_RECV-x:%d,y:%d,direction:%s\n", x, y, direction);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%s", x, y, direction);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_W2_recv(fd, x, y, direction);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_EV_RECV) {
		int checksum=0, checksumrecv;
		int event;
		int seqno;
		int x;
		int y;
		int dir;

		checksum += util_deint(2, &event);
		checksum += util_deint(3, &seqno);
		checksum += util_deint(4, &x);
		checksum += util_deint(5, &y);
		checksum += util_deint(6, &dir);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_EV_RECV-event:%d,seqno:%d,x:%d,y:%d,dir:%d\n", event, seqno, x, y, dir);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d:%d:%d", event, seqno, x, y, dir);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_EV_recv(fd, event, seqno, x, y, dir);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DU_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_DU_RECV-x:%d,y:%d\n", x, y);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d", x, y);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_DU_recv(fd, x, y);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_EO_RECV) {
		int checksum=0, checksumrecv;
		int dummy;

		checksum += util_deint(2, &dummy);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_EO_RECV-dummy:%d\n", dummy);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d", dummy);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_EO_recv(fd, dummy);
		util_DiscardMessage();
		return 0;
	}
     
	if (func==LSSPROTO_BU_RECV) {
		int checksum=0, checksumrecv;
		int dummy;

		checksum += util_deint(2, &dummy);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_BU_RECV-dummy:%d\n", dummy);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d", dummy);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_BU_recv(fd, dummy);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_JB_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_JB_RECV-x:%d,y:%d\n", x, y);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d", x, y);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_JB_recv(fd, x, y);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_LB_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_LB_RECV-x:%d,y:%d\n", x, y);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d", x, y);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_LB_recv(fd, x, y);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_B_RECV) {
		int checksum=0, checksumrecv;
		char command[1024 * 64];

		checksum += util_destring(2, command);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_B_RECV-command:%s\n", command);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%s", command);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_B_recv(fd, command);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_SKD_RECV) {
		int checksum=0, checksumrecv;
		int dir;
		int index;

		checksum += util_deint(2, &dir);
		checksum += util_deint(3, &index);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_SKD_RECV-dir:%d,index:%d\n", dir, index);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d", dir, index);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_SKD_recv(fd, dir, index);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_ID_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int haveitemindex;
		int toindex;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &haveitemindex);
		checksum += util_deint(5, &toindex);
		util_deint(6, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_ID_RECV-x:%d,y:%d,haveitemindex:%d,toindex:%d\n", x, y, haveitemindex, toindex);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d:%d", x, y, haveitemindex, toindex);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_ID_recv(fd, x, y, haveitemindex, toindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PI_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int dir;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &dir);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_PI_RECV-x:%d,y:%d,dir:%d\n", x, y, dir);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d", x, y, dir);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_PI_recv(fd, x, y, dir);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DI_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int itemindex;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &itemindex);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_DI_RECV-x:%d,y:%d,itemindex:%d\n", x, y, itemindex);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d", x, y, itemindex);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_DI_recv(fd, x, y, itemindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DG_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int amount;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &amount);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_DG_RECV-x:%d,y:%d,amount:%d\n", x, y, amount);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d", x, y, amount);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_DG_recv(fd, x, y, amount);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DP_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int petindex;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &petindex);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_DP_RECV-x:%d,y:%d,petindex:%d\n", x, y, petindex);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d", x, y, petindex);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_DP_recv(fd, x, y, petindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_MI_RECV) {
		int checksum=0, checksumrecv;
		int fromindex;
		int toindex;

		checksum += util_deint(2, &fromindex);
		checksum += util_deint(3, &toindex);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_MI_RECV-fromindex:%d,toindex:%d\n", fromindex, toindex);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d", fromindex, toindex);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_MI_recv(fd, fromindex, toindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_MSG_RECV) {
		int checksum=0, checksumrecv;
		int index;
		char message[1024 * 64];
		int color;

		checksum += util_deint(2, &index);
		checksum += util_destring(3, message);
		checksum += util_deint(4, &color);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_MSG_RECV-index:%d,message:%s,color:%d\n", index, message, color);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%s:%d", index, message, color);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_MSG_recv(fd, index, message, color);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PMSG_RECV) {
		int checksum=0, checksumrecv;
		int index;
		int petindex;
		int itemindex;
		char message[1024 * 64];
		int color;

		checksum += util_deint(2, &index);
		checksum += util_deint(3, &petindex);
		checksum += util_deint(4, &itemindex);
		checksum += util_destring(5, message);
		checksum += util_deint(6, &color);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_PMSG_RECV-index:%d,petindex:%d,itemindex:%d,message:%s,color:%d\n", index, petindex, itemindex, message, color);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d:%s:%d", index, petindex, itemindex, message, color);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_PMSG_recv(fd, index, petindex, itemindex, message, color);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_AB_RECV) {
		int checksum=0, checksumrecv;
		util_deint(2, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_AB_RECV\n");
#endif
#ifdef _ABSOLUTE_DEBUG
		strcpy(errordata, "");
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_AB_recv(fd);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DAB_RECV) {
		int checksum=0, checksumrecv;
		int index;

		checksum += util_deint(2, &index);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_DAB_RECV-index:%d\n", index);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d", index);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_DAB_recv(fd, index);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_AAB_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_AAB_RECV-x:%d,y:%d\n", x, y);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d", x, y);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_AAB_recv(fd, x, y);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_L_RECV) {
		int checksum=0, checksumrecv;
		int dir;

		checksum += util_deint(2, &dir);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_L_RECV-dir:%d\n", dir);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d", dir);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_L_recv(fd, dir);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_TK_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		char message[1024 * 64];
		int color;
		int area;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_destring(4, message);
		checksum += util_deint(5, &color);
		checksum += util_deint(6, &area);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_TK_RECV-x:%d,y:%d,message:%s,color:%d,area:%d\n", x, y, message, color, area);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%s:%d:%d", x, y, message, color, area);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_TK_recv(fd, x, y, message, color, area);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_M_RECV) {
		int checksum=0, checksumrecv;
		int fl;
		int x1;
		int y1;
		int x2;
		int y2;

		checksum += util_deint(2, &fl);
		checksum += util_deint(3, &x1);
		checksum += util_deint(4, &y1);
		checksum += util_deint(5, &x2);
		checksum += util_deint(6, &y2);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_M_RECV-fl:%d,x1:%d,y1:%d,x2:%d,y2:%d\n", fl, x1, y1, x2, y2);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d:%d:%d", fl, x1, y1, x2, y2);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_M_recv(fd, fl, x1, y1, x2, y2);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_C_RECV) {
		int checksum=0, checksumrecv;
		int index;

		checksum += util_deint(2, &index);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_C_RECV-index:%d\n", index);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d", index);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_C_recv(fd, index);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_S_RECV) {
		int checksum=0, checksumrecv;
		char category[1024 * 64];

		checksum += util_destring(2, category);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_S_RECV-category:%s\n", category);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%s", category);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_S_recv(fd, category);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_FS_RECV) {
		int checksum=0, checksumrecv;
		int flg;

		checksum += util_deint(2, &flg);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_FS_RECV-flg:%d\n", flg);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d", flg);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_FS_recv(fd, flg);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_HL_RECV) {
		int checksum=0, checksumrecv;
		int flg;

		checksum += util_deint(2, &flg);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_HL_RECV-flg:%d\n", flg);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d", flg);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_HL_recv(fd, flg);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PR_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int request;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &request);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_PR_RECV-x:%d,y:%d,request:%d\n", x, y, request);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d", x, y, request);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_PR_recv(fd, x, y, request);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_KS_RECV) {
		int checksum=0, checksumrecv;
		int petarray;
		checksum += util_deint(2, &petarray);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_KS_RECV-petarray:%d\n", petarray);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d", petarray);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_KS_recv(fd, petarray);
		util_DiscardMessage();
		return 0;
	}
#ifdef _STANDBYPET
	if (func==LSSPROTO_SPET_RECV) {
		int checksum=0, checksumrecv;
		int standbypet;
		checksum += util_deint(2, &standbypet);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_SPET_RECV-standbypet:%d\n", standbypet);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d", standbypet);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_SPET_recv(fd, standbypet);
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _RIGHTCLICK
	if (func==LSSPROTO_RCLICK_RECV) {
		int checksum=0, checksumrecv;
		int type;
		char data[1024];
		checksum += util_deint(2, &type);
		checksum += util_destring(3, data);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_RCLICK_RECV-type:%d,data:%s\n", type, data);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%s", type, data);
#endif
		lssproto_RCLICK_recv(fd, type, data);
		util_DiscardMessage();
		return 0;
	}
#endif

	if (func==LSSPROTO_AC_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int actionno;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &actionno);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_AC_RECV-x:%d,y:%d,actionno:%d\n", x, y, actionno);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d", x, y, actionno);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_AC_recv(fd, x, y, actionno);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_MU_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int array;
		int toindex;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &array);
		checksum += util_deint(5, &toindex);
		util_deint(6, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_MU_RECV-x:%d,y:%d,array:%d,toindex:%d\n", x, y, array, toindex);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d:%d", x, y, array, toindex);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_MU_recv(fd, x, y, array, toindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PS_RECV) {
		int checksum=0, checksumrecv;
		int havepetindex;
		int havepetskill;
		int toindex;
		char data[1024 * 64];
		
		checksum += util_deint(2, &havepetindex);
		checksum += util_deint(3, &havepetskill);
		checksum += util_deint(4, &toindex);
		checksum += util_destring(5, data);
		util_deint(6, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);

			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_PS_RECV-havepetindex:%d,havepetskill:%d,toindex:%d,data:%s\n", havepetindex, havepetskill, toindex, data);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d:%s", havepetindex, havepetskill, toindex, data);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_PS_recv(fd, havepetindex, havepetskill, toindex, data);
		
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_ST_RECV) {
		int checksum=0, checksumrecv;
		int titleindex;

		checksum += util_deint(2, &titleindex);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_ST_RECV-titleindex:%d\n", titleindex);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d", titleindex);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_ST_recv(fd, titleindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DT_RECV) {
		int checksum=0, checksumrecv;
		int titleindex;

		checksum += util_deint(2, &titleindex);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_DT_RECV-titleindex:%d\n", titleindex);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d", titleindex);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_DT_recv(fd, titleindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_FT_RECV) {
		int checksum=0, checksumrecv;
		char data[1024 * 64];

		checksum += util_destring(2, data);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_FT_RECV-data:%s\n", data);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%s", data);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_FT_recv(fd, data);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_SKUP_RECV) {
		int checksum=0, checksumrecv;
		int skillid;

		checksum += util_deint(2, &skillid);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_SKUP_RECV-skillid:%d\n", skillid);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d", skillid);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_SKUP_recv(fd, skillid);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_KN_RECV) {
		int checksum=0, checksumrecv;
		int havepetindex;
		char data[1024 * 64];

		checksum += util_deint(2, &havepetindex);
		checksum += util_destring(3, data);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_KN_RECV-havepetindex:%d,data:%s\n", havepetindex, data);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%s", havepetindex, data);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_KN_recv(fd, havepetindex, data);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_WN_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int seqno;
		int objindex;
		int select;
		char data[1024 * 64];

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &seqno);
		checksum += util_deint(5, &objindex);
		checksum += util_deint(6, &select);
		checksum += util_destring(7, data);
		
		util_deint(8, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_WN_RECV-x:%d,y:%d,seqno:%d,objindex:%d,select:%d,data:%s\n", x, y, seqno, objindex, select, data);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d:%d:%d:%s", x, y, seqno, objindex, select, data);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_WN_recv(fd, x, y, seqno, objindex, select, data);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_SP_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int dir;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &dir);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_SP_RECV-x:%d,y:%d,dir:%d\n", x, y, dir);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d", x, y, dir);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_SP_recv(fd, x, y, dir);
		util_DiscardMessage();
		return 0;
	}
	if (func==LSSPROTO_CLIENTLOGIN_RECV) {
		int checksum=0, checksumrecv;
		char cdkey[CDKEYLEN];
		char passwd[PASSWDLEN];
		char mac[512] = "";
		char ip[128];
		int servid = 0;
		strcpy(PersonalKey, _DEFAULT_PKEY);
		checksum += util_destring(2, cdkey);
		checksum += util_destring(3, passwd);
		util_deint(4, &checksumrecv);
		if (checksum==checksumrecv) {
#ifdef _DEBUG_RET_CLI
			printf("[接收]LSSPROTO_CLIENTLOGIN_RECV-cdkey:%s,passwd:%s\n", cdkey, passwd);
#endif
#ifdef _ABSOLUTE_DEBUG
			sprintf(errordata, "%s:%s", cdkey, passwd);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		}else{
			checksum += util_destring(4, mac);
			util_deint(5, &checksumrecv);
			if (checksum==checksumrecv) {
#ifdef _DEBUG_RET_CLI
			printf("[接收]LSSPROTO_CLIENTLOGIN_RECV-cdkey:%s,passwd:%s,mac:%s\n", cdkey, passwd, mac);
#endif
#ifdef _ABSOLUTE_DEBUG
			sprintf(errordata, "%s:%s:%s", cdkey, passwd, mac);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
			}else{
				checksum += util_deint(5, &servid);
				checksum += util_destring(6, ip);
				util_deint(7, &checksumrecv);
				if (checksum!=checksumrecv) {
					util_DiscardMessage();
					logHack(fd, HACK_CHECKSUMERROR);
					DME(); return -1;
				}
#ifdef _DEBUG_RET_CLI
				printf("[接收]LSSPROTO_CLIENTLOGIN_RECV-cdkey:%s,passwd:%s,mac:%s,servid:%d\n", cdkey, passwd, mac, servid);
#endif
#ifdef _ABSOLUTE_DEBUG
				sprintf(errordata, "%s:%s:%s:%d", cdkey, passwd, mac, servid);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
			}
		}
		
		if(strlen(cdkey) == 0 
			|| strlen(passwd) == 0){
			util_DiscardMessage();
			return -1;
		}

		lssproto_ClientLogin_recv(fd, cdkey, passwd, mac, servid,ip);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CREATENEWCHAR_RECV) {
		int checksum=0, checksumrecv;
		int dataplacenum;
		char charname[CHARNAMELEN];;
		int imgno;
		int faceimgno;
		int vital;
		int str;
		int tgh;
		int dex;
		int earth;
		int water;
		int fire;
		int wind;
		int hometown;

		checksum += util_deint(2, &dataplacenum);
		checksum += util_destring(3, charname);
		checksum += util_deint(4, &imgno);
		checksum += util_deint(5, &faceimgno);
		checksum += util_deint(6, &vital);
		checksum += util_deint(7, &str);
		checksum += util_deint(8, &tgh);
		checksum += util_deint(9, &dex);
		checksum += util_deint(10, &earth);
		checksum += util_deint(11, &water);
		checksum += util_deint(12, &fire);
		checksum += util_deint(13, &wind);
		checksum += util_deint(14, &hometown);
		util_deint(15, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_CREATENEWCHAR_RECV-dataplacenum:%d,charname:%s,imgno:%d,faceimgno:%d,vital:%d,str:%d,"
						"tgh:%d,dex:%d,earth:%d,water:%d,fire:%d,wind:%d,hometown:%d\n",
						dataplacenum, charname, imgno, faceimgno, vital, str, tgh, dex, earth, water, fire, wind, hometown);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%s:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d", dataplacenum, charname, imgno, faceimgno, vital, str, tgh, dex, earth, water, fire, wind, hometown);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_CreateNewChar_recv(fd, dataplacenum, charname, imgno, faceimgno, vital, str, tgh, dex, earth, water, fire, wind, hometown);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CHARDELETE_RECV) {
		int checksum=0, checksumrecv;
		char charname[CHARNAMELEN];
		char passwd[PASSWDLEN];

		checksum += util_destring(2, charname);
		checksum += util_destring(3, passwd);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_CHARDELETE_RECV-charname:%s\n", charname);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%s", charname);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_CharDelete_recv(fd, charname,passwd);
		util_DiscardMessage();
		return 0;
	}

	if (func == LSSPROTO_UPSHOP_RECV) {
		int checksumrecv;
		int checksum = 0;
		char shop[5][1024]={0};
		
		checksum += util_destring(2,shop[0]);
		checksum += util_destring(3,shop[1]);
		checksum += util_destring(4,shop[2]);
		checksum += util_destring(5,shop[3]);
		checksum += util_destring(6,shop[4]);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			return -1;
		}
		lssproto_upshopdata_recv(fd,shop[0]);
		util_DiscardMessage();
		return 0;
	}
	if (func==LSSPROTO_CHARLOGIN_RECV) {
		int checksum=0, checksumrecv;
		char charname[CHARNAMELEN];;

		checksum += util_destring(2, charname);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_CHARLOGIN_RECV-charname:%s\n", charname);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%s", charname);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_CharLogin_recv(fd, charname);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CHARLIST_RECV) {
		int checksum=0, checksumrecv;
		util_deint(2, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_CHARLIST_RECV\n");
#endif
#ifdef _ABSOLUTE_DEBUG
		strcpy(errordata, "");
#endif

		lssproto_CharList_recv( fd);

		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CHARLOGOUT_RECV) {
		int checksum=0, checksumrecv;
		int Flg=1;
#ifdef _CHAR_NEWLOGOUT
		checksum += util_deint(2, &Flg);
		util_deint(3, &checksumrecv);
#else
		util_deint(2, &checksumrecv);
#endif
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_CHARLOGOUT_RECV-Flg:%d\n", Flg);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d", Flg);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_CharLogout_recv(fd, Flg);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PROCGET_RECV) {
		int checksum=0, checksumrecv;
		
		strcpy( PersonalKey, _DEFAULT_PKEY);

		util_deint(2, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_PROCGET_RECV\n");
#endif
#ifdef _ABSOLUTE_DEBUG
		strcpy(errordata, "");
#endif
		lssproto_ProcGet_recv(fd);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PLAYERNUMGET_RECV) {
		int checksum=0, checksumrecv;
		util_deint(2, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_PLAYERNUMGET_RECV\n");
#endif
#ifdef _ABSOLUTE_DEBUG
		strcpy(errordata, "");
#endif
		lssproto_PlayerNumGet_recv(fd);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_ECHO_RECV) {
		int checksum=0, checksumrecv;
		char test[1024 * 64];

		checksum += util_destring(2, test);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_ECHO_RECV-test:%s\n", test);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%s", test);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_Echo_recv(fd, test);
		util_DiscardMessage();
		return 0;
	}
/*
	if (func==LSSPROTO_SHUTDOWN_RECV) {
		int checksum=0, checksumrecv;
		char passwd[1024 * 64];
		int min;

		checksum += util_destring(2, passwd);
		checksum += util_deint(3, &min);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_SHUTDOWN_RECV-passwd:%s,min:%d\n", passwd,min);
#endif
//		lssproto_Shutdown_recv(fd, passwd, min);
		util_DiscardMessage();
		return 0;
	}
*/
	if (func==LSSPROTO_TD_RECV) {
		int checksum=0, checksumrecv;
		char message[1024 * 64];

		checksum += util_destring(2, message);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_TD_RECV-message:%s\n", message);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%s", message);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_TD_recv(fd, message);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_FM_RECV) {
		int checksum=0, checksumrecv;
		char message[1024 * 64];

		checksum += util_destring(2, message);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_FM_RECV-message:%s\n", message);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%s", message);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_FM_recv(fd, message);
		util_DiscardMessage();
		return 0;
	}
    
	if (func==LSSPROTO_PETST_RECV) {
		int checksum=0, checksumrecv;
		int nPet;
		int sPet;		

		checksum += util_deint(2, &nPet);
		checksum += util_deint(3, &sPet);		
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}	
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_PETST_RECV-nPet:%d,sPet:%d\n", nPet, sPet);
#endif

#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d", nPet, sPet);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_PETST_recv(fd, nPet, sPet);		
		util_DiscardMessage();
		return 0;
	}

#ifdef _CHECK_GAMESPEED
	if (func==LSSPROTO_CS_RECV) {
	    char buffer[2];
	    buffer[0] = '\0';
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_CS_RECV\n");
#endif
#ifdef _ABSOLUTE_DEBUG
		strcpy(errordata, "");
#endif

		lssproto_CS_recv( fd );
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _TEAM_KICKPARTY
	if ( func == LSSPROTO_KTEAM_RECV ) {
		int checksum = 0, checksumrecv;
		int sindex;
		checksum += util_deint( 2, &sindex);
		util_deint( 3, &checksumrecv);
		if(checksum!=checksumrecv){
			util_DiscardMessage();
			logHack( fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_KTEAM_RECV-sindex:%d\n", sindex);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d", sindex);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_KTEAM_recv( fd, sindex);
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _MIND_ICON
	if(func==LSSPROTO_MA_RECV){
		int checksum = 0, checksumrecv;
		int nMind;
		int x, y;
		
		checksum += util_deint( 2, &nMind);
		checksum += util_deint( 3, &x);
		checksum += util_deint( 4, &y);
		util_deint( 5, &checksumrecv);
		if(checksum!=checksumrecv){
			util_DiscardMessage();
			logHack( fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}		
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_MA_RECV-x:%d,y:%d,nMind:%d\n", x, y, nMind);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d", x, y, nMind);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_MA_recv(fd, x, y, nMind);
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _CHATROOMPROTOCOL			// (不可开) Syu ADD 聊天室频道
	if (func==LSSPROTO_CHATROOM_RECV) {
		int checksum=0, checksumrecv;
		char test[1024 * 64];

		checksum += util_destring(2, test);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_CHATROOM_RECV-test:%s\n", test);
#endif

#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%s", test);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_CHATROOM_recv(fd, test);
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _NEWREQUESTPROTOCOL			// (不可开) Syu ADD 新增Protocol要求细项
	if ( func==LSSPROTO_RESIST_RECV ) {
	    char buffer[2];
	    buffer[0] = '\0';
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_RESIST_RECV\n");
#endif

#ifdef _ABSOLUTE_DEBUG
		strcpy(errordata, "");
#endif

		lssproto_RESIST_recv( fd );
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _OUTOFBATTLESKILL			// (不可开) Syu ADD 非战斗时技能Protocol
		if(func==LSSPROTO_BATTLESKILL_RECV){
				int checksum = 0, checksumrecv;
				int iNum;
				
				checksum += util_deint( 2, &iNum);
				util_deint( 3, &checksumrecv);
				if(checksum!=checksumrecv){
					util_DiscardMessage();
					logHack( fd, HACK_CHECKSUMERROR);
					DME(); return -1;
				}		
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_BATTLESKILL_RECV-iNum:%d\n", iNum);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d", iNum);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
				lssproto_BATTLESKILL_recv(fd, iNum);
				util_DiscardMessage();
				return 0;
		}
#endif

#ifdef _STREET_VENDOR
	if(func == LSSPROTO_STREET_VENDOR_RECV){
		int checksum = 0,checksumrecv;
		char message[1024 * 64];

		checksum += util_destring(2,message);
		util_deint(3,&checksumrecv);
		if(checksum != checksumrecv){
			util_DiscardMessage();
			logHack(fd,HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_STREET_VENDOR_RECV-message:%s\n", message);
#endif

#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%s", message);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif

		lssproto_STREET_VENDOR_recv(fd,message);
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _JOBDAILY
	if(func == LSSPROTO_JOBDAILY_RECV){
		int checksum = 0,checksumrecv;
		char buffer[1024 * 32];

		checksum += util_destring(2,buffer);
		util_deint(3,&checksumrecv);
		if(checksum != checksumrecv){
			util_DiscardMessage();
			logHack(fd,HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_JOBDAILY_RECV-buffer:%s\n", buffer);
#endif

#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%s", buffer);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_JOBDAILY_recv(fd,buffer);
		util_DiscardMessage();
		return 0;

	}
#endif
#ifdef _TEACHER_SYSTEM
	if(func == LSSPROTO_TEACHER_SYSTEM_RECV){
		int checksum = 0,checksumrecv;
		char message[1024 * 64];

		checksum += util_destring(2,message);
		util_deint(3,&checksumrecv);
		if(checksum != checksumrecv){
			util_DiscardMessage();
			logHack(fd,HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_TEACHER_SYSTEM_RECV-message:%s\n", message);
#endif

#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%s", message);
#endif

		lssproto_TEACHER_SYSTEM_recv(fd,message);
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _ADD_STATUS_2
	if(func == LSSPROTO_S2_RECV){
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_S2_RECV\n");
#endif
#ifdef _ABSOLUTE_DEBUG
		strcpy(errordata, "");
#endif

		return 0;
	}
#endif

#ifdef _PET_ITEM
	if (func==LSSPROTO_PET_ITEM_RECV) {
		int checksum=0, checksumrecv;
		int petindex;
		int x,y;
		int fromindex;
		int toindex;
		
		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &petindex);
		checksum += util_deint(5, &fromindex);
		checksum += util_deint(6, &toindex);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_PET_ITEM_RECV-x:%d,y:%d,petindex:%d,fromindex:%d,toindex:%d\n", x, y, petindex, fromindex, toindex);
#endif

#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d:%d:%d", x, y, petindex, fromindex, toindex);
#endif

		lssproto_PETITEM_recv( fd, x, y, petindex, fromindex, toindex );
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _ASSESS_ABILITY
	if (func==LSSPROTO_ASSESS_ABILITY_RECV) {
		int checksum=0, checksumrecv;
		char data[256];
		
		checksum += util_destring(2,data);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_ASSESS_ABILITY_RECV-data:%s\n", data);
#endif

#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%s", data);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		//lssproto_ASSESS_ABILITY_send( fd, "10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|" );
		lssproto_ASSESS_ABILITY_recv(fd);
		util_DiscardMessage();
		return 0;
	}
#endif


#ifdef _ONLINE_SHOP
	if (func==LSSPROTO_VIP_SHOP_RECV) {
		int checksum=0, checksumrecv;
		int type,page;
		
		checksum += util_deint(2, &type);
		checksum += util_deint(3, &page);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_VIP_SHOP_RECV-x:%d,y:%d\n", type, page);
#endif

#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d", type, page);
#endif
		lssproto_VIP_SHOP_recv( fd, type, page );
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _ONLINE_SHOP
	if (func==LSSPROTO_VIP_SHOP_BUY_RECV) {
		int checksum=0, checksumrecv;
		int type,page,id,num;
		
		checksum += util_deint(2, &type);
		checksum += util_deint(3, &page);
		checksum += util_deint(4, &id);
		checksum += util_deint(5, &num);
		util_deint(6, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_VIP_SHOP_RECV-type:%d,page:%d,id:%d,num:%d\n", type, page, id, num);
#endif

#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d:%d", type, page, id, num);
#endif

		lssproto_VIP_SHOP_buy_recv(fd, type, page, id, num);
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _ONLINE_SHOP
	if (func==LSSPROTO_VIP_SHOP_AMPOINT_BUY_RECV) {
		int checksum=0, checksumrecv;
		int type,page,id,num;
		
		checksum += util_deint(2, &type);
		checksum += util_deint(3, &page);
		checksum += util_deint(4, &id);
		checksum += util_deint(5, &num);
		util_deint(6, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_VIP_SHOP_RECV-type:%d,page:%d,id:%d,num:%d\n", type, page, id, num);
#endif

#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d:%d:%d:%d", type, page, id, num);
#endif

		lssproto_VIP_SHOP_buy_recv(fd, type, page, id, num);
		util_DiscardMessage();
		return 0;
	}
#endif

	if (func==LSSPROTO_SAMENU_RECV) {
		int checksum=0, checksumrecv;
		int index;
		checksum += util_deint(2, &index);
		util_deint(3, &checksumrecv);
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_SAMENU_RECV-index:%d\n", index);
#endif

#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "");
#endif

		lssproto_SaMenu_recv(fd, index);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_SHOPOK_RECV) {
		int escindex,checksumrecv;
		int checksum = 0;
		checksum += util_deint(2, &escindex);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			return -1;
		}
		NPC_Lua_NEWSHOP_Recv("ITRF_NEWSHOP",CONNECT_getCharaindex(fd));
		//ABNPC_Lua_NEWSHOP_Recv(CONNECT_getCharaindex(fd));
		util_DiscardMessage();
		return 0;
	}
#ifdef _FAMILYBADGE_
	if (func==LSSPROTO_FAMILYBADGE_RECV) {
		int checksum=0, checksumrecv;
		int test;

		checksum += util_deint(2, &test);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
#ifdef _DEBUG_RET_CLI
		printf("[接收]LSSPROTO_FAMILYBADGE_RECV:%s\n", test);
#endif
#ifdef _ABSOLUTE_DEBUG
		sprintf(errordata, "%d", test);
#endif
#ifdef _MO_SHOW_DEBUG
		if (isDebug()==1) {
			printf("%s\n",errordata);
		}
#endif
		lssproto_FamilyBadge_recv(fd);
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _NEW_TITLE
	if (func == LSSPROTO_CHARTITLE_RECV) {
		int chartitleindex, checksumrecv;
		int checksum = 0;
		checksum += util_deint(2, &chartitleindex);
		util_deint(3, &checksumrecv);
		if (checksum != checksumrecv) {
			util_DiscardMessage();
			DME();
			return -1;
		}
		if(chartitleindex==-1){
			CHAR_CancelNewTitle(CONNECT_getCharaindex(fd));
		}else{
			CHAR_SetNewTitleUse(CONNECT_getCharaindex(fd),chartitleindex);
		}
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _VISUAL_BEATITUDE
	if (func == LSSPROTO_VB_RECV) {
		int checksumrecv;
		int checksum = 0;
		int petindex = 0;
		int type = 0;
		checksum += util_deint(2, &petindex);
		checksum += util_deint(3, &type);
		util_deint(4, &checksumrecv);
		if (checksum != checksumrecv) {
			util_DiscardMessage();
			DME();
			return -1;
		}
		FreeVisualBeatitude(CONNECT_getCharaindex(fd),CHAR_getCharPet( CONNECT_getCharaindex(fd), petindex),type);
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _RIDEQUERY_
	if (func == LSSPROTO_RIDEQUERY_RECV) {
		int checksumrecv;
		int checksum = 0;

		int id = 0;
		checksum += util_deint(2, &id);
		util_deint(3, &checksumrecv);
		if (checksum != checksumrecv || id != 1458) {
			util_DiscardMessage();
			DME();
			return -1;
		}
		FreeRideQuery(CONNECT_getCharaindex(fd));
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _RED_MEMOY_
	if (func == LSSPROTO_REDMEMOY_RECV) {
		int checksumrecv;
		int checksum = 0;

		int type;
		char str[512];
		checksum += util_deint(2, &type);
		checksum += util_destring(3,str);
		util_deint(4, &checksumrecv);
		if (checksum != checksumrecv) {
			util_DiscardMessage();
			DME();
			return -1;
		}
		extern void CHAR_RedMemoy_WindowSend( int charaindex);
		extern void CHAR_RedMemoy_Give(int charaindex,char *data);
		if(type==1){//发送窗体
			CHAR_RedMemoy_WindowSend(CONNECT_getCharaindex(fd));
		}else if(type==2){//领取红包
			CHAR_RedMemoy_Give(CONNECT_getCharaindex(fd),str);
		}
		util_DiscardMessage();
		return 0;
	}
#endif





#ifdef _CHARSIGNDAY_
	if (func == LSSPROTO_SIGNDAY_RECV) {
		int checksumrecv;
		int checksum = 0;

		int id = 0;
		checksum += util_deint(2, &id);
		util_deint(3, &checksumrecv);
		if (checksum != checksumrecv || id != 1458) {
			util_DiscardMessage();
			DME();
			return -1;
		}

		NPC_Lua_CharSignDay_Recv("CharSignDay", CONNECT_getCharaindex(fd));
		util_DiscardMessage();
		return 0;
	}

#endif



	printf("\n无法找到客户端接口=%d\n",func);
	util_DiscardMessage();
	logHack(fd,HACK_NOTDISPATCHED);
	DME(); return -1;
}

/*
   servertoclient XYD( int x, int y, int dir );
*/
void lssproto_XYD_send(int fd,int x,int y,int dir)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_XYD_SEND-x:%d,y:%d,dir:%d\n",x,y,dir);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, x);
	checksum += util_mkint(buffer, y);
	checksum += util_mkint(buffer, dir);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_XYD_SEND, buffer);
}

void lssproto_EV_send(int fd,int seqno,int result)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_EV_SEND-seqno:%d,result:%d\n",seqno,result);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, seqno);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_EV_SEND, buffer);
}

void lssproto_EN_send(int fd,int result,int field)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_EN_SEND-result:%d,field:%d\n",result,field);
#endif
	char buffer[65500];
	int checksum=0;

	//print(" EN_send ");

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, result);
	checksum += util_mkint(buffer, field);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_EN_SEND, buffer);
}


void lssproto_RS_send(int fd,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_RS_SEND-data:%s\n",data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_RS_SEND, buffer);
}


void lssproto_RD_send(int fd,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_RD_SEND-data:%s\n",data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_RD_SEND, buffer);
}

void lssproto_B_send(int fd,char* command)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
	if(strlen(command)==0){
		sprintf( command, "FF|");
	}
	char buffer[65500];
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_B_SEND-command:%s\n",command);
#endif

	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);
	checksum += util_mkstring(buffer, command);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_B_SEND, buffer);
}

void lssproto_I_send(int fd,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_I_SEND-data:%s\n",data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_I_SEND, buffer);
}

void lssproto_SI_send(int fd,int fromindex,int toindex)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_SI_SEND-fromindex:%d,toindex:%d\n",fromindex,toindex);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);
	checksum += util_mkint(buffer, fromindex);
	checksum += util_mkint(buffer, toindex);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SI_SEND, buffer);
}

void lssproto_MSG_send(int fd,int aindex,char* text,int color)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]lssproto_MSG_send-aindex:%d,text:%s,color:%d\n",aindex,text,color);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, aindex);
	checksum += util_mkstring(buffer, text);
	checksum += util_mkint(buffer, color);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_MSG_SEND, buffer);
}


void lssproto_PME_send(int fd,int objindex,int graphicsno,int x,int y,int dir,int flg,int no,char* cdata)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_PME_SEND-objindex:%d,graphicsno:%d,x:%d,y:%d,dir:%d,flg:%d,no:%d,cdata:%s\n",objindex,graphicsno,x,y,dir,flg,no,cdata);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, objindex);
	checksum += util_mkint(buffer, graphicsno);
	checksum += util_mkint(buffer, x);
	checksum += util_mkint(buffer, y);
	checksum += util_mkint(buffer, dir);
	checksum += util_mkint(buffer, flg);
	checksum += util_mkint(buffer, no);
	checksum += util_mkstring(buffer, cdata);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PME_SEND, buffer);
}


void lssproto_AB_send(int fd,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_AB_SEND-data:%s\n",data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_AB_SEND, buffer);
}


void lssproto_ABI_send(int fd,int num,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_ABI_SEND-num:%d,data:%s\n",num,data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, num);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_ABI_SEND, buffer);
}

void lssproto_TK_send(int fd,int index,char* message,int color)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_TK_SEND-index:%d,message:%s,color:%d\n",index,message,color);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, index);
	checksum += util_mkstring(buffer, message);
	checksum += util_mkint(buffer, color);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_TK_SEND, buffer);
}

void lssproto_MC_send(int fd,int fl,int x1,int y1,int x2,int y2,int tilesum,int objsum,int eventsum,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_MC_SEND-fl:%d,x1:%d,y1:%d,x2:%d,y2:%d,tilesum:%d,objsum:%d,eventsum:%d,data:%s\n",fl,x1,y1,x2,y2,tilesum,objsum,eventsum,data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, fl);
	checksum += util_mkint(buffer, x1);
	checksum += util_mkint(buffer, y1);
	checksum += util_mkint(buffer, x2);
	checksum += util_mkint(buffer, y2);
	checksum += util_mkint(buffer, tilesum);
	checksum += util_mkint(buffer, objsum);
	checksum += util_mkint(buffer, eventsum);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_MC_SEND, buffer);
}


void lssproto_M_send(int fd,int fl,int x1,int y1,int x2,int y2,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_M_SEND-fl:%d,x1:%d,y1:%d,x2:%d,y2:%d,data:%s\n",fl,x1,y1,x2,y2,data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, fl);
	checksum += util_mkint(buffer, x1);
	checksum += util_mkint(buffer, y1);
	checksum += util_mkint(buffer, x2);
	checksum += util_mkint(buffer, y2);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_M_SEND, buffer);
}

void lssproto_C_send(int fd,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_C_SEND-data:%s\n",data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_C_SEND, buffer);
}

void lssproto_CA_send(int fd,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_CA_SEND-data:%s\n",data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CA_SEND, buffer);
}

void lssproto_CD_send(int fd,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_CD_SEND-data:%s\n",data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CD_SEND, buffer);
}

void lssproto_R_send(int fd,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_R_SEND-data:%s\n",data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_R_SEND, buffer);
}

void lssproto_S_send(int fd,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_S_SEND-data:%s\n",data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_S_SEND, buffer);
}

void lssproto_D_send(int fd,int category,int dx,int dy,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_D_SEND-category:%d,dx:%d,dy:%d,data:%s\n",category,dx,dy,data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, category);
	checksum += util_mkint(buffer, dx);
	checksum += util_mkint(buffer, dy);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_D_SEND, buffer);
}

void lssproto_FS_send(int fd,int flg)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_FS_SEND-flg:%d\n",flg);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, flg);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_FS_SEND, buffer);
}

void lssproto_HL_send(int fd,int flg)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_HL_SEND-flg:%d\n",flg);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, flg);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_HL_SEND, buffer);
}

void lssproto_PR_send(int fd,int request,int result)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_PR_SEND-request:%d,result:%d\n",request,result);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, request);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PR_SEND, buffer);
}

#ifdef _PETS_SELECTCON
void lssproto_PETS_send(int fd,int petarray,int result)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_PETST_SEND-petarray:%d,result:%d\n",petarray,result);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, petarray);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PETST_SEND, buffer);
}
#endif

void lssproto_KS_send(int fd,int petarray,int result)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_KS_SEND-petarray:%d,result:%d\n",petarray,result);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, petarray);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_KS_SEND, buffer);
}

void lssproto_SPET_send(int fd, int standbypet, int result)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_SPET_SEND-standbypet:%d,result:%d\n",standbypet,result);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, standbypet);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SPET_SEND, buffer);
}

void lssproto_PS_send(int fd,int result,int havepetindex,int havepetskill,int toindex)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_PS_SEND-result:%d,havepetindex:%d,havepetskill:%d,toindex:%d\n",result,havepetindex,havepetskill,toindex);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, result);
	checksum += util_mkint(buffer, havepetindex);
	checksum += util_mkint(buffer, havepetskill);
	checksum += util_mkint(buffer, toindex);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PS_SEND, buffer);
}

void lssproto_SKUP_send(int fd,int point)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_SKUP_SEND-point:%d\n",point);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, point);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SKUP_SEND, buffer);
}
void lssproto_WN_send(int fd,int windowtype,int buttontype,int seqno,int objindex,char* data)
{
	char buffer[65500];
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_WN_SEND-windowtype:%d,buttontype:%d,seqno:%d,objindex:%d,data:%s\n",windowtype,buttontype,seqno,objindex,data);
#endif

	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, windowtype);
	checksum += util_mkint(buffer, buttontype);
	checksum += util_mkint(buffer, seqno);
	checksum += util_mkint(buffer, objindex);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_WN_SEND, buffer);
	
}

void lssproto_EF_send(int fd,int effect,int level,char* option)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_EF_SEND-effect:%d,level:%d,char:%s\n",effect,level,option);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, effect);
	checksum += util_mkint(buffer, level);
	checksum += util_mkstring(buffer, option);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_EF_SEND, buffer);
}
void lssproto_SE_send(int fd,int x,int y,int senumber,int sw)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_SE_SEND-x:%d,y:%d,senumber:%d,sw:%d\n",x,y,senumber,sw);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, x);
	checksum += util_mkint(buffer, y);
	checksum += util_mkint(buffer, senumber);
	checksum += util_mkint(buffer, sw);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SE_SEND, buffer);
}

void lssproto_ClientLogin_send(int fd,char* result)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_CLIENTLOGIN_SEND-result:%s\n",result);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);
	checksum += util_mkstring(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CLIENTLOGIN_SEND, buffer);

	
}

void _lssproto_CreateNewChar_send(int fd,char* result,char* data,char *file,int line)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_CREATENEWCHAR_SEND-result:%s,data:%s\n",result,data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CREATENEWCHAR_SEND, buffer);
}

void lssproto_CharDelete_send(int fd,char* result,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_CHARDELETE_SEND-result:%s,data:%s\n",result,data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHARDELETE_SEND, buffer);
}

void lssproto_CharLogin_send(int fd,char* result,char* data)
{
	
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_CHARLOGIN_SEND-result:%s,data:%s\n",result,data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHARLOGIN_SEND, buffer);
}

void lssproto_CharList_send(int fd,char* result,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_CHARLIST_SEND-result:%s,data:%s\n",result,data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHARLIST_SEND, buffer);
}

void lssproto_CharLogout_send(int fd,char* result,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_CHARLOGOUT_SEND-result:%s,data:%s\n",result,data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHARLOGOUT_SEND, buffer);
}

void lssproto_ProcGet_send(int fd,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_PROCGET_SEND-data:%s\n",data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");
	
	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PROCGET_SEND, buffer);
}

void lssproto_PlayerNumGet_send(int fd,int logincount,int player)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_PLAYERNUMGET_SEND-logincount:%d,player:%d\n",logincount,player);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, logincount);
	checksum += util_mkint(buffer, player);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PLAYERNUMGET_SEND, buffer);
}

void lssproto_Echo_send(int fd,char* test)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_ECHO_SEND-test:%s\n",test);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, test);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_ECHO_SEND, buffer);
}

// CoolFish: Trade 2001/4/18

void lssproto_TD_send(int fd, char* message)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_TD_SEND-message:%s\n",message);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_TD_SEND, buffer);
}
#ifdef _CHATROOMPROTOCOL			// (不可开) Syu ADD 聊天室频道
void lssproto_CHATROOM_send ( int fd , char* message )
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_CHATROOM_SEND-message:%s\n",message);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHATROOM_SEND, buffer);
}
#endif
#ifdef _NEWREQUESTPROTOCOL			// (不可开) Syu ADD 新增Protocol要求细项
void lssproto_RESIST_send ( int fd , char* message )
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_RESIST_SEND-message:%s\n",message);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_RESIST_SEND, buffer);
}
#endif
#ifdef _OUTOFBATTLESKILL			// (不可开) Syu ADD 非战斗时技能Protocol
void lssproto_BATTLESKILL_send ( int fd , char* message )
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_BATTLESKILL_SEND-message:%s\n",message);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_BATTLESKILL_SEND, buffer);
}
#endif
#ifdef _FAMILYBADGE_
void lssproto_CHAREFFECT_send ( int fd , char* message )
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_BATTLESKILL_SEND-message:%s\n",message);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHAREFFECT_SEND, buffer);
}
#endif


void lssproto_NU_send(int fd, int nu)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_NU_SEND-nu:%d\n",nu);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, nu);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_NU_SEND, buffer);
}


void lssproto_FM_send(int fd, char* message)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_FM_SEND-message:%s\n",message);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_FM_SEND, buffer);
}


void lssproto_WO_send(int fd,int effect)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_WO_SEND-effect:%d\n",effect);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, effect);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_WO_SEND, buffer);
}
#ifdef _ITEM_CRACKER
void lssproto_IC_send(int fd, int x, int y)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]lssproto_IC_send-x:%d,y:%d\n",x,y);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, x);
	checksum += util_mkint(buffer, y);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_IC_SEND, buffer);
}
#endif
#ifdef _MAGIC_NOCAST             // 精灵:沉默
void lssproto_NC_send(int fd,int flg)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_NC_SEND-flg:%d\n",flg);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, flg);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_NC_SEND, buffer);
}
#endif

#ifdef _STREET_VENDOR
void lssproto_STREET_VENDOR_send(int fd,char *message)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_STREET_VENDOR_SEND-message:%s\n",message);
#endif
	char buffer[65500];
	int checksum = 0;

	strcpy(buffer,"");

	CONNECT_getCdkey(fd,PersonalKey,4096);
	strcat(PersonalKey,_RUNNING_KEY);

	checksum += util_mkstring(buffer,message);
	util_mkint(buffer,checksum);
	util_SendMesg(fd,LSSPROTO_STREET_VENDOR_SEND,buffer);
}
#endif

#ifdef _RIGHTCLICK
void lssproto_RCLICK_send(int fd, int type, char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_RCLICK_SEND-type:%d,data:%s\n",type,data);
#endif
	char buffer[65500];
	int checksum=0;

	print("\n RCLICK_send( type=%d data=%s) ", type, data );

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, type);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_RCLICK_SEND, buffer);
}
#endif

#ifdef _JOBDAILY
void lssproto_JOBDAILY_send(int fd,char *data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_JOBDAILY_SEND-data:%s\n",data);
#endif
	char buffer[65500];
	int checksum = 0;

	strcpy(buffer,"");

	CONNECT_getCdkey(fd,PersonalKey,4096);
	strcat(PersonalKey,_RUNNING_KEY);

	checksum += util_mkstring(buffer,data);
	util_mkint(buffer,checksum);
	util_SendMesg(fd,LSSPROTO_JOBDAILY_SEND,buffer);
}
#endif

#ifdef _TEACHER_SYSTEM
void lssproto_TEACHER_SYSTEM_send(int fd,char *data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_TEACHER_SYSTEM_SEND-data:%s\n",data);
#endif
	char buffer[65500];
	int checksum = 0;

	strcpy(buffer,"");

	CONNECT_getCdkey(fd,PersonalKey,4096);
	strcat(PersonalKey,_RUNNING_KEY);

	checksum += util_mkstring(buffer,data);
	util_mkint(buffer,checksum);
	util_SendMesg(fd,LSSPROTO_TEACHER_SYSTEM_SEND,buffer);
}
#endif

#ifdef _ADD_STATUS_2
void lssproto_S2_send(int fd,char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_S2_SEND-data:%s\n",data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_S2_SEND, buffer);
}
#endif

#ifdef _ONLINE_SHOP
void lssproto_VIP_SHOP_send(int fd, int num,int BJ,int type, int shoppage, int page, char *data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]lssproto_VIP_SHOP_send-num:%d,BJ:%d,type:%d,shoppage:%d,page:%d,data:%s\n", num, BJ, type, shoppage, page, data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, num);    //有效数目
	checksum += util_mkint(buffer, BJ);   //贝币数目
	checksum += util_mkint(buffer, shoppage);    //显示页面
	checksum += util_mkint(buffer, page); //总共页数
	checksum += util_mkint(buffer, type); //显示类别
	checksum += util_mkstring(buffer, data);

	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_VIP_SHOP_SEND, buffer);
}
#endif

#ifdef _ASSESS_ABILITY
void lssproto_ASSESS_ABILITY_send(int fd, char *data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]lssproto_ASSESS_ABILITY_send-data:%s\n", data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);

	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_ASSESS_ABILITY_SEND, buffer);
}
#endif

void lssproto_DENGON_send(int fd, char *data, int color, int num)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]lssproto_DENGON_send-data:%s,%d,%d\n", data, color, num);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	checksum += util_mkint(buffer, color);
	checksum += util_mkint(buffer, num);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_DENGON_SEND, buffer);
}

void lssproto_SAMENU_send(int fd, int index, char* data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]lssproto_SAMENU_send-data:%d,%s\n", index, data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, index);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SAMENU_SEND, buffer);
}
void lssproto_UpShopData_send(int fd,char *data,char * md5,int id)
{
	unsigned long len = strlen(data);
	unsigned long comprLen=1024*100-55;
	char compr[1024*100]={0};
	compress(compr+55, &comprLen,data , len);
	snprintf(compr,sizeof(compr),"商城%d|%d|%d|%s",len,comprLen,id,md5);
  	fd_set rfds, wfds , efds;
	FD_ZERO( & rfds );
	FD_ZERO( & wfds );
	FD_ZERO( & efds );
	FD_SET( fd , & rfds );
	FD_SET( fd , & wfds );
	FD_SET( fd , & efds );
	struct timeval tmv;
	tmv.tv_sec = tmv.tv_usec = 0;
	int ret = select( fd + 1 , &rfds,&wfds,&efds,&tmv );
	if ( ret > 0 && FD_ISSET( fd , &wfds ) ) {
		ret=send(fd,compr,comprLen+55,0);
        if ( ret == -1 && errno != EINTR ) {
#ifdef _NETLOG_
		char cdkey[16];
		char charname[32];
		CONNECT_getCharname(CONNECT_getCharaindex( fd ),charname,32);
		CONNECT_getCdkey(CONNECT_getCharaindex( fd ),cdkey,16);
		char token[128];
		sprintf(token, "商城 send T人 ret=%d  errno=%s",ret, strerror(errno));
		LogCharOut(charname,cdkey,__FILE__,__FUNCTION__,__LINE__,token);
#endif


			CONNECT_endOne_debug( fd );
        }
    }else if( ret < 0 && errno != EINTR ){
#ifdef _NETLOG_
		char cdkey[16];
		char charname[32];
		CONNECT_getCharname(CONNECT_getCharaindex( fd ),charname,32);
		CONNECT_getCdkey(CONNECT_getCharaindex( fd ),cdkey,16);
		char token[128];
		sprintf(token, "商城 select T人 ret=%d  errno=%s",ret, strerror(errno));
		LogCharOut(charname,cdkey,__FILE__,__FUNCTION__,__LINE__,token);
#endif


      	CONNECT_endOne_debug(fd );
    }
}

void lssproto_ShopOK_send(int fd)
{
	char buffer[1024 * 64];
	strcpy(buffer, "");
	int checksum = 0;
	checksum += util_mkint(buffer, 0);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SHOPOK_SEND, buffer);
}

#ifdef _FAMILYBADGE_
void lssproto_FamilyBadge_send(int fd, char *data)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]lssproto_FamilyBadge_send-data:%s\n", data);
#endif
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);

	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_FAMILYBADGE_SEND, buffer);
}
#endif
#ifdef _NEW_TITLE
void lssproto_CharTitle_send(int fd,char* data)//发送新数据
{
	char buffer[1024 * 64];
	int checksum=0;
	strcpy(buffer,"");
	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHARTITLE_SEND, buffer);
	char msg[256];
	int fd_charaindex = CONNECT_getCharaindex(fd);
	sprintf(msg,"4|%d",CHAR_getInt( fd_charaindex, CHAR_TITLE_DEFAULT ));
	lssproto_CHAREFFECT_send(getfdFromCharaIndex(fd_charaindex), msg);
}
#endif

#ifdef _VISUAL_BEATITUDE
void lssproto_VisualBeatitude_send(int fd,char* data)
{
	char buffer[1024 * 64];
	int checksum=0;
	strcpy(buffer,"");
	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_VB_SEND, buffer);
}
#endif


#ifdef _RED_MEMOY_
void lssproto_RedMemoy_send(int fd,int type,int time,int vip,char *name,int index)
{
	if( CONNECT_checkfd(fd) == FALSE )return;
	char buffer[65500];
	int checksum=0;
	strcpy(buffer,"");
	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, type);
	checksum += util_mkint(buffer, time);
	checksum += util_mkint(buffer, vip);
	checksum += util_mkstring(buffer, name);
	checksum += util_mkint(buffer, index);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_REDMEMOY_SEND, buffer);
}
#endif

#ifdef _MOVE_SCREEN
void lssproto_MoveScreen_send(int fd, BOOL bMoveScreenMove, int iXY)
{
	char szBuffer[65500];
	int iChecksum = 0;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_MOVESCREEN_SEND-fd:%d,bMoveScreenMove:%d,iXY:%d\n",fd,bMoveScreenMove,iXY);
#endif
	szBuffer[0] = '\0';

	CONNECT_getCdkey(fd, PersonalKey, 4096);
#ifdef _NEW_ENCRYT
	CONNECT_catRunKey(fd, PersonalKey);
#else
	strcat(PersonalKey, _RUNNING_KEY);
#endif
	iChecksum += util_mkint(szBuffer, bMoveScreenMove);
	iChecksum += util_mkint(szBuffer, iXY);
	util_mkint(szBuffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_MOVE_SCREEN_SEND, szBuffer);
}
#endif

#ifdef _THEATER
void lssproto_TheaterData_send(int fd, char *pData)
{
	int		iChecksum = 0;
	char	szBuffer[65500];
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_TheaterData_SEND-fd:%d,pData:%s\n",fd,pData);
#endif
	strcpy(szBuffer,"");

	CONNECT_getCdkey(fd, PersonalKey, 4096);
#ifdef _NEW_ENCRYT
	CONNECT_catRunKey(fd, PersonalKey);
#else
	strcat(PersonalKey, _RUNNING_KEY);
#endif
	iChecksum += util_mkstring(szBuffer, pData);
	util_mkint(szBuffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_THEATER_DATA_SEND, szBuffer);
}
#endif

#ifdef _NPC_MAGICCARD
void lssproto_MagiccardAction_send(int fd, char *data)
{
	char buffer[65500];
	int checksum = 0;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTO_MagiccardAction_SEND-fd:%d,data:%s\n",fd,data);
#endif
	strcpy(buffer,"");

	CONNECT_getCdkey(fd,PersonalKey,4096);
#ifdef _NEW_ENCRYT
	CONNECT_catRunKey(fd, PersonalKey);
#else
	strcat(PersonalKey, _RUNNING_KEY);
#endif
	checksum += util_mkstring(buffer,data);
	util_mkint(buffer,checksum);
	util_SendMesg(fd,LSSPROTO_MAGICCARD_ACTION_SEND,buffer);
}

void lssproto_MagiccardDamage_send(int fd, int position ,int damage ,int offsetx,int offsety )
{
	char buffer[65500];
	int checksum = 0;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]LSSPROTOMagiccardDamage_SEND-fd:%d,position:%d,damage:%d,offsetx:%d,offsety:%d\n",fd,position,damage,offsetx,offsety);
#endif
	strcpy(buffer,"");

	CONNECT_getCdkey(fd,PersonalKey,4096);
#ifdef _NEW_ENCRYT
	CONNECT_catRunKey(fd, PersonalKey);
#else
	strcat(PersonalKey, _RUNNING_KEY);
#endif
	checksum += util_mkint(buffer, position );
	checksum += util_mkint(buffer, damage );
	checksum += util_mkint(buffer, offsetx );
	checksum += util_mkint(buffer, offsety );
	util_mkint(buffer,checksum);
	util_SendMesg(fd,LSSPROTO_MAGICCARD_DAMAGE_SEND,buffer);
}
#endif

#ifdef _NPC_DANCE
void lssproto_DancemanOption_send(int fd, int option )
{
	char buffer[65500];
	int checksum = 0;
#ifdef _DEBUG_SEND_CLI
	printf("[发送]lssproto_DancemanOption_send-fd:%d,option:%d\n", fd, option);
#endif
	strcpy(buffer,"");

	CONNECT_getCdkey(fd,PersonalKey,4096);
#ifdef _NEW_ENCRYT
	CONNECT_catRunKey(fd, PersonalKey);
#else
	strcat(PersonalKey, _RUNNING_KEY);
#endif
	checksum += util_mkint(buffer, option );
	util_mkint(buffer,checksum);
	util_SendMesg(fd,LSSPROTO_DANCEMAN_OPTION_SEND,buffer);
}
#endif




