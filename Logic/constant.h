/*=========================================================================
 * Author : Minseob Lee
 * 2020.11.14 Added MAXPATH, FILESIZE
 * 2020.11.15 Added KILOBYTE ~ PETABYTE
 * 2020.11.16 Added EXPECTED_FILESIZE 
 * 2020.11.17 Added EXPECTED_MAX_RAMSIZE
 *=========================================================================*/

#pragma once

#ifndef _CONSTANT_
#define _CONSTANT_

constexpr unsigned int show_or_hide_col = 2;

constexpr unsigned int literal_tablerow = 3;
constexpr unsigned int literal_tablecolumn = 1;

constexpr unsigned int numeric_tablerow = 4;
constexpr unsigned int numeric_tablecolumn = 3;

constexpr double KILOBYTES = 1024;
constexpr double MEGABYTES = 1048576;    // 1024 * 1024
constexpr double GIGABYTES = 1073741824;  // 1024 * 1024 * 1024
constexpr double TERABYTES = 1099511627776; // 1024 * 1024 * 1024 * 1024
constexpr double PETABYTES = 1125899906842624;  // 1024 * 1024 * 1024 * 1024 * 1024

constexpr double MAXPATH = 1024;
constexpr double EXPECTED_AVERAGE_FILESIZE = 500 * MEGABYTES; // = 500 MB since filesize which is used in medical industry averages ~ 500 MB.  
constexpr double EXPECTED_MEMORY_PER_THREAD = 3 * EXPECTED_AVERAGE_FILESIZE; // We must reserve a space for 3 times of EXPECTED_AVERAGE_FILESIZE (read, execute, write)
constexpr unsigned char LEADER_THREAD_INDEX = 0; // Array always starts from 0

#endif //_CONSTANT_
