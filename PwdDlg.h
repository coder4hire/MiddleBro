#pragma once


// PwdDlg dialog

#define EXIT_PWD _T("exit")
#define MAIN_PWD _T("set")

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
	static bool ShowCheckPwd(const CString& pwd);
	CString enteredPwd;
	virtual void OnOK();
};
