#include "stdafx.h"

#include "Mine.h"
#include "MineWnd.h"
#include "Def.h"
#include "BlockArea.h"

#include "NewRecordDlg.h"
#include "HeroDlg.h"
#include "CustomDlg.h"

#include <Mmsystem.h>
#include <string>

using namespace def;
using namespace std;

#define TIMER_ID 1234

CMineWnd::CMineWnd()
: m_pBlockArea(CBlockArea::GetInstance())
{
	InitGame();	
}

CMineWnd::~CMineWnd()
{
	SaveConfig();
	FreeWave();	
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
	ON_COMMAND(IDM_HELP_USE, OnMemuHelpUse)
	ON_COMMAND(IDM_ABOUT, OnMemuAbout)	
	ON_COMMAND(IDM_HERO, OnMemuHero)
END_MESSAGE_MAP()

//获取配置文件路径
CString CMineWnd::GetCfgPath()
{
	static CString strCfgPath;

	if (strCfgPath.IsEmpty())
	{
		TCHAR szExePath[MAX_PATH];
		GetModuleFileName(nullptr, szExePath, MAX_PATH);

		strCfgPath = szExePath;
		strCfgPath = strCfgPath.Left(strCfgPath.ReverseFind('\\') + 1);
		strCfgPath += g_strCfgName;
	}	

	return strCfgPath;
}


//载入配置
void CMineWnd::LoadConfig()
{
	CString strCfgPath = GetCfgPath();	

	m_uRowNum    = GetPrivateProfileInt(g_strMine, g_strXNum,    g_nDefPrimaryXNum,    strCfgPath);
	m_uColNum    = GetPrivateProfileInt(g_strMine, g_strYNum,    g_nDefPrimaryYNum,    strCfgPath);
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

	WritePrivateProfileString(g_strMine, g_strXNum,    to_wstring(m_uRowNum).c_str(),    strCfgPath);
	WritePrivateProfileString(g_strMine, g_strYNum,    to_wstring(m_uColNum).c_str(),    strCfgPath);
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
	LoadConfig();
	LoadWave();
	LoadBitmap();
	
	m_brsBG.CreateSolidBrush(g_clrGray);	

	m_nLeftNum = m_uMineNum;
	m_uSpendTime = 0;
	m_uBtnState = EBtnS_Normal;
	m_uGameState = EGS_Wait;	

	if (m_uTimer)
	{
		KillTimer(TIMER_ID);
		m_uTimer = 0;
	}

	m_coCurBlock = {0,0};
	m_coPreBlock = { 0, 0 };

	m_pBlockArea->Init(m_uRowNum, m_uColNum, m_uMineNum);//初始化地图	
}

//失败处理
void CMineWnd::Dead(uint row, uint col)
{
	m_pBlockArea->DeadAt(row, col);

	CRect rectBtn(m_rectButton);//按钮区域	
	CRect rectBlockArea(m_rectBlockArea);//雷区

	InvalidateRect(rectBtn);
	InvalidateRect(rectBlockArea);
	
	m_uBtnState = EBtnS_Dead;
	m_uGameState = EGS_Dead;

	if (m_uTimer != 0)//将定时器去激活
	{
		KillTimer(TIMER_ID);
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
	if (!m_pBlockArea->IsVictory())
		return FALSE;

	//修改状态
	m_uBtnState = EBtnS_Victory;
	m_uGameState = EGS_Victory;

	Invalidate();
	//删除timer
	if (m_uTimer != 0)
	{
		KillTimer(TIMER_ID);
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
	for (uint i = row - 1; i <= row + 1; i++)
	{
		for (uint j = col - 1; j <= col + 1; j++)
		{
			if (IsInBlockArea(i, j))
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
	for (uint i = row - 1; i <= row + 1; i++)
	{
		for (uint j = col - 1; j <= col + 1; j++)
		{
			if (IsInBlockArea(i, j))
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
}

//设定各项尺寸
void CMineWnd::SizeWindow()
{
	//见图片
	uint uWidth = g_nXOffset * 2 + m_uRowNum * g_nBlockWidth + 18;
	uint uHeight = g_nYOffset * 2 + m_uColNum * g_nBlockHeight + g_nInnerYOffset * 2 + g_nNumHeight + g_nGap + 60;
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

//绘制控制面板和方块区域
void CMineWnd::DrawShell(CPaintDC &dc)
{
	dc.Draw3dRect(m_rectPanel.left, m_rectPanel.top, m_rectPanel.right - m_rectPanel.left, m_rectPanel.bottom - m_rectPanel.top, m_clrDark, g_clrWhite);
	dc.Draw3dRect(m_rectBlockArea.left, m_rectBlockArea.top, m_rectBlockArea.right - m_rectBlockArea.left, m_rectBlockArea.bottom - m_rectBlockArea.top, m_clrDark, g_clrWhite);
}

//绘制雷区
void CMineWnd::DrawBlockArea(CPaintDC &dc)
{
	CDC dcMemory; //用作内存设备
	dcMemory.CreateCompatibleDC(&dc); //使得这个设备与dc兼容
	dcMemory.SelectObject(m_bmpMine); //将内存设备与位图资源关联

	for (uint i = 0; i < m_uColNum; i++)
	{
		for (uint j = 0; j < m_uRowNum; j++)
		{//根据[i][j]区域的方块状态拷贝相应的图像到[i][j]区域
			dc.StretchBlt(m_rectBlockArea.left + g_nBlockWidth * j, m_rectBlockArea.top + g_nBlockHeight * i, g_nBlockWidth, g_nBlockHeight, &dcMemory, 
				0, g_nBlockHeight * m_pMines[i][j].uState, g_nBlockWidth, g_nBlockHeight, SRCCOPY);
		}
	}
}

//绘制不可扩展方块对应的数字
void CMineWnd::DrawDownNum(MINEWND* mine, uint num)
{
	mine->uState = EBS_Empty - num;
	mine->uOldState = EBS_Empty - num;

	DrawOneBlock(mine->uRow, mine->uCol);
}

//重新绘制某个方块
void CMineWnd::DrawOneBlock(uint row, uint col)
{
	CRect rcMine(col * g_nBlockWidth, row * g_nBlockHeight, (col + 1) * g_nBlockWidth, (row + 1) * g_nBlockHeight);
	InvalidateRect(rcMine);
}

//获取坐标(x,y)对应的方块
pair<uint, uint> CMineWnd::GetBlock(long x, long y)
{
	//保证参数合格
	if (x < m_rectBlockArea.left  || 
		x > m_rectBlockArea.right ||
		y < m_rectBlockArea.top   ||
		y > m_rectBlockArea.bottom)
	{
		return{0, 0};
	}
	
	return { (uint)(x - m_rectBlockArea.left) / g_nBlockWidth,
			 (uint)(y - m_rectBlockArea.top) / g_nBlockHeight };
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
	m_uRowNum = xNum;
	m_uColNum = yNum;
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
				dcMemory.SelectObject(&bitmap);//选择位图				
				dcMemory.FillRect(&m_rectClient, &m_brsBG);//绘制背景框

				DrawButton((CPaintDC&)dcMemory);
				DrawNumber((CPaintDC&)dcMemory);
				DrawShell((CPaintDC&)dcMemory);
				DrawBlockArea((CPaintDC&)dcMemory);

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
	if (nIDEvent == TIMER_ID)
	{		
		if (m_bSoundful)//播放声音
		{
			sndPlaySound((LPCTSTR)LockResource(m_pSndClock), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
		}

		m_uSpendTime++;//增加计时		
		Invalidate();
		if (m_uSpendTime >= g_nDefRecord)//最大999
		{
			KillTimer(TIMER_ID);
			m_uTimer = 0;
		}

		InvalidateRect(CRect(m_rectSpendTime));//重绘时间
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

	if (rectBtn.PtInRect(pt))//点击按钮
	{
		m_bClickBtn = TRUE;
		m_uBtnState = EBtnS_Down;//修改按钮状态
		InvalidateRect(rectBtn);//重绘按钮
	}
	else if (rectBlockArea.PtInRect(pt))//点击方块
	{		
		switch (m_uGameState)//根据游戏状态修改方块状态
		{
		case EGS_Wait: 
		case EGS_Run:
			if (m_pCurBlock = GetBlock(pt.x, pt.y))
			{
				if (m_pCurBlock->uState == EBS_Normal)
				{
					m_pCurBlock->uState = EBS_Empty;
				}

				if (m_pCurBlock->uState == EBS_Dicey)
				{
					m_pCurBlock->uState = EBS_DiceyDown;
				}

				m_pPreBlock = m_pCurBlock;//
			}
			break;
		case EGS_Dead: 
		case EGS_Victory:
			return;
			break;
		default:
			break;
		}

		m_uBtnState = EBtnS_Click;//修改按钮状态
		InvalidateRect(rectBtn);//重绘按钮
		
		if (nFlags == (MK_LBUTTON | MK_RBUTTON))//左右键同时按下
		{
			m_bLRBtnDown = TRUE;
			OnLRBtnDown(m_pPreBlock->uRow, m_pPreBlock->uCol);
		}

		InvalidateRect(rectBlockArea);//重绘方块区域
	}
	else//非交互区域
	{
		if (m_uGameState == EGS_Wait || m_uGameState == EGS_Run)
		{
			m_uBtnState = EBtnS_Click;
			InvalidateRect(rectBtn);//重绘按钮
		}
	}

	CWnd::OnLButtonDown(nFlags, pt);
}

void CMineWnd::OnLButtonUp(uint nFlags, CPoint pt)
{
	class CAutoReleaseCapture
	{
	public:
		CAutoReleaseCapture(){}
		~CAutoReleaseCapture()
		{
			ReleaseCapture();
		}

		CAutoReleaseCapture(const CAutoReleaseCapture&) = delete;
		CAutoReleaseCapture& operator=(const CAutoReleaseCapture&) = delete;
	};

	CAutoReleaseCapture obj;//自动释放

	CRect rectBtn(m_rectButton);//按钮区域	
	CRect rectBlockArea(m_rectBlockArea);//方块区域

	if (rectBtn.PtInRect(pt))//重新开始
	{		
		InitGame();
		Invalidate();
	}
	else if (rectBlockArea.PtInRect(pt))//点击雷区
	{		
		switch (m_uGameState)//根据游戏状态作处理
		{
		//游戏进行状态
		case EGS_Wait: 
		case EGS_Run:
		{
						m_pPreBlock = GetBlock(pt.x, pt.y);
						if (!m_pPreBlock)
						{
							return;
						}

						if (m_bLRBtnDown)//左右鼠标同时按下
						{
							m_bLRBtnDown = FALSE;
							OnLRBtnUp(m_pPreBlock->uRow, m_pPreBlock->uCol);

							if (m_uGameState == EGS_Wait)
							{
								m_uBtnState = EBS_Normal;
								Invalidate();

								return;
							}

							if (!HasCorrectFlags(m_pPreBlock->uRow, m_pPreBlock->uCol))//标记不准确
							{
								Dead(m_pPreBlock->uRow, m_pPreBlock->uCol);

								return;
							}

							if (m_pPreBlock->uState != EBS_Flag)//非标记状态即可扩展
							{
								OpenAround(m_pPreBlock->uRow, m_pPreBlock->uCol);//flag与mine个数相同即可打开扩展
							}					
						}
						else//仅单击左键
						{							
							if (m_uGameState == EGS_Wait)//启动游戏
							{
								if (m_uTimer)
								{
									KillTimer(TIMER_ID);
									m_uTimer = 0;
								}

								m_uSpendTime = 1;
								Invalidate();//重绘界面

								if (m_bSoundful)
								{
									sndPlaySound((LPCTSTR)LockResource(m_pSndClock), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
								}
								
								m_uTimer = SetTimer(TIMER_ID, 1000, nullptr);//启动定时器								
								LayMines(m_pPreBlock->uRow, m_pPreBlock->uCol);//布雷								
								m_uGameState = EGS_Run;//改变游戏状态
							}

							if (m_pPreBlock->uOldState == EBS_Normal)//该方块未作任何标记
							{
								if (IsMine(m_pPreBlock->uRow, m_pPreBlock->uCol))//若为雷，直接gg
								{
									Dead(m_pPreBlock->uRow, m_pPreBlock->uCol);
									
									return;
								}
								else
								{
									uint around = GetAroundMines(m_pPreBlock->uRow, m_pPreBlock->uCol);

									if (around == 0)//周围无雷，展开
										ExpandMines(m_pPreBlock->uRow, m_pPreBlock->uCol);
									else//周围有雷，显示数字
										DrawDownNum(m_pPreBlock, around);
								}
								
							}
							else if (m_pPreBlock->uOldState == EBS_Dicey)//该方块标记为问号
							{
								m_pPreBlock->uState = EBS_Dicey;
							}

							//判断是否为胜利
							if (Victory())
							{
								Invalidate();
								
								return;
							}
						}
		}
			break;
		case EGS_Victory:
		case EGS_Dead:
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

	CWnd::OnLButtonUp(nFlags, pt);
}

void CMineWnd::OnRButtonDown(uint nFlags, CPoint pt)
{
	CRect rectBtn(m_rectButton);
	CRect rectBlockArea(m_rectBlockArea);

	m_bLRBtnDown = FALSE;

	if (rectBlockArea.PtInRect(pt))//点击方块区域	
	{		
		if (m_uGameState == EGS_Wait || m_uGameState == EGS_Run)
		{
			if (m_pCurBlock = GetBlock(pt.x, pt.y))
			{
				if (nFlags == (MK_LBUTTON | MK_RBUTTON))//左右键同时按下
				{
					m_bLRBtnDown = TRUE;
					OnLRBtnDown(m_pCurBlock->uRow, m_pCurBlock->uCol);
				}
				else
				{
					switch (m_pCurBlock->uState)
					{
					case EBS_Normal:
						m_pCurBlock->uState = EBS_Flag;
						m_pCurBlock->uOldState = EBS_Flag;
						m_nLeftNum--;
						break;
					case EBS_Flag:
						m_pCurBlock->uState = EBS_Dicey;
						m_pCurBlock->uOldState = EBS_Dicey;
						m_nLeftNum++;
						break;
					case EBS_Dicey:
						m_pCurBlock->uState = EBS_Normal;
						m_pCurBlock->uOldState = EBS_Normal;
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
	m_pPreBlock = GetBlock(pt.x, pt.y);
	if (!m_pPreBlock) 
		return;

	if (m_bLRBtnDown)//左右键同时按下
	{
		m_bLRBtnDown = FALSE;
		OnLRBtnUp(m_pPreBlock->uRow, m_pPreBlock->uCol);

		if (m_uGameState == EGS_Wait)
		{
			m_uBtnState = EBtnS_Normal;
			Invalidate();
			return;
		}
		// if the around flags number equal to the around mines number, expand.
		else if (m_pPreBlock->uState != EBS_Flag)
		{
			OpenAround(m_pPreBlock->uRow, m_pPreBlock->uCol);
		}
		// check whether the MINEWND around the special MINEWND is a mine, if it is then dead.
		if (!HasCorrectFlags(m_pPreBlock->uRow, m_pPreBlock->uCol))
		{
			//			Dead(m_pPreBlock->uRow, m_pPreBlock->uCol);
			//			ReleaseCapture();
			return;
		}
	}
	else
	{
		//Victory();
	}

	CWnd::OnRButtonUp(nFlags, pt);
}

void CMineWnd::OnMouseMove(uint nFlags, CPoint pt)
{
	if (nFlags == MK_LBUTTON || nFlags == (MK_LBUTTON | MK_RBUTTON))
	{
		CRect rectBtn(m_rectButton);	
		CRect rectBlockArea(m_rectBlockArea);

		if (rectBtn.PtInRect(pt))//按钮区域
		{
			switch (m_uGameState)
			{
			case EGS_Run:
				m_uBtnState = m_bClickBtn ? EBtnS_Down : EBtnS_Click;
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
		else if (rectBlockArea.PtInRect(pt))//方块区域
		{
			switch (m_uGameState)
			{
			case EGS_Run:
				m_pCurBlock = GetBlock(pt.x, pt.y);
				if (!m_pCurBlock || !m_pPreBlock) 
					return;
				
				if (m_pCurBlock->uCol != m_pPreBlock->uCol ||
					m_pCurBlock->uRow != m_pPreBlock->uRow)
				{
					// change the new mine rect state
					switch (m_pCurBlock->uState)
					{
					case EBS_Normal:
						m_pCurBlock->uState = EBS_Empty;
						break;
					case EBS_Dicey:
						m_pCurBlock->uState = EBS_DiceyDown;
						break;
					}
					// resume the old mine rect state
					switch (m_pPreBlock->uOldState)
					{
					case EBS_Normal:
						m_pPreBlock->uState = EBS_Normal;
						break;
					case EBS_Dicey:
						m_pPreBlock->uState = EBS_Dicey;
						break;
					default:
						break;
					}
					// judge whether the lr button are pushed down
					if (m_bLRBtnDown)
					{
						OnLRBtnUp(m_pPreBlock->uRow, m_pPreBlock->uCol);
						OnLRBtnDown(m_pCurBlock->uRow, m_pCurBlock->uCol);
					}

					m_pPreBlock = m_pCurBlock;
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
		else//非交互区域
		{		
			switch (m_uGameState)
			{
			case EGS_Run:
				m_uBtnState = (m_bClickBtn) ? EBtnS_Normal: EBtnS_Click;
				if (m_pCurBlock)
				{
					if (m_pCurBlock->uOldState == EBS_Normal)
					{
						m_pCurBlock->uState = EBS_Normal;
					}
					else if (m_pCurBlock->uOldState == EBS_Dicey)
					{
						m_pCurBlock->uState = EBS_Dicey;
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
	m_uRowNum = g_nDefPrimaryXNum;
	m_uColNum = g_nDefPrimaryYNum;
	m_uMineNum = g_nDefPrimaryMineNum;

	SetCheckedLevel();
	InitGame();
	Invalidate();
	SizeWindow();
}

void CMineWnd::OnMemuMedium()
{
	m_uLevel = ELevel_Medium;
	m_uRowNum = g_nDefMediumXNum;
	m_uColNum = g_nDefMediumYNum;
	m_uMineNum = g_nDefMediumMineNum;

	SetCheckedLevel();
	InitGame();
	Invalidate();
	SizeWindow();
}

void CMineWnd::OnMemuAdvanced()
{
	m_uLevel = ELevel_Advanced;
	m_uRowNum = g_nDefAdvancedXNum;
	m_uColNum = g_nDefAdvancedYNum;
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
	dlg.InitData(m_uRowNum, m_uColNum, m_uMineNum);
	dlg.DoModal();

	InitGame();
	Invalidate();
	SizeWindow();
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
	KillTimer(TIMER_ID);

	CWnd::OnClose();
}



