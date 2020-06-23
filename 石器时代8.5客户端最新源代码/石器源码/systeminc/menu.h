/************************/
/*	menu.h				*/
/************************/
#ifndef _MENU_H_
#define _MENU_H_

#include "pc.h"
#include "chat.h"

#ifdef _TELLCHANNEL				//ROG ADD 躇逄耋
#include "netproc.h"
#endif

#ifdef _NEW_SYSTEM_MENU
#define MENU_SYSTEM_0	28
#else
#define MENU_SYSTEM_0	9
#endif
#define MENU_MAP_0		3

#define MAIL_STR_LEN 140		// ???更????
#define MAIL_DATE_STR_LEN 20	// 吋至?更?????
#define MAIL_MAX_HISTORY 20		// ?????
#define MAX_CHAT_REGISTY_STR		8		// ?????更?仕???

// ??????????﹨▍
typedef struct{
	int 	x, y;			// 牙?火
	int 	defX, defY;		// ??????火
	int 	bmpNo;			// ??吻?
	int 	dispPrio;		// 牙?！
	BOOL	dragFlag;		// ???????
	BOOL	mixFlag;		// ????
}ITEM_BUFFER;

// ???????﹨▍
typedef struct{
	char 	str[ MAIL_MAX_HISTORY ][ MAIL_STR_LEN + 1 ];			// ??????更??
	char 	dateStr[ MAIL_MAX_HISTORY ][ MAIL_DATE_STR_LEN + 1 ];	// 吋至?更??
	int 	noReadFlag[ MAIL_MAX_HISTORY ];							// ???????????????吻??
	int 	petLevel[ MAIL_MAX_HISTORY ];							// ??????????吻?
	char 	petName[ MAIL_MAX_HISTORY ][ PET_NAME_LEN + 1 ];		// ?????〈
	int 	itemGraNo[ MAIL_MAX_HISTORY ];							// ??????????????吻?
	int 	newHistoryNo;											// ????吻?
}MAIL_HISTORY;

// ?????牙????????﹨▍
typedef struct{
	int sizeX,sizeY;	// ???
	int titleNo;		// ????吻?
	int titleX, titleY;	// ??????火
	int mx, my;			// ??火
	int nowX, nowY;		// ???????
	int cnt;			// ??火
	int wndType;		// ???????
	int hitFlag;		// ???刪叉????
	int yesDispNo;		// ??????刪叉吻?
	int noDispNo;		// ???????刪叉吻?
	int boxDispPrio;	// ???????牙?！??
	int yesNoResult;	// YesNo?????ㄩ???  ?ㄩ???
}WINDOW_DISP;

#if defined(__ALBUM_47)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND_39 540 
#define MAX_PET_KIND_40 542
#define MAX_PET_KIND_41 543
#define MAX_PET_KIND_42 545
#define MAX_PET_KIND_43 549
#define MAX_PET_KIND_44 551
#define MAX_PET_KIND_45 553
#define MAX_PET_KIND_46 555
//#define MAX_PET_KIND    557
//#define MAX_PET_KIND    558 // Robin fix
//#define MAX_PET_KIND    564 // Robin fix
//#define MAX_PET_KIND    566 // Robin fix
#define MAX_PET_KIND    570 // Robin fix

#elif defined(__ALBUM_46)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND_39 540 
#define MAX_PET_KIND_40 542
#define MAX_PET_KIND_41 543
#define MAX_PET_KIND_42 545
#define MAX_PET_KIND_43 549
#define MAX_PET_KIND_44 551
#define MAX_PET_KIND_45 553
#define MAX_PET_KIND    555

#elif defined(__ALBUM_45)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND_39 540 
#define MAX_PET_KIND_40 542
#define MAX_PET_KIND_41 543
#define MAX_PET_KIND_42 545
#define MAX_PET_KIND_43 549
#define MAX_PET_KIND_44 551
#define MAX_PET_KIND    553

#elif defined(__ALBUM_44)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND_39 540 
#define MAX_PET_KIND_40 542
#define MAX_PET_KIND_41 543
#define MAX_PET_KIND_42 545
#define MAX_PET_KIND_43 549
#define MAX_PET_KIND    551

#elif defined(__ALBUM_43)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND_39 540 
#define MAX_PET_KIND_40 542
#define MAX_PET_KIND_41 543
#define MAX_PET_KIND_42 545
#define MAX_PET_KIND    549

#elif defined(__ALBUM_42)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND_39 540 
#define MAX_PET_KIND_40 542
#define MAX_PET_KIND_41 543
#define MAX_PET_KIND    545

#elif defined(__ALBUM_41)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND_39 540 
#define MAX_PET_KIND_40 542
#define MAX_PET_KIND    543

#elif defined(__ALBUM_40)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND_39 540 
#define MAX_PET_KIND    542

#elif defined(__ALBUM_39)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND    540

#elif defined(__ALBUM_38)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND    521 

#elif defined(__ALBUM_37)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND    496 

#elif defined(__ALBUM_36)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND    483 
#elif defined(__ALBUM_35)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND    462 //460

#elif defined(__ALBUM_34)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND    436

#elif defined(__ALBUM_33)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND    432

#elif defined(__ALBUM_32)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND    431

#elif defined(__ALBUM_15)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND    357
#else
#define MAX_PET_KIND 	10000		// ?????
#define MAX_PET_TBL 	181		// ???????????
#endif

#define MAX_PET_PAGE_SIZE 	8	// ?????牙????????
#ifdef _PET_ITEM
#define MAX_PET_SPECIES		42	// 唾昜昜笱杅講
#endif

// ????????????去吻?
enum{
	ANIM_DISP_PET,				// ???牙
	ANIM_DISP_PET_SEND,			// ??????????????去
	ANIM_DISP_PET_RECIEVE,		// ?????????????去
	
	ANIM_DISP_PET_MAIN,			// ?????去︻???
	ANIM_DISP_PET_WALK,			// 卷?
	ANIM_DISP_PET_STAND,		// ┴?
	ANIM_DISP_PET_ATTACK,		// ??
	ANIM_DISP_PET_GUARD,		// ???
	ANIM_DISP_PET_DAMAGE,		// ????
	ANIM_DISP_PET_DEAD,			// ?昀
	ANIM_DISP_PET_ITEM,			// ???????牙
	
	ANIM_DISP_LETTER_MOVE,		// ??去
	ANIM_DISP_LETTER,			// ????????
	
	ANIM_DISP_MIX_PET_INIT,		// ???????
	ANIM_DISP_MIX_PET_MOVE,		// ?????????去
	ANIM_DISP_MIX_PET_MOJI,		// ???????????????
#ifdef _THEATER
	ANIM_DISP_THEATER_NPC,		// 曄部NPC珆尨
#endif
#ifdef _AniCrossFrame	  // Syu ADD 雄賒脯蚔徹賒醱汜昜
	ANIM_DISP_CROSSFRAME = 18,
#endif
	ANIM_DISP_MIX_PET_END,		// ????????
};

// ?????﹨▍
typedef struct{
	char name[ PET_NAME_LEN + 1 ];	// ?????〈
	char freeName[ PET_NAME_LEN + 1 ];	// ???????
	int faceGraNo;					// ???????????吻?
	int level;						// ???
	int maxHp;						// ????
	int str;						// ???
	int quick;						// ???
	int def;						// ??
	int earth, water, fire, wind;	// ???
	int flag;						// 仕??????ㄩ?????? ?ㄩ?? ?ㄩ????? ?ㄩ仕??? ?
}PET_ALBUM;

// ????????????????﹨▍
typedef struct{
	int albumNo;					// ?????????吻?
	char name[ PET_NAME_LEN + 1 ];	// ?????〈
	int faceGraNo;					// ???????????吻?
}PET_ALBUM_TBL;


// ????????
extern BOOL TaskBarFlag;
// ??????
extern unsigned int MenuToggleFlag;
// ?????????
extern ITEM_BUFFER ItemBuffer[];
// ????????????
extern int StatusUpPoint;
// ·???阪??????????
extern BOOL BattlePetReceiveFlag;
#ifdef _STANDBYPET		
extern BOOL StandbyPetSendFlag;
#endif
// ????????吻?
extern int BattlePetReceivePetNo; 
// ??????更??
extern MAIL_HISTORY MailHistory[];	
// ?????更?仕??向??????
extern STR_BUFFER chatRegistryStr[];
// ????向?更??
extern STR_BUFFER MailStr;		
// ??????????????
extern ACTION *pActMenuWnd3;
// ?????更?仕??向??????
extern STR_BUFFER chatRegistryStr[];
// ????????更??
extern STR_BUFFER shougouChange;

#ifdef _MONEYINPUT //Syu ADD 忒雄怀踢講
extern STR_BUFFER MymoneyBuffer;
#endif
#ifdef _ITEM_JIGSAW
extern int JigsawIdx;
void SetJigsaw(int img, char *str);
#endif

typedef enum{
	MAIL_WND_VIEW,
	MAIL_WND_SELECT,
	MAIL_WND_SEND,
	MAIL_WND_PET_SEND,
	MAIL_WND_ITEM,
	MAIL_WND_HISTORY,
}MAIL_WND_TYPE;

extern STR_BUFFER TradeBuffer;
// ?????〈阪??更??
extern STR_BUFFER petNameChange;
// Terry add 2002/01/04
#ifdef __EDEN_AUCTION
extern STR_BUFFER AuctionStr;		// 鼴闖唦扴僕,50跺趼
extern STR_BUFFER AuctionMoney;	// 鼴闖歎郔嗣珨啃勀
#endif
// Terry end
// ???????牙?????????吻?
extern int  petStatusNo;

extern int mailHistoryWndPageNo;	// ???????????吻?
extern int mailHistoryWndSelectNo;	// ︻???????吻?	
//extern int mailReceivePetGraNo;		// ??????????????????吻?

// ·卯??去????????牙?????
extern int MapWmdFlagBak;

// ·卯??????????????????
#define RESULT_WND_CLOSE_TIME 	300	// ?企
// ·卯?????????????????
extern int ResultWndTimer;

// ??????
extern BOOL ItemMixRecvFlag;

// ???????
extern int MouseCursorFlag;							
#ifdef _TRADETALKWND				// (祥褫羲) Syu ADD 蝠眢陔崝勤趕遺殤
void TradeTalk ( char *msg ) ; 
#endif

void InitItem( int x, int y , BOOL bPetItemFlag);

// ?????????? *******************************************************/
void InitMenu( void );

// ??????????? *******************************************************/
void InitMenu2( void );

// ??????????? *******************************************************/
BOOL CheckMenuFlag( void );

// ?????? ***************************************************************/
void MenuProc( void );

// ?????牙???? ***************************************************/
ACTION *MakeWindowDisp( int x, int y, int sizeX, int sizeY, int titleNo, int wndType 
#ifdef _NEW_RESOMODE  //800 600耀宒
					   , BOOL fixType = TRUE
#endif
					   );

// ???刪叉????? ******************************************************/
BOOL MakeHitBox( int x1, int y1, int x2, int y2, int dispPrio );

// ???????卒㎏ ****************************************************************/
BOOL SaveMailHistory( int no );

// ??????????? ****************************************************************/
BOOL LoadMailHistory( void );

// ???????? ****************************************************************/
BOOL CheckMailNoReadFlag( void );

// ??????????兣?﹐?? **************************************************/
void InitOhtaParam( void );

// ????????? **********************************************************/
void DeathLetterAction( void ); 

// ?????仕?更??卒㎏ ****************************************************************/
BOOL SaveChatRegistyStr( void );

// ?????仕?更?????? ****************************************************************/
BOOL LoadChatRegistyStr( void );

// 更?????????? *****************************************************/
void CenteringStr( char *inStr, char *outStr, int max );

// ????????? ****************************************************************/
void CheckNewPet( int sprNo );

// ·卯?????????? **********************************************************/
void CheckBattleNewPet( void );

// ????????? ****************************************************************/
BOOL LoadAlbum( void );

// ?????卒㎏ ****************************************************************/
BOOL SaveAlbum( int no );
ACTION *MakeAnimDisp( int x, int y, int sprNo, int mode );

// Robin 04/16 trade
void tradeInit( void );

void BankmanInit( char* data );
void BankmanWN( void );
void closeBankman( void );
#endif

#ifdef _TELLCHANNEL				//ROG ADD 躇逄耋
void InitSelectChar( char *msg, BOOL endFlag);
void SelectChar( void );
void DeathTellChannel( void );
extern int TalkMode;
extern char secretName[32];
#endif


#ifdef _FRIENDCHANNEL      //ROG ADD 疑衭耋
#define MAX_ROOM_NUM  50
extern bool setRoomFlag;
extern bool secretFlag;				   //躇逄梓
extern int selChar;
extern ACTION *pChtChanlWnd;            //謐毞弅弝敦
extern ACTION *pSelChanlWnd;
void SelectChatRoom( void );
void InitSelChatRoom(char *msg);
void InitCreateChatRoom(char *msg);
void ChatRoomWnd(void);
void initAssentWnd(char *data);
void AssentWnd(void);
extern STR_BUFFER chatRoomName;            //謐毞弅靡備 
extern ACTION *pSetRoomWnd;
void setRoomName(void);					   //扢隅謐毞弅靡備

#endif


/////////////////////////////////////////
#ifdef _TIMEBAR_FUNCTION
int SetTimeBarPos(int timBarIdent, int CurPos);
int SetTimeBar(char *title, int range);
#endif

#ifdef _STANDBYPET
void send_StandBy_Pet( void);
#endif

#ifdef _NPC_ITEMUP
void ShowItemup(int value, int x, int y);
#endif

#ifdef _NPC_ITEMUP
void ShowCounttime(int value, int x, int y);
#endif

#ifdef _NEW_SPEED_ 
extern BOOL GameSpeedFlag;
#endif


