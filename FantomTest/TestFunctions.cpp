#include "pre.h"

#include "TestFunctions.h"

#include <FantomLibrary/CTomogram.h>
#include <FantomLibrary/XRay.h>
#include <FantomLibrary/Mamogram.h>

#include <FantomLibrary/FantomLibrary.h>
#include <FantomLibrary/FantomClass.h>
#include <FantomLibrary/FantomDefs.h>
#include <XRADBasic/Sources/Utils/ConsoleProgress.h>

#include <QtGui/QImage>
#include <QtCore/QBuffer>
#include <QtCore/QFile>

XRAD_BEGIN

void	TestHeap()
{
	Init2DInterpolators(ConsoleProgressProxy());
	
	InitHeapFiltered_N(L"C:/wired/ERIS000001201895");

}

#if 0

void	TestTomogram()
{
	Init2DInterpolators(ConsoleProgressProxy());
	CTomogram tomogram;

//	tomogram.InitHeap(L"C:/dicom/7 LUNG RADS 4A/003");
	tomogram.InitHeap(L"C:/dicom/+AGFA000000015851_AGFA000000015807");
//	tomogram.InitHeap(L"C:/dicom");

//	tomogram.HeapDump(L"test.ct.txt");
	wstring acc_no;
	tomogram.GetAccNumber(0,acc_no);

	tomogram.LoadByAccession(acc_no);

	frame_t  frame;

//	tomogram.GetImage(frame, { modality_t::CT(), image_t::e_ct_axial, 125 });

//	DisplayMathFunction2D(frame, L"Выбраный срез");

	const unsigned char* img;
	int length;

	tomogram.GetScreenImage(&img, &length, { modality_t::CT(), image_t::e_ct_sagittal(), 199 }, -500., 500., 1, { mip_method_t::e_mip_average, 3 });

	QByteArray png = QByteArray();

	tomogram.CreateQByteArrayPngFromChar(png, img, length);
	
	QFile file("C:/temp/tomo001.txt");
	file.open(QIODevice::WriteOnly);
	file.write(png);
	file.close();


	double value;
	tomogram.GetBrightness(&value, { modality_t::CT(), image_t::e_ct_axial(), 90 }, 170, 150);
	cout << value << endl;
	fflush(stdout);

}

void	TestXRAYImage()
{
	Init2DInterpolators(ConsoleProgressProxy());
	XRay radiogram;

	radiogram.InitHeap(L"C:/xray/туберкулез/16");
//	radiogram.HeapDump(L"C:/xray/dump.xray.txt");

	wstring acc_no;
		
	radiogram.GetAccNumber(0, acc_no);

	radiogram.LoadByAccession(acc_no);

	cout << radiogram.m_XR_Images()[0].sizes(0) << endl;
	fflush(stdout);

	cout << radiogram.m_XR_Images()[0].sizes(1) << endl;
	fflush(stdout);

	frame_t frame;

//	radiogram.GetImage(frame, { modality_t::DX(), image_t::e_dx_generic, 0 });

//	DisplayMathFunction2D(frame, L"Выбраный срез");

	double value;

	const unsigned char* img;
	int length;

	radiogram.GetScreenImage(&img, &length, { modality_t::DX(), image_t::e_dx_generic(), 2 }, -1000., 10000., 1., { mip_method_t::e_mip_minvalue, 0 });

	QByteArray png = QByteArray();

//	radiogram.CreateQByteArrayPngFromChar(png, img, length);

	QFile file("C:/temp/xray002.txt");
	file.open(QIODevice::WriteOnly);
	file.write(png);
	file.close();


	radiogram.GetBrightness(&value, { modality_t::DX(), image_t::e_dx_generic(), 2 }, 1474, 989);

	cout << value << endl;
	fflush(stdout);
}

void	TestMamoImage()
{
	Init2DInterpolators(ConsoleProgressProxy());

	Mamogram mamogram;

	mamogram.InitHeap(L"C:/temp/2D_modes/ММГ");

	wstring acc_no;
	mamogram.GetAccNumber(0,acc_no);

	mamogram.LoadByAccession(acc_no);
	
	cout << mamogram.m_MM_Images()[image_t::e_mg_lcc()].sizes(1) << endl;
	fflush(stdout);
	cout << mamogram.m_MM_Images()[image_t::e_mg_lcc()].sizes(0) << endl;
	fflush(stdout);

	frame_t frame;

	mamogram.GetImage(frame, { modality_t::MG(), image_t::e_mg_rcc(), 0 });

	DisplayMathFunction2D(frame, L"Выбраный срез");

	const unsigned char* img;
	int length;

	mamogram.GetScreenImage(&img, &length, { modality_t::MG(), image_t::e_mg_lmlo(), 1 }, 1000, 3300., 1., { mip_method_t::e_mip_minvalue, 0 });

	QByteArray png = QByteArray();

//	mamogram.CreateQByteArrayPngFromChar(png, img, length);

	QFile file("C:/temp/mamo003.txt");
	file.open(QIODevice::WriteOnly);
	file.write(png);
	file.close();


	double value;

	mamogram.GetBrightness(&value, { modality_t::MG(), image_t::e_mg_lcc(), 2 }, 4474, 2872);

	cout << value << endl;
	fflush(stdout);
	
}
#endif

void CreateQByteArrayPngFromChar(QByteArray &png, const unsigned char *img, int length)
{
	QImage q_image;

	if (q_image.loadFromData(img, length, ".bmp"))
	{
		QBuffer buffer;
		buffer.open(QIODevice::ReadWrite);
		//q_image.save(&buffer, "bmp"); // writes pixmap into bytes in BMP format
		q_image.save(&buffer, "png");	// writes pixmap into bytes in PNG format

		q_image.save("C:/temp/ddd.png", "png");

		png = buffer.buffer().toBase64();
	}
}

void	TestLibraryImage(const wstring &folder_path_p)
{
	std::string tmp = convert_to_string8(folder_path_p);
	InitFantom_J(tmp.c_str());

	char* accession_numbers;
	int length;

	GetStudiesIDs_J(&accession_numbers, &length);
	if (length == 0)
		return;

	std::string tmp1;
	int i = 0;
	while (accession_numbers[i] != '\t')
	{
		tmp1.append(1u,accession_numbers[i]);
		i++;
	}

	LoadCTbyAccession_J(tmp1.c_str());

	double value;
	GetPointHU_J(&value, 100, 250, 300);
	cout << "------------------------------" << endl;
	cout << value << endl;
	cout << "------------------------------" << endl;

/*	const unsigned char *img;
	int  len;

	GetSlice_J(&img, &len,
		slice_type::e_axial,
		333,//number
		-500.5,//black
		1100.5,//white
		0.3,//gamma
		4,//mipvalue
		mip_method_type::e_minvalue
	);


	QByteArray png = QByteArray();

    CreateQByteArrayPngFromChar(png, img, len);

	QFile file("C:/temp/tomo001.txt");
	file.open(QIODevice::WriteOnly);
	file.write(png);
	file.close();
*/
//	GetSlice(image, st, slice_no, black, white, gamma, 0, mip_method);

//	DisplayMathFunction2D(image, L"Выбраный срез");
//	DisplayMathFunction3D(mct.marked_tomogram_data, "Data in MCT container");
}


XRAD_END