#include "pre.h"

#include "TestFunctions.h"

#include "CTomogram.h"
#include "XRay.h"
#include "Mamogram.h"

#include <FantomLibrary/FantomLibrary.h>
#include <FantomLibrary/FantomClass.h>
#include <XRADBasic/Sources/Utils/ConsoleProgress.h>

#include <QtGui/QImage>
#include <QtCore/QBuffer>
#include <QtCore/QFile>

XRAD_BEGIN


void	TestTomogram()
{
	Init2DInterpolators(ConsoleProgressProxy());
	CTomogram tomogram;

//	tomogram.InitHeap(L"C:/dicom/7 LUNG RADS 4A/003");
	tomogram.InitHeap(L"C:/dicom/+AGFA000000015851_AGFA000000015807");
//	tomogram.InitHeap(L"C:/dicom");

//	tomogram.HeapDump(L"test.ct.txt");
	wstring acc_no = tomogram.GetAccNumber(0);

	tomogram.LoadByAccession(acc_no);

	frame_t  frame;

//	tomogram.GetImage(frame, { modality_t::CT(), image_t::e_ct_axial, 125 });

//	DisplayMathFunction2D(frame, L"Выбраный срез");

	const unsigned char* img;
	int length;

	tomogram.GetScreenImage(&img, &length, { modality_t::CT(), image_t::e_ct_axial(), 225 }, -1000., 1000., 0.5, { mip_method_t::e_mip_maxvalue, 10 });

	QByteArray png = QByteArray();

	tomogram.CreateQByteArrayPngFromChar(png, img, length);
	
	QFile file("C:/temp/tomo001.txt");
	file.open(QIODevice::WriteOnly);
	file.write(png);
	file.close();


//	double value;
//	tomogram.GetBrightness(&value, { modality_t::CT, image_t::e_ct_frontal, 150 }, 170, 90);
//	cout << value << endl;
//	fflush(stdout);

}

void	TestXRAYImage()
{
	Init2DInterpolators(ConsoleProgressProxy());
	XRay radiogram;

	radiogram.InitHeap(L"C:/xray/туберкулез/16");
//	radiogram.HeapDump(L"C:/xray/dump.xray.txt");

	wstring acc_no = radiogram.GetAccNumber(0);

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

	radiogram.CreateQByteArrayPngFromChar(png, img, length);

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

	wstring acc_no = mamogram.GetAccNumber(0);

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

	mamogram.CreateQByteArrayPngFromChar(png, img, length);

	QFile file("C:/temp/mamo003.txt");
	file.open(QIODevice::WriteOnly);
	file.write(png);
	file.close();


	double value;

	mamogram.GetBrightness(&value, { modality_t::MG(), image_t::e_mg_lcc(), 2 }, 4474, 2872);

	cout << value << endl;
	fflush(stdout);
	
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

	size_t st_decision = GetButtonDecision(L"Выберете Тип Среза", { L"Аксиальный", L"Фронтальный", L"Сагитальный" });
	slice_type st;
	switch (st_decision)
	{
	/*case 0:
		st = e_axial;
		break;*/
	case 1:
		st = e_frontal;
		break;
	case 2:
		st = e_sagittal;
		break;
	case 0:
	default:
		st = e_axial;
		break;
	}
	size_t frames_number;
	//GetNFrames_interpolated(frames_number, st);
	GetTomogramDimension_J(&frames_number, st);

	size_t slice_no = GetUnsigned(L"Выберете срез для просмотра", 1, 1, frames_number);

	RealFunction2D_F32 image;
	double black(0);
	double white(255);
	double gamma(1);
	size_t coord;
	//GetCoordinateNative(coord, st, slice_no);
	//GetTomogramLocationFromScreenCoordinate_J(size_t *pixel_coord, slice_type st, size_t rescaled_slice_no, bool interpolate_z)
	GetTomogramLocationFromScreenCoordinate_J(&coord, st, slice_no, true);

	mip_method_type mip_method = e_average;
	ShowFloating("Coord for chosen slice", coord);

//	GetSlice(image, st, slice_no, black, white, gamma, 0, mip_method);

//	DisplayMathFunction2D(image, L"Выбраный срез");
//	DisplayMathFunction3D(mct.marked_tomogram_data, "Data in MCT container");
}


XRAD_END