#include "pch.h"
#include "Settings.h"
#include <fstream>

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

#define LIMITED_NAMES _T("LimitedNames")
#define WHITE_NAMES _T("WhiteNames")
#define BLACK_NAMES _T("BlackNames")

Settings Settings::Inst;

LPCTSTR Settings::OperationModeNames[] = { _T("Unlimited"),_T("Limited"),_T("Whitelisted") };

bool Settings::LoadDataFromFiles(LPCTSTR fileName)
{
	FILE* fp = NULL;
	if (!_tfopen_s(&fp, fileName, _T("rb")))
	{

		char readBuffer[8192];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		rapidjson::GenericDocument<
#ifdef _UNICODE        
			rapidjson::UTF8<wchar_t>
#else
			rapidjson::UTF8<>
#endif
		> d;

		d.ParseStream(is);
		fclose(fp);

		if (d.HasParseError())
		{
			return false;
		}

		if (d.HasMember(LIMITED_NAMES) && d[LIMITED_NAMES].IsArray())
		{
			for (const auto& x : d[LIMITED_NAMES].GetArray())
			{
				LimitedNames.push_back(x.GetString());
			}
		}

		if (d.HasMember(WHITE_NAMES) && d[WHITE_NAMES].IsArray())
		{
			for (const auto& x : d[WHITE_NAMES].GetArray())
			{
				WhiteNames.push_back(x.GetString());
			}
		}

		if (d.HasMember(BLACK_NAMES) && d[BLACK_NAMES].IsArray())
		{
			for (const auto& x : d[BLACK_NAMES].GetArray())
			{
				BlackNames.push_back(x.GetString());
			}
		}
		return true;
	}

	return false;
}
