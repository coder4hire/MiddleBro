
// MiddleBroDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MiddleBro.h"
#include "MiddleBroDlg.h"
#include "afxdialogex.h"

#include "PwdDlg.h"
#include "Settings.h"
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
	DDX_Control(pDX, IDC_STATIC_TIME, ctrlClock);
}

BEGIN_MESSAGE_MAP(CMiddleBroDlg, CDialogTray)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_COMMAND(ID_DUMMY_EXIT, &CMiddleBroDlg::OnDummyExit)
	ON_COMMAND(ID_DUMMY_SHOW, &CMiddleBroDlg::OnDummyShow)
	ON_COMMAND(ID_DUMMY_SETTINGS, &CMiddleBroDlg::OnDummySettings)
	ON_WM_POWERBROADCAST()
	ON_MESSAGE(WM_BLOCKING_REMOVED, &CMiddleBroDlg::OnBlockingRemoved)
	ON_COMMAND(ID_DUMMY_STATISTICS, &CMiddleBroDlg::OnDummyStatistics)
END_MESSAGE_MAP()


// CMiddleBroDlg message handlers

void CMiddleBroDlg::RestartCounters()
{
	SetTimer(0, 900, NULL);
	displayTime = 0;
	lastTimeCheck = CTime::GetCurrentTime();
}

BOOL CMiddleBroDlg::OnInitDialog()
{
	CDialogTray::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	Settings::Inst.ProcessProperties(NULL, false);
	if (!Settings::Inst.LoadDataFromFiles(Settings::Inst.ConfigFilesLocation + _T("\\settings.json")))
	{
		AfxMessageBox(_T("Cannot load config files from:\r\n")+ Settings::Inst.ConfigFilesLocation + _T("\\settings.json"), MB_OK | MB_ICONEXCLAMATION);
	}

	startTime = CTime::GetCurrentTime();
	lastTimeCheck = startTime;
	RestartCounters();

	Tooltip = "MiddleBro";
	ShowTrayIcon();

	RegisterPowerSettingNotification(GetSafeHwnd(), &GUID_MONITOR_POWER_ON, 0);
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
	CTime now = CTime::GetCurrentTime();
	if (isMonitorOn)
	{
		displayTime += now - lastTimeCheck;
	}
	lastTimeCheck = now;

	auto tmLeft = CTimeSpan(Settings::Inst.DailyTimeLimit) - displayTime;

	ctrlClock.SetOutputTime(tmLeft, displayTime);
	
	if (tmLeft == Settings::Inst.SecondsBeforeFirstSignal)
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
		Sleep(3000);
		KillTimer(0);
		OnTimeExpired();
	}

	CDialogTray::OnTimer(nIDEvent);
}


void CMiddleBroDlg::OnDummyExit()
{
	//TODO: change to real password from settings
	if (PwdDlg::ShowCheckPwd(EXIT_PWD))
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
	Watcher::Inst.SaveStatistics();
	BlockingDlg::Show(MAKEINTRESOURCE(IDS_STRING_TURN_OFF_WARNING));
}


void CMiddleBroDlg::OnDummySettings()
{
	if (!settingsDlg && PwdDlg::ShowCheckPwd(MAIN_PWD))
	{
		settingsDlg.DoModal();
	}
	else
	{
		settingsDlg.SetFocus();
	}
}


UINT CMiddleBroDlg::OnPowerBroadcast(UINT nPowerEvent, LPARAM nEventData)
{
	if (nPowerEvent == PBT_POWERSETTINGCHANGE)
	{
		POWERBROADCAST_SETTING* pbs = (POWERBROADCAST_SETTING*)nEventData;
		if (pbs->PowerSetting == GUID_MONITOR_POWER_ON)
		{
			isMonitorOn = pbs->Data[0] != 0;
		}
	}
	return CDialogTray::OnPowerBroadcast(nPowerEvent, nEventData);
}


afx_msg LRESULT CMiddleBroDlg::OnBlockingRemoved(WPARAM wParam, LPARAM lParam)
{
	RestartCounters();
	return 0;
}


void CMiddleBroDlg::OnDummyStatistics()
{
	if (!statDlg)
	{
		statDlg.Create(IDD_StatisticsDlg);
	}
	else
	{
		statDlg.ShowWindow(SW_SHOW);
		statDlg.SetFocus();
	}
}
