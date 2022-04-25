// BlockingDlg.cpp : implementation file
//

#include "pch.h"
#include "MiddleBro.h"
#include "BlockingDlg.h"
#include "afxdialogex.h"
#include "PwdDlg.h"

BlockingDlg BlockingDlg::dlg;

#define MINIMIZING_TIMEOUT 10000

// BlockingDlg dialog

IMPLEMENT_DYNAMIC(BlockingDlg, CDialog)

BlockingDlg::BlockingDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_BlockingDlg, pParent)
{

}

BlockingDlg::~BlockingDlg()
{
}

void BlockingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(BlockingDlg, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CONTINUE, &BlockingDlg::OnBnClickedButtonContinue)
END_MESSAGE_MAP()


// BlockingDlg message handlers

BOOL BlockingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	RECT screenRect, rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);
	GetWindowRect(&rect);

	this->SetWindowPos(&wndTopMost, (screenRect.right - screenRect.left) / 10,
		(screenRect.bottom - screenRect.top) / 10,
		(screenRect.right + screenRect.left) * 4 / 5,
		(screenRect.bottom + screenRect.top) * 4 / 5,
		WS_EX_TOPMOST);

	//CMenu* pSystemMenu = GetSystemMenu(FALSE);
	//pSystemMenu->EnableMenuItem(SC_MOVE, MF_BYCOMMAND | MF_GRAYED);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void BlockingDlg::OnTimer(UINT_PTR nIDEvent)
{
	auto wnd = FindWindow(_T("Shell_TrayWnd"), NULL);
	wnd->SendMessage(WM_COMMAND, 419, 0); // MIN_ALL

	CDialog::OnTimer(nIDEvent);
}

void BlockingDlg::Show(LPCTSTR message)
{
	if (!::IsWindow(dlg.GetSafeHwnd()))
	{
		dlg.Create(IDD_BlockingDlg);
	}

	dlg.GetDlgItem(IDC_STATIC_MESSAGE)->SetWindowText(message);
	dlg.ShowWindow(SW_SHOW);
	dlg.SetTimer(0, MINIMIZING_TIMEOUT, NULL);
	
	auto wnd = FindWindow(_T("Shell_TrayWnd"), NULL);
	wnd->SendMessage(WM_COMMAND, 419, 0); // MIN_ALL
}

BOOL BlockingDlg::DestroyWindow()
{
	KillTimer(0);
	return CDialog::DestroyWindow();
}


void BlockingDlg::OnBnClickedButtonContinue()
{
	if (PwdDlg::ShowCheckPwd(_T("cont")))
	{
		KillTimer(0);
		AfxGetMainWnd()->SendMessage(WM_BLOCKING_REMOVED);
		DestroyWindow();
		return;
	}
}


void BlockingDlg::OnOK()
{
}


void BlockingDlg::OnCancel()
{
}
