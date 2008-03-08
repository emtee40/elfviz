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
#include "ucrt_posix_file.h"

static int stde_is_eof(rt_file_t* file){	return cfalse;}
static char* stde_get_line(rt_file_t* file, char* buf, int len){	return 0;}
static int stde_flush(rt_file_t* file){
	RT_DECLARE_POSIX_FILE(rf, file)
	return fflush(rf->fd);}
static char stde_get_char(rt_file_t* file){
	RT_DECLARE_POSIX_FILE(rf, file)
	return getc(rf->fd); }
static void stde_unget_char(rt_file_t* file){return;}
static void stde_final(rt_file_t* file){	return;}
static void stde_print(struct _rt_file_t* file, char* format, ...){
	RT_DECLARE_POSIX_FILE(rf, file)
	rt_list list;
	rt_start (list, format);
	vfprintf(rf->fd, format, list);
	rt_end(list);
}
int stde_write(struct _rt_file_t* file, void* buf, int size){
	RT_DECLARE_POSIX_FILE(rf, file)
	return fwrite(buf, size, 1, rf->fd);
}

cbool stde_seek(struct _rt_file_t* file, int offset, int origin){
	return cfalse;
}
int stde_size(struct _rt_file_t* file){
	return 0;
}
void* stde_get_ptr(struct _rt_file_t* file){
	return cnull;
}

static __inline int rt_stderr_init(rt_posix_file_t* file){
	file->type = RT_FILE_TYPE_STDERR;
	file->fd = stderr;
	file->ifile.is_eof = stde_is_eof;
	file->ifile.get_line = stde_get_line;
	file->ifile.flush = stde_flush;
	file->ifile.print = stde_print;
	file->ifile.get_char = stde_get_char;
	file->ifile.unget_char = stde_unget_char;
	file->ifile.final = stde_final;
	file->ifile.write = stde_write;
	file->ifile.seek = stde_seek;
	file->ifile.size = stde_size;
	file->ifile.get_ptr = stde_get_ptr;
	return 0;
}

rt_file_t* rt_stderr_new(void){
	rt_posix_file_t* file = rt_new(sizeof(rt_posix_file_t));
	if(!file) return cnull;
	if(rt_stderr_init(file) < 0){
		rt_delete(file);
		file = cnull;
	}
	return (rt_file_t*)file;
}
