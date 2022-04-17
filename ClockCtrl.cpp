// ClockCtrl.cpp : implementation file
//

#include "pch.h"
#include "MiddleBro.h"
#include "ClockCtrl.h"


// ClockCtrl

IMPLEMENT_DYNAMIC(ClockCtrl, CStatic)

ClockCtrl::ClockCtrl()
{
	font.CreateFont(
		28,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                   // nWeight
		TRUE,                      // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial"));                 // lpszFacename

}

ClockCtrl::~ClockCtrl()
{
	font.DeleteObject();
}


BEGIN_MESSAGE_MAP(ClockCtrl, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// ClockCtrl message handlers




void ClockCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CStatic::OnPaint() for painting messages
	auto oldFont = dc.SelectObject(font);
	dc.SetBkMode(OPAQUE);
	dc.SetBkColor(GetSysColor(COLOR_3DFACE));
	dc.SetTextColor(0x00008000);
	dc.TextOut(5, 5, timeLeft.Format("%H:%M:%S"));
	dc.SelectObject(oldFont);
	dc.SetTextColor(0);
	dc.TextOut(5, 35, timeElapsed.Format("%H:%M:%S"));
}

void ClockCtrl::SetOutputTime(const CTimeSpan& timeLeft, const CTimeSpan& timeElapsed)
{
	this->timeLeft = timeLeft;
	this->timeElapsed = timeElapsed;
	Invalidate();
}
