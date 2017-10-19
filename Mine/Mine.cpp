#include "stdafx.h"
#include "Mine.h"
#include "MineDefs.h"

BEGIN_MESSAGE_MAP(CMineApp, CWinApp)
	
END_MESSAGE_MAP()

CMineApp::CMineApp()
{
}

CMineApp theApp;

const CString className = TEXT("MineClass");

BOOL CMineApp::InitInstance()
{
	SetRegistryKey(TEXT("CSER_513_2"));	

	//创建窗口类
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.hInstance = AfxGetInstanceHandle();
	wc.lpfnWndProc = ::DefWindowProc;
	wc.hbrBackground = nullptr;
	wc.hCursor = LoadCursor(IDC_ARROW);
	wc.hIcon = LoadIcon(IDR_MAINFRAME);
	wc.lpszClassName = className;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);

	//注册窗口类
	if (!AfxRegisterClass(&wc)) 
	{
		AfxMessageBox(TEXT("注册类失败！"));
		return FALSE;
	}

	// get the minewnd coordinates, width and height
	/*UINT uXMineNum = GetProfileInt(GAME_SETTING, CUSTOM_XNUM, PRIMARY_XNUM);
	UINT uYMineNum = GetProfileInt(GAME_SETTING, CUSTOM_YNUM, PRIMARY_YNUM);

	UINT uWidth = DEFAULT_FRAME_X + uXMineNum * MINE_WIDTH +
				LINE_WIDTH_0 * 3 + SIDE_WIDTH_0 + SIDE_WIDTH_1;
	UINT uHeight = DEFAULT_FRAME_Y + uYMineNum * MINE_HEIGHT + 
				LINE_WIDTH_0 * 3 + SIDE_WIDTH_0 * 2 + SIDE_WIDTH_1 + SHELL_S_H;
	UINT uXPos = GetSystemMetrics(SM_CXSCREEN) / 2 - uWidth / 2;
	UINT uYPos = GetSystemMetrics(SM_CYSCREEN) / 2 - uHeight / 2;*/
	
	//创建窗口
	m_pMineWnd = new CMineWnd();
	if (nullptr == m_pMineWnd || 
		!m_pMineWnd->CreateEx(0, className, TEXT("扫雷"), WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, 
			//uXPos, uYPos, uWidth, uHeight, nullptr, nullptr))	
			GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2, 0, 0, nullptr, nullptr))
	{
		AfxMessageBox(TEXT("创建主窗口失败！"));
		return FALSE;
	}
	//绑定该窗口为主窗口
	m_pMainWnd = m_pMineWnd;
	//显示窗口
	m_pMineWnd->ShowWindow(SW_NORMAL);

	return TRUE;
}

int CMineApp::ExitInstance() 
{
	delete m_pMineWnd;
	
	return CWinApp::ExitInstance();
}