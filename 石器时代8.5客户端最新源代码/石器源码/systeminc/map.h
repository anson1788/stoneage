#ifndef _MAP_H_ 
#define _MAP_H_

// ?????????????????????

#define MAP_TILE_GRID_X1	-20
#define MAP_TILE_GRID_X2	+17		// ?????
#define MAP_TILE_GRID_Y1	-16
#define MAP_TILE_GRID_Y2	+21		// ?????

// ??????
#define MAP_X_SIZE	(MAP_TILE_GRID_X2 - MAP_TILE_GRID_X1)
#define MAP_Y_SIZE	(MAP_TILE_GRID_Y2 - MAP_TILE_GRID_Y1)

// ????????
#define GRID_SIZE		64

// ?
#define MOVE_SPEED			4
#define MOVE_FRAME			(GRID_SIZE/MOVE_SPEED)

// ?????
enum
{
	EVENT_NONE,			// ????????
	EVENT_NPC,			// ?NPC
	EVENT_ENEMY,		// ???????
	EVENT_WARP,			// ???
	EVENT_DOOR,			// ??
	EVENT_ALTERRATIVE,	// ?????????????????????
	EVENT_WARP_MONING,	// ????
	EVENT_WARP_NOON,	// ?????
	EVENT_WARP_NIGHT,	// ?????

	EVENT_END		// ????????
};

// ???????
#define MAP_READ_FLAG	0x8000		// ???????????????????
#define MAP_SEE_FLAG	0x4000		// ????????????????

// ?????
#define FLOOR_NAME_LEN	24

extern char nowFloorName[];
extern int nowFloor;
extern int nowFloorGxSize, nowFloorGySize;
extern int nowGx, nowGy;
extern float nowX, nowY;
extern float nowVx, nowVy, nowSpdRate;
extern int nextGx, nextGy;
extern int oldGx, oldGy;
extern int oldNextGx, oldNextGy;
extern int mouseMapGx, mouseMapGy;
extern int mouseMapX, mouseMapY;

extern short moveRouteCnt;

extern BOOL mapEmptyFlag;

extern float viewPointX;
extern float viewPointY;
extern int viewOffsetX;
extern int viewOffsetY;

extern short moveAddTbl[8][2];


extern BOOL mouseLeftCrick;
extern BOOL mouseLeftOn;
extern BOOL mouseRightCrick;
extern BOOL mouseRightOn;
#ifdef _MOUSE_DBL_CLICK
extern BOOL mouseDblRightOn;
#endif
extern unsigned int mouseLeftPushTime;
extern unsigned int beforeMouseLeftPushTime;


extern short nowEncountPercentage;
extern short nowEncountExtra;
extern short minEncountPercentage;
extern short maxEncountPercentage;
extern short sendEnFlag;
extern short encountNowFlag;

extern short eventWarpSendFlag;
extern short eventWarpSendId;
extern short eventEnemySendFlag;
extern short eventEnemySendId;
extern short eventEnemyFlag;

extern BOOL loginFlag;

extern short vsLookFlag;

extern BOOL floorChangeFlag;

extern BOOL warpEffectFlag;
extern BOOL warpEffectStart;
extern BOOL warpEffectOk;

extern int transmigrationEffectFlag;
extern int transEffectPaletteStatus;

void initMap( void );
void resetMap( void );
void createMap( int, int, int );
BOOL writeMap( int, int, int, int, int,
	unsigned short *, unsigned short *, unsigned short * );
void setMap( int, int, int );
void setWarpMap( int, int );
BOOL mapCheckSum( int, int, int, int, int, int, int, int );

#ifdef _NEWDRAWBATTLEMAP		   // (不可开放) Syu ADD 自动产生BattleMap
void ddrawBattleMap( void );
#endif
void drawMap( void );
void drawMap2( void );
void drawTile( void );
void redrawMap( void );
void drawGrid( void );
void moveProc( void );
BOOL checkEmptyMap( int );
void setMapMovePoint( int, int );
void setMapMovePoint2( int, int );
void mapMove2( void );
void updateMapArea( void );

void getRouteData( int, int *, int * );

char cnvServDir( int, int );

void goFrontPartyCharacter( int, int, int );


void initCharPartsPrio( void );
void setPartsPrio( int, int, int, int, int, float, float, int);
#ifdef _SFUMATO
void setCharPrio( int, int, int, int, int, float, float, int );
#else
void setCharPrio( int, int, int, int, int, float, float);
#endif
void stockCharParts( void );


#if 0
BOOL checkNpcEvent( int, int, int, int );
#endif

void drawAutoMap( int x, int y );
void readAutoMapSeeFlag( void );
void writeAutoMapSeeFlag( void );

BOOL checkHitMap( int, int );

void camMapToGamen( float, float, float *, float * );
void camGamenToMap( float, float, float *, float * );


#if 1

enum
{
	MAP_EFFECT_TYPE_NONE,
	MAP_EFFECT_TYPE_RAIN,
	MAP_EFFECT_TYPE_SNOW,
	MAP_EFFECT_TYPE_STAR,
	MAP_EFFECT_TYPE_KAMIFUBUKI,
	MAP_EFFECT_TYPE_HOTARU
#ifdef _HALLOWEEN_EFFECT
	,MAP_EFFECT_TYPE_HALLOWEEN
#endif
};

typedef struct _MAP_EFFECT
{
	int x, y;
	int dx, dy;
	int gx, gy;
	int ggx, ggy;
	int ex, ey;
	int w, h;
	int cnt;
	_MAP_EFFECT *pre;
	_MAP_EFFECT *next;
	short type;
	short type2;
	short mode;
#ifdef _HALLOWEEN_EFFECT
	ACTION *pAction;
#endif
} MAP_EFFECT;

extern MAP_EFFECT *useBufMapEffect;

extern short mapEffectRainLevel;
extern short oldMapEffectRainLevel;
extern short mapEffectSnowLevel;
extern short oldMapEffectSnowLevel;
extern short mapEffectMoveDir;
extern short mapEffectKamiFubukiLevel;
#ifdef _HALLOWEEN_EFFECT
extern int mapEffectHalloween;
#endif
#ifdef __EDEN_DICE
extern BOOL bMapEffectDice;
#endif


extern const short fastDrawTile;
extern short fastDrawTileFlag;
extern int amountXFastDraw, amountYFastDraw;
extern int nowXFastDraw, nowYFastDraw;
extern int baseXFastDraw, baseYFastDraw;
extern int amountXFastDraw2, amountYFastDraw2;
extern int nowXFastDraw2, nowYFastDraw2;
extern int baseXFastDraw2, baseYFastDraw2;
#ifdef _MOVE_SCREEN
extern int iScreenMoveX, iScreenMoveY;
#endif


void initMapEffect( BOOL bFirstRun );
void drawMapEffect( void );
void mapEffectProc( void );
void mapEffectRain( void );
void mapEffectSnow( void );
void mapEffectStar( void );
void mapEffectFallingStar( void );
void mapEffectKamiFubuki( void );
void mapEffectDice(void);

#ifdef _HALLOWEEN_EFFECT
void mapEffectHalloweenProc(void);
#endif

void mapEffectProc2( int );
void mapEffectRain2( int );
void mapEffectSnow2( int );

void _checkEmptyMap( void );

#endif

#ifdef _ITEM_PATH
	extern BOOL ITEMPATHFLAG;
#endif

#endif
