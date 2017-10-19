#include "stdafx.h"
#include "mine.h"
#include "HeroDlg.h"
#include "MineWnd.h"
#include "MineDefs.h"

CHeroDlg::CHeroDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CHeroDlg::IDD, pParent)
{
	m_strBHolder = TEXT("");
	m_strEHolder = TEXT("");
	m_strBRecord = TEXT("");
	m_strERecord = TEXT("");
	m_strIHolder = TEXT("");
	m_strIRecord = TEXT("");
}

void CHeroDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Text(pDX, IDC_B_H, m_strBHolder);
	DDX_Text(pDX, IDC_E_H, m_strEHolder);
	DDX_Text(pDX, IDC_B_R, m_strBRecord);
	DDX_Text(pDX, IDC_E_R, m_strERecord);
	DDX_Text(pDX, IDC_I_H, m_strIHolder);
	DDX_Text(pDX, IDC_I_R, m_strIRecord);
}

BEGIN_MESSAGE_MAP(CHeroDlg, CDialog)
	ON_BN_CLICKED(IDC_RESET, OnReset)
END_MESSAGE_MAP()

void CHeroDlg::SetBHolder(CString holder)
{
	m_strBHolder = holder;
}

void CHeroDlg::SetBRecord(UINT record)
{
	m_strBRecord.Format(TEXT("%d��"), record);
}

void CHeroDlg::SetIHolder(CString holder)
{
	m_strIHolder = holder;
}

void CHeroDlg::SetIRecord(UINT record)
{
	m_strIRecord.Format(TEXT("%d��"), record);
}

void CHeroDlg::SetEHolder(CString holder)
{
	m_strEHolder = holder;
}

void CHeroDlg::SetERecord(UINT record)
{
	m_strERecord.Format(TEXT("%d��"), record);
}

void CHeroDlg::OnReset() 
{
	CMineWnd *pMine = (CMineWnd*)AfxGetMainWnd();
	pMine->ResetRecord();
	m_strBHolder = DEFAULT_HOLDER;
	m_strBRecord.Format(TEXT("%d��"), DEFAULT_RECORD);
	m_strIHolder = DEFAULT_HOLDER;
	m_strIRecord.Format(TEXT("%d��"), DEFAULT_RECORD);
	m_strEHolder = DEFAULT_HOLDER;
	m_strERecord.Format(TEXT("%d��"), DEFAULT_RECORD);
	UpdateData(FALSE);
}
