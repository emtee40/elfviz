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
#include <stdlib.h>
#include "rt_file_interface.h"
#include "ucrt_posix_file.h"

static int stdi_is_eof(_rt_file_t* file){	return cfalse;}
static char* stdi_get_line(_rt_file_t* file, char* buf, int len){	return gets(buf);}
static int stdi_flush(_rt_file_t* file){
	RT_DECLARE_POSIX_FILE(rf, file)
	return fflush(rf->fd);
}

static char stdi_get_char(_rt_file_t* file){
	RT_DECLARE_POSIX_FILE(rf, file)
	return getc(rf->fd);
}

static void stdi_unget_char(_rt_file_t* file){return;}
static void stdi_final(_rt_file_t* file){	return;}
static void stdi_print(_rt_file_t* file, char* format, rt_list list){return;}
int stdi_write(_rt_file_t* file, void* buf, int size){
	RT_DECLARE_POSIX_FILE(rf, file)
	return fwrite(buf, size, 1, rf->fd);
}

cbool stdi_seek(_rt_file_t* file, int offset, int origin){ return cfalse; }
int stdi_size(_rt_file_t* file){ return 0; }
void* stdi_get_ptr(_rt_file_t* file){ return cnull; }
int stdi_read(_rt_file_t* file, void* buf, int size){
	RT_DECLARE_POSIX_FILE(rf, file)
	return fread(buf, size, 1, rf->fd);
}

static __inline int rt_stdin_init(rt_posix_file_t* file){
	file->type = RT_FILE_TYPE_STDIN;
	file->fd = stdin;
	file->ifile.is_eof = stdi_is_eof;
	file->ifile.get_line = stdi_get_line;
	file->ifile.flush = stdi_flush;
	file->ifile.vprint = stdi_print;
	file->ifile.get_char = stdi_get_char;
	file->ifile.unget_char = stdi_unget_char;
	file->ifile.write = stdi_write;
	file->ifile.read = stdi_read;
	file->ifile.final = stdi_final;
	file->ifile.seek = stdi_seek;
	file->ifile.size = stdi_size;
	file->ifile.get_ptr = stdi_get_ptr;
	return 0;
}

rt_file_t rt_stdin_new(void){
	rt_posix_file_t* file = rt_new(sizeof(rt_posix_file_t));
	if(!file) return cnull;
	if(rt_stdin_init(file) < 0){
		rt_delete(file);
		file = cnull;
	}
	return (rt_file_t)file;
}
