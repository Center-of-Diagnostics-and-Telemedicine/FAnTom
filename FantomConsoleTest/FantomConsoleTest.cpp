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
	std::string tmp = convert_to_string8(folder_path_p);
	InitFantom_J(tmp.c_str());

	char* accession_numbers;
	int length;

	GetStudiesIDs_J(&accession_numbers, &length);

	printf("GetStudiesIDs_J: n=%i, data=%s\n",
		EnsureType<int>(length),
		EnsureType<char*>(accession_numbers));

	if (length == 0)
		return;

	std::string tmp1;
	int i = 0;
	while (accession_numbers[i] != '\t')
	{
		tmp1.append(1u, accession_numbers[i]);
		i++;
	}

	LoadCTbyAccession_J(tmp1.c_str());

	slice_type st = e_axial;
	size_t frames_number;
	//GetNFrames_interpolated(frames_number, st);
	GetTomogramDimension_J(&frames_number, st);

	if (!frames_number)
		return;
	size_t slice_no = frames_number/2;

	RealFunction2D_F32 image;
	double black(0);
	double white(255);
	double gamma(1);
	size_t coord;
//	GetCoordinateNative(coord, st, slice_no);
	GetTomogramLocationFromScreenCoordinate_J(&coord, st, slice_no, true);

	mip_method_type mip_method = e_average;

	printf("Coord for chosen slice = %zu\n", EnsureType<size_t>(coord));

//	GetSlice(image, st, slice_no, black, white, gamma, 0, mip_method);

	printf("Image size: %zu x %zu\n",
			EnsureType<size_t>(image.hsize()),
			EnsureType<size_t>(image.vsize()));
	// TODO: save image
	getchar();
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
