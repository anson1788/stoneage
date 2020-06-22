
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/pc.h"
#include "../systeminc/menu.h"
#include "../systeminc/t_music.h"
#include "../systeminc/battlemenu.h"
#include "../systeminc/netmain.h"
#include "../systeminc/lssproto_cli.h"
#include "../NewProto/protocol.h"
#include "../wgs/descrypt.h"
#include"../systeminc/login.h"

#define AI_FILE_NAME "data\\AISetting.dat"
#define TARGET_MYSELF		1  //自己
#define TARGET_MYPET		2   //宠物
#define TARGET_MYSIDE		3   //我方
#define TARGET_OTHERONE		4   //另一边
#define TARGET_OTHERSIDE	5  //对方
#define TARGET_ALL			6   //全体

#define TARGET_1			7       //目标一
#define TARGET_2			8    
#define TARGET_3			9
#define TARGET_4			10
#define TARGET_5			11      //目标五

#define TARGET_ONE_ROW	12

#ifndef _ATTACK_AI
int AI_method[4];
int	AI_target[4];
int AI_limit[4];
#else
int AI_method[5];
int	AI_target[5];
int AI_limit[5];
void AI_SetUpAttackMethod(int which);
void AI_SetDownAttackMethod(int which);
#endif

#ifdef _AI_OTHER
ACTION *pAIOtherFunctionWnd = NULL;
int AI_Other_State = 0;
#ifdef _AI_CAPTURE
int AI_OtherSetting[2];
#else 
int AI_OtherSetting[1];
#endif
#endif
#ifdef _AI_NEW_FIRST
bool bFirstFlag = false;	// true:use skill false:use magic
#endif
int AI_State = 0;
int AI_First = 1;
char *AI_Choosen = NULL;
static int AICnt = 0;
ACTION *pAISettingWnd = NULL;

extern int IdEncryptionTbl[];

#ifdef _READ16BITBMP
extern BOOL g_bUseAlpha;
#endif

#ifdef _ATTACK_AI
int g_iPSindex = -1;
bool g_bUsePS = false;
#endif

BOOL AI_Load( char *user)
{
	char id[16];
	char id2[16];
	FILE	*fp;
	BOOL ret = FALSE;
	int i;
	memcpy( id, user, 16);
	AI = AI_NONE;
	AI_method[0] = MAX_MAGIC;
	AI_target[0] = TARGET_MYSELF;
	AI_limit[0] = 30;
#ifndef _ATTACK_AI
	AI_method[1] = MAX_MAGIC;
#else
	AI_method[1] = -1;
#endif
	AI_target[1] = TARGET_MYSELF;
	AI_limit[1] = 30;
	AI_method[2] = -1;
	AI_method[3] = MAX_MAGIC;
	AI_target[3] = TARGET_MYSELF;
#ifdef _ATTACK_AI
	AI_method[4] = MAX_PROFESSION_SKILL;
	AI_target[4] = TARGET_OTHERONE;
#endif
#ifdef _AI_OTHER
	//AI_OtherSetting[0] = 0;
#endif
#ifdef _AI_NEW_FIRST
	bFirstFlag = false;
#endif
	if( (fp=fopen( AI_FILE_NAME, "r+b")) == NULL){
		if( (fp=fopen( AI_FILE_NAME, "wb")) != NULL){
			for( i = 0; i < 16; i++)
				id[i] ^= IdEncryptionTbl[16-i];
			fwrite( id, sizeof(id), 1, fp);
			fwrite( &AI, sizeof(AI), 1, fp);
			fwrite( AI_method, sizeof(AI_method), 1, fp);
			fwrite( AI_target, sizeof(AI_target), 1, fp);
			fwrite( AI_limit, sizeof(AI_limit), 1, fp);
#ifdef _AI_OTHER
			fwrite(AI_OtherSetting,sizeof(AI_OtherSetting),1,fp);
#endif
#ifdef _AI_NEW_FIRST
			fwrite(&bFirstFlag,sizeof(bFirstFlag),1,fp);
#endif
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[16-i];
			fwrite( id, sizeof(id), 1, fp);
			fwrite( &AI, sizeof(AI), 1, fp);
			fwrite( AI_method, sizeof(AI_method), 1, fp);
			fwrite( AI_target, sizeof(AI_target), 1, fp);
			fwrite( AI_limit, sizeof(AI_limit), 1, fp);
#ifdef _AI_OTHER
			fwrite(AI_OtherSetting,sizeof(AI_OtherSetting),1,fp);
#endif
#ifdef _AI_NEW_FIRST
			fwrite(&bFirstFlag,sizeof(bFirstFlag),1,fp);
#endif
			break;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[i] ^= IdEncryptionTbl[16-i];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			if( fseek( fp, sizeof(AI)+sizeof(AI_method)+sizeof(AI_target)+sizeof(AI_limit)
#ifdef _AI_OTHER
				+sizeof(AI_OtherSetting)
#endif
#ifdef _AI_NEW_FIRST
				+sizeof(bFirstFlag)
#endif
				,SEEK_CUR) != 0)
			{
				fwrite( &AI, sizeof(AI), 1, fp);
				fwrite( AI_method, sizeof(AI_method), 1, fp);
				fwrite( AI_target, sizeof(AI_target), 1, fp);
				fwrite( AI_limit, sizeof(AI_limit), 1, fp);
#ifdef _AI_OTHER
				fwrite(AI_OtherSetting,sizeof(AI_OtherSetting),1,fp);
#endif
#ifdef _AI_NEW_FIRST
				fwrite(&bFirstFlag,sizeof(bFirstFlag),1,fp);
#endif
				break;
			}
		}else{
			if( fread( &AI, sizeof(AI), 1, fp)<1 ||
#ifndef _ATTACK_AI
				fread( AI_method, sizeof(int), 4, fp)<4 ||
				fread( AI_target, sizeof(int), 4, fp)<4 ||
				fread( AI_limit, sizeof(int), 4, fp)<4
#else
				fread( AI_method, sizeof(int), 5, fp)<5 ||
				fread( AI_target, sizeof(int), 5, fp)<5 ||
				fread( AI_limit, sizeof(int), 5, fp)<5
	#ifdef _AI_OTHER
				|| fread(AI_OtherSetting,sizeof(int),1,fp) < 1
	#endif
	#ifdef _AI_NEW_FIRST
				|| fread(&bFirstFlag,sizeof(bool),1,fp) < 1
	#endif
#endif
			){
				fwrite( &AI, sizeof(AI), 1, fp);
				fwrite( AI_method, sizeof(AI_method), 1, fp);
				fwrite( AI_target, sizeof(AI_target), 1, fp);
				fwrite( AI_limit, sizeof(AI_limit), 1, fp);
#ifdef _AI_OTHER
				fwrite(AI_OtherSetting,sizeof(AI_OtherSetting),1,fp);
#endif
#ifdef _AI_NEW_FIRST
				fwrite(&bFirstFlag,sizeof(bFirstFlag),1,fp);
#endif
				break;
			}
			ret = TRUE;
			break;
		}
		AICnt++;
	}
	fclose( fp );
#ifdef _AI_NEW_FIRST
	// 没有职业
	if(pc.profession_class == 0) bFirstFlag = false;
#endif
	return ret;
}

BOOL AI_ReLoad()
{
	FILE	*fp;
	BOOL	ret = FALSE;
	if( fp=fopen( AI_FILE_NAME, "rb")){
		if( 0 == fseek( fp, (16+sizeof(AI)+sizeof(AI_method)+sizeof(AI_target)+sizeof(AI_limit)
#ifdef _AI_OTHER
			+sizeof(AI_OtherSetting)
#endif
#ifdef _AI_NEW_FIRST
			+sizeof(bFirstFlag)
#endif
			)*AICnt+16, SEEK_SET))
		{
			if( fread( &AI, sizeof(AI), 1, fp)>=1 &&
				fread( AI_method, sizeof(AI_method), 1, fp)>=1 &&
				fread( AI_target, sizeof(AI_target), 1, fp)>=1 &&
				fread( AI_limit, sizeof(AI_limit), 1, fp)>=1
#ifdef _AI_OTHER
				&& fread(AI_OtherSetting,sizeof(AI_OtherSetting),1,fp) >= 1
#endif
#ifdef _AI_NEW_FIRST
				&& fread(&bFirstFlag,sizeof(bFirstFlag),1,fp) >= 1
#endif
				) ret = TRUE;
		}
		fclose( fp);
	}
	return ret;
}

BOOL AI_Save()
{
	FILE	*fp;
	BOOL	ret = FALSE;
	if( fp=fopen( AI_FILE_NAME, "r+b")){
		if( 0 == fseek( fp, (16+sizeof(AI)+sizeof(AI_method)+sizeof(AI_target)+sizeof(AI_limit)
#ifdef _AI_OTHER
			+sizeof(AI_OtherSetting)
#endif
#ifdef _AI_NEW_FIRST
			+sizeof(bFirstFlag)
#endif
			)*AICnt+16, SEEK_SET))
		{
			if( fwrite( &AI, sizeof(AI), 1, fp)>=1 &&
				fwrite( AI_method, sizeof(AI_method), 1, fp)>=1 &&
				fwrite( AI_target, sizeof(AI_target), 1, fp)>=1 &&
				fwrite( AI_limit, sizeof(AI_limit), 1, fp)>=1
#ifdef _AI_OTHER
				&& fwrite(AI_OtherSetting,sizeof(AI_OtherSetting),1,fp) >= 1
#endif
#ifdef _AI_NEW_FIRST
				&& fwrite(&bFirstFlag,sizeof(bFirstFlag),1,fp) >= 1
#endif
				)	ret = TRUE;
			fclose( fp);
		}
	}
	return ret;
}

#ifdef _ATTACK_AI
int AI_CheckSkillSetting(int methodkind,int method,int UpDown)
{
	int ret = 1;
	if(methodkind == 4 && method == MAX_PROFESSION_SKILL){
		AI_target[methodkind] = TARGET_OTHERONE;
		return ret;
	}
	else if(methodkind == 2 && method == MAX_PROFESSION_SKILL){
		AI_method[methodkind] = -1;
		return ret;
	}

	if(profession_skill[method].costmp <= 0 
		&& profession_skill[method].skillId != 11 
		&& profession_skill[method].skillId != 75) return 0;

	if(AI_target[methodkind] == 0) AI_target[methodkind] = TARGET_ONE_ROW;


	switch(profession_skill[method].skillId){
		case 1://火山泉			敌单人攻击型态
		case 2://针针相对		敌多人攻击型态
		case 3://世界末日  	敌多人攻击型态
		case 4://冰爆术			敌多人攻击型态
		case 5://附身术			敌单人攻击型态
		case 6://召雷术			敌单人攻击型态
		case 7://暴风雨			敌多人攻击型态
		case 8://电流术			敌多人攻击型态
		case 9://火星球			敌多人攻击型态
		case 10://嗜血蛊		敌单人攻击型态
		case 12://冰箭术		敌单人攻击型态		
		case 13://火龙枪		敌多人攻击型态
		case 14://冰镜术		敌单人攻击型态
		case 22://爆击			敌单人攻击型态
		case 23://连环攻击	敌单人攻击型态
		case 24://双重攻击	敌单人攻击型态
		case 38://盾击			敌单人攻击型态
		case 39://贯穿攻击	敌单人攻击型态
		case 40://濒死攻击	敌单人攻击型态
		case 41://回旋攻击	敌多人攻击型态
		case 42://混乱攻击	敌多人攻击型态
		case 46://树根缠绕	敌单人攻击型态
		case 48://天罗地网	敌单人攻击型态
		case 49://尸体掠夺	敌单人攻击型态
		case 50://毒素武器	敌单人攻击型态
		case 51://弱点攻击	敌单人攻击型态
		case 54://座骑攻击	敌单人攻击型态
		case 72://破除结界  敌我单体
			if(AI_target[methodkind] != TARGET_OTHERONE) AI_target[methodkind] = TARGET_OTHERONE;
			break;
		case 15://火附体		我一方使用
		case 16://雷附体		我一方使用
		case 17://冰附体		我一方使用
			// down
			if(UpDown){
				if(AI_target[methodkind] == TARGET_MYPET || AI_target[methodkind] == TARGET_MYSIDE) AI_target[methodkind] = TARGET_OTHERONE;
				if(AI_target[methodkind] == TARGET_OTHERSIDE || AI_target[methodkind] == TARGET_ALL) AI_target[methodkind] = TARGET_1;
				if(AI_target[methodkind] == TARGET_ONE_ROW) AI_target[methodkind] = TARGET_MYSELF;
			}
			// up
			else{
				if(AI_target[methodkind] == TARGET_MYPET || AI_target[methodkind] == TARGET_MYSIDE) AI_target[methodkind] = TARGET_MYSELF;
				if(AI_target[methodkind] == TARGET_OTHERSIDE || AI_target[methodkind] == TARGET_ALL) AI_target[methodkind] = TARGET_OTHERONE;
				if(AI_target[methodkind] == TARGET_ONE_ROW) AI_target[methodkind] = TARGET_5;
			}
			break;
		case 21://移形换位	本体使用
		case 35://激化攻击	本体使用
		case 36://能量聚集	本体使用
		case 37://专注战斗	本体使用
		case 47://陷阱			本体使用
		case 53://格档			本体使用
		case 59://电抗			本体使用
		case 60://火抗			本体使用
		case 61://冰抗			本体使用
		case 66://自然威能	本体使用
			if(AI_target[methodkind] != TARGET_MYSELF) AI_target[methodkind] = TARGET_MYSELF;
			break;
		case 34://舍已为友	我一方使用

#ifdef _AI_SKILL_UPDATE
		case 76://裂骨断筋
			AI_target[methodkind] = TARGET_OTHERONE;	//顺序 敌方
			break;
		case 77://战狼怒吼		
			AI_target[methodkind] = TARGET_OTHERONE;	//顺序 敌方
			break;
		case 79://魔力咒印		
			AI_target[methodkind] = TARGET_OTHERONE;	//顺序 敌方
			break;
		case 80://恶魔诅咒		
			AI_target[methodkind] = TARGET_OTHERONE;	//顺序 敌方
			break;
		case 82://多重冰箭		
			AI_target[methodkind] = TARGET_ONE_ROW;	    //顺序 敌一排
			break;
		case 83://毒素之网		
			AI_target[methodkind] = TARGET_OTHERONE;	//顺序 敌方
			break;

		case 85://战场急救
			//if (methodkind != 4) return 0;
			// down
			if(UpDown){
				if(AI_target[methodkind] == TARGET_OTHERONE || AI_target[methodkind] == TARGET_MYSIDE) AI_target[methodkind] = TARGET_MYPET;
				if(AI_target[methodkind] == TARGET_OTHERSIDE || AI_target[methodkind] == TARGET_ALL) AI_target[methodkind] = TARGET_1;
				if(AI_target[methodkind] == TARGET_ONE_ROW) AI_target[methodkind] = TARGET_MYSELF;
			}
			// up
			else{
				if(AI_target[methodkind] == TARGET_OTHERONE || AI_target[methodkind] == TARGET_MYSIDE) AI_target[methodkind] = TARGET_MYSELF;
				if(AI_target[methodkind] == TARGET_OTHERSIDE || AI_target[methodkind] == TARGET_ALL) AI_target[methodkind] = TARGET_MYPET;
				if(AI_target[methodkind] == TARGET_ONE_ROW) AI_target[methodkind] = TARGET_5;
			}
			break;
		case 75://四方防御	
			if (methodkind != 2) return 0;
			AI_target[methodkind] = TARGET_MYSELF;	    //顺序 自己
			break;
		case 67://号召自然	我方全体
			//if (methodkind != 4) return 0;
			AI_target[methodkind] = TARGET_MYSIDE;	    //顺序 我方
			break;
#endif
		case 68://地结界		我方全体
		case 69://水结界		我方全体
		case 70://火结界		我方全体
		case 71://风结界		我方全体
			if(AI_target[methodkind] != TARGET_MYSIDE) AI_target[methodkind] = TARGET_MYSIDE;
			break;
		case 52://挑拨			敌单人攻击型态(限宠物)
		case 56://驯服宠物	我一方使用
		case 62://遗忘			敌单人攻击型态(限宠物)
			if(AI_target[methodkind] != TARGET_OTHERONE) AI_target[methodkind] = TARGET_OTHERONE;
			break;
		case 57://激怒宠物	我一方使用
			if(AI_target[methodkind] != TARGET_MYPET) AI_target[methodkind] = TARGET_MYPET;
			break;
		case 11://嗜血成性	本体吸收
		case 18://火熟练度	无法使用
		case 19://雷熟练度	无法使用
		case 20://冰熟练度	无法使用
		case 25://回避			无法使用
		case 26://精通枪		无法使用
		case 27://精通斧		无法使用
		case 28://精通棍		无法使用
		case 33://状态回复	无法使用
		case 43://二刀流		无法使用
		case 44://追寻敌踪	非战斗使用
		case 45://回避战斗	非战斗使用
		case 55://加工			?
		case 58://自给自足	?
		case 29://精通弓		无法使用
		case 30://精通回力镖		无法使用
		case 31://精通投掷石		无法使用
		case 32://精通投掷斧		无法使用
		default:
			ret = 0;
			break;
	}
	return ret;
}
#endif

void AI_CheckSetting()
{
	char *str;
	int method;
	if( AI)
		AI = AI_SELECT;
	int i;
#ifndef _ATTACK_AI
	for( i = 0; i < 4; i++){
#else
	for( i = 0; i < 5; i++){
#endif
		if( AI_limit[i]>100 || AI_limit[i]<0)
			AI_limit[i] = 30;
		else if( AI_limit[i]%5)
			AI_limit[i] = 30;
	}
#ifdef _ATTACK_AI
	// 确认玩家身上的 嗜血成性 在那个位置
	i = 0;
	g_iPSindex = -1;
	g_bUsePS = false;
	while(1){
		if(profession_skill[i].useFlag == 1 && profession_skill[i].skillId == 11){
			g_iPSindex = i;
			g_bUsePS = true;
		}
		if(++i >= MAX_PROFESSION_SKILL) break;
	}
#endif
#ifdef _AI_OTHER
	if(AI_OtherSetting[0] < 0) AI_OtherSetting[0] = 0;
#endif
	method = AI_method[0];
	if( method<MAX_MAGIC && method>=0){//使用精灵
		if( magic[method].useFlag && (str=strstr( magic[method].memo, "耐久力")) && strstr( str, "回复")){//使用耐久力回复精灵
			switch( magic[method].target){
			case MAGIC_TARGET_MYSELF:
				AI_target[0] = TARGET_MYSELF;			//自己
				break;
			case MAGIC_TARGET_ALLMYSIDE:
			case MAGIC_TARGET_WHOLEOTHERSIDE:
				AI_target[0] = TARGET_MYSIDE;			//我方全体
				break;
			case MAGIC_TARGET_OTHER:
				/*
				if( AI_target[0]!=TARGET_MYSELF && AI_target[0]!=TARGET_MYPET &&
					(TARGET_1>AI_target[0] || AI_target[0]>TARGET_5))//检查对象是否错误
					AI_target[0] = TARGET_MYSELF;	//自己
					*/
				break;
			default:
//			case MAGIC_TARGET_ALLOTHERSIDE:				//不该使用这种精灵回复
//			case MAGIC_TARGET_ALL:
				AI_method[0] = MAX_MAGIC;			//设为使用道具
				AI_target[0] = TARGET_MYSELF;
				break;
			}
		}else{//没有耐久力回复精灵
			AI_method[0] = MAX_MAGIC;
			AI_target[0] = TARGET_MYSELF;
		}
	}else{//使用道具补耐久力
		AI_method[0] = MAX_MAGIC;
		if( AI_target[0]!=TARGET_MYSELF && AI_target[0]!=TARGET_MYPET &&
			(TARGET_1>AI_target[0] || AI_target[0]>TARGET_5)){//使用道具但对象不对
			AI_target[0] = TARGET_MYSELF;
		}
	}
#ifndef _ATTACK_AI
	AI_method[1] = MAX_MAGIC;
#else
	if(!g_bUsePS) AI_method[1] = -1;
#endif
	AI_target[1] = TARGET_MYSELF;

	method = AI_method[2];
#ifdef _AI_NEW_FIRST
	// 首次精灵:使用精灵
	if(bFirstFlag == false){
#endif
		if( method<MAX_MAGIC && method>=0 && magic[method].useFlag){//使用首次精灵
			switch( magic[method].target){
			case MAGIC_TARGET_MYSELF:
				if( AI_target[2] != TARGET_MYSELF)
					AI_method[2] = -1;
				break;
			case MAGIC_TARGET_ALLMYSIDE:
				if( AI_target[2] != TARGET_MYSIDE)
					AI_method[2] = -1;
				break;
			case MAGIC_TARGET_WHOLEOTHERSIDE:
				if( AI_target[2]!=TARGET_MYSIDE && AI_target[2]!=TARGET_OTHERSIDE)
					AI_method[2] = -1;
				break;
			case MAGIC_TARGET_OTHER:
				if( AI_target[2]!=TARGET_MYSELF && AI_target[2]!=TARGET_MYPET && AI_target[2]!=TARGET_OTHERONE &&
					(TARGET_1>AI_target[2] || AI_target[2]>TARGET_5))
					AI_method[2] = -1;
				break;
			case MAGIC_TARGET_ALLOTHERSIDE:
				if( AI_target[2] != TARGET_OTHERSIDE)
					AI_method[2] = -1;
				break;
			case MAGIC_TARGET_ALL:
				if( AI_target[2] != TARGET_ALL)
					AI_method[2] = -1;
				break;
#ifdef __ATTACK_MAGIC
				// 敌军某一人
			case MAGIC_TARGET_SINGLE:
				if( AI_target[2] != TARGET_OTHERONE)
					AI_method[2] = -1;
				break;
				// 敌军一整排
			case MAGIC_TARGET_ONE_ROW:
				if( AI_target[2]!=TARGET_ONE_ROW)
					AI_method[2] = -1;
				break;
				// 敌军全体
			case MAGIC_TARGET_ALL_ROWS:
				if( AI_target[2] != TARGET_OTHERSIDE)
					AI_method[2] = -1;
				break;
#endif
			default:
				AI_method[2] = -1;
				break;
			}
		}else{//没有这个精灵
			AI_method[2] = -1;
		}
#ifdef _AI_NEW_FIRST
	}
	// 首次精灵:使用技能
	else{
		if(pc.profession_class != 0 && // 有职业
			(method < MAX_PROFESSION_SKILL && method >= 0) &&
			AI_CheckSkillSetting(2,method,0) == 0) AI_method[2] = -1;
	}
#endif
	method = AI_method[3];
	if( method<MAX_MAGIC && method>=0){//使用精灵
		if( !magic[method].useFlag || !(str=strstr( magic[method].memo, "气绝状态")) || !strstr( str, "回复")){//没有气绝状态回复精灵
			AI_method[3] = MAX_MAGIC;
		}
	}else{//使用道具回复气绝状态
		AI_method[3] = MAX_MAGIC;
	}
	if( AI_target[3]!=TARGET_MYSIDE && AI_target[3]!=TARGET_MYPET && (TARGET_1>AI_target[3] || AI_target[3]>TARGET_5))
		AI_target[3] = TARGET_MYPET;

#ifdef _ATTACK_AI
	method = AI_method[4];
	if(pc.profession_class != 0 && // 有职业
		(method < MAX_PROFESSION_SKILL && method >= 0)){// 使用职业技能
		if(AI_CheckSkillSetting(4,method,0) == 0){
			AI_method[4] = MAX_PROFESSION_SKILL;
			AI_target[4] = TARGET_OTHERONE;
		}
	}
	else{// 普通攻击
		AI_method[4] = MAX_PROFESSION_SKILL;
		AI_target[4] = TARGET_OTHERONE;
	}
#endif
	AI_Save();
}

void AI_Init()
{
	char id[32];
	AI = AI_NONE;
	PauseAI = 0;
	AICnt = 0;
	extern char szUser[];
	CopyMemory( id, szUser, 32);
	ecb_crypt( "f;encor1c", id, 32, DES_DECRYPT);
	id[15] = strlen(id);
	if( AI_Load( id))
		AI_CheckSetting();
}

void AI_SetUpTarget( int which)
{
	int method = AI_method[which];
	//printf("method=%d  which=%d   magic[method].target=%d   AI_target[which]=%d\n",method,which,magic[method].target,AI_target[which]
	//);
#ifdef _ATTACK_AI
	if(which == 4
#ifdef _AI_NEW_FIRST
		|| (which == 2 && bFirstFlag == true)
#endif
	){
		AI_target[which]--;
		AI_CheckSkillSetting(which,method,0);
	}else
#endif
	if( method >= MAX_MAGIC){
		if( TARGET_1<=AI_target[which] && AI_target[which]<=TARGET_5){
			AI_target[which]--;
			if( AI_target[which] < TARGET_1) AI_target[which] = TARGET_MYSELF;
		}
		else if( AI_target[which] == TARGET_MYPET)	AI_target[which] = TARGET_5;
		else AI_target[which] = TARGET_MYPET;
	}else if( method >= 0)
	{
		switch( magic[method].target)
		{
		case MAGIC_TARGET_MYSELF:
			AI_target[which] = TARGET_MYSELF;			//自己
			break;
		case MAGIC_TARGET_ALLMYSIDE:
			AI_target[which] = TARGET_MYSIDE;			//我方全体
			break;
		case MAGIC_TARGET_WHOLEOTHERSIDE:
			if( which !=0)
			{
				if( AI_target[which] == TARGET_MYSIDE)
					AI_target[which] = TARGET_OTHERSIDE;	//敌方全体
				else
					AI_target[which] = TARGET_MYSIDE;		//我方全体
			}
			break;
		case MAGIC_TARGET_OTHER:
			if( TARGET_1<=AI_target[which] && AI_target[which]<=TARGET_5)
			{
				AI_target[which]--;
				if( AI_target[which] < TARGET_1)
					AI_target[which] = TARGET_MYSELF;
			}else if( AI_target[which] == TARGET_MYPET)
			{
				if( which != 0)
					AI_target[which] = TARGET_OTHERONE;
				else
					AI_target[which] = TARGET_MYSIDE;
			}else if( AI_target[which] == TARGET_MYSIDE){
				if( which != 0)
					AI_target[which] = TARGET_OTHERONE;
				else
					AI_target[which] = TARGET_5;
			}else if( AI_target[which] == TARGET_OTHERONE)
			{
				AI_target[which] = TARGET_5;
			}else
			{
				AI_target[which] = TARGET_MYPET;
			}
			break;
		case MAGIC_TARGET_ALLOTHERSIDE:
			AI_target[which] = TARGET_OTHERSIDE;		//敌方全体
			break;
		case MAGIC_TARGET_ALL:
			AI_target[which] = TARGET_ALL;				//全体
			break;
#ifdef __ATTACK_MAGIC
		// 敌军某一人
		case MAGIC_TARGET_SINGLE:
			AI_target[which] = TARGET_OTHERONE;
			break;
		// 敌军一整排
		case MAGIC_TARGET_ONE_ROW:
			AI_target[which] = TARGET_ONE_ROW;
			break;
		// 敌军全体
		case MAGIC_TARGET_ALL_ROWS:
			AI_target[which] = TARGET_OTHERSIDE;
			break;
#endif
		}
//		if( magic[method].target == MAGIC_TARGET_OTHER)
//			AI_target[which] ^= 1;
	}
}

void AI_SetDownTarget( int which)
{
	int method = AI_method[which];
#ifdef _ATTACK_AI
	if(which == 4
#ifdef _AI_NEW_FIRST
		|| (which == 2 && bFirstFlag == true)
#endif
	){
		AI_target[which]++;
		AI_CheckSkillSetting(which,method,1);
	}else
#endif
	if( method >= MAX_MAGIC){
		if( TARGET_1<=AI_target[which] && AI_target[which]<=TARGET_5){
			AI_target[which]++;
			if( AI_target[which] > TARGET_5)
				AI_target[which] = TARGET_MYPET;
		}else if( AI_target[which] == TARGET_MYPET){
			AI_target[which] = TARGET_MYSELF;
		}else{
			AI_target[which] = TARGET_1;
		}
/*		if( AI_target[which] == TARGET_MYSELF){
			if( which == 2)
				AI_target[which] = TARGET_OTHERONE;		//敌方
			else
				AI_target[which] = TARGET_MYPET;		//宠物
		}else if( AI_target[which] == TARGET_OTHERONE)
			AI_target[which] = TARGET_MYPET;			//宠物
		else// if( AI_target[which] == TARGET_MYPET)
			AI_target[which] = TARGET_MYSELF;			//自己*/
	}else if( method >= 0){
		switch( magic[method].target){
		case MAGIC_TARGET_MYSELF:
			AI_target[which] = TARGET_MYSELF;			//自己
			break;
		case MAGIC_TARGET_ALLMYSIDE:
			AI_target[which] = TARGET_MYSIDE;			//我方全体
			break;
		case MAGIC_TARGET_WHOLEOTHERSIDE:
			if( which != 0){
				if( AI_target[which] == TARGET_MYSIDE)
					AI_target[which] = TARGET_OTHERSIDE;	//敌方全体
				else
					AI_target[which] = TARGET_MYSIDE;		//我方全体
			}
			break;
		case MAGIC_TARGET_OTHER:
			if( TARGET_1<=AI_target[which] && AI_target[which]<=TARGET_5){
				AI_target[which]++;
				if( AI_target[which] > TARGET_5){
					if( which != 0)
						AI_target[which] = TARGET_OTHERONE;
					else
						AI_target[which] = TARGET_MYPET;
				}
			}else if( AI_target[which] == TARGET_OTHERONE){
				AI_target[which] = TARGET_MYPET;
			}else if( AI_target[which] == TARGET_MYPET){
				AI_target[which] = TARGET_MYSELF;
			}else if( AI_target[which] == TARGET_MYSELF){
				AI_target[which] = TARGET_MYSIDE;
			}else{
				AI_target[which] = TARGET_1;
			}
/*			if( AI_target[which] == TARGET_MYSELF){
				if( which == 2)
					AI_target[which] = TARGET_OTHERONE;	//敌方
				else
					AI_target[which] = TARGET_MYPET;	//宠物
			}else if( AI_target[which] == TARGET_OTHERONE)
				AI_target[which] = TARGET_MYPET;		//宠物
			else// if( AI_target[which] == TARGET_MYPET)
				AI_target[which] = TARGET_MYSELF;		//自己*/
			break;
		case MAGIC_TARGET_ALLOTHERSIDE:
			AI_target[which] = TARGET_OTHERSIDE;		//敌方全体
			break;
		case MAGIC_TARGET_ALL:
			AI_target[which] = TARGET_ALL;				//全体
			break;
#ifdef __ATTACK_MAGIC
		// 敌军某一人
		case MAGIC_TARGET_SINGLE:
			AI_target[which] = TARGET_OTHERONE;
			break;
		// 敌军一整排
		case MAGIC_TARGET_ONE_ROW:
			AI_target[which] = TARGET_ONE_ROW;
			break;
		// 敌军全体
		case MAGIC_TARGET_ALL_ROWS:
			AI_target[which] = TARGET_OTHERSIDE;
			break;
#endif
		}
//		if( magic[method].target == MAGIC_TARGET_OTHER)
//			AI_target[which] ^= 1;
	}
}

void AI_SetMethod( int target, int which)
{
	switch( target){
	case MAGIC_TARGET_MYSELF:					//自己
		AI_target[which] = TARGET_MYSELF;
		break;
	case MAGIC_TARGET_ALLMYSIDE:				//我方全体
		AI_target[which] = TARGET_MYSIDE;
		break;
	case MAGIC_TARGET_WHOLEOTHERSIDE:			//任何一方全体
		AI_target[which] = TARGET_MYSIDE;
		break;
	case MAGIC_TARGET_OTHER:					//任何一体
		AI_target[which] = TARGET_MYSELF;
		break;
	case MAGIC_TARGET_ALLOTHERSIDE:				//敌方一体
		AI_target[which] = TARGET_OTHERSIDE;
		break;
	case MAGIC_TARGET_ALL:						//全体
		AI_target[which] = TARGET_ALL;
		break;
#ifdef __ATTACK_MAGIC
	// 敌军某一人
	case MAGIC_TARGET_SINGLE:
		AI_target[which] = TARGET_OTHERONE;
		break;
	// 敌军一整排
	case MAGIC_TARGET_ONE_ROW:
		AI_target[which] = TARGET_ONE_ROW;
		break;
	// 敌军全体
	case MAGIC_TARGET_ALL_ROWS:
		AI_target[which] = TARGET_OTHERSIDE;
		break;
#endif
	}
}

void AI_SetUpCureMethod( int which)
{
	char *str;
	int method = AI_method[which]+1;
	if( method > MAX_MAGIC)
		method = 0;
	AI_method[which] = MAX_MAGIC;
	AI_target[which] = TARGET_MYSELF;
	for( int i = method; i < MAX_MAGIC; i++){
		if( magic[i].useFlag && (str=strstr( magic[i].memo, "耐久力")) && strstr( str, "回复")){
			AI_method[which] = i;
			AI_SetMethod( magic[i].target, which);
			break;
		}
	}
/*	if( i >= MAX_MAGIC){
		for( i = 0; i < MAX_ITEM; i++){
			if( pc.item[i].useFlag && (str=strstr( pc.item[i].memo, "耐久力")) && strstr( str, "回复")){
				AI_method[which] = MAX_MAGIC;
				AI_target[which] &= 1;
				break;
			}
		}
	}*/
}

void AI_SetDownCureMethod( int which)
{
	char *str;
	int method = AI_method[which];
	if( method < 0)
		method = MAX_MAGIC;
	AI_method[which] = MAX_MAGIC;
	AI_target[which] = TARGET_MYSELF;
	for( int i = (method-1); i >= 0; i--){
		if( magic[i].useFlag && (str=strstr( magic[i].memo, "耐久力")) && strstr( str, "回复")){
			AI_method[which] = i;
			AI_SetMethod( magic[i].target, which);
			break;
		}
	}
/*	if( i < 0){
		for( i = 0; i < MAX_ITEM; i++){
			if( pc.item[i].useFlag && (str=strstr( pc.item[i].memo, "耐久力")) && strstr( str, "回复")){
				AI_method[which] = MAX_MAGIC;
				AI_target[which] &= 1;
				break;
			}
		}
	}*/
}

void AI_SetUpFirstMethod( int which)
{
	int method = AI_method[which]+1;
#ifndef _AI_NEW_FIRST
	if( method > MAX_MAGIC)	method = 0;
	AI_method[which] = -1;
	for( int i = method; i < MAX_MAGIC; i++){
		if( magic[i].useFlag){
			AI_method[which] = i;
			AI_SetMethod( magic[i].target, which);
			break;
		}
	}
#else
	// 首次用精灵
	if(bFirstFlag == false){
		if(method > MAX_MAGIC){
			if(pc.profession_class != 0){ // 有职业
				AI_method[which] = -1;
				AI_SetDownAttackMethod(which);
				// 设定为用技能
				bFirstFlag = true;
				return;
			}
			else method = 0;
		}
	}
	// 首次用技能
	else{
		if(method > MAX_PROFESSION_SKILL){
			method = -1;
			// 设定为用精灵
			bFirstFlag = false;
		}
		AI_SetDownAttackMethod(which);
	}
	if(bFirstFlag == false){
		AI_method[which] = -1;
		for( int i = method; i < MAX_MAGIC; i++){
			if( magic[i].useFlag){
				AI_method[which] = i;
				AI_SetMethod( magic[i].target, which);
				break;
			}
		}
		if(AI_method[which] == -1 && pc.profession_class != 0){
			AI_SetDownAttackMethod(which);
			// 设定为用技能
			bFirstFlag = true;
		}
	}
	if(AI_method[which] == -1) bFirstFlag = false;
#endif
}

void AI_SetDownFirstMethod( int which)
{
	int method = AI_method[which];
#ifndef _AI_NEW_FIRST
	if( method < 0)	method = MAX_MAGIC;
	AI_method[which] = -1;
	for( int i = (method-1); i >= 0; i--){
		if( magic[i].useFlag){
			AI_method[which] = i;
			AI_SetMethod( magic[i].target, which);
			break;
		}
	}
#else
	// 首次用精灵
	if(bFirstFlag == false){
		if(method < 0){
			if(pc.profession_class != 0){ // 有职业
				AI_method[which] = MAX_PROFESSION_SKILL;
				AI_SetUpAttackMethod(which);
				// 设定为用技能
				bFirstFlag = true;
				return;
			}
			else method = MAX_MAGIC;
		}
	}
	// 首次用技能
	else{
		AI_SetUpAttackMethod(which);
		if(AI_method[which] == -1){
			method = MAX_MAGIC;
			bFirstFlag = false;
		}
	}
	if(bFirstFlag == false){
		AI_method[which] = -1;
		for( int i = (method-1); i >= 0; i--){
			if( magic[i].useFlag){
				AI_method[which] = i;
				AI_SetMethod( magic[i].target, which);
				break;
			}
		}
	}
#endif
}

void AI_SetUpResurrectMethod( int which)
{
	char *str;
	int method = AI_method[which]+1;
	if( method > MAX_MAGIC)
		method = 0;
	AI_method[which] = MAX_MAGIC;
	AI_target[which] = TARGET_MYPET;
	for( int i = method; i < MAX_MAGIC; i++){
		if( magic[i].useFlag && (str=strstr( magic[i].memo, "气绝状态")) && strstr( str, "回复")){
			AI_method[which] = i;
			break;
		}
	}
}

void AI_SetDownResurrectMethod( int which)
{
	char *str;
	int method = AI_method[which];
	if( method < 0)
		method = MAX_MAGIC;
	AI_method[which] = MAX_MAGIC;
	AI_target[which] = TARGET_MYPET;
	for( int i = (method-1); i >= 0; i--){
		if( magic[i].useFlag && (str=strstr( magic[i].memo, "气绝状态")) && strstr( str, "回复")){
			AI_method[which] = i;
			break;
		}
	}
}

void AI_SetUpLimit( int which)
{
	AI_limit[which] += 5;
	if( AI_limit[which] > 100)
		AI_limit[which] = 100;
}

void AI_SetDownLimit( int which)
{
	AI_limit[which] -= 5;
	if( AI_limit[which] < 0)
		AI_limit[which] = 0;
}

#ifdef _ATTACK_AI
void AI_SetUpAttackMethod(int which)
{
	int method = AI_method[which];
	while(1){
		--method;
		if(method < 0) method = MAX_PROFESSION_SKILL;
		if(method == AI_method[which] || method == MAX_PROFESSION_SKILL){
			AI_method[which] = MAX_PROFESSION_SKILL;
			if(AI_CheckSkillSetting(which,method,0) == 0) continue;
			else break;
		}
		if(profession_skill[method].useFlag == 1){
			AI_method[which] = method;
			if(AI_CheckSkillSetting(which,method,0) == 0) continue;
			else break;
		}
	}
}

void AI_SetDownAttackMethod(int which)
{
	int method = AI_method[which];
	while(1){
		++method;
		if(method > MAX_PROFESSION_SKILL) method = 0;
		if(method == AI_method[which] || method == MAX_PROFESSION_SKILL){
			AI_method[which] = MAX_PROFESSION_SKILL;
			if(AI_CheckSkillSetting(which,method,1) == 0) continue;
			else break;
		}
		if(profession_skill[method].useFlag == 1){
			AI_method[which] = method;
			if(AI_CheckSkillSetting(which,method,1) == 0) continue;
			else break;
		}
	}
}
#endif

#define AIBtnNum 24
//自动战斗设定
BOOL AI_SettingProc()
{
	static char Notice[][32] = { "储存设定", "取消设定",
		"选择回复精灵", "选择回复精灵", "选择优先顺序", "选择优先顺序", "增加耐久力忍受度", "减少耐久力忍受度",
		"建议设定高于使用精灵所需MP", "建议设定高于使用精灵所需MP",
		"选择首次精灵", "选择首次精灵", "选择使用目标", "选择使用目标",
		"选择气绝回复方式", "选择气绝回复方式", "选择使用目标", "选择使用目标"
#ifdef _ATTACK_AI
		,"选择攻击方式","选择攻击方式","选择使用目标","选择使用目标","选择回复方式","选择回复方式"
#endif
	};
	static int x = 0, y = 0;
	static DWORD dwPressTime;
	static int btnId[AIBtnNum], btnState[AIBtnNum];
	static int u_offx[] = { 209-93, 296-93, 379-93};
	static int d_offx[] = { 209-112, 296-112, 379-112};
	static char MethodName[] = "不能使用";
	char *str;
	//AI_menu_state
	if( pAISettingWnd == NULL){
		int w = 413;
		int h = 302;
		x = (lpDraw->xSize - w)/2;
		y = (lpDraw->ySize - h)/2;
#ifdef _READ16BITBMP
		if(g_bUseAlpha) pAISettingWnd = MakeWindowDisp( x, y, w, h, OLD_GRAPHICS_START + 57, -1, FALSE);
		else
#endif
		pAISettingWnd = MakeWindowDisp( x, y, w, h, 35225, -1, FALSE);

		dwPressTime = 0;
		AI_ReLoad();
		AI_CheckSetting();
		for( int i = 0 ; i < AIBtnNum ; i++){
			btnId[i] = -2;
			btnState[i] = 0;
		}
	}else{
		if( pAISettingWnd->hp >= 1){
			if( joy_trg[ 0 ] & JOY_ESC){
				DeathAction( pAISettingWnd );
				pAISettingWnd = NULL;
#ifdef _AI_OTHER
				if(pAIOtherFunctionWnd){
					DeathAction(pAIOtherFunctionWnd);
					pAIOtherFunctionWnd = NULL;
					AI_Other_State = 0;
				}
#endif
				return TRUE;
			}
			if( dwPressTime){
				if( TimeGetTime()>(dwPressTime+100)){
					dwPressTime = 0;
					if( btnState[0] == 1){//按下"确定"
						btnState[0] = 0;
						DeathAction( pAISettingWnd );
						pAISettingWnd = NULL;
#ifdef _AI_OTHER
						if(pAIOtherFunctionWnd){
							DeathAction(pAIOtherFunctionWnd);
							pAIOtherFunctionWnd = NULL;
							AI_Other_State = 0;
						}
#endif
						play_se( 217, 320, 240 );
						AI_Save();
						return TRUE;
					}else if( btnState[1] == 1){//按下"取消"
						btnState[1] = 0;
						DeathAction( pAISettingWnd );
						pAISettingWnd = NULL;
#ifdef _AI_OTHER
						if(pAIOtherFunctionWnd){
							DeathAction(pAIOtherFunctionWnd);
							pAIOtherFunctionWnd = NULL;
							AI_Other_State = 0;
						}
#endif
						play_se( 203, 320, 240 );
						return TRUE;
					}else if( btnState[2] == 1){//按下补血的精灵上一个
						btnState[2] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpCureMethod( 0);
					}else  if( btnState[3] == 1){//按下补血的精灵下一个
						btnState[3] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownCureMethod( 0);
					}else if( btnState[4] == 1){//按下补血的顺序上一个
						btnState[4] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpTarget( 0);
					}else if( btnState[5] == 1){//按下补血的顺序下一个
						btnState[5] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownTarget( 0);
					}else if( btnState[6] == 1){//按下补血的数值上一个
						btnState[6] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpLimit( 0);
					}else  if( btnState[7] == 1){//按下补血的数值下一个
						btnState[7] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownLimit( 0);
					}else if( btnState[8] == 1){//按下补气的数值上一个
						btnState[8] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpLimit( 1);
					}else  if( btnState[9] == 1){//按下补气的数值下一个
						btnState[9] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownLimit( 1);
					}else if( btnState[10] == 1){//按下首次的精灵上一个
						btnState[10] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpFirstMethod( 2);
					}else  if( btnState[11] == 1){//按下首次的精灵下一个
						btnState[11] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownFirstMethod( 2);
					}else if( btnState[12] == 1){//按下首次的顺序上一个
						btnState[12] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpTarget( 2);
					}else  if( btnState[13] == 1){//按下首次的顺序下一个
						btnState[13] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownTarget( 2);
					}else if( btnState[14] == 1){//按下复活的精灵上一个
						btnState[14] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpResurrectMethod( 3);
					}else  if( btnState[15] == 1){//按下复活的精灵下一个
						btnState[15] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownResurrectMethod( 3);
					}else if( btnState[16] == 1){//按下复活的顺序上一个
						btnState[16] = 0;
						play_se( 217, 320, 240 );
						if( TARGET_1<=AI_target[3] && AI_target[3]<=TARGET_5){
							AI_target[3]--;
							if( AI_target[3] < TARGET_1)
								AI_target[3] = TARGET_MYSIDE;
						}else if( AI_target[3] == TARGET_MYPET){
							AI_target[3] = TARGET_5;
						}else{
							AI_target[3] = TARGET_MYPET;
						}
					}else  if( btnState[17] == 1){//按下复活的顺序下一个
						btnState[17] = 0;
						play_se( 217, 320, 240 );
						if( TARGET_1<=AI_target[3] && AI_target[3]<=TARGET_5){
							AI_target[3]++;
							if( AI_target[3] > TARGET_5)
								AI_target[3] = TARGET_MYPET;
						}else if( AI_target[3] == TARGET_MYPET){
							AI_target[3] = TARGET_MYSIDE;
						}else{
							AI_target[3] = TARGET_1;
						}
					}
#ifdef _ATTACK_AI
					else if(btnState[18] == 1){// 按下攻击功能列的精灵栏的上一个
						btnState[18] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpAttackMethod(4);
					}
					else if(btnState[19] == 1){// 按下攻击功能列的精灵栏的下一个
						btnState[19] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownAttackMethod(4);
					}
					else if(btnState[20] == 1){// 按下攻击功能列的顺序栏的上一个
						btnState[20] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpTarget(4);
					}else if(btnState[21] == 1){// 按下攻击功能列的顺序栏的下一个
						btnState[21] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownTarget(4);
					}else if( btnState[22] == 1){//按下补气的精灵上一个
						btnState[22] = 0;
						play_se( 217, 320, 240 );
						// 若有补气技能
						if(g_bUsePS){
							if(AI_method[1] == -1) AI_method[1] = g_iPSindex;
							else AI_method[1] = -1;
						}
					}else if( btnState[23] == 1){//按下补气的精灵下一个
						btnState[23] = 0;
						play_se( 217, 320, 240 );
						// 若有补气技能
						if(g_bUsePS){
							if(AI_method[1] == -1) AI_method[1] = g_iPSindex;
							else AI_method[1] = -1;
						}
					}
#endif
				}
			}else{
				//是否按下按钮
				for( int i = 0; i < AIBtnNum; i++){
					if( HitDispNo == btnId[i]){
						if( mouse.onceState & MOUSE_LEFT_CRICK){
							dwPressTime = TimeGetTime();
							btnState[i] = 1;
							break;
						}
					}
				}
#ifdef _AI_OTHER
				//按下其它
				if(MakeHitBox(x+317,y+168,x+386,y+193,DISP_PRIO_BOX2))
					if(mouse.onceState & MOUSE_LEFT_CRICK) AI_Other_State = 1;
#endif
			}
			//说明文字
			for( int i = 0; i < AIBtnNum; i++){
				if( HitDispNo == btnId[i]){
					strcpy( OneLineInfoStr, Notice[i]);
					break;
				}
			}

#ifdef _READ16BITBMP
			if(g_bUseAlpha) StockDispBuffer( ((WINDOW_DISP *)pAISettingWnd->pYobi)->mx, ((WINDOW_DISP *)pAISettingWnd->pYobi)->my, DISP_PRIO_MENU, OLD_GRAPHICS_START + 57, 1);
			else
#endif
			StockDispBuffer( ((WINDOW_DISP *)pAISettingWnd->pYobi)->mx, ((WINDOW_DISP *)pAISettingWnd->pYobi)->my, DISP_PRIO_MENU, 35225, 1);
			btnId[0] = StockDispBuffer( x+207, y+152, DISP_PRIO_IME3, 26262+btnState[0], 2);
			btnId[1] = StockDispBuffer( x+207, y+152, DISP_PRIO_IME3, 26264+btnState[1], 2);
			for( int i = 0; i < 3; i++){
				btnId[2+i*2] = StockDispBuffer( x+u_offx[i], y+92+105, DISP_PRIO_IME3, 26064+btnState[2+i*2], 2);
				btnId[3+i*2] = StockDispBuffer( x+d_offx[i], y+107+106, DISP_PRIO_IME3, 26066+btnState[3+i*2], 2);
			}
			btnId[8] = StockDispBuffer( x+u_offx[2], y+128+105, DISP_PRIO_IME3, 26064+btnState[8], 2);
			btnId[9] = StockDispBuffer( x+d_offx[2], y+143+106, DISP_PRIO_IME3, 26066+btnState[9], 2);
			for( int i = 0; i < 2; i++){
				btnId[10+i*2] = StockDispBuffer( x+u_offx[i], y+164+105, DISP_PRIO_IME3, 26064+btnState[10+i*2], 2);
				btnId[11+i*2] = StockDispBuffer( x+d_offx[i], y+179+106, DISP_PRIO_IME3, 26066+btnState[11+i*2], 2);
			}
			for( int i = 0; i < 2; i++){
				btnId[14+i*2] = StockDispBuffer( x+u_offx[i], y+200+105, DISP_PRIO_IME3, 26064+btnState[14+i*2], 2);
				btnId[15+i*2] = StockDispBuffer( x+d_offx[i], y+215+106, DISP_PRIO_IME3, 26066+btnState[15+i*2], 2);
			}
#ifdef _ATTACK_AI
			for( int i = 0; i < 2; i++){
				btnId[18+i*2] = StockDispBuffer( x+u_offx[i], y+236+105, DISP_PRIO_IME3, 26064+btnState[18+i*2], 2);
				btnId[19+i*2] = StockDispBuffer( x+d_offx[i], y+251+106, DISP_PRIO_IME3, 26066+btnState[19+i*2], 2);
			}
			btnId[22] = StockDispBuffer( x+u_offx[0], y+128+105, DISP_PRIO_IME3, 26064+btnState[22], 2);
			btnId[23] = StockDispBuffer( x+d_offx[0], y+143+106, DISP_PRIO_IME3, 26066+btnState[23], 2);
#endif
#ifdef _AI_OTHER
			StockFontBuffer(x + 322,y + 173,FONT_PRIO_FRONT,FONT_PAL_WHITE,"其它功能",0);
#endif
			char limit[8];
			int dy = 102;
#ifndef _ATTACK_AI
			for( int i = 0; i < 4; i++, dy+=36){
#else
			for( int i = 0; i < 5; i++, dy+=36){
#endif
				str = MethodName;
#ifdef _ATTACK_AI
				if(i == 4){
					if(MAX_PROFESSION_SKILL > AI_method[i] && AI_method[i] >= 0) str = profession_skill[AI_method[i]].name;
					else if(AI_method[i] == MAX_PROFESSION_SKILL)
						str = "普通攻击";
				}
				else if(i == 1){
					if(g_bUsePS && AI_method[1] != -1) str = profession_skill[g_iPSindex].name;
					else str = "使用道具";
				}else
#endif
#ifdef _AI_NEW_FIRST
				if(i == 2){
					if(bFirstFlag){
						if(MAX_PROFESSION_SKILL > AI_method[i] && AI_method[i] >= 0) str = profession_skill[AI_method[i]].name;
					}
					else if(MAX_MAGIC > AI_method[i] && AI_method[i] >= 0) str = magic[AI_method[i]].name;
				}else
#endif
				{
					if( MAX_MAGIC>AI_method[i] && AI_method[i]>=0)
						str = magic[AI_method[i]].name;
					else if( AI_method[i] == MAX_MAGIC)
						str = "使用道具";
				}
				StockFontBuffer( x+85, y+dy, FONT_PRIO_FRONT, FONT_PAL_YELLOW, str, 0 );
				limit[0] = 0;
				if( str != MethodName){
					switch( AI_target[i]){
					case TARGET_MYSELF:
						str = "自己";
						break;
					case TARGET_MYPET:
						str = "宠物";
						break;
					case TARGET_MYSIDE:
						str = "我方全体";
						break;
					case TARGET_OTHERONE:
						str = "敌人";
						break;
					case TARGET_OTHERSIDE:
						str = "敌人全体";
						break;
					case TARGET_ALL:
						str = "全体";
						break;
					case TARGET_1:
						str = "玩家一";
						break;
					case TARGET_2:
						str = "玩家二";
						break;
					case TARGET_3:
						str = "玩家三";
						break;
					case TARGET_4:
						str = "玩家四";
						break;
					case TARGET_5:
						str = "玩家五";
						break;
#ifdef __ATTACK_MAGIC
					case TARGET_ONE_ROW:
						str = "敌一排";
						break;
#endif
					default:
						str = "错误请重设";
						break;
					}
					_itoa_s( AI_limit[i], limit, 10);
					strcat_s( limit, "％");
				}
				StockFontBuffer( x+236, y+dy, FONT_PRIO_FRONT, FONT_PAL_YELLOW, str, 0 );
				if( i < 2)
					StockFontBuffer( x+336, y+dy, FONT_PRIO_FRONT, FONT_PAL_YELLOW, limit, 0 );
			}
		}
	}
	return FALSE;
}

static int Ordinal[] = { 4, 2, 0, 1, 3 };
int GetResurrectTarget( int target_flag)
{
	int target = -1, start;
	if( TARGET_1<=target_flag && target_flag<=TARGET_5){
		start = Ordinal[target_flag-TARGET_1];
		for( int i = 0; i < 2; i++, start+=5){
			if( p_party[start]->func && p_party[start]->hp<=0){
				target = start;
				break;
			}
		}
	}
	return target;
}

int GetFirstTarget( int target_flag)
{
	int target = -1, start;
	if( TARGET_1<=target_flag && target_flag<=TARGET_5){
		start = Ordinal[target_flag-TARGET_1];
		for( int i = 0; i < 2; i++, start+=5){
			if( p_party[start]->func){
				target = start;
				break;
			}
		}
	}
	return target;
}

int GetCureTarget( int target_flag)
{
	int target = -1, start;
	if( TARGET_1<=target_flag && target_flag<=TARGET_5){
		start = Ordinal[target_flag-TARGET_1];
		for( int i = 0; i < 2; i++, start+=5){
			if( p_party[start]->func && p_party[start]->hp>0 && p_party[start]->maxHp*AI_limit[0]/100 > p_party[start]->hp){
				target = start;
				break;
			}
		}
	}
	return target;
}

extern BOOL bNewServer;
extern int GetBattelTarget();
extern int battlePlayerEscFlag;

#ifdef _ATTACK_AI
static int Ordinal2[] = {
	19,17,15,16,18,
	14,12,10,11,13,
	 9, 7, 5, 6, 8,
	 4, 2, 0, 1, 3
};

int GetDeadTarget()
{
	int i, index, end;
	i = 10;
	end = 20;
	if(BattleMyNo < 10){
		i = 0 ;
		end = 10;
	}
	for(;i<end;i++){
		index = Ordinal2[i];
		if(p_party[index]->func == NULL ) continue;
		if(p_party[index]->hp > 0) continue;
		return index;
	}
	return -1;
}

int AttackAIProc(int which,char *moji)
{
	int targetNo;
	int method = AI_method[which];
	int no;

	if(method != MAX_PROFESSION_SKILL){
		if(profession_skill[method].costmp <= p_party[BattleMyNo]->mp){
			targetNo = AI_target[which];
			switch(profession_skill[method].target){
			case PETSKILL_TARGET_NONE:
			case PETSKILL_TARGET_MYSELF:
				sprintf_s( moji,64,"P|%X|%X", method,BattleMyNo);
				break;
			case PETSKILL_TARGER_DEATH:
				targetNo = GetDeadTarget();
				if(targetNo != -1) sprintf_s(moji,64,"P|%X|%X",method,targetNo);
				else{
					targetNo = GetBattelTarget();
					sprintf_s(moji,64,"H|%X",targetNo);
				}
				break;
			case PETSKILL_TARGET_OTHER:
			case PETSKILL_TARGET_OTHERWITHOUTMYSELF:
			case PETSKILL_TARGET_WITHOUTMYSELFANDPET:
			case PETSKILL_TARGET_ONE_LINE:
				if(targetNo == TARGET_MYSELF)	targetNo = BattleMyNo;
				else if(targetNo == TARGET_MYPET) targetNo = BattleMyNo + 5;
				else if(targetNo < TARGET_OTHERONE || targetNo > TARGET_ALL || 
					targetNo == TARGET_ONE_ROW) targetNo = GetFirstTarget(targetNo);
				else targetNo = GetBattelTarget();
				sprintf_s( moji,64,"P|%X|%X", method,targetNo);
				break;
			case PETSKILL_TARGET_ALLMYSIDE:
				if( BattleMyNo < 10 ) no = 20;
				else no = 21;
				sprintf_s( moji,64,"P|%X|%X", method, no );
				break;
			case PETSKILL_TARGET_ALLOTHERSIDE:
				if( BattleMyNo < 10 ) no = 21;
				else no = 20;
				sprintf_s( moji,64,"P|%X|%X", method, no );
				break;
			case PETSKILL_TARGET_ALL:
				sprintf_s( moji,64,"P|%X|%X", method, 22 );
				break;
			case PETSKILL_TARGET_ONE_ROW:
#ifdef _SKILL_ADDBARRIER
			case PETSKILL_TARGET_ONE_ROW_ALL:
#endif
				targetNo = GetBattelTarget();
				if ( targetNo >= 0 && targetNo < 5 ) no = 26 ; 
				if ( targetNo >= 5 && targetNo < 10 ) no = 25 ; 
				if ( targetNo >= 10 && targetNo < 15 ) no = 23 ; 
				if ( targetNo >= 15 && targetNo < 20 ) no = 24 ; 
				sprintf_s( moji,64,"P|%X|%X", method, no );
				break;
			}
			AI_Choosen = profession_skill[method].name;
			return 1;
		}
		return 0;
	}
	return -1;
}
#endif

void AI_ChooseAction()
{
	char *str;
	BOOL done = 0;
	int targetNo = -1;
	char moji[64];
	battlePlayerEscFlag = FALSE;
	//观战中
	if( BattleMyNo >= 20 ){
		if( bNewServer)
			lssproto_B_send( sockfd, "N" );
		else
			old_lssproto_B_send( sockfd, "N" );
		extern void InitBattleAnimFlag();
		InitBattleAnimFlag();
		SubProcNo++;
		return;
	}
	if( p_party[ BattleMyNo ]->hp <= 0 ||
		BattleBpFlag & BATTLE_BP_PLAYER_MENU_NON ){
		if( BattleBpFlag & BATTLE_BP_PLAYER_MENU_NON ){
			if( bNewServer)
				lssproto_B_send( sockfd, "N" );
			else
				old_lssproto_B_send( sockfd, "N" );
		}
	}else{
		if( AI_First){//第一次
			AI_First = 0;
			int method = AI_method[2];
#ifdef _AI_NEW_FIRST
			if(bFirstFlag == false){
#endif
				if( method>-1 && method<MAX_MAGIC && magic[method].useFlag){
					BOOL mp_enough = TRUE;
					if( pc.familySprite == 0){
						if( (magic[method].mp*80/100) > p_party[BattleMyNo]->mp )
							mp_enough = FALSE;
					}else{
						if( magic[method].mp > p_party[BattleMyNo]->mp )
							mp_enough = FALSE;
					}
					if( mp_enough){
						switch( AI_target[2]){
						case TARGET_MYSELF:
							targetNo = BattleMyNo;
							break;
						case TARGET_MYPET:
							if( p_party[BattleMyNo+5]->func)
								targetNo = BattleMyNo + 5;
							break;
						case TARGET_MYSIDE:
							switch( magic[method].target){
							case MAGIC_TARGET_WHOLEOTHERSIDE:
								if( BattleMyNo < 10 ) targetNo = 20;
								else targetNo = 21;
								break;
							case MAGIC_TARGET_ALLMYSIDE:
								if( BattleMyNo < 10 ) targetNo = 20;
								else targetNo = 21;
								break;
							}
							break;
							case TARGET_OTHERONE:
								targetNo = GetBattelTarget();
								break;
							case TARGET_OTHERSIDE:
								switch( magic[method].target){
#ifdef __ATTACK_MAGIC
								case MAGIC_TARGET_ALL_ROWS:
#endif
								case MAGIC_TARGET_WHOLEOTHERSIDE:
								case MAGIC_TARGET_ALLOTHERSIDE:
									if( BattleMyNo < 10 ) targetNo = 21;
									else targetNo = 20;
									break;
								}
								break;
								case TARGET_ALL:
									targetNo = 22;
									break;
								case TARGET_1:
								case TARGET_2:
								case TARGET_3:
								case TARGET_4:
								case TARGET_5:
									targetNo = GetFirstTarget( AI_target[2]);
									break;
#ifdef __ATTACK_MAGIC
									// 23: 左上第一列 , 24: 左上第二列 , 25: 右下第一列 , 26: 右下第二列
								case TARGET_ONE_ROW:
									targetNo = GetBattelTarget();
									if( targetNo < 5)
										targetNo = BATTLKPKPLYAERNUM + 5;
									else if( targetNo < 10)
										targetNo = BATTLKPKPLYAERNUM + 6;
									else if( targetNo < 15)
										targetNo = BATTLKPKPLYAERNUM + 3;
									else
										targetNo = BATTLKPKPLYAERNUM + 4;
									break;
#endif
								default:
									targetNo = -1;
									break;
						}
						if( targetNo > -1){
							AI_Choosen = "使用首次精灵";
							sprintf_s( moji, "J|%X|%X", method, targetNo);
							done = 1;
						}
					}else
						StockChatBufferLine( "MP不足无法使用首次的精灵", FONT_PAL_WHITE);
				}
#ifdef _AI_NEW_FIRST
			}
			else{
				int ret;
				ret = AttackAIProc(2,moji);
				if(ret) done = 1;
				else if(ret == 0) StockChatBufferLine("MP不足无法使用首次的技能", FONT_PAL_WHITE);
				else if(ret == -1) StockChatBufferLine("技能代号错误", FONT_PAL_WHITE);
			}
#endif
		}
		if( 0 == done){
			int magic_id = -1, item_id = -1, target = -1, meat = -1;
			//先找出是否有耐久力回复的道具
			for( int i = 0; i < MAX_ITEM; i++){
				if( pc.item[i].useFlag && (str=strstr( pc.item[i].memo, "耐久力")) && strstr( str, "回复")){
					item_id = i;
					if( strstr( pc.item[i].name, "肉")){
						meat = i;
						break;
					}
				}
			}
			//先找出是否有耐久力回复的精灵和mp是否足够
			int method = AI_method[0];
			if( (method>-1 && method<MAX_MAGIC) && magic[method].useFlag && (str=strstr( magic[method].memo, "耐久力")) && strstr( str, "回复")){
				if( pc.familySprite == 0){
					if( (magic[method].mp*80/100) <= p_party[BattleMyNo]->mp )
						magic_id = method;
				}else{
					if( magic[method].mp <= p_party[BattleMyNo]->mp )
						magic_id = method;
				}
			}
			//找补耐久力的目标
			if( TARGET_1<=AI_target[0] && AI_target[0]<=TARGET_5){
				target = GetCureTarget( AI_target[0]);
			}
			if( target == -1){
				if( AI_target[0] == TARGET_MYSELF){//先补自己血
					if( p_party[BattleMyNo]->maxHp*AI_limit[0]/100 > p_party[BattleMyNo]->hp){
						target = BattleMyNo;
					}else if( p_party[BattleMyNo+5]->func && pet[pc.battlePetNo].hp>0 && pet[pc.battlePetNo].maxHp*AI_limit[0]/100>pet[pc.battlePetNo].hp){
						target = BattleMyNo + 5;
					}
				}else{// if( AI_target[0] == TARGET_MYPET){//先补宠物血
					if( p_party[BattleMyNo+5]->func && pet[pc.battlePetNo].hp>0 && pet[pc.battlePetNo].maxHp*AI_limit[0]/100 > pet[pc.battlePetNo].hp){
						target = BattleMyNo + 5;
					}else if( p_party[BattleMyNo]->maxHp*AI_limit[0]/100>p_party[BattleMyNo]->hp){
						target = BattleMyNo;
					}
				}
			}
			if( target > -1){
				if( item_id>-1 || magic_id>-1){
					if( meat > -1){//使用肉补耐久力
						done = 1;
						sprintf_s( moji,"I|%X|%X", meat, target);
						AI_Choosen = "使用肉回复耐久力";
					}else{
						if( MAX_MAGIC>AI_method[0] && AI_method[0]>=0){//使用精灵补耐久力
							if( magic[AI_method[0]].target != MAGIC_TARGET_MYSELF || target==BattleMyNo){
								if( magic_id > -1){
									int method = AI_method[0];
									if( MAGIC_TARGET_WHOLEOTHERSIDE == magic[method].target){
										if( BattleMyNo < 10 ) target = BATTLKPKPLYAERNUM;
										else target = BATTLKPKPLYAERNUM + 1;
									}
									done = 1;
									AI_Choosen = "使用耐久力回复精灵";
									sprintf_s( moji,"J|%X|%X", magic_id, target);
								}else{
									StockChatBufferLine( "无法使用耐久力回复的精灵", FONT_PAL_WHITE);
								}
							}
						}
						if( !done){//使用道具补耐久力
							if( item_id > -1){
								done = 1;
								sprintf_s( moji,"I|%X|%X", item_id, target);
								AI_Choosen = "使用耐久力回复道具";
							}else{
								StockChatBufferLine( "没有耐久力回复的道具", FONT_PAL_WHITE);
							}
						}
					}
				}else
					StockChatBufferLine( "无法回复耐久力了", FONT_PAL_WHITE);
			}
		}
		if( 0==done){
			int start = 0, end = 10, target = -1;
			switch( AI_target[3]){
			case TARGET_MYSIDE:
				if( p_party[BattleMyNo+5]->func && pet[pc.battlePetNo].hp<=0){
					target = BattleMyNo+5;
				}else{
					if( BattleMyNo >= 10){
						start = 10;
						end = 20;
					}
					for( ; start < end; start++){
						if( BattleMyNo==start || (BattleMyNo+5)==start)
							continue;
						if( p_party[start]->func == NULL)
							continue;
						if( p_party[start]->hp>0)
							continue;
						target = start;
						break;
					}
				}
				break;
			case TARGET_MYPET:
				if( p_party[BattleMyNo+5]->func && pet[pc.battlePetNo].hp<=0)
					target = BattleMyNo+5;
				break;
			case TARGET_1:
			case TARGET_2:
			case TARGET_3:
			case TARGET_4:
			case TARGET_5:
				target = GetResurrectTarget( AI_target[3]);
				break;
			}
			if( target > -1){//宠物死了
				if( MAX_MAGIC == AI_method[3]){
					int i;
					for( i = 0; i < MAX_ITEM; i++){
						if( pc.item[i].useFlag && strstr( pc.item[i].memo, "气绝回复")){
							AI_Choosen = "使用气绝回复道具";
							done = 1;
							sprintf_s( moji,"I|%X|%X", i, target);
							break;
						}
					}
					if( i >= MAX_ITEM)
						StockChatBufferLine( "没有气绝回复的道具", FONT_PAL_WHITE);
				}else{
					int method = AI_method[3];
					if( method>-1 && magic[method].useFlag && (str=strstr( magic[method].memo, "气绝状态")) && strstr( str, "回复")){
						if( pc.familySprite == 0){
							if( (magic[method].mp*80/100) <= p_party[BattleMyNo]->mp )
								done = 1;
						}else{
							if( magic[method].mp <= p_party[BattleMyNo]->mp )
								done = 1;
						}
						if( done == 1){
							AI_Choosen = "使用气绝回复精灵";
							sprintf_s( moji,"J|%X|%X", method, target);
						}
						else
							StockChatBufferLine( "MP不足无法使用气绝回复的精灵", FONT_PAL_WHITE);
					}
					else
						StockChatBufferLine( "没有设定气绝回复的精灵", FONT_PAL_WHITE);
				}
			}
		}
#ifndef _ATTACK_AI
		if( 0==done && (/*p_party[BattleMyNo]->maxMp*/100*AI_limit[1]/100 > p_party[BattleMyNo]->mp) ){//补气
#else
		if(0 == done && ((p_party[BattleMyNo]->maxMp * AI_limit[1] / 100) > p_party[BattleMyNo]->mp) ){//补气
			if(g_bUsePS && AI_method[1] != -1){
				sprintf_s( moji,"P|%X|%X",g_iPSindex,BattleMyNo);
				done = 1;
				AI_Choosen = profession_skill[g_iPSindex].name;
			}
			else
#endif
			{
				int i;
				for( i = 0; i < MAX_ITEM; i++){
					if( pc.item[i].useFlag && (str=strstr( pc.item[i].memo, "气")) && strstr( str, "回复")){
						AI_Choosen = "使用回复气力道具";
						done = 1;
						sprintf_s( moji,"I|%X|%X", i, BattleMyNo);
						break;
					}
				}
				/*
				if( i >= MAX_ITEM){
#ifdef _STONDEBUG_
					extern STR_BUFFER MyChatBuffer;
					sprintf_s(MyChatBuffer.buffer,"[additem 6 2400]");
					MyChatBuffer.cnt = strlen(MyChatBuffer.buffer);
					KeyboardReturn();
#endif
					StockChatBufferLine( "没有回复气力的道具", FONT_PAL_WHITE);
				}
				*/
			}
		}
		if( 0 == done){
//			if( BattleBpFlag & BATTLE_BP_BOOMERANG ){
//			}
			//如自己或宠物没有血 且 没有气 就逃跑
			if( ( (p_party[BattleMyNo]->maxHp*AI_limit[0]/100 > p_party[BattleMyNo]->hp) ||
				(p_party[BattleMyNo+5]->func &&		 
				( (pet[pc.battlePetNo].hp>0 && pet[pc.battlePetNo].maxHp*AI_limit[0]/100>pet[pc.battlePetNo].hp) ||
				pet[pc.battlePetNo].hp<=0) ) ) ){
				int mp_enough = FALSE;
				if( 0<=AI_method[0] && AI_method[0]<MAX_MAGIC){//有设定使用补血精灵
					mp_enough = TRUE;//记录MP是否足够
					if( pc.familySprite == 0){
						if( (magic[AI_method[0]].mp*80/100) > p_party[BattleMyNo]->mp )
							mp_enough = FALSE;
					}else{
						if( magic[AI_method[0]].mp > p_party[BattleMyNo]->mp )
							mp_enough = FALSE;
					}
				}
				if( mp_enough == FALSE){//没有设定使用补血精灵 或 MP不足
					battlePlayerEscFlag = TRUE;
					strcpy( moji, "E");
					done = 1;
				}
			}
			if( !done){
				int targetNo;
#ifdef _ATTACK_AI
				if(AttackAIProc(4,moji) <= 0)
#endif
				{
#ifdef _AI_CAPTURE
				extern int nowFloor;
				extern int nowGx, nowGy;
				if (AI_OtherSetting[1] && (nowFloor!=500||nowGx>=250&&nowGy>=360)){
					targetNo = GetBattelTarget();
					if (p_party[targetNo]->level==1){
						sprintf_s( moji, "T|%X", targetNo );
						AI_Choosen = "自动抓宠";
					}else{
					AI_Choosen = "抓宠逃跑";
					battlePlayerEscFlag = TRUE;
					strcpy( moji, "E");
					done = 1;
					}
				}else{
#endif
					targetNo = GetBattelTarget();
					sprintf_s( moji, "H|%X", targetNo );
					AI_Choosen = "自动攻击";
#ifdef _AI_CAPTURE
				}
#endif
				}

			}
		}
		if( bNewServer)
			lssproto_B_send( sockfd, moji );
		else
			old_lssproto_B_send( sockfd, moji );
	}
	done = 0;
	strcpy( moji, "W|FF|FF");
	if( pc.battlePetNo>-1 && p_party[BattleMyNo+5]->hp>0 ){
		if( BattleBpFlag & BATTLE_BP_PET_MENU_NON || p_party[ BattleMyNo ]->hp <= 0 || battlePlayerEscFlag == TRUE){
		}else{
			int skill_id = -1;
			for( int i = 0; i < pet[pc.battlePetNo].maxSkill; i++){
				if( petSkill[pc.battlePetNo][i].useFlag == TRUE && 
					petSkill[pc.battlePetNo][i].field != PETSKILL_FIELD_MAP){
					if( skill_id == -1)
						skill_id = i;
					if( petSkill[pc.battlePetNo][i].skillId!=2 && petSkill[pc.battlePetNo][i].skillId!=0)
						skill_id = i;
					if( petSkill[pc.battlePetNo][i].skillId == 1){
						skill_id = i;
						break;
					}
				}
			}
			if( skill_id > -1){
				switch( petSkill[pc.battlePetNo][skill_id].target){
				case PETSKILL_TARGET_MYSELF:
					targetNo = BattleMyNo + 5;
					break;
				case PETSKILL_TARGET_OTHER:
				case PETSKILL_TARGET_OTHERWITHOUTMYSELF:
					targetNo = GetBattelTarget();
					break;
				case PETSKILL_TARGET_ALLMYSIDE:
					if( BattleMyNo < 10 ) targetNo = 20;
					else targetNo = 21;
					break;
				case PETSKILL_TARGET_ALLOTHERSIDE:
					if( BattleMyNo < 10 ) targetNo = 21;
					else targetNo = 20;
					break;
				case PETSKILL_TARGET_ALL:
					targetNo = 22;
					break;
				}
				sprintf_s( moji, "W|%X|%X", skill_id, targetNo);
			}
		}
	}
	if( bNewServer)
		lssproto_B_send( sockfd, moji);
	else
		old_lssproto_B_send( sockfd, moji);
	for( int i = 0; i < BATTLKPKPLYAERNUM; i++){
		if( 0 == (p_party[i]->atr & p_party[i]->atr) )
			p_party[i]->atr = ACT_ATR_INFO | ACT_ATR_HIT;
	}
	p_party[ BattleMyNo ]->atr |= ACT_ATR_BTL_CMD_END;
	p_party[ BattleMyNo + 5 ]->atr |= ACT_ATR_BTL_CMD_END;
	SubProcNo++;
}

void AI_CloseWnd()
{
#ifdef _AIDENGLU_
	extern BOOL 自动登陆窗口;
	extern ACTION *p自动登陆窗口;
	extern ACTION *方向动作;
	extern ACTION *人物动作;
	自动登陆窗口=0;
	if(p自动登陆窗口){
		DeathAction(p自动登陆窗口);
		p自动登陆窗口=NULL;
	}
	if((int)方向动作){
		if((int)方向动作>1000)
			DeathAction(方向动作);
		方向动作=NULL;
		if((int)人物动作>1000)
			DeathAction(人物动作);
		人物动作=NULL;	
	}
#endif
	AI_State = 0;
	if( pAISettingWnd){
		DeathAction( pAISettingWnd );
		pAISettingWnd = NULL;
	}
#ifdef _AI_OTHER
	if(pAIOtherFunctionWnd){
		DeathAction(pAIOtherFunctionWnd);
		pAIOtherFunctionWnd = NULL;
		AI_Other_State = 0;
	}
#endif
}

#ifdef _AI_OTHER
#ifdef _AI_CAPTURE
#define AI_OTHER_BTN 3
#else 
#define AI_OTHER_BTN 2
#endif
void AI_OtherProc(){
	static int btnId[AI_OTHER_BTN];
	int x,y,i;
#ifdef _AI_CAPTURE
	static char szFunctionName[AI_OTHER_BTN][30] = {"","自动丢肉","自动抓宠"};

#else 
	static char szFunctionName[AI_OTHER_BTN][30] = {"","自动丢肉"};
#endif
	static char szFunctionSwitch[2][12] = {" Ｎ  Ｏ "," ＹＥＳ "};
	if(pAIOtherFunctionWnd == NULL){
		// 若在战斗中
		pAIOtherFunctionWnd = MakeWindowDisp(413,lpDraw->ySize / 2 + 150,3,2,NULL,1, FALSE);
		//if(ProcNo == PROC_BATTLE) pAIOtherFunctionWnd = MakeWindowDisp(413,150,3,2,NULL,1, FALSE);
		//else pAIOtherFunctionWnd = MakeWindowDisp(413,0,3,2,NULL,1, FALSE);
	}
	else{
		if(pAIOtherFunctionWnd->hp >= 1 ){
			if(joy_trg[0] & JOY_ESC){
				DeathAction(pAIOtherFunctionWnd);
				pAIOtherFunctionWnd = NULL;
				AI_Other_State = 0;
				return;
			}
			x = pAIOtherFunctionWnd->x;
			y = pAIOtherFunctionWnd->y;
			for(i=1;i<AI_OTHER_BTN;i++){
				StockFontBuffer(x+25,y+i*20,FONT_PRIO_FRONT,FONT_PAL_WHITE,szFunctionName[i],0);
				btnId[i] = StockFontBuffer(x+100,y+i*20,FONT_PRIO_FRONT,FONT_PAL_WHITE,
																	 AI_OtherSetting[i-1]?szFunctionSwitch[1]:szFunctionSwitch[0],2);
			}
			btnId[0] = StockDispBuffer(x+100,y+(i+1)*20,DISP_PRIO_IME3,26042,2);

			if(mouse.onceState & MOUSE_LEFT_CRICK){
				int id;
				id = selFontId(btnId,sizeof(btnId)/sizeof(int));
				switch(id){
				// 自动丢肉
				case 1:
					AI_OtherSetting[0] = !AI_OtherSetting[0];
					break;
				case 2:
					AI_OtherSetting[1] = !AI_OtherSetting[1];
					break;
				}
				// 关闭
				if(HitDispNo == btnId[0]){
					DeathAction(pAIOtherFunctionWnd);
					pAIOtherFunctionWnd = NULL;
					AI_Other_State = 0;
					AI_Save();
				}
			}
		}
	}
}
#endif



