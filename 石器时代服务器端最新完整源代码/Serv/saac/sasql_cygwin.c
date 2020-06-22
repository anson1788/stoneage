#define _SASQL_C_

#include "version.h"

#ifdef _SASQL//新添加

#include "main.h"

#include <windows.h>
#include <w32api/sql.h>
#include <w32api/sqlext.h>

HENV henv = SQL_NULL_HENV;
HDBC hdbc = SQL_NULL_HDBC;
HSTMT hstmt = SQL_NULL_HSTMT;
RETCODE rc;

	SQLCHAR sid[16]; 
	SQLCHAR sps[16]; 
	SQLINTEGER err;

typedef struct tagConfig
{
    char    SQL_DSN[16]; 

    char    SQL_USER[16];

    char    SQL_INFOTABLE[16];    

		char    SQL_LOCK[16];   
		
		char    SQL_RegTime[16]; 
		
		char    SQL_LoginTime[16]; 
		
		char    SQL_OnlineName[16]; 
		
}Config;

Config config;

int AUTOREG;
int USEMSSQL;
void check_return (RETCODE rc,HENV henv,HDBC hdbc,HSTMT hstmt );

static int readConfig( char *path )
{
    char buf[255];
    FILE *fp;

    fp = fopen( path , "r" );
    if( fp == NULL ){ return -2; }

    while( fgets( buf , sizeof( buf ) , fp )){
        char command[255];
        char param[255];
        chop(buf);
        
        easyGetTokenFromString( buf , 1 , command , sizeof( command ));
        easyGetTokenFromString( buf , 2 , param , sizeof( param ));

				if( strcmp( command , "USEMSSQL" ) == 0 ){
        		USEMSSQL = atoi( param );
        		if(USEMSSQL==0)break;
      	}
        if( strcmp( command , "SQL_DSN" ) == 0 ){
            snprintf( config.SQL_DSN , sizeof( config.SQL_DSN) , param) ;
            log("数据库 DSN：%s\n",config.SQL_DSN);
        } else if( strcmp( command , "SQL_INFOTABLE" ) == 0 ){
            snprintf( config.SQL_INFOTABLE , sizeof( config.SQL_INFOTABLE) , param) ;
				  	log("人物信息表：%s\n",config.SQL_INFOTABLE);
        } else if( strcmp( command , "SQL_LOCK" ) == 0 ){
            snprintf( config.SQL_LOCK , sizeof( config.SQL_LOCK) , param) ;
						log("锁定账号表：%s\n",config.SQL_LOCK);
        } else if( strcmp( command , "SQL_REGTIME" ) == 0 ){
            snprintf( config.SQL_RegTime , sizeof( config.SQL_RegTime) , param) ;
						log("注册时间：%s\n",config.SQL_RegTime);
				} else if( strcmp( command , "AUTOREG" ) == 0 ){
        		AUTOREG = atoi( param );
        		if(AUTOREG){
        			log("开放自动注册：YES\n");
        		}else{
        			log("开放自动注册：NO\n");
        		}
      	}
    }
   	fclose(fp);
    return 0;
}

BOOL sasql_init( void )
{

  readConfig("acserv.cf");
  if(USEMSSQL==0)return TRUE;
	char buf[257];
	char ODBC[257];
	short buflen;
//	printf ("Initialize the environment structure.\n");
	SQLAllocEnv (&henv);
	
//	printf ("Initialize the connection structure.\n");
	SQLAllocConnect (henv,&hdbc);
	
	printf("连接ODBC驱动器...");
	sprintf(ODBC,"DSN=%s",config.SQL_DSN);

	rc = SQLDriverConnect(hdbc,0,ODBC,SQL_NTS,(UCHAR*) buf,sizeof (buf),&buflen,SQL_DRIVER_COMPLETE);
	check_return (rc,henv,hdbc,hstmt);
	printf ("成功\n");
	SQLAllocStmt (hdbc,&hstmt);
	SQLBindCol(hstmt, 1, SQL_C_CHAR,&sid,16, &err);
	SQLBindCol(hstmt, 2, SQL_C_CHAR,&sps,16, &err);
}

void sasql_close( void )
{
	if(USEMSSQL==0)return;
//	printf ("Free the statement handle.\n");
	SQLFreeStmt (hstmt,SQL_DROP);
	
//	printf ("Disconnect from the data source.\n");
	SQLDisconnect (hdbc);
	
//	printf ("Free the connection structure.\n");
	SQLFreeConnect (hdbc);
	
//	printf ("Free the environment structure.\n");
	SQLFreeEnv (henv);
}

void check_return (RETCODE rc,HENV henv,HDBC hdbc,HSTMT hstmt )
{
	UCHAR state_str [SQL_MAX_MESSAGE_LENGTH];
	SDWORD native_error;
	UCHAR error_msg [SQL_MAX_MESSAGE_LENGTH];
	SWORD error_msg_avail = SQL_MAX_MESSAGE_LENGTH - 1;
	SWORD error_msg_len;
	RETCODE local_rc;
	
	if (rc != SQL_ERROR && rc != SQL_SUCCESS_WITH_INFO )
	{
	return;
	}
	
	local_rc = SQLError(henv,hdbc,hstmt,state_str,&native_error,error_msg,error_msg_avail,&error_msg_len);
	if (local_rc != SQL_SUCCESS && rc !=SQL_SUCCESS_WITH_INFO)
	{
		fprintf(stderr,"Uninterpretable error; exiting \n");
		exit (EXIT_FAILURE);
	}
	
	if (rc == SQL_SUCCESS_WITH_INFO)
	{
		fprintf(stderr,"Fatal Error %s: %s\n", state_str,error_msg);
		return;
	}
	
		fprintf(stderr,"Fatal Error %s: %s\n",state_str,error_msg); 
		exit (EXIT_FAILURE);
}

int sasql_query(char *id, char *ps)
{
	if(USEMSSQL==0)return TRUE;
	SQLCHAR sqlstr[128];
	SQLCHAR szPass[64]={0};
	SQLINTEGER cbPass;
	SQLCancel(hstmt);
	sprintf(sqlstr,"select PassWord from %s where Name='%s'",config.SQL_INFOTABLE,id);
	rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
	check_return (rc,henv,hdbc,hstmt);
	if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
	{
  	while( SQL_SUCCESS == SQLFetch(hstmt) ){
  		if(SQLGetData(hstmt, 1, SQL_C_CHAR, szPass, 50, &cbPass) == SQL_SUCCESS)
  			if(strcmp(szPass,ps)==0){
					return 1;
				}else{
					printf("账号%s密码错误!\n",id);
					return 2;
				}
		}
		printf("账号%s未注册!\n",id);
		return 3;
	}
	return 0;
}

BOOL sasql_register(char *id, char *ps)
{
	if(AUTOREG){
		SQLCHAR sqlstr[128];
		SQLCancel(hstmt);
		sprintf(sqlstr,"select * from %s where Name='%s'",config.SQL_INFOTABLE, id);
		rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
		check_return (rc,henv,hdbc,hstmt);
		if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
		{
			while( SQL_SUCCESS == SQLFetch(hstmt) )
				return FALSE;
			SQLCancel(hstmt);
			sprintf(sqlstr,"INSERT INTO %s (Name,PassWord,RegTime,Path) VALUES ('%s','%s',NOW(),'char/0x%x')",config.SQL_INFOTABLE, id, ps, getHash(id) & 0xff);
			rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
			check_return (rc,henv,hdbc,hstmt);
			if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO){
				printf("新账号%s注册成功!\n",id);
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL sasql_craete_lock( void )
{
	char sqlstr[128];
	SQLCancel(hstmt);
	sprintf(sqlstr,"create table %s (Name varchar(16),PRIMARY KEY (Name));",config.SQL_LOCK);
	rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
	check_return (rc,henv,hdbc,hstmt);
	if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
	{
		printf("锁定用户表建立成功!\n");
  	return TRUE;
	}
	printf("锁定用户表建立失败!\n");
	return FALSE;
}

BOOL sasql_craete_userinfo( void )
{
	char sqlstr[255];
	SQLCancel(hstmt);
	sprintf(sqlstr,"CREATE TABLE %s ("
									"Name varchar(16) NOT NULL,"
									"PassWord varchar(16) NOT NULL,"
									"IP varchar(16),"
									"RegTime datetime,"
									"LoginTime datetime,"
									"OnlineName varchar(32),"
									"Online int,"
									"Path varchar(10),"
									"PRIMARY KEY (Name));",config.SQL_INFOTABLE);
	rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
	check_return (rc,henv,hdbc,hstmt);
	if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
	{
		printf("用户信息表建立成功!\n");
  	return TRUE;
	}
	printf("用户信息表建立失败!\n");
	return FALSE;
}

BOOL sasql_chehk_lock( char *idip )
{
	char sqlstr[128];
	SQLCancel(hstmt);
	sprintf(sqlstr,"select * from %s where Name='%s'", config.SQL_LOCK, idip);
	rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
	check_return (rc,henv,hdbc,hstmt);
	if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
	{
		while( SQL_SUCCESS == SQLFetch(hstmt) )
			return TRUE;
	}
	return FALSE;
}

BOOL sasql_add_lock( char *idip )
{
	char sqlstr[128];
	SQLCancel(hstmt);
	
	sprintf(sqlstr,"INSERT INTO %s (Name) VALUES ('%s')", config.SQL_LOCK, idip);

	rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
	check_return (rc,henv,hdbc,hstmt);
	if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
	{
		return TRUE;
	}
	return FALSE;
}

BOOL sasql_del_lock( char *idip )
{
	char sqlstr[128];
	SQLCancel(hstmt);

	sprintf(sqlstr,"delete from config.SQL_LOCK where Name='%s'", config.SQL_LOCK, idip);

	rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
	check_return (rc,henv,hdbc,hstmt);
	if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
	{
		return TRUE;
	}
	return FALSE;
}


#ifdef _SQL_BACKGROUND
BOOL sasql_online( char *ID, char *NM, char *IP, int flag )
{
	char sqlstr[256];
	SQLCancel(hstmt);
	if(flag==0)
		sprintf(sqlstr,"update %s set Online=0 where Name='%s'", config.SQL_INFOTABLE, ID);
	else if(flag==1)
		sprintf(sqlstr,"update %s set LoginTime=NOW(), IP='%s', Online=1 where Name='%s'", config.SQL_INFOTABLE, IP, ID);
	else if(flag==2)
		sprintf(sqlstr,"update %s set LoginTime=NOW(), OnlineName='%s', Online=1 where Name='%s'", config.SQL_INFOTABLE, NM, ID);
	else if(flag==3)
		sprintf(sqlstr,"update %s set Online=0", config.SQL_INFOTABLE);
	rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
	check_return (rc,henv,hdbc,hstmt);
	if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
	{
  	if( SQL_SUCCESS == SQLFetch(hstmt) ){
			return TRUE;
		}
	}
	
	printf("\n更新数据库失败！\n");
	return FALSE;
}
#endif


int sasql_onlinenum( char *IP )
{
	char sqlstr[256];
	SQLCHAR szNum[64]={0};
	SQLINTEGER cbNum;
	SQLCancel(hstmt);
	
	sprintf(sqlstr,"select count(*) from %s where Online=1 and IP='%s'",config.SQL_INFOTABLE,IP);
	
	rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
	check_return (rc,henv,hdbc,hstmt);
	if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
	{
  	if( SQL_SUCCESS == SQLFetch(hstmt) ){
  		if(SQLGetData(hstmt, 1, SQL_C_CHAR, szNum, 50, &cbNum) == SQL_SUCCESS){
				return atoi(szNum);
			}
		}
	}
	return 0;
}


#ifdef _NEW_VIP_SHOP
int sasql_query_point( char *name )
{
	char sqlstr[256];
	SQLCHAR szPoint[64]={0};
	SQLINTEGER cbPoint;
	SQLCancel(hstmt);
	
	sprintf(sqlstr,"select VipPoint from `%s` where Name='%s'", config.SQL_INFOTABLE, name);
	
	rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
	check_return (rc,henv,hdbc,hstmt);
	if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
	{
  	if( SQL_SUCCESS == SQLFetch(hstmt) ){
  		if(SQLGetData(hstmt, 1, SQL_C_CHAR, szPoint, 50, &cbPoint) == SQL_SUCCESS){
  			return atoi(szPoint);
  		}
		}
	}
	return -1;
}


BOOL sasql_add_vippoint( char *ID, int point )
{
	char sqlstr[256];
	SQLCHAR szPoint[64]={0};
	SQLINTEGER cbPoint;
	SQLCancel(hstmt);
	
	sprintf(sqlstr,"select VipPoint from %s where Name='%s'",config.SQL_INFOTABLE, ID);
	
	rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
	check_return (rc,henv,hdbc,hstmt);
	if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
	{
		if( SQL_SUCCESS == SQLFetch(hstmt) ){
			if(SQLGetData(hstmt, 1, SQL_C_CHAR, szPoint, 50, &cbPoint) == SQL_SUCCESS){
				point+=atoi(szPoint);
				if(point < 0){
					printf("%s会员点数小于0\n",ID);
					return -1;
				}
				SQLCancel(hstmt);
				sprintf(sqlstr,"update %s set VipPoint=%d where NAME='%s'", config.SQL_INFOTABLE, point, ID);	
				rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
				check_return (rc,henv,hdbc,hstmt);
				if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO){
					printf("用户%s当前会员点%d！\n",ID,point);
					return point;
				}
			}
		}
	}
	printf("修改%s会员点数失败！\n",ID);
	return -1;
}
#endif

#ifdef _ITEM_PET_LOCKED
char *sasql_ItemPetLocked( char *id, char *safepasswd )
{
	char sqlstr[256];
	SQLCHAR szSafePasswd[32]={0};
	SQLINTEGER cbSafePasswd;
	SQLCancel(hstmt);
	
	sprintf(sqlstr,"select SafePasswd from `%s` where Name='%s'", config.SQL_INFOTABLE, id);
	
	rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
	check_return (rc,henv,hdbc,hstmt);
	if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
	{
		if( SQL_SUCCESS == SQLFetch(hstmt) ){
			if(SQLGetData(hstmt, 1, SQL_C_CHAR, szSafePasswd, 50, &cbSafePasswd) == SQL_SUCCESS){
		  	if( strlen(szSafePasswd)>0){
			  	if(strcmp(safepasswd, szSafePasswd)==0){
			  		return "安全锁已经成功解锁！";
			  	}else{
			  		return "密码错误，安全锁无法解开！";
			  	}
			  }else{
			  	return "您还未设置安全锁解锁密码，为了确保安全，请输入一次六位以上密码做为安全锁密码并劳劳记住！";
			  }
			}
		}
	}
	return "无法解锁，请与本服管理员联系！";
}

char *sasql_ItemPetLocked_Passwd( char *id, char *safepasswd )
{
	char sqlstr[256];
	SQLCancel(hstmt);
	sprintf(sqlstr,"update %s set SafePasswd='%s' where Name='%s'", config.SQL_INFOTABLE, safepasswd, id);	
	
	rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
	check_return (rc,henv,hdbc,hstmt);
	if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
	{
		return "安全密码修改成功，请妥善保管你的安全密码！";
	}
	return "安全密码修改失败，请与本服管理员联系！";
}
#endif

#ifdef _ONLINE_COST
char *sasql_OnlineCost( char *id, char *costpasswd )
{
	char sqlstr[256];
	SQLCHAR szCostVal[32]={0};
	SQLCHAR szTemp[32]={0};
	SQLINTEGER cbCostVal;
	SQLINTEGER cbTemp;
	SQLCancel(hstmt);
	
	sprintf(sqlstr,"select `CostVal`, `check` from `OnlineCost` where CostPasswd = '%s'", costpasswd);

	rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
	check_return (rc,henv,hdbc,hstmt);
	if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
	{
		if( SQL_SUCCESS == SQLFetch(hstmt) ){
			if(SQLGetData(hstmt, 1, SQL_C_CHAR, szCostVal, 50, &cbCostVal) == SQL_SUCCESS
						&& SQLGetData(hstmt, 2, SQL_C_CHAR, szTemp, 50, &cbTemp) == SQL_SUCCESS){
				if(atoi(szTemp) == 1){
				 	int costval = atoi(szCostVal);
				  int vippoint = sasql_add_vippoint(id, costval);
			  	char token[256];
			  	sprintf(sqlstr,"update OnlineCost set cdkey='%s', CostTime=NOW(), `check`=0  where CostPasswd='%s'", id, costpasswd);	
					
					rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
					check_return (rc,henv,hdbc,hstmt);
					if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
					{
						log("充值卡号%s已充值！\n",costpasswd);
					}
					sprintf(token, "充值已成功，充值卡面值为%d，您当前会员点数共%d", costval, vippoint);
			  	return token;
			  }else{
			  	return "该充值卡已使用过，请勿重复使用！";
			 }
			}else{
				return "充值失败，该充值卡密码不正确！";
			}
		}else{
			return "查找不到该卡号，请与本服管理员联系！";
		}
	}
	return "充值失败，请与本服管理员联系！";
}

void sasql_OnlineCost_add( int cost )
{
	char sqlstr[256];
	char CostPasswd[32];
	memset( CostPasswd, 0, sizeof( CostPasswd ) );
	int i, j;
	for(i=0;i<100;i++){
		for(j=0;j<16;j++){
			CostPasswd[j++]=(rand() % 26) + 65;
			CostPasswd[j]=(rand() % 10) + 48;
		}
		sprintf(sqlstr,"INSERT INTO OnlineCost (CostPasswd, CostVal) VALUES ('%s', %d)", CostPasswd, cost);
		
		rc = SQLExecDirect( hstmt, sqlstr, SQL_NTS);
		check_return (rc,henv,hdbc,hstmt);
		if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
		{
			SQLCancel(hstmt);
		}
	}
	printf("成功添加%d条面值%d的充值卡\n", i, cost);
	return ;
}
#endif

#endif

