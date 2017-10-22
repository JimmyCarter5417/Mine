#ifndef __DEF_H__
#define __DEF_H__

#include <windef.h>

namespace def
{
	typedef unsigned int uint;

	//游戏等级
	enum ELevel
	{
		ELevel_Primary,
		ELevel_Medium,
		ELevel_Advanced,
		ELevel_Custom
	};

	//游戏状态
	enum EGameState
	{
		EGS_Wait,
		EGS_Run,
		EGS_Dead,
		EGS_Victory
	};

	//方块属性
	enum EBlockAttr
	{
		EBA_Empty,
		EBA_Mine
	};

	//方块状态
	enum EBlockState
	{
		EBS_Normal,//正常
		EBS_Flag,//标志为雷
		EBS_Dicey,//未知状态0
		EBS_Blast,//爆炸状态
		EBS_Error,//错误状态
		EBS_Mine,//雷状态
		EBS_DiceyDown,//未知状态1
		EBS_Num8,
		EBS_Num7,
		EBS_Num6,
		EBS_Num5,
		EBS_Num4,
		EBS_Num3,
		EBS_Num2,
		EBS_Num1,		
		EBS_Empty,//无雷

		EBS_End//16
	};

	//按钮状态
	enum EButtonState
	{
		EBtnS_Down,
		EBtnS_Victory,
		EBtnS_Dead,
		EBtnS_Click,
		EBtnS_Normal,
		
		EBtnS_End//5
	};

	//数字
	enum ENum
	{
		ENum_Null,
		ENum_Empty,
		ENum_9,
		ENum_8,
		ENum_7,
		ENum_6,
		ENum_5,
		ENum_4,
		ENum_3,
		ENum_2,
		ENum_1,
		ENum_0,
		
		ENum_End//12
	};

	//方块，16 * 16
	//位图，16 * (16 * 16)
	static uint g_nBlockWidth = 16;
	static uint g_nBlockHeight = 16;
	//按钮，24 * 24
	//位图，24 * (24 * 5)
	static uint g_nBtnWidth = 24;
	static uint g_nBtnHeight = 24;
	//数字，13 * 23
	//位图，13 * (23 * 12)
	static uint g_nNumWidth = 13;
	static uint g_nNumHeight = 23;

	//偏移量，见图片
	static uint g_nXOffset = 5;
	static uint g_nYOffset = 5;
	static uint g_nInnerXOffset = 5;
	static uint g_nInnerYOffset = 8;
	static uint g_nGap = 5;

	//面板高度
	static uint g_nPanelHeight = g_nInnerYOffset * 2 + g_nNumHeight;

	//配置文件默认配置
	static ELevel  g_DefLevel     = ELevel_Primary;
	static CString g_strDefHolder = TEXT("匿名");
	static uint    g_nDefRecord   = 999;
	static uint    g_nDefColorful = 1;
	static uint    g_nDefSoundful = 0;

	//各等级默认参数
	static uint g_nDefPrimaryXNum = 10;
	static uint g_nDefPrimaryYNum = 10;
	static uint g_nDefPrimaryMineNum = 10;

	static uint g_nDefMediumXNum = 16;
	static uint g_nDefMediumYNum = 16;
	static uint g_nDefMediumMineNum = 40;

	static uint g_nDefAdvancedXNum = 30;
	static uint g_nDefAdvancedYNum = 30;
	static uint g_nDefAdvancedMineNum = 99;

	//颜色定义
	static COLORREF g_clrBlack = RGB(0, 0, 0);
	static COLORREF g_clrWhite = RGB(255, 255, 255);
	static COLORREF g_clrGray = RGB(192, 192, 192);
	static COLORREF g_clrDarkGray = RGB(128, 128, 128);

	//配置文件名字
	static CString g_strCfgName = TEXT("Config.ini");
	//配置文件section、key
	static CString g_strMine = TEXT("MINE");
	static CString g_strXNum = TEXT("XNum");
	static CString g_strYNum = TEXT("YNum");
	static CString g_strMineNum = TEXT("MineNum");
	static CString g_strLevel = TEXT("Level");
	
	static CString g_strRecord = TEXT("Record");
	static CString g_strPrimary = TEXT("Primary");
	static CString g_strMedium = TEXT("Medium");
	static CString g_strAdvanced = TEXT("Advanced");

	static CString g_strHolder = TEXT("Holder");
	/*static CString g_strPrimary = TEXT("Primary");
	static CString g_strMedium = TEXT("Medium");
	static CString g_strAdvanced = TEXT("Advanced");*/

	static CString g_strConfig = TEXT("Config");
	static CString g_strColorful = TEXT("Colorful");
	static CString g_strSoundful = TEXT("Soundful");
}

#endif//__DEF_H__