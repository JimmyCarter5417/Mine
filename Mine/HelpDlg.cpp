// HelpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Mine.h"
#include "HelpDlg.h"
#include "afxdialogex.h"


// CHelpDlg �Ի���

IMPLEMENT_DYNAMIC(CHelpDlg, CDialog)

CHelpDlg::CHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpDlg::IDD, pParent)
{

}

CHelpDlg::~CHelpDlg()
{
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialog)
	
END_MESSAGE_MAP()

BOOL CHelpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ListBox.AddString(TEXT("F1: ����"));
	m_ListBox.AddString(TEXT("F2: ����"));
	m_ListBox.AddString(TEXT("F3: ��ʼ"));
	m_ListBox.AddString(TEXT("F4: �˳�"));
	m_ListBox.AddString(TEXT("F5: �ϵ��ӽ�"));
	m_ListBox.AddString(TEXT("F6: ����"));
	m_ListBox.AddString(TEXT("F7: ��ɫ"));

	return TRUE;
}
