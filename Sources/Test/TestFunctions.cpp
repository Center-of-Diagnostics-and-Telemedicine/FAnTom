/*

*/
#include "pre.h"
#include "TestFunctions.h"

#include <FantomLibrary.h>


XRAD_BEGIN


void	TestLibraryImage(const wstring &folder_path_p)
{
	InitFantom(folder_path_p);

	vector<wstring> accession_numbers;
	GetAccessionNumbers(accession_numbers);

	char **acc = new char*[100];
	int n;

	auto op = GetStudiesIDs_J(acc, &n);

	bool is_series_loaded;
	LoadCTbyAccession(accession_numbers.front(), is_series_loaded);

	wstring accession_number;
	GetStudyAccessionNumber(accession_number);
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
	GetNFrames_real(frames_number, st);

	size_t slice_no = GetUnsigned(L"Выберете срез для просмотра", 1, 1, frames_number);

	RealFunction2D_F32 image;
	double black(0);
	double white(255);
	double gamma(1);
	double coord;
	GetCoordinateNative(coord, st, slice_no);
	mip_method_type mip_method = e_average;
	ShowFloating("Coord for chosen slice", coord);
	GetSlice(image, st, slice_no, black, white, gamma, 0, mip_method);
	DisplayMathFunction2D(image, L"Выбраный срез");
//	DisplayMathFunction3D(mct.marked_tomogram_data, "Data in MCT container");
}


XRAD_END