#pragma once
#ifndef _IMAGEWRITEBASE_
#define _IMAGEWRITEBASE_

#include <vector>
#include "itkCommonEnums.h"
#include "itkImage.h"
#include "itkImageIOBase.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkFlipImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkScaleTransform.h"
#include "../status_def.h"

struct ImageModi
{
	Status status;
	std::string filename;
	itk::IOComponentEnum component;
	unsigned int dimension;
	std::vector<unsigned int> size;
	std::vector<double> spacing;
	std::vector<double> origin;
	std::vector<bool> flip;
};

template <const unsigned int Dimension>
Status Is_FIleSize_Changing(ImageModi InFile, ImageModi OutFile)
{
	Status status = NONE;
	for (int i = 0; i < Dimension; i++)
	{
		if (InFile.size[i] != OutFile.size[i]) return FILESIZE_MODIFYING;
	}
	return NONE;
}

template <const unsigned int Dimension, typename InputPixel, typename OutputPixel>
Status WriteImage(ImageModi InFile, ImageModi OutFile)
{
	using InputImageType = itk::Image<InputPixel, Dimension>;
	using OutputImageType = itk::Image<OutputPixel, Dimension>;

	using ScalarType = double;
	using ReaderType = itk::ImageFileReader<InputImageType>;
	ReaderType::Pointer reader = ReaderType::New();

	reader->SetFileName(InFile.filename);
	
	try
	{
		reader->Update();
	}
	catch (itk::ExceptionObject & error)
	{
		return FILE_NOT_WRITEABLE;
	}
	
	InputImageType::Pointer inputImage = reader->GetOutput();
	InputImageType::RegionType  inputRegion = inputImage->GetLargestPossibleRegion();
	InputImageType::SpacingType inputSpacing = inputImage->GetSpacing();
	InputImageType::PointType InputOrigin = inputImage->GetOrigin();
	InputOrigin[0] = 0;
	InputOrigin[1] = 0;
	InputOrigin[2] = 0;
	inputImage->SetOrigin(InputOrigin);

	std::vector <float> scale;
	for (int i = 0; i < Dimension; i++)
	{
		const float coord_scale = static_cast<float>(OutFile.size[i]) / static_cast<float>(InFile.size[i]);
		scale.push_back(coord_scale);
	}

	itk::Point<ScalarType, Dimension> centralPoint;
	for (int i = 0; i < Dimension ; i++) centralPoint[i] = InputOrigin[i] * scale[i];

	using ScaleTransformType = itk::ScaleTransform<ScalarType, Dimension>;
	ScaleTransformType::Pointer scaleTransform = ScaleTransformType::New();
	ScaleTransformType::ParametersType parameters = scaleTransform->GetParameters();

	for (int i = 0; i < Dimension; i++) parameters[i] = 1 / scale[i];

	scaleTransform->SetParameters(parameters);
	scaleTransform->SetCenter(centralPoint);

	using LinearInterpolatorType = itk::LinearInterpolateImageFunction<InputImageType, ScalarType>;
	LinearInterpolatorType::Pointer interpolator = LinearInterpolatorType::New();

	using ResampleFilterType = itk::ResampleImageFilter<InputImageType, OutputImageType>;
	ResampleFilterType::Pointer resampleFilter = ResampleFilterType::New();
	OutputImageType::SizeType    resized_size = inputRegion.GetSize();
	OutputImageType::PointType newOrigin;

	for (int i = 0; i < Dimension ; i++) resized_size[i] *= scale[i];
	for (int i = 0; i < Dimension ; i++) newOrigin[i] = 0;

	resampleFilter->SetInput(inputImage);
	resampleFilter->SetOutputOrigin(newOrigin);
	resampleFilter->SetTransform(scaleTransform);
	resampleFilter->SetInterpolator(interpolator);
	resampleFilter->SetSize(resized_size);
	resampleFilter->SetOutputSpacing(inputSpacing);
	resampleFilter->UpdateOutputInformation();
	resampleFilter->Update();

	using WriterType = itk::ImageFileWriter<OutputImageType>;
	WriterType::Pointer writer = WriterType::New();

	OutputImageType::Pointer OutputImage = resampleFilter->GetOutput();
	OutputImageType::SpacingType OutSpacing;
	OutputImageType::PointType OutOrigin;

	if (Is_FIleSize_Changing<Dimension>(InFile, OutFile) == FILESIZE_MODIFYING)
	{
		for (int i = 0; i < Dimension; i++)
		{
			OutSpacing[i] = (1 / scale[i]) * inputSpacing[i];
			OutOrigin[i] = 0; // Acutally, scale[i] * InputOrigin[i] is correct. However, 0 is compatible with jiwoong's work
		} 
	}
	else 
	{
		for (int i = 0; i < Dimension; i++)
		{
			OutSpacing[i] = OutFile.spacing[i];
			OutOrigin[i] = OutFile.origin[i];
		}
	}

	OutputImage->SetSpacing(OutSpacing);
	OutputImage->SetOrigin(OutOrigin);

	using FlipImageFilterType = itk::FlipImageFilter<OutputImageType>;
	FlipImageFilterType::Pointer flipFilter = FlipImageFilterType::New();
	flipFilter->SetInput(OutputImage);

	FlipImageFilterType::FlipAxesArrayType flipAxes;
	for (int i = 0; i < Dimension; i++) flipAxes[i] = InFile.flip[i] ^ OutFile.flip[i];
	flipFilter->SetFlipAxes(flipAxes);

	writer->SetFileName(OutFile.filename);
	writer->SetInput(flipFilter->GetOutput());

	try
	{
		writer->Update();
	}
	catch (itk::ExceptionObject & error)
	{
		return FILE_NOT_WRITEABLE;
	}
	
	return NORMAL;
}

template <unsigned int VDimension, typename InputPixel>
Status AddWriteImage(ImageModi InFile, ImageModi OutFile)
{
	switch (OutFile.component)
	{
		default:
		case itk::IOComponentEnum::UNKNOWNCOMPONENTTYPE: // For UNKNOWNCOMPONENTTYPE, try "float" type to open an image. 
		{
			return FILE_NOT_WRITEABLE;
		}
		case itk::IOComponentEnum::UCHAR:
		{
			return WriteImage<VDimension, InputPixel, unsigned char>(InFile, OutFile);
		}
		case itk::IOComponentEnum::CHAR:
		{
			return WriteImage<VDimension, InputPixel, char>(InFile, OutFile);
		}
		case itk::IOComponentEnum::USHORT:
		{
			return WriteImage<VDimension, InputPixel, unsigned short>(InFile, OutFile);
		}
		case itk::IOComponentEnum::SHORT:
		{
			return WriteImage<VDimension, InputPixel, short>(InFile, OutFile);
		}
		case itk::IOComponentEnum::UINT:
		{
			return WriteImage<VDimension, InputPixel, unsigned int>(InFile, OutFile);
		}
		case itk::IOComponentEnum::INT:
		{
			return WriteImage<VDimension, InputPixel, int>(InFile, OutFile);
		}
		case itk::IOComponentEnum::ULONG:
		{
			return WriteImage<VDimension, InputPixel, unsigned long>(InFile, OutFile);
		}
		case itk::IOComponentEnum::LONG:
		{
			return WriteImage<VDimension, InputPixel, long>(InFile, OutFile);
		}
		case itk::IOComponentEnum::FLOAT:
		{
			return WriteImage<VDimension, InputPixel, float>(InFile, OutFile);
		}
		case itk::IOComponentEnum::DOUBLE:
		{
			return WriteImage<VDimension, InputPixel, double>(InFile, OutFile);
		}
	}
}

template <unsigned int VDimension>
Status ReadWriteScalarImage(ImageModi InFile, ImageModi OutFile)
{
	switch (InFile.component)
	{
		default:
		case itk::IOComponentEnum::UNKNOWNCOMPONENTTYPE: // For UNKNOWNCOMPONENTTYPE, try "float" type to open an image. 
		{
			return FILE_NOT_WRITEABLE;
		}
		case itk::IOComponentEnum::UCHAR:
		{
			return AddWriteImage<VDimension, unsigned char>(InFile, OutFile);
		}
		case itk::IOComponentEnum::CHAR:
		{
			return AddWriteImage<VDimension, char>(InFile, OutFile);
		}
        case itk::IOComponentEnum::USHORT:
		{
			return AddWriteImage<VDimension, unsigned short>(InFile, OutFile);
		}
		case itk::IOComponentEnum::SHORT:
		{
			return AddWriteImage<VDimension, short>(InFile, OutFile);
		}
	    case itk::IOComponentEnum::UINT:
		{
			return AddWriteImage<VDimension, unsigned int>(InFile, OutFile);
		}
        case itk::IOComponentEnum::INT:
		{
			return AddWriteImage<VDimension, int>(InFile, OutFile);
		}
		case itk::IOComponentEnum::ULONG:
		{
			return AddWriteImage<VDimension, unsigned long>(InFile, OutFile);
		}
		case itk::IOComponentEnum::LONG:
		{
			return AddWriteImage<VDimension, long>(InFile, OutFile);
		}
        case itk::IOComponentEnum::FLOAT:
		{
			return AddWriteImage<VDimension, float>(InFile, OutFile);
		}
        case itk::IOComponentEnum::DOUBLE:
		{
			return AddWriteImage<VDimension, double>(InFile, OutFile);
		}
	}
}

#endif 

