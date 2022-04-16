#pragma once


// BlockingDlg dialog

class BlockingDlg : public CDialog
{
	DECLARE_DYNAMIC(BlockingDlg)

public:
	BlockingDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~BlockingDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BlockingDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	static void Show();
	
	static BlockingDlg dlg;
};
