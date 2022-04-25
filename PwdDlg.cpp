// PwdDlg.cpp : implementation file
//

#include "pch.h"
#include "MiddleBro.h"
#include "PwdDlg.h"
#include "afxdialogex.h"


// PwdDlg dialog

IMPLEMENT_DYNAMIC(PwdDlg, CDialog)

PwdDlg::PwdDlg(const CString& pwd, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PwdDlg, pParent),
	pwd(pwd)
	, enteredPwd(_T(""))
{
}

PwdDlg::~PwdDlg()
{
}

void PwdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PWD, enteredPwd);
}


BEGIN_MESSAGE_MAP(PwdDlg, CDialog)
END_MESSAGE_MAP()


// PwdDlg message handlers

bool PwdDlg::ShowCheckPwd(const CString& pwd)
{
	PwdDlg dlg(pwd);
	return dlg.DoModal() == IDOK;
}


void PwdDlg::OnOK()
{
	UpdateData(TRUE);
	if (pwd == enteredPwd)
	{
		CDialog::OnOK();
	}
	else
	{
		SetWindowText(_T("Incorrect password ! Re-enter please"));
		FlashWindow(false);
	}
}
