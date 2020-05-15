#ifndef FantomClass_h__
#define FantomClass_h__

#include <XRADBasic/MathFunctionTypesMD.h>
#include <XRADDicom/XRADDicom.h>
#include "FantomDefs.h"

XRAD_USING

class slice_manager
{
public:
	operation_result	LoadCTbyAccession(const wstring &accession_number, bool &series_loaded);

	enum axis_t
	{
		e_z = 0,
		e_y = 1,
		e_x = 2
	};

	axis_t	normal_axis(slice_type st) const;
	slice_type	normal_slice(axis_t at) const;

protected:
	vector<Dicom::study_loader> m_studies_heap;
	shared_ptr<ProcessAcquisition> proc_acquisition_work_ptr;
	CTAcquisition& ct_acquisition_ptr() { return dynamic_cast<CTAcquisition&>(*proc_acquisition_work_ptr); }

	point3_F64 m_scales;
	VectorFunction3_F64 m_image_positions_patient;
	bool	b_flip_z;

	point2_ST	slice_sizes(slice_type st, size_t no) const;
	size_t	n_slices(slice_type st) const;
	frame_t	slice(slice_type st, size_t no);

	unique_ptr<char[]> buf_ct_accession_numbers;
	unique_ptr<char[]> buffer_detailed_study_info;

	size_t			  GetAccessionHeapPosition(const wstring &accession_number, bool &series_loaded);

	point3_ST		  interpolation_sizes;
//! Отношение масштабов дискретных изображений
	operation_result  CalculateInterpolationScales();
	operation_result  CalculateMIPFrame(frame_t &buffer, double native_slice_position, slice_type st, size_t aprox_size, mip_method_type mmt);

	string	DetailedStudyInfo();

	double	dicom_to_screen_coordinate(double x, axis_t axis);
	double	screen_to_dicom_coordinate(double x, axis_t axis);

	const RealFunctionMD_F32	&slices() const{ return m_slices; }


private:
	wstring m_patient_id;
	wstring m_patient_sex;
	wstring m_patient_age;

	wstring m_accession_number;
	wstring m_study_id;
	wstring m_study_instance_uid;


	point3_F64		  m_interpolation_factor;
	RealFunctionMD_F32 m_slices;
};

class Fantom : protected slice_manager
{
	PARENT(slice_manager);

	map<slice_type, unique_ptr<unsigned char[]>> bitmap_buffers;

public:
	using parent::LoadCTbyAccession;
	operation_result	InitFantom(const wstring &data_store_path);
	//operation_result	CloseCTStudyAcession(const wstring &accession_number);

	operation_result	GetScreenSlice(frame_t &interpolated_image, slice_type st, size_t rescaled_slice_no, double black, double white, double gamma, size_t slice_aprox, mip_method_type mip_method);

	operation_result	GetScreenDimension(size_t &frames_no, slice_type st);
	operation_result	GetMillimeterCoordinateFromTomogramPosition(double &coord, slice_type st, size_t rescaled_slice_no);
	operation_result	GetDatabaseCoordinateFromScreenPosition(double &coord, slice_type st, size_t rescaled_slice_no);

	operation_result	GetNumbersOfAccessions(vector<wstring> &accession_numbers);

	operation_result	GetStudiesIDs(vector<Dicom::complete_study_id_t> &accession_numbers);
	operation_result	GetTomogramSampleHU(double &value, point3_ST rescaled_value_coord);

 	operation_result	GetDicomLocationFromScreenCoordinate(size_t &pixel_coord, slice_type st, size_t rescaled_slice_no, bool interpolate_z);
	operation_result	GetScreenCoordinateFromDicomLocation(size_t &rescaled_pixel_coord, slice_type st, size_t original_slice_no);
	operation_result	GetPixelLengthCoefficient(double &length_pixel_coef);
	operation_result	GetTomogramDimension(size_t &frames_no, slice_type st);

	int GetStudiesNumber();

	// Java =====================================================================
	operation_result InitFantom_J(const char *data_store_path);
	operation_result GetStudiesIDs_J(char **studies_ids_p, int &length);
//	operation_result LoadCTbyAccession_J(const char *accession_number);
	operation_result GetSlice_J(const unsigned char **imgData, int &length, slice_type st, size_t rescaled_slice_no, double black, double white, double gamma, size_t slice_aprox, mip_method_type mip_method);

	operation_result GetDetailedStudyInfo_J(char **info_json_p, int &length);

private:
	operation_result  GetTomogramSlice(frame_t &buffer, double native_slice_position, slice_type st, size_t aprox_size, mip_method_type mip_method);
	operation_result  RescaleImageFromTomogramToScreenCoordinates(frame_t &interpolated_image, const frame_t &in_image, slice_type st);
	//size_t			GetCTVectorPosition(const wstring &accession_number);

};


#endif // FantomClass_h__

