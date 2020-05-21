#include "pre.h"
/*!
 * \file ManageStrings.cpp
 * \date 2017/10/05 12:29
 *
 * \author kulberg
 *
 * \brief
 *
 * TODO: long description
 *
 * \note
*/
#include "ManageStrings.h"
#include <QtCore/QString>


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


string	u16tou8(const wstring s)
{
	ustring	buffer = wstring_to_ustring(s);
	string	result(buffer.begin(), buffer.end());
	return result;
}

wstring	qs_to_ws(const QString &qs)
{
	wstring	ws;
	ws.resize(qs.length());
	for(int i = 0; i < qs.length(); ++i) ws[i] = qs[i].unicode();
	return ws;
}

wstring	interpret_url(const QByteArray &url)
{
	QString	decoded = QByteArray::fromPercentEncoding(url);
	return qs_to_ws(decoded);

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
