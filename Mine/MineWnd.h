#ifndef __MINE_WND_H__
#define __MINE_WND_H__

class CMineWnd : public CWnd
{
	// �׷���ṹ��
	typedef struct
	{
		UINT uRow;         //����������ά�������
		UINT uCol;         //����������λ�������
		UINT uState;       //��ǰ״̬
		UINT uAttrib;      //��������
		UINT uOldState;    //��ʷ״̬
	} MINEWND;	

public:
	CMineWnd();
	 ~CMineWnd();

	void ResetRecord();
	void SetCustom(UINT xNum, UINT yNum, UINT mNum);
	
protected:	
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
	afx_msg void OnMemuStart();
	afx_msg void OnMemuPrimary();
	afx_msg void OnMemuSecond();
	afx_msg void OnMemuAdvance();
	afx_msg void OnMemuCustom();
	afx_msg void OnMemuMark();
	afx_msg void OnMemuColor();
	afx_msg void OnMemuSound();
	afx_msg void OnMemuExit();
	afx_msg void OnMemuHelpList();
	afx_msg void OnMemuHelpFind();
	afx_msg void OnMemuHelpUse();
	afx_msg void OnMemuAbout();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
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
	void LayMines(UINT row, UINT col);
	// draw functions
	void DrawShell(CPaintDC &dc);
	void DrawButton(CPaintDC &dc);
	void DrawNumber(CPaintDC &dc);
	void DrawMineArea(CPaintDC &dc);
	void DrawDownNum(MINEWND* mine, UINT num);
	void DrawSpecialMine(UINT row, UINT col);
	// change menu check state funtions
	void SetCheckedSound();
	void SetCheckedColor();
	void SetCheckedMark();
	void SetCheckedLevel();
	void SetCheckedCheat();
	// other functions
	void ExpandMines(UINT row, UINT col);
	BOOL IsInMineArea(UINT row, UINT col);
	BOOL IsMine(UINT row, UINT col);
	UINT GetAroundNum(UINT row, UINT col);
	UINT GetAroundFlags(UINT row, UINT col);
	void Dead(UINT row, UINT col);
	BOOL Victory();
	// both button click fuctions
	void OnLRBtnDown(UINT row, UINT col);
	void OnLRBtnUp(UINT row, UINT col);
	void OpenAround(UINT row, UINT col);
	BOOL ErrorAroundFlag(UINT row, UINT col/*, UINT &errorRow, UINT &errorCol*/);
	void OpenByCheat();

	MINEWND* GetMine(long x, long y);

private:
	UINT		m_uXNum;				// X����С�������
	UINT		m_uYNum;				// Y����С�������
	UINT		m_uMineNum;				// �ܵ��׸���
	int 		m_nLeftNum;			// ʣ����׸���
	UINT		m_uSpendTime;			// ��Ϸ��ʼ��Ŀǰ�����ѵ�ʱ��
	UINT		m_uGameState;			// ��Ϸ״̬
	UINT		m_uTimer;				// ��ʱ����ʶ
	UINT		m_uNewState;			// ��ǰѡ�е�С�����״̬

	UINT		m_uLevel;				// ��ǰ��Ϸ�ȼ�
	UINT		m_uPrimary;				// ������¼
	UINT		m_uSecond;				// �м���¼
	UINT		m_uAdvance;				// �߼���¼
	CString		m_strPrimary;			// ������¼������
	CString		m_strSecond;			// �м���¼������
	CString		m_strAdvance;			// �߼���¼������

	BOOL		m_bLRBtnDown;			// �Ƿ�Ϊ���Ҽ�ͬʱ����
	BOOL		m_bClickBtn;			// ������µ�ʱ������Ƿ�λ�ڰ�ť������
	BOOL		m_bMarkful;				// �Ƿ�����ʾ���
	BOOL		m_bColorful;			// �Ƿ��ɫ��ʾ
	BOOL		m_bSoundful;			// �Ƿ�������
	CMenu*		m_pSubMenu;				// �Ӳ˵�
	CBitmap		m_bmpMine;				// ��������ͼ��
	CBitmap		m_bmpNumber;			// ���ֱ���ͼ��
	CBitmap		m_bmpButton;			// Ц����ť����ͼ��
	CBrush		m_brsBG;				// ������ˢ����
	COLORREF	m_clrDark;				// ����ť����ɫ��
	RECT		m_rcClient;				// �ͻ�����
	UINT		m_uBtnRect[3];			// ��ť��������������
	UINT		m_uBtnState;			// ��ť״̬
	UINT		m_uNumRect[3];			// ���ֿ�������������(����ʱ����׸���)
	UINT		m_uShellRcX[2];			// �ڿ��Լ��߽������X����
	UINT		m_uShellRcY[2];			// �ڿ��Լ��߽������Y����
	
	MINEWND		m_pMines[100][100];		// ��ʾ�����ڵ�����С����Ķ�ά����
	MINEWND*	m_pNewMine;				// ��ǰѡ�е�С����
	MINEWND*	m_pOldMine;				// �ϴ�ѡ�е�С����
	void*		m_pSndDead;				// ʧ����ʾ��
	void*		m_pSndVictory;			// ʤ����ʾ��
	void*		m_pSndClock;			// ʱ����ʾ��
};

#endif//__MINE_WND_H__
