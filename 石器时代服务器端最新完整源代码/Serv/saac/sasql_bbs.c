#define _SASQL_C_

#include "version.h"

#ifdef _SASQL//新添加

#include "main.h"
#include "sasql.h"
#include "util.h"
#include "md5.h"
#include <mysql/mysql.h>

MYSQL mysql;
MYSQL_RES *mysql_result;
MYSQL_ROW mysql_row;
MYSQL_FIELD *fields;

typedef struct tagConfig
{
    char    sql_IP[16]; 

		int     sql_Port; 
		
		char    sql_Port1[16];
		
    char    sql_ID[16];
    
    char    sql_PS[16];

    char    sql_DataBase[16];  

    char    sql_Table[32];    
    
    char    sql_LOCK[16];   
    
    char    sql_NAME[16];  
    
    char    sql_PASS[16];  
    
    int 		AutoReg;
    
		int 		openbackground;
}Config;

Config config;

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
            strcmp( config.sql_IP , param ) ;
            snprintf( config.sql_IP , sizeof( config.sql_IP) , param );
            printf("\n数据库地址：  %s",config.sql_IP);
        } else if( strcmp( command , "sql_Port" ) == 0 ){
        		config.sql_Port = atoi( param );
            snprintf( config.sql_Port1 , sizeof( config.sql_Port1) , param );
				  	printf("\n数据库端口：  %d",config.sql_Port);
        } else if( strcmp( command , "sql_ID" ) == 0 ){
        		strcmp( config.sql_ID , param ) ;
            snprintf( config.sql_ID , sizeof( config.sql_ID) , param );
						printf("\n数据库用户：  %s",config.sql_ID);
        } else if( strcmp( command , "sql_PS" ) == 0 ){
        		strcmp( config.sql_PS , param ) ;
            snprintf( config.sql_PS , sizeof( config.sql_PS) , param );
						printf("\n数据库密码：  %s",config.sql_PS);
        } else if( strcmp( command , "sql_DataBase" ) == 0 ){
        		strcmp( config.sql_DataBase , param ) ;
            snprintf( config.sql_DataBase , sizeof( config.sql_DataBase) , param );
						printf("\n登陆数据库名：%s",config.sql_DataBase);
        } else if( strcmp( command , "sql_Table" ) == 0 ){
        		strcmp( config.sql_Table , param ) ;
            snprintf( config.sql_Table , sizeof( config.sql_Table) , param );
				  	printf("\n用户信息表名：  %s",config.sql_Table);
				} else if( strcmp( command , "sql_LOCK" ) == 0 ){
        		strcmp( config.sql_LOCK , param ) ;
            snprintf( config.sql_LOCK , sizeof( config.sql_LOCK) , param );
				  	printf("\n用户锁定表名：  %s",config.sql_LOCK);
				} else if( strcmp( command , "sql_NAME" ) == 0 ){
        		strcmp( config.sql_NAME , param ) ;
            snprintf( config.sql_NAME , sizeof( config.sql_NAME) , param );
				  	printf("\n账号字段名称：  %s",config.sql_NAME);
				} else if( strcmp( command , "sql_PASS" ) == 0 ){
        		strcmp( config.sql_PASS , param ) ;
            snprintf( config.sql_PASS , sizeof( config.sql_PASS) , param );
				  	printf("\n密码字段名称：  %s",config.sql_PASS);
				} else if( strcmp( command , "openbackground" ) == 0 ){
        		config.openbackground = atoi( param );
        		if(config.openbackground == 1){
        			printf("\n后台功能：    YES");
        		}else{
        			printf("\n后台功能：    NO");
        		}
				} else if( strcmp( command , "AutoReg" ) == 0 ){
        		config.AutoReg = atoi( param );
        		if(config.AutoReg){
        			printf("\n开放自动注册：YES");
        		}else{
        			printf("\n开放自动注册：NO");
        		}
    		}
    }
    fclose(fp);
    return 0;
}

BOOL sasql_init( void )
{
		if( (mysql_init(&mysql) == NULL) & readSqlConfig("acserv.cf"))
		{
			  printf("\n数据库初始化失败！");
			  exit(1);
		    return FALSE;
		}
	
	  if( !mysql_real_connect( &mysql,
	          config.sql_IP,
	          config.sql_ID,//帐号
	          config.sql_PS,//密码
	          config.sql_DataBase,//选择的资料库
	          config.sql_Port,
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


int sasql_query(char *nm, char *pas){
	char sqlstr[256];
	sprintf(sqlstr,"select `%s`, `salt` from %s where %s=BINARY'%s'",config.sql_PASS, config.sql_Table,config.sql_NAME,nm);
	
	if(!sasql_mysql_query(sqlstr)){
	  int num_row=0;
	  mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
		
	  if(num_row>0){
	  	char token[64];
	  	mysql_row=mysql_fetch_row(mysql_result);
	  	strcpy(token, mysql_row[0]);
#ifdef _OLDPS_TO_MD5PS
			if(strlen(mysql_row[0])>16){
				char md5pas[256];
				char salt[256];
				char md5passalt[256];
				strcpy(salt, mysql_row[1]);
				strcpy(md5pas, MD5String(pas));
				sprintf(md5passalt, "%s%s", md5pas, salt);
				if(strcmp(MD5String(md5passalt),token) == 0){
	  			return 1;
			  }else{
			  	printf("用户%s密码错误！\n",nm);
			  	return 2;
			  }
			}else{
				if(strcmp(pas,token) == 0){
	  			return 1;
			  }else{
			  	printf("用户%s密码错误！\n",nm);
			  	return 2;
			  }
			}
#else
	  	if(strcmp(pas,token) == 0){
	  		return 1;
		  }else{
		  	printf("用户%s密码错误！\n",nm);
		  	return 2;
		  }
#endif
	  }else{
	  	printf("用户%s未注册！\n",nm);
	  	return 3;
	  }
	}else{
		printf("\n数据库查找失败！\n");
		printf("重新连接数据库...");
		sasql_close();
		sasql_init();
		printf("完成\n");
		return 0;
	}
}


#ifdef _SQL_BACKGROUND
BOOL sasql_online( char *ID, char *NM, char *IP, int flag )
{
	if(config.openbackground == 1){
		char sqlstr[256];
		if(flag==0)
			sprintf(sqlstr,"update %s set Online=0 where %s=BINARY'%s'", config.sql_Table, config.sql_NAME, ID);
		else if(flag==1)
			sprintf(sqlstr,"update %s set LoginTime=NOW(), MAC='%s', IP='%s', Online=1 where %s=BINARY'%s'", config.sql_Table, MAC, IP, config.sql_NAME, ID);
		else if(flag==2)
			sprintf(sqlstr,"update %s set LoginTime=NOW(), OnlineName='%s', Online=1, Path='char/0x%x'where %s=BINARY'%s'", config.sql_Table, NM, getHash(ID) & 0xff, config.sql_NAME, ID);
		else if(flag==3)
			sprintf(sqlstr,"update %s set Online=0", config.sql_Table);
		if(!sasql_mysql_query(sqlstr)){
			return TRUE;
		}
		printf("\n更新数据库失败%s\n", sqlstr);
	}
	return TRUE;
}
#endif

#ifdef _SQL_REGISTER
BOOL sasql_register(char *id, char *ps)
{
	char sqlstr[256];
	if(config.AutoReg!=1)return FALSE;
#ifdef _OLDPS_TO_MD5PS
	sprintf(sqlstr,"INSERT INTO %s (%s,%s,RegTime,Path) VALUES (BINARY'%s','%s',NOW(),'char/0x%x')",config.sql_Table, config.sql_NAME, config.sql_PASS, id, MD5String(ps), getHash(id) & 0xff);
#else
	sprintf(sqlstr,"INSERT INTO %s (%s,%s,RegTime,Path) VALUES (BINARY'%s','%s',NOW(),'char/0x%x')",config.sql_Table, config.sql_NAME, config.sql_PASS, id, ps, getHash(id) & 0xff);
#endif

	if(!sasql_mysql_query(sqlstr)){
		printf("\n新用户注册成功！\n");
		return TRUE;
	}
	printf("\n新用户注册失败！\n");
	return FALSE;
}
#endif


BOOL sasql_chehk_lock( char *idip )
{
	char sqlstr[256];
	sprintf(sqlstr,"select * from `%s` where Name='%s'", config.sql_LOCK, idip);
	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
	
	  if(num_row>0){
			return TRUE;
		}
	}
	return FALSE;
}

BOOL sasql_add_lock( char *idip )
{
	char sqlstr[256];
	sprintf(sqlstr,"INSERT INTO `%s` (Name) VALUES (BINARY'%s')", config.sql_LOCK, idip);
	if(!sasql_mysql_query(sqlstr)){
			printf("\n添加锁定%s成功！\n",idip);
			return TRUE;
	}
	return FALSE;
}

BOOL sasql_del_lock( char *idip )
{
	char sqlstr[256];
	sprintf(sqlstr,"delete from `%s` where Name=BINARY'%s'", config.sql_LOCK, idip);
	if(!sasql_mysql_query(sqlstr)){
		printf("\n解除锁定%s成功！\n",idip);
		return TRUE;
	}
	return FALSE;
}

#ifdef _NEW_VIP_SHOP
int sasql_query_point( char *name )
{
	char sqlstr[256];
	sprintf(sqlstr,"select VipPoint from `%s` where %s=BINARY'%s'", config.sql_Table, config.sql_NAME, name);
	
	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
		
	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);
	  	return atoi(mysql_row[0]);
		}
	}
	return -1;
}


BOOL sasql_add_vippoint( char *ID, int point )
{
	char sqlstr[256];
	sprintf(sqlstr,"select VipPoint from %s where %s=BINARY'%s'",config.sql_Table,config.sql_NAME,ID);
	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
		mysql_result=mysql_store_result(&mysql);
		num_row=mysql_num_rows(mysql_result);

		if(num_row>0){
			mysql_row=mysql_fetch_row(mysql_result);
			point+=atoi(mysql_row[0]);
			if(point < 0){
				printf("%s会员点数小于0\n",ID);
				return -1;
			}
			sprintf(sqlstr,"update %s set VipPoint=%d where %s=BINARY'%s'", config.sql_Table, point,config.sql_NAME, ID);	
			if(!sasql_mysql_query(sqlstr)){
				printf("用户%s当前会员点%d！\n",ID,point);
				return point;
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
	sprintf(sqlstr,"select SafePasswd from `%s` where %s=BINARY'%s'", config.sql_Table, config.sql_NAME, id);
	
	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);

	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);
	  	if( mysql_row[0] != NULL && strlen(mysql_row[0])>0){
		  	if(strcmp(safepasswd, mysql_row[0])==0){
		  		return "安全锁已经成功解锁！";
		  	}else{
		  		return "密码错误，安全锁无法解开！";
		  	}
		  }else{
		  	return "您还未设置安全锁解锁密码，为了确保安全，请输入一次六位以上密码做为安全锁密码并劳劳记住！";
		  }
		}
	}
	return "无法解锁，请与本服管理员联系！";
}

char *sasql_ItemPetLocked_Passwd( char *id, char *safepasswd )
{
	char sqlstr[256];
	sprintf(sqlstr,"update %s set SafePasswd=BINARY'%s' where %s=BINARY'%s'", config.sql_Table, safepasswd, config.sql_NAME, id);	
	
	if(!sasql_mysql_query(sqlstr)){
		return "安全密码修改成功，请妥善保管你的安全密码！";
	}
	return "安全密码修改失败，请与本服管理员联系！";
}

BOOL sasql_ItemPetLocked_Char( char *id, char *safepasswd )
{
	char sqlstr[256];
	sprintf(sqlstr,"select safepasswd from %s where %s=BINARY'%s'", config.sql_Table,config.sql_NAME, id);
	
	if(!sasql_mysql_query(sqlstr)){
	  int num_row=0;
	  mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);
	  	if( mysql_row[0] != NULL && strlen(mysql_row[0])>0){
		  	if(strcmp(safepasswd,mysql_row[0]) == 0){
		  		return 1;
			  }else{
			  	printf("用户%s安全密码错误！\n",id);
			  	return -1;
			  }
			}else{
		  	return 0;
		  }
	  }else{
	  	printf("用户%s未注册！\n",id);
	  	return -1;
	  }
	}else{
		printf("\n数据库查找失败！\n");
		printf("重新连接数据库...");
		sasql_close();
		sasql_init();
		printf("完成\n");
		return -1;
	}
}
#endif

#ifdef _ONLINE_COST
char *sasql_OnlineCost( char *id, char *costpasswd )
{
	char sqlstr[256];
	static char token[256]="";
	sprintf(sqlstr,"select `CostVal`, `check` from `OnlineCost` where CostPasswd = BINARY'%s'", costpasswd);
	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);
	  	if(atoi(mysql_row[1]) == 1){
			 	int costval = atoi(mysql_row[0]);
			  int vippoint = sasql_add_vippoint(id, costval);
		  	sprintf(sqlstr,"update OnlineCost set cdkey=BINARY'%s', CostTime=NOW(), `check`=0  where CostPasswd=BINARY'%s'", id, costpasswd);	
				if(!sasql_mysql_query(sqlstr)){
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
	}
	return "充值失败，请与本服管理员联系！";
}

void sasql_OnlineCost_add( int cost, int num)
{
	char sqlstr[256];
	char CostPasswd[32];
	memset( CostPasswd, 0, sizeof( CostPasswd ) );
	int i, j, k=0;
	for(i=0;i<num;i++){
		for(j=0;j<16;j++){
			if(rand() % 2==0){
				CostPasswd[j]=(rand() % 26) + 65;
			}else{
				CostPasswd[j]=(rand() % 10) + 48;
			}
		}
		sprintf(sqlstr,"INSERT INTO OnlineCost (CostPasswd, CostVal) VALUES (BINARY'%s', %d)", CostPasswd, cost);
		if(!sasql_mysql_query(sqlstr)){
			k++;
		}else{
			continue;
		}
	}
	printf("成功添加%d条面值%d的充值卡\n", k, cost);
	return ;
}

#endif


#ifdef _SQL_BUY_FUNC
char *sasql_OnlineBuy( char *id, char *costpasswd )
{
	char sqlstr[256];
	static char token[256]="";
	sprintf(sqlstr,"select `CostStr`, `check` from `OnlineBuy` where CostPasswd = BINARY'%s'", costpasswd);
	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
	  mysql_result=mysql_store_result(&mysql);
	  num_row=mysql_num_rows(mysql_result);
		
	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);
	  	if(atoi(mysql_row[1]) == 1){
			 	char *coststr = mysql_row[0];
		  	sprintf(sqlstr,"update OnlineBuy set cdkey=BINARY'%s', CostTime=NOW(), `check`=0  where CostPasswd=BINARY'%s'", id, costpasswd);	
				if(!sasql_mysql_query(sqlstr)){
					log("提货卡号%s已提取！\n",costpasswd);
				}
				sprintf(token, "%c|%s", costpasswd[0], coststr);
				
		  	return token;
		  }else{
		  	return "该提货卡已使用过，请勿重复使用！";
		  }
		}else{
			return "提货失败，该提货卡密码不正确！";
		}
	}
	return "提货失败，请与本服管理员联系！";
}

void sasql_OnlineBuy_add( char *coststr, int type, int num )
{
	char sqlstr[256];
	char CostPasswd[32];
	int i, j, k=0;
	for(i=0;i<num;i++){
		memset( CostPasswd, 0, sizeof( CostPasswd ) );
		CostPasswd[0]='0' + type;
		for(j=1;j<16;j++){
			if(rand() % 2==0){
				CostPasswd[j]=(rand() % 26) + 65;
			}else{
				CostPasswd[j]=(rand() % 10) + 48;
			}
		}
		sprintf(sqlstr,"INSERT INTO OnlineBuy (CostPasswd, CostStr) VALUES (BINARY'%s', '%s')", CostPasswd, coststr);
		if(!sasql_mysql_query(sqlstr)){
			k++;
		}else{
			break;
		}
	}
	if(type <= 0){
		printf("成功添加%d条宠物提货卡\n", k);
	}else if(type == 1){
		printf("成功添加%d条物品提货卡\n", k);
	}else if(type >= 2){
		printf("成功添加%d条石币提货卡\n", k);
	}
	return ;
}

#endif


int sasql_onlinenum( char *IP )
{
	char sqlstr[256];
	sprintf(sqlstr,"select count(*) from %s where Online=1 and IP='%s'",config.sql_Table,IP);
	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
		mysql_result=mysql_store_result(&mysql);
		num_row=mysql_num_rows(mysql_result);
		if(num_row>0){
			mysql_row=mysql_fetch_row(mysql_result);
			return atoi(mysql_row[0]);
		}
	}
	return 0;
}

#ifdef _FORMULATE_AUTO_PK
BOOL sasql_add_FormulateAutoPk( char *ID, int point )
{
	char sqlstr[256];
	sprintf(sqlstr,"select PKPoint from %s where %s=BINARY'%s'",config.sql_Table,config.sql_NAME,ID);
	if(!sasql_mysql_query(sqlstr)){
		int num_row=0;
		mysql_free_result(mysql_result);
		mysql_result=mysql_store_result(&mysql);
		num_row=mysql_num_rows(mysql_result);
		if(num_row>0){
			mysql_row=mysql_fetch_row(mysql_result);
			point+=atoi(mysql_row[0]);
			if(point < 0){
				return -1;
			}
			sprintf(sqlstr,"update %s set PKPoint=%d where %s=BINARY'%s'", config.sql_Table, point,config.sql_NAME, ID);	
			if(!sasql_mysql_query(sqlstr)){
				return point;
			}
		}
	}
	return -1;
}
#endif

#ifdef _OLDPS_TO_MD5PS
void sasql_OldpsToMd5ps()
{
	char sqlstr[256];
	sprintf(sqlstr,"select %s, %s from %s",config.sql_NAME, config.sql_PASS, config.sql_Table);
	if(!sasql_mysql_query(sqlstr)){
		mysql_free_result(mysql_result);
		mysql_result=mysql_store_result(&mysql);
		printf("      转换账号             原密码               转换MD5码\n");
		while((mysql_row = mysql_fetch_row(mysql_result))){
			char * name = mysql_row[0];
			char * ps = mysql_row[1];                     
	    if(strlen(ps)<=16) {                        
	    	sprintf(sqlstr,"update %s set %s=BINARY'%s' where %s=BINARY'%s'", config.sql_Table, config.sql_PASS, MD5String(ps),config.sql_NAME, name);	
				if(!sasql_mysql_query(sqlstr)){
					printf("%16s  %16s    %32s\n", name, ps, MD5String(ps));
				}
	    }
	  }
	}
}
#endif


#ifdef _CHARADATA_SAVE_SQL
void sasql_charadata_Save(char *id, char* table, char *data, int saveindex, int flag)
{
	if(strlen(data)==0)return;
	
	char sqlstr[1024*16];
	
	if(flag==INSERT){
		sprintf(sqlstr, "INSERT INTO %s VALUES %s" , table, data);
	}else if(flag==SELECT){
	}else if(flag==UPDATE){
		sprintf(sqlstr, "UPDATE %s set %s WHERE CHAR_CDKEY=BINARY'%s' and CHAR_SAVEINDEX=%d" , table, data, id, saveindex);
	}else if(flag==DELETE){
		sprintf(sqlstr, "DELETE FROM %s WHERE CHAR_CDKEY=BINARY'%s' and CHAR_SAVEINDEX=%d" , table, id, saveindex);
	}
	
	if(!mysql_query(&mysql,sqlstr)){
		if(flag==DELETE){
			sasql_charadata_Save(id, table, data, saveindex, INSERT);
		}
		return;
	}else{
		printf(table);
		if(flag==INSERT){
			printf(" INSERT error\n");
		}else if(flag==SELECT){
			printf(" SELECT error\n");
		}else if(flag==UPDATE){
			printf(" UPDATE error\n");
		}else if(flag==DELETE){
			printf(" DELETE error\n");
		}
	}
}

#endif


BOOL sasql_craete_userinfo( void )
{
	return FALSE;
}

BOOL sasql_craete_lock( void )
{
	return FALSE;
}

#endif
