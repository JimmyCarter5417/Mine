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

//��������
void CMineWnd::LoadConfig()
{
	m_pCfgMgr->GetMineInfo(m_uRowNum, m_uColNum, m_uMineNum, m_uLevel);

	m_bColorful = m_pCfgMgr->GetColorful();
	m_bSoundful = m_pCfgMgr->GetSoundful();
}

//����λͼ
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

//��ʼ��
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

	m_pBlockArea->Init(m_uRowNum, m_uColNum, m_uMineNum);//��ʼ����ͼ	
}

//ʧ�ܴ���
void CMineWnd::Dead()
{	
	m_uBtnState = EBtnS_Dead;
	m_uGameState = EGS_Dead;

	/*InvalidateRect(&m_rectButton);
	InvalidateRect(&m_rectBlockArea);*/
	Invalidate();

	if (m_uTimer != 0)//����ʱ��ȥ����
	{
		KillTimer(m_uTimer);
		m_uTimer = 0;
	}

	if (m_bSoundful)
	{
		sndPlaySound((LPCTSTR)LockResource(m_pSndDead), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
	}
}

//ʤ���жϲ�����
void CMineWnd::Victory()
{
	//�޸�״̬
	m_uBtnState = EBtnS_Victory;
	m_uGameState = EGS_Victory;

	Invalidate();
	//ɾ��timer
	if (m_uTimer != 0)
	{
		KillTimer(m_uTimer);
		m_uTimer = 0;
	}
	//ʤ������
	if (m_bSoundful)
	{
		sndPlaySound((LPCTSTR)LockResource(m_pSndVictory), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
	}

	if (CNewRecordDlg::IsNewRecord(m_uSpendTime))
	{
		CNewRecordDlg dlg;
		dlg.Init(m_uSpendTime);
		dlg.DoModal();

		//Ӣ�۰�
		OnMenuHero();
	}
}

//�趨����ߴ�
void CMineWnd::CalcWindowSize()
{
	//��ͼƬ
	uint uWidth = g_nXOffset * 2 + m_uColNum * g_nBlockWidth + 18;
	uint uHeight = g_nYOffset * 2 + m_uRowNum * g_nBlockHeight + g_nInnerYOffset * 2 + g_nNumHeight + g_nGap + 60;
	SetWindowPos(&wndTopMost, 0, 0, uWidth, uHeight, SWP_NOZORDER | SWP_NOMOVE | SWP_NOCOPYBITS);
	//�ͻ�����
	GetClientRect(&m_rectClient);
	//���	
	m_rectPanel.left = m_rectClient.left + g_nXOffset;
	m_rectPanel.top = m_rectClient.top + g_nYOffset;
	m_rectPanel.right = m_rectClient.right - g_nXOffset;
	m_rectPanel.bottom = m_rectPanel.top + g_nPanelHeight;
	//��������	
	m_rectBlockArea.left = m_rectPanel.left;
	m_rectBlockArea.top = m_rectPanel.bottom + g_nGap;
	m_rectBlockArea.right = m_rectPanel.right;
	m_rectBlockArea.bottom = m_rectClient.bottom - g_nYOffset;
	//�м䰴ť
	m_rectButton.left = m_rectClient.right / 2 - g_nBtnWidth / 2;
	m_rectButton.top = m_rectPanel.top + g_nInnerYOffset;
	m_rectButton.right = m_rectButton.left + g_nBtnWidth;
	m_rectButton.bottom = m_rectButton.top + g_nBtnHeight;
	//���ʣ��������
	m_rectLeftMines.left = m_rectPanel.left + g_nInnerXOffset;
	m_rectLeftMines.top = m_rectPanel.top + g_nInnerYOffset;
	m_rectLeftMines.right = m_rectLeftMines.left + g_nNumWidth * 3;
	m_rectLeftMines.bottom = m_rectLeftMines.top + g_nNumHeight;
	//�Ҳ�ʱ���
	m_rectSpendTime.left = m_rectPanel.right - g_nInnerXOffset - g_nNumWidth * 3;
	m_rectSpendTime.top = m_rectLeftMines.top;
	m_rectSpendTime.right = m_rectPanel.right - g_nInnerXOffset;
	m_rectSpendTime.bottom = m_rectLeftMines.bottom;
}

//����Ц����ťͼ
void CMineWnd::DrawButton(CPaintDC &dc)
{
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_bmpButton);

	dc.Draw3dRect(m_rectButton.left, m_rectButton.top, g_nBtnWidth, g_nBtnHeight, m_clrBg, m_clrBg);
	dc.StretchBlt(m_rectButton.left, m_rectButton.top, g_nBtnWidth, g_nBtnHeight, &cdc, 
		0, g_nBtnHeight * m_uBtnState, g_nBtnWidth, g_nBtnHeight, SRCCOPY);	
}

// ��������
void CMineWnd::DrawNumber(CPaintDC &dc)
{
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_bmpNumber);

	//���ƾ��ο�
	dc.Draw3dRect(m_rectLeftMines.left, m_rectLeftMines.top, g_nNumWidth * 3, g_nNumHeight, m_clrBg, g_clrWhite);
	dc.Draw3dRect(m_rectSpendTime.left, m_rectSpendTime.top, g_nNumWidth * 3, g_nNumHeight, m_clrBg, g_clrWhite);

	//����ʣ������
	int num = (m_nLeftNum < 0) ? 11 : m_nLeftNum / 100;
	dc.StretchBlt(m_rectLeftMines.left, m_rectLeftMines.top, g_nNumWidth, g_nNumHeight, &cdc,
		0, g_nNumHeight * (ENum_End - 1 - num), g_nNumWidth, g_nNumHeight, SRCCOPY);
	
	num = (m_nLeftNum < 0) ? -(m_nLeftNum - num * 100) / 10 : (m_nLeftNum - num * 100) / 10;
	dc.StretchBlt(m_rectLeftMines.left + g_nNumWidth, m_rectLeftMines.top, g_nNumWidth, g_nNumHeight, &cdc, 
		0, g_nNumHeight * (ENum_End - 1 - num), g_nNumWidth, g_nNumHeight, SRCCOPY);
	
	num = (m_nLeftNum < 0) ? -m_nLeftNum % 10 : m_nLeftNum % 10;
	dc.StretchBlt(m_rectLeftMines.left + g_nNumWidth * 2, m_rectLeftMines.top, g_nNumWidth, g_nNumHeight, &cdc,
		0, g_nNumHeight * (ENum_End - 1 - num), g_nNumWidth, g_nNumHeight, SRCCOPY);

	//����ʱ��
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

//���ƿ������ͷ�����������
void CMineWnd::DrawFrame(CPaintDC &dc)
{
	dc.Draw3dRect(m_rectPanel.left, m_rectPanel.top, m_rectPanel.right - m_rectPanel.left, m_rectPanel.bottom - m_rectPanel.top, m_clrBg, g_clrWhite);
	dc.Draw3dRect(m_rectBlockArea.left, m_rectBlockArea.top, m_rectBlockArea.right - m_rectBlockArea.left, m_rectBlockArea.bottom - m_rectBlockArea.top, m_clrBg, g_clrWhite);
}

//���Ʒ�������
void CMineWnd::DrawBlockArea(CPaintDC &dc)
{
	CDC dcMemory; //�����ڴ��豸
	dcMemory.CreateCompatibleDC(&dc); //ʹ������豸��dc����
	dcMemory.SelectObject(m_bmpMine); //���ڴ��豸��λͼ��Դ����

	for (int i = 0; i < m_uRowNum; i++)
	{
		for (int j = 0; j < m_uColNum; j++)
		{
			//����i,jλ�õķ���״̬������Ӧ��ͼ�񵽶�Ӧ����
			dc.StretchBlt(m_rectBlockArea.left + g_nBlockWidth * j, m_rectBlockArea.top + g_nBlockHeight * i, g_nBlockWidth, g_nBlockHeight, &dcMemory, 
				0, g_nBlockHeight * m_pBlockArea->GetCurState({ i, j }), g_nBlockWidth, g_nBlockHeight, SRCCOPY);
		}
	}
}

//��ȡ����(x,y)��Ӧ�ķ���
bool CMineWnd::GetBlock(long x, long y, TPos& pos)
{
	//��֤�����ϸ�
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
	CPaintDC dc(this);    // ������Ļ��ʾ���豸
	CDC dcMemory;  // �ڴ��豸

	CBitmap bitmap;

	if (!dc.IsPrinting())
	{
		// ��dc�豸����
		if (dcMemory.CreateCompatibleDC(&dc))
		{
			// ʹ��bitmap��ʵ����ʾ���豸����
			if (bitmap.CreateCompatibleBitmap(&dc, m_rectClient.right, m_rectClient.bottom))
			{				
				dcMemory.SelectObject(&bitmap);//ѡ��λͼ				
				dcMemory.FillRect(&m_rectClient, &m_brsBG);//���Ʊ�����

				DrawFrame((CPaintDC&)dcMemory);
				DrawButton((CPaintDC&)dcMemory);
				DrawNumber((CPaintDC&)dcMemory);				
				DrawBlockArea((CPaintDC&)dcMemory);

				// ���ڴ��豸�����ݿ�����ʵ����Ļ��ʾ���豸
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
		if (m_bSoundful)//��������
		{
			sndPlaySound((LPCTSTR)LockResource(m_pSndClock), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
		}

		if (++m_uSpendTime >= g_nDefRecord)//���999
		{
			KillTimer(m_uTimer);
			m_uTimer = 0;
		}

		InvalidateRect(CRect(m_rectSpendTime));//�ػ�ʱ��
	}

	CWnd::OnTimer(nIDEvent);
}

void CMineWnd::OnLButtonDown(uint nFlags, CPoint pt)
{	
	SetCapture();//����Ƴ�����Ҳ��������Ϣ

	m_bClickBtn = FALSE;
	m_bLRBtnDown = FALSE;

	if (IsInBtn(pt))//�����ť
	{
		m_bClickBtn = TRUE;
		m_uBtnState = EBtnS_Down;
	}
	else if (IsInBlockArea(pt))//�����������
	{		
		GetBlock(pt.x, pt.y, m_posCurBlock);//��ȡ�����߼�����

		switch (m_uGameState)
		{
		case EGS_Wait://�ȴ�����Ϸ�У������޸�m_posCurBlock����״̬
		case EGS_Run:			
		   {
				m_pBlockArea->LBtnDown(m_posCurBlock);//�޸ķ���״̬
				
				m_posOldBlock = m_posCurBlock;//���浱ǰλ��
				m_uBtnState = EBtnS_Click;

				if (nFlags == (MK_LBUTTON | MK_RBUTTON))//���Ҽ�ͬʱ����
				{
					m_bLRBtnDown = TRUE;
					m_pBlockArea->LRBtnDown(m_posCurBlock);
				}				
			}
			break;
		case EGS_Dead: //gg��ֱ�ӷ���
		case EGS_Victory:
			return;
			break;
		default:
			break;
		}		
	}
	else//�ǽ�������
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

	CAutoReleaseCapture obj;//�Զ��ͷ�	

	if (IsInBtn(pt))//�����ť
	{		
		InitGame();//���¿�ʼ
	}
	else if (IsInBlockArea(pt))//�����������
	{		
		switch (m_uGameState)
		{
		case EGS_Wait://�ȴ�
		{						  
						  m_uBtnState = EBtnS_Normal;//��ť״̬	
						  m_uGameState = EGS_Run;//��ʼ��Ϸ 
						  m_uSpendTime = 0;	  

						  if (m_uTimer)
						  {
							  KillTimer(m_uTimer);							  
						  }		
						  m_uTimer = SetTimer(TIMER_ID, 1000, nullptr);//������ʱ��							  
						
						  GetBlock(pt.x, pt.y, m_posCurBlock);

						  m_pBlockArea->LayMines(m_posCurBlock);//����						  
						  
						  if (m_bLRBtnDown)//�������ͬʱ����
						  {
							  m_bLRBtnDown = FALSE;	
							  
							  m_pBlockArea->LRBtnUp(m_posCurBlock);//�޸���Χ״̬ 
						  }	
						  else
						  {
							  if (m_pBlockArea->GetOldState(m_posCurBlock) == EBS_Mine)//������
							  {
								  m_pBlockArea->DeadAt(m_posCurBlock);
								  Dead();
							  }
							  else
							  {
								  if (m_pBlockArea->GetOldState(m_posCurBlock) == EBS_Normal)//�ɴ�
								  {
									  m_pBlockArea->Open(m_posCurBlock);

									  //�ж��Ƿ�Ϊʤ��
									  if (m_pBlockArea->IsVictory())
									  {
										  Victory();
									  }
								  }
							  }
						  }
		}
			break;
		case EGS_Run://��Ϸ��
		{
						m_uBtnState = EBtnS_Normal;

						GetBlock(pt.x, pt.y, m_posCurBlock);

						if (m_bLRBtnDown)//�������ͬʱ����
						{
							m_bLRBtnDown = FALSE;

							m_pBlockArea->LRBtnUp(m_posCurBlock);//�޸���Χ״̬									

							if ((m_pBlockArea->GetAroundMineNum(m_posCurBlock) == m_pBlockArea->GetAroundFlagNum(m_posCurBlock)) &&//��Χ�ı������������
								(m_pBlockArea->GetCurState(m_posCurBlock) >= EBS_Num8))//��������Ѵ�
							{
								if (m_pBlockArea->HasCorrectFlags(m_posCurBlock))//���׼ȷ
								{
									m_pBlockArea->OpenAround(m_posCurBlock);//����Χ����

									//�ж��Ƿ�Ϊʤ��
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
						else//���������
						{								
							if (m_pBlockArea->GetOldState(m_posCurBlock) == EBS_Normal)//������֮ǰ�÷���δ���κα��
							{
								if (m_pBlockArea->IsMine(m_posCurBlock))//��Ϊ�ף�ֱ��gg
								{
									m_pBlockArea->DeadAt(m_posCurBlock);
									Dead();
								}
								else
								{
									m_pBlockArea->Open(m_posCurBlock);//�����ף���									
								}								
							}
							else if (m_pBlockArea->GetOldState(m_posCurBlock) == EBS_Dicey)//�÷�����Ϊ�ʺ�
							{
								m_pBlockArea->SetCurState(m_posCurBlock, EBS_Dicey);
							}

							//�ж��Ƿ�Ϊʤ��
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
	else//�ǽ�������
	{
		if (m_uGameState == EGS_Wait || m_uGameState == EGS_Run)
		{
			m_uBtnState = EBtnS_Normal;
			//InvalidateRect(&m_rectButton);
		}
	}

	Invalidate();//���������ȫ�ػ�
	CWnd::OnLButtonUp(nFlags, pt);
}

void CMineWnd::OnRButtonDown(uint nFlags, CPoint pt)
{
	m_bLRBtnDown = FALSE;

	if (IsInBlockArea(pt))//�����������	
	{
		if (m_uGameState == EGS_Wait || m_uGameState == EGS_Run)
		{
			GetBlock(pt.x, pt.y, m_posCurBlock);//����ʧ��

			if ((nFlags & MK_LBUTTON) && (nFlags & MK_RBUTTON))//���Ҽ�ͬʱ����
			{
				m_bLRBtnDown = TRUE;
				m_pBlockArea->LRBtnDown(m_posCurBlock);//�޸���Χ״̬					
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
	if (m_bLRBtnDown)//���Ҽ�ͬʱ����
	{
		m_bLRBtnDown = FALSE;

		if (GetBlock(pt.x, pt.y, m_posCurBlock))
		{
			m_pBlockArea->LRBtnUp(m_posCurBlock);
		}				

		//����������ɿ�ʱ�Ĵ�����ͬ
		if ((m_pBlockArea->GetAroundMineNum(m_posCurBlock) == m_pBlockArea->GetAroundFlagNum(m_posCurBlock)) &&//��Χ�ı������������
			(m_pBlockArea->GetCurState(m_posCurBlock) >= EBS_Num8))//��������Ѵ�
		{
			if (m_pBlockArea->HasCorrectFlags(m_posCurBlock))//���׼ȷ
			{
				m_pBlockArea->OpenAround(m_posCurBlock);//����Χ����

				//�ж��Ƿ�Ϊʤ��
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
		if (IsInBtn(pt))//��ť����
		{
			m_uBtnState = EBtnS_Down;
			//InvalidateRect(&m_rectButton);
		}
		else if (IsInBlockArea(pt))//��������
		{
			switch (m_uGameState)
			{
			case EGS_Wait:
			case EGS_Run:
			{
							GetBlock(pt.x, pt.y, m_posCurBlock);

							if (m_posCurBlock.x != m_posOldBlock.x || m_posCurBlock.y != m_posOldBlock.y)//�ƶ����·���
							{
								//�����·���״̬
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
								//�ָ��Ϸ��鱣����ϵ�״̬
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
									m_pBlockArea->LRBtnUp(m_posOldBlock);//����
									m_pBlockArea->LRBtnDown(m_posCurBlock);//����				
								}
								else
								{
									m_pBlockArea->LBtnUp(m_posOldBlock);//����
									m_pBlockArea->LBtnDown(m_posCurBlock);//����	
								}

								m_posOldBlock = m_posCurBlock;//���浱ǰλ��
							}
			}
				break;
			case EGS_Victory: 
			case EGS_Dead:				
			default:
				break;
			}
		}
		else//�ǽ�������
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
	SetCheckedColor();//�˵�
	m_pCfgMgr->SetColorful(m_bColorful);//���������ļ�

	LoadBitmap();
	Invalidate();
}

void CMineWnd::OnMenuSound()
{
	m_bSoundful = !m_bSoundful;
	SetCheckedSound();
	m_pCfgMgr->SetSoundful(m_bColorful);//���������ļ�

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
	ShellAbout(this->m_hWnd, TEXT("ɨ��"), TEXT("my mine game"), nullptr);
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
		AfxMessageBox(TEXT("��ʼ���˵�ʧ�ܣ�"));
		PostQuitMessage(0);
	}
	else
	{
		SetCheckedLevel();
		SetCheckedColor();
		SetCheckedSound();
	}
}

//��ť����
bool CMineWnd::IsInBtn(CPoint pt)
{
	return CRect(m_rectButton).PtInRect(pt);
}

//��ť����
bool CMineWnd::IsInBlockArea(CPoint pt)
{
	return CRect(m_rectBlockArea).PtInRect(pt);
}