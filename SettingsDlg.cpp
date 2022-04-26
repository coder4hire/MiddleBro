// SettingsDlg.cpp : implementation file
//

#include "pch.h"
#include "MiddleBro.h"
#include "SettingsDlg.h"
#include "afxdialogex.h"

#include "Settings.h"


// SettingsDlg dialog

IMPLEMENT_DYNAMIC(SettingsDlg, CDialog)

SettingsDlg::SettingsDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SettingsDlg, pParent)
	, valConfigFilesLocation(_T(""))
	, valSecondsBeforeFirstSignal(0)
	, valStatisticsLogsLocation(_T(""))
	, valDailyTimeLimit(0)
{

}

SettingsDlg::~SettingsDlg()
{
}

void SettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CONFIG_FILES_LOCATION, valConfigFilesLocation);
	DDX_Text(pDX, IDC_SECONDS_BEFORE_FIRST_SIGNAL, valSecondsBeforeFirstSignal);
	DDV_MinMaxUInt(pDX, valSecondsBeforeFirstSignal, 5, 1200);
	DDX_Text(pDX, IDC_STATISTICS_LOGS_LOCATION, valStatisticsLogsLocation);
	DDX_Text(pDX, IDC_DAILY_TIME_LIMIT, valDailyTimeLimit);
	DDV_MinMaxUInt(pDX, valDailyTimeLimit, 5, 86400);
}


BEGIN_MESSAGE_MAP(SettingsDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_GET_FOLDER, &SettingsDlg::OnBnClickedButtonGetFolder)
	ON_BN_CLICKED(IDC_BUTTON_GET_FOLDER_STAT_LOGS, &SettingsDlg::OnBnClickedButtonGetFolderStatLogs)
END_MESSAGE_MAP()


// SettingsDlg message handlers


BOOL SettingsDlg::OnInitDialog()
{
	Settings::Inst.ProcessProperties(this, false);

	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void SettingsDlg::OnOK()
{
	if (UpdateData())
	{
		Settings::Inst.ProcessProperties(this, true);

		CDialog::OnOK();
	}
}


void SettingsDlg::OnBnClickedButtonGetFolder()
{
	ShowGetFolder(IDC_CONFIG_FILES_LOCATION);
}

void SettingsDlg::OnBnClickedButtonGetFolderStatLogs()
{
	ShowGetFolder(IDC_STATISTICS_LOGS_LOCATION);
}

void SettingsDlg::ShowGetFolder(int nID)
{
	CString currentText;
	GetDlgItem(nID)->GetWindowText(currentText);
	CFolderPickerDialog dlg(currentText, 0, this);
	if (dlg.DoModal() == IDOK)
	{
		GetDlgItem(nID)->SetWindowText(dlg.GetFolderPath());
	}
}