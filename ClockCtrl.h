#pragma once


// ClockCtrl

class ClockCtrl : public CStatic
{
	DECLARE_DYNAMIC(ClockCtrl)

public:
	ClockCtrl();
	virtual ~ClockCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	void SetOutputTime(const CTimeSpan& timeLeft, const CTimeSpan& timeElapsed);

protected:
	CTimeSpan timeLeft;
	CTimeSpan timeElapsed;
	CFont font;
};


