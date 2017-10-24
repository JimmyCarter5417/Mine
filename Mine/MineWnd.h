#ifndef __MINE_WND_H__
#define __MINE_WND_H__

#include "def.h"

using def::uint;

class CBlockArea;

class CMineWnd : public CWnd
{
	// 雷方块结构体
	typedef struct
	{
		uint uRow;         //所在雷区二维数组的行
		uint uCol;         //所在雷区二位数组的列
		uint uState;       //当前状态
		uint uAttrib;      //方块属性
		uint uOldState;    //历史状态
	} MINEWND;	

public:
	CMineWnd();
	 ~CMineWnd();

	void ResetRecord();
	void SetCustom(uint xNum, uint yNum, uint mNum);
	
protected:	
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, uint nStatus);
	afx_msg void OnTimer(uint nIDEvent);
	afx_msg void OnLButtonUp(uint nFlags, CPoint pt);
	afx_msg void OnRButtonUp(uint nFlags, CPoint pt);
	afx_msg void OnLButtonDown(uint nFlags, CPoint pt);
	afx_msg void OnRButtonDown(uint nFlags, CPoint pt);
	afx_msg void OnMouseMove(uint nFlags, CPoint pt);
	afx_msg void OnMemuStart();
	afx_msg void OnMemuPrimary();
	afx_msg void OnMemuMedium();
	afx_msg void OnMemuAdvanced();
	afx_msg void OnMemuCustom();
	afx_msg void OnMemuColor();
	afx_msg void OnMemuSound();
	afx_msg void OnMemuExit();
	afx_msg void OnMemuHelpList();
	afx_msg void OnMemuHelpFind();
	afx_msg void OnMemuHelpUse();
	afx_msg void OnMemuAbout();
	afx_msg void OnKeyDown(uint nChar, uint nRepCnt, uint nFlags);
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnMemuClose();
	afx_msg void OnMemuHero();
	afx_msg void OnMemuCheat();

	DECLARE_MESSAGE_MAP()

protected:
	CString GetCfgPath();
	
	void LoadConfig();
	void SaveConfig();

	void LoadWave();	
	void FreeWave();
	
	void LoadBitmap();

	void InitGame();	

	void CalcWindowSize();
	
	// draw functions
	void DrawFrame(CPaintDC &dc);
	void DrawButton(CPaintDC &dc);
	void DrawNumber(CPaintDC &dc);
	void DrawBlockArea(CPaintDC &dc);
	void DrawDownNum(MINEWND* mine, uint num);
	void DrawOneBlock(uint row, uint col);
	// change menu check state funtions
	void SetCheckedSound();
	void SetCheckedColor();
	void SetCheckedLevel();
	void SetCheckedCheat();
	// other functions
	void ExpandMines(uint row, uint col);
	BOOL IsInBlockArea(uint row, uint col);
	BOOL IsMine(uint row, uint col);
	uint GetAroundMines(uint row, uint col);
	uint GetAroundFlags(uint row, uint col);
	void Dead(uint row, uint col);
	BOOL Victory();
	// both button click fuctions
	void OnLRBtnDown(uint row, uint col);
	void OnLRBtnUp(uint row, uint col);
	void OpenAround(uint row, uint col);
	BOOL HasCorrectFlags(uint row, uint col/*, uint &errorRow, uint &errorCol*/);
	void OpenByCheat();

	bool GetBlock(long x, long y, pair<uint, uint>& pos);
	
	bool IsInBtn(CPoint pt);
	bool IsInBlockArea(CPoint pt);

private:
	uint		m_uRowNum;				// X方向小方块个数
	uint		m_uColNum;				// Y方向小方块个数
	uint		m_uLevel;				// 当前游戏等级
	uint		m_uMineNum;				// 总的雷个数
	int 		m_nLeftNum;			    // 剩余的雷个数
	uint		m_uSpendTime;			// 游戏开始到目前所花费的时间
	uint		m_uGameState;			// 游戏状态
	uint		m_uTimer;				// 定时器标识
	
	uint		m_uPrimary;				// 初级记录
	uint		m_uMedium;				// 中级记录
	uint		m_uAdvanced;			// 高级记录
	CString		m_strPrimary;			// 初级记录保持者
	CString		m_strMedium;			// 中级记录保持者
	CString		m_strAdvanced;			// 高级记录保持者

	BOOL		m_bLRBtnDown;			// 是否为左右键同时按下
	BOOL		m_bClickBtn;			// 左键按下的时候鼠标是否位于按钮区域内

	BOOL		m_bColorful;			// 是否彩色显示
	BOOL		m_bSoundful;			// 是否有声音
	CMenu*		m_pSubMenu;				// 子菜单
	CBitmap		m_bmpMine;				// 雷区背景图像
	CBitmap		m_bmpNumber;			// 数字背景图像
	CBitmap		m_bmpButton;			// 笑脸按钮背景图像
	CBrush		m_brsBG;				// 背景画刷对象
	COLORREF	m_clrDark;				// 各按钮的深色调

	RECT m_rectClient;				// 客户区域
	RECT m_rectPanel;
	RECT m_rectBlockArea;
	RECT m_rectLeftMines;
	RECT m_rectSpendTime;
	RECT m_rectButton;

	uint		m_uBtnState;			// 按钮状态
	
	pair<uint, uint>	m_posCurBlock;				// 当前选中的小方块
	pair<uint, uint>	m_posOldBlock;				// 上次选中的小方块

	void*		m_pSndDead;				// 失败提示音
	void*		m_pSndVictory;			// 胜利提示音
	void*		m_pSndClock;			// 时钟提示音

	CBlockArea* m_pBlockArea;
};

#endif//__MINE_WND_H__
