#ifndef __NEW_RECORD_DLG_H__
#define __NEW_RECORD_DLG_H__

using namespace def;

class CNewRecordDlg : public CDialog
{
public:
    CNewRecordDlg(CWnd* pParent = nullptr);  
    
    enum { IDD = IDD_DIALOG_NEWRECORD };

    static bool IsNewRecord(uint time);

    void Init(uint time){ m_uTime = time; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    
    afx_msg void OnBnClickedOk();
    DECLARE_MESSAGE_MAP()
    
private:
    uint m_uTime;   
};

#endif // __NEW_RECORD_DLG_H__
