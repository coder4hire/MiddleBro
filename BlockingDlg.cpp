// BlockingDlg.cpp : implementation file
//

#include "pch.h"
#include "MiddleBro.h"
#include "BlockingDlg.h"
#include "afxdialogex.h"

BlockingDlg BlockingDlg::dlg;

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
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// BlockingDlg message handlers


void BlockingDlg::OnKillFocus(CWnd* pNewWnd)
{
	this->SetFocus();

}


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

	SetTimer(0, 10000, NULL);

	auto wnd = FindWindow(_T("Shell_TrayWnd"), NULL);
	wnd->SendMessage(WM_COMMAND, 419, 0); // MIN_ALL

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

void BlockingDlg::Show()
{
	if (!dlg)
	{
		dlg.Create(IDD_BlockingDlg);
	}
	else
	{
		dlg.ShowWindow(WM_SHOWWINDOW);
	}
}