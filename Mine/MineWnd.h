#ifndef __MINE_WND_H__
#define __MINE_WND_H__

#include "def.h"

using def::uint;

class CMineWnd : public CWnd
{
	// �׷���ṹ��
	typedef struct
	{
		uint uRow;         //����������ά�������
		uint uCol;         //����������λ�������
		uint uState;       //��ǰ״̬
		uint uAttrib;      //��������
		uint uOldState;    //��ʷ״̬
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
	// init and save functions
	void LoadConfig();
	void SaveConfig();
	void LoadWave();
	void FreeWave();
	void InitGame();
	void LoadBitmap();
	void FreeMines();
	void SizeWindow();
	void LayMines(uint row, uint col);
	// draw functions
	void DrawShell(CPaintDC &dc);
	void DrawButton(CPaintDC &dc);
	void DrawNumber(CPaintDC &dc);
	void DrawMineArea(CPaintDC &dc);
	void DrawDownNum(MINEWND* mine, uint num);
	void DrawSpecialMine(uint row, uint col);
	// change menu check state funtions
	void SetCheckedSound();
	void SetCheckedColor();
	void SetCheckedLevel();
	void SetCheckedCheat();
	// other functions
	void ExpandMines(uint row, uint col);
	BOOL IsInMineArea(uint row, uint col);
	BOOL IsMine(uint row, uint col);
	uint GetAroundMines(uint row, uint col);
	uint GetAroundFlags(uint row, uint col);
	void Dead(uint row, uint col);
	BOOL Victory();
	// both button click fuctions
	void OnLRBtnDown(uint row, uint col);
	void OnLRBtnUp(uint row, uint col);
	void OpenAround(uint row, uint col);
	BOOL ErrorAroundFlag(uint row, uint col/*, uint &errorRow, uint &errorCol*/);
	void OpenByCheat();

	MINEWND* GetMine(long x, long y);

private:
	uint		m_uXNum;				// X����С�������
	uint		m_uYNum;				// Y����С�������
	uint		m_uMineNum;				// �ܵ��׸���
	int 		m_nLeftNum;			// ʣ����׸���
	uint		m_uSpendTime;			// ��Ϸ��ʼ��Ŀǰ�����ѵ�ʱ��
	uint		m_uGameState;			// ��Ϸ״̬
	uint		m_uTimer;				// ��ʱ����ʶ
	uint		m_uNewState;			// ��ǰѡ�е�С�����״̬

	uint		m_uLevel;				// ��ǰ��Ϸ�ȼ�
	uint		m_uPrimary;				// ������¼
	uint		m_uMedium;				// �м���¼
	uint		m_uAdvanced;				// �߼���¼
	CString		m_strPrimary;			// ������¼������
	CString		m_strMedium;			// �м���¼������
	CString		m_strAdvanced;			// �߼���¼������

	BOOL		m_bLRBtnDown;			// �Ƿ�Ϊ���Ҽ�ͬʱ����
	BOOL		m_bClickBtn;			// ������µ�ʱ������Ƿ�λ�ڰ�ť������
	BOOL		m_bColorful;			// �Ƿ��ɫ��ʾ
	BOOL		m_bSoundful;			// �Ƿ�������
	CMenu*		m_pSubMenu;				// �Ӳ˵�
	CBitmap		m_bmpMine;				// ��������ͼ��
	CBitmap		m_bmpNumber;			// ���ֱ���ͼ��
	CBitmap		m_bmpButton;			// Ц����ť����ͼ��
	CBrush		m_brsBG;				// ������ˢ����
	COLORREF	m_clrDark;				// ����ť����ɫ��

	RECT m_rectClient;				// �ͻ�����
	RECT m_rectPanel;
	RECT m_rectBlockArea;
	RECT m_rectLeftMines;
	RECT m_rectSpendTime;
	RECT m_rectButton;

	uint		m_uBtnState;			// ��ť״̬
	
	MINEWND		m_pMines[100][100];		// ��ʾ�����ڵ�����С����Ķ�ά����
	MINEWND*	m_pNewMine;				// ��ǰѡ�е�С����
	MINEWND*	m_pOldMine;				// �ϴ�ѡ�е�С����
	void*		m_pSndDead;				// ʧ����ʾ��
	void*		m_pSndVictory;			// ʤ����ʾ��
	void*		m_pSndClock;			// ʱ����ʾ��
};

#endif//__MINE_WND_H__
