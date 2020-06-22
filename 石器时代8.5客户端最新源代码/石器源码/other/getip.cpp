#include "../systeminc/version.h"

#ifdef _LOGIP_
#include <WinSock.h>
int SendData(SOCKET s, char * data)
{
	return send(s, data, strlen(data), 0);
}

BOOL ªÒ»°IP(char *IP)
{ 
  WSAData wsaDat;
  short int Port = 80;
  if(WSAStartup(MAKEWORD(1,1),&wsaDat)) return FALSE;
  
  SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
  if(sd ==INVALID_SOCKET) return FALSE;
  SOCKADDR_IN sockAddr;
  memset((void*)&sockAddr,0,sizeof(sockAddr));
  sockAddr.sin_family=AF_INET;
  sockAddr.sin_port=htons(Port);
  sockAddr.sin_addr.s_addr=inet_addr("115.238.240.61");
  if(connect(sd,(LPSOCKADDR)&sockAddr,sizeof(sockAddr))){
	closesocket(sd);
	WSACleanup();
	return	FALSE;
  }

  SendData(sd, "GET /getip.php HTTP/1.1\r\n");
  SendData(sd, "Host:www.shiqi.hk\r\n");
  SendData(sd, "Accept: */*\r\n");
  SendData(sd, "User-Agent: Mozilla/4.0(compatible; MSIE 5.00; Windows NT)\r\n");
  SendData(sd, "\r\n");
  SendData(sd, "\r\n");

  char recvstr[1024];
  memset(recvstr,0,1024);
  char *p = recvstr;
  char* iIndex_Head ;
  int iLen = 1024;
  while (iLen){
    int iRet = recv(sd,p,iLen,0);
    if (iRet <= 0) {
		  closesocket(sd);
		  WSACleanup();
        return FALSE;
    }
    p += iRet;
    iLen -= iRet;
    if ((iIndex_Head = strstr(recvstr,"°æ"))){
		char* iIndex = strstr(recvstr,"°ø");
		if (!iIndex) {
          continue;
		}
		char *p1 =  iIndex_Head+2;
		char *p2 = iIndex;
		memcpy(IP,p1,p2-p1);
		break;
    }
  }
  closesocket(sd);
  WSACleanup();
  return TRUE;
}

#endif

