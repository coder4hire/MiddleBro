
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
	void RestartCounters();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStat();
	
	CTime startTime;
	
	int timeLimit = 25;// 2100;
	bool isMonitorOn = true;
	CTimeSpan displayTime;
	CTime lastTimeCheck;

	afx_msg void OnDummyExit();
	afx_msg void OnDummyShow();

	void OnTimeExpired();
	StatisticsDlg statDlg;

public:
	ClockCtrl ctrlClock;
	afx_msg void OnMenu();
	afx_msg UINT OnPowerBroadcast(UINT nPowerEvent, LPARAM nEventData);
protected:
	afx_msg LRESULT OnBlockingRemoved(WPARAM wParam, LPARAM lParam);
};
