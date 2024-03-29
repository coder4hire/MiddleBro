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
//	ON_WM_SIZING()
ON_WM_SIZE()
END_MESSAGE_MAP()


// BlockingDlg message handlers

BOOL BlockingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	RECT screenRect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);

	calculatedRect = CRect((screenRect.right - screenRect.left) / 10,
		(screenRect.bottom - screenRect.top) / 10,
		(screenRect.right - screenRect.left) * 9 / 10,
		(screenRect.bottom - screenRect.top) * 9 / 10);

	this->SetWindowPos(&wndTopMost, calculatedRect.left, calculatedRect.top, 
		calculatedRect.Width(), calculatedRect.Height(),
		WS_EX_TOPMOST);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void BlockingDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 0:
	{
		auto wnd = FindWindow(_T("Shell_TrayWnd"), NULL);
		wnd->SendMessage(WM_COMMAND, 419, 0); // MIN_ALL

		this->SetWindowPos(&wndTopMost, calculatedRect.left, calculatedRect.top,
			calculatedRect.Width(), calculatedRect.Height(),
			WS_EX_TOPMOST);
		this->SetFocus();
		break;
	}
	case 1:
	{
		CTime currentTime = CTime::GetCurrentTime();
		CTimeSpan elapsed = startTime + timeout - currentTime;
		GetDlgItem(IDC_STATIC_TIMEOUT)->SetWindowText(elapsed.Format("%H:%M:%S"));
		if (elapsed<0)
		{
			Close();
		}
		break;
	}
	}

	CDialog::OnTimer(nIDEvent);
}

void BlockingDlg::Show(DWORD id, const CString& message, int timeout)
{
	if (!::IsWindow(dlg.GetSafeHwnd()))
	{
		dlg.Create(IDD_BlockingDlg);
	}
	else
	{
		if (dlg.id != 0xFFFFFFFF)
		{
			AfxGetMainWnd()->SendMessage(WM_BLOCKING_REMOVED, dlg.id);
		}
	}

	dlg.id = id;
	dlg.GetDlgItem(IDC_STATIC_MESSAGE)->SetWindowText(message);
	dlg.ShowWindow(SW_SHOW);
	dlg.KillTimer(0);
	dlg.SetTimer(0, MINIMIZING_TIMEOUT, NULL);
	dlg.KillTimer(1);
	if (timeout > 0)
	{
		dlg.SetTimer(1, 1000, NULL);
	}
	dlg.timeout = timeout;
	dlg.startTime = CTime::GetCurrentTime();
	
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
	KillTimer(0);
	dlg.SetTimer(0, 2*MINIMIZING_TIMEOUT, NULL);
	if (PwdDlg::ShowCheckPwd(MAIN_PWD))
	{
		Close();
		return;
	}
	KillTimer(0);
	dlg.SetTimer(0, MINIMIZING_TIMEOUT, NULL);
}


void BlockingDlg::OnOK()
{
}


void BlockingDlg::OnCancel()
{
}

void BlockingDlg::Close()
{
	KillTimer(0);
	AfxGetMainWnd()->SendMessage(WM_BLOCKING_REMOVED, id);
	id = 0xFFFFFFFF;
	DestroyWindow();
}


void BlockingDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rect;
	if (GetDlgItem(IDC_STATIC_MESSAGE) && GetDlgItem(IDC_BUTTON_CONTINUE))
	{
		GetDlgItem(IDC_STATIC_MESSAGE)->GetWindowRect(&rect);
		GetDlgItem(IDC_STATIC_MESSAGE)->MoveWindow(15, (calculatedRect.Height() - rect.Height()) / 2,
			calculatedRect.Width() - 30, rect.Height());

		GetDlgItem(IDC_BUTTON_CONTINUE)->GetWindowRect(&rect);
		GetDlgItem(IDC_BUTTON_CONTINUE)->SetWindowPos(NULL, (calculatedRect.Width() - rect.Width()) - 15, (calculatedRect.Height() - rect.Height()) - 15,
			0, 0, SWP_NOSIZE | SWP_NOZORDER);

		GetDlgItem(IDC_STATIC_TIMEOUT)->GetWindowRect(&rect);
		GetDlgItem(IDC_STATIC_TIMEOUT)->SetWindowPos(NULL, 15, (calculatedRect.Height() - rect.Height()) - 15,
			0, 0, SWP_NOSIZE | SWP_NOZORDER);

	}
}
