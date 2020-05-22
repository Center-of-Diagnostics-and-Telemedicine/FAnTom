/*!
	\file
	\date 2017/10/05 12:29
	\author kulberg
*/
#include "pre.h"
#include "ManageStrings.h"
#include <QtCore/QString>

#include <Common/StringConverters_Qt_Fantom.h>

XRAD_BEGIN



wstring DeleteSlash(wstring params)
{
	auto slash = std::find(params.begin(), params.end(), '/');
	if (slash == params.end())
	{
		return params;
	}
	wstring	params_no_slash(slash + 1, params.end());
	return params_no_slash;
}

wstring	interpret_url(const QByteArray &url)
{
	QString	decoded = QByteArray::fromPercentEncoding(url);
	return qstring_to_wstring(decoded);

}

wstring	filetype(const wstring &s)
{
	auto	is_slash = [](wchar_t x)->bool {return x==L'/' || x==L'\\';};

	ptrdiff_t slash_position = s.rend() - std::find_if(s.rbegin(), s.rend(), is_slash) - 1;
	ptrdiff_t dot_position = s.rend() - std::find(s.rbegin(), s.rend(), L'.') - 1;
	if(dot_position<=0 || dot_position < slash_position || dot_position == s.length()-1) return L"";

	wstring	ext(s.begin() + dot_position + 1, s.end());
	return get_lower(ext);
}

bool	is_filetype(const wstring &s, const wstring &t)
{
	return filetype(s) == get_lower(t);
}

int string_to_int(const wstring &s)
{
	return wcstol(s.c_str(), nullptr, 10);
}

XRAD_END
