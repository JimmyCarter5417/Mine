#ifndef __CUSTOM_DLG_H__
#define __CUSTOM_DLG_H__

using namespace def;

class CCustomDlg : public CDialog
{
public: 
    CCustomDlg(CWnd* pParent = nullptr);
    enum { IDD = IDD_DIALOG_CUSTOM };

public:
    afx_msg void OnBnClickedOk();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
    
private:
    uint m_uRowNum;
    uint m_uColNum;
    uint m_uMineNum;
};

#endif//__CUSTOM_DLG_H__
