#pragma once
#ifndef _IMAGEMERGEBASE_
#define _IMAGEMERGEBASE_

#include <vector>
#include "itkTileImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkJoinImageFilter.h"
#include "itkVectorImageToImageAdaptor.h"
#include "itkNumericSeriesFileNames.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMetaDataObject.h"
#include "itkImage.h"
#include "itkMath.h"
#include "itkCommonEnums.h"
#include "../status_def.h"

struct ImageMerge
{
	Status status;
	std::vector<std::string> filenames;
	itk::IOComponentEnum component;
	unsigned int dimension;

	void ImageMerge::init()
	{
		filenames.clear();
	}
};

template <unsigned int VDimension, typename PixelType>
Status MergeImage(ImageMerge InFile, ImageMerge OutFile)
{
	using InputImageType = itk::Image<PixelType, VDimension>;
	using OutputImageType = itk::Image<PixelType, 3>;
	using ImageReaderType = itk::ImageFileReader<InputImageType>;
	using TilerType = itk::TileImageFilter<InputImageType, OutputImageType>;

	TilerType::Pointer tiler = TilerType::New();
	itk::FixedArray<unsigned int, 3> layout;
	layout[0] = 1;
	layout[1] = 1;
	layout[2] = 0;

	tiler->SetLayout(layout);
	unsigned int inputImageNumber = 0;

	ImageReaderType::Pointer reader = ImageReaderType::New();
	InputImageType::Pointer inputImageTile;

	for (auto filename : InFile.filenames)
	{
		reader->SetFileName(filename);
		reader->UpdateLargestPossibleRegion();
		inputImageTile = reader->GetOutput();
		inputImageTile->DisconnectPipeline();
		tiler->SetInput(inputImageNumber++, inputImageTile);
	}

	PixelType filler = 65535;
	tiler->SetDefaultPixelValue(filler);
	tiler->Update();

	using WriterType = itk::ImageFileWriter<OutputImageType>;
	WriterType::Pointer writer = WriterType::New();
	writer->SetInput(tiler->GetOutput());
	writer->SetFileName(OutFile.filenames.front().data());

	try
	{
		writer->Update();
	}
	catch (itk::ExceptionObject& excp)
	{
		return FILE_NOT_WRITEABLE;
	}
	return NORMAL;
}


template <unsigned int VDimension>
Status MergeScalarImage(ImageMerge InFile, ImageMerge OutFile)
{
	switch (InFile.component)
	{
		default:
		case itk::IOComponentEnum::UNKNOWNCOMPONENTTYPE: // For UNKNOWNCOMPONENTTYPE, try "float" type to open an image. 
		{
			return FILE_NOT_READABLE;
		}
		case itk::IOComponentEnum::UCHAR:
		{
			return MergeImage<VDimension, unsigned char>(InFile, OutFile);
		}
		case itk::IOComponentEnum::CHAR:
		{
			return MergeImage<VDimension, char>(InFile, OutFile);
		}
		case itk::IOComponentEnum::USHORT:
		{
			return MergeImage<VDimension, unsigned short>(InFile, OutFile);
		}
		case itk::IOComponentEnum::SHORT:
		{
			return MergeImage<VDimension, short>(InFile, OutFile);
		}
		case itk::IOComponentEnum::UINT:
		{
			return MergeImage<VDimension, unsigned int>(InFile, OutFile);
		}
		case itk::IOComponentEnum::INT:
		{
			return MergeImage<VDimension, int>(InFile, OutFile);
		}
		case itk::IOComponentEnum::ULONG:
		{
			return MergeImage<VDimension, unsigned long>(InFile, OutFile);
		}
		case itk::IOComponentEnum::LONG:
		{
			return MergeImage<VDimension, long>(InFile, OutFile);
		}
		case itk::IOComponentEnum::FLOAT:
		{
			return MergeImage<VDimension, float>(InFile, OutFile);
		}
		case itk::IOComponentEnum::DOUBLE:
		{
			return MergeImage<VDimension, double>(InFile, OutFile);
		}
	}
}

#endif 