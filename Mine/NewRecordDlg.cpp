#include "stdafx.h"
#include "Def.h"
#include "Mine.h"
#include "CfgMgr.h"
#include "NewRecordDlg.h"

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
    LEVEL_E level;
    CCfgMgr::GetInstance()->GetMineInfo(row, col, mine, level); 
    
    switch (level)
    {
    case LEVEL_Primary:
        return time < CCfgMgr::GetInstance()->GetPrimaryRecord();
    case LEVEL_Medium:
        return time < CCfgMgr::GetInstance()->GetMediumRecord();
    case LEVEL_Advanced:
        return time < CCfgMgr::GetInstance()->GetAdvancedRecord();
    default:
        return false;
    }

    return false;
}

BEGIN_MESSAGE_MAP(CNewRecordDlg, CDialog)   
    ON_BN_CLICKED(IDOK, &CNewRecordDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CNewRecordDlg::OnBnClickedOk()
{
    uint row, col, mine;
    LEVEL_E level;
    CCfgMgr::GetInstance()->GetMineInfo(row, col, mine, level);

    CString strHolder;
    GetDlgItem(IDC_EDIT_NAME)->GetWindowTextW(strHolder);

    switch (level)
    {
    case LEVEL_Primary:
        CCfgMgr::GetInstance()->SetPrimaryRecord(m_uTime);
        CCfgMgr::GetInstance()->SetPrimaryHolder(strHolder);
        break;
    case LEVEL_Medium:
        CCfgMgr::GetInstance()->SetMediumRecord(m_uTime);
        CCfgMgr::GetInstance()->SetMediumHolder(strHolder);
        break;
    case LEVEL_Advanced:
        CCfgMgr::GetInstance()->SetPrimaryRecord(m_uTime);
        CCfgMgr::GetInstance()->SetPrimaryHolder(strHolder);
        break;
    case LEVEL_Custom:
        return;
    default:
        break;
    }

    CDialog::OnOK();
}
