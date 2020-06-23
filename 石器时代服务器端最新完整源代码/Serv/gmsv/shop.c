#include "version.h"
#include "autil.h"
#include "lssproto_serv.h"
#include "common.h"
#include <stdio.h>
#include "buf.h"
#include "md5.h"
#include "shop.h"

typedef struct tagShopData{
	char shopMd5[64];
	char *shopData;
}tagShopData1;

tagShopData1 shopData[5];

void ShopData_Init()
{
	memset(&shopData,0,sizeof(tagShopData1)*5);
}

void WriteShopData(char *data,int id)
{
	char szDigest[17];
	int len=strlen(data)+1;
	if(shopData[id].shopData){
		freeMemory(shopData[id].shopData);
		shopData[id].shopData=NULL;
	}
	if(*data!=0){
		shopData[id].shopData = (char*)allocateMemory(len);
		memset(shopData[id].shopData,0,len);
		memcpy(shopData[id].shopData,data,len);
		MD5Digest(shopData[id].shopData,len-1,szDigest);
		szDigest[16]='\0';
		int i ;
		for(i=0;i<16;i++)
		{
			snprintf(shopData[id].shopMd5+i*2,sizeof(shopData[id].shopMd5+i*2),"%02X",(unsigned char)szDigest[i]);
		}
		shopData[id].shopMd5[32]='\0';
	}else{
		shopData[id].shopMd5[0]=0;
	}
}


void lssproto_upshopdata_recv(int fd,char shop[5][1024])
{
	int i=0;
	for(i;i<5;i++){
		if(*shopData[i].shopMd5){
			if(strcmp(shopData[i].shopMd5,shop[i]) || !shop[i]){
				if(shopData[i].shopData){
					lssproto_UpShopData_send(fd,shopData[i].shopData,shopData[i].shopMd5,i);
				}
			}
		}
	}
	lssproto_ShopOK_send(fd);
}




