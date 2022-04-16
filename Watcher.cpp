#include "pch.h"
#include "Watcher.h"
#include "MiddleBroDlg.h"

Watcher Watcher::Inst;

Watcher::Watcher()
{
    timerID = SetTimer(NULL, 1234, refreshPeriod, [](
        HWND hwnd,
        UINT message,
        UINT_PTR idTimer,
        DWORD dwTime
        ) {
            try
            {
                Inst.RefreshWindowsNames();
                Inst.RefreshProgramsInfo();
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
    for (HWND hwnd = ::GetTopWindow(NULL); hwnd != NULL; hwnd = ::GetNextWindow(hwnd, GW_HWNDNEXT))
    {

        if (!::IsWindowVisible(hwnd))
            continue;

        int length = ::GetWindowTextLength(hwnd);
        if (length == 0)
            continue;

        std::basic_string<TCHAR> title;
        title.resize(length, ' ');

        ::GetWindowText(hwnd, (TCHAR*)title.data(), length + 1);

        if (title == _T("Program Manager"))
            continue;
        windowsNames.push_back(title);

    }
}

void Watcher::RefreshProgramsInfo()
{
    Timestamp now = std::chrono::time_point_cast<Timestamp::duration>(Timestamp::clock::now());
    for (auto& prog : programsInfoMap)
    {
        prog.second.IsOpen = false;
    }

    // Marking newly appeared programs
    for (auto& name : windowsNames)
    {
        auto& prog = programsInfoMap.find(name);
        if (prog != programsInfoMap.end())
        {
            auto& programInfo = prog->second;
            if (programInfo.WorkPeriods.empty() || programInfo.WorkPeriods.back().second != Timestamp(std::chrono::milliseconds(0)))
            {
                programInfo.WorkPeriods.emplace_back(now, std::chrono::milliseconds(0));
            }
            programInfo.IsOpen = true;
        }
        else
        {
            auto& programInfo = programsInfoMap[name];
            programInfo.WorkPeriods.emplace_back(now, std::chrono::milliseconds(0));
            programInfo.IsOpen = true;
        }
    }
    
    // Setting end time for closed programs
    for (auto& prog : programsInfoMap)
    {
        if (!prog.second.IsOpen && !prog.second.WorkPeriods.empty() && prog.second.WorkPeriods.back().second == Timestamp(std::chrono::milliseconds(0)))
        {
            prog.second.WorkPeriods.back().second = now;
        }
    }
}
