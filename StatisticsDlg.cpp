// StatisticsDlg.cpp : implementation file
//

#include "pch.h"
#include "MiddleBro.h"
#include "StatisticsDlg.h"
#include "afxdialogex.h"

#include <iomanip>
#include <sstream>
#include "watcher.h"


// StatisticsDlg dialog

IMPLEMENT_DYNAMIC(StatisticsDlg, CDialog)

StatisticsDlg::StatisticsDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_StatisticsDlg, pParent)
{

}

StatisticsDlg::~StatisticsDlg()
{
}

void StatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STAT_TREE, treeStatistics);
}


BEGIN_MESSAGE_MAP(StatisticsDlg, CDialog)
END_MESSAGE_MAP()


// StatisticsDlg message handlers


BOOL StatisticsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	RefreshData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void StatisticsDlg::RefreshData()
{
	treeStatistics.DeleteAllItems();
	//for (auto& txt : Watcher::Inst.GetWindowsNames())
	//{
	//	str += txt + _T("\r\n");
	//}

	const auto& infoMap = Watcher::Inst.GetProgramsInfoMap();
	for (const auto& info : infoMap)
	{
		auto progItem = treeStatistics.InsertItem(info.first.c_str());
		for (const auto& period : info.second.WorkPeriods)
		{
			std::time_t tt = std::chrono::system_clock::to_time_t(period.first);
			std::tm tm;
			localtime_s(&tm, &tt);
			std::basic_stringstream<TCHAR> ss;
			ss << _T("    ") << std::put_time(&tm, _T("%H:%M:%S"));
			if (period.second.time_since_epoch().count())
			{
				tt = std::chrono::system_clock::to_time_t(period.second);
				localtime_s(&tm, &tt);
				ss << _T(" - ") << std::put_time(&tm, _T("%H:%M:%S"));
			}
			else
			{
				ss << _T(" - now");
			}
			treeStatistics.InsertItem(ss.str().c_str(), progItem);
		}
	}
}
