#pragma once
#ifndef _IMAGEREADBASE_
#define _IMAGEREADBASE_

#include <tuple>
#include <vector>
#include "itkCommonEnums.h"
#include "itkImageIOBase.h"
#include "itkImageFileReader.h"
#include "itkFlipImageFilter.h"
#include "../status_def.h"

template <class TImage, unsigned int VDimension>
std::tuple<std::vector<unsigned int>, std::vector<double>, std::vector<double>, std::vector<bool>> ReadImage(const char * fileName, typename TImage::Pointer image)
{
	using ImageType = TImage;
	using ImageReaderType = itk::ImageFileReader<ImageType>;
	using FlipImageFilterType = itk::FlipImageFilter<ImageType>;
	FlipImageFilterType::Pointer flipFilter = FlipImageFilterType::New();
	typename ImageReaderType::Pointer reader = ImageReaderType::New();

	reader->SetFileName(fileName);
	reader->Update();
	image->Graft(reader->GetOutput());
	flipFilter->SetInput(reader->GetOutput());

	ImageType::SizeType size_arr = image->GetLargestPossibleRegion().GetSize();
	std::vector<unsigned int> sizevec;
	for (int i = 0; i < VDimension; i++) sizevec.push_back(size_arr[i]);

	ImageType::SpacingType spacing_arr = image->GetSpacing();
	std::vector<double> spacingvec;
	for (int j = 0; j < VDimension; j++) spacingvec.push_back(spacing_arr[j]);

	ImageType::PointType origin_arr = image->GetOrigin();
	std::vector<double> originvec;
	for (int k = 0; k < VDimension; k++) originvec.push_back(origin_arr[k]);

	FlipImageFilterType::FlipAxesArrayType flipAxes = flipFilter->GetFlipAxes();
	std::vector<bool> flipvec;
	for (int m = 0; m < VDimension; m++) flipvec.push_back(flipAxes[m]);
	
	std::tuple<std::vector<unsigned int>, std::vector<double>, std::vector<double>, std::vector<bool>> imageinfo (sizevec, spacingvec, originvec, flipvec);
	reader = NULL;
	return imageinfo;
}

template <unsigned int VDimension>
std::tuple<std::vector<unsigned int>, std::vector<double>, std::vector<double>, std::vector<bool>> ReadScalarImage(const char * FileName, const itk::IOComponentEnum componentType)
{
	switch (componentType)
	{
		default: 
		case itk::IOComponentEnum::UNKNOWNCOMPONENTTYPE: // For UNKNOWNCOMPONENTTYPE, try "float" type to open an image. 
		{
			using PixelType = float;
			using ImageType = itk::Image<PixelType, VDimension>;
			typename ImageType::Pointer image = ImageType::New();

			return ReadImage<ImageType, VDimension>(FileName, image);
		}
		case itk::IOComponentEnum::UCHAR:
		{
			using PixelType = unsigned char;
			using ImageType = itk::Image<PixelType, VDimension>;
			typename ImageType::Pointer image = ImageType::New();
			
			return ReadImage<ImageType, VDimension>(FileName, image);
		}

		case itk::IOComponentEnum::CHAR:
		{
			using PixelType = char;
			using ImageType = itk::Image<PixelType, VDimension>;
			typename ImageType::Pointer image = ImageType::New();
			
			return ReadImage<ImageType, VDimension>(FileName, image);
		}

		case itk::IOComponentEnum::USHORT:
		{
			using PixelType = unsigned short;
			using ImageType = itk::Image<PixelType, VDimension>;
			typename ImageType::Pointer image = ImageType::New();
			
			return ReadImage<ImageType, VDimension>(FileName, image);
		}

		case itk::IOComponentEnum::SHORT:
		{
			using PixelType = short;
			using ImageType = itk::Image<PixelType, VDimension>;
			typename ImageType::Pointer image = ImageType::New();
			
			return ReadImage<ImageType, VDimension>(FileName, image);
		}

		case itk::IOComponentEnum::UINT:
		{
			using PixelType = unsigned int;
			using ImageType = itk::Image<PixelType, VDimension>;
			typename ImageType::Pointer image = ImageType::New();
			
			return ReadImage<ImageType, VDimension>(FileName, image);
		}

		case itk::IOComponentEnum::INT:
		{
			using PixelType = int;
			using ImageType = itk::Image<PixelType, VDimension>;
			typename ImageType::Pointer image = ImageType::New();
			
			return ReadImage<ImageType, VDimension>(FileName, image);
		}

		case itk::IOComponentEnum::ULONG:
		{
			using PixelType = unsigned long;
			using ImageType = itk::Image<PixelType, VDimension>;
			typename ImageType::Pointer image = ImageType::New();
			
			return ReadImage<ImageType, VDimension>(FileName, image);
		}

		case itk::IOComponentEnum::LONG:
		{
			using PixelType = long;
			using ImageType = itk::Image<PixelType, VDimension>;
			typename ImageType::Pointer image = ImageType::New();

			return ReadImage<ImageType, VDimension>(FileName, image);
		}

		case itk::IOComponentEnum::FLOAT:
		{
			using PixelType = float;
			using ImageType = itk::Image<PixelType, VDimension>;
			typename ImageType::Pointer image = ImageType::New();

			return ReadImage<ImageType, VDimension>(FileName, image);
		}

		case itk::IOComponentEnum::DOUBLE:
		{
			using PixelType = double;
			using ImageType = itk::Image<PixelType, VDimension>;
			typename ImageType::Pointer image = ImageType::New();

			return ReadImage<ImageType, VDimension>(FileName, image);
		}
	}
}

#endif 

