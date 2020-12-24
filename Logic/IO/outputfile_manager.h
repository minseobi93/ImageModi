/*=========================================================================
 * Author : Minseob Lee
 * 2020.11.14 Declares a template class with basic functions
 *=========================================================================*/

#pragma once

#ifndef _OUTPUTFILEMANAGER_
#define _OUTPUTFILEMANAGER_

#include "file_manager.h"

template <typename FileType>
class OutputFileManager : public FileManager <typename FileType>
{

public:

	Status OutputFileManager::GetOutputFileStatus(FileType clicked_filename, MainWindow_Model* m_window)
	{
		Status status = NONE;
		std::string clicked_filename_str = clicked_filename.toStdString();

		for (auto File : m_window->AccessListedFilesVec())
		{
			if (File.GetOutputPathValueAsString() == clicked_filename_str)
			{
				status = File.GetOutputFileStatus();
				break;
			}
		}

		return status;
	}

	void OutputFileManager::SetOutputPath(FileType folderpath, MainWindow_Model* m_window)
	{
		for (auto& File : m_window->AccessListedFilesVec())
		{
			std::string folderpath_str = folderpath.toStdString();
			std::string filename_str = File.GetInputPathValueAsString();
			std::string savepath = GetSavePath(filename_str, folderpath_str);
			File.SetOutputPath(true, savepath);
		}
	}

	unsigned int OutputFileManager::GetDimension(FileType clicked_filename, MainWindow_Model* m_window)
	{
		unsigned int output_dimension = 3;

		std::string clicked_filename_str = clicked_filename.toStdString();

		for (auto File : m_window->AccessListedFilesVec())
		{
			if (File.GetInputPathValueAsString() == clicked_filename_str)
			{
				output_dimension = std::stoul(File.GetOutputDimensionAsString(), nullptr, 0);
				break;
			}
		}

		return output_dimension;
	}

	std::vector<std::string> OutputFileManager::GetOutputFileLiteralInfo(FileType clicked_filename, MainWindow_Model* m_window)
	{
		std::vector<std::string> literal_clicked_fileinfo;
		std::string clicked_filename_str = clicked_filename.toStdString();

		for (auto File : m_window->AccessListedFilesVec())
		{
			if (File.GetInputPathValueAsString() == clicked_filename_str)
			{
				literal_clicked_fileinfo.push_back(File.GetOutputPathValueAsString());
				literal_clicked_fileinfo.push_back(File.GetOutputDataTypeAsString());
				literal_clicked_fileinfo.push_back(File.GetOutputDimensionAsString());
				break;
			}
		}

		return literal_clicked_fileinfo;
	};


	std::vector<std::vector<std::string>> OutputFileManager::GetOutputFileNumericInfo(FileType clicked_filename, MainWindow_Model* m_window)
	{
		std::vector<std::vector<std::string>> numeric_clicked_fileinfo;
		std::string clicked_filename_str = clicked_filename.toStdString();

		for (auto File : m_window->AccessListedFilesVec())
		{
			if (File.GetInputPathValueAsString() == clicked_filename_str)
			{
				numeric_clicked_fileinfo.push_back(File.GetOutputSizeValueAsStringVector());
				numeric_clicked_fileinfo.push_back(File.GetOutputSpacingValueAsStringVector());
				numeric_clicked_fileinfo.push_back(File.GetOutputOriginValueAsStringVector());
				numeric_clicked_fileinfo.push_back(File.GetOutputFlipAxisValueAsStringVector());
				break;
			}
		}

		return numeric_clicked_fileinfo;
	};

};

#endif 