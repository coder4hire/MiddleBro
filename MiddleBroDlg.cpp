
// MiddleBroDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MiddleBro.h"
#include "MiddleBroDlg.h"
#include "afxdialogex.h"

#include "StatisticsDlg.h"
#include "PwdDlg.h"
#include <Mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMiddleBroDlg dialog



CMiddleBroDlg::CMiddleBroDlg(CWnd* pParent /*=nullptr*/)
	: CDialogTray(IDD_MIDDLEBRO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMiddleBroDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogTray::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TIME_LEFT, lblTimeLeft);
	DDX_Control(pDX, IDC_STATIC_TIME_ELAPSED, lblTimeElapsed);
}

BEGIN_MESSAGE_MAP(CMiddleBroDlg, CDialogTray)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_STAT, &CMiddleBroDlg::OnBnClickedButtonStat)
	ON_COMMAND(ID_DUMMY_EXIT, &CMiddleBroDlg::OnDummyExit)
	ON_COMMAND(ID_DUMMY_SHOW, &CMiddleBroDlg::OnDummyShow)
END_MESSAGE_MAP()


// CMiddleBroDlg message handlers

BOOL CMiddleBroDlg::OnInitDialog()
{
	CDialogTray::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	CFont font;
	font.CreateFont(
		1,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		TRUE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial"));                 // lpszFacename

	lblTimeLeft.SetFont(&font);

	startTime = CTime::GetCurrentTime();
	SetTimer(0, 800, NULL);

	ShowTrayIcon();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMiddleBroDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogTray::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMiddleBroDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMiddleBroDlg::OnTimer(UINT_PTR nIDEvent)
{
	auto tmElapsed = (CTime::GetCurrentTime() - startTime);
	auto tmLeft = CTimeSpan(timeLimit) - tmElapsed;

	lblTimeLeft.SetWindowText(tmLeft.Format("%H:%M:%S"));
	lblTimeElapsed.SetWindowText(tmElapsed.Format("%H:%M:%S"));

	if (tmLeft == timeFirstSignal)
	{
		PlaySound(MAKEINTRESOURCE(IDR_WAVE_1RING),
			GetModuleHandle(NULL), 
			SND_RESOURCE | SND_ASYNC);

		ShowBaloon(_T("Middle Bro"), _T("Session will be blocked soon.\r\nTime left: " + tmLeft.Format("%H:%M:%S")));
	}

	if (tmLeft <= 0)
	{
		PlaySound(MAKEINTRESOURCE(IDR_WAVE_END),
			GetModuleHandle(NULL),
			SND_RESOURCE | SND_ASYNC);
		ShowBaloon(_T("Middle Bro"), _T("Session time is over."));
		OnTimeExpired();
		KillTimer(0);
	}

	CDialogTray::OnTimer(nIDEvent);
}


void CMiddleBroDlg::OnBnClickedButtonStat()
{
	StatisticsDlg dlg;
	dlg.DoModal();
}


void CMiddleBroDlg::OnDummyExit()
{
	//TODO: change to real password from settings
	if (PwdDlg::ShowCheckPwd(_T("exit")))
	{
		RemoveTrayIcon();
		CDialogEx::OnCancel();
	}
}


void CMiddleBroDlg::OnDummyShow()
{
	RestoreFromTray();
}

void CMiddleBroDlg::OnTimeExpired()
{
	BlockingDlg::Show();
}
