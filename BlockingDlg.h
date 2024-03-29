#pragma once


// BlockingDlg dialog
#define WM_BLOCKING_REMOVED WM_USER+2

enum BLOCKING_CASES
{
	BC_BREAK,
	BC_NO_MORE_LIMITED_TIME
};

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
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	static void Show(DWORD id, const CString& message, int timeout = 0);
	static bool IsVisible() { return ::IsWindow(dlg.GetSafeHwnd()); }
	
	static BlockingDlg dlg;
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedButtonContinue();
	virtual void OnOK();
	virtual void OnCancel();

protected:
	void Close();
	DWORD id = 0xFFFFFFFF;
	CRect calculatedRect;
	int timeout;
	CTime startTime;
public:
//	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
