/*=========================================================================
 * Author : Minseob Lee
 * 2020.11.14 Describes a valid file formtat, extension function
 *=========================================================================*/

#pragma once

#ifndef _FILEMANAGER_
#define _FILEMANAGER_

#include <set>
#include "../status_def.h"
#include "../constant.h"
#include "../../Model/mainwindow_model.h"

template <typename FileType>
class FileManager
{
protected:

	void FileManager::FillFormatRegistry()
	{
		ValidFormatRegistry.insert("bmp"), ValidFormatRegistry.insert("BMP");   // 2D image
		ValidFormatRegistry.insert("png"); ValidFormatRegistry.insert("PNG");   // 2D image
		ValidFormatRegistry.insert("tif"), ValidFormatRegistry.insert("tiff");  // 2D image
		ValidFormatRegistry.insert("jpeg"), ValidFormatRegistry.insert("JPEG"),  // 2D image
		ValidFormatRegistry.insert("jpg"), ValidFormatRegistry.insert("JPG");   // 2D image
		ValidFormatRegistry.insert("dcm"), ValidFormatRegistry.insert("DCM");   // DICOM
		ValidFormatRegistry.insert("img"), ValidFormatRegistry.insert("hdr");   // ANALYZE
		ValidFormatRegistry.insert("nii"), ValidFormatRegistry.insert("nia"), ValidFormatRegistry.insert("NiFTI");   // NiFTI
		ValidFormatRegistry.insert("nrrd"), ValidFormatRegistry.insert("nhdr"); // NRRD
		ValidFormatRegistry.insert("gipl");                                     // GIPL
		ValidFormatRegistry.insert("mha"), ValidFormatRegistry.insert("mhd");   // MetaImage
		ValidFormatRegistry.insert("vtk");                                      // vtk
		ValidFormatRegistry.insert("cub");                                      // VoxBo CUB
	}

	std::set <std::string> ValidFormatRegistry;
	std::vector<std::vector<std::string>> clicked_fileinfo;

public:

	FileManager::FileManager()
	{
		FillFormatRegistry();
	}

	FileManager::~FileManager()
	{
		ValidFormatRegistry.clear();
	}

	double FileManager::GetTotalSizeAsBytes(QStringList FileNameList)
	{
		double total_bytes = 0;
		for (const auto filename : FileNameList)
		{
			FILE* fp = fopen(filename.toStdString().data(), "r");
			_fseeki64(fp, 0, SEEK_END);
			total_bytes += _ftelli64(fp);
			fclose(fp);
		}
		return total_bytes;
	}

	std::string FileManager::GetFileSizeAsString(double FileSize)
	{
		int floating_point = 0;
		std::string StrFileSize = "";
		if (FileSize == 0)
		{
			return StrFileSize;
		}
		else if ((0 < FileSize) && (FileSize < KILOBYTES))
		{
			StrFileSize = std::to_string(FileSize);
			floating_point = StrFileSize.find_last_of(".");
			StrFileSize = StrFileSize.substr(0, floating_point + 2) + "Bytes";
		}
		else if ((KILOBYTES <= FileSize) && (FileSize < MEGABYTES))
		{
			StrFileSize = std::to_string(FileSize / KILOBYTES);
			floating_point = StrFileSize.find_last_of(".");
			StrFileSize = StrFileSize.substr(0, floating_point + 2) + "KB";
		}
		else if ((MEGABYTES <= FileSize) && (FileSize < GIGABYTES))
		{
			StrFileSize = std::to_string(FileSize / MEGABYTES);
			floating_point = StrFileSize.find_last_of(".");
			StrFileSize = StrFileSize.substr(0, floating_point + 2) + "MB";
		}
		else if ((GIGABYTES <= FileSize) && (FileSize < TERABYTES))
		{
			StrFileSize = std::to_string(FileSize / GIGABYTES);
			floating_point = StrFileSize.find_last_of(".");
			StrFileSize = StrFileSize.substr(0, floating_point + 2) + "GB";
		}
		else if ((TERABYTES <= FileSize) && (FileSize < PETABYTES))
		{
			StrFileSize = std::to_string(FileSize / TERABYTES);
			floating_point = StrFileSize.find_last_of(".");
			StrFileSize = StrFileSize.substr(0, floating_point + 2) + "TB";
		}
		else if (PETABYTES <= FileSize)
		{
			StrFileSize = std::to_string(FileSize / PETABYTES);
			floating_point = StrFileSize.find_last_of(".");
			StrFileSize = StrFileSize.substr(0, floating_point + 2) + "PB";
		}

		return StrFileSize;
	}

	std::string FileManager::GetSavePath(std::string readpath, std::string save_folderpath)
	{
		int first_ch_filename = readpath.find_last_of("/");
		std::string filename = readpath.substr(first_ch_filename, readpath.size() - first_ch_filename);
		std::string writepath = save_folderpath + filename;
		return writepath;
	}

	Status FileManager::IsValidSavePath(FileType folderpath, std::set<std::string> m_filenames)
	{
		std::string folderpath_str = folderpath.toStdString();
		if (folderpath_str.empty()) return EMPTY;

		for (const auto m_filename : m_filenames)
		{
			std::string writepath = GetSavePath(m_filename, folderpath_str);

			if (writepath == m_filename) return FILE_ALREADY_EXISTS;
		}

		return NORMAL;
	}

	std::string FileManager::GetExtension(std::string s)
	{
		int extension_index = s.find_last_of(".");
		std::string extension_str = s.substr((extension_index + 1), s.length() - extension_index - 1);

		if (extension_str == "gz") // "gz" file found. So se need second check for a compressed image (.gz)
		{
			std::string temp_str = s.substr(0, extension_index);
			extension_str.clear();
			extension_index = temp_str.find_last_of(".");
			extension_str = temp_str.substr((extension_index + 1), temp_str.length() - extension_index - 1);
		}
		return extension_str;
	}

	std::string FileManager::GetFileNameWithoutExtension(std::string s)
	{
		return s.substr(0, s.find_first_of(".", 0));
	}
};

#endif 