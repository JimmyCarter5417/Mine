#ifndef __CUSTOM_DLG_H__
#define __CUSTOM_DLG_H__

class CCustomDlg : public CDialog
{
public:	
	CCustomDlg(CWnd* pParent = nullptr);  	

	enum { IDD = IDD_DIALOG_CUSTOM };
	
	void InitData(uint xNum, uint yNum, uint mNum);	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
	
private:
	uint	m_uHeight;
	uint	m_uNumber;
	uint	m_uWidth;
};

#endif//__CUSTOM_DLG_H__
