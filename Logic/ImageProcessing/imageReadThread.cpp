#include "imageReadThread.h"

ImageReadThread::ImageReadThread(QObject* parent) : QThread(parent)
{
	m_window = new MainWindow_Model();
}
ImageReadThread::~ImageReadThread()
{

}

void ImageReadThread::SetModel(MainWindow_Model* m_window_from_GUI)
{
	this->m_window = m_window_from_GUI;
}

void ImageReadThread::SetTasksPerThread(std::string TaskName)
{
	ImageReadThread::Image newimage(TaskName);
	(this->images).push_back(newimage);
}

std::vector<ImageReadThread::Image> ImageReadThread::GetAllTask()
{
	return (this->images);
}

void ImageReadThread::UpdateInputFileInfo()
{
	for (const auto imageinfo : this->images)
	{
		FileManager <QString>* filemanager = new FileManager<QString>();
		std::string processed_image_without_ext = filemanager->GetFileNameWithoutExtension(imageinfo.read_path);

		for (auto& File : m_window->AccessListedFilesVec())
		{
			std::string raw_image_without_ext = filemanager->GetFileNameWithoutExtension(File.GetInputPathValueAsString());

			if (processed_image_without_ext == raw_image_without_ext)
			{
				File.SetInputFileStatus(imageinfo.read_status);
				File.SetInputDataType(imageinfo.read_datatype);
				File.SetInputDimension(imageinfo.read_dimension);
				File.SetInputSize(imageinfo.ReadSize_Type.x_size, imageinfo.ReadSize_Type.y_size, imageinfo.ReadSize_Type.z_size);
				File.SetInputSpacing(imageinfo.ReadSpacing_Type.x_spacing, imageinfo.ReadSpacing_Type.y_spacing, imageinfo.ReadSpacing_Type.z_spacing);
				File.SetInputOrigin(imageinfo.ReadOrigin_Type.x_origin, imageinfo.ReadOrigin_Type.y_origin, imageinfo.ReadOrigin_Type.z_origin);
				File.SetInputFlipAxis(imageinfo.ReadFlipAxis_Type.x_flipaxis, imageinfo.ReadFlipAxis_Type.y_flipaxis, imageinfo.ReadFlipAxis_Type.z_flipaxis);

				File.SetOutputFileStatus(true, imageinfo.read_status);
				File.SetOutputDataType(true, imageinfo.read_datatype);
				File.SetOutputDimension(true, imageinfo.read_dimension);
				File.SetOutputSize(true, true, true, imageinfo.ReadSize_Type.x_size, imageinfo.ReadSize_Type.y_size, imageinfo.ReadSize_Type.z_size);
				File.SetOutputSpacing(true, true, true, imageinfo.ReadSpacing_Type.x_spacing, imageinfo.ReadSpacing_Type.y_spacing, imageinfo.ReadSpacing_Type.z_spacing);
				File.SetOutputOrigin(true, true, true, imageinfo.ReadOrigin_Type.x_origin, imageinfo.ReadOrigin_Type.y_origin, imageinfo.ReadOrigin_Type.z_origin);
				File.SetOutputFlipAxis(true, true, true, imageinfo.ReadFlipAxis_Type.x_flipaxis, imageinfo.ReadFlipAxis_Type.y_flipaxis, imageinfo.ReadFlipAxis_Type.z_flipaxis);
			}
		}
	}
}

void ImageReadThread::run()
{
	for (auto& task : this->images)
	{
		std::string inputfilename = task.read_path;
		emit R_SendStartTask("Reading : " + inputfilename); // update progressbar text

		try
		{
			itk::ImageIOBase::Pointer imageIO = itk::ImageIOFactory::CreateImageIO(inputfilename.data(), static_cast<itk::ImageIOFactory::FileModeType>(itk::CommonEnums::IOFileMode::ReadMode));
			imageIO->SetFileName(inputfilename.data());
			imageIO->ReadImageInformation();
		
			task.read_status = NORMAL;
			task.read_datatype = imageIO->GetComponentTypeAsString(imageIO->GetComponentType());

			unsigned int imageDimension = imageIO->GetNumberOfDimensions();
			task.read_dimension = imageDimension;

			itk::IOComponentEnum componentType = static_cast<itk::IOComponentEnum>(imageIO->GetPixelType());
			std::tuple<std::vector<unsigned int>, std::vector<double>, std::vector<double>, std::vector<bool>> ImageInfoTuple;

			switch (imageDimension)
			{
			case 2:
				ImageInfoTuple = ReadScalarImage<2>(inputfilename.data(), componentType);

				task.ReadSize_Type.x_size = std::get<0>(ImageInfoTuple)[0];
				task.ReadSize_Type.y_size = std::get<0>(ImageInfoTuple)[1];

				task.ReadSpacing_Type.x_spacing = std::get<1>(ImageInfoTuple)[0];
				task.ReadSpacing_Type.y_spacing = std::get<1>(ImageInfoTuple)[1];

				task.ReadOrigin_Type.x_origin = std::get<2>(ImageInfoTuple)[0];
				task.ReadOrigin_Type.y_origin = std::get<2>(ImageInfoTuple)[1];

				task.ReadFlipAxis_Type.x_flipaxis = std::get<3>(ImageInfoTuple)[0];
				task.ReadFlipAxis_Type.y_flipaxis = std::get<3>(ImageInfoTuple)[1];
				break;

			case 3:
				ImageInfoTuple = ReadScalarImage<3>(inputfilename.data(), componentType);

				task.ReadSize_Type.x_size = std::get<0>(ImageInfoTuple)[0];
				task.ReadSize_Type.y_size = std::get<0>(ImageInfoTuple)[1];
				task.ReadSize_Type.z_size = std::get<0>(ImageInfoTuple)[2];

				task.ReadSpacing_Type.x_spacing = std::get<1>(ImageInfoTuple)[0];
				task.ReadSpacing_Type.y_spacing = std::get<1>(ImageInfoTuple)[1];
				task.ReadSpacing_Type.z_spacing = std::get<1>(ImageInfoTuple)[2];

				task.ReadOrigin_Type.x_origin = std::get<2>(ImageInfoTuple)[0];
				task.ReadOrigin_Type.y_origin = std::get<2>(ImageInfoTuple)[1];
				task.ReadOrigin_Type.z_origin = std::get<2>(ImageInfoTuple)[2];

				task.ReadFlipAxis_Type.x_flipaxis = std::get<3>(ImageInfoTuple)[0];
				task.ReadFlipAxis_Type.y_flipaxis = std::get<3>(ImageInfoTuple)[1];
				task.ReadFlipAxis_Type.z_flipaxis = std::get<3>(ImageInfoTuple)[2];
				break;
			}

			emit R_SendFinishedTask(task.read_path); // update progressbar
		}
		catch (const itk::ExceptionObject& err)
		{
			task.read_status = FILE_NOT_READABLE;
		}
	}

	this->UpdateInputFileInfo();
	emit R_SendJobDone(true);
	return;
}