#pragma once

#include "SettingsDlg.h"
#include "MiddleBro.h"

#define DEFINE_STRING(NAME) if(isSaving) {if(dlg){NAME = dlg->val##NAME;}theApp.WriteProfileString(_T("Settings"),_T(#NAME),NAME);}else{NAME = theApp.GetProfileString(_T("Settings"),_T(#NAME),NAME); if(dlg){dlg->val##NAME = NAME;}}
#define DEFINE_INT(NAME) if(isSaving) {if(dlg){NAME = dlg->val##NAME;}theApp.WriteProfileInt(_T("Settings"),_T(#NAME),NAME);}else{NAME = theApp.GetProfileInt(_T("Settings"),_T(#NAME),NAME);if(dlg){dlg->val##NAME = NAME;}}

class Settings
{
public:
	static Settings Inst;
	bool LoadDataFromFiles(LPCTSTR fileName);
	std::vector<CString> LimitedNames; // Names that enable limited time counting
	std::vector<CString> WhiteNames;   // Names that are never counted in limited time (even if they conform to LimitedNames list)
	std::vector<CString> BlackNames;   // Names of unwanted programs that will trigger sound alarm (and probably will be subject to auto-close - depending on mode)
	CString ConfigFilesLocation=_T("AAA");
	int SecondsBeforeFirstSignal = 300;

protected:
	Settings() {}

public:
	void ProcessProperties(SettingsDlg* dlg, bool isSaving)
	{
		DEFINE_STRING(ConfigFilesLocation);
		DEFINE_INT(SecondsBeforeFirstSignal);
	}
};

