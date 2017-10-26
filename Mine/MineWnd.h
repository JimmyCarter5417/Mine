#ifndef __MINE_WND_H__
#define __MINE_WND_H__

#include "def.h"
#include "BlockArea.h"
#include "CfgMgr.h"

using def::uint;

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
	def::ELevel m_uLevel;
	uint m_uRowNum;
	uint m_uColNum;	
	uint m_uMineNum;

	int  m_nLeftNum;//ʣ����׸���
	uint m_uSpendTime;//��Ϸ��ʼ��Ŀǰ�����ѵ�ʱ��
	
	uint m_uTimer;//��ʱ��
	
	CMenu* m_pSubMenu;

	CBitmap	m_bmpMine;
	CBitmap	m_bmpNumber;
	CBitmap	m_bmpButton;

	CBrush	 m_brsBG;//client����ˢ
	COLORREF m_clrBg;//���ؼ�������ɫ

	RECT m_rectClient;//�����ͻ�����
	RECT m_rectPanel;//�ϲ��������
	RECT m_rectBlockArea;//�²���������
	RECT m_rectLeftMines;//������ʣ������
	RECT m_rectSpendTime;//����Ҳ�ʱ��
	RECT m_rectButton;//����м䰴ť

	def::EButtonState m_uBtnState;//��ť״̬
	def::EGameState	  m_uGameState;//��Ϸ״̬

	bool m_bLRBtnDown;//���Ҽ�ͬʱ����
	bool m_bClickBtn;//�ѵ�����ťδ�ɿ�
	bool m_bColorful;
	bool m_bSoundful;
	
	TPos m_posCurBlock;//��ǰѡ�еķ���
	TPos m_posOldBlock;//�ϴ�ѡ�еķ���

	void* m_pSndDead;//ʧ����
	void* m_pSndVictory;//ʤ����
	void* m_pSndClock;//ʱ����

	CBlockArea* m_pBlockArea;//�����������ݽṹ
	CCfgMgr*    m_pCfgMgr;
};

#endif//__MINE_WND_H__
