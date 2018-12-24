#include "stdafx.h"
#include <afxdialogex.h>
#include "Def.h"
#include "Mine.h"
#include "HelpDlg.h"



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

    m_ListBox.AddString(TEXT("F1: 帮助"));
    m_ListBox.AddString(TEXT("F2: 关于"));
    m_ListBox.AddString(TEXT("F3: 开始"));
    m_ListBox.AddString(TEXT("F4: 退出"));
    m_ListBox.AddString(TEXT("F5: 上帝视角"));
    m_ListBox.AddString(TEXT("F6: 声音"));
    m_ListBox.AddString(TEXT("F7: 颜色"));

    return TRUE;
}
