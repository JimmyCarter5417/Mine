#include "stdafx.h"
#include "mine.h"
#include "CustomDlg.h"
#include "MineWnd.h"

CCustomDlg::CCustomDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CCustomDlg::IDD, pParent)
{
	m_uHeight = 0;
	m_uNumber = 0;
	m_uWidth = 0;
}

void CCustomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_HEIGHT, m_uHeight);
	DDX_Text(pDX, IDC_NUMBER, m_uNumber);
	DDX_Text(pDX, IDC_WIDTH, m_uWidth);
}

BEGIN_MESSAGE_MAP(CCustomDlg, CDialog)

END_MESSAGE_MAP()

void CCustomDlg::OnOK() 
{
	UpdateData();

	if (m_uNumber > m_uWidth * m_uHeight) 
		m_uNumber = m_uWidth * m_uHeight - 1;

	if (CMineWnd *pMine = (CMineWnd*)AfxGetMainWnd())
		pMine->SetCustom(m_uHeight, m_uWidth, m_uNumber);
	
	CDialog::OnOK();
}

void CCustomDlg::InitData(uint xNum, uint yNum, uint mNum)
{
	m_uWidth = xNum;
	m_uHeight = yNum;
	m_uNumber = mNum;
}
