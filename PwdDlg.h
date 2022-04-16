#pragma once


// PwdDlg dialog

class PwdDlg : public CDialog
{
	DECLARE_DYNAMIC(PwdDlg)

public:
	PwdDlg(const CString& pwd, CWnd* pParent = nullptr);   // standard constructor
	virtual ~PwdDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PwdDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CString pwd;
public:
	virtual INT_PTR DoModal();
	static bool ShowCheckPwd(const CString& pwd);
	CString enteredPwd;
};
