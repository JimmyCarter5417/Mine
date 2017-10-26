#include "stdafx.h"

#include "Mine.h"
#include "MineWnd.h"
#include "Def.h"
#include "BlockArea.h"

#include "NewRecordDlg.h"
#include "HeroDlg.h"
#include "CustomDlg.h"
#include "HelpDlg.h"

#include <Mmsystem.h>
#include <string>

using namespace def;
using namespace std;

#define TIMER_ID 1234

CMineWnd::CMineWnd()
: m_pBlockArea(CBlockArea::GetInstance())
, m_pCfgMgr(CCfgMgr::GetInstance())
, m_pSubMenu(nullptr)
, m_uTimer(0)
{
	m_brsBG.CreateSolidBrush(g_clrGray);

	InitGame();	
}

CMineWnd::~CMineWnd()
{
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
	ON_COMMAND(IDM_START, OnMenuStart)
	ON_COMMAND(IDM_PRIMARY, OnMenuPrimary)
	ON_COMMAND(IDM_MEDIUM, OnMenuMedium)
	ON_COMMAND(IDM_ADVANCED, OnMenuAdvanced)
	ON_COMMAND(IDM_CUSTOM, OnMenuCustom)
	ON_COMMAND(IDM_COLOR, OnMenuColor)
	ON_COMMAND(IDM_SOUND, OnMenuSound)
	ON_COMMAND(IDM_EXIT, OnMenuExit)
	ON_COMMAND(IDM_HELP, OnMenuHelp)
	ON_COMMAND(IDM_ABOUT, OnMenuAbout)	
	ON_COMMAND(IDM_HERO, OnMenuHero)
END_MESSAGE_MAP()

//载入配置
void CMineWnd::LoadConfig()
{
	m_pCfgMgr->GetMineInfo(m_uRowNum, m_uColNum, m_uMineNum, m_uLevel);

	m_bColorful = m_pCfgMgr->GetColorful();
	m_bSoundful = m_pCfgMgr->GetSoundful();
}

//载入位图
void CMineWnd::LoadBitmap()
{
	m_bmpMine.DeleteObject();
	m_bmpNumber.DeleteObject();
	m_bmpButton.DeleteObject();

	if (m_bColorful)
	{
		m_clrBg = g_clrDarkGray;
		
		m_bmpMine.LoadBitmap(IDB_MINE_COLOR);		
		m_bmpNumber.LoadBitmap(IDB_NUM_COLOR);		
		m_bmpButton.LoadBitmap(IDB_BTN_COLOR);
	}
	else
	{
		m_clrBg = g_clrGray;
		
		m_bmpMine.LoadBitmap(IDB_MINE_GRAY);		
		m_bmpNumber.LoadBitmap(IDB_NUM_GRAY);		
		m_bmpButton.LoadBitmap(IDB_BTN_GRAY);
	}
}

//初始化
void CMineWnd::InitGame()
{	
	LoadWave();
	LoadConfig();
	LoadBitmap();	

	m_nLeftNum = m_uMineNum;
	m_uSpendTime = 0;
	m_uBtnState = EBtnS_Normal;
	m_uGameState = EGS_Wait;	

	if (m_uTimer)
	{
		KillTimer(m_uTimer);
		m_uTimer = 0;
	}

	m_posCurBlock = { 0, 0 };
	m_posOldBlock = { 0, 0 };

	m_pBlockArea->Init(m_uRowNum, m_uColNum, m_uMineNum);//初始化地图	
}

//失败处理
void CMineWnd::Dead()
{	
	m_uBtnState = EBtnS_Dead;
	m_uGameState = EGS_Dead;

	/*InvalidateRect(&m_rectButton);
	InvalidateRect(&m_rectBlockArea);*/
	Invalidate();

	if (m_uTimer != 0)//将定时器去激活
	{
		KillTimer(m_uTimer);
		m_uTimer = 0;
	}

	if (m_bSoundful)
	{
		sndPlaySound((LPCTSTR)LockResource(m_pSndDead), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
	}
}

//胜利判断并处理
void CMineWnd::Victory()
{
	//修改状态
	m_uBtnState = EBtnS_Victory;
	m_uGameState = EGS_Victory;

	Invalidate();
	//删除timer
	if (m_uTimer != 0)
	{
		KillTimer(m_uTimer);
		m_uTimer = 0;
	}
	//胜利声音
	if (m_bSoundful)
	{
		sndPlaySound((LPCTSTR)LockResource(m_pSndVictory), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
	}

	if (CNewRecordDlg::IsNewRecord(m_uSpendTime))
	{
		CNewRecordDlg dlg;
		dlg.Init(m_uSpendTime);
		dlg.DoModal();

		//英雄榜
		OnMenuHero();
	}
}

//设定各项尺寸
void CMineWnd::CalcWindowSize()
{
	//见图片
	uint uWidth = g_nXOffset * 2 + m_uColNum * g_nBlockWidth + 18;
	uint uHeight = g_nYOffset * 2 + m_uRowNum * g_nBlockHeight + g_nInnerYOffset * 2 + g_nNumHeight + g_nGap + 60;
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

	dc.Draw3dRect(m_rectButton.left, m_rectButton.top, g_nBtnWidth, g_nBtnHeight, m_clrBg, m_clrBg);
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
	dc.Draw3dRect(m_rectLeftMines.left, m_rectLeftMines.top, g_nNumWidth * 3, g_nNumHeight, m_clrBg, g_clrWhite);
	dc.Draw3dRect(m_rectSpendTime.left, m_rectSpendTime.top, g_nNumWidth * 3, g_nNumHeight, m_clrBg, g_clrWhite);

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

//绘制控制面板和方块区域的外框
void CMineWnd::DrawFrame(CPaintDC &dc)
{
	dc.Draw3dRect(m_rectPanel.left, m_rectPanel.top, m_rectPanel.right - m_rectPanel.left, m_rectPanel.bottom - m_rectPanel.top, m_clrBg, g_clrWhite);
	dc.Draw3dRect(m_rectBlockArea.left, m_rectBlockArea.top, m_rectBlockArea.right - m_rectBlockArea.left, m_rectBlockArea.bottom - m_rectBlockArea.top, m_clrBg, g_clrWhite);
}

//绘制方块区域
void CMineWnd::DrawBlockArea(CPaintDC &dc)
{
	CDC dcMemory; //用作内存设备
	dcMemory.CreateCompatibleDC(&dc); //使得这个设备与dc兼容
	dcMemory.SelectObject(m_bmpMine); //将内存设备与位图资源关联

	for (int i = 0; i < m_uRowNum; i++)
	{
		for (int j = 0; j < m_uColNum; j++)
		{
			//根据i,j位置的方块状态拷贝相应的图像到对应区域
			dc.StretchBlt(m_rectBlockArea.left + g_nBlockWidth * j, m_rectBlockArea.top + g_nBlockHeight * i, g_nBlockWidth, g_nBlockHeight, &dcMemory, 
				0, g_nBlockHeight * m_pBlockArea->GetCurState({ i, j }), g_nBlockWidth, g_nBlockHeight, SRCCOPY);
		}
	}
}

//获取坐标(x,y)对应的方块
bool CMineWnd::GetBlock(long x, long y, TPos& pos)
{
	//保证参数合格
	if (x < m_rectBlockArea.left  || 
		x > m_rectBlockArea.right ||
		y < m_rectBlockArea.top   ||
		y > m_rectBlockArea.bottom)
	{
		return false;
	}
	
	pos = { (y - m_rectBlockArea.top) / g_nBlockHeight, (x - m_rectBlockArea.left) / g_nBlockWidth };
	return true;
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
	FreeWave();

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

				DrawFrame((CPaintDC&)dcMemory);
				DrawButton((CPaintDC&)dcMemory);
				DrawNumber((CPaintDC&)dcMemory);				
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
	CalcWindowSize();

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

		if (++m_uSpendTime >= g_nDefRecord)//最大999
		{
			KillTimer(m_uTimer);
			m_uTimer = 0;
		}

		InvalidateRect(CRect(m_rectSpendTime));//重绘时间
	}

	CWnd::OnTimer(nIDEvent);
}

void CMineWnd::OnLButtonDown(uint nFlags, CPoint pt)
{	
	SetCapture();//鼠标移出界面也捕获其消息

	m_bClickBtn = FALSE;
	m_bLRBtnDown = FALSE;

	if (IsInBtn(pt))//点击按钮
	{
		m_bClickBtn = TRUE;
		m_uBtnState = EBtnS_Down;
	}
	else if (IsInBlockArea(pt))//点击方块区域
	{		
		GetBlock(pt.x, pt.y, m_posCurBlock);//获取方块逻辑坐标

		switch (m_uGameState)
		{
		case EGS_Wait://等待或游戏中，正常修改m_posCurBlock方块状态
		case EGS_Run:			
		   {
				m_pBlockArea->LBtnDown(m_posCurBlock);//修改方块状态
				
				m_posOldBlock = m_posCurBlock;//保存当前位置
				m_uBtnState = EBtnS_Click;

				if (nFlags == (MK_LBUTTON | MK_RBUTTON))//左右键同时按下
				{
					m_bLRBtnDown = TRUE;
					m_pBlockArea->LRBtnDown(m_posCurBlock);
				}				
			}
			break;
		case EGS_Dead: //gg，直接返回
		case EGS_Victory:
			return;
			break;
		default:
			break;
		}		
	}
	else//非交互区域
	{
		if (m_uGameState == EGS_Wait || m_uGameState == EGS_Run)
		{
			m_uBtnState = EBtnS_Click;			
		}
	}

	Invalidate();
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

		/*CAutoReleaseCapture(const CAutoReleaseCapture&) = delete;
		CAutoReleaseCapture& operator=(const CAutoReleaseCapture&) = delete;*/
	};

	CAutoReleaseCapture obj;//自动释放	

	if (IsInBtn(pt))//点击按钮
	{		
		InitGame();//重新开始
	}
	else if (IsInBlockArea(pt))//点击方块区域
	{		
		switch (m_uGameState)
		{
		case EGS_Wait://等待
		{						  
						  m_uBtnState = EBtnS_Normal;//按钮状态	
						  m_uGameState = EGS_Run;//开始游戏 
						  m_uSpendTime = 0;	  

						  if (m_uTimer)
						  {
							  KillTimer(m_uTimer);							  
						  }		
						  m_uTimer = SetTimer(TIMER_ID, 1000, nullptr);//启动定时器							  
						
						  GetBlock(pt.x, pt.y, m_posCurBlock);

						  m_pBlockArea->LayMines(m_posCurBlock);//布雷						  
						  
						  if (m_bLRBtnDown)//左右鼠标同时按下
						  {
							  m_bLRBtnDown = FALSE;	
							  
							  m_pBlockArea->LRBtnUp(m_posCurBlock);//修改周围状态 
						  }	
						  else
						  {
							  if (m_pBlockArea->GetOldState(m_posCurBlock) == EBS_Mine)//点中雷
							  {
								  m_pBlockArea->DeadAt(m_posCurBlock);
								  Dead();
							  }
							  else
							  {
								  if (m_pBlockArea->GetOldState(m_posCurBlock) == EBS_Normal)//可打开
								  {
									  m_pBlockArea->Open(m_posCurBlock);

									  //判断是否为胜利
									  if (m_pBlockArea->IsVictory())
									  {
										  Victory();
									  }
								  }
							  }
						  }
		}
			break;
		case EGS_Run://游戏中
		{
						m_uBtnState = EBtnS_Normal;

						GetBlock(pt.x, pt.y, m_posCurBlock);

						if (m_bLRBtnDown)//左右鼠标同时按下
						{
							m_bLRBtnDown = FALSE;

							m_pBlockArea->LRBtnUp(m_posCurBlock);//修改周围状态									

							if ((m_pBlockArea->GetAroundMineNum(m_posCurBlock) == m_pBlockArea->GetAroundFlagNum(m_posCurBlock)) &&//周围的标记数等于雷数
								(m_pBlockArea->GetCurState(m_posCurBlock) >= EBS_Num8))//方块必须已打开
							{
								if (m_pBlockArea->HasCorrectFlags(m_posCurBlock))//标记准确
								{
									m_pBlockArea->OpenAround(m_posCurBlock);//打开周围方块

									//判断是否为胜利
									if (m_pBlockArea->IsVictory())
									{
										Victory();
									}
								}
								else
								{
									m_pBlockArea->DeadAt(m_posCurBlock);//gg
									Dead();
								}							
							}
						}
						else//仅单击左键
						{								
							if (m_pBlockArea->GetOldState(m_posCurBlock) == EBS_Normal)//点击左键之前该方块未作任何标记
							{
								if (m_pBlockArea->IsMine(m_posCurBlock))//若为雷，直接gg
								{
									m_pBlockArea->DeadAt(m_posCurBlock);
									Dead();
								}
								else
								{
									m_pBlockArea->Open(m_posCurBlock);//不是雷，打开									
								}								
							}
							else if (m_pBlockArea->GetOldState(m_posCurBlock) == EBS_Dicey)//该方块标记为问号
							{
								m_pBlockArea->SetCurState(m_posCurBlock, EBS_Dicey);
							}

							//判断是否为胜利
							if (m_pBlockArea->IsVictory())
							{
								Victory();								
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
	}
	else//非交互区域
	{
		if (m_uGameState == EGS_Wait || m_uGameState == EGS_Run)
		{
			m_uBtnState = EBtnS_Normal;
			//InvalidateRect(&m_rectButton);
		}
	}

	Invalidate();//简单起见，完全重绘
	CWnd::OnLButtonUp(nFlags, pt);
}

void CMineWnd::OnRButtonDown(uint nFlags, CPoint pt)
{
	m_bLRBtnDown = FALSE;

	if (IsInBlockArea(pt))//点击方块区域	
	{
		if (m_uGameState == EGS_Wait || m_uGameState == EGS_Run)
		{
			GetBlock(pt.x, pt.y, m_posCurBlock);//不会失败

			if ((nFlags & MK_LBUTTON) && (nFlags & MK_RBUTTON))//左右键同时按下
			{
				m_bLRBtnDown = TRUE;
				m_pBlockArea->LRBtnDown(m_posCurBlock);//修改周围状态					
			}
			else
			{
				//EBS_Normal -> EBS_Flag -> EBS_Dicey -> EBS_Normal
				switch (m_pBlockArea->GetCurState(m_posCurBlock))
				{
				case EBS_Normal:
				{
								   m_pBlockArea->SetCurState(m_posCurBlock, EBS_Flag);
								   m_pBlockArea->SetOldState(m_posCurBlock, EBS_Flag);

								   m_nLeftNum--;
				}
					break;
				case EBS_Flag:
				{
								 m_pBlockArea->SetCurState(m_posCurBlock, EBS_Dicey);
								 m_pBlockArea->SetOldState(m_posCurBlock, EBS_Dicey);

								 m_nLeftNum++;
				}
					break;
				case EBS_Dicey:
				{
								  m_pBlockArea->SetCurState(m_posCurBlock, EBS_Normal);
								  m_pBlockArea->SetOldState(m_posCurBlock, EBS_Normal);
				}
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

void CMineWnd::OnRButtonUp(uint nFlags, CPoint pt)
{
	if (m_bLRBtnDown)//左右键同时按下
	{
		m_bLRBtnDown = FALSE;

		if (GetBlock(pt.x, pt.y, m_posCurBlock))
		{
			m_pBlockArea->LRBtnUp(m_posCurBlock);
		}				

		//以下与左键松开时的处理相同
		if ((m_pBlockArea->GetAroundMineNum(m_posCurBlock) == m_pBlockArea->GetAroundFlagNum(m_posCurBlock)) &&//周围的标记数等于雷数
			(m_pBlockArea->GetCurState(m_posCurBlock) >= EBS_Num8))//方块必须已打开
		{
			if (m_pBlockArea->HasCorrectFlags(m_posCurBlock))//标记准确
			{
				m_pBlockArea->OpenAround(m_posCurBlock);//打开周围方块

				//判断是否为胜利
				if (m_pBlockArea->IsVictory())
				{
					Victory();
				}
			}
			else
			{
				m_pBlockArea->DeadAt(m_posCurBlock);//gg
				Dead();
			}
		}
	}

	CWnd::OnRButtonUp(nFlags, pt);
}

void CMineWnd::OnMouseMove(uint nFlags, CPoint pt)
{
	if (nFlags & MK_LBUTTON)
	{
		if (IsInBtn(pt))//按钮区域
		{
			m_uBtnState = EBtnS_Down;
			//InvalidateRect(&m_rectButton);
		}
		else if (IsInBlockArea(pt))//方块区域
		{
			switch (m_uGameState)
			{
			case EGS_Wait:
			case EGS_Run:
			{
							GetBlock(pt.x, pt.y, m_posCurBlock);

							if (m_posCurBlock.x != m_posOldBlock.x || m_posCurBlock.y != m_posOldBlock.y)//移动到新方块
							{
								//设置新方块状态
								switch (m_pBlockArea->GetCurState(m_posCurBlock))
								{
								case EBS_Normal:
									m_pBlockArea->SetCurState(m_posCurBlock, EBS_Empty);
									break;
								case EBS_Dicey:
									m_pBlockArea->SetCurState(m_posCurBlock, EBS_DiceyDown);
									break;
								default:
									break;
								}
								//恢复老方块保存的老的状态
								switch (m_pBlockArea->GetOldState(m_posOldBlock))
								{
								case EBS_Normal:
									m_pBlockArea->SetOldState(m_posOldBlock, EBS_Normal);
									break;
								case EBS_Dicey:
									m_pBlockArea->SetOldState(m_posOldBlock, EBS_Dicey);
									break;
								default:
									break;
								}

								if (m_bLRBtnDown)
								{
									m_pBlockArea->LRBtnUp(m_posOldBlock);//弹起
									m_pBlockArea->LRBtnDown(m_posCurBlock);//按下				
								}
								else
								{
									m_pBlockArea->LBtnUp(m_posOldBlock);//弹起
									m_pBlockArea->LBtnDown(m_posCurBlock);//按下	
								}

								m_posOldBlock = m_posCurBlock;//保存当前位置
							}
			}
				break;
			case EGS_Victory: 
			case EGS_Dead:				
			default:
				break;
			}
		}
		else//非交互区域
		{		
			switch (m_uGameState)
			{
			case EGS_Run:
				m_uBtnState = m_bClickBtn ? EBtnS_Normal: EBtnS_Click;				
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
		}

		Invalidate();
	}	

	CWnd::OnMouseMove(nFlags, pt);
}

void CMineWnd::OnMenuStart()
{
	InitGame();
	Invalidate();
}

void CMineWnd::OnMenuPrimary()
{
	SetCheckedLevel();

	m_pCfgMgr->SetMineInfo(g_nDefPrimaryRowNum, g_nDefPrimaryColNum, g_nDefPrimaryMineNum, ELevel_Primary);	

	InitGame();	
	CalcWindowSize();

	Invalidate();
}

void CMineWnd::OnMenuMedium()
{
	SetCheckedLevel();

	m_pCfgMgr->SetMineInfo(g_nDefMediumRowNum, g_nDefMediumColNum, g_nDefMediumMineNum, ELevel_Medium);

	InitGame();
	CalcWindowSize();

	Invalidate();
}

void CMineWnd::OnMenuAdvanced()
{
	SetCheckedLevel();

	m_pCfgMgr->SetMineInfo(g_nDefAdvancedRowNum, g_nDefAdvancedColNum, g_nDefAdvancedMineNum, ELevel_Advanced);

	InitGame();
	CalcWindowSize();

	Invalidate();
}

void CMineWnd::OnMenuCustom()
{
	SetCheckedLevel();

	CCustomDlg dlg;
	dlg.DoModal();

	InitGame();	
	CalcWindowSize();
	
	Invalidate();
}

void CMineWnd::OnMenuColor()
{
	m_bColorful = !m_bColorful;
	SetCheckedColor();//菜单
	m_pCfgMgr->SetColorful(m_bColorful);//更新配置文件

	LoadBitmap();
	Invalidate();
}

void CMineWnd::OnMenuSound()
{
	m_bSoundful = !m_bSoundful;
	SetCheckedSound();
	m_pCfgMgr->SetSoundful(m_bColorful);//更新配置文件

	if (m_bSoundful)
	{
		LoadWave();
	}
	else
	{
		FreeWave();
	}
}

void CMineWnd::OnMenuHero()
{
	CHeroDlg dlg;	
	dlg.DoModal();
}

void CMineWnd::OnMenuExit()
{
	PostQuitMessage(0);
}

void CMineWnd::OnMenuHelp()
{
	CHelpDlg dlg;
	dlg.DoModal();
}

void CMineWnd::OnMenuAbout()
{
	ShellAbout(this->m_hWnd, TEXT("扫雷"), TEXT("my mine game"), nullptr);
}

void CMineWnd::OnKeyDown(uint nChar, uint nRepCnt, uint nFlags)
{
	switch (nChar)
	{
	case VK_F1:
	case 'H':
		OnMenuHelp();
		break;
	case VK_F2:
		OnMenuAbout();
		break;
	case VK_F3:
		OnMenuStart();
		break;
	case VK_F4:
		OnMenuExit();
		break;
	case VK_F5:
		GodView();
		break;
	case VK_F6:
	case 'C':
		OnMenuColor();
		break;
	case VK_F7:
	case 'S':
		OnMenuSound();
		break;
	default:
		break;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMineWnd::GodView()
{
	static bool show = false;
	show = !show;	

	m_pBlockArea->ShowMines(show);
	Invalidate();
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
	}
}

//按钮区域
bool CMineWnd::IsInBtn(CPoint pt)
{
	return CRect(m_rectButton).PtInRect(pt);
}

//按钮区域
bool CMineWnd::IsInBlockArea(CPoint pt)
{
	return CRect(m_rectBlockArea).PtInRect(pt);
}