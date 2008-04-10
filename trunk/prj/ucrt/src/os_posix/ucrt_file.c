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
#include "rt_file_interface.h"

int rt_fis_eof(rt_file_t file){
	_rt_file_t* fd = (_rt_file_t*)file;
	return fd->is_eof(fd);
}

char* rt_fget_line(rt_file_t file, char* buf, int size){
	_rt_file_t* fd = (_rt_file_t*)file;
	return fd->get_line(fd, buf, size);
}

int rt_fflush(rt_file_t file){
	_rt_file_t* fd = (_rt_file_t*)file;
	return fd->flush(fd);
}

void rt_fprint(rt_file_t file, char* format, ...){
	_rt_file_t* fd = (_rt_file_t*)file;
	rt_list list;
	rt_start (list, format);
	fd->vprint(fd, format, list);
	rt_end(list);
}
 
char rt_fget_char(rt_file_t file){
	_rt_file_t* fd = (_rt_file_t*)file;
	return fd->get_char(fd);
}

void rt_funget_char(rt_file_t file){
	_rt_file_t* fd = (_rt_file_t*)file;
	fd->unget_char(fd);
}

int rt_fwrite(rt_file_t file, void* buf, int size){
	_rt_file_t* fd = (_rt_file_t*)file;
	return fd->write(fd, buf, size);
}

int rt_fread(rt_file_t file, void* buf, int size){
	_rt_file_t* fd = (_rt_file_t*)file;
	return fd->read(fd, buf, size);
}

void rt_ffinal(rt_file_t file){
	_rt_file_t* fd = (_rt_file_t*)file;
	fd->final(fd);
}

cbool rt_fseek(rt_file_t file, int offset, int origin){
	_rt_file_t* fd = (_rt_file_t*)file;
	return fd->seek(fd, offset, origin);
}

int rt_fsize(rt_file_t file){
	_rt_file_t* fd = (_rt_file_t*)file;
	return fd->size(fd);
}

void* rt_fget_ptr(rt_file_t file){
	_rt_file_t* fd = (_rt_file_t*)file;
	return fd->get_ptr(fd);
}
