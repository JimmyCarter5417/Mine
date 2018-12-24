#ifndef __DEF_H__
#define __DEF_H__

#include <windef.h>

namespace def
{
    typedef unsigned int uint;

    // 游戏等级
    enum LEVEL_E
    {
        LEVEL_Primary,
        LEVEL_Medium,
        LEVEL_Advanced,
        LEVEL_Custom
    };

    // 游戏状态
    enum GAME_STATE_E
    {
        GAME_STATE_Wait,
        GAME_STATE_Run,
        GAME_STATE_Dead,
        GAME_STATE_Victory
    };

    // 方块属性
    enum BLOCK_ATTR_E
    {
        BLOCK_ATTR_Empty,
        BLOCK_ATTR_Mine,

        BLOCK_ATTR_End
    };

    // 方块状态
    enum BLOCK_STATE_E
    {
        BLOCK_STATE_Normal = 0,     // 正常
        BLOCK_STATE_Flag,           // 标志为雷
        BLOCK_STATE_Dicey,          // 未知状态0
        BLOCK_STATE_Blast,          // 爆炸状态
        BLOCK_STATE_Error,          // 错误状态
        BLOCK_STATE_Mine,           // 雷状态
        BLOCK_STATE_DiceyDown,      // 未知状态1
        BLOCK_STATE_Num8,
        BLOCK_STATE_Num7,
        BLOCK_STATE_Num6,
        BLOCK_STATE_Num5,
        BLOCK_STATE_Num4,
        BLOCK_STATE_Num3,
        BLOCK_STATE_Num2,
        BLOCK_STATE_Num1,
        BLOCK_STATE_Empty,          // 无雷

        BLOCK_STATE_End = 16        // 16
    };

    // 按钮状态
    enum BUTTON_STATE_E
    {
        BUTTON_STATE_Down,
        BUTTON_STATE_Victory,
        BUTTON_STATE_Dead,
        BUTTON_STATE_Click,
        BUTTON_STATE_Normal,
        
        BUTTON_STATE_End // 5
    };

    // 数字
    enum NUM_E
    {
        NUM_Null,
        NUM_Empty,
        NUM_9,
        NUM_8,
        NUM_7,
        NUM_6,
        NUM_5,
        NUM_4,
        NUM_3,
        NUM_2,
        NUM_1,
        NUM_0,
        
        NUM_End // 12
    };

    struct TPos
    {
        uint x; // 行
        uint y; // 列
    };

    struct TBlock
    {
        TPos          pos;       // 坐标
        BLOCK_ATTR_E  attr;      // 雷、空
        BLOCK_STATE_E cur_state; // 当前状态
        BLOCK_STATE_E old_state; // 用以恢复以前的状态
    };

    // 方块，16 * 16
    // 位图，16 * (16 * 16)
    static uint g_nBlockWidth  = 16;
    static uint g_nBlockHeight = 16;
    // 按钮，24 * 24
    // 位图，24 * (24 * 5)
    static uint g_nBtnWidth  = 24;
    static uint g_nBtnHeight = 24;
    // 数字，13 * 23
    // 位图，13 * (23 * 12)
    static uint g_nNumWidth  = 13;
    static uint g_nNumHeight = 23;

    // 偏移量，见图片
    static uint g_nXOffset      = 5;
    static uint g_nYOffset      = 5;
    static uint g_nInnerXOffset = 5;
    static uint g_nInnerYOffset = 8;
    static uint g_nGap          = 5;

    // 面板高度
    static uint g_nPanelHeight = g_nInnerYOffset * 2 + g_nNumHeight;

    // 配置文件默认配置
    static LEVEL_E g_eDefLevel    = LEVEL_Primary;
    static CString g_strDefHolder = TEXT("匿名");
    static uint    g_nDefRecord   = 999;
    static uint    g_nDefColorful = 1;
    static uint    g_nDefSoundful = 0;

    // 各等级默认参数
    static uint g_nDefPrimaryRowNum   = 10;
    static uint g_nDefPrimaryColNum   = 15;
    static uint g_nDefPrimaryMineNum  = 30;

    static uint g_nDefMediumRowNum    = 20;
    static uint g_nDefMediumColNum    = 30;
    static uint g_nDefMediumMineNum   = 120;

    static uint g_nDefAdvancedRowNum  = 30;
    static uint g_nDefAdvancedColNum  = 45;
    static uint g_nDefAdvancedMineNum = 480;

    // 颜色定义
    static COLORREF g_clrBlack    = RGB(  0,   0,   0);
    static COLORREF g_clrWhite    = RGB(255, 255, 255);
    static COLORREF g_clrGray     = RGB(192, 192, 192);
    static COLORREF g_clrDarkGray = RGB(128, 128, 128);

    // 配置文件名字
    static CString g_strCfgName  = TEXT("Config.ini");
    // 配置文件section、key
    static CString g_strMine     = TEXT("Mine");
    static CString g_strRowNum   = TEXT("RowNum");
    static CString g_strColNum   = TEXT("ColNum");
    static CString g_strMineNum  = TEXT("MineNum");
    static CString g_strLevel    = TEXT("Level");
    
    static CString g_strRecord   = TEXT("Record");
    static CString g_strPrimary  = TEXT("Primary");
    static CString g_strMedium   = TEXT("Medium");
    static CString g_strAdvanced = TEXT("Advanced");

    static CString g_strHolder   = TEXT("Holder");
    /*static CString g_strPrimary = TEXT("Primary");
    static CString g_strMedium = TEXT("Medium");
    static CString g_strAdvanced = TEXT("Advanced");*/

    static CString g_strConfig   = TEXT("Config");
    static CString g_strColorful = TEXT("Colorful");
    static CString g_strSoundful = TEXT("Soundful");
}

#endif // __DEF_H__