#pragma once

#define MYWM_NOTIFYICON (WM_USER + 1)
#define ID_FLIPPED_TO_TRAY 10000

class CDialogTray :
	public CDialogEx
{
public:

	CDialogTray(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	~CDialogTray(void);

	BOOL ShowBaloon(LPCTSTR caption,LPCTSTR text);
    void HideToTray();
    void RestoreFromTray();
	BOOL ShowTrayIcon();
	BOOL RemoveTrayIcon();

	HICON hIcon;
	CString Tooltip;

protected:
    DECLARE_MESSAGE_MAP()
//    DECLARE_DYNCREATE(CDialogTray)

    afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);
    afx_msg void OnSize (UINT nType, int cx, int cy);

	bool initializing;
	virtual void OnOK();
	virtual void OnCancel();
};
