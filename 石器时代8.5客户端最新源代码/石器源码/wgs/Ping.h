//ping.h
#ifndef _CPING_H_
#define _CPING_H_
#pragma pack(1)

#define ICMP_ECHOREPLY  0
#define ICMP_ECHOREQ    8
#define REQ_DATASIZE    32      // Echo 请求数据的大小

class CPing
{
public:
	//ping host， nRetries-ping次数
	bool Ping(LPCSTR pstrHost, UINT nRetries = 4);

	void Result(int* nElapseTime, float* fMissPack = NULL, u_char* cTTL = NULL);
	//void Result(CPing::REQ_RESULT& result);

private:
	int  WaitForEchoReply(SOCKET s);
	//ICMP回应的请求和回答函数
	int     SendEchoRequest(SOCKET, LPSOCKADDR_IN);
	DWORD   RecvEchoReply(SOCKET, LPSOCKADDR_IN, u_char *);
	u_short in_cksum(u_short *addr, int len);

private:
	struct REQ_RESULT
	{
		int         nElapseTime;    //请求响应时间。
		u_char      cTTL;           //请求TTL(生存时间)
		float       fMissPack;  //丢包率
	};

	REQ_RESULT m_Result;

};



// IP Header -- RFC 791
typedef struct tagIPHDR
{
	u_char  VIHL;           // Version and IHL
	u_char  TOS;            // Type Of Service
	short   TotLen;         // Total Length
	short   ID;             // Identification
	short   FlagOff;        // Flags and Fragment Offset
	u_char  TTL;            // Time To Live
	u_char  Protocol;       // Protocol
	u_short Checksum;       // Checksum
	struct  in_addr iaSrc;  // Internet Address - Source
	struct  in_addr iaDst;  // Internet Address - Destination
}IPHDR, *PIPHDR;


// ICMP Header - RFC 792
typedef struct tagICMPHDR
{
	u_char  Type;           // Type
	u_char  Code;           // Code
	u_short Checksum;       // Checksum
	u_short ID;             // Identification
	u_short Seq;            // Sequence
	char    Data;           // Data
}ICMPHDR, *PICMPHDR;


// ICMP Echo Request
typedef struct tagECHOREQUEST
{
	ICMPHDR icmpHdr;
	DWORD   dwTime;
	char    cData[REQ_DATASIZE];
}ECHOREQUEST, *PECHOREQUEST;


// ICMP Echo Reply
typedef struct tagECHOREPLY
{
	IPHDR   ipHdr;
	ECHOREQUEST echoRequest;
	char    cFiller[256];
}ECHOREPLY, *PECHOREPLY;

#pragma pack()

#endif
