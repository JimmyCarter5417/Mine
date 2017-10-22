#ifndef __NEW_RECORD_DLG_H__
#define __NEW_RECORD_DLG_H__

class CNewRecordDlg : public CDialog
{
public:
	CNewRecordDlg(CWnd* pParent = nullptr);  
	
	enum { IDD = IDD_DLG_NEWRECORD };
	
	void SetLevel(uint level);
	CString GetName();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
	
private:
	CString	m_strName;
	CString	m_strDesc;
};

#endif//__NEW_RECORD_DLG_H__
