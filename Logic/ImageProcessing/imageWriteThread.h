#pragma once
#ifndef _IMAGEWRITETHREAD_
#define _IMAGEWRITETHREAD_

#include <QThread>
#include <tuple>
#include <vector>
#include "../constant.h"
#include "../status_def.h"
#include "../IO/file_manager.h"
#include "../IO/outputfile_manager.h"
#include "../../Model/mainwindow_model.h"
#include "imageWriteBase.h"
#include "itkImage.h"
#include "itkImageIOBase.h"
#include "itkImageFileReader.h"
#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"

class ImageWriteThread : public QThread
{
	Q_OBJECT

public: 

	explicit ImageWriteThread(QObject* parent = 0);
	~ImageWriteThread();

	ImageWriteThread* operator->() { return ImageWriteThread_ptr; };
	ImageWriteThread* ImageWriteThread_ptr = this;
	ImageWriteThread* operator [] (unsigned int thread_index) { return (this + thread_index); }

	struct Image
	{
	    Status read_status;
	    std::string read_path;
	    unsigned int read_dimension;
	    std::string  read_datatype;
	    struct ReadSize { unsigned int x_size, y_size, z_size; } ReadSize_Type;
	    struct ReadSpacing { double x_spacing, y_spacing, z_spacing; } ReadSpacing_Type;
	    struct ReadOrigin { double x_origin, y_origin, z_origin; } ReadOrigin_Type;
	    struct ReadFlipAxis { bool x_flipaxis, y_flipaxis, z_flipaxis; } ReadFlipAxis_Type;

	    Status write_status;
	    std::string write_path;
	    std::string write_datatype;
	    unsigned int write_dimension;
		struct WriteSize { unsigned int x_size, y_size, z_size; } WriteSize_Type;
		struct WriteSpacing { double x_spacing, y_spacing, z_spacing; } WriteSpacing_Type;
		struct WriteOrigin { double x_origin, y_origin, z_origin; } WriteOrigin_Type;
		struct WriteFlipAxis { bool x_flipaxis, y_flipaxis, z_flipaxis; } WriteFlipAxis_Type;

		Image::Image(std::string readpath)
		{
			(Image::read_status) = NONE;
			(Image::read_path) = readpath;
			(Image::read_datatype) = "";
			(Image::read_dimension) = 0;
			(Image::ReadSize_Type).x_size = 0, (Image::ReadSize_Type).y_size = 0, (Image::ReadSize_Type).z_size = 0;
			(Image::ReadSpacing_Type).x_spacing = 0, (Image::ReadSpacing_Type).y_spacing = 0, (Image::ReadSpacing_Type).z_spacing = 0;
			(Image::ReadOrigin_Type).x_origin = 0, (Image::ReadOrigin_Type).y_origin = 0, (Image::ReadOrigin_Type).z_origin = 0;
			(Image::ReadFlipAxis_Type).x_flipaxis = false, (Image::ReadFlipAxis_Type).y_flipaxis = false, (Image::ReadFlipAxis_Type).z_flipaxis = false;

			(Image::write_status) = NONE;
			(Image::write_path) = "";
			(Image::write_datatype) = "";
			(Image::write_dimension) = 0;
			(Image::WriteSize_Type).x_size= 0, (Image::WriteSize_Type).y_size = 0, (Image::WriteSize_Type).z_size = 0;
			(Image::WriteSpacing_Type).x_spacing = 0, (Image::WriteSpacing_Type).y_spacing = 0, (Image::WriteSpacing_Type).z_spacing = 0;
			(Image::WriteOrigin_Type).x_origin = 0, (Image::WriteOrigin_Type).y_origin = 0, (Image::WriteOrigin_Type).z_origin = 0;
			(Image::WriteFlipAxis_Type).x_flipaxis = false, (Image::WriteFlipAxis_Type).y_flipaxis = false, (Image::WriteFlipAxis_Type).z_flipaxis = false;
		}
	};


	MainWindow_Model* m_window;
	std::vector<ImageWriteThread::Image> images;

signals:

	void W_SendStartTask(std::string);
	void W_SendFinishedTask(std::string);
	void W_SendJobDone(bool);

public:

	void run();
	std::vector<ImageWriteThread::Image> GetAllTask();
	void UpdateInputFileInfo();
	void SetTasksPerThread(std::string readpath);
	void SetModel(MainWindow_Model* m_window_from_GUI);
	void ConfigureTask();
	itk::IOComponentEnum GetIOPixelTypeFromString(std::string DataTypeStr);
};

#endif 

