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

#include "ucrt.h"

#include <string.h>
#include <stdio.h>
#include "rt_file_interface.h"
#include "ucrt_posix_file.h"

static int stdo_is_eof(_rt_file_t* file){	return cfalse;}
static char* stdo_get_line(_rt_file_t* file, char* buf, int len){	return 0;}
static int stdo_flush(_rt_file_t* file){
	RT_DECLARE_POSIX_FILE(rf, file)
	return fflush(rf->fd);
}

static char stdo_get_char(_rt_file_t* file){
	RT_DECLARE_POSIX_FILE(rf, file)
	return getc(rf->fd);
}

static void stdo_unget_char(_rt_file_t* file){return;}
static void stdo_final(_rt_file_t* file){	return;}
static void stdo_print(_rt_file_t* file, char* format, rt_list list){
	RT_DECLARE_POSIX_FILE(rf, file)
	vfprintf(rf->fd, format, list);
}
int stdo_write(_rt_file_t* file, void* buf, int size){
	RT_DECLARE_POSIX_FILE(rf, file)
	return fwrite(buf, size, 1, rf->fd);
}

cbool stdo_seek(_rt_file_t* file, int offset, int origin){ return cfalse; }
int stdo_size(_rt_file_t* file){ return 0; }
void* stdo_get_ptr(_rt_file_t* file){ return cnull; }
int stdo_read(_rt_file_t* file, void* buf, int size){
	RT_DECLARE_POSIX_FILE(rf, file)
	return fwrite(buf, size, 1, rf->fd);
}

static __inline int rt_stdout_init(rt_posix_file_t* file){
	file->type = RT_FILE_TYPE_STDOUT;
	file->fd = stdout;
	file->ifile.is_eof = stdo_is_eof;
	file->ifile.get_line = stdo_get_line;
	file->ifile.flush = stdo_flush;
	file->ifile.vprint = stdo_print;
	file->ifile.get_char = stdo_get_char;
	file->ifile.unget_char = stdo_unget_char;
	file->ifile.write = stdo_write;
	file->ifile.read = stdo_read;
	file->ifile.final = stdo_final;
	file->ifile.seek = stdo_seek;
	file->ifile.size = stdo_size;
	file->ifile.get_ptr = stdo_get_ptr;
	return 0;
}

rt_file_t rt_stdout_new(void){
	rt_posix_file_t* file = rt_new(sizeof(rt_posix_file_t));
	if(!file) return cnull;
	if(rt_stdout_init(file) < 0){
		rt_delete(file);
		file = cnull;
	}
	return (rt_file_t)file;
}
