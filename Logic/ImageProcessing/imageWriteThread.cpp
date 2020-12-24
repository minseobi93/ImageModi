#include "imageWriteThread.h"

ImageWriteThread::ImageWriteThread(QObject* parent) : QThread(parent)
{
	m_window = new MainWindow_Model();
}
ImageWriteThread::~ImageWriteThread()
{

}

itk::IOComponentEnum ImageWriteThread::GetIOPixelTypeFromString(std::string DataTypeStr)
{
	if (DataTypeStr == "unsigned_char") return itk::IOComponentEnum::UCHAR;
	else if (DataTypeStr == "char") return itk::IOComponentEnum::CHAR;
	else if (DataTypeStr == "unsigned_short") return itk::IOComponentEnum::USHORT;
	else if (DataTypeStr == "short") return itk::IOComponentEnum::SHORT;
	else if (DataTypeStr == "unsigned_int") return itk::IOComponentEnum::UINT;
	else if (DataTypeStr == "int") return itk::IOComponentEnum::INT;
	else if (DataTypeStr == "unsigned_long") return itk::IOComponentEnum::ULONG;
	else if (DataTypeStr == "long") return itk::IOComponentEnum::LONG;
	else if (DataTypeStr == "float") return itk::IOComponentEnum::FLOAT;
	else if (DataTypeStr == "double") return itk::IOComponentEnum::DOUBLE;
	else { return itk::IOComponentEnum::UNKNOWNCOMPONENTTYPE; }
}

void ImageWriteThread::SetModel(MainWindow_Model* m_window_from_GUI)
{
	this->m_window = m_window_from_GUI;
}
void ImageWriteThread::ConfigureTask()
{
	for (auto& image : this->images)
	{
		for (auto listed_task : m_window->AccessListedFilesVec())
		{
			if (image.read_path == listed_task.GetInputPathValueAsString())
			{
				// inputpath already met the condition. 
				image.read_path = listed_task.GetInputPathValueAsString();
				image.read_status = listed_task.GetInputFileStatus();
				image.read_datatype = listed_task.GetInputDataTypeAsString();
				image.read_dimension = stoul(listed_task.GetInputDimensionAsString(), nullptr, 0);

				std::vector<std::string> input_size = listed_task.GetInputSizeValueAsStringVector();
				image.ReadSize_Type.x_size = stod(input_size[0]);
				image.ReadSize_Type.y_size = stod(input_size[1]);
				image.ReadSize_Type.z_size = stod(input_size[2]);

				std::vector<std::string> input_spacing = listed_task.GetInputSpacingValueAsStringVector();
				image.ReadSpacing_Type.x_spacing = stod(input_spacing[0]);
				image.ReadSpacing_Type.y_spacing = stod(input_spacing[1]);
				image.ReadSpacing_Type.z_spacing = stod(input_spacing[2]);

				std::vector<std::string> input_origin = listed_task.GetInputOriginValueAsStringVector();
				image.ReadOrigin_Type.x_origin = stod(input_origin[0]);
				image.ReadOrigin_Type.y_origin = stod(input_origin[1]);
				image.ReadOrigin_Type.z_origin = stod(input_origin[2]);

				std::vector<bool> input_flip = listed_task.GetInputFlipAxisValueAsBoolVector();
				image.ReadFlipAxis_Type.x_flipaxis = input_flip[0];
				image.ReadFlipAxis_Type.y_flipaxis = input_flip[1];
				image.ReadFlipAxis_Type.z_flipaxis = input_flip[2];

				image.write_status = NONE;
				image.write_path = listed_task.GetOutputPathValueAsString();
				image.write_datatype = listed_task.GetOutputDataTypeAsString();
				image.write_dimension = stoul(listed_task.GetOutputDimensionAsString(), nullptr, 0);
			
				std::vector<std::string> size = listed_task.GetOutputSizeValueAsStringVector();
				image.WriteSize_Type.x_size = stod(size[0]);
				image.WriteSize_Type.y_size = stod(size[1]);
				image.WriteSize_Type.z_size = stod(size[2]);

				std::vector<std::string> spacing = listed_task.GetOutputSpacingValueAsStringVector();
				image.WriteSpacing_Type.x_spacing = stod(spacing[0]);
				image.WriteSpacing_Type.y_spacing = stod(spacing[1]);
				image.WriteSpacing_Type.z_spacing = stod(spacing[2]);

				std::vector<std::string> origin = listed_task.GetOutputOriginValueAsStringVector();
				image.WriteOrigin_Type.x_origin = stod(origin[0]);
				image.WriteOrigin_Type.y_origin = stod(origin[1]);
				image.WriteOrigin_Type.z_origin = stod(origin[2]);

				std::vector<bool> output_flip = listed_task.GetOutputFlipAxisValueAsBoolVector();
				image.WriteFlipAxis_Type.x_flipaxis = output_flip[0];
				image.WriteFlipAxis_Type.y_flipaxis = output_flip[1];
				image.WriteFlipAxis_Type.z_flipaxis = output_flip[2];
			}
		}
	}
}

void ImageWriteThread::SetTasksPerThread(std::string readpath)
{
	ImageWriteThread::Image newimage(readpath);
	(this->images).push_back(newimage);
}

std::vector<ImageWriteThread::Image> ImageWriteThread::GetAllTask()
{
	return (this->images);
}

void ImageWriteThread::UpdateInputFileInfo()
{
	for (const auto image : this->images)
	{
		FileManager <QString>* filemanager = new FileManager<QString>();
		std::string processed_image_without_ext = filemanager->GetFileNameWithoutExtension(image.read_path);

		for (auto& File : m_window->AccessListedFilesVec())
		{
			std::string raw_image_without_ext = filemanager->GetFileNameWithoutExtension(File.GetInputPathValueAsString());

			if (processed_image_without_ext == raw_image_without_ext)
			{
				File.SetOutputFileStatus(true, image.write_status);
			}
		}
	}
}

void ImageWriteThread::run()
{
	for (auto& task : this->images)
	{
		emit W_SendStartTask("Saving : " + task.write_path); // update progressbar text

		ImageModi infile, outfile;
		infile.status = task.read_status;
		infile.filename = task.read_path;
		infile.component = GetIOPixelTypeFromString(task.read_datatype);
		infile.dimension = task.read_dimension;

		outfile.status = task.write_status;
		outfile.filename = task.write_path;
		outfile.component = GetIOPixelTypeFromString(task.write_datatype);
		outfile.dimension = task.write_dimension;

		for (int dim_index = 1; dim_index <= infile.dimension ; dim_index++)
		{
			if (dim_index == 1)
			{
				infile.size.push_back(task.ReadSize_Type.x_size);
				infile.spacing.push_back(task.ReadSpacing_Type.x_spacing);
				infile.origin.push_back(task.ReadOrigin_Type.x_origin);
				infile.flip.push_back(task.ReadFlipAxis_Type.x_flipaxis);
				outfile.size.push_back(task.WriteSize_Type.x_size);
				outfile.spacing.push_back(task.WriteSpacing_Type.x_spacing);
				outfile.origin.push_back(task.WriteOrigin_Type.x_origin);
				outfile.flip.push_back(task.WriteFlipAxis_Type.x_flipaxis);
			}
			else if (dim_index == 2) // 2d
			{
				infile.size.push_back(task.ReadSize_Type.y_size);
				infile.spacing.push_back(task.ReadSpacing_Type.y_spacing);
				infile.origin.push_back(task.ReadOrigin_Type.y_origin);
				infile.flip.push_back(task.ReadFlipAxis_Type.y_flipaxis);
				outfile.size.push_back(task.WriteSize_Type.y_size);
				outfile.spacing.push_back(task.WriteSpacing_Type.y_spacing);
				outfile.origin.push_back(task.WriteOrigin_Type.y_origin);
				outfile.flip.push_back(task.WriteFlipAxis_Type.y_flipaxis);
			}
			else if (dim_index == 3) // 3d
			{
				infile.size.push_back(task.ReadSize_Type.z_size);
				infile.spacing.push_back(task.ReadSpacing_Type.z_spacing);
				infile.origin.push_back(task.ReadOrigin_Type.z_origin);
				infile.flip.push_back(task.ReadFlipAxis_Type.z_flipaxis);
				outfile.size.push_back(task.WriteSize_Type.z_size);
				outfile.spacing.push_back(task.WriteSpacing_Type.z_spacing);
				outfile.origin.push_back(task.WriteOrigin_Type.z_origin);
				outfile.flip.push_back(task.WriteFlipAxis_Type.z_flipaxis);
			}
		}
	
		switch (infile.dimension)
		{
			case 2:
				task.write_status = ReadWriteScalarImage<2>(infile, outfile);
				break;

			case 3:
				task.write_status = ReadWriteScalarImage<3>(infile, outfile);
				break;
		}

		emit W_SendFinishedTask(task.read_path); // update progressbar
	}

	this->UpdateInputFileInfo();
	emit W_SendJobDone(true);
	return;
}