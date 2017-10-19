#ifndef __HERO_DLG_H__
#define __HERO_DLG_H__

class CHeroDlg : public CDialog
{
public:	
	CHeroDlg(CWnd* pParent = nullptr); 

	enum { IDD = IDD_DLG_HERO };
	
	void SetBHolder(CString holder);
	void SetBRecord(UINT record);
	void SetIHolder(CString holder);
	void SetIRecord(UINT record);
	void SetEHolder(CString holder);
	void SetERecord(UINT record);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	afx_msg void OnReset();

	DECLARE_MESSAGE_MAP()

private:
	CString	m_strBHolder;
	CString	m_strEHolder;
	CString	m_strBRecord;
	CString	m_strERecord;
	CString	m_strIHolder;
	CString	m_strIRecord;
};

#endif//__HERO_DLG_H__