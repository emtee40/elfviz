/* -------------------------------------------------------------------------
 *
 * micro C Runtime, OS indepentend platform
 * ----------------------------------------
 *
 * Copyright (C) 2008 Song-Hwan Kim
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * -------------------------------------------------------------------------
*/

#ifndef __UC_RUNTIME_H__
#define __UC_RUNTIME_H__

#define ctrue 1
#define cfalse 0
#define cnull 0

typedef void cvoid;
typedef unsigned char cbool;
typedef unsigned char cbyte;

#ifdef RT_STATIC
#	define RT_API 
#else /*RT_STATIC*/
#	ifdef _WIN32 
#		ifdef LIBUCRT_DLL_EXPORTS
#			define RT_API __declspec(dllexport)
#		else /*_WIN32*/
#			define RT_API __declspec(dllimport)
#		endif /*_WIN32*/
#	elif __APPLE__
#		define RT_API extern
#	else
#		define RT_API
#	endif /*_WIN32*/
#endif /*RT_STATIC*/


#include "ucrt_file.h"
#include "ucrt_std.h"
#include "ucrt_tree.h"
#include "ucrt_sym_tree.h"
#endif //__UC_RUNTIME_H__
