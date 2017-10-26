#include "stdafx.h"
#include "Mine.h"
#include "NewRecordDlg.h"
#include "Def.h"
#include "CfgMgr.h"

using namespace def;

CNewRecordDlg::CNewRecordDlg(CWnd* pParent /*=nullptr*/)
: CDialog(CNewRecordDlg::IDD, pParent)
, m_uTime(def::g_nDefRecord)
{

}

void CNewRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

bool CNewRecordDlg::IsNewRecord(uint time)
{
	uint row, col, mine;
	ELevel level;
	CCfgMgr::GetInstance()->GetMineInfo(row, col, mine, level);	
	
	switch (level)
	{
	case ELevel_Primary:
		return time < CCfgMgr::GetInstance()->GetPrimaryRecord();
	case ELevel_Medium:
		return time < CCfgMgr::GetInstance()->GetMediumRecord();				
	case ELevel_Advanced:
		return time < CCfgMgr::GetInstance()->GetAdvancedRecord();		
	default:
		return false;
	}
}

BEGIN_MESSAGE_MAP(CNewRecordDlg, CDialog)	
	ON_BN_CLICKED(IDOK, &CNewRecordDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CNewRecordDlg::OnBnClickedOk()
{
	uint row, col, mine;
	ELevel level;
	CCfgMgr::GetInstance()->GetMineInfo(row, col, mine, level);

	CString strHolder;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowTextW(strHolder);

	switch (level)
	{
	case ELevel_Primary:
		CCfgMgr::GetInstance()->SetPrimaryRecord(m_uTime);
		CCfgMgr::GetInstance()->SetPrimaryHolder(strHolder);
		break;
	case ELevel_Medium:
		CCfgMgr::GetInstance()->SetMediumRecord(m_uTime);
		CCfgMgr::GetInstance()->SetMediumHolder(strHolder);
		break;
	case ELevel_Advanced:
		CCfgMgr::GetInstance()->SetPrimaryRecord(m_uTime);
		CCfgMgr::GetInstance()->SetPrimaryHolder(strHolder);
		break;
	case ELevel_Custom:
		return;
	default:
		break;
	}

	CDialog::OnOK();
}
