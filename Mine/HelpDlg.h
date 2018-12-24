#pragma once

#include "resource.h"
#include "afxwin.h"

using namespace def;

class CHelpDlg : public CDialog
{
    DECLARE_DYNAMIC(CHelpDlg)

public:
    CHelpDlg(CWnd* pParent = NULL);
    virtual ~CHelpDlg();

    enum { IDD = IDD_DIALOG_HELP };

    BOOL OnInitDialog();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()
public:
    CListBox m_ListBox;
};
