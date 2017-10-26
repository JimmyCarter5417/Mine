#ifndef __HERO_DLG_H__
#define __HERO_DLG_H__

class CHeroDlg : public CDialog
{
public:	
	CHeroDlg(CWnd* pParent = nullptr); 

	enum { IDD = IDD_DIALOG_HERO };
		
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	afx_msg void OnReset();

	DECLARE_MESSAGE_MAP()

public:
	CString m_strPrimaryHolder;
	CString m_strMediumHolder;
	CString m_strAdvancedHolder;
	int     m_nPrimaryRecord;
	int     m_nMediumRecord;
	int     m_nAdvancedRecord;
};

#endif//__HERO_DLG_H__
