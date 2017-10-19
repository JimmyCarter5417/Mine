#include "stdafx.h"

#include "Mine.h"
#include "MineWnd.h"
#include "MineDefs.h"

#include "NewRecordDlg.h"
#include "HeroDlg.h"
#include "CustomDlg.h"

#include <Mmsystem.h>

#define ID_TIMER_EVENT	WM_USER + 1

CMineWnd::CMineWnd()
{
	m_pSndDead = nullptr;
	m_pSndVictory = nullptr;
	m_pSndClock = nullptr;
	m_uTimer = 0;
	m_brsBG.CreateSolidBrush(COLOR_GRAY);

	LoadConfig();

	if (m_bSoundful) 
		LoadWave();

	InitGame();
}

CMineWnd::~CMineWnd()
{
	FreeMines();
	FreeWave();
	SaveConfig();
}

BEGIN_MESSAGE_MAP(CMineWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_INITMENU()
	ON_WM_CLOSE()
	ON_COMMAND(IDM_START, OnMemuStart)
	ON_COMMAND(IDM_PRIMARY, OnMemuPrimary)
	ON_COMMAND(IDM_SECOND, OnMemuSecond)
	ON_COMMAND(IDM_ADVANCE, OnMemuAdvance)
	ON_COMMAND(IDM_CUSTOM, OnMemuCustom)
	ON_COMMAND(IDM_MARK, OnMemuMark)
	ON_COMMAND(IDM_COLOR, OnMemuColor)
	ON_COMMAND(IDM_SOUND, OnMemuSound)
	ON_COMMAND(IDM_EXIT, OnMemuExit)
	ON_COMMAND(IDM_HELP_LIST, OnMemuHelpList)
	ON_COMMAND(IDM_HELP_FIND, OnMemuHelpFind)
	ON_COMMAND(IDM_HELP_USE, OnMemuHelpUse)
	ON_COMMAND(IDM_ABOUT, OnMemuAbout)	
	ON_COMMAND(IDM_HERO, OnMemuHero)
	ON_COMMAND(IDM_CHEAT, OnMemuCheat)
END_MESSAGE_MAP()

//��ȡ�����ļ�·��
CString CMineWnd::GetCfgPath()
{
	static CString strCfgPath;

	if (strCfgPath.IsEmpty())
	{
		TCHAR strExePath[MAX_PATH];
		GetModuleFileName(nullptr, strExePath, MAX_PATH);

		strCfgPath = strExePath;
		strCfgPath = strCfgPath.Left(strCfgPath.ReverseFind('\\'));
		strCfgPath += "\\config.ini";
	}	

	return strCfgPath;
}


//��������
void CMineWnd::LoadConfig()
{
	CString strCfgPath = GetCfgPath();	

	m_uXNum = GetPrivateProfileInt(TEXT("WINDOW"), TEXT("XNum"), 10, strCfgPath);
	m_uYNum = GetPrivateProfileInt(TEXT("WINDOW"), TEXT("YNum"), 10, strCfgPath);

	m_uMineNum = GetPrivateProfileInt(TEXT("MINE"), TEXT("MineNum"), 10, strCfgPath);
	m_uLevel = GetPrivateProfileInt(TEXT("MINE"), TEXT("Level"), 0, strCfgPath);

	m_uPrimary = GetPrivateProfileInt(TEXT("SCORE"), TEXT("Primary"), 999, strCfgPath);
	m_uSecond = GetPrivateProfileInt(TEXT("SCORE"), TEXT("Second"), 999, strCfgPath);
	m_uAdvance = GetPrivateProfileInt(TEXT("SCORE"), TEXT("Advance"), 999, strCfgPath);

	TCHAR buf[128] = {0};
	GetPrivateProfileString(TEXT("HOLDER"), TEXT("PrimaryHolder"), TEXT("����"), buf, 128, strCfgPath);
	m_strPrimary = buf;

	memset(buf, 0, sizeof buf);
	GetPrivateProfileString(TEXT("HOLDER"), TEXT("SecondHolder"), TEXT("����"), buf, 128, strCfgPath);
	m_strSecond = buf;

	memset(buf, 0, sizeof buf);
	GetPrivateProfileString(TEXT("HOLDER"), TEXT("AdvanceHolder"), TEXT("����"), buf, 128, strCfgPath);
	m_strSecond = buf;
	
	m_bMarkful = GetPrivateProfileInt(TEXT("CONFIG"), TEXT("Markful"), 0, strCfgPath);
	m_bColorful = GetPrivateProfileInt(TEXT("CONFIG"), TEXT("Colorful"), 1, strCfgPath);
	m_bSoundful = GetPrivateProfileInt(TEXT("CONFIG"), TEXT("Soundful"), 1, strCfgPath);
}

// ��������
void CMineWnd::SaveConfig()
{
	CString strCfgPath = GetCfgPath();

	//����������
	CString strXNum, strYNum;
	//���� ѡ�񼶱�
	CString strMineNum, strLevel;
	//��¼���ַ���
	CString strPrimary, strSecond, strAdvance;
	//��¼������
	CString strPrimaryHolder, strSecondHolder, strAdvanceHolder;
	//��������
	CString strMarkful, strColorful, strSoundful;

	strXNum.Format(TEXT("%u"), m_uXNum);
	strYNum.Format(TEXT("%u"), m_uYNum);

	strMineNum.Format(TEXT("%u"), m_uMineNum);
	strLevel.Format(TEXT("%u"), m_uLevel);

	strPrimary.Format(TEXT("%u"), m_uPrimary);
	strSecond.Format(TEXT("%u"), m_uSecond);
	strAdvance.Format(TEXT("%u"), m_uAdvance);

	strPrimaryHolder.Format(TEXT("%s"), m_strPrimary);
	strSecondHolder.Format(TEXT("%s"), m_strSecond);
	strAdvanceHolder.Format(TEXT("%s"), m_strAdvance);

	strMarkful.Format(TEXT("%d"), m_bMarkful);
	strColorful.Format(TEXT("%d"), m_bColorful);
	strSoundful.Format(TEXT("%d"), m_bSoundful);

	WritePrivateProfileString(TEXT("WINDOW"), TEXT("XNum"), strXNum, strCfgPath);
	WritePrivateProfileString(TEXT("WINDOW"), TEXT("YNum"), strYNum, strCfgPath);

	WritePrivateProfileString(TEXT("MINE"), TEXT("MineNum"), strMineNum, strCfgPath);
	WritePrivateProfileString(TEXT("MINE"), TEXT("Level"), strLevel, strCfgPath);

	WritePrivateProfileString(TEXT("SCORE"), TEXT("Primary"), strPrimary, strCfgPath);
	WritePrivateProfileString(TEXT("SCORE"), TEXT("Second"), strSecond, strCfgPath);
	WritePrivateProfileString(TEXT("SCORE"), TEXT("Advance"), strAdvance, strCfgPath);

	WritePrivateProfileString(TEXT("HOLDER"), TEXT("PrimaryHolder"), strPrimaryHolder, strCfgPath);
	WritePrivateProfileString(TEXT("HOLDER"), TEXT("SecondHolder"), strSecondHolder, strCfgPath);
	WritePrivateProfileString(TEXT("HOLDER"), TEXT("AdvanceHolder"), strAdvanceHolder, strCfgPath);

	WritePrivateProfileString(TEXT("CONFIG"), TEXT("Markful"), strMarkful, strCfgPath);
	WritePrivateProfileString(TEXT("CONFIG"), TEXT("Colorful"), strColorful, strCfgPath);
	WritePrivateProfileString(TEXT("CONFIG"), TEXT("Soundful"), strSoundful, strCfgPath);
}

//����λͼ
void CMineWnd::LoadBitmap()
{
	m_bmpMine.DeleteObject();
	m_bmpNumber.DeleteObject();
	m_bmpButton.DeleteObject();

	if (m_bColorful)
	{
		m_clrDark = COLOR_DARK_GRAY;
		
		m_bmpMine.LoadBitmap(IDB_MINE_COLOR);		
		m_bmpNumber.LoadBitmap(IDB_NUM_COLOR);		
		m_bmpButton.LoadBitmap(IDB_BTN_COLOR);
	}
	else
	{
		m_clrDark = COLOR_BLACK;
		
		m_bmpMine.LoadBitmap(IDB_MINE_GRAY);		
		m_bmpNumber.LoadBitmap(IDB_NUM_GRAY);		
		m_bmpButton.LoadBitmap(IDB_BTN_GRAY);
	}
}

//��ʼ��
void CMineWnd::InitGame()
{	
	LoadBitmap();//��ʼ��λͼ

	m_nLeftNum = m_uMineNum;
	m_uSpendTime = 0;
	m_uBtnState = BUTTON_NORMAL;
	m_uGameState = GS_WAIT;

	if (m_uTimer)
	{
		KillTimer(ID_TIMER_EVENT);
		m_uTimer = 0;
	}

	m_pNewMine = nullptr;
	m_pOldMine = nullptr;

	FreeMines();
	//��ʼ����ͼ
	for (UINT i = 0; i < m_uYNum; i++)
	{
		for (UINT j = 0; j < m_uXNum; j++)
		{
			m_pMines[i][j].uRow = i;
			m_pMines[i][j].uCol = j;
			m_pMines[i][j].uState = STATE_NORMAL;
			m_pMines[i][j].uAttrib = ATTRIB_EMPTY;
			m_pMines[i][j].uOldState = STATE_NORMAL;
		}
	}
}

//����
void CMineWnd::LayMines(UINT row, UINT col)
{
	//�����������
	srand((unsigned)time(nullptr));
	UINT i, j;

	for (UINT index = 0; index < m_uMineNum;)//��m_uMineNum����
	{
		//ȡ�����
		i = rand() % m_uYNum;
		j = rand() % m_uXNum;

		if (i == row && j == col) continue;
		if (m_pMines[i][j].uAttrib != ATTRIB_MINE)
		{
			m_pMines[i][j].uAttrib = ATTRIB_MINE;//�޸�����Ϊ��

			index++;
		}
	}
}

//�׷�����չ(������Χ���׵Ŀհ�����)
void CMineWnd::ExpandMines(UINT row, UINT col)
{
	UINT i, j;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;
	UINT around = GetAroundNum(row, col);


	m_pMines[row][col].uState = 15 - around;
	m_pMines[row][col].uOldState = 15 - around;

	// ���򿪡��������ػ�
	DrawSpecialMine(row, col);

	//����Χһ���׶�û�еĿհ�����
	if (around == 0)
	{
		for (i = minRow; i < maxRow; i++)
		{
			for (j = minCol; j < maxCol; j++)
			{//������Χ������չ��������еĹ���չ			
				if (!(i == row && j == col) &&
					m_pMines[i][j].uState == STATE_NORMAL
					&& m_pMines[i][j].uAttrib != ATTRIB_MINE)
				{

					if (!IsInMineArea(i, j)) continue;
					ExpandMines(i, j);
				}
			}
		}
	}
}

//  ��ȡĳ��С������������8��������׸���
UINT CMineWnd::GetAroundNum(UINT row, UINT col)
{
	UINT i, j;
	UINT around = 0;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;

	for (i = minRow; i < maxRow; i++)
	{
		for (j = minCol; j < maxCol; j++)
		{
			if (!IsInMineArea(i, j)) continue;
			if (m_pMines[i][j].uAttrib == ATTRIB_MINE) around++;
		}
	}
	return around;
}

//��ȡĳ��С������������8��������ѱ�־״̬��
UINT CMineWnd::GetAroundFlags(UINT row, UINT col)
{
	UINT i, j;
	UINT flags = 0;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;

	for (i = minRow; i < maxRow; i++)
	{
		for (j = minCol; j < maxCol; j++)
		{
			if (!IsInMineArea(i, j)) continue;
			if (m_pMines[i][j].uState == STATE_FLAG) flags++;
		}
	}

	return flags;
}

//�ж��Ƿ�Ϊ��
BOOL CMineWnd::IsMine(UINT row, UINT col)
{
	return (m_pMines[row][col].uAttrib == ATTRIB_MINE);
}

//�ж��Ƿ���������
BOOL CMineWnd::IsInMineArea(UINT row, UINT col)
{
	return (row >= 0 && row < m_uYNum && col >= 0 && col < m_uXNum);
}

//ʧ�ܴ���
void CMineWnd::Dead(UINT row, UINT col)
{
	//��ť���ڵ�����
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
	//�������ڵ�����
	CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP,
		MINE_AREA_LEFT + m_uXNum * MINE_WIDTH,
		MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);

	UINT i, j;

	if (m_pMines[row][col].uAttrib == ATTRIB_MINE)
	{//ʧ�ܣ���������
		for (i = 0; i < m_uYNum; i++)
		{
			for (j = 0; j < m_uXNum; j++)
			{
				m_pMines[row][col].uState = STATE_BLAST;
				m_pMines[row][col].uOldState = STATE_BLAST;
				if (m_pMines[i][j].uAttrib == ATTRIB_MINE
					&& m_pMines[i][j].uState != STATE_FLAG)
				{
					m_pMines[i][j].uState = STATE_MINE;
					m_pMines[i][j].uOldState = STATE_MINE;
				}
			}
		}
	}
	else {//ʧ�ܣ���������
		for (i = 0; i < m_uYNum; i++)
		{
			for (j = 0; j < m_uXNum; j++)
			{
				m_pMines[row][col].uState = STATE_ERROR;
				m_pMines[row][col].uOldState = STATE_ERROR;
				if (m_pMines[i][j].uAttrib == ATTRIB_MINE
					&& m_pMines[i][j].uState != STATE_FLAG)
				{
					m_pMines[i][j].uState = STATE_MINE;
					m_pMines[i][j].uOldState = STATE_MINE;
				}
			}
		}
	}
	//ʧ�ܴ���
	InvalidateRect(rcMineArea);
	m_uBtnState = BUTTON_DEAD;
	InvalidateRect(rcBtn);
	m_uGameState = GS_DEAD;

	if (m_uTimer != 0)
	{//����ʱ��ȥ����
		KillTimer(ID_TIMER_EVENT);
		m_uTimer = 0;
	}

	if (m_bSoundful)
	{//ʧ����������
		sndPlaySound((LPCTSTR)LockResource(m_pSndDead), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
	}
}

//ʤ���жϲ�����
BOOL CMineWnd::Victory()
{
	UINT i, j;
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);

	// ����Ƿ�ʤ��
	for (i = 0; i < m_uYNum; i++)
	{
		for (j = 0; j < m_uXNum; j++)
		{
			if (m_pMines[i][j].uState == STATE_NORMAL) return FALSE;
			if (m_pMines[i][j].uState == STATE_DICEY) return FALSE;
		}
	}

	//ʤ������ʤ������
	m_uBtnState = BUTTON_VICTORY;
	m_uGameState = GS_VICTORY;
	Invalidate();
	if (m_uTimer != 0)
	{//����ʱ��ȥ����
		KillTimer(ID_TIMER_EVENT);
		m_uTimer = 0;
	}
	//ʤ������
	if (m_bSoundful)
	{
		sndPlaySound((LPCTSTR)LockResource(m_pSndVictory),
			SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
	}

	//Ӣ�۰�
	UINT uRecord;
	if (m_uLevel == LEVEL_PRIMARY) uRecord = m_uPrimary;
	else if (m_uLevel == LEVEL_SECONDRY) uRecord = m_uSecond;
	else if (m_uLevel == LEVEL_ADVANCE) uRecord = m_uAdvance;
	if (uRecord > m_uSpendTime)
	{
		CNewRecordDlg dlg;
		dlg.SetLevel(m_uLevel);
		dlg.DoModal();
		switch (m_uLevel)
		{
		case LEVEL_PRIMARY:
			m_uPrimary = m_uSpendTime;
			m_strPrimary = dlg.GetName();
			break;
		case LEVEL_SECONDRY:
			m_uSecond = m_uSpendTime;
			m_strSecond = dlg.GetName();
			break;
		case LEVEL_ADVANCE:
			m_uAdvance = m_uSpendTime;
			m_strAdvance = dlg.GetName();
			break;
		case LEVEL_CUSTOM:
			return TRUE;
		default:
			break;
		}
		OnMemuHero();
	}

	return TRUE;
}

//����Ҽ�������ԭ�Ȳ�ͬ�ı����״̬ѭ���޸ģ��Ա��û������޸���ԭ�ȱ�־
void CMineWnd::OnLRBtnDown(UINT row, UINT col)
{
	UINT i, j;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;

	for (i = minRow; i < maxRow; i++)
	{
		for (j = minCol; j < maxCol; j++)
		{
			if (!IsInMineArea(i, j)) continue;
			//			if (i == row && j == col) continue;
			if (m_pMines[i][j].uState == STATE_NORMAL)
			{
				m_pMines[i][j].uState = STATE_EMPTY;
			}
			else if (m_pMines[i][j].uState == STATE_DICEY)
			{
				m_pMines[i][j].uState = STATE_DICEY_DOWN;
			}
		}
	}
}

void CMineWnd::OnLRBtnUp(UINT row, UINT col)
{
	UINT i, j;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;

	for (i = minRow; i < maxRow; i++)
	{
		for (j = minCol; j < maxCol; j++)
		{
			if (!IsInMineArea(i, j)) continue;
			//			if (i == row && j == col) continue;
			if (m_pMines[i][j].uOldState == STATE_NORMAL)
			{
				m_pMines[i][j].uState = STATE_NORMAL;
			}
			else if (m_pMines[i][j].uOldState == STATE_DICEY)
			{
				m_pMines[i][j].uState = STATE_DICEY;
			}
		}
	}
	//	Invalidate();
}

//չ����չ��Χ8������
void CMineWnd::OpenAround(UINT row, UINT col)
{
	//�����Χ���ڵı�־���� != ��Χ���ڵ����� �򷵻�
	if (GetAroundFlags(row, col) != GetAroundNum(row, col)) return;

	UINT i, j;
	UINT around = 0;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;

	for (i = minRow; i < maxRow; i++)
	{
		for (j = minCol; j < maxCol; j++)
		{
			if (!IsInMineArea(i, j)) continue;
			if (m_pMines[i][j].uState == STATE_NORMAL)
			{//���������Ϊ��������

				//��չ������
				ExpandMines(i, j);
				around = GetAroundNum(i, j);
				m_pMines[i][j].uState = 15 - around;
				m_pMines[i][j].uOldState = 15 - around;
			}
		}
	}

	// �ж��Ƿ�ʤ�������򽫵�ͼ�������ױ�ʶ����
	if (Victory())
	{
		for (i = 0; i < m_uYNum; i++)
		{
			for (j = 0; j < m_uXNum; j++)
			{
				if (m_pMines[i][j].uAttrib == ATTRIB_MINE)
				{
					m_pMines[i][j].uState = STATE_FLAG;
					m_pMines[i][j].uOldState = STATE_FLAG;
				}
			}
		}
		m_nLeftNum = 0;
		Invalidate();
	}
}

BOOL CMineWnd::ErrorAroundFlag(UINT row, UINT col)
{
	//�����Χ���ڵı�־���� != ��Χ���ڵ����� �򷵻�
	if (GetAroundFlags(row, col) != GetAroundNum(row, col)) return FALSE;

	UINT i, j;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;

	for (i = minRow; i < maxRow; i++)
	{
		for (j = minCol; j < maxCol; j++)
		{
			if (!IsInMineArea(i, j)) continue;
			if (m_pMines[i][j].uState == STATE_FLAG)
			{
				if (m_pMines[i][j].uAttrib != ATTRIB_MINE)
				{
					Dead(i, j);
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

void CMineWnd::OpenByCheat()
{
	UINT i, j;
	for (i = 0; i < m_uYNum; i++)
	{
		for (j = 0; j < m_uXNum; j++)
		{
			if (m_pMines[i][j].uState == STATE_FLAG) continue;
			OpenAround(i, j);
		}
	}
}

void CMineWnd::FreeMines()
{
	//	if (m_pMines) {
	//		delete[] m_pMines;
	//		m_pMines = nullptr;
	//	}
	//	for (UINT row = 0; row<m_uYNum; row++) {
	//		if (m_pMines[row]) {
	//			delete []m_pMines[m_uXNum];
	//			delete m_pMines[row];
	//		}
	//	}
}

void CMineWnd::SizeWindow(void)
{
	UINT uWidth = DEFAULT_FRAME_X + m_uXNum * MINE_WIDTH + 4 +
		LINE_WIDTH_0 * 3 + SIDE_WIDTH_0 + SIDE_WIDTH_1;
	UINT uHeight = DEFAULT_FRAME_Y + m_uYNum * MINE_HEIGHT + 4 +
		LINE_WIDTH_0 * 3 + SIDE_WIDTH_0 * 2 + SIDE_WIDTH_1 + SHELL_S_H;
	SetWindowPos(&wndTopMost, 0, 0, uWidth, uHeight,
		SWP_NOZORDER | SWP_NOMOVE | SWP_NOCOPYBITS);	// SWP_NOCOPYBITS does not do function������

	GetClientRect(&m_rcClient);
	// Button Position
	m_uBtnRect[0] = m_rcClient.right / 2 - 12;			// the left side of the start(state) button without 3d border
	m_uBtnRect[1] = m_rcClient.right / 2 - 13;			// the left side of the start(state) button with 3d border
	m_uBtnRect[2] = m_rcClient.right / 2 + 12;			// the right side of the start(state) button 

	// Number Position
	m_uNumRect[0] = m_rcClient.right - 55;				// the left side of the spend time rect	with 3d border
	m_uNumRect[1] = m_rcClient.right - 15;				// the right side of the spend time rect
	m_uNumRect[2] = m_rcClient.right - 54;				// the left side of the spend time rect without 3d border
	//	m_uNumRect[3] = m_rcClient.right - 41;
	//	m_uNumRect[4] = m_rcClient.right - 28;	

	// Shell Position
	m_uShellRcX[0] = m_rcClient.right;					//
	m_uShellRcX[1] = m_rcClient.right - 10;				// the width of the small(big) shell

	m_uShellRcY[0] = m_rcClient.bottom;					// 
	m_uShellRcY[1] = m_rcClient.bottom - SHELL_L_START_Y - 3;		// the height of the big shell
}

//����Ц����ťͼ
void CMineWnd::DrawButton(CPaintDC &dc)
{
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_bmpButton);
	dc.StretchBlt(m_uBtnRect[0], 16, 24, 24, &cdc, 0, 24 * m_uBtnState, 24, 24, SRCCOPY);
	dc.Draw3dRect(m_uBtnRect[1], 15, 26, 26, m_clrDark, m_clrDark);
}

// ��������
void CMineWnd::DrawNumber(CPaintDC &dc)
{
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_bmpNumber);

	dc.Draw3dRect(16, 15, 41, 25, m_clrDark, COLOR_WHITE);
	dc.Draw3dRect(m_uNumRect[0], 15, 41, 25, m_clrDark, COLOR_WHITE);

	//����λͼ�ߴ磺13 * 276��276 == 23 * 12
	//����ʣ������
	int num = (m_nLeftNum < 0) ? 11 : m_nLeftNum / 100;
	dc.StretchBlt(17, 16, 13, 23, &cdc, 0, 276 - 23 * (num + 1), 13, 23, SRCCOPY);
	num = (m_nLeftNum < 0) ? -(m_nLeftNum - num * 100) / 10 : (m_nLeftNum - num * 100) / 10;
	dc.StretchBlt(30, 16, 13, 23, &cdc, 0, 276 - 23 * (num + 1), 13, 23, SRCCOPY);
	num = (m_nLeftNum < 0) ? -m_nLeftNum % 10 : m_nLeftNum % 10;
	dc.StretchBlt(43, 16, 13, 23, &cdc, 0, 276 - 23 * (num + 1), 13, 23, SRCCOPY);

	//����ʱ��
	num = m_uSpendTime / 100;
	dc.StretchBlt(m_uNumRect[0], 16, 13, 23, &cdc, 0, 276 - 23 * (num + 1), 13, 23, SRCCOPY);
	num = (m_uSpendTime - num * 100) / 10;
	dc.StretchBlt(m_uNumRect[0] + 13, 16, 13, 23, &cdc, 0, 276 - 23 * (num + 1), 13, 23, SRCCOPY);
	num = m_uSpendTime % 10;
	dc.StretchBlt(m_uNumRect[0] + 26, 16, 13, 23, &cdc, 0, 276 - 23 * (num + 1), 13, 23, SRCCOPY);
}

//�������
void CMineWnd::DrawShell(CPaintDC &dc)
{
	// �滭2������
	//dc.FillSolidRect(0, 0, m_uShellRcX[0], LINE_WIDTH_0, COLOR_WHITE);
	//dc.FillSolidRect(0, 0, LINE_WIDTH_0, m_uShellRcY[0], COLOR_WHITE);

	// ��С�����
	dc.Draw3dRect(/*SHELL_S_START_X, SHELL_S_START_Y,*/5, SHELL_S_START_Y,
		m_uShellRcX[1], SHELL_S_H, m_clrDark, COLOR_WHITE);
	/*dc.Draw3dRect(SHELL_S_START_X + 1, SHELL_S_START_Y + 1,
		m_uShellRcX[1] - 2, SHELL_S_H - 2, m_clrDark, COLOR_WHITE);*/

	// ��������
	dc.Draw3dRect(/*SHELL_L_START_X, SHELL_L_START_Y,*/5, SHELL_L_START_Y,
		m_uShellRcX[1], m_uShellRcY[1], m_clrDark, COLOR_WHITE);
	/*dc.Draw3dRect(SHELL_L_START_X + 1, SHELL_L_START_Y + 1,
		m_uShellRcX[1] - 2, m_uShellRcY[1] - 2, m_clrDark, COLOR_WHITE);
	dc.Draw3dRect(SHELL_L_START_X + 2, SHELL_L_START_Y + 2,
		m_uShellRcX[1] - 4, m_uShellRcY[1] - 4, m_clrDark, COLOR_WHITE);*/
}

//��������
void CMineWnd::DrawMineArea(CPaintDC &dc)
{
	CDC dcMemory; //�����ڴ��豸
	dcMemory.CreateCompatibleDC(&dc); //ʹ������豸��dc����
	dcMemory.SelectObject(m_bmpMine); //���ڴ��豸��λͼ��Դ����

	for (UINT i = 0; i < m_uYNum; i++)
	{
		for (UINT j = 0; j < m_uXNum; j++)
		{//����[i][j]������׷���״̬������Ӧ��ͼ��[i][j]�������ض�����
			dc.StretchBlt(MINEAREA_FRAME_X + 16 * j, MINEAREA_FRAME_Y + 16 * i,
				16, 16, &dcMemory, 0, 16 * m_pMines[i][j].uState, 16, 16, SRCCOPY);
		}
	}
}

//���ư���ɨ�׺������
void CMineWnd::DrawDownNum(MINEWND* mine, UINT num)
{
	mine->uState = 15 - num;
	mine->uOldState = 15 - num;
	CRect rcMine(mine->uCol * 16, mine->uRow * 16, (mine->uCol + 1) * 16, (mine->uRow + 1) * 16);
	InvalidateRect(rcMine);
}

//���»����������ĳ������
void CMineWnd::DrawSpecialMine(UINT row, UINT col)
{
	CRect rcMine(col * 16, row * 16, (col + 1) * 16, (row + 1) * 16);
	InvalidateRect(rcMine);
}

//��ȡ��ͼ��(x,y)���������Ϣ
CMineWnd::MINEWND* CMineWnd::GetMine(long x, long y)
{
	//��֤�����ϸ�
	if (x < MINEAREA_FRAME_X || y < MINEAREA_FRAME_Y)
	{
		return nullptr;
	}

	//��������ֵ�����С�������ڵ�ͼ���к���
	UINT uCol = (UINT)(x - MINEAREA_FRAME_X) / 16;
	UINT uRow = (UINT)(y - MINEAREA_FRAME_Y) / 16;
	//���ظ����������Ϣ
	return &m_pMines[uRow][uCol];
}

void CMineWnd::SetCheckedLevel()
{
	if (m_pSubMenu)
	{
		m_pSubMenu->CheckMenuItem(IDM_PRIMARY, MF_UNCHECKED | MF_BYCOMMAND);
		m_pSubMenu->CheckMenuItem(IDM_SECOND, MF_UNCHECKED | MF_BYCOMMAND);
		m_pSubMenu->CheckMenuItem(IDM_ADVANCE, MF_UNCHECKED | MF_BYCOMMAND);
		m_pSubMenu->CheckMenuItem(IDM_CUSTOM, MF_UNCHECKED | MF_BYCOMMAND);
		switch (m_uLevel)
		{
		case LEVEL_PRIMARY:
			m_pSubMenu->CheckMenuItem(IDM_PRIMARY, MF_CHECKED | MF_BYCOMMAND);
			break;
		case LEVEL_SECONDRY:
			m_pSubMenu->CheckMenuItem(IDM_SECOND, MF_CHECKED | MF_BYCOMMAND);
			break;
		case LEVEL_ADVANCE:
			m_pSubMenu->CheckMenuItem(IDM_ADVANCE, MF_CHECKED | MF_BYCOMMAND);
			break;
		case LEVEL_CUSTOM:
			m_pSubMenu->CheckMenuItem(IDM_CUSTOM, MF_CHECKED | MF_BYCOMMAND);
			break;
		default:
			break;
		}
	}
}

void CMineWnd::SetCheckedMark()
{
	if (m_pSubMenu) {
		if (m_bMarkful) {
			m_pSubMenu->CheckMenuItem(IDM_MARK, MF_CHECKED | MF_BYCOMMAND);
		}
		else {
			m_pSubMenu->CheckMenuItem(IDM_MARK, MF_UNCHECKED | MF_BYCOMMAND);
		}
	}
}

void CMineWnd::SetCheckedColor()
{
	if (m_pSubMenu)
	{
		if (m_bColorful)
		{
			m_pSubMenu->CheckMenuItem(IDM_COLOR, MF_CHECKED | MF_BYCOMMAND);
		}
		else
		{
			m_pSubMenu->CheckMenuItem(IDM_COLOR, MF_UNCHECKED | MF_BYCOMMAND);
		}
	}
}

void CMineWnd::SetCheckedSound()
{
	if (m_pSubMenu)
	{
		if (m_bSoundful)
		{
			m_pSubMenu->CheckMenuItem(IDM_SOUND, MF_CHECKED | MF_BYCOMMAND);
		}
		else
		{
			m_pSubMenu->CheckMenuItem(IDM_SOUND, MF_UNCHECKED | MF_BYCOMMAND);
		}
	}
}

void CMineWnd::SetCheckedCheat()
{
	if (m_pSubMenu)
	{
		//		if (m_bCheat) 
		//		{
		//			m_pSubMenu->CheckMenuItem(IDM_CHEAT, MF_CHECKED | MF_BYCOMMAND);
		//		}
		//	else 
		//		{
		m_pSubMenu->CheckMenuItem(IDM_CHEAT, MF_UNCHECKED | MF_BYCOMMAND);
		//		}
	}
}

void CMineWnd::LoadWave()
{
	HMODULE hMdl = AfxGetResourceHandle();

	HRSRC hSrc = FindResource(hMdl, MAKEINTRESOURCE(IDR_WAVE_DEAD), TEXT("WAVE"));
	m_pSndDead = LoadResource(hMdl, hSrc);

	hSrc = FindResource(hMdl, MAKEINTRESOURCE(IDR_WAVE_VICTORY), TEXT("WAVE"));
	m_pSndVictory = LoadResource(hMdl, hSrc);

	hSrc = FindResource(hMdl, MAKEINTRESOURCE(IDR_WAVE_CLOCK), TEXT("WAVE"));
	m_pSndClock = LoadResource(hMdl, hSrc);
}

void CMineWnd::FreeWave()
{
	if (m_pSndDead)
	{
		FreeResource(m_pSndDead);
		m_pSndDead = nullptr;
	}

	if (m_pSndVictory)
	{
		FreeResource(m_pSndVictory);
		m_pSndVictory = nullptr;
	}

	if (m_pSndClock)
	{
		FreeResource(m_pSndClock);
		m_pSndClock = nullptr;
	}
}

void CMineWnd::ResetRecord()
{
	m_uPrimary = DEFAULT_RECORD;
	m_strPrimary = DEFAULT_HOLDER;
	m_uSecond = DEFAULT_RECORD;
	m_strSecond = DEFAULT_HOLDER;
	m_uAdvance = DEFAULT_RECORD;
	m_strAdvance = DEFAULT_HOLDER;
}

void CMineWnd::SetCustom(UINT xNum, UINT yNum, UINT mNum)
{
	m_uXNum = xNum;
	m_uYNum = yNum;
	m_uMineNum = mNum;
}

void CMineWnd::OnPaint()
{
	CPaintDC dc(this);    // ������Ļ��ʾ���豸
	CDC dcMemory;  // �ڴ��豸

	CBitmap bitmap;

	if (!dc.IsPrinting())
	{
		// ��dc�豸����
		if (dcMemory.CreateCompatibleDC(&dc))
		{
			// ʹ��bitmap��ʵ����ʾ���豸����
			if (bitmap.CreateCompatibleBitmap(&dc, m_rcClient.right, m_rcClient.bottom))
			{
				// �ڴ��豸ѡ�������λͼ
				dcMemory.SelectObject(&bitmap);
				//���Ʊ�����
				dcMemory.FillRect(&m_rcClient, &m_brsBG);

				DrawButton((CPaintDC&)dcMemory);//Ц����ť��ͼ
				DrawNumber((CPaintDC&)dcMemory);//���ֻ�ͼ(��ʱ�����ֺ�ʣ����������)
				DrawShell((CPaintDC&)dcMemory);//3DЧ����ǻ�ͼ
				DrawMineArea((CPaintDC&)dcMemory);//������ͼ


				// ���ڴ��豸�����ݿ�����ʵ����Ļ��ʾ���豸
				dc.BitBlt(m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom, &dcMemory, 0, 0, SRCCOPY);
				bitmap.DeleteObject();
			}
		}
	}
}

void CMineWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	SizeWindow();
	CWnd::OnShowWindow(bShow, nStatus);
}

void CMineWnd::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == ID_TIMER_EVENT)
	{
		if (m_bSoundful)
		{
			sndPlaySound((LPCTSTR)LockResource(m_pSndClock), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
		}
		m_uSpendTime++;
		CRect rcNum(m_uNumRect[0], 15, m_uNumRect[1], 39);
		//	InvalidateRect(rcNum);
		Invalidate();
		if (m_uSpendTime >= DEFAULT_RECORD)
		{
			KillTimer(ID_TIMER_EVENT);
			m_uTimer = 0;
		}
	}

	CWnd::OnTimer(nIDEvent);
}

void CMineWnd::OnLButtonDown(UINT nFlags, CPoint pt)
{
	//��ť���ڵ�����
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
	//�������ڵ�����
	CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP,
		MINE_AREA_LEFT + m_uXNum * MINE_WIDTH, MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);

	SetCapture();//����Ƴ�����Ҳ��������Ϣ

	m_bClickBtn = FALSE;
	m_bLRBtnDown = FALSE;

	if (rcBtn.PtInRect(pt))
	{// click in the button area
		m_bClickBtn = TRUE;
		m_uBtnState = BUTTON_DOWN;
		InvalidateRect(rcBtn);
	}
	else if (rcMineArea.PtInRect(pt))
	{// click in the mine area
		// change mine state by gamestate
		switch (m_uGameState)
		{
		case GS_WAIT: case GS_RUN:
			m_pNewMine = GetMine(pt.x, pt.y);
			if (!m_pNewMine) return;
			if (m_pNewMine->uState == STATE_NORMAL)
			{
				m_pNewMine->uState = STATE_EMPTY;
			}
			if (m_pNewMine->uState == STATE_DICEY)
			{
				m_pNewMine->uState = STATE_DICEY_DOWN;
			}
			m_pOldMine = m_pNewMine;
			break;
		case GS_DEAD: case GS_VICTORY:
			return;
			break;
		default:
			break;
		}
		m_uBtnState = BUTTON_CLICK;
		InvalidateRect(rcBtn);
		// both of the left button and the right button are pushing down 
		if (nFlags == (MK_LBUTTON | MK_RBUTTON))
		{
			m_bLRBtnDown = TRUE;
			OnLRBtnDown(m_pOldMine->uRow, m_pOldMine->uCol);
		}
		InvalidateRect(rcMineArea);
	}
	else
	{											// click in other area
		if (m_uGameState == GS_WAIT || m_uGameState == GS_RUN)
		{
			m_uBtnState = BUTTON_CLICK;
			InvalidateRect(rcBtn);
		}
	}

	CWnd::OnLButtonDown(nFlags, pt);
}

void CMineWnd::OnLButtonUp(UINT nFlags, CPoint pt)
{
	//Ц��ͼ��ť���ڵ�����
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
	//�������ڵ�����
	CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP,
		MINE_AREA_LEFT + m_uXNum * MINE_WIDTH,
		MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);

	if (rcBtn.PtInRect(pt))
	{// ���Ц��ͼ
		Invalidate();
		InitGame();
	}
	else if (rcMineArea.PtInRect(pt))
	{//���������
		CString value;
		UINT around = 0;

		//���ݲ�ͬ����Ϸ״̬������
		switch (m_uGameState)
		{
			//��Ϸ����״̬
		case GS_WAIT: case GS_RUN:
			// first get the MINEWND which if pushing down
			m_pOldMine = GetMine(pt.x, pt.y);
			if (!m_pOldMine)
			{
				ReleaseCapture();
				return;
			}
			// do normal process
			// judge whether the lr button are both pushed down
			//����жϵ�ǰ״̬�Ƿ�Ϊ�������ͬʱ����
			if (m_bLRBtnDown)
			{
				m_bLRBtnDown = FALSE;
				OnLRBtnUp(m_pOldMine->uRow, m_pOldMine->uCol);
				if (m_uGameState == GS_WAIT)
				{
					m_uBtnState = BUTTON_NORMAL;
					Invalidate();
					ReleaseCapture();
					return;
				}
				// if the around flags number equal to the around mines number, expand.
				//������Χ�Ѿ���ʶ���ף���Χ��������������չ
				if (m_pOldMine->uState != STATE_FLAG)
				{
					OpenAround(m_pOldMine->uRow, m_pOldMine->uCol);
				}
				// check whether the MINEWND around the special MINEWND is a mine, if it is then dead.
				if (ErrorAroundFlag(m_pOldMine->uRow, m_pOldMine->uCol))
				{

					Dead(m_pOldMine->uRow, m_pOldMine->uCol);
					ReleaseCapture();
					return;
				}
			}
			else
			{
				// start the game, init the mines area
				//�����Ϸ��δ��ʼ��������������Ϸ
				if (m_uGameState == GS_WAIT)
				{
					if (m_uTimer)
					{
						KillTimer(ID_TIMER_EVENT);
						m_uTimer = 0;
					}
					// the following five lines refresh the remining mine num rect immediately 
					// when click in the mine area at the first time
					m_uSpendTime = 1;
					Invalidate();
					if (m_bSoundful)
					{
						sndPlaySound((LPCTSTR)LockResource(m_pSndClock), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
					}
					//������ʱ��
					m_uTimer = SetTimer(ID_TIMER_EVENT, 1000, nullptr);
					//����
					LayMines(m_pOldMine->uRow, m_pOldMine->uCol);		// lay all the mines down 
					//�ı���Ϸ״̬ΪTEXT("����/GS_RUN")
					m_uGameState = GS_RUN;

				}

				if (m_pOldMine->uOldState == STATE_NORMAL)
				{//����������Ϊ����δ����ǲŴ�
					// first judge if the special MINEWND is a mine
					//���������Ϊ�ף�������
					if (IsMine(m_pOldMine->uRow, m_pOldMine->uCol))
					{
						Dead(m_pOldMine->uRow, m_pOldMine->uCol);
						ReleaseCapture();
						return;
					}
					// the special MINEWND is not a mine 
					//�����׵�ʱ�򣬻�ȡ����Χ������Ŀ
					around = GetAroundNum(m_pOldMine->uRow, m_pOldMine->uCol);
					// ���Ϊ�հ�������չ������򿪸�������ʾ��Χ�ж���������
					if (around == 0) ExpandMines(m_pOldMine->uRow, m_pOldMine->uCol);
					else DrawDownNum(m_pOldMine, around);
				}
				else if (m_pOldMine->uOldState == STATE_DICEY)
				{//��־Ϊ�������ʺŵ�ʱ��
					m_pOldMine->uState = STATE_DICEY;
				}

				//�ж��Ƿ�Ϊʤ��
				if (Victory())
				{
					Invalidate();
					ReleaseCapture();
					return;
				}
			}
			break;
		case GS_VICTORY:
		case GS_DEAD:
			ReleaseCapture();		// release the cursor
			return;
		default:
			break;
		}
		m_uBtnState = BUTTON_NORMAL;
		Invalidate();
	}
	else
	{//�����������
		if (m_uGameState == GS_WAIT || m_uGameState == GS_RUN)
		{
			m_uBtnState = BUTTON_NORMAL;
			InvalidateRect(rcBtn);
		}
	}

	ReleaseCapture();		// release the cursor
	CWnd::OnLButtonUp(nFlags, pt);
}

void CMineWnd::OnRButtonDown(UINT nFlags, CPoint pt)
{
	//Ц��ͼ��ť���ڵ�����
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
	//�������ڵ�����
	CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP,
		MINE_AREA_LEFT + m_uXNum * MINE_WIDTH,
		MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);

	m_bLRBtnDown = FALSE;

	if (rcMineArea.PtInRect(pt))
	{//���������			
		if (m_uGameState == GS_WAIT || m_uGameState == GS_RUN)
		{
			m_pNewMine = GetMine(pt.x, pt.y);
			if (!m_pNewMine) return;
			// both of the left button and the right button are pushing down 
			if (nFlags == (MK_LBUTTON | MK_RBUTTON))
			{
				m_bLRBtnDown = TRUE;
				OnLRBtnDown(m_pNewMine->uRow, m_pNewMine->uCol);
			}
			else
			{
				switch (m_pNewMine->uState)
				{
				case STATE_NORMAL:
					m_pNewMine->uState = STATE_FLAG;
					m_pNewMine->uOldState = STATE_FLAG;
					m_nLeftNum--;
					break;
				case STATE_FLAG:
					m_pNewMine->uState = STATE_DICEY;
					m_pNewMine->uOldState = STATE_DICEY;
					m_nLeftNum++;
					break;
				case STATE_DICEY:
					m_pNewMine->uState = STATE_NORMAL;
					m_pNewMine->uOldState = STATE_NORMAL;
					break;
				default:
					break;
				}
			}
			Invalidate();

		}
	}

	CWnd::OnRButtonDown(nFlags, pt);
}

void CMineWnd::OnRButtonUp(UINT nFlags, CPoint pt)
{
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
	CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP,
		MINE_AREA_LEFT + m_uXNum * MINE_WIDTH, MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);

	m_pOldMine = GetMine(pt.x, pt.y);
	if (!m_pOldMine) return;
	// judge whether the lr button are both pushed down
	if (m_bLRBtnDown)
	{
		m_bLRBtnDown = FALSE;
		OnLRBtnUp(m_pOldMine->uRow, m_pOldMine->uCol);
		if (m_uGameState == GS_WAIT)
		{
			m_uBtnState = BUTTON_NORMAL;
			Invalidate();
			return;
		}
		// if the around flags number equal to the around mines number, expand.
		if (m_pOldMine->uState != STATE_FLAG)
		{
			OpenAround(m_pOldMine->uRow, m_pOldMine->uCol);
		}
		// check whether the MINEWND around the special MINEWND is a mine, if it is then dead.
		if (ErrorAroundFlag(m_pOldMine->uRow, m_pOldMine->uCol))
		{
			//			Dead(m_pOldMine->uRow, m_pOldMine->uCol);
			//			ReleaseCapture();
			return;
		}
	}
	else
	{
		Victory();
	}

	CWnd::OnRButtonUp(nFlags, pt);
}

void CMineWnd::OnMouseMove(UINT nFlags, CPoint pt)
{
	if (nFlags == MK_LBUTTON || nFlags == (MK_LBUTTON | MK_RBUTTON))
	{
		CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
		CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP,
			MINE_AREA_LEFT + m_uXNum * MINE_WIDTH, MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);

		if (rcBtn.PtInRect(pt))
		{				// pt in button area
			switch (m_uGameState)
			{
			case GS_RUN:
				m_uBtnState = (m_bClickBtn) ? BUTTON_DOWN : BUTTON_CLICK;
				break;
			case GS_DEAD: case GS_VICTORY:
				if (m_bClickBtn) m_uBtnState = BUTTON_DOWN;
				break;
			default:
				break;
			}
			InvalidateRect(rcBtn);
		}
		else if (rcMineArea.PtInRect(pt))
		{		// pt in mine area
			switch (m_uGameState)
			{
			case GS_RUN:
				m_pNewMine = GetMine(pt.x, pt.y);
				if (!m_pNewMine || !m_pOldMine) return;
				if (m_pNewMine->uCol != m_pOldMine->uCol ||
					m_pNewMine->uRow != m_pOldMine->uRow)
				{
					// change the new mine rect state
					switch (m_pNewMine->uState)
					{
					case STATE_NORMAL:
						m_pNewMine->uState = STATE_EMPTY;
						break;
					case STATE_DICEY:
						m_pNewMine->uState = STATE_DICEY_DOWN;
						break;
					}
					// resume the old mine rect state
					switch (m_pOldMine->uOldState)
					{
					case STATE_NORMAL:
						m_pOldMine->uState = STATE_NORMAL;
						break;
					case STATE_DICEY:
						m_pOldMine->uState = STATE_DICEY;
						break;
					default:
						break;
					}
					// judge whether the lr button are pushed down
					if (m_bLRBtnDown)
					{
						OnLRBtnUp(m_pOldMine->uRow, m_pOldMine->uCol);
						OnLRBtnDown(m_pNewMine->uRow, m_pNewMine->uCol);
					}
					m_pOldMine = m_pNewMine;
				}
				InvalidateRect(rcMineArea);
				break;
			case GS_VICTORY: case GS_DEAD:
				return;
			default:
				break;
			}
		}
		else
		{										// pt in other area
			switch (m_uGameState)
			{
			case GS_RUN:
				m_uBtnState = (m_bClickBtn) ? BUTTON_NORMAL : BUTTON_CLICK;
				if (m_pNewMine)
				{
					if (m_pNewMine->uOldState == STATE_NORMAL)
					{
						m_pNewMine->uState = STATE_NORMAL;
					}
					else if (m_pNewMine->uOldState == STATE_DICEY)
					{
						m_pNewMine->uState = STATE_DICEY;
					}
				}
				break;
			case GS_DEAD:
				m_uBtnState = BUTTON_DEAD;
				break;
			case GS_VICTORY:
				m_uBtnState = BUTTON_VICTORY;
				break;
			default:
				break;
			}
			Invalidate();
		}
	}

	CWnd::OnMouseMove(nFlags, pt);
}

void CMineWnd::OnMemuStart()
{
	InitGame();
	Invalidate();
}

void CMineWnd::OnMemuPrimary()
{
	m_uLevel = LEVEL_PRIMARY;
	m_uXNum = PRIMARY_XNUM;
	m_uYNum = PRIMARY_YNUM;
	m_uMineNum = PRIMARY_MINENUM;

	SetCheckedLevel();
	InitGame();
	Invalidate();
	SizeWindow();
}

void CMineWnd::OnMemuSecond()
{
	m_uLevel = LEVEL_SECONDRY;
	m_uXNum = SECONDRY_XNUM;
	m_uYNum = SECONDRY_YNUM;
	m_uMineNum = SECONDRY_MINENUM;

	SetCheckedLevel();
	InitGame();
	Invalidate();
	SizeWindow();
}

void CMineWnd::OnMemuAdvance()
{
	m_uLevel = LEVEL_ADVANCE;
	m_uXNum = ADVANCE_XNUM;
	m_uYNum = ADVANCE_YNUM;
	m_uMineNum = ADVANCE_MINENUM;

	SetCheckedLevel();
	InitGame();
	Invalidate();
	SizeWindow();
}

void CMineWnd::OnMemuCustom()
{
	m_uLevel = LEVEL_CUSTOM;
	SetCheckedLevel();
	CCustomDlg dlg;
	dlg.InitData(m_uXNum, m_uYNum, m_uMineNum);
	dlg.DoModal();

	InitGame();
	Invalidate();
	SizeWindow();
}

void CMineWnd::OnMemuCheat()
{
	//	m_bCheat = !m_bCheat;
	//	SetCheckedCheat();
	//	Invalidate();
}

void CMineWnd::OnMemuMark()
{
	m_bMarkful = !m_bMarkful;
	SetCheckedMark();

	Invalidate();
}

void CMineWnd::OnMemuColor()
{
	m_bColorful = !m_bColorful;
	SetCheckedColor();

	LoadBitmap();
	Invalidate();
}

void CMineWnd::OnMemuSound()
{
	m_bSoundful = !m_bSoundful;
	SetCheckedSound();

	if (m_bSoundful)
	{
		LoadWave();
	}
	else
	{
		FreeWave();
	}
}

void CMineWnd::OnMemuHero()
{
	CHeroDlg dlg;
	dlg.SetBRecord(m_uPrimary);
	dlg.SetBHolder(m_strPrimary);
	dlg.SetIRecord(m_uSecond);
	dlg.SetIHolder(m_strSecond);
	dlg.SetERecord(m_uAdvance);
	dlg.SetEHolder(m_strAdvance);

	dlg.DoModal();
}

void CMineWnd::OnMemuExit()
{
	PostQuitMessage(0);
}

void CMineWnd::OnMemuHelpList()
{
	::WinExec("HH	WINMINE.CHM", SW_SHOW);
	//system("HH	WINMINE.CHM");
}

void CMineWnd::OnMemuHelpFind()
{
	::WinExec("HH	WINMINE.CHM", SW_SHOW);
}

void CMineWnd::OnMemuHelpUse()
{
	::WinExec("HH	NTHelp.CHM", SW_SHOW);
}

void CMineWnd::OnMemuAbout()
{
	ShellAbout(this->m_hWnd, TEXT("ɨ��"), TEXT("my mine game"), nullptr);
}

void CMineWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_F1:
		OnMemuHelpList();
		break;
	case VK_F2:
		OnMemuStart();
		break;
	default:
		break;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMineWnd::OnInitMenu(CMenu* pMenu)
{
	CWnd::OnInitMenu(pMenu);
	if ((m_pSubMenu = pMenu->GetSubMenu(0)) == 0)
	{
		AfxMessageBox(TEXT("��ʼ���˵�ʧ�ܣ�"));
		PostQuitMessage(0);
	}
	else
	{
		SetCheckedLevel();
		SetCheckedMark();
		SetCheckedColor();
		SetCheckedSound();
		//	SetCheckedCheat();
	}
}

void CMineWnd::OnMemuClose()
{
	SaveConfig();
	KillTimer(ID_TIMER_EVENT);

	CWnd::OnClose();
}



