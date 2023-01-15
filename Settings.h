#pragma once

#include "SettingsDlg.h"
#include "MiddleBro.h"

#define DEFINE_STRING(NAME) if(isSaving) {if(dlg){NAME = dlg->val##NAME;}theApp.WriteProfileString(_T("Settings"),_T(#NAME),NAME);}else{NAME = theApp.GetProfileString(_T("Settings"),_T(#NAME),NAME); if(dlg){dlg->val##NAME = NAME;}}
#define DEFINE_INT(NAME) if(isSaving) {if(dlg){NAME = dlg->val##NAME;}theApp.WriteProfileInt(_T("Settings"),_T(#NAME),NAME);}else{NAME = theApp.GetProfileInt(_T("Settings"),_T(#NAME),NAME);if(dlg){dlg->val##NAME = NAME;}}
#define DEFINE_STRING_NO_DLG(NAME) if(isSaving) {theApp.WriteProfileString(_T("Settings"),_T(#NAME),NAME);}else{NAME = theApp.GetProfileString(_T("Settings"),_T(#NAME),NAME);}
#define DEFINE_INT_NO_DLG(NAME) if(isSaving) {theApp.WriteProfileInt(_T("Settings"),_T(#NAME),NAME);}else{NAME = theApp.GetProfileInt(_T("Settings"),_T(#NAME),NAME);}
#define DEFINE_TYPE_NO_DLG(NAME,TYPE) if(isSaving) {theApp.WriteProfileInt(_T("Settings"),_T(#NAME),(int)NAME);}else{NAME = (TYPE)theApp.GetProfileInt(_T("Settings"),_T(#NAME),NAME);}

enum OperationMode
{
	OM_UNLIMITED,
	OM_LIMITED,
	OM_WHITELISTED
};

class Settings
{
public:
	static Settings Inst;
	bool LoadDataFromFiles(LPCTSTR fileName);
	std::vector<CString> LimitedNames; // Names that enable limited time counting
	std::vector<CString> WhiteNames;   // Names that are never counted in limited time (even if they conform to LimitedNames list)
	std::vector<CString> BlackNames;   // Names of unwanted programs that will trigger sound alarm (and probably will be subject to auto-close - depending on mode)
	CString ConfigFilesLocation=_T(".");
	CString StatisticsLogsLocation = _T(".");
	int SecondsBeforeFirstSignal = 300;
	int DailyTimeLimit = 3600;
	int WorkTimeLimit = 1860;
	int BreakLength = 600;
	OperationMode Mode = OM_LIMITED;

	static LPCTSTR OperationModeNames[];

protected:
	Settings() {}

public:
	void ProcessProperties(SettingsDlg* dlg, bool isSaving)
	{
		DEFINE_STRING(ConfigFilesLocation);
		DEFINE_INT(SecondsBeforeFirstSignal);
		DEFINE_STRING(StatisticsLogsLocation);
		DEFINE_INT(DailyTimeLimit);
		DEFINE_INT(WorkTimeLimit);
		DEFINE_INT(BreakLength);
		DEFINE_TYPE_NO_DLG(Mode,OperationMode);
	}
};

