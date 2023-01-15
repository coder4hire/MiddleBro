
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
	ON_COMMAND(ID_DUMMY_EXIT, &CMiddleBroDlg::OnDummyExit)
	ON_COMMAND(ID_DUMMY_SHOW, &CMiddleBroDlg::OnDummyShow)
	ON_COMMAND(ID_DUMMY_SETTINGS, &CMiddleBroDlg::OnDummySettings)
	ON_WM_POWERBROADCAST()
	ON_MESSAGE(WM_BLOCKING_REMOVED, &CMiddleBroDlg::OnBlockingRemoved)
	ON_COMMAND(ID_DUMMY_STATISTICS, &CMiddleBroDlg::OnDummyStatistics)
	ON_COMMAND(ID_MODE_LIMITED, &CMiddleBroDlg::OnModeLimited)
	ON_COMMAND(ID_MODE_UNLIMITED, &CMiddleBroDlg::OnModeUnlimited)
	ON_COMMAND(ID_MODE_WHITELISTED, &CMiddleBroDlg::OnModeWhitelisted)
END_MESSAGE_MAP()


// CMiddleBroDlg message handlers

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
		AfxMessageBox(_T("Cannot load config files from:\r\n") + Settings::Inst.ConfigFilesLocation + _T("\\settings.json"), MB_OK | MB_ICONEXCLAMATION);
	}

	startTime = CTime::GetCurrentTime();
	lastTimeCheck = startTime;
	displayTime = 0;
	workTime = 0;

	int lastSavedDay = theApp.GetProfileInt(_T("Runtime"), _T("DateSaved"), 0);
	int nowDay = (int)(CTime::GetCurrentTime().GetTime() / 3600);
	if (lastSavedDay != nowDay)
	{
		limitedTime = 0;
		theApp.WriteProfileInt(_T("Runtime"), _T("DateSaved"), nowDay);
		theApp.WriteProfileInt(_T("Runtime"), _T("LimitedTime"), 0);
	}
	else
	{
		limitedTime = theApp.GetProfileInt(_T("Runtime"), _T("LimitedTime"), 0);
	}

	srand(nowDay);
	SetTimer(0, 1000, NULL);
	ShowTimeLeft();

	Tooltip = "MiddleBro";
	ShowTrayIcon();

	RegisterPowerSettingNotification(GetSafeHwnd(), &GUID_MONITOR_POWER_ON, 0);

	Watcher::Inst.SetCallback(std::bind(&CMiddleBroDlg::OnWatcherEvent, this, std::placeholders::_1));
	RefreshMode();
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

	if (lastTimeCheck.GetDay() != now.GetDay())
	{
		theApp.WriteProfileInt(_T("Runtime"), _T("DateSaved"), (int)(now.GetTime() / 3600));
		limitedTime = 0;
		
		Settings::Inst.Mode = OM_UNLIMITED;
		RefreshMode();
	}

	if (isMonitorOn)
	{
		displayTime += now - lastTimeCheck;
	}

	if (Settings::Inst.Mode != OM_UNLIMITED)
	{
		if (isMonitorOn)
		{
			if (!BlockingDlg::IsVisible() && Watcher::Inst.IsLimitedOnScreen())
			{
				limitedTime += now - lastTimeCheck;
			}

			workTime += now - lastTimeCheck;
		}

		if (!(limitedTime.GetTotalSeconds() % timeSaveThreshold))
		{
			timeSaveThreshold = (rand() * 600 / RAND_MAX + 30);
			theApp.WriteProfileInt(_T("Runtime"), _T("LimitedTime"), (int)(limitedTime.GetTotalSeconds()));
		}
	}

	lastTimeCheck = now;

	ShowTimeLeft();

	CDialogTray::OnTimer(nIDEvent);
}

void CMiddleBroDlg::ShowTimeLeft()
{
	auto limitedTimeLeft = CTimeSpan(Settings::Inst.DailyTimeLimit) - limitedTime;
	if (limitedTimeLeft < 0)
	{
		limitedTimeLeft = 0;
	}
	auto workTimeLeft = CTimeSpan(Settings::Inst.WorkTimeLimit) - workTime;
	if (workTimeLeft < 0)
	{
		workTimeLeft = 0;
	}

	ctrlClock.SetOutputTime(limitedTimeLeft, workTimeLeft, displayTime);

	if (Settings::Inst.Mode != OM_UNLIMITED)
	{
		if (limitedTimeLeft == Settings::Inst.SecondsBeforeFirstSignal)
		{
			PlaySound(MAKEINTRESOURCE(IDR_WAVE_1RING),
				GetModuleHandle(NULL),
				SND_RESOURCE | SND_ASYNC);

			ShowBaloon(_T("Middle Bro"), _T("Session will be blocked soon.\r\nTime left: " + limitedTimeLeft.Format("%H:%M:%S")));
		}

		if (limitedTimeLeft == 0 && !BlockingDlg::IsVisible())
		{
			PlaySound(MAKEINTRESOURCE(IDR_WAVE_END),
				GetModuleHandle(NULL),
				SND_RESOURCE | SND_ASYNC);
			ShowBaloon(_T("Middle Bro"), _T("Session time is over."));
			Sleep(2000);
			OnTimeExpired();
		}

		if (workTimeLeft == Settings::Inst.SecondsBeforeFirstSignal)
		{
			PlaySound(MAKEINTRESOURCE(IDR_WAVE_1RING),
				GetModuleHandle(NULL),
				SND_RESOURCE | SND_ASYNC);

			ShowBaloon(_T("Middle Bro"), _T("You need to make break soon.\r\nTime left: " + workTimeLeft.Format("%H:%M:%S")));
		}

		if (workTimeLeft == 0 && !BlockingDlg::IsVisible())
		{
			theApp.WriteProfileInt(_T("Runtime"), _T("LimitedTime"), (int)(limitedTime.GetTotalSeconds()));
			PlaySound(MAKEINTRESOURCE(IDR_WAVE_END),
				GetModuleHandle(NULL),
				SND_RESOURCE | SND_ASYNC);
			ShowBaloon(_T("Middle Bro"), _T("Time to make a break."));
			Sleep(2000);
			Watcher::Inst.SaveStatistics();
			BlockingDlg::Show(BC_BREAK, MAKEINTRESOURCE(IDS_STRING_MAKE_A_BREAK), Settings::Inst.BreakLength);
		}
	}
}

void CMiddleBroDlg::OnDummyExit()
{
	theApp.WriteProfileInt(_T("Runtime"), _T("LimitedTime"), (int)(limitedTime.GetTotalSeconds()));

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
	BlockingDlg::Show(BC_NO_MORE_LIMITED_TIME, MAKEINTRESOURCE(IDS_STRING_TURN_OFF_WARNING));
}

void CMiddleBroDlg::OnWatcherEvent(Watcher::EVENT_TYPE type)
{
	switch (type)
	{
	case Watcher::BLACKLISTED_IS_ON_SCREEN:
		PlaySound(MAKEINTRESOURCE(IDR_WAVE_WARNING),
			GetModuleHandle(NULL),
			SND_RESOURCE | SND_ASYNC);
		break;
	case Watcher::LIMITED_DETECTED:
		PlaySound(MAKEINTRESOURCE(IDR_WAVE_COOCOO),
			GetModuleHandle(NULL),
			SND_RESOURCE | SND_ASYNC);
		break;
	case Watcher::LIMITED_CLEARED:
		break;

	}
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
	switch (wParam)
	{
	case BC_BREAK:
		workTime = 0;
		break;
	case BC_NO_MORE_LIMITED_TIME:
		limitedTime = 0;
		break;
	}
	lastTimeCheck = CTime::GetCurrentTime();
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


void CMiddleBroDlg::OnModeLimited()
{
	if (PwdDlg::ShowCheckPwd(MAIN_PWD))
	{
		Settings::Inst.Mode = OM_LIMITED;
		Settings::Inst.ProcessProperties(NULL, true);
		RefreshMode();
	}
}


void CMiddleBroDlg::OnModeUnlimited()
{
	if (PwdDlg::ShowCheckPwd(MAIN_PWD))
	{
		Settings::Inst.Mode = OM_UNLIMITED;
		Settings::Inst.ProcessProperties(NULL, true);
		RefreshMode();
	}
}


void CMiddleBroDlg::OnModeWhitelisted()
{
	if (PwdDlg::ShowCheckPwd(MAIN_PWD))
	{
		Settings::Inst.Mode = OM_WHITELISTED;
		Settings::Inst.ProcessProperties(NULL, true);
		RefreshMode();
	}
}

void CMiddleBroDlg::RefreshMode()
{
	auto pItem = GetDlgItem(IDC_STATIC_MODE);
	if (pItem)
	{
		pItem->SetWindowText(Settings::OperationModeNames[(int)Settings::Inst.Mode]);
	}
}