/*!
 * \file ManageStrings.h
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
#ifndef ManageStrings_h__
#define ManageStrings_h__


#include <QtCore/QByteArray>

XRAD_BEGIN

string	u16tou8(const wstring s);
wstring	interpret_url(const QByteArray &url);
wstring	qs_to_ws(const QString &qs);
wstring	filetype(const wstring &s);
bool	is_filetype(const wstring &s, const wstring &t);
wstring DeleteSlash(wstring params);

int string_to_int(const wstring &s);

XRAD_END

#endif // ManageStrings_h__
