#pragma once
#ifndef _IMAGEREADTHREAD_
#define _IMAGEREADTHREAD_

#include <QThread>
#include <tuple>
#include <vector>
#include "../constant.h"
#include "../status_def.h"
#include "../IO/file_manager.h"
#include "../../Model/mainwindow_model.h"
#include "imageReadBase.h"
#include "itkImage.h"
#include "itkImageIOBase.h"
#include "itkImageFileReader.h"
#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"

class ImageReadThread : public QThread
{
	Q_OBJECT

public:

	explicit ImageReadThread(QObject* parent = 0);
	~ImageReadThread();

	ImageReadThread* operator->() { return ImageReadThread_ptr; };
	ImageReadThread* ImageReadThread_ptr = this;
	ImageReadThread* operator [] (unsigned int thread_index) { return (this + thread_index); }

	struct Image
	{
		Status read_status;
		std::string read_path;
		std::string read_datatype;
		unsigned int read_dimension;
		struct ReadSize { unsigned int x_size, y_size, z_size; } ReadSize_Type;
		struct ReadSpacing { double x_spacing, y_spacing, z_spacing; } ReadSpacing_Type;
		struct ReadOrigin { double x_origin, y_origin, z_origin; } ReadOrigin_Type;
		struct ReadFlipAxis { bool x_flipaxis, y_flipaxis, z_flipaxis; } ReadFlipAxis_Type;

		Image::Image(std::string imagepath)
		{
			(Image::read_status) = NONE;
			(Image::read_path) = imagepath;
			(Image::read_datatype) = "";
			(Image::read_dimension) = 0;
			(Image::ReadSize_Type).x_size = 0, (Image::ReadSize_Type).y_size = 0, (Image::ReadSize_Type).z_size = 0;
			(Image::ReadSpacing_Type).x_spacing = 0, (Image::ReadSpacing_Type).y_spacing = 0, (Image::ReadSpacing_Type).z_spacing = 0;
			(Image::ReadOrigin_Type).x_origin = 0, (Image::ReadOrigin_Type).y_origin = 0, (Image::ReadOrigin_Type).z_origin = 0;
			(Image::ReadFlipAxis_Type).x_flipaxis = false, (Image::ReadFlipAxis_Type).y_flipaxis = false, (Image::ReadFlipAxis_Type).z_flipaxis = false;
		}
	};

	MainWindow_Model* m_window;
	std::vector<ImageReadThread::Image> images;

signals:

	void R_SendStartTask(std::string);
	void R_SendFinishedTask(std::string);
	void R_SendJobDone(bool);

public:

	void run();
	std::vector<ImageReadThread::Image> GetAllTask();
	void UpdateInputFileInfo();
	void SetTasksPerThread(std::string TaskName);
	void SetModel(MainWindow_Model* m_window_from_GUI);
};

#endif 

