/*=========================================================================
 * Author : Minseob Lee
 * 2020.11.14 Declares a template class with basic functions
 * 2020.11.15 Declares importfiles and deportfiles
 *=========================================================================*/

#pragma once

#ifndef _INPUTFILEMANAGER_
#define _INPUTFILEMANAGER_

#include <set>
#include "../status_def.h"
#include "file_manager.h"
#include "../../Model/mainwindow_model.h"

template <typename FileType>
class InputFileManager : public FileManager <typename FileType>
{
public : 

	FileType& InputFileManager::ImportFiles(FileType FileNames, MainWindow_Model* m_window)
	{
		// We don't insert files which already exist in DB.

		std::set <std::string> DB_Filenames;
		for (const auto filename_in_model : m_window->GetAllSourcePathValue())
		{
			DB_Filenames.insert(filename_in_model); // first
		}

		for (const auto FileName : FileNames)
		{
			std::string StdStringFileName = FileName.toStdString();
			std::string extension = GetExtension(StdStringFileName);

			if (DB_Filenames.find(StdStringFileName) != DB_Filenames.end()) // This File already exists in db   
			{
				continue;
			}
			if (ValidFormatRegistry.find(extension) == ValidFormatRegistry.end()) 
			{
				InValidFormatFiles.push_back(FileName);  // This Extension is not supported, this is a illegal file 
			}
			else 
			{
			    ValidFormatFiles.insert(StdStringFileName); // This Extension is supported, update to model
			}
		}

	    m_window->FillFileList(ValidFormatFiles); // No illegal files updated to db
		return InValidFormatFiles;
	}

	void InputFileManager::DeportFile(FileType dummy_files, MainWindow_Model* m_window)
	{
		for (const auto dummy_file : dummy_files)
		{
			std::string StdStringFileName = dummy_file.toStdString();
			m_window->DeleteInputFile(StdStringFileName);
		}
	}

	Status InputFileManager::GetInputFileStatus(FileType clicked_filename, MainWindow_Model* m_window)
	{
		Status status = NONE;
		std::string clicked_filename_str = clicked_filename.toStdString();

		for (auto File : m_window->AccessListedFilesVec())
		{
			if (File.GetInputPathValueAsString() == clicked_filename_str)
			{
				status = File.GetInputFileStatus();
				break;
			}
		}

		return status;
	}

	FileType InputFileManager::GetInputFilenamesValue(MainWindow_Model* m_window)
	{
		FileType Filenames;

		for (const auto m_inputfilename : m_window->GetAllSourcePathValue())
		{
			Filenames.push_back(m_inputfilename.data());
		}
		return Filenames;
	}

	unsigned int InputFileManager::GetDimension(FileType clicked_filename, MainWindow_Model* m_window)
	{
		unsigned int input_dimension = 3;

		std::string clicked_filename_str = clicked_filename.toStdString();

		for (auto File : m_window->AccessListedFilesVec())
		{
			if (File.GetInputPathValueAsString() == clicked_filename_str)
			{
				input_dimension = std::stoul(File.GetInputDimensionAsString(), nullptr, 0);
				break;
			}
		}

		return input_dimension;
	}

	std::vector<std::string> InputFileManager::GetInputFileLiteralInfo(FileType clicked_filename, MainWindow_Model* m_window)
	{
		std::vector<std::string> literal_clicked_fileinfo;
		std::string clicked_filename_str = clicked_filename.toStdString();

		for (auto File : m_window->AccessListedFilesVec())
		{
			if (File.GetInputPathValueAsString() == clicked_filename_str)
			{
				literal_clicked_fileinfo.push_back(File.GetInputPathValueAsString());
				literal_clicked_fileinfo.push_back(File.GetInputDataTypeAsString());
				literal_clicked_fileinfo.push_back(File.GetInputDimensionAsString());
				break;
			}
		}

		return literal_clicked_fileinfo;
	};


	std::vector<std::vector<std::string>> InputFileManager::GetInputFileNumericInfo(FileType clicked_filename, MainWindow_Model* m_window)
	{
		std::vector<std::vector<std::string>> numeric_clicked_fileinfo;
		std::string clicked_filename_str = clicked_filename.toStdString();

		for (auto File : m_window->AccessListedFilesVec())
		{
			if (File.GetInputPathValueAsString() == clicked_filename_str)
			{
				numeric_clicked_fileinfo.push_back(File.GetInputSizeValueAsStringVector());
				numeric_clicked_fileinfo.push_back(File.GetInputSpacingValueAsStringVector());
				numeric_clicked_fileinfo.push_back(File.GetInputOriginValueAsStringVector());
				numeric_clicked_fileinfo.push_back(File.GetInputFlipAxisValueAsStringVector());
				break;
			}
		}

		return numeric_clicked_fileinfo;
	};

protected:

	FileType InValidFormatFiles;
	std::set <std::string> ValidFormatFiles;
};

#endif 