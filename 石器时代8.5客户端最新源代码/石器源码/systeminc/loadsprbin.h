#ifndef _SPRITE_H_
#define _SPRITE_H_

//????????????????
#define mxSPRITE	40000

#define maxBUFFER	1024*1024*6



typedef unsigned char MOJI;
typedef unsigned char U1;
typedef          char S1;
typedef unsigned short U2;
typedef          short S2;
typedef unsigned long U4;
typedef          long S4;
typedef float  F4;
typedef double F8;

typedef enum {
	ANIM_ATTACK, //攻击
	ANIM_DAMAGE, //受伤
	ANIM_DEAD,  //死亡
	ANIM_STAND, //站立
	ANIM_WALK, //行走
	ANIM_SIT, //坐下
	ANIM_HAND, //挥手
	ANIM_HAPPY, //高兴
	ANIM_ANGRY, //生气
	ANIM_SAD, //伤心
	ANIM_GUARD, //防守
	ANIM_NOD, //点头
	ANIM_THROW, //投掷
	ANIM_LIST_
} ANIM_LIST;


// 1?????
typedef struct
{
	U4 BmpNo;				//Adrn图片编号
	S2 PosX,PosY;			//图片显示时的x，y偏移
	U2 SoundNo;				// 小于10000代表音效编号,表示要放音效;大于等于10000小于10100代表要显示伤害效果,数字或其他效果
							// 大于等于10100代表连击?????
} FRAMELIST;//一帧动作的具体信息

typedef struct
{
	U2 dir;	// 动作方向(0-8)
	U2 no;	// 动作的分类见后面的ANIM_LIST的定义
	U4 dtAnim;	// 次动作循环的耗时
	U4 frameCnt; // FRAMELIST的数量
	FRAMELIST *ptFramelist;	
} ANIMLIST;


// SPR??
typedef struct
{
	U4 sprNo;	// Spr序号
	U4 offset;	// 指明在spr.bin中的地址
	U2 animSize;	// 脚色有多少个完整的动作
} SPRADRN;


typedef struct
{
	U2 animSize;
	ANIMLIST	*ptAnimlist;
} SPRITEDATA;


typedef struct {
	U2			dtz;
	U2			noSpritedata;
	U2			mdAnim;
	U2			mdAnim2;
	U2			dir;
	U2			ctLoop;
	float		ctAnim;
	float		dtAnim;
} SPRITE;


typedef struct
{
	U2 dir;		// 动作方向(0-8)
	U2 no;		// 动作的分类见后面的ANIM_LIST的定义
	U4 dtAnim;	// 一次动作循环的耗时
	U4 frameCnt;	// 动作桢的数量
} ANIM_HEADER;


extern SPRITEDATA SpriteData[mxSPRITE];


BOOL InitSprBinFileOpen( char *, char * );


#endif
