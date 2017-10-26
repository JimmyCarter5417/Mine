#include "stdafx.h"
#include "Resource.h"
#include "CustomDlg.h"
#include "CfgMgr.h"

CCustomDlg::CCustomDlg(CWnd* pParent /*=nullptr*/)
: CDialog(CCustomDlg::IDD, pParent)
, m_uRowNum(def::g_nDefMediumRowNum)
, m_uColNum(def::g_nDefMediumColNum)
, m_uMineNum(def::g_nDefMediumMineNum)
{

}

void CCustomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_ROW_NUM, m_uRowNum);
	DDX_Text(pDX, IDC_EDIT_COL_NUM, m_uColNum);
	DDX_Text(pDX, IDC_EDIT_MINE_NUM, m_uMineNum);
}

BEGIN_MESSAGE_MAP(CCustomDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CCustomDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CCustomDlg::OnBnClickedOk()
{
	UpdateData();

	if (m_uMineNum > m_uRowNum * m_uColNum)
		m_uMineNum = m_uRowNum * m_uColNum - 1;

	CCfgMgr::GetInstance()->SetMineInfo(m_uRowNum, m_uColNum, m_uMineNum, def::ELevel_Custom);

	CDialog::OnOK();
}
