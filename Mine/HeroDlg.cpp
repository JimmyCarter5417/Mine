#include "stdafx.h"
#include "Resource.h"
#include "HeroDlg.h"
#include "CfgMgr.h"
#include "Def.h"

CHeroDlg::CHeroDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CHeroDlg::IDD, pParent)
{
	m_strPrimaryHolder = def::g_strDefHolder;
	m_strMediumHolder = def::g_strDefHolder;
	m_strAdvancedHolder = def::g_strDefHolder;
	m_nPrimaryRecord = def::g_nDefRecord;
	m_nMediumRecord = def::g_nDefRecord;
	m_nAdvancedRecord = def::g_nDefRecord;
}

void CHeroDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_STATIC_PRIMARY_HOLDER,  m_strPrimaryHolder);
	DDX_Text(pDX, IDC_STATIC_MEDIUM_HOLDER,   m_strMediumHolder);
	DDX_Text(pDX, IDC_STATIC_ADVANCED_HOLDER, m_strAdvancedHolder);
	DDX_Text(pDX, IDC_STATIC_PRIMARY_RECORD,  m_nPrimaryRecord);
	DDX_Text(pDX, IDC_STATIC_MEDIUM_RECORD,   m_nMediumRecord);
	DDX_Text(pDX, IDC_STATIC_ADVANCED_RECORD, m_nAdvancedRecord);
}

BEGIN_MESSAGE_MAP(CHeroDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnReset)
END_MESSAGE_MAP()

BOOL CHeroDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_strPrimaryHolder = CCfgMgr::GetInstance()->GetPrimaryHolder();
	m_strMediumHolder = CCfgMgr::GetInstance()->GetMediumHolder();
	m_strAdvancedHolder = CCfgMgr::GetInstance()->GetAdvancedHolder();
	m_nPrimaryRecord = CCfgMgr::GetInstance()->GetPrimaryRecord();
	m_nMediumRecord = CCfgMgr::GetInstance()->GetMediumRecord();
	m_nAdvancedRecord = CCfgMgr::GetInstance()->GetAdvancedRecord();

	UpdateData(FALSE);

	return TRUE;
}

void CHeroDlg::OnReset() 
{
	/*m_strPrimaryHolder = def::g_strDefHolder;
	m_strMediumHolder = def::g_strDefHolder;
	m_strAdvancedHolder = def::g_strDefHolder;*/
	m_nPrimaryRecord = def::g_nDefRecord;
	m_nMediumRecord = def::g_nDefRecord;
	m_nAdvancedRecord = def::g_nDefRecord;

	CCfgMgr::GetInstance()->SetPrimaryRecord(m_nPrimaryRecord);
	CCfgMgr::GetInstance()->SetMediumRecord(m_nMediumRecord);
	CCfgMgr::GetInstance()->SetAdvancedRecord(m_nAdvancedRecord);

	UpdateData(FALSE);
}
