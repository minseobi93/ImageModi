/*=========================================================================
 * Author : Minseob Lee
 * 2020.11.14 Declares a model class with for mainwindow
 * 2020.11.14 This class's role is model(data) for main window
 *=========================================================================*/

#pragma once
#ifndef _MAINWINDOW_MODEL_
#define _MAINWINDOW_MODEL_

#include <string>
#include <vector>
#include <set>
#include "../Logic/status_def.h"

class MainWindow_Model
{
 private:

	class File
	{
	private:

		Status inputfile_status;
		std::string inputpath;
		std::string inputdatatype;
		unsigned int inputdimension;
		struct InputSize { unsigned int input_x_size, input_y_size, input_z_size; }InputFileSize_Type;
		struct InputSpacing { double input_x_spacing, input_y_spacing, input_z_spacing; } InputFileSpacing_Type;
		struct InputOrigin { double input_x_origin, input_y_origin, input_z_origin; } InputFileOrigin_Type;
		struct InputFlipAxis { bool input_x_flipaxis, input_y_flipaxis, input_z_flipaxis; } InputFileAxis_Type;

		Status outputfile_status;
		std::string outputpath;
		std::string outputdatatype;
		unsigned int outputdimension;
		struct OutputSize { unsigned int output_x_size, output_y_size, output_z_size; } OutputFileSize_Type;
		struct OutputSpacing { double output_x_spacing, output_y_spacing, output_z_spacing; } OutputFileSpacing_Type;
		struct OutputOrigin { double output_x_origin, output_y_origin, output_z_origin; } OutputFileOrigin_Type;
		struct OutputFlipAxis { bool output_x_flipaxis, output_y_flipaxis, output_z_flipaxis; } OutputFileAxis_Type;

	public:

		File::File(std::string sourcepath)
		{
			SetInputFileStatus(NONE);
			SetInputPath(sourcepath);
			SetInputDataType("");
			SetInputDimension(0);
			SetInputSize(0, 0, 0);
			SetInputSpacing(0, 0, 0);
			SetInputOrigin(0, 0, 0);
			SetInputFlipAxis(false, false, false);

			SetOutputFileStatus(true, NONE);
			SetOutputPath(true, "");
			SetOutputDataType(true, "");
			SetOutputDimension(true, 0);
			SetOutputSize(true, true, true, 0, 0, 0);
			SetOutputSpacing(true, true, true, 0, 0, 0);
			SetOutputOrigin(true, true, true, 0, 0, 0);
			SetOutputFlipAxis(true, true, true, false, false, false);
		}

		void SetInputFileStatus(Status in_status);
		void SetInputPath(std::string in_inputpath);
		void SetInputDataType(std::string in_datatype);
		void SetInputDimension(unsigned int in_dim);
		void SetInputSize(unsigned int in_x_size, unsigned int in_y_size, unsigned int in_z_size);
		void SetInputSpacing(double in_x_spacing, double in_y_spacing, double in_z_spacing);
		void SetInputOrigin(double in_x_origin, double in_y_origin, double in_z_dim);
		void SetInputFlipAxis(bool in_x_flipaxis, bool in_y_flipaxis, bool in_z_flipaxis);

		void SetOutputFileStatus(bool out_status_valid, Status out_status);
		void SetOutputPath(bool out_path_valid, std::string out_path);
		void SetOutputDataType(bool out_datatype_valid, std::string out_datatype);
		void SetOutputDimension(bool out_dim_valid, unsigned int out_dim);
		void SetOutputSize(bool out_x_size_valid, bool out_y_size_valid, bool out_z_size_valid, unsigned int out_x_size, unsigned int out_y_size, unsigned int out_z_size);
		void SetOutputSpacing(bool out_x_spacing_valid, bool out_y_spacing_valid, bool out_z_spacing_valid, double out_x_spacing, double out_y_spacing, double out_z_spacing);
		void SetOutputOrigin(bool out_x_origin_valid, bool out_y_origin_valid, bool out_z_origin_valid, double out_x_origin, double out_y_origin, double out_z_dim);
		void SetOutputFlipAxis(bool out_x_flip_valid, bool out_y_flip_valid, bool out_z_flip_valid, bool out_x_flipaxis, bool out_y_flipaxis, bool out_z_flipaxis);

		Status GetInputFileStatus();
		std::string GetInputPathValueAsString();
		std::string GetInputDataTypeAsString();
		std::string GetInputDimensionAsString();
		std::vector<std::string> GetInputSizeValueAsStringVector();
		std::vector<std::string> GetInputSpacingValueAsStringVector();
		std::vector<std::string> GetInputOriginValueAsStringVector();
		std::vector<std::string> GetInputFlipAxisValueAsStringVector();
		std::vector<bool> GetInputFlipAxisValueAsBoolVector();

		Status GetOutputFileStatus();
		std::string GetOutputPathValueAsString();
		std::string GetOutputDataTypeAsString();
		std::string GetOutputDimensionAsString();
		std::vector<std::string> GetOutputSizeValueAsStringVector();
		std::vector<std::string> GetOutputSpacingValueAsStringVector();
		std::vector<std::string> GetOutputOriginValueAsStringVector();
		std::vector<std::string> GetOutputFlipAxisValueAsStringVector();
		std::vector<bool> GetOutputFlipAxisValueAsBoolVector();
	};

private:

	std::vector<MainWindow_Model::File> ListedFilesVec;

public:

	MainWindow_Model();
	~MainWindow_Model();

	std::vector<MainWindow_Model::File>& AccessListedFilesVec();
	void FillFileList(std::set<std::string>& filenames);
	unsigned int GetFilesNum();
	int DeleteInputFile(std::string filename);
	std::set<std::string> GetAllSourcePathValue();
};

#endif /*defined */