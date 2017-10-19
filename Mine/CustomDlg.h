#ifndef __CUSTOM_DLG_H__
#define __CUSTOM_DLG_H__

class CCustomDlg : public CDialog
{
public:	
	CCustomDlg(CWnd* pParent = nullptr);  	

	enum { IDD = IDD_DLG_CUSTOM };
	
	void InitData(UINT xNum, UINT yNum, UINT mNum);	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
	
private:
	UINT	m_uHeight;
	UINT	m_uNumber;
	UINT	m_uWidth;
};

#endif//__CUSTOM_DLG_H__
