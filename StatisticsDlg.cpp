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
	ON_BN_CLICKED(IDC_REFRESH, &StatisticsDlg::OnBnClickedRefresh)
END_MESSAGE_MAP()


// StatisticsDlg message handlers


BOOL StatisticsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	reinterpret_cast<CButton*>(GetDlgItem(IDC_CHECK_MERGE_SINGLE_WND_PROC))->SetCheck(TRUE);

	RefreshData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void StatisticsDlg::RefreshData()
{
	treeStatistics.DeleteAllItems();

	bool isMergingData = reinterpret_cast<CButton*>(GetDlgItem(IDC_CHECK_MERGE_SINGLE_WND_PROC))->GetCheck();

	const auto& infoMap = Watcher::Inst.GetProgramsInfoMap();
	for (const auto& info : infoMap)
	{
		HTREEITEM processItem = (info.second.size() > 1 || !isMergingData) ? 
			treeStatistics.InsertItem(_T("")) : 
			TVI_ROOT;

		CTimeSpan totalProcessTime = 0;
		for (const auto& wndData : info.second)
		{
			auto wndTime = wndData.second.GetTotalDuration();
			totalProcessTime += wndTime;
			auto progItem = treeStatistics.InsertItem(CString(wndData.first) + wndTime.Format(_T(" (%H:%M:%S)")),processItem);
			treeStatistics.SetItemData(progItem, IT_WND);

			for (const auto& period : wndData.second.WorkPeriods)
			{
				auto periodItem = treeStatistics.InsertItem(period.first.Format(_T("    %H:%M:%S")) +
					(period.second != 0 ? period.second.Format(_T(" - %H:%M:%S")) : CString(_T(" - now")))
					, progItem
				);
				treeStatistics.SetItemData(periodItem, IT_PERIOD);
			}
		}

		if (processItem != TVI_ROOT)
		{
			treeStatistics.SetItemData(processItem, IT_PROC);
			treeStatistics.SetItemText(processItem, CString(info.first) + totalProcessTime.Format(_T(" (%H:%M:%S)")));
		}
	}

	HTREEITEM hItem;

	hItem = treeStatistics.GetRootItem();
	while (hItem != NULL)
	{
		if (treeStatistics.GetItemData(hItem) == IT_PROC)
		{
			treeStatistics.Expand(hItem, TVE_EXPAND);
		}
		hItem = treeStatistics.GetNextItem(hItem, TVGN_NEXTVISIBLE);
	}
}


void StatisticsDlg::OnBnClickedRefresh()
{
	RefreshData();
}
