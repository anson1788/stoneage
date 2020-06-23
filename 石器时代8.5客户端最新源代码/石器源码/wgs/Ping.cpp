//ping.cpp
#include <Winsock2.h>
#include <Windows.h>
#include "ping.h"
#pragma comment(lib, "ws2_32.lib")

bool CPing::Ping(LPCSTR pstrHost, UINT nRetries)
{
	//创建一个Raw套节字
	SOCKET rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (rawSocket == INVALID_SOCKET)
	{
		int err = WSAGetLastError();
		return false;
	}
	int nNetTimeout = 1000;//1秒 
						   //发送时限
	setsockopt(rawSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
	//接收时限
	setsockopt(rawSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

	//获得主机信息
	LPHOSTENT lpHost = gethostbyname(pstrHost);
	if (lpHost == NULL)
	{
		return false;
	}
	//构造目标套节字地址信息
	struct    sockaddr_in saDest;
	struct    sockaddr_in saSrc;
	saDest.sin_addr.s_addr = *((u_long FAR *) (lpHost->h_addr));
	saDest.sin_family = AF_INET;
	saDest.sin_port = 3077;

	DWORD    dwTimeSent;
	u_char   cTTL;
	int     nRet;
	int     nRecvNum = 0;
	int     nTotalTime = 0;

	//多次ping
	for (UINT nLoop = 0; nLoop < nRetries; ++nLoop)
	{
		//发送ICMP回应请求
		if ((nRet = SendEchoRequest(rawSocket, &saDest)) < 0)
		{
			break;
		}

		if ((nRet = WaitForEchoReply(rawSocket)) == SOCKET_ERROR)
		{
			break;
		}

		if (nRet)
		{
			//获得回应
			if ((dwTimeSent = RecvEchoReply(rawSocket, &saSrc, &cTTL)) < 0)
			{
				nRet = dwTimeSent;
				break;
			}

			//计算时间
			nTotalTime += GetTickCount() - dwTimeSent;
			++nRecvNum;
		}
	}

	closesocket(rawSocket);

	if (nRecvNum > 0 && nRet >= 0)
	{
		m_Result.nElapseTime = nTotalTime / nRetries;
		m_Result.cTTL = cTTL;
		m_Result.fMissPack = (float)(nRetries - nRecvNum) / nRetries;
		return true;
	}
	return false;
}



//发送ICMPECHO数据包请求
int CPing::SendEchoRequest(SOCKET s, LPSOCKADDR_IN lpstToAddr)
{
	static ECHOREQUEST echoReq;
	static int nId = 1;
	static int nSeq = 1;
	int nRet;

	//构造回应请求
	echoReq.icmpHdr.Type = ICMP_ECHOREQ;
	echoReq.icmpHdr.Code = 0;
	echoReq.icmpHdr.Checksum = 0;
	echoReq.icmpHdr.ID = nId++;
	echoReq.icmpHdr.Seq = nSeq++;

	for (nRet = 0; nRet < REQ_DATASIZE; nRet++)
		echoReq.cData[nRet] = ' ' + nRet;

	//保存发送时间
	echoReq.dwTime = GetTickCount();

	echoReq.icmpHdr.Checksum = in_cksum((u_short *)&echoReq, sizeof(ECHOREQUEST));

	//发送请求
	nRet = sendto(s,
		(LPSTR)&echoReq,
		sizeof(ECHOREQUEST),
		0,
		(LPSOCKADDR)lpstToAddr,
		sizeof(SOCKADDR_IN));
	//检查返回值
	if (nRet == SOCKET_ERROR)
	{

	}

	return (nRet);
}



//接收ICMPECHO数据包回应
DWORD CPing::RecvEchoReply(SOCKET s, LPSOCKADDR_IN lpsaFrom, u_char *pTTL)
{
	ECHOREPLY echoReply;
	int nRet;
	int nAddrLen = sizeof(struct sockaddr_in);

	//接收请求回应
	nRet = recvfrom(s,
		(LPSTR)&echoReply,
		sizeof(ECHOREPLY),
		0,
		(LPSOCKADDR)lpsaFrom,
		&nAddrLen);

	//检查返回值
	if (nRet == SOCKET_ERROR)
	{
		return nRet;
	}

	//返回发送的时间
	*pTTL = echoReply.ipHdr.TTL;

	return(echoReply.echoRequest.dwTime);
}



//等待回应
int CPing::WaitForEchoReply(SOCKET s)
{
	struct timeval Timeout;
	fd_set readfds;

	FD_ZERO(&readfds);
	FD_SET(s,&readfds);
	Timeout.tv_sec = 1;
	Timeout.tv_usec = 0;

	int ret = select(s+1, &readfds, NULL, NULL, &Timeout);

	return ret;
}



//转换地址
u_short CPing::in_cksum(u_short *addr, int len)
{
	register int nleft = len;
	register u_short *w = addr;
	register u_short answer;
	register int sum = 0;

	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1) {
		u_short u = 0;

		*(u_char *)(&u) = *(u_char *)w;
		sum += u;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}



void CPing::Result(int* nElapseTime, float* fMissPack, u_char* cTTL)
{
	if (nElapseTime)
	{
		*nElapseTime = m_Result.nElapseTime;
	}

	if (fMissPack)
	{
		*fMissPack = m_Result.fMissPack;
	}
	if (cTTL)
	{
		*cTTL = m_Result.cTTL;
	}
}



