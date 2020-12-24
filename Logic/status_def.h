/*=========================================================================
 * Author : Minseob Lee
 * 2020.11.14 Declares a enum with possible errors when a file is loaded.
 *=========================================================================*/

#pragma once

#ifndef _STATUS_
#define _STATUS_

#include <vector>

enum Status
{
	NONE,
	EMPTY,
	NORMAL,
	FILE_NOT_SELECTED,
	FILE_NOT_FOUND,
	FILE_NOT_READ,
	FILE_NOT_READABLE,
	FILE_NOT_WRITEABLE,
	FILE_ALREADY_EXISTS,
	FILESIZE_MODIFYING,
};

enum UserResponse
{
	OK,
	CANCEL,
};

#endif 