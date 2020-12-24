/*=========================================================================
  *
  *  Copyright NumFOCUS
  *
  *  Licensed under the Apache License, Version 2.0 (the "License");
  *  you may not use this file except in compliance with the License.
  *  You may obtain a copy of the License at
  *
  *         http://www.apache.org/licenses/LICENSE-2.0.txt
  *
  *  Unless required by applicable law or agreed to in writing, software
  *  distributed under the License is distributed on an "AS IS" BASIS,
  *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  *  See the License for the specific language governing permissions and
  *  limitations under the License.
  *
  *=========================================================================*/

  // Place common enumerations to be used throughout the toolkit in this file
#ifndef itkCommonEnums_h
#define itkCommonEnums_h

#include "itkIntTypes.h"
#include <ostream>

namespace itk
{

	class CommonEnums
	{
	public:
		// Used in Input/Output for Images/Mesh/Transform types
		enum class IOPixel : uint8_t
		{
			UNKNOWNPIXELTYPE,
			SCALAR,
			RGB,
			RGBA,
			OFFSET,
			VECTOR,
			POINT,
			COVARIANTVECTOR,
			SYMMETRICSECONDRANKTENSOR,
			DIFFUSIONTENSOR3D,
			COMPLEX,
			FIXEDARRAY,
			ARRAY,
			MATRIX,
			VARIABLELENGTHVECTOR,
			VARIABLESIZEMATRIX
		};

		enum class IOComponent : uint8_t
		{
			UNKNOWNCOMPONENTTYPE,
			UCHAR,
			CHAR,
			USHORT,
			SHORT,
			UINT,
			INT,
			ULONG,
			LONG,
			LONGLONG,
			ULONGLONG,
			FLOAT,
			DOUBLE,
			LDOUBLE
		};

		enum class IOFile : uint8_t
		{
			ASCII = 0,
			Binary = 1,
			TypeNotApplicable = 2,
			// for backward compatibility
			BINARY = 1,           // Spelling difference between IOMeshBase and IOImageBase
			TYPENOTAPPLICABLE = 2 // Spelling difference between IOMeshBase and IOImageBase
		};

		enum class IOFileMode : uint8_t
		{
			ReadMode,
			WriteMode
		};

		enum class IOByteOrder : uint8_t
		{
			BigEndian,
			LittleEndian,
			OrderNotApplicable
		};

		enum class CellGeometry : uint8_t
		{
			VERTEX_CELL = 0,
			LINE_CELL = 1,
			TRIANGLE_CELL = 2,
			QUADRILATERAL_CELL = 3,
			POLYGON_CELL = 4,
			TETRAHEDRON_CELL = 5,
			HEXAHEDRON_CELL = 6,
			QUADRATIC_EDGE_CELL = 7,
			QUADRATIC_TRIANGLE_CELL = 8,
			LAST_ITK_CELL = 9,
			MAX_ITK_CELLS = 255
		};

	}; // CommonEnums

	// Convenience
	using IOPixelEnum = CommonEnums::IOPixel;
	using IOComponentEnum = CommonEnums::IOComponent;
	using IOFileEnum = CommonEnums::IOFile;
	using IOFileModeEnum = CommonEnums::IOFileMode;
	using IOByteOrderEnum = CommonEnums::IOByteOrder;
	using CellGeometryEnum = CommonEnums::CellGeometry;

#if !defined(ITK_LEGACY_REMOVE)

	using IOPixelType = CommonEnums::IOPixel;
	using IOComponentType = CommonEnums::IOComponent;
	using IOFileType = CommonEnums::IOFile;
	using IOFileModeType = CommonEnums::IOFileMode;
	using IOByteOrderType = CommonEnums::IOByteOrder;
	using CellGeometryType = CommonEnums::CellGeometry;
#endif
	// Define how to print enumeration
	extern ITKCommon_EXPORT std::ostream &
		operator<<(std::ostream & out, IOPixelEnum value);
	extern ITKCommon_EXPORT std::ostream &
		operator<<(std::ostream & out, IOComponentEnum value);
	extern ITKCommon_EXPORT std::ostream &
		operator<<(std::ostream & out, IOFileEnum value);
	extern ITKCommon_EXPORT std::ostream &
		operator<<(std::ostream & out, IOFileModeEnum value);
	extern ITKCommon_EXPORT std::ostream &
		operator<<(std::ostream & out, IOByteOrderEnum value);
	extern ITKCommon_EXPORT std::ostream &
		operator<<(std::ostream & out, CellGeometryEnum value);

	class MeshEnums
	{
	public:
		enum class MeshClassCellsAllocationMethod : uint8_t
		{
			CellsAllocationMethodUndefined,
			CellsAllocatedAsStaticArray,
			CellsAllocatedAsADynamicArray,
			CellsAllocatedDynamicallyCellByCell
		};
	};
	extern ITKCommon_EXPORT std::ostream &
		operator<<(std::ostream & out, MeshEnums::MeshClassCellsAllocationMethod value);
	class OctreeEnums
	{
	public:
		enum class Octree : uint8_t
		{
			UNKNOWN_PLANE,
			SAGITAL_PLANE,
			CORONAL_PLANE,
			TRANSVERSE_PLANE
		};

		/***
		   * \class LeafIdentifier
		   * \ingroup ITKCommon
		   */
		enum class LeafIdentifier : uint8_t
		{
			ZERO = 0,
			ONE = 1,
			TWO = 2,
			THREE = 3,
			FOUR = 4,
			FIVE = 5,
			SIX = 6,
			SEVEN = 7
		};
	};

	// Define how to print enumeration
	extern ITKCommon_EXPORT std::ostream &
		operator<<(std::ostream & out, const OctreeEnums::Octree value);
	extern ITKCommon_EXPORT std::ostream &
		operator<<(std::ostream & out, const OctreeEnums::LeafIdentifier value);
	class ObjectEnums
	{
	public:
		enum class RegionEnum : uint8_t
		{
			ITK_UNSTRUCTURED_REGION,
			ITK_STRUCTURED_REGION
		};
	};

	extern ITKCommon_EXPORT std::ostream &
		operator<<(std::ostream & out, const ObjectEnums::RegionEnum value);

	class ObjectFactoryEnums
	{
	public:
		enum class InsertionPosition : uint8_t
		{
			INSERT_AT_FRONT,
			INSERT_AT_BACK,
			INSERT_AT_POSITION
		};
	};
	extern ITKCommon_EXPORT std::ostream &
		operator<<(std::ostream & out, const ObjectFactoryEnums::InsertionPosition value);

} // namespace itk

#endif // itkCommonEnums_h