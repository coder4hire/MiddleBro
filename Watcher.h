#pragma once
#include <vector>
#include <string>
#include <chrono>
#include <map>

using Timestamp = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>;

class ProgramInfo
{
public:
	std::vector<std::pair<Timestamp, Timestamp>> WorkPeriods;
	bool IsOpen = false;

	std::chrono::milliseconds GetTotalDuration(Timestamp start,Timestamp end) const
	{
		std::chrono::milliseconds total(0);
		for (const auto& item : WorkPeriods)
		{
			auto second = item.second == Timestamp(std::chrono::milliseconds(0)) ? end : item.second;
			if (item.first >= start && second <= end)
				total += second - item.first;
			else if (item.first < start && second > end)
				total += end-start;
			else if (item.first < start && second > start)
				total += second - start;
			else if (item.first < end && second > end)
				total += end - item.first;
		}
		return total;
	}

	CString GetTotalDurationString(Timestamp start, Timestamp end) const
	{
		auto span = CTimeSpan(GetTotalDuration(start, end).count()/1000);
		return span.Format("%H:%M:%S");
	}
};

class Watcher
{
public:
	static Watcher Inst;
	~Watcher();
	const std::vector<tstring>& GetWindowsNames() { return windowsNames; }
	const std::map<tstring, ProgramInfo>& GetProgramsInfoMap() { return programsInfoMap; }
protected:
	Watcher();
	std::map<tstring, ProgramInfo> programsInfoMap;
	std::vector<tstring> windowsNames;
	int refreshPeriod = 2000;
	static std::map<UINT_PTR, Watcher*> timerToWatcherMap;
	void RefreshWindowsNames();
	void RefreshProgramsInfo();
	UINT_PTR timerID = 0;
};

