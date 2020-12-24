#include <iostream>
#include <string>
#include "mainwindow_model.h"

MainWindow_Model::MainWindow_Model()
{
}

MainWindow_Model::~MainWindow_Model()
{
}

void MainWindow_Model::File::SetInputFileStatus(Status in_status)
{
	this->inputfile_status = in_status;
}

void MainWindow_Model::File::SetInputPath(std::string in_path)
{
	this->inputpath = in_path;
}

void MainWindow_Model::File::SetInputDataType(std::string in_datatype)
{
    this->inputdatatype = in_datatype;
}

void MainWindow_Model::File::SetInputDimension(unsigned int in_dim)
{
	this->inputdimension = in_dim;
}

void MainWindow_Model::File::SetInputSize(unsigned int in_x_size, unsigned int in_y_size, unsigned int in_z_size)
{
	this->InputFileSize_Type.input_x_size = in_x_size;
	this->InputFileSize_Type.input_y_size = in_y_size;
	this->InputFileSize_Type.input_z_size = in_z_size;
}

void MainWindow_Model::File::SetInputSpacing(double in_x_spacing, double in_y_spacing, double in_z_spacing)
{
	this->InputFileSpacing_Type.input_x_spacing = in_x_spacing; 
	this->InputFileSpacing_Type.input_y_spacing = in_y_spacing; 
	this->InputFileSpacing_Type.input_z_spacing = in_z_spacing;
}

void MainWindow_Model::File::SetInputOrigin(double in_x_origin, double in_y_origin, double in_z_origin)
{
	this->InputFileOrigin_Type.input_x_origin = in_x_origin;
	this->InputFileOrigin_Type.input_y_origin = in_y_origin;
	this->InputFileOrigin_Type.input_z_origin = in_z_origin;
}

void MainWindow_Model::File::SetInputFlipAxis(bool in_x_flipaxis,  bool in_y_flipaxis, bool in_z_flipaxis)
{
	this->InputFileAxis_Type.input_x_flipaxis = in_x_flipaxis;
	this->InputFileAxis_Type.input_y_flipaxis = in_y_flipaxis;
	this->InputFileAxis_Type.input_z_flipaxis = in_z_flipaxis;
}

void MainWindow_Model::File::SetOutputFileStatus(bool out_status_valid, Status out_status)
{
	if (out_status_valid) this->outputfile_status = out_status;
}

void MainWindow_Model::File::SetOutputPath(bool out_path_valid, std::string out_path)
{
	if (out_path_valid) this->outputpath = out_path;
}

void MainWindow_Model::File::SetOutputDataType(bool out_datatype_valid, std::string out_datatype)
{
	if (out_datatype_valid) this->outputdatatype = out_datatype;
}

void MainWindow_Model::File::SetOutputDimension(bool out_dim_valid, unsigned int out_dim)
{
	if (out_dim_valid) this->outputdimension = out_dim;
}

void MainWindow_Model::File::SetOutputSize(bool out_x_size_valid, bool out_y_size_valid, bool out_z_size_valid, unsigned int out_x_size, unsigned int out_y_size, unsigned int out_z_size)
{
	if (out_x_size_valid) this->OutputFileSize_Type.output_x_size = out_x_size;
	if (out_y_size_valid) this->OutputFileSize_Type.output_y_size = out_y_size;
	if (out_z_size_valid) this->OutputFileSize_Type.output_z_size = out_z_size;
}

void MainWindow_Model::File::SetOutputSpacing(bool out_x_spacing_valid, bool out_y_spacing_valid, bool out_z_spacing_valid, double out_x_spacing, double out_y_spacing, double out_z_spacing)
{
	if (out_x_spacing_valid) this->OutputFileSpacing_Type.output_x_spacing = out_x_spacing;
	if (out_y_spacing_valid) this->OutputFileSpacing_Type.output_y_spacing = out_y_spacing;
	if (out_z_spacing_valid) this->OutputFileSpacing_Type.output_z_spacing = out_z_spacing;
}

void MainWindow_Model::File::SetOutputOrigin(bool out_x_origin_valid, bool out_y_origin_valid, bool out_z_origin_valid, double out_x_origin, double out_y_origin, double out_z_origin)
{
	if (out_x_origin_valid) this->OutputFileOrigin_Type.output_x_origin = out_x_origin;
	if (out_y_origin_valid) this->OutputFileOrigin_Type.output_y_origin = out_y_origin;
	if (out_z_origin_valid) this->OutputFileOrigin_Type.output_z_origin = out_z_origin;
}

void MainWindow_Model::File::SetOutputFlipAxis(bool out_x_flip_valid, bool out_y_flip_valid, bool out_z_flip_valid, bool out_x_flipaxis, bool out_y_flipaxis, bool out_z_flipaxis)
{
	if (out_x_flip_valid) this->OutputFileAxis_Type.output_x_flipaxis = out_x_flipaxis;
	if (out_y_flip_valid) this->OutputFileAxis_Type.output_y_flipaxis = out_y_flipaxis;
	if (out_z_flip_valid) this->OutputFileAxis_Type.output_z_flipaxis = out_z_flipaxis;
}


Status MainWindow_Model::File::GetInputFileStatus()
{
	return this->inputfile_status;
}

std::string MainWindow_Model::File::GetInputPathValueAsString()
{
	return this->inputpath;
}

std::string MainWindow_Model::File::GetInputDataTypeAsString()
{
	return this->inputdatatype;
}

std::string MainWindow_Model::File::GetInputDimensionAsString()
{
	return std::to_string(this->inputdimension);
}

std::vector<std::string> MainWindow_Model::File::GetInputSizeValueAsStringVector()
{
	std::vector<std::string> input_size;
	input_size.push_back(std::to_string(this->InputFileSize_Type.input_x_size));
	input_size.push_back(std::to_string(this->InputFileSize_Type.input_y_size));
	input_size.push_back(std::to_string(this->InputFileSize_Type.input_z_size));
	return input_size;
}

std::vector<std::string> MainWindow_Model::File::GetInputSpacingValueAsStringVector()
{
	std::vector<std::string> input_spacingvalue;
	input_spacingvalue.push_back(std::to_string(this->InputFileSpacing_Type.input_x_spacing));
	input_spacingvalue.push_back(std::to_string(this->InputFileSpacing_Type.input_y_spacing));
	input_spacingvalue.push_back(std::to_string(this->InputFileSpacing_Type.input_z_spacing));
	return input_spacingvalue;
}

std::vector<std::string> MainWindow_Model::File::GetInputOriginValueAsStringVector()
{
	std::vector<std::string> input_originvalue;
	input_originvalue.push_back(std::to_string(this->InputFileOrigin_Type.input_x_origin));
	input_originvalue.push_back(std::to_string(this->InputFileOrigin_Type.input_y_origin));
	input_originvalue.push_back(std::to_string(this->InputFileOrigin_Type.input_z_origin));
	return input_originvalue;
}

std::vector<std::string> MainWindow_Model::File::GetInputFlipAxisValueAsStringVector()
{
	std::vector<std::string> input_flipaxis;
	
	std::string in_x_flipaxis = "Off";
	std::string in_y_flipaxis = "Off";
	std::string in_z_flipaxis = "Off";

	if (this->InputFileAxis_Type.input_x_flipaxis == true) in_x_flipaxis = "On";
	if (this->InputFileAxis_Type.input_y_flipaxis == true) in_y_flipaxis = "On";
	if (this->InputFileAxis_Type.input_z_flipaxis == true) in_z_flipaxis = "On";
	
	input_flipaxis.push_back(in_x_flipaxis);
	input_flipaxis.push_back(in_y_flipaxis);
	input_flipaxis.push_back(in_z_flipaxis);
	return input_flipaxis;
}

std::vector<bool> MainWindow_Model::File::GetInputFlipAxisValueAsBoolVector()
{
	std::vector<bool> input_flipaxis;
	input_flipaxis.push_back(this->InputFileAxis_Type.input_x_flipaxis);
	input_flipaxis.push_back(this->InputFileAxis_Type.input_y_flipaxis);
	input_flipaxis.push_back(this->InputFileAxis_Type.input_z_flipaxis);
	return input_flipaxis;
}

Status MainWindow_Model::File::GetOutputFileStatus()
{
	return this->outputfile_status;
}

std::string MainWindow_Model::File::GetOutputPathValueAsString()
{
	return this->outputpath;
}

std::string MainWindow_Model::File::GetOutputDataTypeAsString()
{
	return this->outputdatatype;
}

std::string MainWindow_Model::File::GetOutputDimensionAsString()
{
	return std::to_string(this->outputdimension);
}

std::vector<std::string> MainWindow_Model::File::GetOutputSizeValueAsStringVector()
{
	std::vector<std::string> output_size;
	output_size.push_back(std::to_string(this->OutputFileSize_Type.output_x_size));
	output_size.push_back(std::to_string(this->OutputFileSize_Type.output_y_size));
	output_size.push_back(std::to_string(this->OutputFileSize_Type.output_z_size));
	return output_size;
};

std::vector<std::string> MainWindow_Model::File::GetOutputSpacingValueAsStringVector()
{
	std::vector<std::string> output_spacingvalue;
	output_spacingvalue.push_back(std::to_string(this->OutputFileSpacing_Type.output_x_spacing));
	output_spacingvalue.push_back(std::to_string(this->OutputFileSpacing_Type.output_y_spacing));
	output_spacingvalue.push_back(std::to_string(this->OutputFileSpacing_Type.output_z_spacing));
	return output_spacingvalue;
}

std::vector<std::string> MainWindow_Model::File::GetOutputOriginValueAsStringVector()
{
	std::vector<std::string> output_originvalue;
	output_originvalue.push_back(std::to_string(this->OutputFileOrigin_Type.output_x_origin));
	output_originvalue.push_back(std::to_string(this->OutputFileOrigin_Type.output_y_origin));
	output_originvalue.push_back(std::to_string(this->OutputFileOrigin_Type.output_z_origin));
	return output_originvalue;
}

std::vector<std::string> MainWindow_Model::File::GetOutputFlipAxisValueAsStringVector()
{
	std::vector<std::string> output_flipaxis;

	std::string out_x_flipaxis = "Off";
	std::string out_y_flipaxis = "Off";
	std::string out_z_flipaxis = "Off";

	if (this->OutputFileAxis_Type.output_x_flipaxis == true) out_x_flipaxis = "On";
	if (this->OutputFileAxis_Type.output_y_flipaxis == true) out_y_flipaxis = "On";
	if (this->OutputFileAxis_Type.output_z_flipaxis == true) out_z_flipaxis = "On";

	output_flipaxis.push_back(out_x_flipaxis);
	output_flipaxis.push_back(out_y_flipaxis);
	output_flipaxis.push_back(out_z_flipaxis);
	return output_flipaxis;
}

std::vector<bool> MainWindow_Model::File::GetOutputFlipAxisValueAsBoolVector()
{
	std::vector<bool> output_flipaxis;
	output_flipaxis.push_back(this->OutputFileAxis_Type.output_x_flipaxis);
	output_flipaxis.push_back(this->OutputFileAxis_Type.output_y_flipaxis);
	output_flipaxis.push_back(this->OutputFileAxis_Type.output_z_flipaxis);
	return output_flipaxis;
}

void MainWindow_Model::FillFileList(std::set<std::string>& filenames)
{
	for (const auto filename : filenames)
	{
		MainWindow_Model::File NewFile(filename);
		(this->ListedFilesVec).push_back(NewFile);
	}
}

unsigned int  MainWindow_Model::GetFilesNum()
{
	return (this->ListedFilesVec).size();
}

int MainWindow_Model::DeleteInputFile(std::string del_filename)
{
	for (int i = 0; i < (this->ListedFilesVec).size(); i++)
	{
		std::string inputpath = (this->ListedFilesVec[i]).GetInputPathValueAsString();
		if (inputpath == del_filename)
		{
			(this->ListedFilesVec).erase((this->ListedFilesVec).begin() + i);
			return NORMAL;
		}
	}

	return FILE_NOT_FOUND;
}

std::set<std::string> MainWindow_Model::GetAllSourcePathValue()
{
	std::set <std::string> AllInputPaths;

	for (auto& File : this->ListedFilesVec)
	{
		std::string FileName = File.GetInputPathValueAsString();
		AllInputPaths.insert(FileName);
	}

	return AllInputPaths;
}

std::vector<MainWindow_Model::File>& MainWindow_Model::AccessListedFilesVec()
{
	return (this->ListedFilesVec);
}
