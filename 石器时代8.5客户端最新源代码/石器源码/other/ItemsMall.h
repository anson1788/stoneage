#ifndef _ITEMS_MALL_H__
#define _ITEMS_MALL_H__

#include <map>


#define ITEMS_MALL_FILE_NAME "shopsellitems.bin"


typedef struct ITEMSMALL
{
	int id;                  //id
	int itemId;              //物品/宠物ID
	char itemName[128];      //物品/宠物名字
	int itemFlag;            //是物品还是宠物，物品为：1；宠物为：2。
	int bmpNo;               //宠物/物品外观图片
	int currencyType;        //购买些物品所需的货币种类
	int currencyNum;        //购买此物品所需的金币数量，不能用此货币购买，则此字段为：-1
	int recordType;          //物品种类：1表示最新上价；4表示其它货品；
	char szLine1[128];
	char szLine2[128];
	char szLine3[128];
}*LP_ITEMSMALL;

//内置商城相关操作显示类
class ItemsMall
{
public:
	static ItemsMall& MakeInstance();
	static void Destroy();

	void SetItemDatas(char* pszData);
	
	int GetItemsNums();
	int GetPetsNums();
	int GetAllItemSize() { return g_mapItemsMall.size(); }

	ITEMSMALL* GetItemByIndex(int nIndex);
	ITEMSMALL* GetItemByType(int nIndex, int nType, int recordType = 1);   //recodType默认值为1表示最新上价物品，4为其它物品
	ITEMSMALL* GetAllItemsByIndex(int nIndex, int nType);
	ITEMSMALL* GetAllPetsByIndex(int nIndex, int nType);
	ITEMSMALL* GetAllOtherItemsByIndex(int nIndex, int nType);

	int GetCurrentItemNum(int nCurrencyType, int nType);

	//int GetAllLatestNum();  //最新上价货品数量
	//int GetAllItemsNum(); //装备货品数量
	//int GetAllPetsNum(); //宠物货品数量
	//int GetAllOthersNum(); //其它货品数量

protected:
	ItemsMall(void){}
	ItemsMall(ItemsMall&);

	~ItemsMall(void){}

	static ItemsMall* instance;
	static std::map<int, ITEMSMALL> g_mapItemsMall;  

	//static int m_nAllLatestNum;
	//static int m_nAllItemsNum;
	//static int m_nAllPetsNum;
	//static int m_nAllOthersNum;
};

#endif