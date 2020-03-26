#include "pre.h"
/*!
 * \file ManageBitmap.cpp
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
#include "ManageBitmap.h"

#include <XRADBasic/Sources/Utils/StatisticUtils.h>
#include <XRADBasic/Sources/Utils/BitmapContainer.h>
#include <QTGui/QPixMap.h>
#include <QTGui/QImage.h>
#include <QTCore/QBuffer.h>

XRAD_BEGIN

bool	SaveImageFormat(const QByteArray &raw_image_body, QString filename,  char* format)
{
	QImage q_image;
	if (q_image.loadFromData(raw_image_body, format))
	{
		q_image.save(filename, format); // writes pixmap into bytes in PNG format
		return true;
	}
	else return false;
}

void	CreateBitMap(QByteArray &raw_bitmap, const RealFunction2D_UI8 &img, const wstring &format)
{
	BitmapContainerIndexed	bmp;
	bmp.SetSizes(img.vsize(), img.hsize());

	for (size_t i = 0; i < bmp.palette.size(); ++i)
	{
		bmp.palette[i] = uint8_t(i);
	}
	for(size_t i = 0; i < img.vsize(); ++i)
	{
		for(size_t j = 0; j < img.hsize(); ++j)
		{
			bmp.at(i,j) = img.at(img.vsize()-i-1,j);
		}
	}

	size_t	sz = bmp.GetBitmapFileSize();
	QImage q_image;
	if (q_image.loadFromData((const uchar*)bmp.GetBitmapFile(), int(sz), ".bmp"))
	{
		QBuffer buffer;
		buffer.open(QIODevice::ReadWrite);
		//q_image.save(&buffer, "bmp"); // writes pixmap into bytes in BMP format
		q_image.save(&buffer, convert_to_string(format).c_str()); // writes pixmap into bytes in PNG format
		//q_image.save("D:/_kovbas/tmp/__/ttt.png", "png");
		raw_bitmap = buffer.buffer();
	}
}


XRAD_END

