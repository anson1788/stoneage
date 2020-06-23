
#ifndef _ONLINEGM_
#define _ONLINEGM_

#define ONLINEGM_BACKGROUND		OLD_GRAPHICS_START+821  //背景图
#define ONLINEGM_YES_OR_NO      35322                   //小长方石板背景图 26195有问题的图
#define ONLINEGM_CHECK_U		OLD_GRAPHICS_START+820	//查询(弹起)
#define ONLINEGM_CHECK_D		35319					//查询(按下)		
#define ONLINEGM_DELETE_U		OLD_GRAPHICS_START+819	//删除(弹起)
#define ONLINEGM_DELETE_D		35321					//删除(按下)
#define ONLINEGM_ENTER_U		26099					//传送(弹起)
#define ONLINEGM_ENTER_D		35320//传送(按下)
#define	ONLINEGM_CLEAR_U		26298					//清除(弹起)
#define ONLINEGM_CLEAR_D		26299					//清除(按下)
#define ONLINEGM_HELP_U			OLD_GRAPHICS_START+818	//注意事项(弹起)
#define ONLINEGM_HELP_D			35318					//注意事项(按下)
#define ONLINEGM_QUIT			26042					    //关闭
#define ONLINEGM_UP_U			26064					//上卷(弹起)
#define ONLINEGM_UP_D			26065					//上卷(按下)
#define ONLINEGM_DOWN_U			26066					//下卷(弹起)
#define ONLINEGM_DOWN_D			26067					//下卷(按下)
#define ONLINEGM_SURE_D			26288					//确定(按下)(黄色的)
#define ONLINEGM_SURE_U			26289					//确定(弹起)(黄色的)
#define ONLINEGM_CHANCEL        26291					//取消


#define DEF_UPDOWN_TOTAL        22						// 上下卷总数     
#define DEF_BUTTON_TOTAL        28                      // 线上回报系统按钮总数
#define DEF_CLASS_TOTAL         2                       // 问题类别总数
#define DEF_C1_TOTAL			14						// 类别一细项总数
#define DEF_C2_TOTAL			11 						// 类别二细项总数

#define DEF_MAX_ADD_VALUE     20   //目前设定玩家最多留20笔

#define MAX_INPUT_LINE 		30 			// 问题输入行数限制

#define DEF_INPUT_LINE 		4 			// 问题数入显示行数 chat是20

#define MAX_INPUT_HISTORY 	20			// ???????????

const long UP_ANIM_ID[DEF_BUTTON_TOTAL]={
	ONLINEGM_CHECK_U	,
	ONLINEGM_DELETE_U	,
	ONLINEGM_ENTER_U	,
	ONLINEGM_CLEAR_U	,
	ONLINEGM_HELP_U		, 
	ONLINEGM_QUIT	,
	ONLINEGM_UP_U		,
	ONLINEGM_DOWN_U		,
	ONLINEGM_UP_U		,
	ONLINEGM_DOWN_U		,
	ONLINEGM_UP_U		,
	ONLINEGM_DOWN_U		,
	ONLINEGM_UP_U		,
	ONLINEGM_DOWN_U		,
	ONLINEGM_UP_U		,
	ONLINEGM_DOWN_U		,
	ONLINEGM_UP_U		,
	ONLINEGM_DOWN_U		,
	ONLINEGM_UP_U		,
	ONLINEGM_DOWN_U		,
	ONLINEGM_UP_U		,
	ONLINEGM_DOWN_U		,
	ONLINEGM_UP_U		,
	ONLINEGM_DOWN_U		,
	ONLINEGM_UP_U		,
	ONLINEGM_DOWN_U		,
	ONLINEGM_UP_U		,
	ONLINEGM_DOWN_U		
};
const long DOWN_ANIM_ID[DEF_BUTTON_TOTAL]={
	ONLINEGM_CHECK_D	,
	ONLINEGM_DELETE_D	,
	ONLINEGM_ENTER_D	,
	ONLINEGM_CLEAR_D	,
	ONLINEGM_HELP_D	    , 
	ONLINEGM_QUIT	    ,
	ONLINEGM_UP_D		,
	ONLINEGM_DOWN_D		,
	ONLINEGM_UP_D		,
	ONLINEGM_DOWN_D		,
	ONLINEGM_UP_D		,
	ONLINEGM_DOWN_D		,
	ONLINEGM_UP_D		,
	ONLINEGM_DOWN_D		,
	ONLINEGM_UP_D		,
	ONLINEGM_DOWN_D		,
	ONLINEGM_UP_D		,
	ONLINEGM_DOWN_D		,
	ONLINEGM_UP_D		,
	ONLINEGM_DOWN_D		,
	ONLINEGM_UP_D		,
	ONLINEGM_DOWN_D		,
	ONLINEGM_UP_D		,
	ONLINEGM_DOWN_D		,
	ONLINEGM_UP_D		,
	ONLINEGM_DOWN_D		,
	ONLINEGM_UP_D		,
	ONLINEGM_DOWN_D		
};
const short BUTTON_XY[DEF_BUTTON_TOTAL][2]={
	{584,97 },{584  ,120},{584,316},{584,  340},{584,409},{584  ,434},
	{550-100,54+100 },{530+1-100,72+100 },{550-100,366+100},{530+1-100,384+100},
	{164-100,175+100},{144+1-100,193+100},{229-100,175+100},{209+1-100,193+100},{298-100,175+100},{278+1-100,193+100},
	{468-100+21,175+100},{448+1-100+21,193+100},{530-100+21,175+100},{510+1-100+21,193+100},{593-100+21,175+100},{573+1-100+21,193+100},
	{164-100,212+100},{144+1-100,230+100},{392-100,212+100},{372+1-100,230+100},{614-100,212+100},{594+1-100,230+100}	
};

int iOnlineGmProc( void );

#endif
