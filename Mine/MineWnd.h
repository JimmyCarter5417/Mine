#ifndef __MINE_WND_H__
#define __MINE_WND_H__

using namespace def;

class CBlockArea;

class CMineWnd : public CWnd
{
public:
    CMineWnd();
     ~CMineWnd();
    
protected:  
    afx_msg void OnInitMenu(CMenu* pMenu);

    afx_msg void OnPaint();
    afx_msg void OnShowWindow(BOOL bShow, uint nStatus);
    afx_msg void OnTimer(uint nIDEvent);

    afx_msg void OnLButtonUp(uint nFlags, CPoint pt);
    afx_msg void OnRButtonUp(uint nFlags, CPoint pt);
    afx_msg void OnLButtonDown(uint nFlags, CPoint pt);
    afx_msg void OnRButtonDown(uint nFlags, CPoint pt);
    afx_msg void OnMouseMove(uint nFlags, CPoint pt);   
    afx_msg void OnKeyDown(uint nChar, uint nRepCnt, uint nFlags);

    afx_msg void OnMenuStart();
    afx_msg void OnMenuHero();
    afx_msg void OnMenuPrimary();
    afx_msg void OnMenuMedium();
    afx_msg void OnMenuAdvanced();
    afx_msg void OnMenuCustom();
    afx_msg void OnMenuColor();
    afx_msg void OnMenuSound();
    afx_msg void OnMenuExit();

    afx_msg void OnMenuHelp();
    afx_msg void OnMenuAbout();

    DECLARE_MESSAGE_MAP()

protected:
    void LoadConfig();
    void LoadBitmap();
    void LoadWave();    
    void FreeWave();    

    void InitGame();    

    void CalcWindowSize();
    
    // draw functions
    void DrawFrame(CPaintDC &dc);
    void DrawButton(CPaintDC &dc);
    void DrawNumber(CPaintDC &dc);
    void DrawBlockArea(CPaintDC &dc);

    // change menu check state funtions
    void SetCheckedSound();
    void SetCheckedColor();
    void SetCheckedLevel();
    void SetCheckedCheat();

    bool GetBlock(long x, long y, TPos& pos);
    
    bool IsInBtn(CPoint pt);
    bool IsInBlockArea(CPoint pt);

    void Victory();
    void Dead();

    void GodView();

private:
    LEVEL_E m_uLevel;
    uint m_uRowNum;
    uint m_uColNum; 
    uint m_uMineNum;

    int  m_nLeftNum;   // 剩余的雷个数
    uint m_uSpendTime; // 游戏开始到目前所花费的时间    
    uint m_uTimer;     // 定时器
    
    CMenu* m_pSubMenu;

    CBitmap m_bmpMine;
    CBitmap m_bmpNumber;
    CBitmap m_bmpButton;

    CBrush   m_brsBG; // client区域画刷
    COLORREF m_clrBg; // 各控件背景颜色

    RECT m_rectClient;    // 整个客户区域
    RECT m_rectPanel;     // 上部面板区域
    RECT m_rectBlockArea; // 下部方块区域
    RECT m_rectLeftMines; // 面板左侧剩余雷数
    RECT m_rectSpendTime; // 面板右侧时间
    RECT m_rectButton;    // 面板中间按钮

    BUTTON_STATE_E m_uBtnState;  // 按钮状态
    GAME_STATE_E   m_uGameState; // 游戏状态

    bool m_bLRBtnDown; // 左右键同时按下
    bool m_bClickBtn;  // 已单击按钮未松开
    bool m_bColorful;
    bool m_bSoundful;
    
    TPos m_posCurBlock; // 当前选中的方块
    TPos m_posOldBlock; // 上次选中的方块

    void* m_pSndDead;    // 失败音
    void* m_pSndVictory; // 胜利音
    void* m_pSndClock;   // 时钟音

    CBlockArea* m_pBlockArea; // 方块区域数据结构
    CCfgMgr*    m_pCfgMgr;
};

#endif // __MINE_WND_H__
