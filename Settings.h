#pragma once
#include <vector>

class Settings
{
public:
	static Settings Inst;
	bool Load(LPCTSTR fileName);
	std::vector<CString> LimitedNames; // Names that enable limited time counting
	std::vector<CString> WhiteNames;   // Names that are never counted in limited time (even if they conform to LimitedNames list)
	std::vector<CString> BlackNames;   // Names of unwanted programs that will trigger sound alarm (and probably will be subject to auto-close - depending on mode)

protected:
	Settings() {}
};

