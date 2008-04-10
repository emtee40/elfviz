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


static int file_is_eof(_rt_file_t* file){
	RT_DECLARE_POSIX_FILE(rf, file)
	return feof(rf->fd);
}

static char* file_get_line(_rt_file_t* file, char* buf, int len){
	RT_DECLARE_POSIX_FILE(rf, file)
	return fgets(buf, len, rf->fd);
}

static int file_flush(_rt_file_t* file){
	RT_DECLARE_POSIX_FILE(rf, file)
	return fflush(rf->fd);
}

static char file_get_char(_rt_file_t* file){
	RT_DECLARE_POSIX_FILE(rf, file)
	return getc(rf->fd);
}

static void file_unget_char(_rt_file_t* file){
	RT_DECLARE_POSIX_FILE(rf, file)
	fseek(rf->fd, -1, SEEK_CUR);
}

static void file_final(_rt_file_t* file){
	RT_DECLARE_POSIX_FILE(rf, file)
	fclose(rf->fd);
}

static void file_print(_rt_file_t* file, char* format, rt_list list){
	RT_DECLARE_POSIX_FILE(rf, file)
	vfprintf(rf->fd, format, list);
}

int file_write(_rt_file_t* file, void* buf, int size){
	RT_DECLARE_POSIX_FILE(rf, file)
	return fwrite(buf, size, 1, rf->fd);
}
int file_read(_rt_file_t* file, void* buf, int size){
	RT_DECLARE_POSIX_FILE(rf, file)
	return fread(buf, size, 1, rf->fd);
}
cbool file_seek(_rt_file_t* file, int offset, int origin){
	RT_DECLARE_POSIX_FILE(rf, file)
	int org;
	switch(origin){
	case RT_FILE_SEEK_SET:	org = SEEK_SET;	break;
	case RT_FILE_SEEK_END:	org = SEEK_END;	break;
	case RT_FILE_SEEK_CUR:	org = SEEK_CUR;	break;
	}
	return (!fseek(rf->fd, offset, org)) ? ctrue : cfalse;
}

int file_size(_rt_file_t* file){
	RT_DECLARE_POSIX_FILE(rf, file)
	int org = ftell(rf->fd);
	int size;
	fseek(rf->fd, 0, SEEK_END);
	size = ftell(rf->fd);
	fseek(rf->fd, org, SEEK_SET);
	return size;
}

void* file_get_ptr(_rt_file_t* file){
	return cnull;
}

static __inline int rt_file_init(rt_posix_file_t* file, char* url, int mode){
	char fmode[8];
	switch(mode){
		case RT_FILE_OPEN_RDONLY:
			strcpy(fmode, "rb");
			break;
		case RT_FILE_OPEN_WRTRUNC:
			strcpy(fmode, "wb");
			break;
		case RT_FILE_OPEN_APPEND:
			strcpy(fmode, "ab");
			break;
		default:
			fmode[0] = 0;
	}
	file->type = RT_FILE_TYPE_GENERAL;
	file->fd = fopen(url, fmode);
	if(!file->fd)	return -1;
	file->ifile.is_eof = file_is_eof;
	file->ifile.get_line = file_get_line;
	file->ifile.flush = file_flush;
	file->ifile.vprint = file_print;
	file->ifile.get_char = file_get_char;
	file->ifile.unget_char = file_unget_char;
	file->ifile.write = file_write;
	file->ifile.read = file_read;
	file->ifile.final = file_final;
	file->ifile.seek = file_seek;
	file->ifile.size = file_size;
	file->ifile.get_ptr = file_get_ptr;
	return 0;
}

rt_file_t rt_file_new(char* url, int mode){
	rt_posix_file_t* file = rt_new(sizeof(rt_posix_file_t));
	if(!file) return cnull;
	if(!url || !rt_strlen(url)) {
			rt_delete(file);
			file = cnull;
			return (rt_file_t)file;
	}
	if(rt_file_init(file, url, mode) < 0){
			rt_delete(file);
			file = cnull;
	}
	return (rt_file_t)file;
}

void rt_file_delete(rt_file_t file){
	_rt_file_t* fd = (_rt_file_t*)file;
	fd->final(fd);
	rt_delete(fd);
}
