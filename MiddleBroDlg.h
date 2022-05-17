
// MiddleBroDlg.h : header file
//

#pragma once

#include <vector>
#include <string>
#include "watcher.h"
#include "DialogTray.h"
#include "BlockingDlg.h"
#include "ClockCtrl.h"
#include "StatisticsDlg.h"
#include "SettingsDlg.h"

enum OPERATION_MODE
{
	MODE_WHITELISTED,
	MODE_LIMITED,
	MODE_UNLIMITED
};

// CMiddleBroDlg dialog
class CMiddleBroDlg : public CDialogTray
{
// Construction
public:
	CMiddleBroDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MIDDLEBRO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ShowTimeLeft();
	
	CTime startTime;
	
	bool isMonitorOn = true;
	CTimeSpan displayTime;
	CTimeSpan limitedTime;
	CTimeSpan workTime;
	CTime lastTimeCheck;

	afx_msg void OnDummyExit();
	afx_msg void OnDummyShow();

	void OnTimeExpired();
	void OnWatcherEvent(Watcher::EVENT_TYPE type);

	StatisticsDlg statDlg;
	SettingsDlg settingsDlg;

	ClockCtrl ctrlClock;
	afx_msg void OnDummySettings();
	afx_msg UINT OnPowerBroadcast(UINT nPowerEvent, LPARAM nEventData);
	afx_msg LRESULT OnBlockingRemoved(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDummyStatistics();
	int timeSaveThreshold=30;
};
