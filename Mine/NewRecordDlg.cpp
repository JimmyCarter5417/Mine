#include "stdafx.h"
#include "Mine.h"
#include "NewRecordDlg.h"
#include "MineDefs.h"


CNewRecordDlg::CNewRecordDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CNewRecordDlg::IDD, pParent)
{	
	m_strName = TEXT("匿名");
	m_strDesc = TEXT("");
}

void CNewRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_DESCRIBE, m_strDesc);	
}

BEGIN_MESSAGE_MAP(CNewRecordDlg, CDialog)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void CNewRecordDlg::SetLevel(UINT level)
{
	CString strLevel;
	if (level == LEVEL_PRIMARY) strLevel = TEXT("初级");
	else if (level == LEVEL_SECONDRY) strLevel = TEXT("中级");
	else if (level == LEVEL_ADVANCE) strLevel = TEXT("高级");
	m_strDesc.Format(TEXT("已破%s记录\n\r请留尊姓大名"), strLevel);
}

CString CNewRecordDlg::GetName()
{
	return m_strName;
}
