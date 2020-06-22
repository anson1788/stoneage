#include <stdio.h>
#include <winsock.h>
#include <time.h>
#include <tchar.h>
#include "../systeminc/version.h"
#include"../systeminc/system.h"
#include "../systeminc/netmain.h"
#include "../systeminc/netproc.h"
#include "../systeminc/lssproto_cli.h"
#include "../systeminc/savedata.h"
#include "../systeminc/process.h"
#include "../systeminc/pc.h"
#include "../systeminc/tool.h"
#include "../systeminc/map.h"
#include "../systeminc/character.h"
#include "../systeminc/action.h"
#include "../systeminc/battlemenu.h"
#include "../systeminc/battleProc.h"
#include "../systeminc/battleMap.h"
#include "../systeminc/menu.h"
#include "../systeminc/anim_tbl.h"
#include "../systeminc/login.h"
#include "../systeminc/handletime.h"
#include "../systeminc/field.h"
#include "../systeminc/t_music.h"
#include "../systeminc/battleMenu.h"
#include "../wgs/message.h"
#include "../wgs/descrypt.h"
#include "./test.h"
#include "../NewProto/autil.h"
#ifdef _REMAKE_20
#include "../MMOGprotect.h"
#endif
#ifdef _AIDENGLU_
#include "../systeminc/pc.h"
extern Landed PcLanded;
#endif

#ifdef _TALK_WINDOW
#include "../systeminc/TalkWindow.h"
#endif
#include "../openssl/md5.h"
#include "../systeminc/VMProtectSDK.h"
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")


#ifdef _STONDEBUG_
extern int g_iMallocCount;
#endif
#define NETPROC_NOTSEND     0           // ??????????
#define NETPROC_SENDING     1           // ??????
#define NETPROC_RECEIVED    2           // ???
#define SET_SENDTIME(time)	time = TimeGetTime()
#define CHECK_TIMEOUT(time, timeout) \
	if ((TimeGetTime() - (time)) > (timeout)) \
	return -1;
#define SETSENDING  netproc_sending = NETPROC_SENDING; start_time = TimeGetTime();
#define SETTIMEOUT(msg, state) \
	if ((TimeGetTime() - start_time) > TIMEOUT)	\
	return;
#define SETTIMEOUT2(msg) \
	if ((TimeGetTime() - start_time) > TIMEOUT)	\
{											\
	sprintf_s( netprocErrmsg, msg );			\
	return -1;								\
}
#define SETNEEDCHOOSE(mode)	\
	if (server_choosed == 0) \
	return;
#ifdef _STONDEBUG_
void sendDataToServer(char* data);
#endif

extern char szUser[], szPassword[];

short selectServerIndex = -1;
short clientLoginStatus = 0;
short charListStatus = 0;

short charDelStatus = 0;
short newCharStatus = 0;
short charLoginStatus = 0;
short charLogoutStatus = 0;

#ifdef _MAILSHOWPLANET				// (可开放) Syu ADD 显示名片星球
extern struct gameserver gmsv[];
#endif

#ifdef _NEW_WGS_MSG				// WON ADD WGS的新视窗
int ERROR_MESSAGE = 0;
#endif

// GAMESTATE_CHOOSESERVER?????????????⿳?
char gamestate_chooseserver_name[128];
// GAMESTATE_LOGIN?????????????⿳?
char gamestate_login_charname[128];
// GAMESTATE_DELETECHAR?????????????⿳?
char gamestate_deletechar_charname[128];
char netprocErrmsg[1024];
int netproc_sending = NETPROC_NOTSEND; 
DWORD start_time = 0;
BOOL bNewServer;
char c_temp[1024];
int connectServerCounter = 0;

#ifdef _SKILLSORT
int AdvanceSkill[20] = {-1};
int AssitSkill[20] = {-1};
int BattleSkill[20] = {-1};
void SortSkill()
{
	int count1 = 0, count2 = 0, count3 = 0;

	for (int i = 0; i < 20; i++)
	{
		AdvanceSkill[i] = -1;
		AssitSkill[i] = -1;		
		BattleSkill[i] = -1;
	}
#ifdef _PRO3_ADDSKILL
	for (int i = 0; i < 30; i++)
#else
	for (int i = 0; i < 26; i++)
#endif
	{
		switch (profession_skill[i].kind)
		{
		case 1: // 战斗技能
			BattleSkill[count1] = i;
			count1++;
			break;
		case 2: // 辅助
			AssitSkill[count2] = i;
			count2++;
			break;
		case 3: // 进阶
			AdvanceSkill[count3] = i;
			count3++;
			break;
		}
	}
}
#endif

#ifdef _CHANNEL_MODIFY
// 储存对话内容
void SaveChatData(char *msg,char KindOfChannel,bool bCloseFile);
#endif
void initConnectServer(void)
{
	connectServerCounter = 0;
}

void LogToRecvdata2(char *data)
{
	char lssproto_readlogfilename[256] = "recvdata.txt";

	if (lssproto_readlogfilename[0] != '\0')
	{
		FILE *rfp;
		rfp = fopen(lssproto_readlogfilename, "a+");
		if (rfp)
		{
			fprintf(rfp, "收到：\t%s\n", data);
			fclose(rfp);
		}
	}
}

extern int	iEncrypt;
int ConnectWGS()
{
	if (connectServerCounter == 0)
	{
		HANDLE hFileMapping = NULL;
		LPSTR lpstr = NULL;

		cleanupNetwork();
		if (initNet())
		{
			unsigned long flg = 1;
			struct sockaddr_in sin;
			struct hostent *h;

			iWGS = 0;
			iEncrypt = E_INIT;
			start_time = TimeGetTime();
			if (INVALID_SOCKET == (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
			{
#ifdef _AIDENGLU_
				PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
				sprintf_s(netprocErrmsg, NET_ERRMSG_SOCKETERROR);
				return -3;
			}
			ioctlsocket(sockfd, FIONBIO, &flg);
			sin.sin_family = AF_INET;
			sin.sin_port = htons(wWGS);
			sin.sin_addr.s_addr = inet_addr(szWGS);
			if (sin.sin_addr.s_addr == -1)
			{
				if (!(h = gethostbyname(szWGS)))
				{
#ifdef _AIDENGLU_
					PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
					sprintf_s(netprocErrmsg, NET_ERRMSG_NOTGETADDR);
					closesocket(sockfd);
					dwServer = NULL;
					return -4;
				}
				memcpy((void*)&sin.sin_addr.s_addr, h->h_addr, sizeof(struct in_addr));
			}
			int ret = connect(sockfd, (struct sockaddr*)&sin, sizeof(sin));
			if (ret == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					closesocket(sockfd);
					dwServer = NULL;
#ifdef _AIDENGLU_
					PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
					sprintf_s(netprocErrmsg, NET_ERRMSG_NOTCONNECT_S);
					return -5;
				}
			}
			dwServer = WGS;
			iWGS = 1;
			connectServerCounter = 1;
		}
		else
		{
#ifdef _AIDENGLU_
			PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
			strcpy(netprocErrmsg, NET_ERRMSG_SOCKLIBERROR);
			return -1;
		}
	}
	else if (connectServerCounter >= 1 && connectServerCounter <= 70)
	{
		fd_set rfds, wfds, efds;
		struct timeval tm;

		if (++connectServerCounter == 70)
			connectServerCounter = 69;
		tm.tv_sec = 0;
		tm.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_ZERO(&efds);
		FD_SET(sockfd, &rfds);
		FD_SET(sockfd, &wfds);
		FD_SET(sockfd, &efds);
		int a = select(2, &rfds, &wfds, &efds, &tm);
		if (FD_ISSET(sockfd, &wfds))
		{
			connectServerCounter = 71;
			server_choosed = 100;
		}
#ifdef _OMIT_WGS
		testtest();
		connectServerCounter = 143;   // 暂定
		iWGS = 7;
#endif
		if (FD_ISSET(sockfd, &efds))
		{
#ifdef _AIDENGLU_
			PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
			sprintf_s(netprocErrmsg, NET_ERRMSG_NOTCONNECT);
			closesocket(sockfd);
			dwServer = NULL;
			return -6;
		}
	}
	else if (connectServerCounter == 71)
	{
		if (iWGS == 2)
		{
			unsigned long flg = 1;
			struct sockaddr_in sin;
			struct hostent *h;

			closesocket(sockfd);
			dwServer = NULL;
			if (INVALID_SOCKET == (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
			{
#ifdef _AIDENGLU_
				PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
				sprintf_s(netprocErrmsg, NET_ERRMSG_SOCKETERROR);
				return -3;
			}
			ioctlsocket(sockfd, FIONBIO, &flg);
			sin.sin_family = AF_INET;
			sin.sin_port = htons(wCS);
			sin.sin_addr.s_addr = inet_addr(szCSIP);
			if (sin.sin_addr.s_addr == -1)
			{
				if (!(h = gethostbyname(szCSIP)))
				{
#ifdef _AIDENGLU_
					PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
					sprintf_s(netprocErrmsg, NET_ERRMSG_NOTGETADDR);
					closesocket(sockfd);
					dwServer = NULL;
					return -4;
				}
				memcpy((void*)&sin.sin_addr.s_addr, h->h_addr, sizeof(struct in_addr));
			}
			int ret = connect(sockfd, (struct sockaddr*)&sin, sizeof(sin));
			if (ret == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					closesocket(sockfd);
					dwServer = NULL;
#ifdef _AIDENGLU_
					PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
					sprintf_s(netprocErrmsg, NET_ERRMSG_NOTCONNECT_S);
					return -5;
				}
			}
			dwServer = CS;
			iWGS = 3;
			connectServerCounter = 72;
		}
	}
	else if (connectServerCounter >= 72 && connectServerCounter <= 140)
	{
		fd_set rfds, wfds, efds;
		struct timeval tm;

		if (++connectServerCounter == 140)
			connectServerCounter = 139;
		tm.tv_sec = 0;
		tm.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_ZERO(&efds);
		FD_SET(sockfd, &rfds);
		FD_SET(sockfd, &wfds);
		FD_SET(sockfd, &efds);
		int a = select(2, &rfds, &wfds, &efds, &tm);
		if (FD_ISSET(sockfd, &wfds))
		{
			connectServerCounter = 141;
			server_choosed = 100;
		}
		if (FD_ISSET(sockfd, &efds))
		{
#ifdef _AIDENGLU_
			PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
			sprintf_s(netprocErrmsg, NET_ERRMSG_NOTCONNECT);
			closesocket(sockfd);
			dwServer = NULL;
			return -6;
		}
	}
	else if (connectServerCounter == 141)
	{
		if (iWGS == 4)
		{
			DoSKey();
			connectServerCounter = 142;
			iWGS = 5;
		}
	}
	else if (connectServerCounter == 142)
	{
		if (iWGS == 6)
		{
			DoHellow();
			connectServerCounter = 143;
			iWGS = 7;
		}
	}
	else if (connectServerCounter == 143)
	{
		if (iWGS == 8)
		{
			cleanupNetwork();
			disconnectServerFlag = TRUE;
			oldDisconnectServerFlag = TRUE;
			closesocket(sockfd);
			dwServer = GS;
			return 1;
		}
	}
	if (iWGS == 9)
		return -7;
	SETTIMEOUT2(NET_ERRMSG_CONNECTTIMEOUT);
	return 0;
}

struct sockaddr_in sin_server;
int connectServer(void)
{
	if (!init_net)
		return 0;
	static int count = 0;
	if (connectServerCounter == 0)
	{
		char hostname[128];
		short pt;
		unsigned long flg = 1;
		struct hostent *h;
#ifdef _VMP_
		lstrcpy(PersonalKey, VMProtectDecryptStringA(_DEFAULT_PKEY));
#else
		lstrcpy(PersonalKey, _DEFAULT_PKEY);
#endif
		//		lstrcpy(PersonalKey, "forever");
		start_time = TimeGetTime();
		if (getServerInfo(selectServerIndex, hostname, &pt) < 0)
		{
			count = 0;
#ifdef _AIDENGLU_
			PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
			sprintf_s(netprocErrmsg, NET_ERRMSG_BADNAME);
			return -2;
		}
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd == INVALID_SOCKET)
		{
			count = 0;
#ifdef _AIDENGLU_
			PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
			sprintf_s(netprocErrmsg, NET_ERRMSG_SOCKETERROR);
			return -3;
		}
		ioctlsocket(sockfd, FIONBIO, &flg);

		extern BOOL NoDelay;
		if (NoDelay)
		{
			int flag = 1;

			if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int)) != 0)
			{
				count = 0;
				return -100;
			}
		}
		ZeroMemory(&sin_server, sizeof(sin_server));
		sin_server.sin_family = AF_INET;
		sin_server.sin_port = htons(pt);
		sin_server.sin_addr.s_addr = inet_addr(hostname);     /* accept only dot notaion  */
		if (sin_server.sin_addr.s_addr == -1)
		{
			h = gethostbyname(hostname);
			if (h == NULL)
			{
				count = 0;
#ifdef _AIDENGLU_
				PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
				sprintf_s(netprocErrmsg, NET_ERRMSG_NOTGETADDR);
				closesocket(sockfd); 
				dwServer = NULL;
				return -4;
			}
			memcpy((void*)&sin_server.sin_addr.s_addr, h->h_addr, sizeof(struct in_addr));
		}
		// Non blocking Connect
		int ret = connect(sockfd, (struct sockaddr*)&sin_server, sizeof(sin_server));
		if (ret == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				count = 0;
				closesocket(sockfd);
				dwServer = NULL;
#ifdef _AIDENGLU_
				PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
				sprintf_s(netprocErrmsg, NET_ERRMSG_NOTCONNECT_S);
				return -5;
			}
			connectServerCounter = 1;
			c_temp[0] = 0;
			bNewServer = FALSE;
			/*sprintf_s(netprocErrmsg, NET_ERRMSG_NOTCONNECT);
			closesocket(sockfd);
			dwServer = NULL;
			return -6; */
		}
	}
	else if (connectServerCounter >= 1 && connectServerCounter <= 70)
	{
		fd_set rfds, wfds, efds;
		struct timeval tm;

		connectServerCounter++;
		if (connectServerCounter == 70)
			connectServerCounter = 69;
		// select??
		tm.tv_sec = 0;
		tm.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_ZERO(&efds);
		FD_SET(sockfd, &rfds);
		FD_SET(sockfd, &wfds);
		FD_SET(sockfd, &efds);
		int a = select(sockfd, &rfds, &wfds, &efds, &tm);
		if (a > 0)
		{
			if (FD_ISSET(sockfd, &rfds))
			{
				// Nuke start
				int i_len = recv(sockfd, c_temp, sizeof(c_temp) - 1, 0);

				if (i_len > 0)
				{
					if (c_temp[0] == 'E')
					{
#ifdef _AIDENGLU_
						PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
						count = 0;
						sprintf_s(netprocErrmsg, c_temp + 1);
						closesocket(sockfd);
						dwServer = NULL;
						return -6;
					}
					else if (c_temp[0] == _SA_VERSION)
					{
						bNewServer = 0xf000000 | 1;
						if (FD_ISSET(sockfd, &wfds))
						{
							connectServerCounter = 71;						
							server_choosed = 1;
						}
					}
					else
					{
#ifdef _AIDENGLU_
						PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
						count = 0;
						sprintf_s(netprocErrmsg, NET_ERRMSG_VERSIONERROR);
						closesocket(sockfd);
						dwServer = NULL;
						return -8; 
					}
				}else
				{
#ifdef _AIDENGLU_
					PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
					count = 0;
					sprintf_s(netprocErrmsg, NET_ERRMSG_NOTCONNECT);
					closesocket(sockfd);
					dwServer = NULL;
					return -6; 
				}
			}
			//end cary
			if (FD_ISSET(sockfd, &efds))
			{
#ifdef _AIDENGLU_
				PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
				count = 0;
				sprintf_s(netprocErrmsg, NET_ERRMSG_NOTCONNECT);
				closesocket(sockfd);
				dwServer = NULL;
				return -6; 
			}
		}else
		{
			count++;

			if(count >= 1500)
			{
#ifdef _AIDENGLU_
				PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
				count = 0;
				sprintf_s(netprocErrmsg, NET_ERRMSG_NOTCONNECT);
				closesocket(sockfd);
				dwServer = NULL;
				return -6; 
			}
		}
	}
	else if (connectServerCounter >= 71 && connectServerCounter <= 80)
	{
		count = 0;
		if (connectServerCounter == 71)
		{
			char userId[32], userPassword[32];

			clientLoginStatus = 0;
			CopyMemory(userId, szUser, 32);
			ecb_crypt("f;encor1c", userId, 32, DES_DECRYPT);
			CopyMemory(userPassword, szPassword, 32);
			ecb_crypt("f;encor1c", userPassword, 32, DES_DECRYPT);

			extern char 机器数据[];
			unsigned char tmp[256];
			CHAR mac[64];
			MD5( (const unsigned char*)机器数据, strlen(机器数据), tmp );
			size_t leng = 0;
			for (int i = 0; i < 16; i ++)
			{
				sprintf( &mac[ leng ], "%02x", tmp[ i ] );
				leng = strlen( mac );
			}
#ifdef _NEW_CLIENT_LOGIN
#ifdef _SA_MAC_VERSION_CONTROL
			CHAR token[64];
			sprintf_s(token, "%s-%s",_SA_MAC_VERSION, mac);
#ifdef _LOGIP_
			extern char 玩家公网IP[];
			lssproto_ClientLogin_send(sockfd, userId,userPassword, token, selectServerIndex,玩家公网IP);
#else
			lssproto_ClientLogin_send(sockfd, userId,userPassword, token, selectServerIndex,"192.168.1.1");
#endif
#else
			lssproto_ClientLogin_send(sockfd, userId,userPassword, mac, selectServerIndex,"192.168.1.1");
#endif
#endif
			if ((bNewServer & 0xf000000) == 0xf000000)
			{
				lstrcpy(PersonalKey, userId);
#ifdef _VMP_
				lstrcat(PersonalKey, VMProtectDecryptStringA(_RUNNING_KEY));
#else
				lstrcat(PersonalKey, _RUNNING_KEY);
#endif
				//				lstrcat(PersonalKey, "520999");
			}
			else
			{
				lstrcpy(PersonalKey, userId);
				lstrcat(PersonalKey, "19761101");
			}
			netproc_sending = NETPROC_SENDING;
		}
		if (netproc_sending == NETPROC_RECEIVED)
		{
			//  recv ?????
			if (clientLoginStatus)
				connectServerCounter = 81;
			else
			{
#ifdef _AIDENGLU_
				PcLanded.登陆延时时间 = TimeGetTime() + 2000;
#endif
				count = 0;
				netproc_sending = NETPROC_NOTSEND;
				sprintf_s(netprocErrmsg, NET_ERRMSG_LOGINFAIL);
				closesocket(sockfd);
				dwServer = NULL;
				return -7;
			}
		}
		connectServerCounter++;
		if (connectServerCounter == 81)
			connectServerCounter = 80;
	}
	else if (connectServerCounter >= 81 && connectServerCounter <= 98)
		connectServerCounter ++;
	else if (connectServerCounter == 99)
	{
		netproc_sending = NETPROC_NOTSEND;
		return 1;
	}
	SETTIMEOUT2(NET_ERRMSG_CONNECTTIMEOUT);
	return 0;
}
void lssproto_ClientLogin_recv(int fd, char *result)
{
	if (netproc_sending == NETPROC_SENDING)
	{
		netproc_sending = NETPROC_RECEIVED;	
		if (strcmp(result, OKSTR) == 0)
		{
			clientLoginStatus = 1;
			time(&serverAliveLongTime);
			localtime_s(&serverAliveTime,&serverAliveLongTime);
		}else if(strcmp (result, CANCLE) == 0)
		{
			//ChangeProc(PROC_TITLE_MENU , 6 );

			cleanupNetwork();
			// ????????????
			PaletteChange(DEF_PAL, 0);
			// ?????䴘?
			//cary
			ChangeProc( PROC_ID_PASSWORD );
			SubProcNo = 5;
			// ??????
			DeathAllAction();

		}
	}
}

void charListStart(void)
{
	int i;

	for (i = 0; i < MAXCHARACTER; i++)
		resetCharacterList(i);
#ifdef _MORECHARACTERS_
	extern int 多人物当前页数;
	多人物当前页数=0;
#endif
	charListStatus = 0;
	if (bNewServer)
		lssproto_CharList_send(sockfd);
	else
		old_lssproto_CharList_send(sockfd);
	SETSENDING;
}

int charListProc(void)
{
	if (netproc_sending == NETPROC_RECEIVED)
	{
		netproc_sending = NETPROC_NOTSEND;
		if (charListStatus == 1)
			return 1;
		else if (charListStatus == 2)
			return -3;
#ifdef _CHANGEGALAXY
		else if (charListStatus == 3)
			return -12;
#endif
#ifdef _ERROR301
		else if (charListStatus == 4)
			return -13;
#endif
		else
			return -2;
	}
	SETTIMEOUT2(NET_ERRMSG_CHARLISTTIMEOUT);
	return 0;
}


TCHAR 登陆错误内容[1024];

void lssproto_CharList_recv(int fd, char *result, char *data)
{
	memset(登陆错误内容,0,1024);
	if(strcmp(result,_T("failed"))==0) {
		strcpy(登陆错误内容,data);
#ifdef _AIDENGLU_
		PcLanded.登陆延时时间 = TimeGetTime()+2000;
#endif
	}
	if (netproc_sending == NETPROC_SENDING)
	{
		char nm[1024], opt[1024];
		int i;

		netproc_sending = NETPROC_RECEIVED;
		if (strcmp(result, SUCCESSFULSTR) != 0)
		{
			if (strcmp(data, "OUTOFSERVICE") == 0)
				charListStatus = 2;
#ifdef _CHANGEGALAXY
			if (strcmp(data, "CHANGE_GALAXY") == 0)
				charListStatus = 3;
#endif
#ifdef _ERROR301
			if (strcmp(data, "301") == 0)
				charListStatus = 4;
#endif
			return;
		}
		charListStatus = 1;
		for (i = 0; i < MAXCHARACTER; i++)
		{
			strcpy(nm, "");
			strcpy(opt, "");
			getStringToken(data, '|', i * 2 + 1, sizeof(nm) - 1 , nm);
			getStringToken(data, '|', i * 2 + 2, sizeof(opt) - 1, opt);
			setCharacterList(nm, opt);
		}
	}
}

void charLoginStart(void)
{
	charLoginStatus = 0;
#ifdef _NEW_WGS_MSG				// WON ADD WGS的新视窗
	ERROR_MESSAGE = 0;
#endif
	if (bNewServer){
#ifdef _TRADITIONAL_LONG_
		CHAR szOutBuffer[128+1] = {0};  
		WORD wLanguageID = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED); 
		LCID Locale = MAKELCID(wLanguageID, SORT_CHINESE_PRCP);
		int iRet = LCMapString(Locale,  
			LCMAP_SIMPLIFIED_CHINESE,  
			gamestate_login_charname, -1,  
			szOutBuffer, 128);
		lssproto_CharLogin_send(sockfd, szOutBuffer);
#else
		lssproto_CharLogin_send(sockfd, gamestate_login_charname);
#endif
	}
	else
		old_lssproto_CharLogin_send(sockfd, gamestate_login_charname);
	SETSENDING;
}

int charLoginProc(void)
{
	tradeStatus = 0;
	if (!server_choosed)
		return 0;
	if (netproc_sending == NETPROC_RECEIVED)
	{
		netproc_sending = NETPROC_NOTSEND;
		if (charLoginStatus == 1)
			return 1;
		else

#ifdef _NEW_WGS_MSG				// WON ADD WGS的新视窗
			if (ERROR_MESSAGE != 0)
				return ERROR_MESSAGE;
			else
#endif
				return -2;
	}
	SETTIMEOUT2(NET_ERRMSG_LOGINTIMEOUT);
	return 0;
}

#ifdef __NEW_CLIENT
HANDLE hPing = NULL;
DWORD dwPingID;
DWORD WINAPI PingFunc(LPVOID param);
#endif

void lssproto_CharLogin_recv(int fd, char* result, char* data)
{
	if (netproc_sending == NETPROC_SENDING)
	{
		netproc_sending = NETPROC_RECEIVED;
#ifdef __NEW_CLIENT
		if (strcmp(result, SUCCESSFULSTR) == 0 && !hPing)
#else
		if (strcmp(result, SUCCESSFULSTR) == 0)
#endif
		{
			charLoginStatus = 1;
#ifdef __NEW_CLIENT
			hPing = CreateThread(NULL, 0, PingFunc, &sin_server.sin_addr, 0, &dwPingID);
#endif
		}

#ifdef __NEW_CLIENT
#ifdef _NEW_WGS_MSG				// WON ADD WGS的新视窗
		if (strcmp(result, "failed" ) == 0 && !hPing)
			ERROR_MESSAGE = atoi(data);
#endif
#endif
#ifdef _ANGEL_SUMMON
		angelFlag = FALSE;
		angelMsg[0] = NULL;
#endif
	}
}

void charLogoutStart(void)
{
	charLogoutStatus = 0;
	if (bNewServer)
		lssproto_CharLogout_send(sockfd, 1);
	else
		old_lssproto_CharLogout_send(sockfd);
	SETSENDING;
}

int charLogoutProc(void)
{
	if (netproc_sending == NETPROC_RECEIVED)
	{
		netproc_sending = NETPROC_NOTSEND;
		if (charLogoutStatus == 1)
			return 1;
		else
			return -2;
    }
	SETTIMEOUT2(NET_ERRMSG_LOGOUTTIMEOUT);
	return 0;
}

void lssproto_CharLogout_recv(int fd, char *result, char *data)
{
	if (netproc_sending == NETPROC_SENDING)
	{
		netproc_sending = NETPROC_RECEIVED;
		if (strcmp(result, SUCCESSFULSTR) == 0)
			charLogoutStatus = 1;
	}
}

#define S_DELIM '|'

/*================================
C warp 用
D 修正时间
X 骑宠
P 人物状态
F 家族状态
M HP,MP,EXP
K 宠物状态
E nowEncountPercentage
J 魔法
N 队伍资讯
I 道具
W 宠物技能
S 职业技能
G 职业技能冷却时间
================================*/
void lssproto_S_recv(int fd, char *data)
{
	if (logOutFlag)
		return;
	switch (data[0])
	{
		case 'C':
		{
			int fl, maxx, maxy, gx, gy;

			floorChangeFlag = TRUE;
			if (!loginFlag && ProcNo == PROC_GAME)
			{
				if (!warpEffectFlag)
				{
					SubProcNo = 200;
					warpEffectProc();
					if (MenuToggleFlag & JOY_CTRL_M)
						MapWmdFlagBak = TRUE;
				}
				resetPc();
				warpEffectFlag = FALSE;
				warpEffectStart = TRUE;
			}
			data++;
			fl   = getIntegerToken(data, S_DELIM, 1);
			maxx = getIntegerToken(data, S_DELIM, 2);
			maxy = getIntegerToken(data, S_DELIM, 3);
			gx   = getIntegerToken(data, S_DELIM, 4);
			gy   = getIntegerToken(data, S_DELIM, 5);
			setMap(fl, gx, gy);
			createMap(fl, maxx, maxy);
			nowFloorGxSize = maxx;
			nowFloorGySize = maxy;
			resetCharObj();
			mapEmptyFlag = FALSE;
			nowEncountPercentage = minEncountPercentage;
			nowEncountExtra = 0;
			resetMap();
			transmigrationEffectFlag = 0;
#ifdef __SKYISLAND
			extern void SkyIslandSetNo( int fl);
			SkyIslandSetNo( fl);
#endif
            break;
		}
		case 'D':
			data++;
			setPcId(getIntegerToken(data, S_DELIM, 1));
			serverTime = getIntegerToken(data, S_DELIM, 2);
			FirstTime = TimeGetTime();
			RealTimeToSATime(&SaTime);
			SaTimeZoneNo = getLSTime(&SaTime);
			PaletteChange(SaTimeZoneNo, 0);
			break;
		//andy_add
		case 'X':
			pc.lowsride = getIntegerToken(data, S_DELIM, 2);
			break;
		case 'P':
			{
				char name[256], freeName[256];
				int i, kubun;
				unsigned int mask;

				data++;
				kubun = getInteger62Token(data, S_DELIM, 1);
				if (!bNewServer)
					pc.ridePetNo = -1;
				if (kubun == 1)
				{
					pc.hp		= getIntegerToken(data, S_DELIM, 2);		// 0x00000002
					pc.maxHp	= getIntegerToken(data, S_DELIM, 3);		// 0x00000004
					pc.mp		= getIntegerToken(data, S_DELIM, 4);		// 0x00000008
					pc.maxMp	= getIntegerToken(data, S_DELIM, 5);		// 0x00000010
					pc.vital	= getIntegerToken(data, S_DELIM, 6);		// 0x00000020
					pc.str		= getIntegerToken(data, S_DELIM, 7);		// 0x00000040
					pc.tgh		= getIntegerToken(data, S_DELIM, 8);		// 0x00000080
					pc.dex		= getIntegerToken(data, S_DELIM, 9);		// 0x00000100
					pc.exp		= getIntegerToken(data, S_DELIM, 10);		// 0x00000200
					pc.maxExp	= getIntegerToken(data, S_DELIM, 11);		// 0x00000400
					pc.level	= getIntegerToken(data, S_DELIM, 12);		// 0x00000800
					pc.atk		= getIntegerToken(data, S_DELIM, 13);		// 0x00001000
					pc.def		= getIntegerToken(data, S_DELIM, 14);		// 0x00002000
					pc.quick	= getIntegerToken(data, S_DELIM, 15);		// 0x00004000
					pc.charm	= getIntegerToken(data, S_DELIM, 16);		// 0x00008000
					pc.luck		= getIntegerToken(data, S_DELIM, 17);		// 0x00010000
					pc.earth	= getIntegerToken(data, S_DELIM, 18);		// 0x00020000
					pc.water	= getIntegerToken(data, S_DELIM, 19);		// 0x00040000
					pc.fire		= getIntegerToken(data, S_DELIM, 20);		// 0x00080000
					pc.wind		= getIntegerToken(data, S_DELIM, 21);		// 0x00100000
					pc.gold		= getIntegerToken(data, S_DELIM, 22);		// 0x00200000
					pc.titleNo	= getIntegerToken(data, S_DELIM, 23);		// 0x00400000
					pc.dp		= getIntegerToken(data, S_DELIM, 24);		// 0x00800000
					pc.transmigration = getIntegerToken(data, S_DELIM, 25);// 0x01000000
					pc.ridePetNo = getIntegerToken(data, S_DELIM, 26);	// 0x02000000
					pc.learnride = getIntegerToken(data, S_DELIM, 27);	// 0x04000000
					pc.baseGraNo = getIntegerToken(data, S_DELIM, 28);	// 0x08000000
#ifdef _NEW_RIDEPETS
					pc.lowsride = getIntegerToken(data, S_DELIM, 29);		// 0x08000000
#endif
#ifdef _SFUMATO
					pc.sfumato = 0xff0000;	
#endif
					getStringToken(data, S_DELIM, 30, sizeof(name) - 1, name);
					makeStringFromEscaped(name);
					if (strlen(name) <= CHAR_NAME_LEN)
						strcpy_s(pc.name, name);
					getStringToken(data, S_DELIM, 31, sizeof(freeName) - 1, freeName);
					makeStringFromEscaped(freeName);
					if (strlen(freeName) <= CHAR_FREENAME_LEN)
						strcpy_s(pc.freeName, freeName);
#ifdef _NEW_ITEM_
					pc.道具栏状态 = getIntegerToken(data, S_DELIM, 32);
#endif
#ifdef _SA_VERSION_25
					int pointindex = getIntegerToken(data, S_DELIM, 33);
					char pontname[][32]={
						"萨姆吉尔村",
						"玛丽娜丝村",
						"加加村",
						"卡鲁它那村",
					};
					sprintf(pc.chusheng,"%s",pontname[pointindex]);
#ifdef _MAGIC_ITEM_
					pc.法宝道具状态 = getIntegerToken(data, S_DELIM, 34);
					pc.道具光环效果 = getIntegerToken(data, S_DELIM, 35);
#endif
#endif

				}
				else
				{
					mask = 2;
					i = 2;
					for (; mask > 0; mask <<= 1)
					{
						if (kubun & mask)
						{
							if (mask == 0x00000002) // ( 1 << 1 )
							{
								pc.hp = getIntegerToken(data, S_DELIM, i);// 0x00000002
								i++;
							}
							else if (mask == 0x00000004) // ( 1 << 2 )
							{
								pc.maxHp = getIntegerToken(data, S_DELIM, i);// 0x00000004
								i++;
							}
							else if (mask == 0x00000008)
							{
								pc.mp = getIntegerToken(data, S_DELIM, i);// 0x00000008
								i++;
							}
							else if (mask == 0x00000010)
							{
								pc.maxMp = getIntegerToken(data, S_DELIM, i);// 0x00000010
								i++;
							}
							else if (mask == 0x00000020)
							{
								pc.vital = getIntegerToken(data, S_DELIM, i);// 0x00000020
								i++;
							}
							else if (mask == 0x00000040)
							{
								pc.str = getIntegerToken(data, S_DELIM, i);// 0x00000040
								i++;
							}
							else if (mask == 0x00000080)
							{
								pc.tgh = getIntegerToken(data, S_DELIM, i);// 0x00000080
								i++;
							}
							else if (mask == 0x00000100)
							{
								pc.dex = getIntegerToken(data, S_DELIM, i);// 0x00000100
								i++;
							}
							else if (mask == 0x00000200)
							{
								pc.exp = getIntegerToken(data, S_DELIM, i);// 0x00000200
								i++;
							}
							else if (mask == 0x00000400)
							{
								pc.maxExp = getIntegerToken(data, S_DELIM, i);// 0x00000400
								i++;
							}
							else if (mask == 0x00000800)
							{
								pc.level = getIntegerToken(data, S_DELIM, i);// 0x00000800
								i++;
							}
							else if (mask == 0x00001000)
							{
								pc.atk = getIntegerToken(data, S_DELIM, i);// 0x00001000
								i++;
							}
							else if (mask == 0x00002000)
							{
								pc.def = getIntegerToken(data, S_DELIM, i);// 0x00002000
								i++;
							}
							else if (mask == 0x00004000)
							{
								pc.quick = getIntegerToken(data, S_DELIM, i);// 0x00004000
								i++;
							}
							else if (mask == 0x00008000)
							{
								pc.charm = getIntegerToken(data, S_DELIM, i);// 0x00008000
								i++;
							}
							else if (mask == 0x00010000)
							{
								pc.luck = getIntegerToken(data, S_DELIM, i);// 0x00010000
								i++;
							}
							else if (mask == 0x00020000)
							{
								pc.earth = getIntegerToken(data, S_DELIM, i);// 0x00020000
								i++;
							}
							else if (mask == 0x00040000)
							{
								pc.water = getIntegerToken(data, S_DELIM, i);// 0x00040000
								i++;
							}
							else if (mask == 0x00080000)
							{
								pc.fire = getIntegerToken(data, S_DELIM, i);// 0x00080000
								i++;
							}
							else if (mask == 0x00100000)
							{
								pc.wind = getIntegerToken(data, S_DELIM, i);// 0x00100000
								i++;
							}
							else if (mask == 0x00200000)
							{
								pc.gold = getIntegerToken(data, S_DELIM, i);// 0x00200000
								i++;
							}
							else if (mask == 0x00400000)
							{
								pc.titleNo = getIntegerToken(data, S_DELIM, i);// 0x00400000
								i++;
							}
							else if (mask == 0x00800000)
							{
								pc.dp = getIntegerToken(data, S_DELIM, i);// 0x00800000
								i++;
							}
							else if (mask == 0x01000000)
							{
								pc.transmigration = getIntegerToken(data, S_DELIM, i);// 0x01000000
								i++;
							}
							else if (mask == 0x02000000)
							{
								getStringToken(data, S_DELIM, i, sizeof(name) - 1, name);// 0x01000000
								makeStringFromEscaped(name);
								if (strlen(name) <= CHAR_NAME_LEN)
									strcpy_s(pc.name, name);
								i++;
							}
							else if (mask == 0x04000000)
							{
								getStringToken(data, S_DELIM, i, sizeof(freeName) - 1, freeName);// 0x02000000
								makeStringFromEscaped(freeName);
								if (strlen(freeName) <= CHAR_FREENAME_LEN)
									strcpy_s(pc.freeName, freeName);
								i++;
							}
							else if (mask == 0x08000000) // ( 1 << 27 )
							{
								pc.ridePetNo = getIntegerToken(data, S_DELIM, i);// 0x08000000
								i++;
							}
							else if (mask == 0x10000000) // ( 1 << 28 )
							{
								pc.learnride = getIntegerToken(data, S_DELIM, i);// 0x10000000
								i++;
							}
							else if (mask == 0x20000000) // ( 1 << 29 )
							{
								pc.baseGraNo = getIntegerToken(data, S_DELIM, i);// 0x20000000
								i++;
							}
							else if (mask == 0x40000000) // ( 1 << 30 )
							{
								pc.skywalker = getIntegerToken(data, S_DELIM, i);// 0x40000000
								i++;
							}
#ifdef _CHARSIGNADY_NO_
							else if (mask == 0x80000000) // ( 1 << 31 )
							{
								pc.签到标记 = getIntegerToken(data, S_DELIM, i);// 0x80000000
								i++;
							}
#endif
						}
					}
				}
				updataPcAct();
				if ((pc.status & CHR_STATUS_LEADER) != 0 && party[0].useFlag != 0)
				{
					party[0].level = pc.level;
					party[0].maxHp = pc.maxHp;
					party[0].hp = pc.hp;
					strcpy_s(party[0].name, pc.name);
				}

#ifdef _STONDEBUG_
				char title[128];
				sprintf_s( title, "%s %s [%s  %s:%s]", DEF_APPNAME, "调试版本",
					gmsv[selectServerIndex].name,
					gmsv[selectServerIndex].ipaddr, gmsv[selectServerIndex].port );
#else
				char title[128];
				extern int nServerGroup;
				sprintf_s( title, "%s %s [%s] %s", DEF_APPNAME, gmgroup[nServerGroup].name, gmsv[selectServerIndex].name,pc.name );

				extern int 繁体开关;	
				if(繁体开关){
					char 繁体[1024]={0};
					LCMapString (0x804,0x4000000,title, strlen(title),繁体,1024);
					sprintf(title,"%s",繁体);
				}

#endif
				extern int 编码;
				extern char* GB2312ToBIG5(const char* szGBString);
				if(编码==950){
					SetWindowText( hWnd, GB2312ToBIG5((const char *)title));
				}else{

					SetWindowText(hWnd,title);
				}
			}
			if (!bNewServer)
				pc.ridePetNo = -1;
			if ((bNewServer & 0xf000000) == 0xf000000 && sPetStatFlag == 1)
				saveUserSetting();
			break;
		case 'F':
			char familyName[256];

			data++;
			getStringToken(data, S_DELIM, 1, sizeof(familyName) - 1, familyName);
			makeStringFromEscaped(familyName);
			if (strlen(familyName) <= CHAR_NAME_LEN)
				strcpy_s(pc.familyName, familyName);
			pc.familyleader = getIntegerToken(data, S_DELIM, 2);
			pc.channel = getIntegerToken(data, S_DELIM, 3);
			pc.familySprite = getIntegerToken(data, S_DELIM, 4);
			pc.big4fm =	getIntegerToken(data, S_DELIM, 5);
#ifdef _CHANNEL_MODIFY
			if (pc.familyleader == FMMEMBER_NONE){
				pc.etcFlag &= ~PC_ETCFLAG_CHAT_FM;
				TalkMode = 0;
			}
#endif
			break;
		// HP,MP,EXP
		case 'M':
			data++;
			pc.hp  = getIntegerToken(data, '|', 1);
			pc.mp  = getIntegerToken(data, '|', 2);
			pc.exp = getIntegerToken(data, '|', 3);
			updataPcAct();
			if ((pc.status & CHR_STATUS_LEADER) != 0 && party[0].useFlag != 0)
				party[0].hp = pc.hp;
			break;
		case 'K':
			{
				char name[256], freeName[256];
				int no, kubun, i;
				unsigned int mask;

				no = data[1] - '0';
				data += 3;
				kubun = getInteger62Token(data, S_DELIM, 1);
				if (kubun == 0)
				{
					if (pet[no].useFlag)
					{
						if (no == pc.battlePetNo)
							pc.battlePetNo = -1;
						if (no == pc.mailPetNo)
							pc.mailPetNo = -1;
						pc.selectPetNo[no] = FALSE;
					}
					pet[no].useFlag = 0;
					break;
				}else{
					pet[no].useFlag = 1;
					if (kubun == 1){
						pet[no].graNo	= getIntegerToken(data, S_DELIM, 2);		// 0x00000002
						pet[no].hp		= getIntegerToken(data, S_DELIM, 3);		// 0x00000004
						pet[no].maxHp	= getIntegerToken(data, S_DELIM, 4);		// 0x00000008
						pet[no].mp		= getIntegerToken(data, S_DELIM, 5);		// 0x00000010
						pet[no].maxMp	= getIntegerToken(data, S_DELIM, 6);		// 0x00000020
						pet[no].exp		= getIntegerToken(data, S_DELIM, 7);		// 0x00000040
						pet[no].maxExp	= getIntegerToken(data, S_DELIM, 8);		// 0x00000080
						pet[no].level	= getIntegerToken(data, S_DELIM, 9);		// 0x00000100
						pet[no].atk		= getIntegerToken(data, S_DELIM, 10);		// 0x00000200
						pet[no].def		= getIntegerToken(data, S_DELIM, 11);		// 0x00000400
						pet[no].quick	= getIntegerToken(data, S_DELIM, 12);		// 0x00000800
						pet[no].ai		= getIntegerToken(data, S_DELIM, 13);		// 0x00001000
						pet[no].earth	= getIntegerToken(data, S_DELIM, 14);		// 0x00002000
						pet[no].water	= getIntegerToken(data, S_DELIM, 15);		// 0x00004000
						pet[no].fire	= getIntegerToken(data, S_DELIM, 16);		// 0x00008000
						pet[no].wind	= getIntegerToken(data, S_DELIM, 17);		// 0x00010000
						pet[no].maxSkill= getIntegerToken(data, S_DELIM, 18);		// 0x00020000
						pet[no].changeNameFlag = getIntegerToken(data, S_DELIM, 19);// 0x00040000
						pet[no].trn = getIntegerToken(data , S_DELIM, 20);
#ifdef _SHOW_FUSION
						pet[no].fusion = getIntegerToken(data , S_DELIM, 21);
						getStringToken(data, S_DELIM, 22, sizeof(name) - 1, name);// 0x00080000
						makeStringFromEscaped(name);
						if (strlen(name) <= PET_NAME_LEN)
							strcpy(pet[no].name, name);
						getStringToken(data, S_DELIM, 23, sizeof(freeName) - 1, freeName);// 0x00100000
						makeStringFromEscaped(freeName);
						if (strlen(freeName) <= PET_NAME_LEN)
							strcpy(pet[no].freeName, freeName);
#else
						getStringToken(data, S_DELIM, 21, sizeof(name) - 1, name);// 0x00080000
						makeStringFromEscaped(name);
						if (strlen(name) <= PET_NAME_LEN)
							strcpy(pet[no].name, name);
						getStringToken(data, S_DELIM, 22, sizeof(freeName) - 1, freeName);// 0x00100000
						makeStringFromEscaped(freeName);
						if (strlen(freeName) <= PET_NAME_LEN)
							strcpy(pet[no].freeName, freeName);
#endif
#ifdef _PETCOM_
						pet[no].oldhp = getIntegerToken(data, S_DELIM, 24);
						pet[no].oldatk = getIntegerToken(data, S_DELIM, 25);
						pet[no].olddef = getIntegerToken(data, S_DELIM, 26);
						pet[no].oldquick = getIntegerToken(data, S_DELIM, 27);
						pet[no].oldlevel = getIntegerToken(data, S_DELIM, 28);
#endif
#ifdef _RIDEPET_
						pet[no].rideflg = getIntegerToken(data, S_DELIM, 29);
#endif
#ifdef _PETBLESS_
						pet[no].blessflg = getIntegerToken(data, S_DELIM, 30);
						pet[no].blesshp = getIntegerToken(data, S_DELIM, 31);
						pet[no].blessatk = getIntegerToken(data, S_DELIM, 32);
						pet[no].blessdef = getIntegerToken(data, S_DELIM, 33);
						pet[no].blessquick  = getIntegerToken(data, S_DELIM, 34);
#endif
					}
					else
					{
						mask = 2;
						i = 2;
						for (; mask > 0; mask <<= 1)
						{
							if (kubun & mask)
							{
								if (mask == 0x00000002)
								{
									pet[no].graNo = getIntegerToken(data, S_DELIM, i);// 0x00000002
									i++;
								}
								else if (mask == 0x00000004)
								{
									pet[no].hp = getIntegerToken(data, S_DELIM, i);// 0x00000004
									i++;
								}
								else if (mask == 0x00000008)
								{
									pet[no].maxHp = getIntegerToken(data, S_DELIM, i);// 0x00000008
									i++;
								}
								else if (mask == 0x00000010)
								{
									pet[no].mp = getIntegerToken(data, S_DELIM, i);// 0x00000010
									i++;
								}
								else if (mask == 0x00000020)
								{
									pet[no].maxMp = getIntegerToken(data, S_DELIM, i);// 0x00000020
									i++;
								}
								else if (mask == 0x00000040)
								{
									pet[no].exp = getIntegerToken(data, S_DELIM, i);// 0x00000040
									i++;
								}
								else if (mask == 0x00000080)
								{
									pet[no].maxExp = getIntegerToken(data, S_DELIM, i);// 0x00000080
									i++;
								}
								else if (mask == 0x00000100)
								{
									pet[no].level = getIntegerToken(data, S_DELIM, i);// 0x00000100
									i++;
								}
								else if (mask == 0x00000200)
								{
									pet[no].atk = getIntegerToken(data, S_DELIM, i);// 0x00000200
									i++;
								}
								else if (mask == 0x00000400)
								{
									pet[no].def = getIntegerToken(data, S_DELIM, i);// 0x00000400
									i++;
								}
								else if (mask == 0x00000800)
								{
									pet[no].quick = getIntegerToken(data, S_DELIM, i);// 0x00000800
									i++;
								}
								else if (mask == 0x00001000 )
								{
									pet[no].ai = getIntegerToken(data, S_DELIM, i);// 0x00001000
									i++;
								}
								else if (mask == 0x00002000)
								{
									pet[no].earth = getIntegerToken(data, S_DELIM, i);// 0x00002000
									i++;
								}
								else if (mask == 0x00004000 )
								{
									pet[no].water = getIntegerToken(data, S_DELIM, i);// 0x00004000
									i++;
								}
								else if (mask == 0x00008000)
								{
									pet[no].fire = getIntegerToken(data, S_DELIM, i);// 0x00008000
									i++;
								}
								else if (mask == 0x00010000)
								{
									pet[no].wind = getIntegerToken(data, S_DELIM, i);// 0x00010000
									i++;
								}
								else if (mask == 0x00020000)
								{
									pet[no].maxSkill = getIntegerToken(data, S_DELIM, i);// 0x00020000
									i++;
								}
								else if (mask == 0x00040000)
								{
									pet[no].changeNameFlag = getIntegerToken(data, S_DELIM, i);// 0x00040000
									i++;
								}
								else if (mask == 0x00080000)
								{
									getStringToken(data, S_DELIM, i, sizeof(name) - 1, name);// 0x00080000
									makeStringFromEscaped(name);
									if (strlen(name) <= PET_NAME_LEN)
										strcpy(pet[no].name, name);
									i++;
								}
								else if (mask == 0x00100000)
								{
									getStringToken(data, S_DELIM, i, sizeof(freeName) - 1, freeName);// 0x00100000
									makeStringFromEscaped(freeName);
									if (strlen(freeName) <= PET_NAME_LEN)
										strcpy(pet[no].freeName, freeName);
									i++;
								}
#ifdef _PETCOM_
								else if (mask == 0x200000)
								{
									pet[no].oldhp = getIntegerToken(data, S_DELIM, i);
									i++;
								}
								else if (mask == 0x400000)
								{
									pet[no].oldatk = getIntegerToken(data, S_DELIM, i);
									i++;
								}
								else if (mask == 0x800000)
								{
									pet[no].olddef = getIntegerToken(data, S_DELIM, i);
									i++;
								}
								else if (mask == 0x1000000)
								{
									pet[no].oldquick = getIntegerToken(data, S_DELIM, i);
									i++;
								}
								else if (mask == 0x2000000)
								{
									pet[no].oldlevel = getIntegerToken(data, S_DELIM, i);
									i++;
								}
#endif
#ifdef _PETBLESS_
								else if (mask == 0x4000000)
								{
									pet[no].blessflg = getIntegerToken(data, S_DELIM, i);
									i++;
								}
								else if (mask == 0x8000000)
								{
									pet[no].blesshp = getIntegerToken(data, S_DELIM, i);
									i++;
								}
								else if (mask == 0x10000000)
								{
									pet[no].blessatk = getIntegerToken(data, S_DELIM, i);
									i++;
								}
								else if (mask == 0x20000000)
								{
									pet[no].blessquick = getIntegerToken(data, S_DELIM, i);
									i++;
								}
								else if (mask == 0x40000000)
								{
									pet[no].blessdef = getIntegerToken(data, S_DELIM, i);
									i++;
								}
#endif
							}
						}
					}
				}
			}
			break;
		case 'E':
			{
				data++;
				minEncountPercentage = getIntegerToken(data, S_DELIM, 1);
				maxEncountPercentage = getIntegerToken(data, S_DELIM, 2);
				nowEncountPercentage = minEncountPercentage;
			}
			break;
		case 'J':
			{
				char name[256], memo[256];
				int no;

				no = data[1] - '0';
				data += 3;
				magic[no].useFlag = getIntegerToken(data, S_DELIM, 1);
				if (magic[no].useFlag != 0)
				{
					magic[no].mp		= getIntegerToken(data, S_DELIM, 2);
					magic[no].field		= getIntegerToken(data, S_DELIM, 3);
					magic[no].target	= getIntegerToken(data, S_DELIM, 4);
					if (magic[no].target >= 100)
					{
						magic[no].target %= 100;
						magic[no].deadTargetFlag = 1;
					}
					else
						magic[no].deadTargetFlag = 0;
					getStringToken(data, S_DELIM, 5, sizeof(name) - 1, name);
					makeStringFromEscaped(name);
					if (strlen(name) <= sizeof(magic[no].name) - 1){
						strcpy_s(magic[no].name, name);
					}
					getStringToken(data, S_DELIM, 6, sizeof(memo) - 1, memo);
					makeStringFromEscaped(memo);
					if (strlen(memo) <= sizeof(magic[no].memo) - 1){
						strcpy_s(magic[no].memo, memo);
					}
				}
			}
			break;
		case 'N':
			{
				ACTION *ptAct;
				char name[256];
				int no, kubun, i, checkPartyCount, gx, gy, no2;
				unsigned int mask;
				
				no = data[1] - 48;
				data += 3;
				kubun = getInteger62Token(data, S_DELIM, 1);
				if (kubun == 0)
				{
					if (party[no].useFlag != 0 && party[no].id != pc.id)
					{
						ptAct = getCharObjAct(party[no].id);
						if (ptAct != NULL)
							delCharParty(ptAct);
					}
					gx = -1;
					gy = -1;
					if (party[no].ptAct != NULL)
					{
						gx = party[no].ptAct->nextGx;
						gy = party[no].ptAct->nextGy;
					}
					party[no].useFlag = 0;
					party[no].ptAct	= NULL;
					checkPartyCount = 0;
					no2 = -1;
#ifdef MAX_AIRPLANENUM
					for (i = 0; i < MAX_AIRPLANENUM; i++)
#else
					for (i = 0; i < MAX_PARTY; i++)
#endif
					{
						if (party[i].useFlag != 0)
						{
							checkPartyCount++;
							if (no2 == -1 && i > no)
								no2 = i;
						}
					}
					if (checkPartyCount <= 1)
					{
						partyModeFlag = 0;
						clearPartyParam();
#ifdef _CHANNEL_MODIFY
						pc.etcFlag &= ~PC_ETCFLAG_CHAT_MODE;
						if (TalkMode == 2)
							TalkMode = 0;
#endif
					}
					else
					{
						if (no2 >= 0 || gx >= 0 || gy >= 0)
							goFrontPartyCharacter(no2, gx, gy);
					}
					break;
				}
				partyModeFlag = 1;
				prSendFlag = 0;
				party[no].useFlag = 1;
				
				if (kubun == 1)
				{
					party[no].id		= getIntegerToken(data, S_DELIM, 2);	// 0x00000002
					party[no].level		= getIntegerToken(data, S_DELIM, 3);	// 0x00000004
					party[no].maxHp		= getIntegerToken(data, S_DELIM, 4);	// 0x00000008
					party[no].hp		= getIntegerToken(data, S_DELIM, 5);	// 0x00000010
					party[no].mp		= getIntegerToken(data, S_DELIM, 6);	// 0x00000020
					getStringToken(data, S_DELIM, 7, sizeof(name) - 1, name);	// 0x00000040
					makeStringFromEscaped(name);
					if (strlen(name) <= sizeof(party[no].name) - 1)
						strcpy(party[no].name, name);
					else
						strcpy(party[no].name, "???");
				}
				else
				{
					mask = 2;
					i = 2;
					for (; mask > 0; mask <<= 1)
					{
						if (kubun & mask)
						{
							if (mask == 0x00000002)
							{
								party[no].id = getIntegerToken(data, S_DELIM, i);// 0x00000002
								i++;
							}
							else if (mask == 0x00000004)
							{
								party[no].level = getIntegerToken(data, S_DELIM, i);// 0x00000004
								i++;
							}
							else if (mask == 0x00000008)
							{
								party[no].maxHp = getIntegerToken(data, S_DELIM, i);// 0x00000008
								i++;
							}
							else if (mask == 0x00000010)
							{
								party[no].hp = getIntegerToken(data, S_DELIM, i);// 0x00000010
								i++;
							}
							else if (mask == 0x00000020)
							{
								party[no].mp = getIntegerToken(data, S_DELIM, i);// 0x00000020
								i++;
							}
							else if (mask == 0x00000040)
							{
								getStringToken(data, S_DELIM, i, sizeof(name) - 1, name);// 0x00000040
								makeStringFromEscaped(name);
								if (strlen(name) <= sizeof(party[no].name) - 1)
									strcpy(party[no].name, name);
								else
									strcpy(party[no].name, "???");
								i++;
							}
						}
					}
				}
				if (party[no].id != pc.id)
				{
					ptAct = getCharObjAct(party[no].id);
					if (ptAct != NULL)
					{
						party[no].ptAct = ptAct;
						setCharParty(ptAct);
						// NPC???????
						if (no == 0)
							setCharLeader(ptAct);
					}
					else
						party[no].ptAct = NULL;
				}
				else
				{
					party[no].ptAct = pc.ptAct;
					setPcParty();
					// PC???????
					if (no == 0)
						setPcLeader();
				}
			}
			break;
		case 'I':
			{
				int i, no;
				char temp[256];
				
				data++;
				for (i = 0; i < MAX_ITEM; i++)
				{
#ifdef _ITEM_JIGSAW
#ifdef _NPC_ITEMUP
#ifdef _ITEM_COUNTDOWN
					no = i * 16;
#else
					no = i * 15;
#endif
#else
					no = i * 14;
#endif
#else
#ifdef _PET_ITEM
					no = i * 13;
#else
#ifdef _ITEM_PILENUMS
#ifdef _ALCHEMIST //#ifdef _ITEMSET7_TXT
					no = i * 14;
#else

					no = i * 11;

#endif//_ALCHEMIST
#else

					no = i * 10;

					//end modified by lsh

#endif//_ITEM_PILENUMS
#endif//_PET_ITEM
#endif//_ITEM_JIGSAW
					getStringToken(data, '|', no + 1, sizeof(temp) - 1 , temp);
					makeStringFromEscaped(temp);
					if (strlen(temp) == 0 )
					{
						pc.item[i].useFlag = 0;
						continue;
					}
					pc.item[i].useFlag = 1;
					if (strlen(temp) <= ITEM_NAME_LEN)
						strcpy(pc.item[i].name, temp);
					getStringToken(data, '|', no + 2, sizeof(temp) - 1, temp);
					makeStringFromEscaped(temp);
					if (strlen(temp) <= ITEM_NAME2_LEN)
						strcpy(pc.item[i].name2, temp);
					pc.item[i].color = getIntegerToken(data, '|', no + 3);
					if (pc.item[i].color < 0)
						pc.item[i].color = 0;
					getStringToken(data, '|', no + 4, sizeof(temp) - 1, temp);
					makeStringFromEscaped(temp);
					if (strlen(temp) <= ITEM_MEMO_LEN)
						strcpy(pc.item[i].memo, temp);
					pc.item[i].graNo = getIntegerToken(data, '|', no + 5);
					pc.item[i].field = getIntegerToken(data, '|', no + 6);
					pc.item[i].target = getIntegerToken(data, '|', no + 7);
					if (pc.item[i].target >= 100)
					{
						pc.item[i].target %= 100;
						pc.item[i].deadTargetFlag = 1;
					}
					else
						pc.item[i].deadTargetFlag = 0;
					pc.item[i].level = getIntegerToken(data, '|', no + 8);
					pc.item[i].sendFlag = getIntegerToken(data, '|', no + 9);

					// 显示物品耐久度
					getStringToken(data, '|', no + 10, sizeof(temp) - 1, temp);
					makeStringFromEscaped(temp);
					if (strlen(temp) <= 16)
						strcpy(pc.item[i].damage, temp);
#ifdef _ITEM_PILENUMS
					getStringToken(data, '|', no + 11, sizeof(temp) - 1, temp);
					makeStringFromEscaped(temp);
					pc.item[i].pile = atoi(temp);
#endif
#ifdef _ALCHEMIST //_ITEMSET7_TXT
					getStringToken(data, '|', no + 12, sizeof(temp) - 1, temp);
					makeStringFromEscaped(temp);
					strcpy(pc.item[i].alch, temp);
#endif
#ifdef _PET_ITEM
					pc.item[i].type = getIntegerToken(data, '|', no + 13);
#else
#ifdef _MAGIC_ITEM_
					pc.item[i].道具类型 = getIntegerToken(data, '|', no + 13);
#endif
#endif
#ifdef _ITEM_JIGSAW
					getStringToken(data, '|', no + 14, sizeof(temp) - 1 , temp);
					if (strlen(temp) <= 10)
						strcpy(pc.item[i].jigsaw, temp);
#endif
#ifdef _NPC_ITEMUP
					pc.item[i].itemup = getIntegerToken(data, '|', no + 15);
#endif
#ifdef _ITEM_COUNTDOWN
					pc.item[i].counttime = getIntegerToken(data, '|', no + 16);
#endif
				}
			}
			break;
		//接收到的宠物技能
		case 'W':
			{
				int i, no, no2;
				char temp[256];

				no = data[1] - '0';
				data += 3;
				for (i = 0; i < MAX_SKILL; i++)
					petSkill[no][i].useFlag = 0;
				for (i = 0; i < MAX_SKILL; i++)
				{
					no2 = i * 5;
					getStringToken(data, '|', no2 + 4, sizeof(temp) - 1, temp);
					makeStringFromEscaped(temp);
					if (strlen(temp) == 0)
						continue;
					petSkill[no][i].useFlag = 1;
					if (strlen(temp) <= SKILL_NAME_LEN)
						strcpy(petSkill[no][i].name, temp);
					else
						strcpy(petSkill[no][i].name, "??? name ???");
					petSkill[no][i].skillId = getIntegerToken(data, '|', no2 + 1);
					petSkill[no][i].field = getIntegerToken(data, '|', no2 + 2);
					petSkill[no][i].target = getIntegerToken(data, '|', no2 + 3);
					getStringToken(data, '|', no2 + 5, sizeof(temp) - 1, temp);
					makeStringFromEscaped(temp);
					if (strlen(temp) <= SKILL_MEMO_LEN)
						strcpy(petSkill[no][i].memo, temp);
					else
						strcpy(petSkill[no][i].memo, "??? memo ???");
				}
			}
		break;
#ifdef _CHAR_PROFESSION			// WON ADD 人物职业
		case 'S':
			{
				char name[CHAR_NAME_LEN + 1];
				char memo[PROFESSION_MEMO_LEN + 1];
				int i, count = 0;
				
				data++;
				for (i = 0; i < MAX_PROFESSION_SKILL; i++)
				{
					profession_skill[i].useFlag = 0;
					profession_skill[i].kind = 0;
				}
				for (i = 0; i < MAX_PROFESSION_SKILL; i++)
				{
					count = i * 9;
					profession_skill[i].useFlag  =  getIntegerToken(data, S_DELIM, 1 + count);
					profession_skill[i].skillId  =  getIntegerToken(data, S_DELIM, 2 + count);
					profession_skill[i].target   =  getIntegerToken(data, S_DELIM, 3 + count);
					profession_skill[i].kind	 =  getIntegerToken(data, S_DELIM, 4 + count);
					profession_skill[i].icon	 =  getIntegerToken(data, S_DELIM, 5 + count);
					profession_skill[i].costmp	 =  getIntegerToken(data, S_DELIM, 6 + count);
					profession_skill[i].skill_level = getIntegerToken(data, S_DELIM, 7 + count);
					memset(name, 0, sizeof(name));
					getStringToken(data, S_DELIM, 8 + count, sizeof(name) - 1, name);
					makeStringFromEscaped(name);
					if (strlen(name) <= CHAR_NAME_LEN)
						strcpy(profession_skill[i].name, name);
					memset(memo, 0, sizeof(memo));
					getStringToken(data, S_DELIM, 9 + count, sizeof(memo) - 1, memo);
					makeStringFromEscaped(memo);
					if (strlen(memo) <= PROFESSION_MEMO_LEN)
						strcpy(profession_skill[i].memo, memo);
				}
#ifdef _SKILLSORT
				SortSkill();
#endif
			}
		break;
#endif
#ifdef _PRO3_ADDSKILL
		case 'G':
			{
				int i, count = 0;
				data++;
				for (i = 0; i < MAX_PROFESSION_SKILL; i++)
					profession_skill[i].cooltime = 0;
				for (i = 0; i < MAX_PROFESSION_SKILL; i++)
				{
					count = i * 1;
					profession_skill[i].cooltime  =  getIntegerToken(data, S_DELIM, 1 + count);
				}
			}
		break;
#endif
#ifdef _PET_ITEM
		case 'B':
			{
				int i, no, nPetIndex;
				char szData[256];

				nPetIndex = data[1] - '0';
				data += 2;
				for (i = 0; i < MAX_PET_ITEM; i++)
				{
#ifdef _ITEM_JIGSAW
	#ifdef _NPC_ITEMUP
		#ifdef _ITEM_COUNTDOWN
					no = i * 16;
		#else
					no = i * 15;
		#endif	
	#else
					no = i * 14;
	#endif
#else
					no = i * 13;
#endif
					getStringToken(data, '|', no + 1, sizeof(szData) - 1, szData);
					makeStringFromEscaped(szData);
					if (strlen(szData) == 0)	// 没道具
					{
						memset(&pet[nPetIndex].item[i], 0, sizeof(pet[nPetIndex].item[i]));
						continue;
					}
					pet[nPetIndex].item[i].useFlag = 1;
					if (strlen(szData) <= ITEM_NAME_LEN)
						strcpy(pet[nPetIndex].item[i].name, szData);
					getStringToken(data, '|', no + 2, sizeof(szData) - 1, szData);
					makeStringFromEscaped(szData);
					if (strlen(szData) <= ITEM_NAME2_LEN)
						strcpy(pet[nPetIndex].item[i].name2, szData);
					pet[nPetIndex].item[i].color		= getIntegerToken(data, '|', no + 3);
					if (pet[nPetIndex].item[i].color < 0)
						pet[nPetIndex].item[i].color			= 0;
					getStringToken(data, '|', no + 4, sizeof(szData) - 1, szData);
					makeStringFromEscaped(szData);
					if (strlen(szData) <= ITEM_MEMO_LEN)
						strcpy(pet[nPetIndex].item[i].memo, szData);
					pet[nPetIndex].item[i].graNo				= getIntegerToken(data, '|', no + 5);
					pet[nPetIndex].item[i].field				= getIntegerToken(data, '|', no + 6);
					pet[nPetIndex].item[i].target				= getIntegerToken(data, '|', no + 7);
					if (pet[nPetIndex].item[i].target >= 100)
					{
						pet[nPetIndex].item[i].target			%= 100;
						pet[nPetIndex].item[i].deadTargetFlag	= 1;
					}
					else
						pet[nPetIndex].item[i].deadTargetFlag	= 0;
					pet[nPetIndex].item[i].level				= getIntegerToken(data, '|', no + 8);
					pet[nPetIndex].item[i].sendFlag				= getIntegerToken(data, '|', no + 9);
					
					// 显示物品耐久度
					getStringToken(data, '|', no + 10, sizeof(szData) - 1, szData);
					makeStringFromEscaped(szData);
					if (strlen(szData) <= 16)
						strcpy(pet[nPetIndex].item[i].damage, szData);
					pet[nPetIndex].item[i].pile					= getIntegerToken(data, '|', no + 11);
	#ifdef _ALCHEMIST //_ITEMSET7_TXT
					getStringToken(data, '|', no + 12, sizeof(szData) - 1, szData);
					makeStringFromEscaped(szData);
					strcpy(pet[nPetIndex].item[i].alch, szData);
	#endif
					pet[nPetIndex].item[i].type					= getIntegerToken(data, '|', no + 13);
#ifdef _ITEM_JIGSAW
					getStringToken(data, '|', no + 14, sizeof(szData) - 1, szData);
					makeStringFromEscaped(szData);
					strcpy( pet[nPetIndex].item[i].jigsaw, szData );
					//可拿给宠物装备的道具,就不会是拼图了,以下就免了
					//if( i == JigsawIdx )
					//	SetJigsaw( pc.item[i].graNo, pc.item[i].jigsaw );
#endif
#ifdef _NPC_ITEMUP
					pet[nPetIndex].item[i].itemup = getIntegerToken(data, '|', no + 15);
#endif
#ifdef _ITEM_COUNTDOWN
					pet[nPetIndex].item[i].counttime = getIntegerToken(data, '|', no + 16);
#endif
				}
			}
			break;
#endif
    }
}

void lssproto_MC_recv( int fd, int fl, int x1, int y1, int x2, int y2, int tileSum, int partsSum, int eventSum, char *data)
{
	char showString[512], floorName[32];

	if (logOutFlag)
		return;

#ifdef DEBUGPUSH
    char msg[800];
    sprintf_s(msg, "佋???????????????FL%d %d,%d-%d,%d (%ud/%ud)", fl, x1, y1, x2, y2, tileSum, partsSum);
    PUSH(msg);
#endif
	getStringToken(data, '|', 1, sizeof(showString) - 1, showString);
	makeStringFromEscaped(showString);
	if (nowFloor == fl)
	{
		char strPal[32];

		getStringToken(showString, '|', 1, sizeof(floorName) - 1, floorName);
		if (strlen(floorName) <= FLOOR_NAME_LEN)
			strcpy(nowFloorName, floorName);
		else
			strcpy(nowFloorName, "???");
		palNo = -2;
		getStringToken(showString, '|', 2, sizeof(strPal) - 1, strPal);
		if (strlen(strPal) == 0)
		{
			if (TimeZonePalChangeFlag == FALSE || loginFlag)
			{
				palNo = -1;
				palTime = 0;
				drawTimeAnimeFlag = 1;
			}
		}
		else
		{
			int pal;

			pal = atoi(strPal);
			if (pal >= 0)
			{
				if (TimeZonePalChangeFlag == TRUE || loginFlag)
				{
					palNo = pal;
					palTime = 0;
					drawTimeAnimeFlag = 0;
				}
			}
			else
			{
				if (TimeZonePalChangeFlag == FALSE || loginFlag)
				{
					palNo = -1;
					palTime = 0;
					drawTimeAnimeFlag = 1;
				}
			}
		}
	}

	if (mapCheckSum(fl, x1, y1, x2, y2, tileSum, partsSum, eventSum))
	{
		if (nowFloor == fl)
		{
			floorChangeFlag = FALSE;
			if (warpEffectStart)
				warpEffectOk = TRUE;
		}
		loginFlag = FALSE;
	}
}

void lssproto_M_recv(int fd, int fl, int x1, int y1, int x2, int y2, char* data)
{
	char showString[512], floorName[32], tilestring[18192], partsstring[18192], eventstring[18192], tmp[100];
	unsigned short tile[2048] , parts[2048], event[2048];
	int i, flag;

	if (logOutFlag)
		return;

#ifdef DEBUGPUSH
    char msg[800];
    sprintf_s(msg , "??????佋????????FL%d %d,%d-%d,%d", fl, x1, y1, x2, y2);
    PUSH(msg);
#endif
	getStringToken(data, '|', 1, sizeof(showString) - 1, showString);
	makeStringFromEscaped(showString);
	if (nowFloor == fl)
	{
		char strPal[32];

		getStringToken(showString, '|', 1, sizeof(floorName) - 1, floorName);
		if (strlen(floorName) <= FLOOR_NAME_LEN)
			strcpy(nowFloorName, floorName);
		else
			strcpy(nowFloorName, "???");
		palNo = -2;
		getStringToken(showString, '|', 2, sizeof(strPal) - 1, strPal);
		if (strlen(strPal) == 0)
		{
			if (TimeZonePalChangeFlag == FALSE || loginFlag)
			{
				palNo = -1;
				palTime = 0;
				drawTimeAnimeFlag = 1;
			}
		}
		else
		{
			int pal;

			pal = atoi(strPal);
			if (pal >= 0)
			{
				if (TimeZonePalChangeFlag == TRUE || loginFlag)
				{
					palNo = pal;
					palTime = 0;
					drawTimeAnimeFlag = 0;
				}
			}
			else
			{
				if (TimeZonePalChangeFlag == FALSE || loginFlag)
				{
					palNo = -1;
					palTime = 0;
					drawTimeAnimeFlag = 1;
				}
			}
		}
	}
	getStringToken(data, '|', 2, sizeof(tilestring) - 1, tilestring);	
	getStringToken(data, '|', 3, sizeof(partsstring) - 1, partsstring);
	getStringToken(data, '|', 4, sizeof(eventstring) - 1, eventstring);
	for (i = 0; ; i++)
	{
		flag = getStringToken(tilestring, ',', i + 1, sizeof(tmp) - 1, tmp);
		tile[i] = a62toi(tmp);
		getStringToken(partsstring, ',', i + 1, sizeof(tmp) - 1, tmp);
		parts[i] = a62toi(tmp);
		getStringToken(eventstring, ',', i + 1, sizeof(tmp) - 1, tmp);
		event[i] = a62toi(tmp);
		if (flag == 1)
			break;
	}
	writeMap(fl, x1, y1, x2, y2, tile, parts, event);
	if (mapEmptyFlag || floorChangeFlag)
	{
		if (nowFloor == fl)
		{
			redrawMap();
			floorChangeFlag = FALSE;
			if (warpEffectStart)
				warpEffectOk = TRUE;
		}
	}
	loginFlag = FALSE;
}

/*===========================
1 OBJTYPE_CHARA
2 OBJTYPE_ITEM
3 OBJTYPE_GOLD
4 NPC&other player
===========================*/
void lssproto_C_recv(int fd, char *data)
{
	int i, j, id, x, y, dir, graNo, level, nameColor, walkable, height, classNo, money, charType, charNameColor;
	char bigtoken[2048], smalltoken[2048], name[2048], freeName[2048], info[1024], fmname[2048], petname[1024];
#ifdef _CHARTITLE_STR_
	char titlestr[128];
	int titleindex=0;
	*titlestr = 0;
#endif
	int petlevel;
#ifdef _CHAR_PROFESSION			// WON ADD 人物职业
	int profession_class, profession_level, profession_skill_point;
#endif
#ifdef _ALLDOMAN // (不可开) Syu ADD 排行榜NPC
	int herofloor;
#endif
#ifdef _NPC_PICTURE
	int picture;
#endif
#ifdef _NPC_EVENT_NOTICE
	int noticeNo;
#endif
	ACTION *ptAct;
	if (logOutFlag)
		return;
	if (encountNowFlag)
		return;
	for (i = 0; ; i++)
	{
		getStringToken(data, ',', i + 1, sizeof(bigtoken) - 1, bigtoken);
		if (strlen(bigtoken) == 0)
			break;
#ifdef _OBJSEND_C
		getStringToken(bigtoken, '|', 1, sizeof(smalltoken) - 1, smalltoken);
		if (strlen(smalltoken) == 0)
			return;
		switch (atoi(smalltoken))
		{
		case 1://OBJTYPE_CHARA
			charType = getIntegerToken(bigtoken, '|', 2);
			getStringToken(bigtoken, '|', 3, sizeof(smalltoken) - 1, smalltoken);
			id = a62toi(smalltoken);

			extern BOOL 人物屏蔽开关;
			if(人物屏蔽开关){
				if(id != pc.id){
					if(charType <4 )
						continue;
				}
			}


			getStringToken(bigtoken, '|', 4, sizeof(smalltoken) - 1, smalltoken);
			x = atoi(smalltoken);
			getStringToken(bigtoken, '|', 5, sizeof(smalltoken) - 1, smalltoken);
			y = atoi(smalltoken);
			getStringToken(bigtoken, '|', 6, sizeof(smalltoken) - 1, smalltoken);
			dir = (atoi(smalltoken) + 3) % 8;
			getStringToken(bigtoken, '|', 7, sizeof(smalltoken) - 1, smalltoken);
			graNo = atoi(smalltoken);
			if(graNo==9999) continue;
			getStringToken(bigtoken, '|', 8, sizeof(smalltoken) - 1,smalltoken);
			level = atoi(smalltoken);
			nameColor = getIntegerToken(bigtoken, '|', 9);
			getStringToken(bigtoken, '|', 10, sizeof(name) - 1, name);
			makeStringFromEscaped(name);
			getStringToken(bigtoken, '|', 11, sizeof(freeName) - 1, freeName);
			makeStringFromEscaped(freeName);
			getStringToken(bigtoken, '|', 12, sizeof(smalltoken) - 1, smalltoken);
			walkable = atoi(smalltoken);
			getStringToken(bigtoken, '|', 13, sizeof(smalltoken) - 1, smalltoken);
			height = atoi(smalltoken);
			charNameColor = getIntegerToken(bigtoken, '|', 14);
			getStringToken(bigtoken , '|', 15, sizeof(fmname) - 1, fmname);
			makeStringFromEscaped(fmname);
			getStringToken(bigtoken , '|', 16, sizeof(petname) - 1, petname);
			makeStringFromEscaped(petname);
			getStringToken(bigtoken, '|', 17, sizeof(smalltoken) - 1, smalltoken);
			petlevel = atoi(smalltoken);
#ifdef _NPC_EVENT_NOTICE
			getStringToken(bigtoken, '|', 18, sizeof(smalltoken) - 1, smalltoken);
			noticeNo = atoi(smalltoken);
#endif
#ifdef _CHARTITLE_STR_
			getStringToken(bigtoken, '|', 23, sizeof(titlestr) - 1, titlestr);
			titleindex = atoi(titlestr);
			memset(titlestr,0,128);
			if(titleindex > 0){
				extern char* FreeGetTitleStr(int id);
				sprintf(titlestr,"%s",FreeGetTitleStr(titleindex));
			}
#endif
#ifdef _CHAR_PROFESSION			// WON ADD 人物职业
			getStringToken(bigtoken, '|', 18, sizeof(smalltoken) - 1, smalltoken);
			profession_class = atoi(smalltoken);
			getStringToken(bigtoken, '|', 19, sizeof(smalltoken) - 1, smalltoken);
			profession_level = atoi(smalltoken);
//			getStringToken(bigtoken, '|', 20, sizeof(smalltoken) - 1, smalltoken);
//			profession_exp = atoi(smalltoken);
			getStringToken(bigtoken, '|', 20, sizeof(smalltoken) - 1, smalltoken);
			profession_skill_point = atoi(smalltoken);
#ifdef _ALLDOMAN // Syu ADD 排行榜NPC
			getStringToken(bigtoken, '|', 21, sizeof(smalltoken) - 1, smalltoken);
			herofloor = atoi(smalltoken);
#endif
#ifdef _NPC_PICTURE
			getStringToken(bigtoken, '|', 22, sizeof(smalltoken) - 1, smalltoken);
			picture = atoi(smalltoken);
#endif
//    #ifdef _GM_IDENTIFY		// Rog ADD GM识别
//			getStringToken(bigtoken , '|', 23 , sizeof( gm_name ) - 1, gm_name );
//			makeStringFromEscaped( gm_name );
//  #endif
#endif
			if (charNameColor < 0)
				charNameColor = 0;
			if (pc.id == id)
			{
				if (pc.ptAct == NULL)
				{
					createPc(graNo, x, y, dir);
					updataPcAct();
				}
				else
					setPcGraNo(graNo, pc.dir);

#ifdef _CHARTITLE_STR_
				getCharTitleSplit(titlestr,&pc.ptAct->TitleText);
#endif
				updateMapArea();
#ifdef _CHAR_PROFESSION			// WON ADD 人物职业
//    #ifdef _GM_IDENTIFY		// Rog ADD GM识别
//				setPcParam(name, freeName, level, petname, petlevel, nameColor, walkable, height, profession_class, profession_level, profession_exp, profession_skill_point , gm_name);
//				setPcParam(name, freeName, level, petname, petlevel, nameColor, walkable, height, profession_class, profession_level, profession_skill_point , gm_name);
//    #else
//				setPcParam(name, freeName, level, petname, petlevel, nameColor, walkable, height, profession_class, profession_level, profession_exp, profession_skill_point);
#ifdef _ALLDOMAN // Syu ADD 排行榜NPC
				setPcParam(name, freeName, level, petname, petlevel, nameColor, walkable, height, profession_class, profession_level, profession_skill_point , herofloor);
#else
				setPcParam(name, freeName, level, petname, petlevel, nameColor, walkable, height, profession_class, profession_level, profession_skill_point);
#endif
//    #endif
#else
				setPcParam(name, freeName, level, petname, petlevel, nameColor, walkable, height);
#endif				
				setPcNameColor(charNameColor);
				if ((pc.status & CHR_STATUS_LEADER) != 0 && party[0].useFlag != 0)
				{
					party[0].level = pc.level;
					strcpy(party[0].name, pc.name);
				}
#ifdef MAX_AIRPLANENUM
				for (j = 0; j < MAX_AIRPLANENUM; j++)
#else
				for (j = 0; j < MAX_PARTY; j++)
#endif
				{
					if (party[j].useFlag != 0 && party[j].id == id)
					{
						party[j].ptAct = pc.ptAct;
						setPcParty();
						if (j == 0)
							setPcLeader();
						break;
					}
				}
			}
			else
			{
#ifdef _CHAR_PROFESSION			// WON ADD 人物职业
    #ifdef _GM_IDENTIFY		// Rog ADD GM识别
				setNpcCharObj(id, graNo, x, y, dir, fmname, name, freeName,
					level, petname, petlevel, nameColor, walkable, height, charType, profession_class, gm_name);
    #else
			#ifdef _NPC_PICTURE
				setNpcCharObj(id, graNo, x, y, dir, fmname, name, freeName,
					level, petname, petlevel, nameColor, walkable, height, charType, profession_class, picture );
			#else				
				setNpcCharObj(id, graNo, x, y, dir, fmname, name, freeName,
					level, petname, petlevel, nameColor, walkable, height, charType, profession_class );
			#endif
	#endif
#else
#ifdef _NPC_EVENT_NOTICE
				setNpcCharObj(id, graNo, x, y, dir, fmname, name, freeName,
					level, petname, petlevel, nameColor, walkable, height, charType,noticeNo
#ifdef _CHARTITLE_STR_
					,titlestr
#endif
					
					);
#else
				setNpcCharObj(id, graNo, x, y, dir, fmname, name, freeName,
					level, petname, petlevel, nameColor, walkable, height, charType);
#endif
#endif
				ptAct = getCharObjAct(id);
#ifdef _NPC_EVENT_NOTICE
			//	noticeNo=120137;
				if(charType==13 && noticeNo>0){
				setNpcNotice( ptAct, noticeNo);
				}
#endif
				if (ptAct != NULL)
				{
#ifdef MAX_AIRPLANENUM
					for (j = 0; j < MAX_AIRPLANENUM; j++)
#else
					for (j = 0; j < MAX_PARTY; j++)
#endif
					{
						if (party[j].useFlag != 0 && party[j].id == id)
						{
							party[j].ptAct = ptAct;
							setCharParty(ptAct);
							if (j == 0)
								setCharLeader(ptAct);
							break;
						}
					}
					setCharNameColor(ptAct, charNameColor);
				}
			}
			break;
		case 2://OBJTYPE_ITEM
				getStringToken(bigtoken, '|', 2, sizeof(smalltoken) - 1, smalltoken);
				id = a62toi(smalltoken);
				getStringToken(bigtoken, '|', 3, sizeof(smalltoken) - 1, smalltoken);
				x = atoi(smalltoken);
				getStringToken(bigtoken, '|', 4, sizeof(smalltoken) - 1, smalltoken);
				y = atoi(smalltoken);
				getStringToken(bigtoken, '|', 5, sizeof(smalltoken) - 1, smalltoken);
				graNo = atoi(smalltoken);
				classNo = getIntegerToken(bigtoken, '|', 6);
				getStringToken(bigtoken, '|', 7, sizeof(info) - 1, info);
				makeStringFromEscaped(info);
				setItemCharObj(id, graNo, x, y, 0, classNo, info);
			break;
		case 3://OBJTYPE_GOLD
				getStringToken(bigtoken, '|', 2, sizeof(smalltoken) - 1, smalltoken);
				id = a62toi(smalltoken);
				getStringToken(bigtoken, '|', 3, sizeof(smalltoken) - 1, smalltoken);
				x = atoi(smalltoken);
				getStringToken(bigtoken, '|', 4, sizeof(smalltoken) - 1, smalltoken);
				y = atoi(smalltoken);
				getStringToken(bigtoken, '|', 5, sizeof(smalltoken) - 1, smalltoken);
				money = atoi(smalltoken);
				sprintf_s(info, "%d Stone", money);
				if (money > 10000)
					setMoneyCharObj(id, 24050, x, y, 0, money, info);
				else if (money > 1000)
					setMoneyCharObj(id, 24051, x, y, 0, money, info);
				else
					setMoneyCharObj(id, 24052, x, y, 0, money, info);
			break;
		case 4:
			getStringToken(bigtoken, '|', 2, sizeof(smalltoken) - 1, smalltoken);
			id = a62toi(smalltoken);
			getStringToken(bigtoken, '|', 3, sizeof(name) - 1, name);
			makeStringFromEscaped(name);
			getStringToken(bigtoken, '|', 4, sizeof(smalltoken) - 1, smalltoken);
			dir = (atoi(smalltoken) + 3) % 8;
			getStringToken(bigtoken, '|', 5, sizeof(smalltoken) - 1, smalltoken);
			graNo = atoi(smalltoken);
			getStringToken(bigtoken, '|', 6, sizeof(smalltoken) - 1, smalltoken);
			x = atoi(smalltoken);
			getStringToken(bigtoken, '|', 7, sizeof(smalltoken) - 1, smalltoken);
			y = atoi(smalltoken);

#ifdef _CHAR_PROFESSION			// WON ADD 人物职业
    #ifdef _GM_IDENTIFY		// Rog ADD GM识别
			setNpcCharObj( id, graNo, x, y, dir, "", name, "",
				level, petname, petlevel, nameColor, 0, height, 2, 0, "");
    #else
		#ifdef _NPC_PICTURE
			setNpcCharObj( id, graNo, x, y, dir, "", name, "",
				level, petname, petlevel, nameColor, 0, height, 2, 0, 0 );
		#else
			setNpcCharObj( id, graNo, x, y, dir, "", name, "",
				level, petname, petlevel, nameColor, 0, height, 2, 0 );
		#endif
    #endif
#else			
#ifdef _NPC_EVENT_NOTICE
			setNpcCharObj( id, graNo, x, y, dir, "", name, "",
				level, petname, petlevel, nameColor, 0, height, 2, 0
#ifdef _CHARTITLE_STR_
				,titlestr
#endif
				);
#else
						setNpcCharObj( id, graNo, x, y, dir, "", name, "",
				level, petname, petlevel, nameColor, 0, height, 2);
#endif
#endif
			ptAct = getCharObjAct( id );
			break;
		}
#else
		getStringToken(bigtoken , '|' , 11 , sizeof(smalltoken) - 1, smalltoken);
		if ( strlen(smalltoken) > 0 ){
			// NPC?⿳
			charType = getIntegerToken(bigtoken, '|', 1 );
			getStringToken(bigtoken, '|', 2, sizeof(smalltoken) - 1, smalltoken);
			id = a62toi(smalltoken);
			getStringToken(bigtoken, '|', 3, sizeof(smalltoken) - 1, smalltoken);
			x = atoi(smalltoken);
			getStringToken(bigtoken, '|', 4, sizeof(smalltoken) - 1, smalltoken);
			y = atoi(smalltoken);
			getStringToken(bigtoken, '|', 5, sizeof(smalltoken) - 1, smalltoken);
			dir = (atoi(smalltoken) + 3) % 8;
			getStringToken(bigtoken, '|', 6, sizeof(smalltoken) - 1, smalltoken);
			graNo = atoi(smalltoken);
			getStringToken(bigtoken, '|', 7, sizeof(smalltoken) - 1,smalltoken);
			level = atoi(smalltoken);
			nameColor = getIntegerToken(bigtoken, '|', 8 );
			getStringToken(bigtoken , '|' , 9 , sizeof(name) - 1, name );
			makeStringFromEscaped(name);
			getStringToken(bigtoken , '|' , 10 , sizeof( freeName ) - 1, freeName );
			makeStringFromEscaped( freeName );
			getStringToken(bigtoken, '|', 11, sizeof(smalltoken) - 1, smalltoken);
			walkable = atoi(smalltoken);
			getStringToken(bigtoken, '|', 12, sizeof(smalltoken) - 1, smalltoken);
			height = atoi(smalltoken);
			charNameColor = getIntegerToken(bigtoken, '|', 13 );
			getStringToken(bigtoken , '|' , 14, sizeof( fmname ) - 1, fmname );
			makeStringFromEscaped( fmname );
			getStringToken(bigtoken , '|' , 15 , sizeof( petname ) - 1, petname );
			makeStringFromEscaped( petname );
			getStringToken(bigtoken, '|', 16, sizeof(smalltoken) - 1, smalltoken);
			petlevel = atoi(smalltoken);
			if ( charNameColor < 0 )
				charNameColor = 0;

			if ( pc.id == id ){
				if ( pc.ptAct == NULL ){
					createPc( graNo, x, y, dir );
					updataPcAct();
				}else{
					setPcGraNo( graNo, pc.dir );
				}
				updateMapArea();
				setPcParam( name, freeName, level, petname, petlevel, nameColor, walkable, height );			
				setPcNameColor( charNameColor );
				if ( (pc.status & CHR_STATUS_LEADER) != 0
				 && party[0].useFlag != 0 ){
					party[0].level = pc.level;
					strcpy( party[0].name, pc.name );
				}
				for ( j = 0; j < MAX_PARTY; j++ ){
					if ( party[j].useFlag != 0 && party[j].id == id ){
						party[j].ptAct = pc.ptAct;
						setPcParty();
						if ( j == 0 ){
							setPcLeader();
						}
						break;
					}
				}
			}else{
#ifdef _NPC_PICTURE
				setNpcCharObj( id, graNo, x, y, dir, fmname, name, freeName,
					level, petname, petlevel, nameColor, walkable, height, charType, 0 );
#else
				setNpcCharObj( id, graNo, x, y, dir, fmname, name, freeName,
					level, petname, petlevel, nameColor, walkable, height, charType );
#endif
				ptAct = getCharObjAct( id );
				if ( ptAct != NULL ){
					for ( j = 0; j < MAX_PARTY; j++ ){
						if ( party[j].useFlag != 0 && party[j].id == id ){
							party[j].ptAct = ptAct;
							setCharParty( ptAct );
							if ( j == 0 ){
								setCharLeader( ptAct );
							}
							break;
						}
					}
					setCharNameColor( ptAct, charNameColor );
				}
			}
		}else{
			getStringToken(bigtoken, '|', 6, sizeof(smalltoken) - 1, smalltoken);
			if ( strlen(smalltoken) > 0 ){
				getStringToken(bigtoken, '|', 1, sizeof(smalltoken) - 1, smalltoken);
				id = a62toi(smalltoken);
				getStringToken(bigtoken, '|', 2, sizeof(smalltoken) - 1, smalltoken);
				x = atoi(smalltoken);
				getStringToken(bigtoken, '|', 3, sizeof(smalltoken) - 1, smalltoken);
				y = atoi(smalltoken);
				getStringToken(bigtoken, '|', 4, sizeof(smalltoken) - 1, smalltoken);
				graNo = atoi(smalltoken);
				classNo = getIntegerToken(bigtoken, '|', 5 );
				getStringToken(bigtoken, '|', 6, sizeof( info ) - 1, info );
				makeStringFromEscaped( info );
				setItemCharObj( id, graNo, x, y, 0, classNo, info );
			}else{
				getStringToken(bigtoken, '|', 4, sizeof(smalltoken) - 1, smalltoken);
				if ( strlen(smalltoken) > 0 ){
					getStringToken(bigtoken, '|', 1, sizeof(smalltoken) - 1, smalltoken);
					id = a62toi(smalltoken);
					getStringToken(bigtoken, '|', 2, sizeof(smalltoken) - 1, smalltoken);
					x = atoi(smalltoken);
					getStringToken(bigtoken, '|', 3, sizeof(smalltoken) - 1, smalltoken);
					y = atoi(smalltoken);
					getStringToken(bigtoken, '|', 4, sizeof(smalltoken) - 1, smalltoken);
					money = atoi(smalltoken);
					sprintf_s( info, "%d Stone", money );
					if ( money > 10000 ){
						setMoneyCharObj( id, 24050, x, y, 0, money, info );
					}else
					if ( money > 1000 ){
						setMoneyCharObj( id, 24051, x, y, 0, money, info );
					}else{
						setMoneyCharObj( id, 24052, x, y, 0, money, info );
					}
				}
			}
		}
#endif
	}

}


void lssproto_CA_recv( int fd, char *data ) 
{
	char bigtoken[2048];
	char smalltoken[2048];
	int alreadytellC[1024];
	int tellCindex = 0;
	int tellflag;
	int i, j;
	int charindex;
	int x;
	int y;
	int act;
	int dir;
	int effectno = 0, effectparam1 = 0, effectparam2 = 0;
#ifdef _STREET_VENDOR
	char szStreetVendorTitle[32];
#endif
	ACTION *ptAct;


	// ????????????????⿳?䥺???
	if ( logOutFlag )
		return;

	// ??䥺???
	if ( encountNowFlag )
	{
		return;
	}

	for ( i = 0; ; i++ )
	{
		getStringToken(data, ',', i+1, sizeof(bigtoken ) - 1, bigtoken );
		if ( strlen(bigtoken ) == 0 )
			break;
		getStringToken(bigtoken, '|', 1, sizeof(smalltoken) - 1, smalltoken);
		charindex = a62toi(smalltoken);
		getStringToken(bigtoken, '|', 2, sizeof(smalltoken) - 1, smalltoken);
		x = atoi(smalltoken);
		getStringToken(bigtoken, '|', 3, sizeof(smalltoken) - 1, smalltoken);
		y = atoi(smalltoken);
		getStringToken(bigtoken, '|', 4, sizeof(smalltoken) - 1, smalltoken);
		act = atoi(smalltoken);
		getStringToken(bigtoken, '|', 5, sizeof(smalltoken) - 1, smalltoken);
		dir = (atoi(smalltoken)+3)%8;
		getStringToken(bigtoken, '|', 6, sizeof(smalltoken) - 1, smalltoken);
#ifdef _STREET_VENDOR
		if (act == 41) strncpy_s(szStreetVendorTitle,sizeof(szStreetVendorTitle),smalltoken,sizeof(szStreetVendorTitle));
		else
#endif
		{
		effectno = atoi(smalltoken);
		effectparam1 = getIntegerToken(bigtoken, '|', 7 );
		effectparam2 = getIntegerToken(bigtoken, '|', 8 );
		}


		if ( pc.id == charindex )
		{
			// PC??????
			if ( pc.ptAct == NULL
			 || (pc.ptAct != NULL && pc.ptAct->anim_chr_no == 0) )
			{
				// ???????????C????????????
				if (bNewServer)
					lssproto_C_send(sockfd, charindex );
				else
					old_lssproto_C_send(sockfd, charindex );
			}
			else
			{
#ifdef _STREET_VENDOR
				if (act == 41){
					if (pc.iOnStreetVendor == 1){
						memset(pc.ptAct->szStreetVendorTitle,0,sizeof(pc.ptAct->szStreetVendorTitle));
						sprintf_s(pc.ptAct->szStreetVendorTitle,sizeof(pc.ptAct->szStreetVendorTitle),"%s",szStreetVendorTitle);
						changePcAct( x, y, dir, act, effectno, effectparam1, effectparam2 );
#ifdef _STREET_VENDOR_CHANGE_ICON
						if (bNewServer)
							lssproto_AC_send(sockfd,nowGx,nowGy,5);
						else
							old_lssproto_AC_send(sockfd,nowGx,nowGy,5);
						setPcAction(5);
#endif
					}
				}
				else
#endif
				changePcAct( x, y, dir, act, effectno, effectparam1, effectparam2 );
			}
			continue;
		}

		ptAct = getCharObjAct( charindex );
		if ( ptAct == NULL )
		{
			// ?ID???????????????????ID??????
			tellflag = 0;
			for ( j = 0; j < tellCindex; j++ )
			{
				if ( alreadytellC[j] == charindex )
				{
					tellflag = 1;
					break;
				}
			}
			if ( tellflag == 0 && tellCindex < sizeof(alreadytellC) )
			{
				alreadytellC[tellCindex] = charindex;
				tellCindex++;

				if (bNewServer)
					lssproto_C_send(sockfd, charindex );
				else
					old_lssproto_C_send(sockfd, charindex );
			}
		}
		else
		{
#ifdef _STREET_VENDOR
			if (act == 41){
				memset(ptAct->szStreetVendorTitle,0,sizeof(ptAct->szStreetVendorTitle));
				strncpy_s(ptAct->szStreetVendorTitle,szStreetVendorTitle,sizeof(szStreetVendorTitle));
			}
#endif
			changeCharAct( ptAct, x, y, dir, act, effectno, effectparam1, effectparam2 );
		}
	}
}


// ?????
void lssproto_CD_recv( int fd, char *data )
{
	int i, j;
	int id;

	// ????????????????⿳?䥺???
	if ( logOutFlag )
		return;

	for ( i = 1; ; i++ )
	{
//		id = getInteger62Token(data, '|', i );
		id = getInteger62Token(data, ',', i );
		if ( id == -1 )
			break;

		delCharObj( id );

#ifdef MAX_AIRPLANENUM
		for (j = 0; j < MAX_AIRPLANENUM; j++)
#else
		for ( j = 0; j < MAX_PARTY; j++ )
#endif
		{
			if ( party[j].useFlag != 0 && party[j].id == id )
			{
				party[j].ptAct = NULL;
				break;
			}
		}
	}
}




// ? ///////////////////////////////////////////////////////////////////
// ????????????
void walkSendForServer( int x, int y, char *direction )
{
#ifdef _REMAKE_20
	if (!ChangedLibrary()) 
		RestoreLibrary();
#endif
	if (bNewServer)
		lssproto_W_send(sockfd , x, y, direction );
	else
		old_lssproto_W_send(sockfd , x, y, direction );
}


// ????????????
//  ??????????????????????????????????
void noChecksumWalkSendForServer( int x, int y, char *direction )
{
	if (bNewServer)
		lssproto_W2_send(sockfd , x, y, direction );
	else
		old_lssproto_w_send(sockfd , x, y, direction );
}


// ???????
void lssproto_W_recv( int fd, int id, int x, int y )
{
	// ?????????????????
	// ??????????????????????
}

#ifdef _SETTICK_COUNT
float *fTalkTick=NULL;
#endif
void chatStrSendForServer( char *str, int color )
{
	char dest[1024], m[1024];
	int x, y;
#ifdef _FRIENDCHANNEL
	char tmpMsg[128];
#endif
#ifdef _TELLCHANNEL
	char tmp1[128] ;
#endif


#ifdef _STONDEBUG_ // 手动送出封包
	{
		if ( !strncmp( str, "send ", 5) ) {
			sendDataToServer( str+5);
			return;
		}
	}
#endif


#ifdef _SETTICK_COUNT
	if ( fTalkTick == NULL || (GetTickCount() - *fTalkTick) > 60000 ){
		FREE( fTalkTick);
#ifdef _STONDEBUG_
		g_iMallocCount--;
#endif
		fTalkTick = (float *)MALLOC( sizeof(float));
#ifdef _STONDEBUG_
		g_iMallocCount++;
#endif
		*fTalkTick = (float)0.00;
	}
	if ( (GetTickCount() - *fTalkTick) > 2500 ){
		*fTalkTick = (float)GetTickCount();
	}else{
		return;
	}
#endif
#ifdef _CHAR_MANOR_DEBUG
	if(strstr(str,".光环")){
		int 光环ID=0;
		光环ID = getIntegerToken(str, ' ',2);
		setCharmManor(pc.ptAct, 光环ID);
		return;
	}
#endif
	x = nowGx;
	y = nowGy;
	makeEscapeString( str, dest, sizeof(dest) );

#ifdef _TELLCHANNEL
	getStringToken( str , ' ', 1, sizeof( tmp1 ) -1, tmp1 );
		char *found;
		char tellName[32]={""};

	switch (TalkMode){
	case 0:
#ifdef _THEATER
		if (pc.iTheaterMode & 0x00000001)
		{
			StockChatBufferLine("表演中请勿喧哗。。。", FONT_PAL_YELLOW);
			return;
		}
#endif
#ifdef _NPC_DANCE
		if( pc.iDanceMode )
		{
			StockChatBufferLine("嘘！你可能会吵到别人。", FONT_PAL_YELLOW);
			return;
		}
#endif
#ifdef _CHANNEL_MODIFY
	case 2:
#endif
		sprintf_s( m, "P|%s", dest );
		break;
	case 1:
		if ( strcmp ( tmp1 , str ) == 0 || strlen(tmp1) > 16) {
			StockChatBufferLine(  "指令使用格式不正确！" , FONT_PAL_RED);
			strcpy(secretName,"");
			return ; 
		}
		if ( found = strstr( str, " " )){
			strncpy_s(tellName, str, strlen(str) - strlen(found));
		}
		sprintf_s( secretName, "%s ",tellName);
		sprintf_s( m, "P|/tell %s", dest );
		break;
#ifndef _CHANNEL_MODIFY
	#ifdef _FRIENDCHANNEL
		case 2:
			sprintf_s(tmpMsg,"T|%s : %s",pc.name,dest);
			lssproto_CHATROOM_send (sockfd ,tmpMsg);
			return;
	#endif
#else
		case 3:
			sprintf_s(m,"P|/FM%s",dest);
			break;
			/*
		case 4:
			sprintf_s(m,"P|/OCC%s：%s",pc.name,dest);
			break;
#ifdef _CHATROOMPROTOCOL
		case 5:
			sprintf_s(tmpMsg,"T|%s：%s",pc.name,dest);
			lssproto_CHATROOM_send (sockfd ,tmpMsg);
			return;
#endif
			*/
#ifdef _CHANNEL_WORLD  ///世界频道
		case 4:
			sprintf_s(m,"P|/WD %s",dest);
			break;
#endif
#ifdef _CHANNEL_ALL_SERV
		case 5:
			sprintf_s(m,"P|/XQ %s",dest);
			break;
#endif
#endif

	}
#else
	sprintf_s( m, "P|%s", dest );
#endif
	if (bNewServer){
		CHAR szOutBuffer[1280+1] = {0};  
		WORD wLanguageID = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED); 
		LCID Locale = MAKELCID(wLanguageID, SORT_CHINESE_PRCP);
		int iRet = LCMapString(Locale,  
			LCMAP_SIMPLIFIED_CHINESE,  
			m, -1,  
			szOutBuffer, 1280);
		lssproto_TK_send(sockfd, x, y, szOutBuffer, color, NowMaxVoice );
	}
	else
		old_lssproto_TK_send(sockfd, x, y, m, color, NowMaxVoice );
}




// ???? ///////////////////////////////////////////////////////////
void lssproto_TK_recv( int fd, int index, char *message, int color )
{
	char id[2];
#ifdef _MESSAGE_FRONT_
	char msg1[2024];
	char *msg;
#else
	char msg[2024];
#endif
	ACTION *ptAct;
	int fontsize =0;
#ifdef _MESSAGE_FRONT_
	msg1[0] = 0xA1;
	msg1[1] = 0xF4;
	msg1[2] = 0;
	msg = msg1+2;
#endif
	// ????????????????⿳?䥺???
	if ( logOutFlag )
		return;

	getStringToken( message, '|', 1, sizeof( id  ) - 1, id );

	if ( id[0] == 'P' ){
#ifndef _CHANNEL_MODIFY
		getStringToken( message, '|', 2, 2022 - 1, msg );
		makeStringFromEscaped( msg );
	#ifdef _TRADETALKWND				// (不可开) Syu ADD 交易新增对话框架
		TradeTalk( msg ) ; 
	#endif
#endif

#ifdef _CHANNEL_MODIFY
		char szToken[4];

		if (getStringToken(message,'|',2,sizeof(szToken) - 1,szToken) == 0){
			getStringToken(message,'|',3,2022 - 1,msg);
			makeStringFromEscaped(msg);
#ifdef _FONT_SIZE
			char token[10];
			if ( getStringToken(message,'|',4,sizeof(token) - 1,token) == 1) {
				fontsize = atoi( token);
				if ( fontsize < 0 )	fontsize = 0;
			}else {
				fontsize = 0;
			}
#endif
			if (strlen(szToken) > 1){
				if (strcmp(szToken,"TK") == 0)	InitSelectChar(message,0);
				else if (strcmp(szToken,"TE") == 0) InitSelectChar(message,1);
				return;
			}
			else{
				switch (szToken[0]){
				// 密语频道
				case 'M':
					{
						char tellName[32] = { "" };
						char szMsgBuf[2024];
						char temp[] = "告诉你：";
						char *found;
					
						if (found = strstr(msg,temp)){
							strncpy_s(tellName,msg,strlen(msg) - strlen(found));
							color = 5;
							strncpy_s(szMsgBuf,found,sizeof(szMsgBuf));
							memset(msg,0,2022);
							sprintf(msg,"[%s]%s",tellName,szMsgBuf);
							sprintf(secretName,"%s ",tellName);
						}
					}
					break;
				// 家族频道
				case 'F':
				// 队伍频道
				case 'T':
				// 职业频道
				case 'O':
					break;
				}
				SaveChatData(msg,szToken[0],false);
			}
		}
		else getStringToken(message,'|',2,2022 - 1,msg);
#ifdef _TALK_WINDOW
		if (!g_bTalkWindow)
#endif
		TradeTalk(msg);
		if (strcmp(msg,"成立聊天室扣除２００石币") == 0)	pc.gold -= 200;
#ifdef _FONT_SIZE
#ifdef _MESSAGE_FRONT_
		StockChatBufferLineExt( msg-2, color, fontsize );
#else
		StockChatBufferLineExt( msg, color, fontsize );
#endif
#else
#ifdef _MESSAGE_FRONT_
		StockChatBufferLine(msg-2,color);
#else
		StockChatBufferLine(msg,color);
#endif
#endif
#else
	#ifdef _TELLCHANNEL		// (不可开) ROG ADD 密语频道
		char tellName[128] = { "" };
		char tmpMsg[STR_BUFFER_SIZE + 32];
		char TK[4];

		if ( getStringToken( msg, '|', 1, sizeof( TK ) - 1, TK ) == 0){
			if (strcmp(TK,"TK") == 0)	InitSelectChar( msg, 0);
			else if (strcmp(TK,"TE") == 0) InitSelectChar( msg, 1);
		}
		else{
			char temp[] = "告诉你：";
			char *found;

			if (strcmp( msg, "成立聊天室扣除２００石币") == 0)	pc.gold -= 200;

			if ( found = strstr( msg, temp )){
				strncpy_s(tellName, msg, strlen(msg) - strlen(found));
				color = 5;
				sprintf_s(tmpMsg,"[%s]%s",tellName, found);
				StockChatBufferLine( tmpMsg, color );
				sprintf_s(msg,"");
				sprintf_s(secretName,"%s ",tellName);
			}else StockChatBufferLine( msg, color );
		}
	#else
#ifdef _FONT_SIZE
		StockChatBufferLineExt( msg, color, fontsize );
#else
		StockChatBufferLine( msg, color );
#endif
#endif
#endif
		if ( index >= 0 ){
			if ( pc.ptAct != NULL && pc.id == index ){
				// 1000??
				setPcFukidashi( 1000 );
			}
			else
			{
				ptAct = getCharObjAct( index );
				if ( ptAct != NULL )
				{
					// 1000??
					setCharFukidashi( ptAct, 1000 );
				}
			}
		}
	}
	return;
}




// ???? ///////////////////////////////////////////////////////////
// ????????????????
void createNewCharStart(void)
{
	newCharStatus = 0;

	// ??????????????????
	if (bNewServer){
#ifdef _MORECHARACTERS_
		extern int 多人物当前页数;
		lssproto_CreateNewChar_send(sockfd, selectPcNo+多人物当前页数*2, newCharacterName,
			newCharacterGraNo, newCharacterFaceGraNo,
			newCharacterVit, newCharacterStr, newCharacterTgh, newCharacterDex,
			newCharacterEarth, newCharacterWater, newCharacterFire, newCharacterWind,
			newCharacterHomeTown );
#else
#ifdef _TRADITIONAL_LONG_
		CHAR szOutBuffer[128+1] = {0};  
		WORD wLanguageID = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED); 
		LCID Locale = MAKELCID(wLanguageID, SORT_CHINESE_PRCP);
		int iRet = LCMapString(Locale,  
			LCMAP_SIMPLIFIED_CHINESE,  
			newCharacterName, -1,  
			szOutBuffer, 128);
#endif
		lssproto_CreateNewChar_send(sockfd, selectPcNo, 
#ifdef _TRADITIONAL_LONG_
			szOutBuffer,
#else
			newCharacterName,
#endif
			newCharacterGraNo, newCharacterFaceGraNo,
			newCharacterVit, newCharacterStr, newCharacterTgh, newCharacterDex,
			newCharacterEarth, newCharacterWater, newCharacterFire, newCharacterWind,
			newCharacterHomeTown );
#endif
	}else
		old_lssproto_CreateNewChar_send(sockfd, selectPcNo, newCharacterName,
			newCharacterGraNo, newCharacterFaceGraNo,
			newCharacterVit, newCharacterStr, newCharacterTgh, newCharacterDex,
			newCharacterEarth, newCharacterWater, newCharacterFire, newCharacterWind,
			newCharacterHomeTown );

	SETSENDING;
}


// ???????
//   ??： 0 ... ? / 1 ... ?? / -1 ... ?????? / -2 ... ?????????
int createNewCharProc(void)
{
	if ( netproc_sending == NETPROC_RECEIVED )
	{
		netproc_sending = NETPROC_NOTSEND;
		if ( newCharStatus == 1 )
		{
			return 1;
		}
		else
		{
			return -2;
		}
    }

	SETTIMEOUT2( NET_ERRMSG_CREATECHARTIMEOUT );

	return 0;
}

char 创建人物内容提示[512];
void lssproto_CreateNewChar_recv( int fd, char *result, char *data ) 
{
	if ( netproc_sending == NETPROC_SENDING )
	{
		netproc_sending = NETPROC_RECEIVED;

		if ( strcmp( result, SUCCESSFULSTR ) == 0 )
		{
			newCharStatus = 1;
		}else{
			sprintf(创建人物内容提示,data);
		}
    }
}








// ????? /////////////////////////////////////////////////////////////
// ???????
void delCharStart(void)
{
	charDelStatus = 0;

	if (bNewServer)
		lssproto_CharDelete_send(sockfd, gamestate_deletechar_charname );
	else
		old_lssproto_CharDelete_send(sockfd, gamestate_deletechar_charname );

	SETSENDING;
}


// ??????
//   ??： 0 ... ??? / 1 ... ???? / -1 ... ?????? / -2 ... ?????????
int delCharProc(void)
{
	if ( netproc_sending == NETPROC_RECEIVED )
	{
		netproc_sending = NETPROC_NOTSEND;
		if ( charDelStatus )
		{
			return 1;
		}
		else
		{
			return -2;
		}
	}

	SETTIMEOUT2( NET_ERRMSG_DELETECHARTIMEOUT );

	return 0;
}


// ???????
void lssproto_CharDelete_recv( int fd, char *result, char *data )
{
	if ( netproc_sending == NETPROC_SENDING )
	{
		netproc_sending = NETPROC_RECEIVED;
		if ( strcmp( result, SUCCESSFULSTR ) == 0 )
		{
			charDelStatus = 1;
    	}
    }
}

// ????? /////////////////////////////////////////////////////////
void lssproto_PR_recv( int fd, int request, int result )
{
	// ????????????????⿳?䥺???
	if ( logOutFlag )
		return;

	if ( request == 1 && result == 1 )
	{
		// ??????????????
		setPcParty();
	}
	else
	if ( request == 0 && result == 1 )
	{
		// ??????????
		//delPcParty();
		//delPcLeader();
		partyModeFlag = 0;
		clearPartyParam();
#ifdef _CHANNEL_MODIFY
		pc.etcFlag &= ~PC_ETCFLAG_CHAT_MODE;
		if (TalkMode == 2) TalkMode = 0;
#endif

		char dir = (pc.dir+5) % 8;
		if (bNewServer)
			lssproto_SP_send(sockfd, nextGx, nextGy, dir );
		else
			old_lssproto_SP_send(sockfd, nextGx, nextGy, dir );
	}
	prSendFlag = 0;
}



// EV?????????? /////////////////////////////////////////
void lssproto_EV_recv( int fd, int seqno, int result )
{
	// ????????????????⿳?䥺???
	if ( logOutFlag )
		return;

	if ( eventWarpSendId == seqno )
	{
		eventWarpSendFlag = 0;
		if ( result == 0 )
		{
			// ?????????䴘????
			redrawMap();
			floorChangeFlag = FALSE;
			// ???????????
			warpEffectStart = TRUE;
			warpEffectOk = TRUE;
		}
	}
	else
	if ( eventEnemySendId == seqno )
	{
		if ( result == 0 )
		{
			eventEnemySendFlag = 0;
		}
		//else
		//{
			// ??????process.cpp???
		//}
	}
}


// ??OK???OK????? ＯＫ? ////////////////////////
void lssproto_FS_recv( int fd, int flg )
{
	// ????????????????⿳?䥺???
	if ( logOutFlag )
		return;

	pc.etcFlag = (unsigned short)flg;
}



// ????????? ///////////////////////////////////////////////
void lssproto_AB_recv( int fd, char *data )
{
	int i;
	int no;
	int nameLen;
	char name[256];
	int flag;
	int useFlag;
#ifdef _MAILSHOWPLANET				// (可开放) Syu ADD 显示名片星球
	char planetid[8];
	int j ;
#endif
	// ????????????????⿳?䥺???
	if ( logOutFlag )
		return;

	for ( i = 0; i < MAX_ADR_BOOK; i++ )
	{
		//no = i * 6; //the second
        no = i * 8;
		useFlag = getIntegerToken(data, '|', no+1 );
		if ( useFlag == -1 )
		{
			useFlag = 0;
		}
		if ( useFlag <= 0 )
		{
#if 0
			if ( addressBook[i].useFlag == 1 )
#else
			if ( MailHistory[i].dateStr[MAIL_MAX_HISTORY-1][0] != '\0' )
#endif
			{
				memset( &MailHistory[i], 0, sizeof( MailHistory[0] ) ) ;
				SaveMailHistory( i );
				// ⺶????
				mailLamp = CheckMailNoReadFlag();
				// ?????
				DeathLetterAction();
			}
			addressBook[i].useFlag = 0;
			addressBook[i].name[0] = '\0';
			continue;
		}

#ifdef _EXTEND_AB
		if ( i == MAX_ADR_BOOK-1 )
			addressBook[i].useFlag = useFlag;
		else
			addressBook[i].useFlag = 1;
#else
		addressBook[i].useFlag = 1;
#endif

		flag = getStringToken(data, '|', no+2, sizeof(name) - 1 , name );

		if ( flag == 1 )
			break;

		makeStringFromEscaped(name);
		nameLen = strlen(name);
		if ( 0 < nameLen && nameLen <= CHAR_NAME_LEN )
		{
			strcpy( addressBook[i].name, name );
		}
		addressBook[i].level = getIntegerToken(data, '|', no+3 );
		addressBook[i].dp = getIntegerToken(data, '|', no+4 );
		addressBook[i].onlineFlag = (short)getIntegerToken(data, '|', no+5 );
		addressBook[i].graNo = getIntegerToken(data, '|', no+6 );
		addressBook[i].transmigration = getIntegerToken(data, '|', no+7 );
#ifdef _MAILSHOWPLANET				// (可开放) Syu ADD 显示名片星球
		for ( j = 0 ; j < MAX_GMSV ; j ++ ) {
			if ( gmsv[j].used == '1' ) {
				getStringToken( gmsv[j].ipaddr, '.', 4, sizeof( planetid ) -1, planetid );
				if ( addressBook[i].onlineFlag == atoi(planetid) ) {
					sprintf_s( addressBook[i].planetname , "%s" , gmsv[j].name ) ; 
					break;
				}
			}
		}
#endif
	}
}


// ???????????////////////////////////////////////////
void lssproto_ABI_recv( int fd, int num, char* data )
{
	char name[256];
	int nameLen;
	int useFlag;
#ifdef _MAILSHOWPLANET				// (可开放) Syu ADD 显示名片星球
	char planetid[8];
	int j ;
#endif

	// ????????????????⿳?䥺???
	if ( logOutFlag )
		return;

	useFlag = getIntegerToken(data, '|', 1 );
	if ( useFlag == -1 )
	{
		useFlag = 0;
	}
	if ( useFlag == 0 )
	{
#if 0
		if ( addressBook[num].useFlag == 1 )
#else
		if ( MailHistory[num].dateStr[MAIL_MAX_HISTORY-1][0] != '\0' )
#endif
		{
			memset( &MailHistory[num], 0, sizeof( MailHistory[0] ) ) ;
			SaveMailHistory( num );
			// ⺶????
			mailLamp = CheckMailNoReadFlag();
			// ?????
			DeathLetterAction();
		}
		addressBook[num].useFlag = useFlag;
		addressBook[num].name[0] = '\0';
		return;
	}

#ifdef _EXTEND_AB
	if ( num == MAX_ADR_BOOK-1 )
		addressBook[num].useFlag = useFlag;
	else
		addressBook[num].useFlag = 1;
#else
	addressBook[num].useFlag = useFlag;
#endif

	getStringToken(data, '|', 2, sizeof(name) - 1 , name );
	makeStringFromEscaped(name);
	nameLen = strlen(name);
	if ( 0 < nameLen && nameLen <= CHAR_NAME_LEN )
	{
		strcpy( addressBook[num].name, name );
	}
	addressBook[num].level = getIntegerToken(data, '|', 3 );
	addressBook[num].dp = getIntegerToken(data, '|', 4 );
	addressBook[num].onlineFlag = (short)getIntegerToken(data, '|', 5 );
	addressBook[num].graNo = getIntegerToken(data, '|', 6 );
	addressBook[num].transmigration = getIntegerToken(data, '|', 7 );
#ifdef _MAILSHOWPLANET				// (可开放) Syu ADD 显示名片星球
	if ( addressBook[num].onlineFlag == 0 ) 
		sprintf_s( addressBook[num].planetname , " ");
	for ( j = 0 ; j < MAX_GMSV ; j ++ ) {
		if ( gmsv[j].used == '1' ) {
			getStringToken( gmsv[j].ipaddr, '.', 4, sizeof( planetid ) -1, planetid );
			if ( addressBook[num].onlineFlag == atoi(planetid) ) {
				sprintf_s( addressBook[num].planetname ,64, "%s" , gmsv[j].name ) ; 
				break;
			}
		}
	}
#endif


}


// ??????? /////////////////////////////////////////////////
void lssproto_RS_recv( int fd, char *data )
{
	int i;
	char token[2048];
	char item[2048];

	// ????????????????⿳?䥺???
	if ( logOutFlag )
		return;

	battleResultMsg.useFlag = 1;
	//cary 确定 栏位 数
	int cols = RESULT_CHR_EXP;
	getStringToken(data, ',', RESULT_CHR_EXP+1, sizeof( token ) - 1, token );
	if ( token[0] == 0){
		cols = RESULT_CHR_EXP - 1;
		battleResultMsg.resChr[RESULT_CHR_EXP-1].petNo = -1;
		battleResultMsg.resChr[RESULT_CHR_EXP-1].levelUp = -1;
		battleResultMsg.resChr[RESULT_CHR_EXP-1].exp = -1;
	}
	//end cary

	for ( i = 0; i < cols; i++ ){
		getStringToken(data, ',', i+1, sizeof( token ) - 1, token );

		battleResultMsg.resChr[i].petNo = getIntegerToken( token, '|', 1 );
		battleResultMsg.resChr[i].levelUp = getIntegerToken( token, '|', 2 );
		battleResultMsg.resChr[i].exp = getInteger62Token( token, '|', 3 );
	}

	getStringToken(data, ',', i+1, sizeof( token ) - 1, token );
	getStringToken( token, '|', 1, sizeof( item ) - 1, item );
	makeStringFromEscaped( item );
	if ( strlen( item ) <= RESULT_ITEM_NAME_LEN )
	{
		strcpy( battleResultMsg.item[0], item );
	}
	getStringToken( token, '|', 2, sizeof( item ) - 1, item );
	makeStringFromEscaped( item );
	if ( strlen( item ) <= RESULT_ITEM_NAME_LEN )
	{
		strcpy( battleResultMsg.item[1], item );
	}
	getStringToken( token, '|', 3, sizeof( item ) - 1, item );
	makeStringFromEscaped( item );
	if ( strlen( item ) <= RESULT_ITEM_NAME_LEN )
	{
		strcpy( battleResultMsg.item[2], item );
	}
}


// ??????????? /////////////////////////////////////////////////
void lssproto_RD_recv( int fd, char *data )
{
	// ????????????????⿳?䥺???
	if ( logOutFlag )
		return;

	battleResultMsg.useFlag = 2;

	battleResultMsg.resChr[0].exp = getInteger62Token(data, '|', 1 );
	battleResultMsg.resChr[1].exp = getInteger62Token(data, '|', 2 );
}


// ?????? ///////////////////////////////////////////////////
void lssproto_SI_recv( int fd, int from, int to )
{
	// ????????????????⿳?䥺???
	if ( logOutFlag )
		return;

	swapItem( from, to );
}


// ????⿳ ///////////////////////////////////////////////////////
void lssproto_I_recv( int fd, char *data )
{
	int i, j;
	int no;
	char name[256];
	char name2[256];
	char memo[256];
	//char *data = "9|乌力斯坦的肉||0|耐久力10前後回复|24002|0|1|0|7|不会损坏|1|肉|20||10|乌力斯坦的肉||0|耐久力10前後回复|24002|0|1|0|7|不会损坏|1|肉|20|";
	if ( logOutFlag )//人物未登陆则不接收这个封包
		return;

	for ( j = 0; ; j++ ){
#ifdef _ITEM_JIGSAW
#ifdef _NPC_ITEMUP
#ifdef _ITEM_COUNTDOWN
			no = j * 17;
#else
			no = j * 16;
#endif
#else
		no = j * 15;
#endif
#else
#ifdef _PET_ITEM
		no = j * 14;
#else
#ifdef _ITEM_PILENUMS
#ifdef _ALCHEMIST
#ifdef _MAGIC_ITEM_
			no = j * 15;
#else
			no = j * 13;
#endif
#else
			no = j * 12;
#endif
#else

		no = j * 11;
#endif
#endif//_PET_ITEM
#endif//_ITEM_JIGSAW
		i = getIntegerToken(data, '|', no+1 );//道具位
		if ( getStringToken(data, '|', no+2, sizeof(name) - 1 , name ) == 1 )//道具名
			break;
		makeStringFromEscaped(name);
		if ( strlen(name) == 0 ){
			pc.item[i].useFlag = 0;
			continue;
		}
		pc.item[i].useFlag = 1;
		if ( strlen(name) <= ITEM_NAME_LEN ){
			strcpy( pc.item[i].name, name );
		}
		getStringToken(data, '|', no+3, sizeof( name2 ) - 1, name2 );//第二个道具名
		makeStringFromEscaped( name2 );
		if ( strlen( name2 ) <= ITEM_NAME2_LEN ){
			strcpy( pc.item[i].name2, name2 );
		}
		pc.item[i].color = getIntegerToken(data, '|', no+4 );//颜色
		if ( pc.item[i].color < 0 )
			pc.item[i].color = 0;
		getStringToken(data, '|', no+5, sizeof( memo ) - 1, memo );//道具介绍
		makeStringFromEscaped( memo );
		if ( strlen( memo ) <= ITEM_MEMO_LEN ){
			strcpy( pc.item[i].memo, memo );
		}
		pc.item[i].graNo = getIntegerToken(data, '|', no+6 );//道具形像
		pc.item[i].field = getIntegerToken(data, '|', no+7 );//
		pc.item[i].target = getIntegerToken(data, '|', no+8 );
		if ( pc.item[i].target >= 100 ){
			pc.item[i].target %= 100;
			pc.item[i].deadTargetFlag = 1;
		}else{
			pc.item[i].deadTargetFlag = 0;
		}
		pc.item[i].level = getIntegerToken(data, '|', no+9 );//等级
		pc.item[i].sendFlag = getIntegerToken(data, '|', no+10 );

		{
			// 显示物品耐久度
			char damage[256];
			getStringToken(data, '|', no+11, sizeof( damage ) - 1, damage );
			makeStringFromEscaped( damage );
			if ( strlen( damage ) <= 16 ){	
				strcpy( pc.item[i].damage, damage );
			}
		} 
#ifdef _ITEM_PILENUMS
		{
			char pile[256];
			getStringToken(data, '|', no+12, sizeof( pile ) - 1, pile );
			makeStringFromEscaped( pile );
			pc.item[i].pile = atoi( pile);
		}
#endif

#ifdef _ALCHEMIST //_ITEMSET7_TXT
		{
			char alch[256];
			getStringToken(data, '|', no+13, sizeof( alch ) - 1, alch );
			makeStringFromEscaped( alch );
			strcpy( pc.item[i].alch, alch );
		}
#endif
#ifdef _PET_ITEM
		{
			char type[8];
			getStringToken(data, '|', no + 14, sizeof(type) - 1, type);
			makeStringFromEscaped(type);
			pc.item[i].type = atoi(type);
		}
#else
#ifdef _MAGIC_ITEM_
		pc.item[i].道具类型 = getIntegerToken(data, '|', no + 14);
#endif
#endif
		/*
#ifdef _ITEM_JIGSAW
		{
			char jigsaw[10];
			getStringToken(data, '|', no + 15, sizeof(jigsaw) - 1, jigsaw);
			makeStringFromEscaped(jigsaw);
			strcpy( pc.item[i].jigsaw, jigsaw );
			if( i == JigsawIdx ){
				SetJigsaw( pc.item[i].graNo, pc.item[i].jigsaw );
			}
		}
#endif
#ifdef _NPC_ITEMUP
			pc.item[i].itemup = getIntegerToken(data, '|', no + 16);
#endif
#ifdef _ITEM_COUNTDOWN
			pc.item[i].counttime = getIntegerToken(data, '|', no + 17);
#endif
			*/

	}
}

void lssproto_WN_recv( int fd,int windowtype,int buttontype,int seqno,int objindex,char* data )
{
	// ????????????????⿳?䥺???
	if ( logOutFlag )
		return;

	if( strstr(data,"否则家族在七天之后会消失唷！") ){	
		if( TimeGetTime() - MsgCooltime > 300000 )
			MsgCooltime = TimeGetTime();  
		else
			return;
	}
	openServerWindow( windowtype, buttontype, seqno, objindex, data );

}


// ??????? ///////////////////////////////////////////////////////
void lssproto_PME_recv( int fd, int objindex,
	int graphicsno, int x, int y, int dir, int flg, int no, char *cdata )
{
	// ????????????????⿳?䥺???
	if ( logOutFlag )
		return;

	// ??䥺???
	if ( encountNowFlag )
		return;

	// ??
	if ( flg == 0 )
	{
		switch ( no )
		{
			case 0:
				createPetAction( graphicsno, x, y, dir, 0, dir, -1 );
				break;
			case 1:
				createPetAction( graphicsno, x, y, dir, 2, 0, -1 );
				break;
		}
	}
	else
	// ?
	{
		char smalltoken[2048];
		int id;
		int x;
		int y;
		int dir;
		int graNo;
		int level;
		int nameColor;
		char name[2048];
		char freeName[2048];
		int walkable;
		int height;
		int charType;
		int ps=1;
#ifdef _OBJSEND_C
		ps = 2;
#endif
		charType = getIntegerToken( cdata, '|', ps++);
		getStringToken( cdata, '|', ps++, sizeof(smalltoken) - 1, smalltoken);
		id = a62toi(smalltoken);
		getStringToken( cdata, '|', ps++, sizeof(smalltoken) - 1, smalltoken);
		x = atoi(smalltoken);
		getStringToken( cdata, '|', ps++, sizeof(smalltoken) - 1, smalltoken);
		y = atoi(smalltoken);
		getStringToken( cdata, '|', ps++, sizeof(smalltoken) - 1, smalltoken);
		dir = (atoi(smalltoken)+3)%8;
		getStringToken( cdata, '|', ps++, sizeof(smalltoken) - 1, smalltoken);
		graNo = atoi(smalltoken);
		getStringToken( cdata, '|', ps++, sizeof(smalltoken) - 1,smalltoken);
		level = atoi(smalltoken);
		nameColor = getIntegerToken( cdata, '|', ps++ );
		getStringToken( cdata, '|' , ps++ , sizeof(name) - 1, name );
		makeStringFromEscaped(name);
		getStringToken( cdata, '|' , ps++ , sizeof( freeName ) - 1, freeName );
		makeStringFromEscaped( freeName );
		getStringToken( cdata, '|', ps++, sizeof(smalltoken) - 1, smalltoken);
		walkable = atoi(smalltoken);
		getStringToken( cdata, '|', ps++, sizeof(smalltoken) - 1, smalltoken);
		height = atoi(smalltoken);

		// ???⿳??????????
		if ( setReturnPetObj( id, graNo, x, y, dir, name, freeName,
			level, nameColor, walkable, height, charType ) )
		{
			switch ( no )
			{
				case 0:
					createPetAction( graphicsno, x, y, dir, 1, 0, objindex );
					break;

				case 1:
					createPetAction( graphicsno, x, y, dir, 3, 0, objindex );
					break;
			}
		}
	}
}

char *pCommand = NULL;
DWORD dwDiceTimer;
// ?????????? /////////////////////////////////////////////
void lssproto_EF_recv( int fd, int effect, int level, char *option )
{
	// ??????????
	if ( effect == 0 )
	{
		mapEffectRainLevel = 0;
		mapEffectSnowLevel = 0;
		mapEffectKamiFubukiLevel = 0;
#ifdef _HALLOWEEN_EFFECT
		mapEffectHalloween = 0;
		initMapEffect(FALSE);
#endif
		return;
	}
	// ???????
	if ( effect & 1 )
	{
		mapEffectRainLevel = level;
	}
	// ??????
	if ( effect & 2 )
	{
		mapEffectSnowLevel = level;
	}
	// ????
	if ( effect & 4 )
	{
		mapEffectKamiFubukiLevel = level;
	}
#ifdef _HALLOWEEN_EFFECT
	if (effect & 8) mapEffectHalloween = level;
#endif
// Terry add 2002/01/14
#ifdef __EDEN_DICE
	// 骰子
	if (effect == 10) 
	{
		pCommand = (char*)MALLOC(strlen(option)+1);
#ifdef _STONDEBUG_
		g_iMallocCount++;
#endif
		if (pCommand != NULL) 
		{
			strcpy(pCommand,strlen(option)+1,option);
			bMapEffectDice = TRUE;
			dwDiceTimer = TimeGetTime();
		}
	}
#endif
// Terry end
}


// ? ///////////////////////////////////////////////////////////////////
void lssproto_IS_recv( int fd, char* cdata )
{
}
void lssproto_EN_recv( int fd,int result,int field )
{

	if ( logOutFlag )
		return;
	if ( result > 0 ){
		EncountFlag = TRUE;
		if ( result == 4 )
			vsLookFlag = 1;
		else
			vsLookFlag = 0;
		if ( result == 6 || result == 2 )
			eventEnemyFlag = 1;
		else
			eventEnemyFlag = 0;

		if ( field < 0 || BATTLE_MAP_FILES <= field )
			BattleMapNo = 0;
		else
			BattleMapNo = field;
		if ( result == 2 )
			DuelFlag = TRUE;
		else
			DuelFlag = FALSE;

		if ( result == 2 || result == 5 )
			NoHelpFlag = TRUE;
		else
			NoHelpFlag = FALSE;

		BattleStatusReadPointer = BattleStatusWritePointer =0;
		BattleCmdReadPointer = BattleCmdWritePointer =0;
#ifdef PK_SYSTEM_TIMER_BY_ZHU
		BattleCliTurnNo = -1;
#endif
	}else{
		sendEnFlag = 0;
		duelSendFlag = 0;
		jbSendFlag = 0;
	}
}


void lssproto_HL_recv( int fd, int flg )
{
	helpFlag = flg;
}

void lssproto_B_recv( int fd,char* command )
{

	if ( *( command + 1 ) == 'C'){
		strcpy( BattleStatusBak[ BattleStatusWritePointer ], command );
		BattleStatusWritePointer = ( BattleStatusWritePointer + 1 ) & ( BATTLE_BUF_SIZE-1 );
	}

	else if ( *( command + 1 ) == 'P')
		sscanf_s( command + 3, "%X|%X|%X", &BattleMyNo, &BattleBpFlag, &BattleMyMp );

	else if ( *( command + 1 ) == 'A'){
		sscanf_s( command + 3, "%X|%X", &BattleAnimFlag, &BattleSvTurnNo );

		if ( BattleTurnReceiveFlag == TRUE ){ 
			BattleCliTurnNo = BattleSvTurnNo;
			BattleTurnReceiveFlag = FALSE;
		}
	}

	else if ( *( command + 1 ) == 'U')
		BattleEscFlag = TRUE;
#ifdef PK_SYSTEM_TIMER_BY_ZHU
	else if ( *( command + 1 ) == 'Z') {
		int TurnNo = -1;
		sscanf( command + 3, "%X", &TurnNo );
		if ( TurnNo >= 0 )
		{
			if ( TurnNo > 0 )
			{
				BattleCntDownRest = TRUE;
			} else {
				BattleCntDown = TimeGetTime() + BATTLE_CNT_DOWN_TIME;
			}
			BattleCliTurnNo = TurnNo;
		}
	}
	else if ( *( command + 1 ) == 'F') {
		int TurnNo = -1;
		sscanf( command + 3, "%X", &TurnNo );
		if ( TurnNo >= 0 )
		{
			BattleCliTurnNo = TurnNo;
		}
		//if ( TurnNo >= 0 )
		//{
		//	if ( TurnNo > 0 )
		//	{
		//		BattleCntDownRest = TRUE;
		//	} else {
		//		BattleCntDown = TimeGetTime() + BATTLE_CNT_DOWN_TIME;
		//	}
		//	BattleCliTurnNo = TurnNo;
		//}
		BattleDown();
		//BattleSetWazaHitBox( 0, 0 );
	}
	else if ( *( command + 1 ) == 'O') {
		int TurnNo = -1;
		sscanf( command + 3, "%X", &TurnNo );
		if ( TurnNo >= 0 )
		{
			BattleCliTurnNo = TurnNo;
			BattleCntDown = TimeGetTime() + BATTLE_CNT_DOWN_TIME;
		}
	}
#endif
	else {
		strcpy( BattleCmdBak[ BattleCmdWritePointer ], command );
		BattleCmdWritePointer = ( BattleCmdWritePointer + 1 ) & ( BATTLE_BUF_SIZE-1 );
	}

#ifdef _STONDEBUG__MSG
	//StockChatBufferLine( command, FONT_PAL_RED );
#endif

}

#ifdef _PETS_SELECTCON
void lssproto_PETST_recv( int fd,int petarray,int result )
{
	if ( petarray < 0 || petarray >= 5 ) return;
	pc.selectPetNo[ petarray] = result;
	BattlePetStMenCnt--;
	if ( BattlePetStMenCnt < 0 ) BattlePetStMenCnt = 0;
	if ( BattlePetStMenCnt > 4 ) BattlePetStMenCnt = 4;
	if ( pc.battlePetNo == petarray )
		pc.battlePetNo = -1;
}

#endif

void lssproto_KS_recv( int fd,int petarray,int result )
{
	int cnt = 0;
	int i;

	BattlePetReceiveFlag = FALSE;
	BattlePetReceivePetNo = -1;
	if ( result == TRUE ){ 
		battlePetNoBak = -2;
		if ( petarray != -1 ){ 
			pc.selectPetNo[ petarray ] = TRUE;
			if ( pc.mailPetNo == petarray ) pc.mailPetNo = -1;
			for ( i = 0 ; i < 5 ; i++ ){
				if ( pc.selectPetNo[ i ] == TRUE && i != petarray ) cnt++;
				if ( cnt >= 4 ){ 
					pc.selectPetNo[ i ] = FALSE;
					cnt--;
				}
			}
		}
		pc.battlePetNo = petarray;
	}
#ifdef _AFTER_TRADE_PETWAIT_
	else{
		if (tradeStatus==2){
			pc.selectPetNo[petarray] = 0;
			if (petarray==pc.battlePetNo)
				pc.battlePetNo = -1;
		}		
	}
#endif
}

#ifdef _STANDBYPET
void lssproto_SPET_recv( int fd, int standbypet, int result )
{
	int cnt = 0;
	int i;

	StandbyPetSendFlag = FALSE;

	if ( result == TRUE ) { 
		pc.standbyPet = standbypet;
		for ( i =0; i <MAX_PET; i++) {
			if ( standbypet & ( 1 << i) )
				pc.selectPetNo[ i ] = TRUE;
			else
				pc.selectPetNo[ i ] = FALSE;
		}
	}

}
#endif


void lssproto_SKUP_recv( int fd,int point )
{
	// ????????????
	StatusUpPoint = point;
}

// ????????????????
void lssproto_MSG_recv( int fd,int aindex,char* text ,int color)
{
	char moji[ 256 ];
	int noReadFlag;
	
	// ???????????
	mailLamp = TRUE;
	
	// ??????
	MailHistory[ aindex ].newHistoryNo--;
	// ????????
	if ( MailHistory[ aindex ].newHistoryNo <= -1 ) MailHistory[ aindex ].newHistoryNo = MAIL_MAX_HISTORY - 1;
	
	// ????
	getStringToken( text, '|', 1, sizeof( MailHistory[ aindex ].dateStr[ MailHistory[ aindex ].newHistoryNo ] ) - 1, 
					MailHistory[ aindex ].dateStr[ MailHistory[ aindex ].newHistoryNo ] );
	// ???
	getStringToken( text, '|', 2, sizeof( MailHistory[ aindex ].str[ MailHistory[ aindex ].newHistoryNo ] ) - 1, 
					MailHistory[ aindex ].str[ MailHistory[ aindex ].newHistoryNo ] );
	// ????????
	makeStringFromEscaped( MailHistory[ aindex ].str[ MailHistory[ aindex ].newHistoryNo ] );
	
	// ?????????????
	noReadFlag = getIntegerToken( text, '|', 3 );
	// ????????
	if ( noReadFlag != -1 ){
		// ?????????????
		MailHistory[ aindex ].noReadFlag[ MailHistory[ aindex ].newHistoryNo ] = noReadFlag;
		// ??????????
		MailHistory[ aindex ].petLevel[ MailHistory[ aindex ].newHistoryNo ] = getIntegerToken( text, '|', 4 );
		// ????䦶???
		getStringToken( text, '|', 5, sizeof( MailHistory[ aindex ].petName[ MailHistory[ aindex ].newHistoryNo ] ), 
						MailHistory[ aindex ].petName[ MailHistory[ aindex ].newHistoryNo ] );
		// ????䦶?????????
		makeStringFromEscaped( MailHistory[ aindex ].petName[ MailHistory[ aindex ].newHistoryNo ] );
		// ???????????????
		MailHistory[ aindex ].itemGraNo[ MailHistory[ aindex ].newHistoryNo ] = getIntegerToken( text, '|', 6 );
		// ????????????
		sprintf_s( moji,"收到%s送来的宠物邮件！", addressBook[ aindex ].name );
	}
	// ??????
	else{	
		MailHistory[ aindex ].noReadFlag[ MailHistory[ aindex ].newHistoryNo ] = TRUE;
		// ????????????
		sprintf_s( moji,"收到%s送来的邮件！", addressBook[ aindex ].name );
	}
	
	// ??????????????????
	StockChatBufferLine( moji, FONT_PAL_WHITE );
	
	// ????????
	if ( mailHistoryWndSelectNo == aindex ){
		// ??????????????
		mailHistoryWndPageNo++;
		// ????????
		if ( mailHistoryWndPageNo >= MAIL_MAX_HISTORY ) mailHistoryWndPageNo = 0;
		// ?????????
	//	DeathLetterAction();
	}
	// ?
	play_se( 101, 320, 240 );
	// ???????
	SaveMailHistory( aindex );
}


// ???????????
void lssproto_PS_recv( int fd,int result,int havepetindex,int havepetskill,int toindex )
{
	char moji[ 256 ];
	
	// ???????
	ItemMixRecvFlag = FALSE;
	
	// ???
	if ( result == 0 ){
		//???????
		sprintf_s( moji,"失败！");
		// ??????????????????
		StockChatBufferLine( moji, FONT_PAL_WHITE );
	}
	
}


// ?????????
void lssproto_SE_recv( int fd, int x, int y, int senumber, int sw )
{
	// ????????????????⿳?䥺???
	if ( logOutFlag )
		return;

	if ( sw )
	{
		play_se( senumber, x, y );
	}
	else
	{
		// ????????????
	}
}

// ??????????
void lssproto_XYD_recv( int fd, int x, int y, int dir )
{
	updateMapArea();
	setPcWarpPoint( x, y );
	setPcPoint();
	dir = (dir+3) % 8;
	setPcDir( dir );
}

void lssproto_WO_recv( int fd, int effect )
{
	return;
	// ???????
	if ( effect == 0 )
	{
		// ????????
		transmigrationEffectFlag = 1;
		// ?????????????
		transEffectPaletteStatus = 1;
		// ????????
		palNo = 15;
		palTime = 300;
	}
}

// ?????? /////////////////////////////////////////////////////////
void lssproto_Echo_recv( int fd, char *test )
{
#if 1
#ifdef _STONDEBUG__MSG

	// ?????
	time( &serverAliveLongTime );
	localtime_s(&serverAliveTime, &serverAliveLongTime );

#endif
#endif
}


// Robin 2001/04/06
void lssproto_NU_recv( int fd, int AddCount)
{

}


// ?????
void lssproto_PlayerNumGet_recv( int fd, int logincount, int player )
{
}


void lssproto_ProcGet_recv( int fd, char* data )
{
}




/*
   ?????⿳??????

*/
void lssproto_R_recv( int fd, char* data ) 
{
}


void lssproto_D_recv( int fd,int category,int dx,int dy,char* data ) 
{
}

/*
	???????????????????????
*/
void lssproto_ACI_recv( int fd, char* data )
{

}

extern void initFamilyList( char * );
extern void initFamilyDetail( char * );
extern void initJoinChannelWN( char * );
extern void initJoinChannel2WN( char * );
extern void initFamilyTaxWN( char * );
extern void initFamilyLeaderChange( char * );

void lssproto_FM_recv( int fd, char* data )
{
	char FMType1[512];
	char FMType2[512];
	char FMType3[512];
	
	getStringToken(data, '|', 1, sizeof( FMType1 ) - 1, FMType1 );
	//makeStringFromEscaped( FMType1 );
	getStringToken(data, '|', 2, sizeof( FMType2 ) - 1, FMType2 );
	//makeStringFromEscaped( FMType2 );

	if ( strcmp(FMType1,"S") ==0 )
	{
		if ( strcmp(FMType2,"F") ==0) // 家族列表
		{
			initFamilyList(data );

		}
		if ( strcmp(FMType2,"D") ==0) // 家族详细
		{
			initFamilyDetail(data );

		}

	}
	else if ( strcmp(FMType1,"C") ==0 )
	{
		if ( strcmp(FMType2,"J") ==0) // 加入频道
		{
			getStringToken(data, '|', 3, sizeof( FMType3 ) - 1, FMType3 );
			pc.channel = atoi( FMType3 );
			if ( pc.channel != -1 )
				pc.quickChannel = pc.channel;
		}
		if ( strcmp(FMType2,"L") ==0) // 频道列表
		{
			initJoinChannel2WN(data );

		}
	}
	else if ( strcmp(FMType1,"B") ==0 )
	{

		//MenuToggleFlag = JOY_CTRL_B;
		if ( strcmp(FMType2,"G") ==0)
		{
			//getStringToken(data, '|', 3, sizeof( FMType3 ) - 1, FMType3 );
			BankmanInit(data );
		}
		if ( strcmp(FMType2,"I") ==0)
		{
			//getStringToken(data, '|', 3, sizeof( FMType3 ) - 1, FMType3 );
			//ItemmanInit(data );
			//initItemman(data );
		}
		if ( strcmp(FMType2,"T") ==0)
		{
			initFamilyTaxWN(data );
		}


	}
	else if ( strcmp(FMType1,"R") ==0 )
	{
		if ( strcmp(FMType2,"P") ==0) // ride Pet
		{
			//initFamilyList(data );
			getStringToken(data, '|', 3, sizeof( FMType3 ) - 1, FMType3 );
			pc.ridePetNo = atoi( FMType3 );

		}

	}
	else if ( strcmp(FMType1,"L") ==0 )	// 族长功能
	{
		if ( strcmp(FMType2,"CHANGE") ==0)
		{
			initFamilyLeaderChange(data );
		}
	}

}


DWORD dwPingTime;
DWORD dwPingState;

#ifdef __NEW_CLIENT

#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0
#define ICMP_MIN 8 // minimum 8 byte icmp packet (just header)

/* The IP header */
typedef struct iphdr {
	unsigned int h_len:4;          // length of the header
	unsigned int version:4;        // Version of IP
	unsigned char tos;             // Type of service
	unsigned short total_len;      // total length of the packet
	unsigned short ident;          // unique identifier
	unsigned short frag_and_flags; // flags
	unsigned char  ttl; 
	unsigned char proto;           // protocol (TCP, UDP etc)
	unsigned short checksum;       // IP checksum

	unsigned int sourceIP;
	unsigned int destIP;

}IpHeader;

typedef struct _ihdr {
  BYTE i_type;
  BYTE i_code; /* type sub code */
  USHORT i_cksum;
  USHORT i_id;
  USHORT i_seq;
  /* This is not the std header, but we reserve space for time */
  ULONG timestamp;
}IcmpHeader;

#define STATUS_FAILED 0xFFFF
#define DEF_PACKET_SIZE 32
#define MAX_PACKET 1024

USHORT checksum(USHORT *buffer, int size)
{
	unsigned long cksum=0;
	while( size > 1){
		cksum += *buffer++;
		size -=sizeof(USHORT);
	}
	if ( size)
		cksum += *(UCHAR*)buffer;
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >>16);
	return (USHORT)(~cksum);
}

int iWrote, iRead;
SOCKET sockRaw = INVALID_SOCKET;
DWORD WINAPI PingFunc(LPVOID param)
{
	struct sockaddr_in from;
	struct sockaddr_in dest;
	int datasize;
	int fromlen = sizeof(from);
	int timeout = 1000;
	IcmpHeader *icmp_hdr;

	char icmp_data[MAX_PACKET];
	char recvbuf[MAX_PACKET];
	USHORT seq_no = 0;

	ZeroMemory( &dest, sizeof(dest));
	memcpy( &(dest.sin_addr), (void*)param, 4);
	dest.sin_family = AF_INET;
	if ( sockRaw != INVALID_SOCKET){
		closesocket( sockRaw);
		sockRaw = INVALID_SOCKET;
	}
	sockRaw = socket( AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if ( sockRaw != INVALID_SOCKET){
		iRead = setsockopt( sockRaw, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
		if ( iRead != SOCKET_ERROR){
			timeout = 1000;
			iRead = setsockopt( sockRaw, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
			if ( iRead != SOCKET_ERROR){
				datasize = DEF_PACKET_SIZE + sizeof(IcmpHeader);  

				icmp_hdr = (IcmpHeader*)icmp_data;
				icmp_hdr->i_type = ICMP_ECHO;
				icmp_hdr->i_code = 0;
				icmp_hdr->i_id = (USHORT)GetCurrentProcessId();
				icmp_hdr->i_cksum = 0;
				icmp_hdr->i_seq = 0;
				memset( icmp_data+sizeof(IcmpHeader), 'E', datasize - sizeof(IcmpHeader));
				while( 1){
					((IcmpHeader*)icmp_data)->i_cksum = 0;
					((IcmpHeader*)icmp_data)->timestamp = GetTickCount();
					
					((IcmpHeader*)icmp_data)->i_seq = seq_no;
					((IcmpHeader*)icmp_data)->i_cksum = checksum( (USHORT*)icmp_data, datasize);
					
					iWrote = sendto( sockRaw, icmp_data, datasize, 0, (struct sockaddr*)&dest, sizeof(struct sockaddr));
					if ( iWrote == SOCKET_ERROR){
						if ( WSAGetLastError() == WSAETIMEDOUT){
							dwPingTime = -1;
							continue;
						}
						dwPingState = 0x40000000 | WSAGetLastError();
						continue;
					}
					if ( iWrote < datasize)
						dwPingState = 0x04000000;//len error
					while( 1){
						iRead = recvfrom( sockRaw, recvbuf, MAX_PACKET, 0, (struct sockaddr*)&from, &fromlen);
						if ( iRead == SOCKET_ERROR){
							if ( WSAGetLastError() == WSAETIMEDOUT){
								dwPingTime = -1;
								break;
							}
							dwPingState = 0x40000000 | WSAGetLastError();
							continue;
						}
						IpHeader *iphdr;
						unsigned short iphdrlen;
						iphdr = (IpHeader *)recvbuf;
						iphdrlen = iphdr->h_len * 4 ; // number of 32-bit words *4 = bytes
						if ( iRead < iphdrlen + ICMP_MIN)
							dwPingState = 0x08000000;//len error
						icmp_hdr = (IcmpHeader*)(recvbuf + iphdrlen);
						if ( icmp_hdr->i_type != ICMP_ECHOREPLY){
							dwPingState = 0x10000000;//non-echo type recvd
							continue;
						}
						if ( icmp_hdr->i_seq != seq_no)
							continue;
						if ( icmp_hdr->i_id != (USHORT)GetCurrentProcessId()){
							dwPingState = 0x20000000;//someone else's packet!
							continue;
						}
						dwPingTime = GetTickCount()-icmp_hdr->timestamp;
						break;
					}
					seq_no++;
					dwPingState = 0x80000000;
					Sleep( 3000);
				}
			}
		}
	}
	return 0;
}

#endif
#ifdef _ITEM_CRACKER 
extern bool m_bt;
void lssproto_IC_recv(int fd, int x, int y)
{
	m_bt = true; setCharMind( pc.ptAct, SPR_cracker);	
	m_bt = false;
}
#endif

#ifdef _CHECK_GAMESPEED
static int delaytimes=0;
void lssproto_CS_recv( int fd, int deltimes)
{
	delaytimes = deltimes;
}
int lssproto_getdelaytimes()
{
	if ( delaytimes < 0 ) delaytimes = 0; 
	return delaytimes;
}
void lssproto_setdelaytimes( int delays)
{
	delaytimes = delays; 
}
#endif

#ifdef _MAGIC_NOCAST//沉默
void lssproto_NC_recv( int fd, int flg )
{  
	if (flg==1 )
		NoCastFlag=TRUE;
    else 
		NoCastFlag=FALSE;
}
#endif
#ifdef _CHANNEL_MODIFY
// 储存对话内容
FILE *pSaveChatDataFile[6]={NULL,NULL,NULL,NULL,NULL,NULL};
void SaveChatData(char *msg,char KindOfChannel,bool bCloseFile){
	static char szFileName[256];
	static struct tm nowTime;
	static time_t longTime;
	static unsigned short Channel[] = {
		PC_ETCFLAG_CHAT_MODE	//队伍频道开关
		,PC_ETCFLAG_CHAT_TELL	//密语频道开关
		,PC_ETCFLAG_CHAT_FM		//家族频道开关
#ifdef _CHAR_PROFESSION
		,PC_ETCFLAG_CHAT_OCC	//职业频道开关
#endif
#ifdef _CHATROOMPROTOCOL
		,PC_ETCFLAG_CHAT_CHAT	//聊天室开关
#endif
#ifdef _CHANNEL_WORLD
		,PC_ETCFLAG_CHAT_WORLD	//世界频道开关
#endif
#ifdef _CHANNEL_ALL_SERV
		,PC_ETCFLAG_ALL_SERV	//星球频道开关
#endif
	};
	char ChannelType[] = {'T','M','F',
#ifdef _CHAR_PROFESSION
		'O',
#endif
		'R',
#ifdef _CHANNEL_WORLD
		'W',
#endif
#ifdef _CHANNEL_ALL_SERV
		'S',
#endif
	};

	// 储存对话内容选项开启
	if ((pc.etcFlag & PC_ETCFLAG_CHAT_SAVE) && !bCloseFile){
		time(&longTime);
		localtime_s(&nowTime,&longTime);
		for (int i=0;i<6;i++){
			if (pc.etcFlag & Channel[i]){
				if (pSaveChatDataFile[i] == NULL){
					sprintf_s(szFileName,"chat\\%c%02d%02d%02d.TXT",ChannelType[i],(nowTime.tm_year % 100),nowTime.tm_mon+1,nowTime.tm_mday);
					if ((pSaveChatDataFile[i] = fopen(szFileName,"a")) == NULL) continue;
				}
			}
			else{
				if (pSaveChatDataFile[i] != NULL){
					fclose(pSaveChatDataFile[i]);
					pSaveChatDataFile[i]=NULL;
				}
			}
		}
		for (int i=0;i<6;i++){
			if (KindOfChannel == ChannelType[i]){
				if (pSaveChatDataFile[i] != NULL) fprintf(pSaveChatDataFile[i],"[%02d:%02d:%02d]%s\n",nowTime.tm_hour,nowTime.tm_min,nowTime.tm_sec,msg);
			}
		}
	}
	else bCloseFile = 1;

	if (bCloseFile){
		for (int i=0;i<6;i++)
			if (pSaveChatDataFile[i] != NULL){
				fclose(pSaveChatDataFile[i]);
				pSaveChatDataFile[i]=NULL;
			}
	}
}
#endif

#ifdef _STREET_VENDOR
extern short sStreetVendorBtn;
extern short sStreetVendorBuyBtn;
extern void StreetVendorWndfunc(bool bReset,char *data);
extern void StreetVendorBuyWndfunc(char *data);

void lssproto_STREET_VENDOR_recv(int fd,char *data)
{
	char szMessage[32];

	getStringToken(data,'|',1,sizeof(szMessage) - 1,szMessage);
	switch (szMessage[0]){
		// 开新摆摊介面
		case 'O':
			sStreetVendorBtn = 1;
			pc.iOnStreetVendor = 1;
			break;
		// 设定摆摊内容
		case 'S':
			sStreetVendorBtn = 3;
			StreetVendorWndfunc(false,data);
			break;
		// server送来的卖方贩卖内容
		case 'B':
			sStreetVendorBuyBtn = 1;
			StreetVendorBuyWndfunc(data);
			break;
		// server 送来关闭视窗
		case 'C':
			sStreetVendorBuyBtn = 0;
			break;
		// server 送来的单笔贩卖物详细资料
		case 'D':
			StreetVendorBuyWndfunc(data);
			break;
	}
}
#endif


#ifdef _STONDEBUG_ // 手动送出封包功能 Robin
/* 
	(封包编号)`d`(数值资料)`s`(字串资料)`......
	例: 35`d`100`d`100`s`P|Hellp~~`d`1`d`1
*/
void sendDataToServer( char* data)
{
	char token[1024];
	char token2[1024];
	char token3[1024];
	char sendbuf[16384] ="";
	char showbuf[16384] ="";
	char showsubbuf[1024];
	int checksum=0;
	int datakind;
	int i =1;

	strcat_s( showbuf, "手动送出 ");

	getStringToken(data, '`', i++, sizeof(token), token );
	if ( token[0] == NULL )	return;
	datakind = atoi( token);
	sprintf_s( showsubbuf, "封包=%d ", datakind);
	strcat_s( showbuf, showsubbuf);

	while(1) {

		getStringToken(data, '`', i++, sizeof(token2), token2 );
		if ( token2[0] == NULL )	break;
		getStringToken(data, '`', i++, sizeof(token3), token3 );
		if ( token3[0] == NULL )	break;

		if ( !strcmp( token2, "d") ) {
			checksum += util_mkint( sendbuf, atoi(token3));
			sprintf_s( showsubbuf, "数=%d ", atoi(token3));
			strcat_s( showbuf, showsubbuf);
		}
		else if ( !strcmp( token2, "s") ) {
			checksum += util_mkstring( sendbuf, token3);
			sprintf_s( showsubbuf, "字=%s ", token3);
			strcat_s( showbuf, showsubbuf);
		}
		else {
			break;
		}

	}

	util_mkint( sendbuf, checksum);
	util_SendMesg(sockfd, datakind, sendbuf);

	StockChatBufferLine( showbuf, FONT_PAL_RED);

}
#endif
#ifdef _FAMILYBADGE_
extern int 徽章数据[];
extern int 徽章个数;
int 徽章价格;
void lssproto_FamilyBadge_recv(char *data)
{
	徽章个数=0;
	int  i=2;
	徽章价格 = getIntegerToken(data,'|',1);
	for(i;i<201;i++){
		徽章数据[i-2] = getIntegerToken(data,'|',i);
		if(徽章数据[i-2]==-1) break;
		徽章个数++;
	}
}
#endif


#ifdef _JOBDAILY
extern JOBDAILY jobdaily[MAXMISSION];
extern int JobdailyGetMax;  //是否有接收到资料
void lssproto_JOBDAILY_recv(int fd,char *data)
{
	//解读资料
	int  i=1,j=1;
	char getdata[250];
	char perdata[200];

	//StockChatBufferLine(data,FONT_PAL_RED); 

	memset(jobdaily,0,sizeof(jobdaily));
	getdata[0] = '\0';
	perdata[0] = '\0';
	while(getStringToken(data,'#',i,sizeof(getdata) - 1,getdata)!=1){
		while(getStringToken(getdata,'|',j,sizeof(perdata) - 1,perdata)!=1){
			switch (j){
			case 1: jobdaily[i-1].JobId = atoi(perdata);break;
			case 2: strcpy(jobdaily[i-1].explain,perdata); break;
			case 3: strcpy(jobdaily[i-1].state,perdata); break;
			default: StockChatBufferLine("每笔资料内参数有错误",FONT_PAL_RED); break;
			}
			perdata[0] = '\0';
			j++;
		}
		getdata[0] = '\0';
		j=1;
		i++;
	}
	if (i>1)
		JobdailyGetMax= i-2;
	else JobdailyGetMax = -1;
}
	
#endif

#ifdef _TEACHER_SYSTEM
extern short sTeacherSystemBtn;
extern void TeacherSystemWndfunc(int flag,char *data);
void lssproto_TEACHER_SYSTEM_recv(int fd,char *data)
{
	char szMessage[16];

	getStringToken(data,'|',1,sizeof(szMessage) - 1,szMessage);
	switch (szMessage[0]){
		// 显示说明
		case 'M':sTeacherSystemBtn = 1;break;
		// 询问是否要对方当你的导师
		case 'C':
			sTeacherSystemBtn = 2;
			TeacherSystemWndfunc(0,data);
			break;
		// 超过一人,询问要找谁当导师
		case 'A':
			sTeacherSystemBtn = 3;
			TeacherSystemWndfunc(1,data);
			break;
		// 显示导师资料
		case 'V':
			sTeacherSystemBtn = 4;
			TeacherSystemWndfunc(2,data);
			break;
#ifdef _TEACHER_SYSTEM_2
		case 'S':
			sTeacherSystemBtn = 6;
			TeacherSystemWndfunc(4,data);
			break;
#endif
	}	
}
#endif

#ifdef _ADD_STATUS_2
void lssproto_S2_recv(int fd,char *data)
{
	char szMessage[16];
#ifdef _EVIL_KILL
	int ftype = 0 , newfame = 0;
#endif

	getStringToken(data,'|',1,sizeof(szMessage) - 1,szMessage);

#ifdef _NEW_MANOR_LAW
	if (strcmp(szMessage,"FAME") == 0){
		getStringToken(data,'|',2,sizeof(szMessage) - 1,szMessage);
		pc.fame = atoi(szMessage);
	}
#endif

#ifdef _EVIL_KILL	
	pc.ftype = 0;
	pc.newfame = 0;

	if( getStringToken(data,'|',3,sizeof(szMessage) - 1,szMessage) == 1 ) return;
		ftype = atoi(szMessage);
	getStringToken(data,'|',4,sizeof(szMessage) - 1,szMessage);
	newfame = atoi(szMessage);
	pc.ftype = ftype;
	pc.newfame = newfame;	
#endif
}
#endif

#ifdef _ITEM_FIREWORK
void lssproto_Firework_recv(int fd, int nCharaindex, int nType, int nActionNum)
{
	ACTION	*pAct;

	if (pc.id == nCharaindex)
		changePcAct(0, 0, 0, 51, nType, nActionNum, 0);
	else
	{
		pAct = getCharObjAct(nCharaindex);
		changeCharAct(pAct, 0, 0, 0, 51, nType, nActionNum, 0);
	
	}
}
#endif

#ifdef _MOVE_SCREEN
// client 移动荧幕
void lssproto_MoveScreen_recv(int fd, BOOL bMoveScreenMode, int iXY)
{
	pc.bMoveScreenMode = bMoveScreenMode;
	pc.bCanUseMouse = bMoveScreenMode;
	if (bMoveScreenMode)
	{
		pc.iDestX = HIWORD(iXY);
		pc.iDestY = LOWORD(iXY);
	}
	else
		iScreenMoveX = iScreenMoveY = 0;
}
#endif

#ifdef _THEATER
void lssproto_TheaterData_recv(int fd, char *pData)
{
	int				iType, iData;
	char			szMessage[16];
	float			fX, fY;

	getStringToken(pData, '|', 1, sizeof(szMessage) - 1, szMessage);
	iType = atoi(szMessage);
	getStringToken(pData, '|', 2, sizeof(szMessage) - 1, szMessage);
	iData = atoi(szMessage);
	switch (iType)
	{
	case E_THEATER_SEND_DATA_THEATER_MODE:
		pc.iTheaterMode = iData;
		if (iData == 0)
		{
			pc.bCanUseMouse = FALSE;		// 表演完毕,可以正常使用滑鼠移动
			pc.iSceneryNumber = -1;
		}
		else
		{
			pc.bCanUseMouse = TRUE;	// 表演中
			pc.iSceneryNumber = 26558;
		}
		break;
	case E_DATA_TYPE_MOVE:		// 移动
		camMapToGamen((float)(HIWORD(iData) * GRID_SIZE), float(LOWORD(iData) * GRID_SIZE), &fX, &fY);
		MouseNowPoint((int)(fX + 0.5f), (int)(fY + 0.5f));
		MouseCrickLeftDownPoint((int)(fX + 0.5f), (int)(fY + 0.5f));
		MouseCrickLeftUpPoint((int)(fX + 0.5f), (int)(fY + 0.5f));
		pc.bCanUseMouse = FALSE;			// 设为 FALSE,不然人物不能移动
		mouse.level = DISP_PRIO_TILE;
		closeCharActionAnimeChange();
		break;
	case E_DATA_TYPE_DIR:		// 方向
		setPcDir(iData);
		szMessage[0] = cnvServDir(iData, 1 );
		szMessage[1] = '\0';
		walkSendForServer(nowGx, nowGy, szMessage);
		break;
	case E_DATA_TYPE_SCENERY:	// 布景
		pc.iSceneryNumber = iData;
		break;
	case E_DATA_TYPE_BGM:		// 背景音乐
		play_bgm(iData);
		break;
	case E_THEATER_SEND_DATA_DISPLAY_SCORE:		// 显示分数
		pc.iTheaterMode |= 0x00000004;
		pc.iTheaterMode |= iData << 16;			// iData 是分数值
		break;
	case E_DATA_TYPE_NPC:						// 产生或是消失或更改临时NPC
		// 当 iType 为 E_DATA_TYPE_NPC 时取出来的 iData 是 NPC 编号
		if (iData >= 0 && iData < 5)
		{
			int iSprNum, iGX, iGY, iAction, iDir;

			getStringToken(pData, '|', 3, sizeof(szMessage) - 1, szMessage);	// 取出指令
			if (atoi(szMessage) == 1)
			{
				getStringToken(pData, '|', 4, sizeof(szMessage) - 1, szMessage);	// 取出图号
				iSprNum = atoi(szMessage);
				getStringToken(pData, '|', 5, sizeof(szMessage) - 1, szMessage);	// 取出座标
				iGX = atoi(szMessage);
				getStringToken(pData, '|', 6, sizeof(szMessage) - 1, szMessage);	// 取出座标
				iGY = atoi(szMessage);
				getStringToken(pData, '|', 7, sizeof(szMessage) - 1, szMessage);	// 取出动作
				iAction = atoi(szMessage);
				getStringToken(pData, '|', 8, sizeof(szMessage) - 1, szMessage);	// 取出方向
				iDir = atoi(szMessage);
				camMapToGamen((float)iGX * GRID_SIZE, (float)iGY * GRID_SIZE, &fX, &fY);
				if (pc.pActNPC[iData] == NULL)
				{
					pc.pActNPC[iData] = MakeAnimDisp((int)fX, (int)fY, iSprNum, ANIM_DISP_THEATER_NPC);
					ATR_DISP_PRIO(pc.pActNPC[iData]) = DISP_PRIO_CHAR - 1;
				}
				if (pc.iTheaterMode & 0x00000001)
				{
					iGX -= iScreenMoveX;
					iGY -= iScreenMoveY;
				}
				pc.pActNPC[iData]->gx = iGX;
				pc.pActNPC[iData]->gy = iGY;
				pc.pActNPC[iData]->mx = (float)(iGX * GRID_SIZE);
				pc.pActNPC[iData]->my = (float)(iGY * GRID_SIZE);
				ATR_CHR_NO(pc.pActNPC[iData]) = iSprNum;
				ATR_CHR_ACT(pc.pActNPC[iData]) = iAction;
				ATR_CHR_ANG(pc.pActNPC[iData]) = iDir;
			}
			else
			{
				if (pc.pActNPC[iData])
				{
					DeathAction(pc.pActNPC[iData]);
					pc.pActNPC[iData] = NULL;
				}
			}
		}
		break;
	}
}
#endif

#ifdef _NPC_MAGICCARD

bool bShowflag[20];
int iShowdamage[20];
int iPosition[20];	
int iOffsetY[20];
int ioffsetsx = 0;
int ioffsetsy = 0;
unsigned int inextexet = 0;

void lssproto_MagiccardDamage_recv(int fd, int position , int damage , int offsetx , int offsety )
{
	int i;
	
	if( position == 10 ) 
	{
		wnCloseFlag = 1;
		return;
	}

	for ( i=0;i<20;i++ ){
		if ( bShowflag[i]==TRUE) continue;
		iPosition[i] = position;
		iShowdamage[i] = damage;
		bShowflag[i] = TRUE;
		break;
	}
	ioffsetsx = offsetx;
	ioffsetsy = offsety;
}

void lssproto_MagiccardAction_recv(int fd, char *data)
{
	ACTION	*pAct;
	int  charaindex,player,card,dir,actionNum,action,offsetx,offsety;
	char token[2048];

	getStringToken(data , '|', 1, sizeof( token ) - 1, token );
	charaindex = atoi( token );
	getStringToken(data, '|', 2, sizeof( token ) - 1, token );
	player = atoi( token );
	getStringToken(data, '|', 3, sizeof( token ) - 1, token );	
	card = atoi(token);
	getStringToken(data, '|', 4, sizeof( token ) - 1, token );
	dir = (atoi( token )+3)%8;
	getStringToken(data, '|', 5, sizeof( token ) - 1, token );
	actionNum = atoi( token );	//图号
	getStringToken(data, '|', 6, sizeof( token ) - 1, token );
	action = atoi( token );
	getStringToken(data, '|', 7, sizeof( token ) - 1, token );
	offsetx = atoi( token );
	getStringToken(data, '|', 8, sizeof( token ) - 1, token );
	offsety = atoi( token );

	//if ( actionNum == 101652 ){
	//	dir = 1;	
	//	dir = 2;
	//}
	if (pc.id == charaindex)
		//changePcAct(0, 0, 0, 51, nType, nActionNum, 0);
		changePcAct( offsetx , offsety , dir, 60 , player , actionNum , action);
	else{	
		pAct = getCharObjAct(charaindex);
		//changeCharAct( ptAct, x, y, dir, act, effectno, effectparam1, effectparam2 );	
		changeCharAct(pAct, offsetx , offsety , dir, 60, player , actionNum , action );
	}
}
#endif

#ifdef _NPC_DANCE
void lssproto_DancemanOption_recv( int fd , int option )
{
	switch( option )
	{
	case 0:	//关闭视窗
		wnCloseFlag = 1;
		break;
	case 1: //开启动一动模式
		pc.iDanceMode = 1;
		break;
	case 2: //关闭动一动模式
		pc.iDanceMode = 0;
		break;
	}
}
#endif

#ifdef _HUNDRED_KILL
void lssproto_hundredkill_recv( int fd, int flag ){

	if ( flag == 1 )
		BattleHundredFlag = TRUE;
	else
		BattleHundredFlag = FALSE;
}
#endif

#ifdef _ANNOUNCEMENT_

void lssproto_DENGON_recv(char *data, int colors, int nums)
{
	extern int 公告数量;
	extern char 公告内容[512];
	extern int 公告颜色;
	extern int 公告时间;
	公告时间=0;
	sprintf(公告内容, "%s", data);
	公告颜色 = colors;
	公告数量 = nums;
}
#endif


#ifdef _PET_SKINS

void lssproto_PetSkins_recv(char *data)
{
	char *str = "宠物栏位置|当前使用皮肤图号|总皮肤数|皮肤图号|说明|皮肤图号|说明|...";
}

#endif



