#include "pch.h"
#include "Watcher.h"
#include "MiddleBroDlg.h"
#include "Settings.h"
#include <regex>
#include <Mmsystem.h>
#include <psapi.h>

Watcher Watcher::Inst;

Watcher::Watcher()
{
    watchStartTime = CTime::GetCurrentTime();
    timerID = SetTimer(NULL, 0, refreshPeriod, [](
        HWND hwnd,
        UINT message,
        UINT_PTR idTimer,
        DWORD dwTime
        ) {
            try
            {
                Inst.RefreshWindowsNames();
                Inst.RefreshProgramsInfo();
                if (Inst.CheckBlackNames())
                {
                    PlaySound(MAKEINTRESOURCE(IDR_WAVE_WARNING),
                        GetModuleHandle(NULL),
                        SND_RESOURCE | SND_ASYNC);
                }
            }
            catch (...)
            {
            }
        });
}

Watcher::~Watcher()
{
    if (timerID)
    {
        KillTimer(0,timerID);
    }
}

void Watcher::RefreshWindowsNames()
{
    windowsNames.clear();

    std::vector<std::basic_regex<TCHAR>> regexps;
    for (const auto& name : Settings::Inst.WhiteNames)
    {
        regexps.emplace_back((LPCTSTR)name, std::regex_constants::icase);
    }

    for (HWND hwnd = ::GetTopWindow(NULL); hwnd != NULL; hwnd = ::GetNextWindow(hwnd, GW_HWNDNEXT))
    {

        if (!::IsWindowVisible(hwnd))
            continue;

        int length = ::GetWindowTextLength(hwnd);
        if (length == 0)
            continue;

        std::basic_string<TCHAR> title(length, ' ');

        ::GetWindowText(hwnd, (TCHAR*)title.data(), length + 1);

        if (title == _T("Program Manager"))
            continue;
        if (std::any_of(regexps.begin(), regexps.end(), [&title](std::basic_regex<TCHAR>& re) {return std::regex_match(title, re); }))
        {
            continue;
        }

        const auto& procName = GetProcessName(hwnd);
        if (std::any_of(regexps.begin(), regexps.end(), [&procName](std::basic_regex<TCHAR>& re) {return std::regex_match((LPCTSTR)procName, re); }))
        {
            continue;
        }

        windowsNames.emplace_back(title.c_str(), procName);
    }
}

void Watcher::RefreshProgramsInfo()
{
    CTime now = CTime::GetCurrentTime();
    for (auto& process : programsInfoMap)
    {
        for (auto& wnd : process.second)
        wnd.second.IsOpen = false;
    }

    // Marking newly appeared programs
    for (auto& name : windowsNames)
    {
        const auto& processData = programsInfoMap.find(name.second);
        if(processData != programsInfoMap.end())
        {
            auto& wndData = processData->second.find(name.first);
            if (wndData != processData->second.end())
            {
                auto& programInfo = wndData->second;
                if (programInfo.WorkPeriods.empty() || programInfo.WorkPeriods.back().second != 0)
                {
                    programInfo.WorkPeriods.emplace_back(now, 0);
                }
                programInfo.IsOpen = true;
                continue;
            }
        }

        // If process/window is not found - add process and window
        auto& programInfo = programsInfoMap[name.second][name.first];
        programInfo.WorkPeriods.emplace_back(now, 0);
        programInfo.IsOpen = true;
    }
    
    // Setting end time for closed programs
    for (auto& processData : programsInfoMap)
    {
        for (auto& wndData : processData.second)
        {
            if (!wndData.second.IsOpen && !wndData.second.WorkPeriods.empty() && wndData.second.WorkPeriods.back().second == 0)
            {
                wndData.second.WorkPeriods.back().second = now;
            }
        }
    }
}

bool Watcher::CheckBlackNames()
{
   for(auto item : Settings::Inst.BlackNames)
   { 
       std::basic_regex<TCHAR> re((LPCTSTR)item, std::regex_constants::icase);
       if (std::any_of(windowsNames.begin(), windowsNames.end(), [&re](const std::pair<CString, CString>& winNames) {
           return std::regex_match((LPCTSTR)winNames.first, re) || std::regex_match((LPCTSTR)winNames.second, re); }
       ))
       {
           return true;
       }
   }
   return false;
}

bool Watcher::SaveStatistics()
{
    FILE* fp = NULL;
    try
    {
        CTime now = CTime::GetCurrentTime();
        if(!_tfopen_s(&fp, (LPCTSTR)(Settings::Inst.StatisticsLogsLocation + _T("\\mblog_") + watchStartTime.Format("%Y%m%d-%H%M%S.csv")),_T("w,ccs=UTF-16LE")))
        {
            _ftprintf(fp,_T("Process;Window;Window time;Periods\n"));
            for (auto& processData : programsInfoMap)
            {
                for (auto& wndData : processData.second)
                {
                    _ftprintf(fp,_T("%s;%s;%s;"), (LPCTSTR)processData.first,(LPCTSTR)wndData.first,(LPCTSTR)wndData.second.GetTotalDurationString());

                    for (auto& period : wndData.second.WorkPeriods)
                    {
                        _ftprintf(fp, _T("%s - %s"),(LPCTSTR)period.first.Format(_T("%H:%M:%S")),
                            (LPCTSTR)(period.second != 0 ? period.second :now).Format(_T("%H:%M:%S;")));
                    }
                    _ftprintf(fp, _T("\n"));
                }
            }
            fclose(fp);
            return true;
        }
    }
    catch (...)
    {
        if (fp)
        {
            fclose(fp);
        }
    }
    return false;
}

CString Watcher::GetProcessName(HWND hWnd)
{
    CString buf;
    DWORD dwProcId = 0;

    GetWindowThreadProcessId(hWnd, &dwProcId);

    HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, dwProcId);
    if (hProc)
    {
        DWORD sz = 255;
        DWORD err = QueryFullProcessImageName(hProc, 0, buf.GetBuffer(255), &sz);
        buf.ReleaseBuffer();
        CloseHandle(hProc);
        int pos = buf.ReverseFind(_T('\\'));
        if (pos != -1)
        {
            return buf.Mid(pos + 1);
        }
    }
    return _T("");
}