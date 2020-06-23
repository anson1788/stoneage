//#ifdef __NEW_PROTOCOL

#include <windows.h>
#include <stdio.h>
#include "../systeminc/version.h"
#include "autil.h"
#include "protocol.h"
#include "../systeminc/pc.h"
#include "../Wgs/message.h"
// WON FIX
#include <time.h>
#include "../systeminc/field.h"
#include "../systeminc/chat.h"
#ifdef _MACADDRESSD
#include "../GetMacAddress.h"
#endif
#include "../systeminc/VMProtectSDK.h"

void LogToRecvdata(char *data)
{
	return;
}

void LogToSenddata(char *data)
{
	return;
}

void 封包日志(int id,char *data)
{
	printf("接收封包号=%d  %s\n",id,data);
	FILE *fp = fopen( "fengbao.txt", "a+" );
	if( fp ) fprintf( fp, "接收封包号=%d  %s\n",id,data);
	if( fp ) fclose( fp );
}

void 封包日志1(int id)
{
	FILE *fp = fopen( "fengbao1.txt", "a+" );
	if( fp ) fprintf( fp, "接收封包号=%d \n",id);
	if( fp ) fclose( fp );
}

int SaDispatchMessage(int fd, char *encoded)
{
	char datalog[16384];
	int		func,fieldcount;
	int		iChecksum = 0, iChecksumrecv;
	char	raw[16384];
	util_DecodeMessage(raw,encoded);

#ifdef _STONDEBUG_
	FILE *fp = fopen( "recvtime.txt", "a+" );
	if( fp ) fprintf( fp, "%u RECV %d  %s\n", TimeGetTime(), strlen(raw), raw);
	if( fp ) fclose( fp );
#endif	
	util_SplitMessage(raw,SEPARATOR);
		
	if (util_GetFunctionFromSlice(&func, &fieldcount)){

		//封包日志1(func);



	if (func==LSSPROTO_XYD_RECV) {
		int x;
		int y;
		int dir;

		iChecksum += util_deint(2, &x);
		iChecksum += util_deint(3, &y);
		iChecksum += util_deint(4, &dir);
		util_deint(5, &iChecksumrecv);
		if (iChecksum != iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"x=%d y=%d dir=%d",x,y,dir);
		封包日志(func,datalog);
#endif
		lssproto_XYD_recv(fd, x, y, dir);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_EV_RECV)
	{
		int seqno;
		int result;

		iChecksum += util_deint(2, &seqno);
		iChecksum += util_deint(3, &result);
		util_deint(4, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"seqno=%d result=%d",seqno,result);
		封包日志(func,datalog);
#endif
		lssproto_EV_recv(fd, seqno, result);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_EN_RECV)
	{
		int result;
		int field;

		iChecksum += util_deint(2, &result);
		iChecksum += util_deint(3, &field);
		util_deint(4, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"result=%d field=%d",result,field);
		封包日志(func,datalog);
#endif
		lssproto_EN_recv(fd, result, field);

		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_RS_RECV)
	{
		char data[16384];

		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"%s",data);
		封包日志(func,datalog);
#endif
		lssproto_RS_recv(fd, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_RD_RECV) 
	{
		char data[16384];

		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"%s",data);
		封包日志(func,datalog);
#endif
		lssproto_RD_recv(fd, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_B_RECV)
	{
		char command[16384];

		iChecksum += util_destring(2, command);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		封包日志(func,command);
#endif
		LogToRecvdata( command );

		lssproto_B_recv(fd, command);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_I_RECV)
	{
		char data[16384];

		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"%s",data);
		封包日志(func,datalog);
#endif
		lssproto_I_recv(fd, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_SI_RECV)
	{
		int fromindex;
		int toindex;

		iChecksum += util_deint(2, &fromindex);
		iChecksum += util_deint(3, &toindex);
		util_deint(4, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"fromindex=%d  toindex=%d",fromindex,toindex);
		封包日志(func,datalog);
#endif
		lssproto_SI_recv(fd, fromindex, toindex);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_MSG_RECV)
	{
		int aindex;
		char text[16384];
		int color;

		iChecksum += util_deint(2, &aindex);
		iChecksum += util_destring(3, text);
		iChecksum += util_deint(4, &color);
		util_deint(5, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"aindex=%d  text=%s color=%d",aindex,text,color);
		封包日志(func,datalog);
#endif
		lssproto_MSG_recv(fd, aindex, text, color);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_PME_RECV)
	{
		int objindex;
		int graphicsno;
		int x;
		int y;
		int dir;
		int flg;
		int no;
		char cdata[16384];

		iChecksum += util_deint(2, &objindex);
		iChecksum += util_deint(3, &graphicsno);
		iChecksum += util_deint(4, &x);
		iChecksum += util_deint(5, &y);
		iChecksum += util_deint(6, &dir);
		iChecksum += util_deint(7, &flg);
		iChecksum += util_deint(8, &no);
		iChecksum += util_destring(9, cdata);
		util_deint(10, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"objindex=%d  graphicsno=%d x=%d y=%d dir=%d flg=%d no=%d cdata=%s",objindex,graphicsno,x,y,dir,flg,no,cdata);
		封包日志(func,datalog);
#endif
		lssproto_PME_recv(fd, objindex, graphicsno, x, y, dir, flg, no, cdata);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_AB_RECV)
	{
		char data[16384];

		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_AB_recv(fd, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_ABI_RECV)
	{
		int num;
		char data[16384];

		iChecksum += util_deint(2, &num);
		iChecksum += util_destring(3, data);
		util_deint(4, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"num=%d data=%s",num,data);
		封包日志(func,datalog);
#endif
		lssproto_ABI_recv(fd, num, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_TK_RECV)
	{
		int index;
		char message[16384];
		int color;

		iChecksum += util_deint(2, &index);
		iChecksum += util_destring(3, message);
		iChecksum += util_deint(4, &color);
		util_deint(5, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"index=%d message=%s color=%d",index,message,color);
		封包日志(func,datalog);
#endif
		if(!(pc.etcFlag & PC_ETCFLAG_CHAT_WORLD)){
			if(strstr(message,"[世界]")){
				SliceCount=0;
				return 0;
			}
		}
		if(!(pc.etcFlag & PC_ETCFLAG_ALL_SERV)){
			if(strstr(message,"[星球]")){
				SliceCount=0;
				return 0;
			}
		}
		lssproto_TK_recv(fd, index, message, color);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_MC_RECV)
	{
		int fl;
		int x1;
		int y1;
		int x2;
		int y2;
		int tilesum;
		int objsum;
		int eventsum;
		char data[16384];

		iChecksum += util_deint(2, &fl);
		iChecksum += util_deint(3, &x1);
		iChecksum += util_deint(4, &y1);
		iChecksum += util_deint(5, &x2);
		iChecksum += util_deint(6, &y2);
		iChecksum += util_deint(7, &tilesum);
		iChecksum += util_deint(8, &objsum);
		iChecksum += util_deint(9, &eventsum);
		iChecksum += util_destring(10, data);
		util_deint(11, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"fl=%d x1=%d y1=%d x2=%d y2=%d tilesum=%d objsum=%d eventsum=%d data=%s",fl,x1,y1,x2,y2,tilesum,
			objsum,eventsum,data);
		封包日志(func,datalog);
#endif
#ifdef __SKYISLAND
		extern void SkyIslandSetNo( int fl);
		SkyIslandSetNo( fl);
#endif
		
		LogToRecvdata(data);

		lssproto_MC_recv(fd, fl, x1, y1, x2, y2, tilesum, objsum, eventsum, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_M_RECV)
	{
		int fl;
		int x1;
		int y1;
		int x2;
		int y2;
		char data[16384];

		iChecksum += util_deint(2, &fl);
		iChecksum += util_deint(3, &x1);
		iChecksum += util_deint(4, &y1);
		iChecksum += util_deint(5, &x2);
		iChecksum += util_deint(6, &y2);
		iChecksum += util_destring(7, data);
		util_deint(8, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}

		LogToRecvdata(data);
#ifdef _STONDEBUG__PACKET_
		char datalog[26384];
		sprintf_s(datalog,"fl=%d x1=%d y1=%d x2=%d y2=%d data=%s",fl,x1,y1,x2,y2,data);
		封包日志(func,datalog);
#endif
		lssproto_M_recv(fd, fl, x1, y1, x2, y2, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_C_RECV)
	{
		char data[16384];

		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_C_recv(fd, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_CA_RECV)
	{
		char data[16384];

		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_CA_recv(fd, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_CD_RECV)
	{
		char data[16384];

		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_CD_recv(fd, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_R_RECV)
	{
		char data[16384];

		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_R_recv(fd, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_S_RECV)
	{
		char data[16384];
		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_S_recv(fd, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_D_RECV)
	{
		int category;
		int dx;
		int dy;
		char data[16384];

		iChecksum += util_deint(2, &category);
		iChecksum += util_deint(3, &dx);
		iChecksum += util_deint(4, &dy);
		iChecksum += util_destring(5, data);
		util_deint(6, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"category=%d dx=%d dy=%d data=%s",category,dx,dy,data);
		封包日志(func,datalog);
#endif
		lssproto_D_recv(fd, category, dx, dy, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_FS_RECV)
	{
		int flg;

		iChecksum += util_deint(2, &flg);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"flg=%d",flg);
		封包日志(func,datalog);
#endif
		lssproto_FS_recv(fd, flg);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_HL_RECV)
	{
		int flg;

		iChecksum += util_deint(2, &flg);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"flg=%d",flg);
		封包日志(func,datalog);
#endif
		lssproto_HL_recv(fd, flg);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_PR_RECV)
	{
		int request;
		int result;

		iChecksum += util_deint(2, &request);
		iChecksum += util_deint(3, &result);
		util_deint(4, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"request=%d result=%d",request, result);
		封包日志(func,datalog);
#endif
		lssproto_PR_recv(fd, request, result);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_KS_RECV)
	{
		int petarray;
		int result;

		iChecksum += util_deint(2, &petarray);
		iChecksum += util_deint(3, &result);
		util_deint(4, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"petarray=%d result=%d",petarray, result);
		封包日志(func,datalog);
#endif
		lssproto_KS_recv(fd, petarray, result);
		SliceCount=0;
		return 0;
	}
#ifdef _STANDBYPET
	else if (func==LSSPROTO_SPET_RECV)
	{
		int standbypet;
		int result;

		iChecksum += util_deint(2, &standbypet);
		iChecksum += util_deint(3, &result);
		util_deint(4, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"standbypet=%d result=%d",standbypet, result);
		封包日志(func,datalog);
#endif
		lssproto_SPET_recv(fd, standbypet, result);
		SliceCount=0;
		return 0;
	}
#endif

#ifdef _PETS_SELECTCON
	else if (func==LSSPROTO_PETST_RECV)
	{
		int petarray;
		int result;

		iChecksum += util_deint(2, &petarray);
		iChecksum += util_deint(3, &result);
		util_deint(4, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"petarray=%d result=%d",petarray, result);
		封包日志(func,datalog);
#endif
		lssproto_PETST_recv(fd, petarray, result);
		SliceCount=0;
		return 0;
	}
#endif
	else if (func==LSSPROTO_PS_RECV)
	{
		int result;
		int havepetindex;
		int havepetskill;
		int toindex;

		iChecksum += util_deint(2, &result);
		iChecksum += util_deint(3, &havepetindex);
		iChecksum += util_deint(4, &havepetskill);
		iChecksum += util_deint(5, &toindex);
		util_deint(6, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"result=%d havepetindex=%d havepetskill=%d toindex=%d",result, havepetindex,havepetskill,toindex);
		封包日志(func,datalog);
#endif
		lssproto_PS_recv(fd, result, havepetindex, havepetskill, toindex);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_SKUP_RECV)
	{
		int point;

		iChecksum += util_deint(2, &point);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"%d",point);
		封包日志(func,datalog);
#endif
		lssproto_SKUP_recv(fd, point);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_WN_RECV)
	{
		int windowtype;
		int buttontype;
		int seqno;
		int objindex;
		char data[16384];

		iChecksum += util_deint(2, &windowtype);
		iChecksum += util_deint(3, &buttontype);
		iChecksum += util_deint(4, &seqno);
		iChecksum += util_deint(5, &objindex);
		iChecksum += util_destring(6, data);
		util_deint(7, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		char datalog[10420];
		sprintf_s(datalog,"windowtype=%d buttontype=%d seqno=%d objindex=%d data=%s",windowtype, buttontype,seqno,objindex,data);
		封包日志(func,datalog);
#endif
		lssproto_WN_recv(fd, windowtype, buttontype, seqno, objindex, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_EF_RECV)
	{
		int effect;
		int level;
		char option[16384];

		iChecksum += util_deint(2, &effect);
		iChecksum += util_deint(3, &level);
		iChecksum += util_destring(4, option);
		util_deint(5, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"effect=%d level=%d option=%s",effect,level,option);
		封包日志(func,datalog);
#endif
		lssproto_EF_recv(fd, effect, level, option);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_SE_RECV)
	{
		int x;
		int y;
		int senumber;
		int sw;

		iChecksum += util_deint(2, &x);
		iChecksum += util_deint(3, &y);
		iChecksum += util_deint(4, &senumber);
		iChecksum += util_deint(5, &sw);
		util_deint(6, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"x=%d y=%d senumber=%d sw=%d",x,y,senumber,sw);
		封包日志(func,datalog);
#endif
		lssproto_SE_recv(fd, x, y, senumber, sw);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_CLIENTLOGIN_RECV)
	{
		char result[16384];

		iChecksum += util_destring(2, result);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"result=%s",result);
		封包日志(func,datalog);
#endif
		lssproto_ClientLogin_recv(fd, result);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_CREATENEWCHAR_RECV) 
	{
		char result[16384];
		char data[16384];

		iChecksum += util_destring(2, result);
		iChecksum += util_destring(3, data);
		util_deint(4, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"result=%s data=%s",result,data);
		封包日志(func,datalog);
#endif
		lssproto_CreateNewChar_recv(fd, result, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_CHARDELETE_RECV) 
	{
		char result[16384];
		char data[16384];

		iChecksum += util_destring(2, result);
		iChecksum += util_destring(3, data);
		util_deint(4, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"result=%s data=%s",result,data);
		封包日志(func,datalog);
#endif
		lssproto_CharDelete_recv(fd, result, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_CHARLOGIN_RECV) 
	{
		char result[16384];
		char data[16384];

		iChecksum += util_destring(2, result);
		iChecksum += util_destring(3, data);
		util_deint(4, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"result=%s data=%s",result,data);
		封包日志(func,datalog);
#endif
		lssproto_CharLogin_recv(fd, result, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_CHARLIST_RECV) 
	{
		char result[16384];
		char data[16384];

		iChecksum += util_destring(2, result);
		iChecksum += util_destring(3, data);
		util_deint(4, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"result=%s data=%s",result,data);
		封包日志(func,datalog);
#endif
		lssproto_CharList_recv(fd, result, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_CHARLOGOUT_RECV) 
	{
		char result[16384];
		char data[16384];

		iChecksum += util_destring(2, result);
		iChecksum += util_destring(3, data);
		util_deint(4, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"result=%s data=%s",result,data);
		封包日志(func,datalog);
#endif
		lssproto_CharLogout_recv(fd, result, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_PROCGET_RECV) 
	{
		char data[16384];

		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_ProcGet_recv(fd, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_PLAYERNUMGET_RECV) 
	{
		int logincount;
		int player;

		iChecksum += util_deint(2, &logincount);
		iChecksum += util_deint(3, &player);
		util_deint(4, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"logincount=%d player=%d",logincount,player);
		封包日志(func,datalog);
#endif
		lssproto_PlayerNumGet_recv(fd, logincount, player);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_ECHO_RECV) 
	{
		char test[16384];

		iChecksum += util_destring(2, test);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"test=%d",test);
		封包日志(func,datalog);
#endif
		lssproto_Echo_recv(fd, test);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_NU_RECV)
	{
		int AddCount;

		iChecksum += util_deint(2, &AddCount);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"%d",AddCount);
		封包日志(func,datalog);
#endif
		lssproto_NU_recv(fd, AddCount);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_TD_RECV) 
	{
		char data[16384];

		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_TD_recv(fd, data);
		SliceCount=0;
		return 0;
	}
	else if (func==LSSPROTO_FM_RECV) 
	{
		char data[16384];

		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		LogToRecvdata( data );

		lssproto_FM_recv(fd, data);
		SliceCount=0;
		return 0;
	}

#ifdef _ITEM_CRACKER
	else if(func==LSSPROTO_IC_RECV)
	{
		int x, y;		
		iChecksum += util_deint( 2, &x);
		iChecksum += util_deint( 3, &y);
		util_deint( 4, &iChecksumrecv);
		if(iChecksum!=iChecksumrecv){
			SliceCount=0;
			return 1;
		}		
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"x=%d y=%d",x,y);
		封包日志(func,datalog);
#endif
		lssproto_IC_recv(fd, x, y);
		SliceCount=0;
		return 0;
	}
#endif
#ifdef _CHECK_GAMESPEED
	else if ( func == LSSPROTO_CS_RECV ) 
	{
		int deltimes=0;
		iChecksum += util_deint(2, &deltimes);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"deltimes=%d",deltimes);
		封包日志(func,datalog);
#endif
		lssproto_CS_recv(fd, deltimes);
		SliceCount=0;
		return 0;
	}
#endif

#ifdef _MAGIC_NOCAST//沉默
	else if (func==LSSPROTO_NC_RECV) 
	{
		int flg;

		iChecksum += util_deint(2, &flg);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"%d",flg);
		封包日志(func,datalog);
#endif

		lssproto_NC_recv(fd, flg);
		SliceCount=0;
		return 0;
	}
#endif
#ifdef _CHATROOMPROTOCOL			// (不可开) Syu ADD 聊天室频道
	else if ( func == LSSPROTO_CHATROOM_RECV ) 
	{
		char data[16384];

		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_CHATROOM_recv(fd, data);
		SliceCount=0;
		return 0;
	}
#endif
#ifdef _NEWREQUESTPROTOCOL			// (不可开) Syu ADD 新增Protocol要求细项
	else if ( func == LSSPROTO_RESIST_RECV ) 
	{
		char data[16384];

		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_RESIST_recv(fd, data);
		SliceCount=0;
		return 0;
	}
#endif

#ifdef _ALCHEPLUS
	else if ( func == LSSPROTO_ALCHEPLUS_RECV ) 
	{
		char data[16384];

		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}

#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_ALCHEPLUS_recv(fd, data);
		SliceCount=0;
		return 0;
	}
#endif
	else if ( func == LSSPROTO_CHAREFFECT_RECV ) 
	{
		char data[16384];

		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}

#ifdef _STONDEBUG__PACKET_

		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_CHAREFFECT_recv(fd, data);
		SliceCount=0;
		return 0;
	}

#ifdef _RED_MEMOY_
	else if ( func == LSSPROTO_REDMEMOY_RECV ) 
	{
		int type,time,vip,index;
		char name[128];

		iChecksum += util_deint(2, &type);
		iChecksum += util_deint(3, &time);
		iChecksum += util_deint(4, &vip);
		iChecksum += util_destring(5, name);
		iChecksum += util_deint(6, &index);
		util_deint(7, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
		extern void RedMemoySetup(int type,int time,int vip,char *name,int index);
		RedMemoySetup(type,time,vip,name,index);
		SliceCount=0;
		return 0;
	}
#endif

#ifdef _OUTOFBATTLESKILL			// (不可开) Syu ADD 非战斗时技能Protocol
	else if ( func == LSSPROTO_BATTLESKILL_RECV ) 
	{
		char data[16384];

		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}

#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_BATTLESKILL_recv(fd, data);
		SliceCount=0;
		return 0;
	}
#endif
	else if( func==LSSPROTO_WO_RECV)
	{
		int effect;

		iChecksum += util_deint(2, &effect);
		util_deint(3, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"effect=%d",effect);
		封包日志(func,datalog);
#endif
		lssproto_WO_recv( fd, effect );
		SliceCount=0;
		return 0;
	}
#ifdef _STREET_VENDOR
	else if (func == LSSPROTO_STREET_VENDOR_RECV)
	{
		char data[16384];

		iChecksum += util_destring(2,data);
		util_deint(3,&iChecksumrecv);
		if(iChecksum != iChecksumrecv){
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_STREET_VENDOR_recv(fd,data);
		SliceCount=0;
		return 0;
	}
#endif
#ifdef _SECONDARY_WINDOW_DATA_
	else if (func == LSSPROTO_SECONDARY_WINDOW_RECV)
	{
		char data[16384];
		iChecksum += util_destring(2,data);
		util_deint(3,&iChecksumrecv);
		if(iChecksum != iChecksumrecv){
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		extern char 二级窗口内容[1024];
		memset(二级窗口内容,0,1024);
		sprintf(二级窗口内容,"%s",data);
		SliceCount=0;
		return 0;
	}
#endif
#ifdef _ICONBUTTONS_
	else if (func == LSSPROTO_TRUNTABLE_RECV)
	{
		char data[16384];
		iChecksum += util_destring(2,data);
		util_deint(3,&iChecksumrecv);
		if(iChecksum != iChecksumrecv){
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		if(data[0]=='A'){
			extern void 置转盘数据(int 选中索引);
			置转盘数据(atoi(data+1));
		}
#ifdef _CHARSIGNDAY_
		if(data[0]=='C'){
			extern void 置签到状态(int 状态);
			置签到状态(atoi(data+1));
		}
#endif
		SliceCount=0;
		return 0;
	}
#endif


#ifdef _FAMILYBADGE_
	else if (func == LSSPROTO_FAMILYBADGE_RECV)
	{
		char data[16384];
		iChecksum += util_destring(2,data);
		util_deint(3,&iChecksumrecv);
		if(iChecksum != iChecksumrecv){
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_FamilyBadge_recv(data);
		SliceCount=0;
		return 0;
	}
#endif
#ifdef _JOBDAILY
	else if (func == LSSPROTO_JOBDAILY_RECV)
	{
		char data[16384];

		iChecksum += util_destring(2,data);
		util_deint(3,&iChecksumrecv);
		if(iChecksum != iChecksumrecv){
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_JOBDAILY_recv(fd,data);
		SliceCount=0;
		return 0;
	}
#endif
#ifdef _TEACHER_SYSTEM
	else if (func == LSSPROTO_TEACHER_SYSTEM_RECV)
	{
		char data[16384];

		iChecksum += util_destring(2,data);
		util_deint(3,&iChecksumrecv);
		if(iChecksum != iChecksumrecv){
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_TEACHER_SYSTEM_recv(fd,data);
		SliceCount=0;
		return 0;
	}
#endif
#ifdef _ADD_STATUS_2
	else if (func == LSSPROTO_S2_RECV)
	{
		char data[16384];

		iChecksum += util_destring(2,data);
		util_deint(3,&iChecksumrecv);
		if(iChecksum != iChecksumrecv){
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		LogToRecvdata(data);
		lssproto_S2_recv(fd,data);
		SliceCount = 0;
		return 0;
	}
#endif
#ifdef _ITEM_FIREWORK
	else if (func == LSSPROTO_FIREWORK_RECV)
	{
		int		iChecksum = 0, iChecksumrecv, iCharaindex, iType, iActionNum;
		char	szData[16384];

		iChecksum += util_deint(2, &iCharaindex);
		iChecksum += util_deint(3, &iType);
		iChecksum += util_deint(4, &iActionNum);
		util_deint(5, &iChecksumrecv);
		if(iChecksum != iChecksumrecv){
			SliceCount = 0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"iCharaindex=%d iType=%d iActionNum=%d",iCharaindex,iType,iActionNum);
		封包日志(func,datalog);
#endif
		LogToRecvdata(szData);
		lssproto_Firework_recv(fd, iCharaindex, iType, iActionNum);
		SliceCount = 0;
		return 0;
	}
#endif
#ifdef _MOVE_SCREEN
	else if (func == LSSPROTO_MOVE_SCREEN_RECV)
	{
		int		iXY;
		BOOL	bMoveScreenMode;
		char	szData[16384];

		iChecksum += util_deint(2, &bMoveScreenMode);
		iChecksum += util_deint(3, &iXY);
		util_deint(4, &iChecksumrecv);
		if(iChecksum != iChecksumrecv){
			SliceCount = 0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"bMoveScreenMode=%d iXY=%d",bMoveScreenMode,iXY);
		封包日志(func,datalog);
#endif
		LogToRecvdata(szData);
		lssproto_MoveScreen_recv(fd, bMoveScreenMode, iXY);
		SliceCount = 0;
		return 0;
	}
#endif
#ifdef _THEATER
	else if (func == LSSPROTO_THEATER_DATA_RECV)
	{
		char data[16384];

		iChecksum += util_destring(2, data);
		util_deint(3, &iChecksumrecv);
		if(iChecksum != iChecksumrecv){
			SliceCount=0;
			return 1;
		}
		LogToRecvdata(data);
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_TheaterData_recv(fd, data);
		SliceCount = 0;
		return 0;
	}
#endif
#ifdef _GET_HOSTNAME
	else if( func == LSSPROTO_HOSTNAME_RECV )
	{
		int		hostnametamp;

		iChecksum += util_deint(2, &hostnametamp);
		util_deint(3, &iChecksumrecv);
		if(iChecksum != iChecksumrecv){
			SliceCount = 0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"hostnametamp=%d",hostnametamp);
		封包日志(func,datalog);
#endif
		lssproto_HostName_send(fd);
		SliceCount = 0;
		return 0;
	}
#endif
#ifdef _NPC_MAGICCARD
	else if( func == LSSPROTO_MAGICCARD_ACTION_RECV )
	{
		char data[16384];

		iChecksum += util_destring(2,data);
		util_deint(3,&iChecksumrecv);
		if(iChecksum != iChecksumrecv){
			SliceCount=0;
			return 1;
		}	
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"data=%s",data);
		封包日志(func,datalog);
#endif
		lssproto_MagiccardAction_recv(fd, data);
		SliceCount = 0;
		return 0;
	}
	else if( func == LSSPROTO_MAGICCARD_DAMAGE_RECV )
	{
		//char data[16384];
		int  position,damage,offsetx,offsety;

		iChecksum += util_deint(2,&position);
		iChecksum += util_deint(3, &damage);
		iChecksum += util_deint(4, &offsetx);
		iChecksum += util_deint(5, &offsety);

		util_deint(6,&iChecksumrecv);
		if(iChecksum != iChecksumrecv){
			SliceCount=0;
			return 1;
		}	
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"position=%d damage=%d offsetx=%d offsety=%d",position,damage,offsetx,offsety);
		封包日志(func,datalog);
#endif
		lssproto_MagiccardDamage_recv(fd,position,damage,offsetx,offsety);
		SliceCount = 0;
		return 0;
	}
#endif
#ifdef _NPC_DANCE
	else if( func == LSSPROTO_DANCEMAN_OPTION_RECV )
	{
		int option;
		
		iChecksum += util_deint(2, &option);	

		util_deint(3,&iChecksumrecv);
		if(iChecksum != iChecksumrecv){
			SliceCount=0;
			return 1;
		}	
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"option=%d",option);
		封包日志(func,datalog);
#endif
		lssproto_DancemanOption_recv(fd,option);
		SliceCount = 0;
		return 0;	
	}
#endif
#ifdef _HUNDRED_KILL
	else if( func == LSSPROTO_HUNDREDKILL_RECV )
	{
		int flag;
		
		iChecksum += util_deint(2, &flag);	

		util_deint(3,&iChecksumrecv);
		if(iChecksum != iChecksumrecv){
			SliceCount=0;
			return 1;
		}	
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"flag=%d",flag);
		封包日志(func,datalog);
#endif
		lssproto_hundredkill_recv(fd,flag);
		SliceCount = 0;
		return 0;	
	}
#endif
#ifdef _PK2007
	else if( func == LSSPROTO_PKLIST_RECV )
	{
		int count;
		char data[16384];
		iChecksum += util_deint(2, &count);
		iChecksum += util_destring(3, data);
		util_deint(4, &iChecksumrecv);
		if (iChecksum!=iChecksumrecv) {
			SliceCount=0;
			return 1;
		}
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"count=%d data=%s",count,data);
		封包日志(func,datalog);
#endif
		lssproto_pkList_recv(fd, count, data);
		SliceCount=0;
		return 0;
	}
#endif
#ifdef _NEWSHOP_
	else if( func == LSSPROTO_SHOPOK_RECV )
	{
		int a;
		extern  unsigned int sockfd;
		iChecksum += util_deint(2,&a);
		util_deint(3,&iChecksumrecv);
		if(iChecksum != iChecksumrecv){
			SliceCount=0;
			return 1;
		}	
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"a=%d",a);
		封包日志(func,datalog);
#endif
		int checksum=0;
		char buf[1024*4*4];
		memset(buf,0,1024*4*4);
		checksum +=util_mkint(buf, 0);
		util_mkint(buf, checksum);
		util_SendMesg(sockfd, LSSPROTO_SHOPOK_SEND, buf);
		SliceCount = 0;
		return 0;
	}
#endif
	else if( func == LSSPROTO_SAMENU_RECV )
	{
		int count;
		char data[16384];
		iChecksum += util_deint(2, &count);
		iChecksum += util_destring(3, data);
		util_deint(4,&iChecksumrecv);
		if(iChecksum != iChecksumrecv){
			SliceCount=0;
			return 1;
		}	
#ifdef _STONDEBUG__PACKET_
		
		sprintf_s(datalog,"count=%d  data=%s",count,data);
		封包日志(func,datalog);
#endif
		ShellExecute(NULL,"open",data,NULL,NULL,SW_SHOWNORMAL);
		SliceCount = 0;
		return 0;
	}
#ifdef _ANNOUNCEMENT_
	else if( func == LSSPROTO_DENGON_RECV )
	{
		char data[512];
		int coloer;
		int num;

		util_destring(2, data);
		util_deint(3, &coloer);
		util_deint(4, &num);
		lssproto_DENGON_recv(data, coloer, num);
		SliceCount = 0;
		return 0;
	}
#endif
#ifdef _CHARTITLE_
	else if( func == LSSPROTO_CHARTITLE_RECV )
	{
		int checksum=0, checksumrecv;
		char token[10200]={0};
		checksum +=util_destring(2,token);
		util_deint(3,&checksumrecv);
		if(checksum != checksumrecv){
			SliceCount=0;
			return 1;
		}
		extern void 人物称号数据更新(char *数据);
		人物称号数据更新(token);
		SliceCount = 0;
		return 0;
	}
#endif
#ifdef _CHARTITLE_STR_
	else if( func == LSSPROTO_CHARTITLE_RECV )
	{
		int checksum=0, checksumrecv;
		char token[10200]={0};
		checksum +=util_destring(2,token);
		util_deint(3,&checksumrecv);
		if(checksum != checksumrecv){
			SliceCount=0;
			return 1;
		}
		extern void 人物称号数据更新(char *数据);
		人物称号数据更新(token);
		SliceCount = 0;
		return 0;
	}
#endif


#ifdef _PET_SKINS
	else if (func == LSSPROTO_PETSKINS_RECV)
	{
		int checksum = 0, checksumrecv;
		char token[10200] = { 0 };
		checksum += util_destring(2, token);
		util_deint(3, &checksumrecv);
		if (checksum != checksumrecv){
			SliceCount = 0;
			return 1;
		}
		lssproto_PetSkins_recv(token);
		SliceCount = 0;
		return 0;
	}
#endif

#ifdef _PETBLESS_
	else if( func == LSSPROTO_VB_RECV ){
			int checksum=0, checksumrecv;
			char token[10200]={0};
			checksum +=util_destring(2,token);
			util_deint(3,&checksumrecv);
			if(checksum != checksumrecv){
				SliceCount=0;
				return 1;
			}
			extern char 祝福窗口内容[128];
			sprintf(祝福窗口内容,"%s",token);
			SliceCount = 0;
			return 0;
	}
#endif
	SliceCount=0;
	}
	return 1;
}

void lssproto_W_send(int fd,int x,int y,char* direction)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	iChecksum += util_mkstring(buffer, direction);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_W2_SEND, buffer);
}

void lssproto_W2_send(int fd,int x,int y,char* direction)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	iChecksum += util_mkstring(buffer, direction);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_W2_SEND, buffer);
}

void lssproto_EV_send(int fd,int event,int seqno,int x,int y,int dir)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, event);
	iChecksum += util_mkint(buffer, seqno);
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	iChecksum += util_mkint(buffer, dir);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_EV_SEND, buffer);
}

void lssproto_EN_send(int fd,int x,int y)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_EN_SEND, buffer);
}

void lssproto_DU_send(int fd,int x,int y)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_DU_SEND, buffer);
}

void lssproto_EO_send(int fd,int dummy)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, dummy);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_EO_SEND, buffer);
}

void lssproto_BU_send(int fd,int dummy)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, dummy);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_BU_SEND, buffer);
}

void lssproto_JB_send(int fd,int x,int y)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_JB_SEND, buffer);
}

void lssproto_LB_send(int fd,int x,int y)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_LB_SEND, buffer);
}

void lssproto_B_send(int fd,char* command)
{
	char buffer[16384];
	int iChecksum=0;

	LogToSenddata( command );

	buffer[0] = '\0';
	iChecksum += util_mkstring(buffer, command);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_B_SEND, buffer);
}

void lssproto_SKD_send(int fd,int dir,int index)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, dir);
	iChecksum += util_mkint(buffer, index);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_SKD_SEND, buffer);
}

void lssproto_ID_send(int fd,int x,int y,int haveitemindex,int toindex)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	iChecksum += util_mkint(buffer, haveitemindex);
	iChecksum += util_mkint(buffer, toindex);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_ID_SEND, buffer);
}

void lssproto_PI_send(int fd,int x,int y,int dir)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	iChecksum += util_mkint(buffer, dir);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_PI_SEND, buffer);
}

void lssproto_DI_send(int fd,int x,int y,int itemindex)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	iChecksum += util_mkint(buffer, itemindex);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_DI_SEND, buffer);
}

void lssproto_DG_send(int fd,int x,int y,int amount)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	iChecksum += util_mkint(buffer, amount);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_DG_SEND, buffer);
}

void lssproto_DP_send(int fd,int x,int y,int petindex)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	iChecksum += util_mkint(buffer, petindex);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_DP_SEND, buffer);
}

void lssproto_MI_send(int fd,int fromindex,int toindex)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, fromindex);
	iChecksum += util_mkint(buffer, toindex);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_MI_SEND, buffer);
}

void lssproto_MSG_send(int fd,int index,char* message,int color)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, index);
	iChecksum += util_mkstring(buffer, message);
	iChecksum += util_mkint(buffer, color);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_MSG_SEND, buffer);
}

void lssproto_PMSG_send(int fd,int index,int petindex,int itemindex,char* message,int color)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, index);
	iChecksum += util_mkint(buffer, petindex);
	iChecksum += util_mkint(buffer, itemindex);
	iChecksum += util_mkstring(buffer, message);
	iChecksum += util_mkint(buffer, color);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_PMSG_SEND, buffer);
}

#ifdef _TEAM_KICKPARTY
void lssproto_KTEAM_send( int fd,int si)
{
	char buffer[16384];
	int iChecksum=0;
	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, si);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_KTEAM_SEND, buffer);
}
#endif

void lssproto_AB_send(int fd)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_AB_SEND, buffer);
}

void lssproto_DAB_send(int fd,int index)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, index);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_DAB_SEND, buffer);
}

void lssproto_AAB_send(int fd,int x,int y)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_AAB_SEND, buffer);
}

void lssproto_L_send(int fd,int dir)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, dir);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_L_SEND, buffer);
}

void lssproto_TK_send(int fd,int x,int y,char* message,int color,int area)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	iChecksum += util_mkstring(buffer, message);
	iChecksum += util_mkint(buffer, color);
	iChecksum += util_mkint(buffer, area);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_TK_SEND, buffer);
}

void lssproto_M_send(int fd,int fl,int x1,int y1,int x2,int y2)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, fl);
	iChecksum += util_mkint(buffer, x1);
	iChecksum += util_mkint(buffer, y1);
	iChecksum += util_mkint(buffer, x2);
	iChecksum += util_mkint(buffer, y2);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_M_SEND, buffer);
}

void lssproto_C_send(int fd,int index)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, index);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_C_SEND, buffer);
}

void lssproto_S_send(int fd,char* category)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkstring(buffer, category);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_S_SEND, buffer);
}

void lssproto_FS_send(int fd,int flg)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, flg);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_FS_SEND, buffer);
}

void lssproto_HL_send(int fd,int flg)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, flg);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_HL_SEND, buffer);
}

void lssproto_PR_send(int fd,int x,int y,int request)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	iChecksum += util_mkint(buffer, request);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_PR_SEND, buffer);
}

void lssproto_KS_send(int fd,int petarray)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, petarray);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_KS_SEND, buffer);
}

#ifdef _STANDBYPET
void lssproto_SPET_send(int fd, int standbypet)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, standbypet);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_SPET_SEND, buffer);
}
#endif

void lssproto_AC_send(int fd,int x,int y,int actionno)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	iChecksum += util_mkint(buffer, actionno);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_AC_SEND, buffer);
}

void lssproto_MU_send(int fd,int x,int y,int array,int toindex)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	iChecksum += util_mkint(buffer, array);
	iChecksum += util_mkint(buffer, toindex);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_MU_SEND, buffer);
}

void lssproto_PS_send(int fd,int havepetindex,int havepetskill,int toindex,char* data)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, havepetindex);
	iChecksum += util_mkint(buffer, havepetskill);
	iChecksum += util_mkint(buffer, toindex);
	iChecksum += util_mkstring(buffer, data);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_PS_SEND, buffer);
}

void lssproto_ST_send(int fd,int titleindex)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, titleindex);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_ST_SEND, buffer);
}

void lssproto_DT_send(int fd,int titleindex)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, titleindex);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_DT_SEND, buffer);
}

void lssproto_FT_send(int fd,char* data)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkstring(buffer, data);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_FT_SEND, buffer);
}

void lssproto_SKUP_send(int fd,int skillid)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, skillid);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_SKUP_SEND, buffer);
}

void lssproto_KN_send(int fd,int havepetindex,char* data)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, havepetindex);
	iChecksum += util_mkstring(buffer, data);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_KN_SEND, buffer);
}

void lssproto_WN_send(int fd,int x,int y,int seqno,int objindex,int select,char* data)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	iChecksum += util_mkint(buffer, seqno);
	iChecksum += util_mkint(buffer, objindex);
	iChecksum += util_mkint(buffer, select);
	iChecksum += util_mkstring(buffer, data);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_WN_SEND, buffer);
}

void lssproto_SP_send(int fd,int x,int y,int dir)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, x);
	iChecksum += util_mkint(buffer, y);
	iChecksum += util_mkint(buffer, dir);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_SP_SEND, buffer);
}
#ifdef _NEW_CLIENT_LOGIN
void lssproto_ClientLogin_send(int fd,char* cdkey,char* passwd,char* mac,int selectServerIndex, char *ip)
#else
void lssproto_ClientLogin_send(int fd,char* cdkey,char* passwd)
#endif
{
	char buffer[16384];
	int iChecksum=0;
#ifdef _CHARTITLE_
	extern void 初始化称号结构();
	初始化称号结构();
#endif
#ifdef _CHARTITLE_STR_
	extern void 初始化称号结构();
	初始化称号结构();
#endif
#ifdef _RED_MEMOY_
	extern void RedMemoyInit();
	RedMemoyInit();
#endif
	buffer[0] = '\0';
	iChecksum += util_mkstring(buffer, cdkey);
	iChecksum += util_mkstring(buffer, passwd);
#ifdef _NEW_CLIENT_LOGIN
	iChecksum += util_mkstring(buffer, mac);
	iChecksum += util_mkint(buffer, selectServerIndex);
	iChecksum += util_mkstring(buffer, ip);
#endif
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_CLIENTLOGIN_SEND, buffer);

}

void lssproto_CreateNewChar_send(int fd,int dataplacenum,char* charname,int imgno,int faceimgno,int vital,int str,int tgh,int dex,int earth,int water,int fire,int wind,int hometown)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, dataplacenum);
	iChecksum += util_mkstring(buffer, charname);
	iChecksum += util_mkint(buffer, imgno);
	iChecksum += util_mkint(buffer, faceimgno);
	iChecksum += util_mkint(buffer, vital);
	iChecksum += util_mkint(buffer, str);
	iChecksum += util_mkint(buffer, tgh);
	iChecksum += util_mkint(buffer, dex);
	iChecksum += util_mkint(buffer, earth);
	iChecksum += util_mkint(buffer, water);
	iChecksum += util_mkint(buffer, fire);
	iChecksum += util_mkint(buffer, wind);
	iChecksum += util_mkint(buffer, hometown);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_CREATENEWCHAR_SEND, buffer);
}
extern STR_BUFFER passwd;
void lssproto_CharDelete_send(int fd,char* charname)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkstring(buffer, charname);
	iChecksum += util_mkstring(buffer, passwd.buffer);
	util_mkint(buffer, iChecksum);
	passwd.cnt = 0;
	passwd.cursor=0;
	ZeroMemory(passwd.buffer,sizeof(passwd.buffer));
	util_SendMesg(fd, LSSPROTO_CHARDELETE_SEND, buffer);
}

void lssproto_CharLogin_send(int fd,char* charname)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkstring(buffer, charname);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_CHARLOGIN_SEND, buffer);
}

void lssproto_CharList_send(int fd)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_CHARLIST_SEND, buffer);
}
#ifdef _PK2007
void lssproto_pkList_send(int fd)
{
	char buffer[16384];
	int iChecksum=0;
	buffer[0] = '\0';
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_PKLIST_SEND, buffer);
}
#endif
void lssproto_CharLogout_send(int fd, int Flg)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
#ifdef _CHAR_NEWLOGOUT
	iChecksum += util_mkint( buffer, Flg);
#endif
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_CHARLOGOUT_SEND, buffer);
}

void lssproto_ProcGet_send(int fd)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_PROCGET_SEND, buffer);
}

void lssproto_PlayerNumGet_send(int fd)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_PLAYERNUMGET_SEND, buffer);
}

void lssproto_Echo_send(int fd,char* test)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkstring(buffer, test);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_ECHO_SEND, buffer);
}

void lssproto_Shutdown_send(int fd,char* passwd,int min)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkstring(buffer, passwd);
	iChecksum += util_mkint(buffer, min);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_SHUTDOWN_SEND, buffer);
}

void lssproto_TD_send(int fd, char* data)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkstring(buffer, data);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_TD_SEND, buffer);
}
#ifdef _CHATROOMPROTOCOL			// (不可开) Syu ADD 聊天室频道
void lssproto_CHATROOM_send ( int fd , char* data )
{
	char buffer[16384] = {0};
	int iChecksum=0;

	// WON FIX	防止传送太频繁
	if( data[0] == 'B' ){
		unsigned int now_time = (unsigned int)time(NULL);
		static unsigned int old_time = 0;

		if( now_time < old_time ){
			SelRoomBtn = 1;	
			return;
		}
		old_time = now_time + 2; // delay 2 秒
	}

	iChecksum += util_mkstring(buffer, data);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_CHATROOM_SEND, buffer);
}
#endif

#ifdef _NEWREQUESTPROTOCOL			// (不可开) Syu ADD 新增Protocol要求细项
void lssproto_RESIST_send ( int fd , char* data )
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkstring(buffer, data);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_RESIST_SEND, buffer);
}
#endif

#ifdef _ALCHEPLUS
void lssproto_ALCHEPLUS_send ( int fd , char* data )
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkstring(buffer, data);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_ALCHEPLUS_SEND, buffer);
}
#endif

#ifdef _OUTOFBATTLESKILL			// (不可开) Syu ADD 非战斗时技能Protocol
void lssproto_BATTLESKILL_send (int fd, int SkillNum )
{
	char buffer[16384];
	int iChecksum=0;
	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, SkillNum);	
	util_mkint(buffer, iChecksum);	
	util_SendMesg(fd, LSSPROTO_BATTLESKILL_SEND, buffer);
}
#endif
// Robin 05/25
void lssproto_FM_send(int fd, char* data)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkstring(buffer, data);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_FM_SEND, buffer);
}

// shan 2002/01/10
void lssproto_PETST_send(int fd, int nPet, int sPet)// sPet  0:休息 1:等待 4:邮件
{
	//cary
	if( dwServer == GS){
		char buffer[16384];
	    int iChecksum=0;
	    memset(buffer,0,16384);
	    iChecksum += util_mkint(buffer, nPet);
	    iChecksum += util_mkint(buffer, sPet);	
	    util_mkint(buffer, iChecksum);	
		util_SendMesg(fd, LSSPROTO_PETST_SEND, buffer);
	}
}
//#endif

#ifdef _FIX_DEL_MAP     // WON ADD 玩家抽地图送监狱
void lssproto_DM_send(int fd)
{ 
	char buffer[2];
	buffer[0] = '\0';
	util_SendMesg(fd, LSSPROTO_DM_SEND, buffer);
}
#endif 

#ifdef _CHECK_GAMESPEED
void lssproto_CS_send( int fd)
{
	char buffer[2];
	buffer[0] = '\0';
	util_SendMesg(fd, LSSPROTO_CS_SEND, buffer);
}
#endif

void lssproto_BM_send(int fd, int iindex)
{
#ifdef _BLACK_MARKET
	char buffer[16384];
	int iChecksum=0;
	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, iindex);	
	util_mkint(buffer, iChecksum);	
	util_SendMesg(fd, LSSPROTO_BM_SEND, buffer);
#endif
}

#ifdef _MIND_ICON 
void lssproto_MA_send(int fd, int x, int y, int nMind)
{
	char buffer[16384];
	int iChecksum=0;
	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, nMind);	
	iChecksum += util_mkint(buffer, x);	
	iChecksum += util_mkint(buffer, y);	
	util_mkint(buffer, iChecksum);	
	util_SendMesg(fd, LSSPROTO_MA_SEND, buffer);
}
#endif

#ifdef _STREET_VENDOR
void lssproto_STREET_VENDOR_send(int fd,char *data)
{
	char buffer[16384];
	int iChecksum = 0;

	buffer[0] = '\0';
	iChecksum += util_mkstring(buffer, data);
	util_mkint(buffer,iChecksum);
	util_SendMesg(fd,LSSPROTO_STREET_VENDOR_SEND,buffer);
}
#endif

#ifdef _JOBDAILY
void lssproto_JOBDAILY_send(int fd,char *data)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkstring(buffer, data);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_JOBDAILY_SEND, buffer);
}
#endif
#ifdef _FAMILYBADGE_
void lssproto_FamilyBadge_send(int fd)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer, 1);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_FAMILYBADGE_SEND, buffer);
}

#endif
#ifdef _TEACHER_SYSTEM
void lssproto_TEACHER_SYSTEM_send(int fd,char *data)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkstring(buffer,data);
	util_mkint(buffer,iChecksum);
	util_SendMesg(fd,LSSPROTO_TEACHER_SYSTEM_SEND,buffer);
}
#endif
#ifdef _PET_ITEM
// 宠物装备功能
void lssproto_PetItemEquip_send(int fd, int iGx, int iGy, int iPetNo, int iItemNo, int iDestNO)
{
	char	szBuffer[16384];
	int		iChecksum=0;

	szBuffer[0] = '\0';
	iChecksum += util_mkint(szBuffer, iGx);
	iChecksum += util_mkint(szBuffer, iGy);
	iChecksum += util_mkint(szBuffer, iPetNo);
	iChecksum += util_mkint(szBuffer, iItemNo);
	iChecksum += util_mkint(szBuffer, iDestNO);
	util_mkint(szBuffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_PET_ITEM_EQUIP_SEND, szBuffer);
}
#endif
#ifdef _GET_HOSTNAME
void lssproto_HostName_send(int fd)
{
	char buffer[16384];
	int iChecksum=0;
	buffer[0] = '\0';
	char MyName[255];
	gethostname(MyName,255);
#ifdef _MACADDRESSD
	MAC_ADDR addrlist[3];
	memset(&addrlist,0,sizeof(addrlist));
	int macnum=GetMACaddress(addrlist,3);
    char MACNUM[16]="";
	sprintf_s(MACNUM,"%2X%2X%2X%2X%2X%2X",addrlist[0].addr[0],addrlist[0].addr[1],addrlist[0].addr[2],addrlist[0].addr[3],addrlist[0].addr[4],addrlist[0].addr[5]);
	for(i=0;i<(int)strlen(MACNUM);i++){
		if( MACNUM[i] == ' ' )
			MACNUM[i] = '0';
		MACNUM[i] ^= 0xff;//编个码
	}
	//for(i=0;i<macnum;i++)
		//PrintMACaddress(addrlist[i].addr);
#endif
	iChecksum += util_mkstring(buffer,MyName);
#ifdef _MACADDRESSD
	iChecksum += util_mkstring(buffer,MACNUM);
#endif
	util_mkint(buffer,iChecksum);
	util_SendMesg(fd,LSSPROTO_HOSTNAME_SEND,buffer);

}
#endif

#ifdef _NEW_SYSTEM_MENU
void lssproto_SaMenu_send(int fd, int index)
{
	char buffer[1024 * 64];
	int checksum = 0;
	strcpy(buffer,"");

	checksum += util_mkint(buffer,index);
	util_mkint(buffer,checksum);
	util_SendMesg(fd, LSSPROTO_SAMENU_SEND, buffer);
}
#endif



#ifdef _PETBLESS_
void lssproto_petbless_send(int fd ,int petpos,int type)
{
	char buffer[16384];
	int iChecksum=0;
	buffer[0] = '\0';
	iChecksum += util_mkint(buffer,petpos);
	iChecksum += util_mkint(buffer,type);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_VB_SEND, buffer);
}
#endif

#ifdef _RIDEQUERY_
void lssproto_RideQuery_send(int fd)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer,1458);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_RIDEQUERY_SEND, buffer);
}
#endif



#ifdef _CHARSIGNDAY_
void lssproto_SignDay_send(int fd)
{
	char buffer[16384];
	int iChecksum=0;

	buffer[0] = '\0';
	iChecksum += util_mkint(buffer,1458);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_SIGNDAY_SEND, buffer);
}
#endif

#ifdef _RED_MEMOY_
void lssproto_redMemoy_send(int fd,int index,char *str)
{
	char buffer[16384];
	int iChecksum=0;
	buffer[0] = '\0';

	iChecksum += util_mkint(buffer, index);
	iChecksum += util_mkstring(buffer, str);
	util_mkint(buffer, iChecksum);
	util_SendMesg(fd, LSSPROTO_REDMEMOY_SEND, buffer);
}
#endif


