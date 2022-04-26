#pragma once

#include <vector>
#include <memory>

// SettingsDlg dialog

class SettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(SettingsDlg)
	friend class Settings;

public:
	SettingsDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SettingsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SettingsDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	virtual void OnOK();
	CString valConfigFilesLocation;
	afx_msg void OnBnClickedButtonGetFolder();
	unsigned int valSecondsBeforeFirstSignal;
	afx_msg void OnBnClickedButtonGetFolderStatLogs();
	void ShowGetFolder(int nID);
	CString valStatisticsLogsLocation;
	unsigned int valDailyTimeLimit;
};
