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

	//����������
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.hInstance = AfxGetInstanceHandle();
	wc.lpfnWndProc = ::DefWindowProc;
	wc.hbrBackground = nullptr;
	wc.hCursor = LoadCursor(IDC_ARROW);
	wc.hIcon = LoadIcon(IDR_MAINFRAME);
	wc.lpszClassName = className;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);

	//ע�ᴰ����
	if (!AfxRegisterClass(&wc)) 
	{
		AfxMessageBox(TEXT("ע����ʧ�ܣ�"));
		return FALSE;
	}

	//��������
	m_pMineWnd = new CMineWnd();
	if (nullptr == m_pMineWnd || 
		!m_pMineWnd->CreateEx(0, className, TEXT("ɨ��"), WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, 
			GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2, 0, 0, nullptr, nullptr))
			//ʵ�ʴ��ڴ�С��CMineWnd�ڲ�����
	{
		AfxMessageBox(TEXT("����������ʧ�ܣ�"));
		return FALSE;
	}
	//��Ϊ������
	m_pMainWnd = m_pMineWnd;
	m_pMineWnd->ShowWindow(SW_NORMAL);

	return TRUE;
}

int CMineApp::ExitInstance() 
{
	delete m_pMineWnd;
	
	return CWinApp::ExitInstance();
}