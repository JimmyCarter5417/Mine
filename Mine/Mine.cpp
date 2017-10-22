#include "stdafx.h"
#include "Mine.h"
#include "Def.h"

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

	//创建窗口
	m_pMineWnd = new CMineWnd();
	if (nullptr == m_pMineWnd || 
		!m_pMineWnd->CreateEx(0, className, TEXT("扫雷"), WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, 
			GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2, 0, 0, nullptr, nullptr))
			//实际窗口大小在CMineWnd内部设置
	{
		AfxMessageBox(TEXT("创建主窗口失败！"));
		return FALSE;
	}
	//绑定为主窗口
	m_pMainWnd = m_pMineWnd;
	m_pMineWnd->ShowWindow(SW_NORMAL);

	return TRUE;
}

int CMineApp::ExitInstance() 
{
	delete m_pMineWnd;
	
	return CWinApp::ExitInstance();
}