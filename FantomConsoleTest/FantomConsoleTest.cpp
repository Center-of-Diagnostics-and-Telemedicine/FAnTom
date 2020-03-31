#include "pre.h"
#include <FantomLibrary/FantomLibrary.h>

#ifdef _MSC_VER
#include <FantomLibrary/MSVC_FantomLibraryLink.h>
#include <XRADConsoleUI/Sources/PlatformSpecific/MSVC/MSVC_XRADConsoleUILink.h>
#endif

XRAD_USING

//--------------------------------------------------------------

namespace
{

//--------------------------------------------------------------

void TestLibraryImage(const wstring &folder_path_p)
{
	InitFantom(folder_path_p);

	vector<wstring> accession_numbers;
	GetAccessionNumbers(accession_numbers);
	if (accession_numbers.empty())
		return;

	char *acc = nullptr;
	int n;
	auto op = GetStudiesIDs_J(&acc, &n);
	printf("GetStudiesIDs_J: n=%i, data=%s\n",
			EnsureType<int>(n),
			EnsureType<char*>(acc));

	bool is_series_loaded;
	LoadCTbyAccession(accession_numbers.front(), is_series_loaded);

	wstring accession_number;
	GetStudyAccessionNumber(accession_number);
	slice_type st = e_axial;
	size_t frames_number;
	//GetNFrames_interpolated(frames_number, st);
	GetNFrames_real(frames_number, st);

	if (!frames_number)
		return;
	size_t slice_no = frames_number/2;

	RealFunction2D_F32 image;
	double black(0);
	double white(255);
	double gamma(1);
	double coord;
	GetCoordinateNative(coord, st, slice_no);
	mip_method_type mip_method = e_average;
	printf("Coord for chosen slice = %lf\n", EnsureType<double>(coord));
	GetSlice(image, st, slice_no, black, white, gamma, 0, mip_method);
	printf("Image size: %zu x %zu\n",
			EnsureType<size_t>(image.hsize()),
			EnsureType<size_t>(image.vsize()));
	// TODO: save image
}

//--------------------------------------------------------------

int	main_unsafe(int n, char **names)
{
	try
	{
		XRAD_ASSERT_THROW_M(n == 2, invalid_argument, "Укажите одну папку с данными формата DICOM");
		wstring	foldername = convert_to_wstring(names[1]);
		TestLibraryImage(foldername);
	}
	catch(canceled_operation &)
	{
		return 0;
	}
	catch(quit_application &)
	{
		return 0;
	}
	return 0;
}

//--------------------------------------------------------------

} // namespace

//--------------------------------------------------------------

int xrad::xrad_main(int argn, char **args)
{
	try
	{
		return main_unsafe(argn, args);
	}
	catch (exception &)
	{
		printf("%s\n", EnsureType<const char*>(GetExceptionString().c_str()));
		return 3;
	}
	catch (...)
	{
		printf("Internal error: Unhandled unknown exception in main.\n%s\n",
				EnsureType<const char*>(GetExceptionString().c_str()));
		return 3;
	}
}

//--------------------------------------------------------------
