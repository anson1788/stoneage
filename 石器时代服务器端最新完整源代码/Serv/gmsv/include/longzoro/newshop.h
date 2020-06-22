#ifndef _NEW_SHOP_H_
#define _NEW_SHOP_H_

#ifdef _ONLINE_SHOP
#define SHOPLISTMAX 100
#define SHOPPAGEMAX 8

//编者#图像#名称#颜色#折扣#金额#说明
typedef struct ONLINE_SHOP_t{
	BOOL use;	
	int id;	
	int picid;
	char name[64];
	int color;
	int percentage;
	int price;
	char readme[256];
}Online_Shop_t;


typedef enum
{
	ONLINESHOP_PET,
	ONLINESHOP_ITEM,
	ONLINESHOP_HEALER,
	ONLINESHOP_COMPOSE,
	ONLINESHOP_MISSION,
	ONLINESHOP_OTHER,
	ONLINESHOP_AMPOINT,
	ONLINESHOP_NUM
}CHAR_ONLINESHOP;

BOOL OnlineShop_init();
void OnlineShop_ShowList(int fd, int charaindex, int type, int page);
void OnlineShop_Buy(int fd, int charaindex, int type, int page, int id, int num);

#endif

#endif
