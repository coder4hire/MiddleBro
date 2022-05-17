#pragma once
#include <vector>
#include <string>
#include <map>
#include <functional>

class ProgramInfo
{
public:
	std::vector<std::pair<CTime, CTime>> WorkPeriods;
	bool IsOpen = false;

	CTimeSpan GetTotalDuration(CTime start = 0, CTime end = CTime::GetCurrentTime()) const
	{
		CTimeSpan total(0);
		for (const auto& item : WorkPeriods)
		{
			auto second = item.second == 0 ? end : item.second;
			if (item.first >= start && second <= end)
				total += second - item.first;
			else if (item.first < start && second > end)
				total += end - start;
			else if (item.first < start && second > start)
				total += second - start;
			else if (item.first < end && second > end)
				total += end - item.first;
		}
		return total;
	}

	CString GetTotalDurationString(CTime start = 0, CTime end = CTime::GetCurrentTime()) const
	{
		return GetTotalDuration(start, end).Format("%H:%M:%S");
	}
};

using ProgramsInfoTree = std::map<CString, std::map<CString, ProgramInfo>>;

class Watcher
{
public:
	enum EVENT_TYPE
	{
		LIMITED_CLEARED,
		LIMITED_DETECTED,
		BLACKLISTED_IS_ON_SCREEN,
	};

	using Callback = std::function<void(EVENT_TYPE)>;

	static Watcher Inst;
	~Watcher();
	const std::vector<std::pair<CString,CString>>& GetWindowsNames() { return windowsNames; }
	const ProgramsInfoTree& GetProgramsInfoMap() { return programsInfoMap; }
	bool SaveStatistics();
	void SetCallback(const Callback& callback) { this->callback = callback; }
	bool IsLimitedOnScreen() { return isLimitedOnScreen; }
protected:
	Watcher();
	ProgramsInfoTree programsInfoMap;
	std::vector<std::pair<CString, CString>> windowsNames;
	int refreshPeriod = 2000;
	static std::map<UINT_PTR, Watcher*> timerToWatcherMap;
	void RefreshWindowsNames();
	void RefreshProgramsInfo();
	bool CheckNames(const std::vector<CString>& names);
	CString GetProcessName(HWND hWnd);
	void CheckAndSaveStatistics(const CString& processName);
	UINT_PTR timerID = 0;
	CTime watchStartTime;
	bool isLimitedOnScreen = false;
	Callback callback;
};

