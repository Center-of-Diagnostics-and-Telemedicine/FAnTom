/*!
 * \file ManageBitmap.h
 * \date 2017/10/05 15:39
 *
 * \author kulberg
 *
 * \brief
 *
 * TODO: long description
 *
 * \note
*/
#ifndef ManageBitmap_h__
#define ManageBitmap_h__


#include <XRADBasic/ContainersAlgebra.h>
#include <QTCore/QByteArray.h>
#include <FantomLibrary.h>

XRAD_BEGIN

void	CreateBitMap(QByteArray &raw_bitmap, const RealFunction2D_UI8 &img, const wstring &format);
bool	SaveImageFormat(const QByteArray &raw_image_body, QString filename, char* format);


XRAD_END

#endif // ManageBitmap_h__
