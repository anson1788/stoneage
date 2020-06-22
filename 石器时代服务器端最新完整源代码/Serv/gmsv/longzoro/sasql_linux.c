#include "longzoro/version.h"

#ifdef _SASQL//新添加

#include "longzoro/sasql.h"
#include "char_base.h"
#include "configfile.h"
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
MYSQL mysql;
MYSQL_RES *mysql_result;
MYSQL_ROW mysql_row;
MYSQL_FIELD *fields;

typedef struct tagSQLConfig
{
    char    sql_IP[32]; 

		int     sql_Port; 
		
		char    sql_Port1[16];
		
    char    sql_ID[16];
    
    char    sql_PS[32];

    char    sql_DataBase[16];  

    char    sql_Table[16];    
    
    char    sql_LOCK[16];   
    
    char    sql_NAME[16];  
    
    char    sql_PASS[16];  
}SQLConfig;

SQLConfig sqlconfig;

int AutoReg;
int openbackground;

static int readSqlConfig( char *path )
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

        if( strcmp( command , "sql_IP" ) == 0 ){
            strcmp( sqlconfig.sql_IP , param ) ;
            snprintf( sqlconfig.sql_IP , sizeof( sqlconfig.sql_IP) , param );
            printf("数据库地址：  %s\n",sqlconfig.sql_IP);
        } else if( strcmp( command , "sql_Port" ) == 0 ){
        		sqlconfig.sql_Port = atoi( param );
            snprintf( sqlconfig.sql_Port1 , sizeof( sqlconfig.sql_Port1) , param );
				  	printf("数据库端口：  %d\n",sqlconfig.sql_Port);
        } else if( strcmp( command , "sql_ID" ) == 0 ){
        		strcmp( sqlconfig.sql_ID , param ) ;
            snprintf( sqlconfig.sql_ID , sizeof( sqlconfig.sql_ID) , param );
						printf("数据库用户：  %s\n",sqlconfig.sql_ID);
        } else if( strcmp( command , "sql_PS" ) == 0 ){
        		strcmp( sqlconfig.sql_PS , param ) ;
            snprintf( sqlconfig.sql_PS , sizeof( sqlconfig.sql_PS) , param );
						printf("数据库密码：  %s\n",sqlconfig.sql_PS);
        } else if( strcmp( command , "sql_DataBase" ) == 0 ){
        		strcmp( sqlconfig.sql_DataBase , param ) ;
            snprintf( sqlconfig.sql_DataBase , sizeof( sqlconfig.sql_DataBase) , param );
						printf("登陆数据库名：%s\n",sqlconfig.sql_DataBase);
        } else if( strcmp( command , "sql_Table" ) == 0 ){
        		strcmp( sqlconfig.sql_Table , param ) ;
            snprintf( sqlconfig.sql_Table , sizeof( sqlconfig.sql_Table) , param );
				  	printf("用户信息表名：  %s\n",sqlconfig.sql_Table);
				} else if( strcmp( command , "sql_NAME" ) == 0 ){
        		strcmp( sqlconfig.sql_NAME , param ) ;
            snprintf( sqlconfig.sql_NAME , sizeof( sqlconfig.sql_NAME) , param );
				  	printf("账号字段名称：  %s\n",sqlconfig.sql_NAME);
				} else if( strcmp( command , "sql_PASS" ) == 0 ){
        		strcmp( sqlconfig.sql_PASS , param ) ;
            snprintf( sqlconfig.sql_PASS , sizeof( sqlconfig.sql_PASS) , param );
				  	printf("密码字段名称：  %s\n",sqlconfig.sql_PASS);
				}
		}
    fclose(fp);
    return 0;
}

BOOL sasql_init( void )
{
		if((mysql_init(&mysql) == NULL) & readSqlConfig("sql.cf"))
		{
			  printf("\n数据库初始化失败！\n");
			  exit(1);
		    return FALSE;
		}
	
	  if( !mysql_real_connect( &mysql,
	          sqlconfig.sql_IP,
	          sqlconfig.sql_ID,//帐号
	          sqlconfig.sql_PS,//密码
	          sqlconfig.sql_DataBase,//选择的资料库
	          sqlconfig.sql_Port,
	          NULL,
	          0 ) )
		{
			printf("\n数据库连接失败！\n");
			return FALSE;
		}

		mysql_query(&mysql,"set names 'gbk'");
  	printf("\n数据库连接成功！\n");
  	return TRUE;
}

int sasql_mysql_query( char *sqlstr )
{
	mysql_query(&mysql,"set names 'gbk'");
  return mysql_query(&mysql,sqlstr);
}

void sasql_close( void )
{
  mysql_close( &mysql );
}

BOOL sasql_ckeckStrint( char *str )
{
  int i;
  for(i=0;i<strlen(str);i++){
  	if((str[i] < 'a' || str[i] > 'z') && (str[i] < 'A' || str[i] > 'Z') && (str[i] < '0' || str[i] > '9') && (str[i] != '.')){
  		return FALSE;
  	}
  }
  return TRUE;
}

int sasql_getVipPoint( int charaindex )
{
	char sqlstr[256];
	char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
	sprintf(sqlstr,"select VipPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);
	  		if (mysql_row!=NULL) {
	  			return atoi(mysql_row[0]);
	  		}else{
	  			return 0;
	  		}
		}
	}
	printf("\n数据库查找失败！\n");
	printf("重新连接数据库...");
	sasql_close();
	sasql_init();
	printf("完成\n");
	return -1;
}

int sasql_setVipPoint( int charaindex, int point )
{
	char sqlstr[256];
	char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
	sprintf(sqlstr,"update %s set VipPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);	
	
	if(!sasql_mysql_query(sqlstr)){
		//printf("用户%s当前重回币%d！\n",id,point);
		return point;
	}
	return -1;
}

int sasql_getVipPointForCdkey( char *id )
{
	if(sasql_ckeckStrint(id) == FALSE){
		printf("异常字符的用户名%s\n",id);
	  return -1;
	}
	
	char sqlstr[256];
	sprintf(sqlstr,"select VipPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);
	  	
  		if (mysql_row!=NULL) {
  			return atoi(mysql_row[0]);
  		}else{
  			return 0;
  			}
		}
	}
	printf("\n数据库查找失败！\n");
	printf("重新连接数据库...");
	sasql_close();
	sasql_init();
	printf("完成\n");
	return -1;
}

int sasql_setVipPointForCdkey( char *id, int point )
{
	if(sasql_ckeckStrint(id) == FALSE){
		printf("异常字符的用户名%s\n",id);
	  return -1;
	}
	
	char sqlstr[256];
	sprintf(sqlstr,"update %s set VipPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);	
	
	if(!sasql_mysql_query(sqlstr)){
		//printf("用户%s当前重回币%d！\n",id,point);
		return point;
	}
	return -1;
}


int sasql_getPayPoint( int charaindex )
{
	char sqlstr[256];
	char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
	sprintf(sqlstr,"select PayPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);
	  	
  		if (mysql_row!=NULL) {
  			return atoi(mysql_row[0]);
  		}else{
  			return 0;
  			}
		}
	}
	printf("\n数据库查找失败！\n");
	printf("重新连接数据库...");
	sasql_close();
	sasql_init();
	printf("完成\n");
	return -1;
}

int sasql_setPayPoint( int charaindex, int point )
{
	char sqlstr[256];
	char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
	sprintf(sqlstr,"update %s set PayPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);

	if(!sasql_mysql_query(sqlstr)){
		//printf("用户%s当前重回币%d！\n",id,point);
		return point;
	}
	return -1;
}

#ifdef _GAMBLE_POINT
int sasql_getGamblePoint( int charaindex )
{
	char sqlstr[256];
	char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
	sprintf(sqlstr,"select GamblePoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);

  		if (mysql_row!=NULL) {
  			return atoi(mysql_row[0]);
  		}else{
  			return 0;
  			}
		}
	}
	printf("\n数据库查找失败！\n");
	printf("重新连接数据库...");
	sasql_close();
	sasql_init();
	printf("完成\n");
	return -1;
}

int sasql_setGamblePoint( int charaindex, int point )
{
	char sqlstr[256];
	char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
	sprintf(sqlstr,"update %s set GamblePoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);
	
	if(!sasql_mysql_query(sqlstr)){
		return point;
	}
	return -1;
}

#endif

#ifdef _TRIAL_POINT
int sasql_getTrialPoint( int charaindex )
{
	char sqlstr[256];
	char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
	sprintf(sqlstr,"select TrialPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);

  		if (mysql_row!=NULL) {
  			return atoi(mysql_row[0]);
  		}else{
  			return 0;
  			}
		}
	}
	printf("\n数据库查找失败！\n");
	printf("重新连接数据库...");
	sasql_close();
	sasql_init();
	printf("完成\n");
	return -1;
}

int sasql_setTrialPoint( int charaindex, int point )
{
	char sqlstr[256];
	char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
	sprintf(sqlstr,"update %s set TrialPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);

	if(!sasql_mysql_query(sqlstr)){
		return point;
	}
	return -1;
}

#endif

#ifdef _GLORY_POINT
int sasql_getGloryPoint( int charaindex )
{
	char sqlstr[256];
	char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
	sprintf(sqlstr,"select GloryPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);

  		if (mysql_row!=NULL) {
  			return atoi(mysql_row[0]);
  		}else{
  			return 0;
  			}
		}
	}
	printf("\n数据库查找失败！\n");
	printf("重新连接数据库...");
	sasql_close();
	sasql_init();
	printf("完成\n");
	return -1;
}

int sasql_setGloryPoint( int charaindex, int point )
{
	char sqlstr[256];
	char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
	sprintf(sqlstr,"update %s set GloryPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);

	if(!sasql_mysql_query(sqlstr)){
		return point;
	}
	return -1;
}

#endif
int sasql_getPayPointForCdkey( char *id )
{
	if(sasql_ckeckStrint(id) == FALSE){
		printf("异常字符的用户名%s\n",id);
	  return -1;
	}
	
	char sqlstr[256];
	sprintf(sqlstr,"select PayPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);
	  	
  		if (mysql_row!=NULL) {
  			return atoi(mysql_row[0]);
  		}else{
  			return 0;
  			}
		}
	}
	printf("\n数据库查找失败！\n");
	printf("重新连接数据库...");
	sasql_close();
	sasql_init();
	printf("完成\n");
	return -1;
}

int sasql_setPayPointForCdkey( char *id, int point )
{
	if(sasql_ckeckStrint(id) == FALSE){
		printf("异常字符的用户名%s\n",id);
	  return -1;
	}
	
	char sqlstr[256];
	sprintf(sqlstr,"update %s set PayPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);	
	
	if(!sasql_mysql_query(sqlstr)){
		//printf("用户%s当前重回币%d！\n",id,point);
		return point;
	}
	return -1;
}

#ifdef _PAUCTION_MAN
BOOL sasql_addPauctionInfo( char *cdkey, char *name, char *effect, int cost, int type, char *info, char *string )
{
	char sqlstr[1024];
	sprintf(sqlstr,"INSERT INTO `PauctionInfo` ( `cdkey` , `name` , `effect` , `cost` , `type` , `info` , `string` , `day` ) "
																								"VALUES ( '%s', '%s', '%s', %d, %d, '%s', '%s', NOW())", 
																								cdkey, name, effect, cost, type, info, string );
	
	if(!sasql_mysql_query(sqlstr)){
		printf("成功添加一条拍卖信息！\n");
		return TRUE;
	}
	return FALSE;
}

int sasql_getPauctionList( char *List, int start, int type, char *cdkey )
{
	char sqlstr[1024];
	char tmp[256];
	char buf[256];
	int num_row=0;
	if(type == 0){
		if(strlen(cdkey)>0){
			sprintf(sqlstr, "select `id` , `cdkey` , `effect` from `PauctionInfo` where `cdkey` = BINARY'%s'", cdkey);
		}else{
			sprintf(sqlstr, "select `id` , `cdkey` , `effect` from `PauctionInfo` where TO_DAYS( NOW( ) ) - TO_DAYS( day ) < 30");
		}
	}else{
		sprintf(sqlstr, "select `id` , `cdkey` , `effect` from `PauctionInfo` where `type` = %d AND TO_DAYS( NOW( ) ) - TO_DAYS( day ) < 30", type);
	}
	if(!sasql_mysql_query(sqlstr)){
		int line = 0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);

		if(num_row > 0){
		  while((mysql_row = mysql_fetch_row(mysql_result))){
		  	if(line >=start){
					sprintf(tmp, "%s|%s|%s", mysql_row[0], mysql_row[1], mysql_row[2]);
					strcat(List, makeEscapeString(tmp, buf, sizeof( buf)));
					strcat(List, "|");
					
					if(line > start + 10)break;
		  	}
		  	line ++;
			}
		}

		return num_row;
	}
	
	return 0;
}

BOOL sasql_getPauctionSurvey( char *token, int id )
{
	char sqlstr[1024];
	char buf[1024];
	sprintf(sqlstr, "select `id` , `name` , `effect`, `cost`, `type`, `info` from `PauctionInfo` where `id` = %d AND TO_DAYS( NOW( ) ) - TO_DAYS( day ) < 30", id);

	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
		if(num_row > 0){
			mysql_row = mysql_fetch_row(mysql_result);
			sprintf(token, "%s|%s|%s|%s|%s|%s", mysql_row[0], mysql_row[1], mysql_row[2],mysql_row[3], mysql_row[4], makeEscapeString(mysql_row[5], buf, sizeof( buf) ));
		}
		
		return TRUE;
	}
	
	return FALSE;
}

int sasql_getMyPauction( char *token, int id, char *cdkey )
{
	char sqlstr[1024];

	if(strlen(cdkey)>0){
		sprintf(sqlstr, "select `string` ,`type` from `PauctionInfo` where `id` = %d AND `cdkey` = BINARY'%s'", id, cdkey);
	}else{
		sprintf(sqlstr, "select `string` ,`type` from `PauctionInfo` where `id` = %d ", id);
	}
	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);

		if(num_row > 0){
			mysql_row = mysql_fetch_row(mysql_result);
			sprintf(token, "%s", mysql_row[0]);
		}
		
		return atoi(mysql_row[1]);
	}
	
	return -1;
}

int sasql_PauctionOK( int id, char *cdkey )
{
	char sqlstr[1024];
	sprintf(sqlstr, "select `cdkey`, `cost` from `PauctionInfo` where `id` = %d", id);

	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
		if(num_row > 0){
			mysql_row = mysql_fetch_row(mysql_result);
			char tocdkey[64];
			sprintf(tocdkey, "%s", mysql_row[0]);
			int cost = atoi(mysql_row[1]);
			sprintf(sqlstr, "select * from `%s` where %s = BINARY'%s' AND VipPoint>%d", sqlconfig.sql_Table, sqlconfig.sql_NAME, cdkey, cost);
			
			if(!sasql_mysql_query(sqlstr)){
				int num_row=0;
				mysql_free_result(mysql_result);
			  mysql_result=mysql_store_result(&mysql);
			  num_row=mysql_num_rows(mysql_result);

				if(num_row > 0){
					sprintf(sqlstr,"update %s set VipPoint=VipPoint + %d where %s=BINARY'%s' "
													, sqlconfig.sql_Table, cost, sqlconfig.sql_NAME, tocdkey);	
					if(!sasql_mysql_query(sqlstr)){
						sprintf(sqlstr,"update %s set VipPoint=VipPoint - %d where %s=BINARY'%s' "
													, sqlconfig.sql_Table, cost, sqlconfig.sql_NAME, cdkey);
						if(!sasql_mysql_query(sqlstr)){
							return 1;
						}
					}
				}else{
					return -2;
				}
			}
		}else{
			return -1;
		}
	}
	return 0;
}

BOOL sasql_delPauctionBuy( int id, char *cdkey )
{
	char sqlstr[1024];

	if(strlen(cdkey)>0){
		sprintf(sqlstr, "delete from `PauctionInfo` where `id` = %d AND `cdkey` = BINARY'%s'", id, cdkey);
	}else{
		sprintf(sqlstr, "delete from `PauctionInfo` where `id` = %d", id);
	}

	if(!sasql_mysql_query(sqlstr)){
		return TRUE;
	}
	
	return FALSE;
}
#endif
#ifdef _JZ_NEWSCRIPT_LUA
int sasql_vippoint( char *ID, int point, int flag )
{
	char sqlstr[256];
//	print("\NVIPpoint1=%d\n",point);
	if(strstr(ID,"'")!=NULL) return -1;
	sprintf(sqlstr,"select `VipPoint` from `%s` where `%s`=BINARY'%s'",sqlconfig.sql_Table,sqlconfig.sql_NAME,ID);
//	printf("22222222:%s\n",sqlstr);
	if(!mysql_query(&mysql,sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
		mysql_result=mysql_store_result(&mysql);
		num_row=mysql_num_rows(mysql_result);
		if(num_row>0){
			mysql_row=mysql_fetch_row(mysql_result);
			if(flag==0){
				return atoi(mysql_row[0]);
			}
			if(flag==520){
				sprintf(sqlstr,"update `%s` set `VipPoint`=%d where `%s`=BINARY'%s'", sqlconfig.sql_Table, point,sqlconfig.sql_NAME, ID);
			}else{
				point+=atoi(mysql_row[0]);
				sprintf(sqlstr,"update `%s` set `VipPoint`=%d where `%s`=BINARY'%s'", sqlconfig.sql_Table, point,sqlconfig.sql_NAME, ID);
			}
			if(!mysql_query(&mysql,sqlstr)){
				return point;
			}else{
				return atoi(mysql_row[0]);
			}
		}else{
			//mysql_free_result(mysql_result);
		}
	}
	printf("更新金币数失败！-- %s\n",sqlstr);
	return -1;
}

int sasql_ampoint( char *ID, int point, int flag )
{
	char sqlstr[256];
	mj_sprintf(sqlstr,"select `AmPoint` from `%s` where `%s`=BINARY'%s'",sqlconfig.sql_Table,sqlconfig.sql_NAME,ID);
	if(!mysql_query(&mysql,sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
		mysql_result=mysql_store_result(&mysql);
		num_row=mysql_num_rows(mysql_result);
		if(num_row>0){
			mysql_row=mysql_fetch_row(mysql_result);
			if(flag==0){
				return atoi(mysql_row[0]);
			}
			point+=atoi(mysql_row[0]);
			mj_sprintf(sqlstr,"update `%s` set `AmPoint`=%d where `%s`=BINARY'%s'", sqlconfig.sql_Table, point,sqlconfig.sql_NAME, ID);
			if(!mysql_query(&mysql,sqlstr)){
				return point;
			}else{
				return atoi(mysql_row[0]);
			}
		}else{
//		mysql_free_result(mysql_result);
		}
	}
	print("更新%s积分失败！\n",ID);
	return -1;
}

int sasql_rmbpoint( char *ID, int point, int flag )
{
	char sqlstr[256];
	mj_sprintf(sqlstr,"select `RMBPOINT` from `%s` where `%s`=BINARY'%s'",sqlconfig.sql_Table,sqlconfig.sql_NAME,ID);
	if(!mysql_query(&mysql,sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
		mysql_result=mysql_store_result(&mysql);
		num_row=mysql_num_rows(mysql_result);
		if(num_row>0){
			mysql_row=mysql_fetch_row(mysql_result);
			if(flag==0){
				return atoi(mysql_row[0]);
			}
			point+=atoi(mysql_row[0]);
			mj_sprintf(sqlstr,"update `%s` set `RMBPOINT`=%d where `%s`=BINARY'%s'", sqlconfig.sql_Table, point,sqlconfig.sql_NAME, ID);
			if(!mysql_query(&mysql,sqlstr)){
				return point;
			}else{
				return atoi(mysql_row[0]);
			}
		}else{
	//	mysql_free_result(mysql_result);
		}
	}
	print("更新%sRMB失败！\n",ID);
	return -1;
}

#endif

#ifdef _MO_LNS_MYSQLSUOXU

struct youx{
	int result;
	int flg;
	int errnono;
	char* errstr;
	int fieldCount;
	int rowCount;
	int rowAt;
}retlua;

void sasql_LUASQLAdv(char* sqlstr,char* filepath,char* function,int npcindex ,int charaindex,char* msg,int flg)
{
	char* data = sasql_LuaAdv_QueryR(sqlstr,flg);
	NPC_Lua_SQLPushAdvCallBack(retlua.result,retlua.flg,retlua.errnono,retlua.errstr,retlua.fieldCount ,retlua.rowCount ,retlua.rowAt, data,filepath,function,npcindex,charaindex,msg );
}
char* sasql_LuaAdv_QueryR(char *sqlstr,int flg)
{
	int rowAtt = 0;
	static char buff[1024] = "";
	snprintf(buff,1024, "");
	buff[1023]='\0';
	/*初始化信息表*/
	retlua.result = 0;//结果集ERROR
	retlua.flg = flg;//分类
	retlua.errnono = 0;//MYSQL错误编号
	retlua.errstr = "";//MYSQL错误信息
	retlua.fieldCount = 0;//查询的项目数
	retlua.rowCount = 0;//查询结果集行数
	retlua.rowAt = 0;//当前记录行数


	if (flg == 1 || flg == 2){

		if(!mysql_query(&mysql,sqlstr)){
			int num_row=0;
			mysql_result=mysql_store_result(&mysql);
			num_row=mysql_num_rows(mysql_result);//结果集行数
			if (mysql_result  == NULL)
			{
				retlua.result = -2;
			}else if (mysql_eof(mysql_result))
			{
				retlua.result = -3;
			}

			if(num_row>0){

				mysql_row = mysql_fetch_row(mysql_result);//从查询结果中取一条

				retlua.errnono = mysql_errno(&mysql);//错误编号
				retlua.errstr = mysql_error(&mysql);//错误信息
				retlua.fieldCount = mysql_field_count(&mysql);//列数

				retlua.rowCount = num_row;//结果行数量
				rowAtt++;
				retlua.rowAt = rowAtt;//当前行数
				if (flg == 1){
					mysql_free_result(mysql_result);//释放结果集
					mysql_result = NULL;
					rowAtt = 0;
				}

				int i;
				for (i=0;i<retlua.fieldCount;i++)
				{
					snprintf(buff,sizeof(buff), "%s%s;",buff,(char*)mysql_row[i]);
				}
				if (num_row > 15) retlua.result = -1;
				return buff;
			}else {
				retlua.errnono = mysql_errno(&mysql);
				retlua.errstr = mysql_error(&mysql);
			}
		}
	}else if (flg == 3){


		int num_row=0;
		if (mysql_result == NULL) {
			retlua.result = -2;
			return "结果集已经释放。";
		}
		num_row=mysql_num_rows(mysql_result);//结果集行数
		if (mysql_eof(mysql_result))
		{
			retlua.result = -3;//已达到尾端，先给你提个醒，如果还继续查询，下面将会自动释放。
		}
		if (num_row == rowAtt) {
			mysql_free_result(mysql_result);//释放结果集
			mysql_result = NULL;
			rowAtt = 0;
			retlua.result = -3;
			return "已到达尾行，自动释放结果集。";
		}
		if(num_row>0){
			mysql_row = mysql_fetch_row(mysql_result);//从查询结果中取一条
			retlua.errnono = mysql_errno(&mysql);//错误编号
			retlua.errstr = mysql_error(&mysql);//错误信息
			rowAtt++;
			retlua.rowAt = rowAtt;//当前行数
			retlua.fieldCount = mysql_field_count(&mysql);//列数
			retlua.rowCount = num_row;//结果行数量
			if (num_row > 15) retlua.result = -1;
			int i;
			for (i=0;i<retlua.fieldCount;i++)
			{
				snprintf(buff,sizeof(buff), "%s%s;",buff,(char*)mysql_row[i]);
			}
			return buff;
		}else {
			retlua.errnono = mysql_errno(&mysql);
			retlua.errstr = mysql_error(&mysql);
		}

	}else if (flg == 4){
		if (mysql_result == NULL) {
			retlua.result = -2;
			return "结果集已经释放。";
		}
		mysql_free_result(mysql_result);//释放结果集
		mysql_result = NULL;
		rowAtt = 0;
		retlua.rowAt = rowAtt;//当前行数
		retlua.errnono = mysql_errno(&mysql);
		retlua.errstr = mysql_error(&mysql);
		return "";
	}

	retlua.errnono = mysql_errno(&mysql);
	retlua.errstr = mysql_error(&mysql);
	return "";
}
void sasql_LUASQL(char* sqlstr,int row,char* filepath,char* function,int npcindex ,int charaindex,char* msg)
{
	if(row == -1 )
	{
		mysql_query(&mysql,sqlstr);
	}else{
		NPC_Lua_SQLPushCallBack(sasql_Lua_QueryR(row,sqlstr),filepath,function,npcindex, charaindex,msg);
	}
}
char* sasql_Lua_QueryR(int row,char *sqlstr)
{
	if(!mysql_query(&mysql,sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
		mysql_result=mysql_store_result(&mysql);
		num_row=mysql_num_rows(mysql_result);

		if(num_row>0){
			mysql_row = mysql_fetch_row(mysql_result);
			return (char*)mysql_row[row];
		}
	}else
		return "SQL Query ERROR";
}
#endif

#endif

int sasql_check_iponline( char *IP )
{
	char sqlstr[256];
	sprintf(sqlstr,"select * from `%s` where `IP`='%s'",sqlconfig.sql_Table,IP);
	if(!mysql_query(&mysql,sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
		mysql_result=mysql_store_result(&mysql);
		num_row=mysql_num_rows(mysql_result);

		if(num_row>0){
			return 1;
		}else{
			return 0;
		}
	}
	return -1;
}

int sasql_query_online_ip(char *IP){
	char sqlstr[256];
	sprintf(sqlstr,"select * from `OnlineIP` where `IP`='%s'",IP);
	if(!mysql_query(&mysql,sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
		mysql_result=mysql_store_result(&mysql);
		num_row=mysql_num_rows(mysql_result);

		if(num_row>0){
			return 1;
		}else{
			return 0;
		}
	}
	return -1;
}

int sasql_query(char *nm, char *pas){
	char sqlstr[256];
	sprintf(sqlstr,"select `%s`,`Password`,`SafePasswd`,`Lock` from `%s` where `%s`=BINARY'%s'",sqlconfig.sql_NAME,sqlconfig.sql_Table,sqlconfig.sql_NAME,nm);
	//print(sqlstr);
	if(!mysql_query(&mysql,sqlstr)){
	  int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);

	  	if(mysql_row[2]!=NULL){
	  		if((strcmp(pas,mysql_row[1]) == 0) && (atoi(mysql_row[3])==0 || atoi(mysql_row[3])==getLockType())){
	  		return 1;
		  	}else{
		  		//printf("用户%s密码错误！\n",nm);
		  		return 2;
				}
		  }else
		  	{
		  		if(strcmp(pas,mysql_row[1]) == 0 && atoi(mysql_row[3])==0){
	  			return 1;
		  		}else{
		  			//printf("用户%s密码错误！\n",nm);
		  			return 2;
					}
		  	}
	  }else{
	  	//printf("用户%s未注册！\n",nm);
	  	return 3;
	  }
	}else{
		//printf("\n数据库查找失败！\n");
		return 0;
	}
}
#ifdef _MO_LOGIN_NO_KICK
BOOL sasql_CheckPasswd( char *id, char *passwd )
{
	if(sasql_ckeckStrint(id) == FALSE){
		printf("异常字符%s\n",id);
		return FALSE;	}
	if(sasql_ckeckStrint(passwd) == FALSE){
		printf("异常字符%s\n",passwd);
		return FALSE;
	}
	char sqlstr[256];
	sprintf(sqlstr,"select `PassWord` from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);

	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);
	  	if( mysql_row[0] != NULL && strlen(mysql_row[0])>0){
		  	if(strcmp(passwd, mysql_row[0])==0){
		  		return TRUE;
		  	}else{
		  		return FALSE;
		  	}
		  }else{
			  return FALSE;
		  }
		}
	}
	return FALSE;
}
#endif
#ifdef _NEW_SQL_MONEY
int sasql_setSQLPoint( int charaindex, int point )
{
	char sqlstr[256];
	char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
	sprintf(sqlstr,"update %s set SQLPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);

	if(!mysql_query(&mysql,sqlstr)){
		return point;
	}
	return -1;
}

int sasql_getSQLPoint( int charaindex )
{
	char sqlstr[256];
	char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
	sprintf(sqlstr,"select SQLPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

	if(!mysql_query(&mysql,sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);
	//  	mysql_free_result(mysql_result);

	  	return atoi(mysql_row[0]);
		}
	}
	printf("\n数据库查找失败[20]！\n");
	printf("重新连接数据库[3]...");
//	sasql_close();
//	sasql_init();
	printf("完成\n");
	return -1;
}

int sasql_setGoldPoint( int charaindex, int point )
{
	char sqlstr[256];
	char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
	sprintf(sqlstr,"update %s set GoldPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);

	if(!mysql_query(&mysql,sqlstr)){
		return point;
	}
	return -1;
}

int sasql_getGoldPoint( int charaindex )
{
	char sqlstr[256];
	char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
	sprintf(sqlstr,"select GoldPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

	if(!mysql_query(&mysql,sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);
	//  	mysql_free_result(mysql_result);

	  	return atoi(mysql_row[0]);
		}
	}
	printf("\n数据库查找失败[21]！\n");
	printf("重新连接数据库[3]...");
//	sasql_close();
//	sasql_init();
	printf("完成\n");
	return -1;
}
int sasql_setSQLPointByCdkey( char* cdkey, int point )
{
	char sqlstr[256];
	sprintf(sqlstr,"update %s set SQLPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, cdkey);

	if(!mysql_query(&mysql,sqlstr)){
		return point;
	}
	return -1;
}

int sasql_getSQLPointByCdkey( char* cdkey )
{
	char sqlstr[256];
	sprintf(sqlstr,"select SQLPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, cdkey);

	if(!mysql_query(&mysql,sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);
	//  	mysql_free_result(mysql_result);

	  	return atoi(mysql_row[0]);
		}
	}
	printf("\n数据库查找失败[22]！\n");
	printf("重新连接数据库[3]...");
//	sasql_close();
//	sasql_init();
	printf("完成\n");
	return -1;
}

int sasql_setGoldPointByCdkey( char* cdkey, int point )
{
	char sqlstr[256];
	sprintf(sqlstr,"update %s set GoldPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, cdkey);

	if(!mysql_query(&mysql,sqlstr)){
		return point;
	}
	return -1;
}

int sasql_getGoldPointByCdkey( char* cdkey )
{
	char sqlstr[256];
	sprintf(sqlstr,"select GoldPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, cdkey);

	if(!mysql_query(&mysql,sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);
	//  	mysql_free_result(mysql_result);

	  	return atoi(mysql_row[0]);
		}
	}
	printf("\n数据库查找失败[23]！\n");
	printf("重新连接数据库[3]...");
//	sasql_close();
//	sasql_init();
	printf("完成\n");
	return -1;
}

#endif
