
// MiddleBroDlg.h : header file
//

#pragma once

#include <vector>
#include <string>
#include "watcher.h"
#include "DialogTray.h"
#include "BlockingDlg.h"

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
	afx_msg void OnBnClickedButtonStat();
	
	CStatic lblTimeLeft;
	CStatic lblTimeElapsed;
	CTime startTime;
	int timeLimit = 1800;
	BlockingDlg blockingDlg;

	afx_msg void OnDummyExit();
	afx_msg void OnDummyShow();
};
