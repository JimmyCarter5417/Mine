#include "stdafx.h"

#include "Mine.h"
#include "MineWnd.h"
#include "Def.h"

#include "NewRecordDlg.h"
#include "HeroDlg.h"
#include "CustomDlg.h"

#include <Mmsystem.h>
#include <string>

using namespace def;
using namespace std;

#define ID_TIMER_EVENT	WM_USER + 1

CMineWnd::CMineWnd()
{
	m_pSndDead = nullptr;
	m_pSndVictory = nullptr;
	m_pSndClock = nullptr;
	m_uTimer = 0;
	m_brsBG.CreateSolidBrush(g_clrGray);

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
	ON_COMMAND(IDM_MEDIUM, OnMemuMedium)
	ON_COMMAND(IDM_ADVANCED, OnMemuAdvanced)
	ON_COMMAND(IDM_CUSTOM, OnMemuCustom)
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

//获取配置文件路径
CString CMineWnd::GetCfgPath()
{
	static CString strCfgPath;

	if (strCfgPath.IsEmpty())
	{
		TCHAR strExePath[MAX_PATH];
		GetModuleFileName(nullptr, strExePath, MAX_PATH);

		strCfgPath = strExePath;
		strCfgPath = strCfgPath.Left(strCfgPath.ReverseFind('\\') + 1);
		strCfgPath += g_strCfgName;
	}	

	return strCfgPath;
}


//载入配置
void CMineWnd::LoadConfig()
{
	CString strCfgPath = GetCfgPath();	

	m_uXNum    = GetPrivateProfileInt(g_strMine, g_strXNum,    g_nDefPrimaryXNum,    strCfgPath);
	m_uYNum    = GetPrivateProfileInt(g_strMine, g_strYNum,    g_nDefPrimaryYNum,    strCfgPath);
	m_uMineNum = GetPrivateProfileInt(g_strMine, g_strMineNum, g_nDefPrimaryMineNum, strCfgPath);
	m_uLevel   = GetPrivateProfileInt(g_strMine, g_strLevel,   g_DefLevel,           strCfgPath);

	m_uPrimary  = GetPrivateProfileInt(g_strRecord, g_strPrimary,  g_nDefRecord, strCfgPath);
	m_uMedium   = GetPrivateProfileInt(g_strRecord, g_strMedium,   g_nDefRecord, strCfgPath);
	m_uAdvanced = GetPrivateProfileInt(g_strRecord, g_strAdvanced, g_nDefRecord, strCfgPath);

	TCHAR buf[128] = {0};
	GetPrivateProfileString(g_strHolder, g_strPrimary, g_strDefHolder, buf, 128, strCfgPath);
	m_strPrimary = buf;

	memset(buf, 0, sizeof buf);
	GetPrivateProfileString(g_strHolder, g_strMedium, g_strDefHolder, buf, 128, strCfgPath);
	m_strMedium = buf;

	memset(buf, 0, sizeof buf);
	GetPrivateProfileString(g_strHolder, g_strAdvanced, g_strDefHolder, buf, 128, strCfgPath);
	m_strAdvanced = buf;
	
	m_bColorful = GetPrivateProfileInt(g_strConfig, g_strColorful, g_nDefColorful, strCfgPath);
	m_bSoundful = GetPrivateProfileInt(g_strConfig, g_strSoundful, g_nDefSoundful, strCfgPath);
}

// 保存配置
void CMineWnd::SaveConfig()
{
	CString strCfgPath = GetCfgPath();

	WritePrivateProfileString(g_strMine, g_strXNum,    to_wstring(m_uXNum).c_str(),    strCfgPath);
	WritePrivateProfileString(g_strMine, g_strYNum,    to_wstring(m_uYNum).c_str(),    strCfgPath);
	WritePrivateProfileString(g_strMine, g_strMineNum, to_wstring(m_uMineNum).c_str(), strCfgPath);
	WritePrivateProfileString(g_strMine, g_strLevel,   to_wstring(m_uLevel).c_str(),   strCfgPath);

	WritePrivateProfileString(g_strRecord, g_strPrimary,  to_wstring(m_uPrimary).c_str(),  strCfgPath);
	WritePrivateProfileString(g_strRecord, g_strMedium,   to_wstring(m_uMedium).c_str(),   strCfgPath);
	WritePrivateProfileString(g_strRecord, g_strAdvanced, to_wstring(m_uAdvanced).c_str(), strCfgPath);

	WritePrivateProfileString(g_strHolder, g_strPrimary, m_strPrimary,  strCfgPath);
	WritePrivateProfileString(g_strHolder, g_strPrimary, m_strMedium,   strCfgPath);
	WritePrivateProfileString(g_strHolder, g_strPrimary, m_strAdvanced, strCfgPath);

	WritePrivateProfileString(g_strConfig, g_strColorful, to_wstring(m_bColorful).c_str(), strCfgPath);
	WritePrivateProfileString(g_strConfig, g_strSoundful, to_wstring(m_bSoundful).c_str(), strCfgPath);
}

//载入位图
void CMineWnd::LoadBitmap()
{
	m_bmpMine.DeleteObject();
	m_bmpNumber.DeleteObject();
	m_bmpButton.DeleteObject();

	if (m_bColorful)
	{
		m_clrDark = g_clrDarkGray;
		
		m_bmpMine.LoadBitmap(IDB_MINE_COLOR);		
		m_bmpNumber.LoadBitmap(IDB_NUM_COLOR);		
		m_bmpButton.LoadBitmap(IDB_BTN_COLOR);
	}
	else
	{
		m_clrDark = g_clrBlack;
		
		m_bmpMine.LoadBitmap(IDB_MINE_GRAY);		
		m_bmpNumber.LoadBitmap(IDB_NUM_GRAY);		
		m_bmpButton.LoadBitmap(IDB_BTN_GRAY);
	}
}

//初始化
void CMineWnd::InitGame()
{	
	LoadBitmap();//初始化位图

	m_nLeftNum = m_uMineNum;
	m_uSpendTime = 0;
	m_uBtnState = EBtnS_Normal;
	m_uGameState = EGS_Wait;

	if (m_uTimer)
	{
		KillTimer(ID_TIMER_EVENT);
		m_uTimer = 0;
	}

	m_pNewMine = nullptr;
	m_pOldMine = nullptr;

	FreeMines();
	//初始化地图
	for (uint i = 0; i < m_uYNum; i++)
	{
		for (uint j = 0; j < m_uXNum; j++)
		{
			m_pMines[i][j].uRow = i;
			m_pMines[i][j].uCol = j;
			m_pMines[i][j].uState = EBS_Normal;
			m_pMines[i][j].uAttrib = EBA_Empty;
			m_pMines[i][j].uOldState = EBS_Normal;
		}
	}
}

//布雷
void CMineWnd::LayMines(uint row, uint col)
{
	srand((uint)time(nullptr));

	uint nLeftMineNum = m_uMineNum;
	while (nLeftMineNum > 0)//布m_uMineNum个雷
	{
		//取随机数
		uint i = rand() % m_uYNum;
		uint j = rand() % m_uXNum;

		if (!(i == row && j == col) && 
			m_pMines[i][j].uAttrib != EBA_Mine)
		{			
			m_pMines[i][j].uAttrib = EBA_Mine;//修改属性为雷
			nLeftMineNum--;//雷增加一			
		}
	}
}

//雷方块拓展(对于周围无雷的空白区域)
void CMineWnd::ExpandMines(uint row, uint col)
{
	uint around = GetAroundMines(row, col);
	m_pMines[row][col].uState = EBS_Empty - around;
	m_pMines[row][col].uOldState = EBS_Empty - around;

	// “打开”该区域，重绘
	DrawSpecialMine(row, col);

	//对周围一个雷都没有的空白区域
	if (around == 0)
	{
		for (uint i = (row == 0 ? 0 : row - 1); i < row + 2; i++)
		{
			for (uint j = (col == 0 ? 0 : col - 1); j < col + 2; j++)
			{
				//对于周围可以拓展的区域进行的规拓展			
				if (!(i == row && j == col) &&
					m_pMines[i][j].uState == EBS_Normal	&& 
					m_pMines[i][j].uAttrib != EBA_Mine)
				{
					if (IsInMineArea(i, j))
						ExpandMines(i, j);
				}
			}
		}
	}
}

//获取某个小方块区域相邻8个区域的雷个数
uint CMineWnd::GetAroundMines(uint row, uint col)
{
	uint res = 0;

	for (uint i = (row == 0 ? 0 : row - 1); i < row + 2; i++)
	{
		for (uint j = (col == 0 ? 0 : col - 1); j < col + 2; j++)
		{
			if (IsInMineArea(i, j) && m_pMines[i][j].uAttrib == EBA_Mine)
				res++;
		}
	}

	return res;
}

//获取某个小方块区域相邻8个区域的已标志状态数
uint CMineWnd::GetAroundFlags(uint row, uint col)
{
	uint res = 0;

	for (uint i = (row == 0 ? 0 : row - 1); i < row + 2; i++)
	{
		for (uint j = (col == 0 ? 0 : col - 1); j < col + 2; j++)
		{
			if (IsInMineArea(i, j) && m_pMines[i][j].uState == EBS_Flag)
				res++;
		}
	}

	return res;
}

//判断是否为雷
BOOL CMineWnd::IsMine(uint row, uint col)
{
	return m_pMines[row][col].uAttrib == EBA_Mine;
}

//判断是否在雷区域
BOOL CMineWnd::IsInMineArea(uint row, uint col)
{
	return row >= 0 && row < m_uYNum && col >= 0 && col < m_uXNum;
}

//失败处理
void CMineWnd::Dead(uint row, uint col)
{	
	if (m_pMines[row][col].uAttrib == EBA_Mine)//点中雷
	{
		m_pMines[row][col].uState = EBS_Blast;
		m_pMines[row][col].uOldState = EBS_Blast;
		
	}
	else//错误雷
	{
		m_pMines[row][col].uState = EBS_Error;
		m_pMines[row][col].uOldState = EBS_Error;		
	}

	for (uint i = 0; i < m_uYNum; i++)
	{
		for (uint j = 0; j < m_uXNum; j++)
		{
			if (m_pMines[i][j].uAttrib == EBA_Mine &&
				m_pMines[i][j].uState != EBS_Flag)
			{
				m_pMines[i][j].uState = EBS_Mine;
				m_pMines[i][j].uOldState = EBS_Mine;
			}
		}
	}

	CRect rectBtn(m_rectButton);//按钮区域	
	CRect rectBlockArea(m_rectBlockArea);//雷区

	InvalidateRect(rectBtn);
	InvalidateRect(rectBlockArea);
	
	m_uBtnState = EBtnS_Dead;
	m_uGameState = EGS_Dead;

	if (m_uTimer != 0)//将定时器去激活
	{
		KillTimer(ID_TIMER_EVENT);
		m_uTimer = 0;
	}

	if (m_bSoundful)
	{
		sndPlaySound((LPCTSTR)LockResource(m_pSndDead), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
	}
}

//胜利判断并处理
BOOL CMineWnd::Victory()
{
	//CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);

	// 检测是否胜利
	for (uint i = 0; i < m_uYNum; i++)
	{
		for (uint j = 0; j < m_uXNum; j++)
		{
			if (m_pMines[i][j].uState == EBS_Normal || 
				m_pMines[i][j].uState == EBS_Dicey)
			{
				return FALSE;
			}
		}
	}

	//修改状态
	m_uBtnState = EBtnS_Victory;
	m_uGameState = EGS_Victory;

	Invalidate();
	//删除timer
	if (m_uTimer != 0)
	{
		KillTimer(ID_TIMER_EVENT);
		m_uTimer = 0;
	}
	//胜利声音
	if (m_bSoundful)
	{
		sndPlaySound((LPCTSTR)LockResource(m_pSndVictory), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
	}

	//英雄榜
	uint uRecord = 999;
	switch (m_uLevel)
	{
	case ELevel_Primary:
		uRecord = m_uPrimary;
		break;
	case ELevel_Medium:
		uRecord = m_uMedium;
		break;
	case ELevel_Advanced:
		uRecord = m_uAdvanced;
		break;
	default:
		break;
	}

	if (uRecord > m_uSpendTime)
	{
		CNewRecordDlg dlg;
		dlg.SetLevel(m_uLevel);
		dlg.DoModal();

		switch (m_uLevel)
		{
		case ELevel_Primary:
			m_uPrimary = m_uSpendTime;
			m_strPrimary = dlg.GetName();
			break;
		case ELevel_Medium:
			m_uMedium = m_uSpendTime;
			m_strMedium = dlg.GetName();
			break;
		case ELevel_Advanced:
			m_uAdvanced = m_uSpendTime;
			m_strAdvanced = dlg.GetName();
			break;
		case ELevel_Custom:
			return TRUE;
		default:
			break;
		}

		OnMemuHero();
	}

	return TRUE;
}

//鼠标右键，根据原先不同的标记作状态循环修改，以便用户可以修改其原先标志
void CMineWnd::OnLRBtnDown(uint row, uint col)
{
	for (uint i = (row == 0 ? 0 : row - 1); i < row + 2; i++)
	{
		for (uint j = (col == 0 ? 0 : col - 1); j < col + 2; j++)
		{
			if (IsInMineArea(i, j))
			{
				if (m_pMines[i][j].uState == EBS_Normal)
				{
					m_pMines[i][j].uState = EBS_Empty;
				}
				else if (m_pMines[i][j].uState == EBS_Dicey)
				{
					m_pMines[i][j].uState = EBS_DiceyDown;
				}
			}
		}
	}
}

void CMineWnd::OnLRBtnUp(uint row, uint col)
{
	for (uint i = (row == 0 ? 0 : row - 1); i < row + 2; i++)
	{
		for (uint j = (col == 0 ? 0 : col - 1); j < col + 2; j++)
		{
			if (IsInMineArea(i, j))
			{
				if (m_pMines[i][j].uOldState == EBS_Normal)
				{
					m_pMines[i][j].uState = EBS_Normal;
				}
				else if (m_pMines[i][j].uOldState == EBS_Dicey)
				{
					m_pMines[i][j].uState = EBS_Dicey;
				}
			}
		}
	}
	//	Invalidate();
}

//展开拓展周围8个方向
void CMineWnd::OpenAround(uint row, uint col)
{
	//如果周围相邻的标志雷数 != 周围相邻的雷数 则返回
	if (GetAroundFlags(row, col) != GetAroundMines(row, col))
		return;

	for (uint i = (row == 0 ? 0 : row - 1); i < row + 2; i++)
	{
		for (uint j = (col == 0 ? 0 : col - 1); j < col + 2; j++)
		{
			if (IsInMineArea(i, j))
			{
				//如果该区域为正常区域，拓展该雷区
				if (m_pMines[i][j].uState == EBS_Normal)
				{					
					ExpandMines(i, j);

					uint around = GetAroundMines(i, j);
					m_pMines[i][j].uState = EBS_Empty - around;//设置周围雷数目
					m_pMines[i][j].uOldState = EBS_Empty - around;
				}
			}
		}
	}

	//胜利则将所有雷标识出来
	if (Victory())
	{
		for (uint i = 0; i < m_uYNum; i++)
		{
			for (uint j = 0; j < m_uXNum; j++)
			{
				if (m_pMines[i][j].uAttrib == EBA_Mine)
				{
					m_pMines[i][j].uState = EBS_Flag;
					m_pMines[i][j].uOldState = EBS_Flag;
				}
			}
		}

		m_nLeftNum = 0;
		Invalidate();
	}
}

BOOL CMineWnd::ErrorAroundFlag(uint row, uint col)
{
	//如果周围相邻的标志雷数 != 周围相邻的雷数 则返回
	if (GetAroundFlags(row, col) != GetAroundMines(row, col)) 
		return FALSE;

	for (uint i = (row == 0 ? 0 : row - 1); i < row + 2; i++)
	{
		for (uint j = (col == 0 ? 0 : col - 1); j < col + 2; j++)
		{
			if (IsInMineArea(i, j) &&
				m_pMines[i][j].uState == EBS_Flag &&
				m_pMines[i][j].uAttrib != EBA_Mine)
				{
					Dead(i, j);
					return TRUE;
				}
		}
	}

	return FALSE;
}

void CMineWnd::OpenByCheat()
{
	for (uint i = 0; i < m_uYNum; i++)
	{
		for (uint j = 0; j < m_uXNum; j++)
		{
			if (m_pMines[i][j].uState != EBS_Flag)
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
	//	for (uint row = 0; row<m_uYNum; row++) {
	//		if (m_pMines[row]) {
	//			delete []m_pMines[m_uXNum];
	//			delete m_pMines[row];
	//		}
	//	}
}

void CMineWnd::SizeWindow()
{
	//见图片
	uint uWidth = g_nXOffset * 2 + m_uXNum * g_nBlockWidth + 18;
	uint uHeight = g_nYOffset * 2 + m_uYNum * g_nBlockHeight + g_nInnerYOffset * 2 + g_nNumHeight + g_nGap + 60;
	SetWindowPos(&wndTopMost, 0, 0, uWidth, uHeight, SWP_NOZORDER | SWP_NOMOVE | SWP_NOCOPYBITS);
	//客户区域
	GetClientRect(&m_rectClient);
	//面板	
	m_rectPanel.left = m_rectClient.left + g_nXOffset;
	m_rectPanel.top = m_rectClient.top + g_nYOffset;
	m_rectPanel.right = m_rectClient.right - g_nXOffset;
	m_rectPanel.bottom = m_rectPanel.top + g_nPanelHeight;
	//方块区域	
	m_rectBlockArea.left = m_rectPanel.left;
	m_rectBlockArea.top = m_rectPanel.bottom + g_nGap;
	m_rectBlockArea.right = m_rectPanel.right;
	m_rectBlockArea.bottom = m_rectClient.bottom - g_nYOffset;
	//中间按钮
	m_rectButton.left = m_rectClient.right / 2 - g_nBtnWidth / 2;
	m_rectButton.top = m_rectPanel.top + g_nInnerYOffset;
	m_rectButton.right = m_rectButton.left + g_nBtnWidth;
	m_rectButton.bottom = m_rectButton.top + g_nBtnHeight;
	//左侧剩余雷数框
	m_rectLeftMines.left = m_rectPanel.left + g_nInnerXOffset;
	m_rectLeftMines.top = m_rectPanel.top + g_nInnerYOffset;
	m_rectLeftMines.right = m_rectLeftMines.left + g_nNumWidth * 3;
	m_rectLeftMines.bottom = m_rectLeftMines.top + g_nNumHeight;
	//右侧时间框
	m_rectSpendTime.left = m_rectPanel.right - g_nInnerXOffset - g_nNumWidth * 3;
	m_rectSpendTime.top = m_rectLeftMines.top;
	m_rectSpendTime.right = m_rectPanel.right - g_nInnerXOffset;
	m_rectSpendTime.bottom = m_rectLeftMines.bottom;
}

//绘制笑脸按钮图
void CMineWnd::DrawButton(CPaintDC &dc)
{
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_bmpButton);

	dc.Draw3dRect(m_rectButton.left, m_rectButton.top, g_nBtnWidth, g_nBtnHeight, m_clrDark, m_clrDark);
	dc.StretchBlt(m_rectButton.left, m_rectButton.top, g_nBtnWidth, g_nBtnHeight, &cdc, 
		0, g_nBtnHeight * m_uBtnState, g_nBtnWidth, g_nBtnHeight, SRCCOPY);	
}

// 绘制数字
void CMineWnd::DrawNumber(CPaintDC &dc)
{
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_bmpNumber);

	//绘制矩形框
	dc.Draw3dRect(m_rectLeftMines.left, m_rectLeftMines.top, g_nNumWidth * 3, g_nNumHeight, m_clrDark, g_clrWhite);
	dc.Draw3dRect(m_rectSpendTime.left, m_rectSpendTime.top, g_nNumWidth * 3, g_nNumHeight, m_clrDark, g_clrWhite);

	//绘制剩余雷数
	int num = (m_nLeftNum < 0) ? 11 : m_nLeftNum / 100;
	dc.StretchBlt(m_rectLeftMines.left, m_rectLeftMines.top, g_nNumWidth, g_nNumHeight, &cdc,
		0, g_nNumHeight * (ENum_End - 1 - num), g_nNumWidth, g_nNumHeight, SRCCOPY);
	
	num = (m_nLeftNum < 0) ? -(m_nLeftNum - num * 100) / 10 : (m_nLeftNum - num * 100) / 10;
	dc.StretchBlt(m_rectLeftMines.left + g_nNumWidth, m_rectLeftMines.top, g_nNumWidth, g_nNumHeight, &cdc, 
		0, g_nNumHeight * (ENum_End - 1 - num), g_nNumWidth, g_nNumHeight, SRCCOPY);
	
	num = (m_nLeftNum < 0) ? -m_nLeftNum % 10 : m_nLeftNum % 10;
	dc.StretchBlt(m_rectLeftMines.left + g_nNumWidth * 2, m_rectLeftMines.top, g_nNumWidth, g_nNumHeight, &cdc,
		0, g_nNumHeight * (ENum_End - 1 - num), g_nNumWidth, g_nNumHeight, SRCCOPY);

	//绘制时间
	num = m_uSpendTime / 100;
	dc.StretchBlt(m_rectSpendTime.left, m_rectSpendTime.top, g_nNumWidth, g_nNumHeight, &cdc, 
		0, g_nNumHeight * (ENum_End - 1 - num), g_nNumWidth, g_nNumHeight, SRCCOPY);

	num = (m_uSpendTime - num * 100) / 10;
	dc.StretchBlt(m_rectSpendTime.left + g_nNumWidth, m_rectSpendTime.top, g_nNumWidth, g_nNumHeight, &cdc,
		0, g_nNumHeight * (ENum_End - 1 - num), g_nNumWidth, g_nNumHeight, SRCCOPY);

	num = m_uSpendTime % 10;
	dc.StretchBlt(m_rectSpendTime.left + g_nNumWidth * 2, m_rectSpendTime.top, g_nNumWidth, g_nNumHeight, &cdc,
		0, g_nNumHeight * (ENum_End - 1 - num), g_nNumWidth, g_nNumHeight, SRCCOPY);
}

//绘制外壳
void CMineWnd::DrawShell(CPaintDC &dc)
{
	dc.Draw3dRect(m_rectPanel.left, m_rectPanel.top, m_rectPanel.right - m_rectPanel.left, m_rectPanel.bottom - m_rectPanel.top, m_clrDark, g_clrWhite);
	dc.Draw3dRect(m_rectBlockArea.left, m_rectBlockArea.top, m_rectBlockArea.right - m_rectBlockArea.left, m_rectBlockArea.bottom - m_rectBlockArea.top, m_clrDark, g_clrWhite);
}

//绘制雷区
void CMineWnd::DrawMineArea(CPaintDC &dc)
{
	CDC dcMemory; //用作内存设备
	dcMemory.CreateCompatibleDC(&dc); //使得这个设备与dc兼容
	dcMemory.SelectObject(m_bmpMine); //将内存设备与位图资源关联

	for (uint i = 0; i < m_uYNum; i++)
	{
		for (uint j = 0; j < m_uXNum; j++)
		{//根据[i][j]区域的雷方块状态拷贝相应的图像到[i][j]雷区的特定区域
			dc.StretchBlt(m_rectBlockArea.left + g_nBlockWidth * j, m_rectBlockArea.top + g_nBlockHeight * i, g_nBlockWidth, g_nBlockHeight, &dcMemory, 
				0, g_nBlockHeight * m_pMines[i][j].uState, g_nBlockWidth, g_nBlockHeight, SRCCOPY);
		}
	}
}

//绘制按下扫雷后的数字
void CMineWnd::DrawDownNum(MINEWND* mine, uint num)
{
	mine->uState = EBS_End - 1 - num;
	mine->uOldState = EBS_End - 1 - num;
	CRect rcMine(mine->uCol * g_nBlockWidth, mine->uRow * g_nBlockHeight, (mine->uCol + 1) * g_nBlockWidth, (mine->uRow + 1) * g_nBlockHeight);
	InvalidateRect(rcMine);
}

//重新绘制雷区域的某个方块
void CMineWnd::DrawSpecialMine(uint row, uint col)
{
	CRect rcMine(col * g_nBlockWidth, row * g_nBlockHeight, (col + 1) * g_nBlockWidth, (row + 1) * g_nBlockHeight);
	InvalidateRect(rcMine);
}

//获取体图中(x,y)区域的雷信息
CMineWnd::MINEWND* CMineWnd::GetMine(long x, long y)
{
	//保证参数合格
	if (x < m_rectBlockArea.left  || 
		x > m_rectBlockArea.right ||
		y < m_rectBlockArea.top   ||
		y > m_rectBlockArea.bottom)
	{
		return nullptr;
	}

	//根据坐标值算出该小方块所在地图的行和列
	uint uCol = (uint)(x - m_rectBlockArea.left) / g_nBlockWidth;
	uint uRow = (uint)(y - m_rectBlockArea.top) / g_nBlockHeight;
	//返回该区域的雷信息
	return &m_pMines[uRow][uCol];
}

void CMineWnd::SetCheckedLevel()
{
	if (m_pSubMenu)
	{
		m_pSubMenu->CheckMenuItem(IDM_PRIMARY,  MF_UNCHECKED | MF_BYCOMMAND);
		m_pSubMenu->CheckMenuItem(IDM_MEDIUM,   MF_UNCHECKED | MF_BYCOMMAND);
		m_pSubMenu->CheckMenuItem(IDM_ADVANCED, MF_UNCHECKED | MF_BYCOMMAND);
		m_pSubMenu->CheckMenuItem(IDM_CUSTOM,   MF_UNCHECKED | MF_BYCOMMAND);
		switch (m_uLevel)
		{
		case ELevel_Primary:
			m_pSubMenu->CheckMenuItem(IDM_PRIMARY, MF_CHECKED | MF_BYCOMMAND);
			break;
		case ELevel_Medium:
			m_pSubMenu->CheckMenuItem(IDM_MEDIUM, MF_CHECKED | MF_BYCOMMAND);
			break;
		case ELevel_Advanced:
			m_pSubMenu->CheckMenuItem(IDM_ADVANCED, MF_CHECKED | MF_BYCOMMAND);
			break;
		case ELevel_Custom:
			m_pSubMenu->CheckMenuItem(IDM_CUSTOM, MF_CHECKED | MF_BYCOMMAND);
			break;
		default:
			break;
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
	HMODULE hResource = AfxGetResourceHandle();

	m_pSndDead    = LoadResource(hResource, FindResource(hResource, MAKEINTRESOURCE(IDR_WAVE_DEAD),    TEXT("WAVE")));
	m_pSndVictory = LoadResource(hResource, FindResource(hResource, MAKEINTRESOURCE(IDR_WAVE_VICTORY), TEXT("WAVE")));
	m_pSndClock   = LoadResource(hResource, FindResource(hResource, MAKEINTRESOURCE(IDR_WAVE_CLOCK),   TEXT("WAVE")));
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
	m_uPrimary = g_nDefRecord;
	m_strPrimary = g_strDefHolder;

	m_uMedium = g_nDefRecord;
	m_strMedium = g_strDefHolder;

	m_uAdvanced = g_nDefRecord;
	m_strAdvanced = g_strDefHolder;
}

void CMineWnd::SetCustom(uint xNum, uint yNum, uint mNum)
{
	m_uXNum = xNum;
	m_uYNum = yNum;
	m_uMineNum = mNum;
}

void CMineWnd::OnPaint()
{
	CPaintDC dc(this);    // 用以屏幕显示的设备
	CDC dcMemory;  // 内存设备

	CBitmap bitmap;

	if (!dc.IsPrinting())
	{
		// 与dc设备兼容
		if (dcMemory.CreateCompatibleDC(&dc))
		{
			// 使得bitmap与实际显示的设备兼容
			if (bitmap.CreateCompatibleBitmap(&dc, m_rectClient.right, m_rectClient.bottom))
			{
				// 内存设备选择物件－位图
				dcMemory.SelectObject(&bitmap);
				//绘制背景框
				dcMemory.FillRect(&m_rectClient, &m_brsBG);

				DrawButton((CPaintDC&)dcMemory);//笑脸按钮绘图
				DrawNumber((CPaintDC&)dcMemory);//文字绘图(计时器文字和剩余雷数文字)
				DrawShell((CPaintDC&)dcMemory);//3D效果外壳绘图
				DrawMineArea((CPaintDC&)dcMemory);//雷区绘图


				// 将内存设备的内容拷贝到实际屏幕显示的设备
				dc.BitBlt(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom, &dcMemory, 0, 0, SRCCOPY);
				bitmap.DeleteObject();
			}
		}
	}
}

void CMineWnd::OnShowWindow(BOOL bShow, uint nStatus)
{
	SizeWindow();
	CWnd::OnShowWindow(bShow, nStatus);
}

void CMineWnd::OnTimer(uint nIDEvent)
{
	if (nIDEvent == ID_TIMER_EVENT)
	{
		if (m_bSoundful)
		{
			sndPlaySound((LPCTSTR)LockResource(m_pSndClock), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
		}
		m_uSpendTime++;
		//CRect rcNum(m_uNumRect[0], 15, m_uNumRect[1], 39);
		//	InvalidateRect(rcNum);
		Invalidate();
		if (m_uSpendTime >= g_nDefRecord)//最大999
		{
			KillTimer(ID_TIMER_EVENT);
			m_uTimer = 0;
		}
	}

	CWnd::OnTimer(nIDEvent);
}

void CMineWnd::OnLButtonDown(uint nFlags, CPoint pt)
{	
	CRect rectBtn(m_rectButton);//按钮区域	
	CRect rectBlockArea(m_rectBlockArea);//雷区

	SetCapture();//鼠标移出界面也捕获其消息

	m_bClickBtn = FALSE;
	m_bLRBtnDown = FALSE;

	if (rectBtn.PtInRect(pt))
	{// click in the button area
		m_bClickBtn = TRUE;
		m_uBtnState = EBtnS_Down;
		InvalidateRect(rectBtn);
	}
	else if (rectBlockArea.PtInRect(pt))
	{// click in the mine area
		// change mine state by gamestate
		switch (m_uGameState)
		{
		case EGS_Wait: 
		case EGS_Run:
			if (m_pNewMine = GetMine(pt.x, pt.y))
			{
				if (m_pNewMine->uState == EBS_Normal)
				{
					m_pNewMine->uState = EBS_Empty;
				}

				if (m_pNewMine->uState == EBS_Dicey)
				{
					m_pNewMine->uState = EBS_DiceyDown;
				}

				m_pOldMine = m_pNewMine;
			}
			break;
		case EGS_Dead: 
		case EGS_Victory:
			return;
			break;
		default:
			break;
		}
		m_uBtnState = EBtnS_Click;
		InvalidateRect(rectBtn);

		//左右键同时按下
		if (nFlags == (MK_LBUTTON | MK_RBUTTON))
		{
			m_bLRBtnDown = TRUE;
			OnLRBtnDown(m_pOldMine->uRow, m_pOldMine->uCol);
		}

		InvalidateRect(rectBlockArea);
	}
	else// click in other area
	{
		if (m_uGameState == EGS_Wait || m_uGameState == EGS_Run)
		{
			m_uBtnState = EBtnS_Click;
			InvalidateRect(rectBtn);
		}
	}

	CWnd::OnLButtonDown(nFlags, pt);
}

void CMineWnd::OnLButtonUp(uint nFlags, CPoint pt)
{
	CRect rectBtn(m_rectButton);//按钮区域	
	CRect rectBlockArea(m_rectBlockArea);//雷区

	if (rectBtn.PtInRect(pt))// 点击笑脸
	{
		Invalidate();
		InitGame();
	}
	else if (rectBlockArea.PtInRect(pt))//点击雷区
	{		
		//根据不同的游戏状态作处理
		switch (m_uGameState)
		{
		//游戏进行状态
		case EGS_Wait: 
		case EGS_Run:
		{
						// first get the MINEWND which if pushing down
						m_pOldMine = GetMine(pt.x, pt.y);
						if (!m_pOldMine)
						{
							ReleaseCapture();
							return;
						}
						// do normal process
						//检测判断当前状态是否为左右鼠标同时按下
						if (m_bLRBtnDown)
						{
							m_bLRBtnDown = FALSE;
							OnLRBtnUp(m_pOldMine->uRow, m_pOldMine->uCol);

							if (m_uGameState == EGS_Wait)
							{
								m_uBtnState = EBS_Normal;
								Invalidate();
								ReleaseCapture();
								return;
							}

							//假若周围已经标识的雷＝周围真正的雷数，拓展
							if (m_pOldMine->uState != EBS_Flag)
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
							//如果游戏尚未开始，点击左键启动游戏
							if (m_uGameState == EGS_Wait)
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
								//启动定时器
								m_uTimer = SetTimer(ID_TIMER_EVENT, 1000, nullptr);
								//布雷
								LayMines(m_pOldMine->uRow, m_pOldMine->uCol);		// lay all the mines down 
								//改变游戏状态
								m_uGameState = EGS_Run;

							}

							if (m_pOldMine->uOldState == EBS_Normal)
							{
								//当该雷区域为正常未作标记才打开								
								//如果该区域为雷，则死亡
								if (IsMine(m_pOldMine->uRow, m_pOldMine->uCol))
								{
									Dead(m_pOldMine->uRow, m_pOldMine->uCol);
									ReleaseCapture();
									return;
								}

								//不是雷的时候，获取其周围的雷数目
								//如果为空白区域，拓展，否则打开该区域（显示周围有多少雷数）
								uint around = GetAroundMines(m_pOldMine->uRow, m_pOldMine->uCol);

								if (around == 0) 
									ExpandMines(m_pOldMine->uRow, m_pOldMine->uCol);
								else 
									DrawDownNum(m_pOldMine, around);
							}
							else if (m_pOldMine->uOldState == EBS_Dicey)
							{//标志为“？”问号的时候
								m_pOldMine->uState = EBS_Dicey;
							}

							//判断是否为胜利
							if (Victory())
							{
								Invalidate();
								ReleaseCapture();
								return;
							}
						}
		}
			break;
		case EGS_Victory:
		case EGS_Dead:
			ReleaseCapture();		// release the cursor
			return;
		default:
			break;
		}

		m_uBtnState = EBtnS_Normal;
		Invalidate();
	}
	else//点击非雷区域
	{
		if (m_uGameState == EGS_Wait || m_uGameState == EGS_Run)
		{
			m_uBtnState = EBtnS_Normal;
			InvalidateRect(rectBtn);
		}
	}

	ReleaseCapture();		// release the cursor
	CWnd::OnLButtonUp(nFlags, pt);
}

void CMineWnd::OnRButtonDown(uint nFlags, CPoint pt)
{
	CRect rectBtn(m_rectButton);//按钮区域	
	CRect rectBlockArea(m_rectBlockArea);//雷区

	m_bLRBtnDown = FALSE;

	if (rectBlockArea.PtInRect(pt))//点击雷区域	
	{		
		if (m_uGameState == EGS_Wait || m_uGameState == EGS_Run)
		{
			if (m_pNewMine = GetMine(pt.x, pt.y))
			{
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
					case EBS_Normal:
						m_pNewMine->uState = EBS_Flag;
						m_pNewMine->uOldState = EBS_Flag;
						m_nLeftNum--;
						break;
					case EBS_Flag:
						m_pNewMine->uState = EBS_Dicey;
						m_pNewMine->uOldState = EBS_Dicey;
						m_nLeftNum++;
						break;
					case EBS_Dicey:
						m_pNewMine->uState = EBS_Normal;
						m_pNewMine->uOldState = EBS_Normal;
						break;
					default:
						break;
					}
				}

				Invalidate();
			}
		}
	}

	CWnd::OnRButtonDown(nFlags, pt);
}

void CMineWnd::OnRButtonUp(uint nFlags, CPoint pt)
{
	CRect rectBtn(m_rectButton);//按钮区域	
	CRect rectBlockArea(m_rectBlockArea);//雷区

	m_pOldMine = GetMine(pt.x, pt.y);
	if (!m_pOldMine) 
		return;

	// judge whether the lr button are both pushed down
	if (m_bLRBtnDown)
	{
		m_bLRBtnDown = FALSE;
		OnLRBtnUp(m_pOldMine->uRow, m_pOldMine->uCol);
		if (m_uGameState == EGS_Wait)
		{
			m_uBtnState = EBtnS_Normal;
			Invalidate();
			return;
		}
		// if the around flags number equal to the around mines number, expand.
		if (m_pOldMine->uState != EBS_Flag)
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

void CMineWnd::OnMouseMove(uint nFlags, CPoint pt)
{
	if (nFlags == MK_LBUTTON || nFlags == (MK_LBUTTON | MK_RBUTTON))
	{
		CRect rectBtn(m_rectButton);//按钮区域	
		CRect rectBlockArea(m_rectBlockArea);//雷区		

		if (rectBtn.PtInRect(pt))
		{				// pt in button area
			switch (m_uGameState)
			{
			case EGS_Run:
				m_uBtnState = (m_bClickBtn) ? EBtnS_Down : EBtnS_Click;
				break;
			case EGS_Dead: 
			case EGS_Victory:
				if (m_bClickBtn) 
					m_uBtnState = EBtnS_Down;
				break;
			default:
				break;
			}

			InvalidateRect(rectBtn);
		}
		else if (rectBlockArea.PtInRect(pt))
		{		// pt in mine area
			switch (m_uGameState)
			{
			case EGS_Run:
				m_pNewMine = GetMine(pt.x, pt.y);
				if (!m_pNewMine || !m_pOldMine) 
					return;
				
				if (m_pNewMine->uCol != m_pOldMine->uCol ||
					m_pNewMine->uRow != m_pOldMine->uRow)
				{
					// change the new mine rect state
					switch (m_pNewMine->uState)
					{
					case EBS_Normal:
						m_pNewMine->uState = EBS_Empty;
						break;
					case EBS_Dicey:
						m_pNewMine->uState = EBS_DiceyDown;
						break;
					}
					// resume the old mine rect state
					switch (m_pOldMine->uOldState)
					{
					case EBS_Normal:
						m_pOldMine->uState = EBS_Normal;
						break;
					case EBS_Dicey:
						m_pOldMine->uState = EBS_Dicey;
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

				InvalidateRect(rectBlockArea);
				break;
			case EGS_Victory: 
			case EGS_Dead:
				return;
			default:
				break;
			}
		}
		else// pt in other area
		{		
			switch (m_uGameState)
			{
			case EGS_Run:
				m_uBtnState = (m_bClickBtn) ? EBtnS_Normal: EBtnS_Click;
				if (m_pNewMine)
				{
					if (m_pNewMine->uOldState == EBS_Normal)
					{
						m_pNewMine->uState = EBS_Normal;
					}
					else if (m_pNewMine->uOldState == EBS_Dicey)
					{
						m_pNewMine->uState = EBS_Dicey;
					}
				}
				break;
			case EGS_Dead:
				m_uBtnState = EBtnS_Dead;
				break;
			case EGS_Victory:
				m_uBtnState = EBtnS_Victory;
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
	m_uLevel = ELevel_Primary;
	m_uXNum = g_nDefPrimaryXNum;
	m_uYNum = g_nDefPrimaryYNum;
	m_uMineNum = g_nDefPrimaryMineNum;

	SetCheckedLevel();
	InitGame();
	Invalidate();
	SizeWindow();
}

void CMineWnd::OnMemuMedium()
{
	m_uLevel = ELevel_Medium;
	m_uXNum = g_nDefMediumXNum;
	m_uYNum = g_nDefMediumYNum;
	m_uMineNum = g_nDefMediumMineNum;

	SetCheckedLevel();
	InitGame();
	Invalidate();
	SizeWindow();
}

void CMineWnd::OnMemuAdvanced()
{
	m_uLevel = ELevel_Advanced;
	m_uXNum = g_nDefAdvancedXNum;
	m_uYNum = g_nDefAdvancedYNum;
	m_uMineNum = g_nDefAdvancedMineNum;

	SetCheckedLevel();
	InitGame();
	Invalidate();
	SizeWindow();
}

void CMineWnd::OnMemuCustom()
{
	m_uLevel = ELevel_Custom;
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
	dlg.SetIRecord(m_uMedium);
	dlg.SetIHolder(m_strMedium);
	dlg.SetERecord(m_uAdvanced);
	dlg.SetEHolder(m_strAdvanced);

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
	ShellAbout(this->m_hWnd, TEXT("扫雷"), TEXT("my mine game"), nullptr);
}

void CMineWnd::OnKeyDown(uint nChar, uint nRepCnt, uint nFlags)
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
		AfxMessageBox(TEXT("初始化菜单失败！"));
		PostQuitMessage(0);
	}
	else
	{
		SetCheckedLevel();
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



