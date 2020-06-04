/*

*/
#include "pre.h"

#include "TestFunctions.h"

#include "NewClass.h"



#include <FantomLibrary/FantomLibrary.h>
#include <FantomLibrary/FantomClass.h>

XRAD_BEGIN

void	TestNewClasses()
{
	CTomogram tomogram;

	tomogram.InitHeap(L"C:/dicom/7 LUNG RADS 4A/19");

	tomogram.HeapDump(L"test.ct.txt");

//	tomogram.GetBrightness({ modality::CT, image_t::e_ct_axial, 0 }, 100, 100);
}

void	TestXRAYImage(const wstring &folder_path_p)
{
	InitFantom_J(convert_to_string8(folder_path_p).c_str());

	char* accession_number;
	int acc_number_length;
	GetStudiesIDs_J(&accession_number, &acc_number_length);

	string str(accession_number);
	string firstAccessionNumber = str.substr(0, str.find(string("\t")));
	//str.erase(acc_number_length - 1, 1);

	cout << "Accession number = " << firstAccessionNumber << endl;

	bool flag;

	LoadXRbyAccession_J(firstAccessionNumber.c_str(), &flag);

	RealFunction2D_F32 img;

	GetXRSlice(img,1);

	DisplayMathFunction2D(img, L"Выбраный срез");
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