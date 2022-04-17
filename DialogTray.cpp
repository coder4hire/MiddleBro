#include "pch.h"
#include "DialogTray.h"
#include "resource.h"

//IMPLEMENT_DYNCREATE(CDialogTray, CDialog)

BEGIN_MESSAGE_MAP(CDialogTray, CDialog)
	ON_WM_SIZE ()
	ON_MESSAGE (MYWM_NOTIFYICON, &CDialogTray::OnNotifyIcon )
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

CDialogTray::CDialogTray(UINT nIDTemplate, CWnd* pParentWnd)
: CDialogEx(nIDTemplate, pParentWnd)
{
	hIcon=NULL;
	Tooltip=_T("");
	initializing=true;
}

CDialogTray::~CDialogTray(void)
{
}

BOOL CDialogTray::ShowBaloon(LPCTSTR caption,LPCTSTR text)
{
    NOTIFYICONDATA nid;
    memset(&nid, 0, sizeof(nid)); 
	nid.cbSize = NOTIFYICONDATA_V2_SIZE;

	nid.hWnd = m_hWnd;
	nid.uFlags = NIF_INFO;
    nid.uID = ID_FLIPPED_TO_TRAY;
	_tcscpy_s(nid.szInfoTitle,_countof(nid.szInfoTitle),caption);
	_tcscpy_s(nid.szInfo,_countof(nid.szInfo),text);
	nid.uTimeout = 15000;
	nid.dwInfoFlags = NIIF_INFO;
	return Shell_NotifyIcon(NIM_MODIFY,&nid);
}

BOOL CDialogTray::ShowTrayIcon()
{
	if(!hIcon)
	{
		hIcon = AfxGetApp()->LoadIcon(IDI_TRAY_ICON);
	}

    // Creating Icon
    NOTIFYICONDATA nid;

    memset(&nid, 0, sizeof(nid)); 
	nid.cbSize = NOTIFYICONDATA_V2_SIZE;
	nid.uVersion = NOTIFYICON_VERSION;
	Shell_NotifyIcon(NIM_SETVERSION,&nid);

    nid.hWnd = m_hWnd;
    nid.uID = ID_FLIPPED_TO_TRAY;
    nid.uCallbackMessage = MYWM_NOTIFYICON;
    nid.hIcon = hIcon;
	nid.dwInfoFlags = NIIF_INFO;
	_tcscpy_s(nid.szTip,_countof(nid.szTip),Tooltip);
    nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;

    // Showing icon
    return Shell_NotifyIcon(NIM_ADD, &nid);
}

//BOOL CDialogTray::SetTooltip(LPCTSTR tooltip)
//{
//	NOTIFYICONDATA nid;
//	memset(&nid, 0, sizeof(nid));
//	nid.cbSize = NOTIFYICONDATA_V2_SIZE;
//	nid.uVersion = NOTIFYICON_VERSION;
//	nid.uID = ID_FLIPPED_TO_TRAY;
//	Shell_NotifyIcon(NIM_SETVERSION, &nid);
//
//	_tcscpy_s(nid.szTip, _countof(nid.szTip), tooltip);
//	nid.uFlags = NIF_TIP;
//
//	// Showing icon
//	return Shell_NotifyIcon(NIM_MODIFY, &nid);
//}

void CDialogTray::HideToTray()
{
    // Hiding window
    ShowWindow(SW_HIDE);
}

BOOL CDialogTray::RemoveTrayIcon()
{
    // Identify icon
    NOTIFYICONDATA nid; 
	memset(&nid, 0, sizeof(nid)); 
	nid.cbSize = NOTIFYICONDATA_V2_SIZE;
    nid.hWnd = m_hWnd;

    nid.uID = ID_FLIPPED_TO_TRAY;

    // Deleting icon
    return Shell_NotifyIcon(NIM_DELETE, &nid);
}

void CDialogTray::RestoreFromTray()
{
    // Restoring window
	initializing=false;
    ShowWindow(SW_SHOWNORMAL);
	ShowWindow(SW_RESTORE);
	SetWindowPos(&wndTop,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	SetFocus();
}

void CDialogTray::OnSize(UINT nType, int cx, int cy) {

    if ( nType == SIZE_MINIMIZED)
    {
        HideToTray();
    }

    CDialog::OnSize(nType, cx, cy);
}

afx_msg LRESULT CDialogTray::OnNotifyIcon(WPARAM wParam, LPARAM lParam) 
{
	if(wParam == ID_FLIPPED_TO_TRAY)
	{
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:
			RestoreFromTray();
			break;
		case WM_RBUTTONDOWN:
		{
			CMenu menu;
			menu.LoadMenu(IDR_TRAY_MENU);
			POINT point;
			GetCursorPos(&point);
			menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN /*| TPM_RIGHTBUTTON | TPM_BOTTOMALIGN*/, point.x, point.y, this);
			break;
		}
		}
	}
    
    return TRUE;
};


void CDialogTray::OnOK()
{
}


void CDialogTray::OnCancel()
{
	HideToTray();
}
