#pragma once


// StatisticsDlg dialog

class StatisticsDlg : public CDialog
{
	DECLARE_DYNAMIC(StatisticsDlg)

public:
	StatisticsDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~StatisticsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_StatisticsDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl treeStatistics;
	virtual BOOL OnInitDialog();
	void RefreshData();
};
