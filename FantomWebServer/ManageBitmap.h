/*!
	\file
	\date 2017/10/05 15:39
	\author kulberg
*/
#ifndef ManageBitmap_h__
#define ManageBitmap_h__


#include <XRADBasic/ContainersAlgebra.h>
#include <QtCore/QByteArray>

XRAD_BEGIN

void CreateQByteArrayPngFromChar(QByteArray &png, const unsigned char *img, int length);
void	CreateBitMap(QByteArray &raw_bitmap, const RealFunction2D_UI8 &img, const wstring &format);
bool	SaveImageFormat(const QByteArray &raw_image_body, QString filename, char* format);


XRAD_END

#endif // ManageBitmap_h__
