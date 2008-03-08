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

#define MF_GROW_RATE	32

typedef struct _rt_mem_file_t{
	rt_file_t ifile;
	int type;
	cbyte* buffer;
	int grow;
	int pos;
	char* file_name;
	int mode;
}rt_mem_file_t;

static int mf_is_eof(rt_file_t* file){
	rt_mem_file_t * state = (rt_mem_file_t*) file;
	return (state->pos == rt_size(state->buffer)) ? ctrue : cfalse;
}
static char* mf_get_line(rt_file_t* file, char* buf, int len){
	char ch = 0;
	int pos = 0;
	rt_mem_file_t * state = (rt_mem_file_t*) file;
	while(ch = state->buffer[state->pos], ch == '\n' || ch == '\r' || !ch)	state->pos++;
	while(ch = state->buffer[state->pos], !(ch == '\n' || ch == '\r' || !ch)) {
		buf[pos++] = ch;
		state->pos++;
	}
	buf[pos] = 0;
	return buf;
}

static int mf_flush(rt_file_t* file){
	int flush_size = 0;
	rt_mem_file_t * state = (rt_mem_file_t*) file;
	if(state->file_name && strlen(state->file_name)){
		FILE* fd = fopen(state->file_name, "wb");
		fwrite(state->buffer, state->pos, 1, fd);
		fclose(fd);
		flush_size = state->pos;
	} 
	return flush_size;
}

static char mf_get_char(rt_file_t* file){
	rt_mem_file_t * state = (rt_mem_file_t*) file;
	return (state->pos < rt_size(state->buffer)) ? state->buffer[state->pos++] : rt_eof;
}
static void mf_unget_char(rt_file_t* file){
	rt_mem_file_t * state = (rt_mem_file_t*) file;
	state->pos--;
}
static void mf_final(rt_file_t* file){
	rt_mem_file_t * state = (rt_mem_file_t*) file;
	file->flush(file);
	rt_delete(state->buffer);
}
static cbool mf_resize(rt_mem_file_t* state, int size){
	if(size > rt_size(state->buffer) - state->pos) {
		int len = ((size + state->pos) / MF_GROW_RATE + 1) * MF_GROW_RATE;
		state->buffer = rt_resize(state->buffer, len);
	}
	return (state->buffer) ? ctrue : cfalse;
}

static void mf_print(struct _rt_file_t* file, char* format, ...){
	char str[1024];
	rt_list list;

	memset(str, 0, 1024);
	rt_start (list, format);
	vsprintf(str, format, list);
	rt_end(list);
	{
		rt_mem_file_t * state = (rt_mem_file_t*) file;
		mf_resize(state, rt_strlen(str));
		strcat(state->buffer, str);
		state->pos += strlen(str);
	}
}
int mf_write(struct _rt_file_t* file, void* buf, int size){
	rt_mem_file_t * state = (rt_mem_file_t*) file;
	if(state->mode == RT_FILE_OPEN_RDONLY) return -1;

	mf_resize(state, size);
	memcpy(state->buffer + state->pos, buf, size);
	state->pos += size;
	return size;
}
cbool mf_seek(struct _rt_file_t* file, int offset, int origin){
	rt_mem_file_t * state = (rt_mem_file_t*) file;
	int end = rt_size(state->buffer);
	int org = 0;
	switch(origin){
	case RT_FILE_SEEK_SET:	org = 0;			break;
	case RT_FILE_SEEK_END:	org = end;			break;
	case RT_FILE_SEEK_CUR:	org = state->pos;	break;
	}
	if(end < org + offset)	mf_resize(state, state->pos + offset);
	if(state->buffer) state->pos = org + offset;
	return (state->buffer) ? ctrue : cfalse;
}
int mf_size(struct _rt_file_t* file){
	rt_mem_file_t * state = (rt_mem_file_t*) file;
	return rt_size(state->buffer);
}
void* mf_get_ptr(struct _rt_file_t* file){
	rt_mem_file_t * state = (rt_mem_file_t*) file;
	return state->buffer;
}
int mf_read(struct _rt_file_t* file, void* buf, int size){
	rt_mem_file_t * state = (rt_mem_file_t*) file;
	memcpy(buf, state->buffer + state->pos, size);
	return size;
}

static __inline cbool mf_state_new(rt_mem_file_t* state, char* url, int mode){
	state->mode = mode;
	state->grow = MF_GROW_RATE;
	if(url){
		FILE* fd = fopen(url, "rb");
		if(fd) {
			int len = 0;
			fseek(fd, 0, SEEK_END);
			len = ftell(fd);
			fseek(fd, 0, SEEK_SET);
			len -= ftell(fd);
			if(len > 0){
				state->buffer = rt_new(len);
				fread(state->buffer, len, 1, fd);
				state->file_name = rt_strdup(url);
			} else {
				len = 0;
			}
			fclose(fd);
		}
	}
	return ctrue;
}

static __inline int rt_mem_file_init(rt_mem_file_t* file, char* url, int mode){
	file->type = RT_FILE_TYPE_MEMORY;
	if(!mf_state_new(file, url, mode)) return -1;
	file->ifile.is_eof = mf_is_eof;
	file->ifile.get_line = mf_get_line;
	file->ifile.flush = mf_flush;
	file->ifile.print = mf_print;
	file->ifile.get_char = mf_get_char;
	file->ifile.unget_char = mf_unget_char;
	file->ifile.write = mf_write;
	file->ifile.read = mf_read;
	file->ifile.final = mf_final;
	file->ifile.seek = mf_seek;
	file->ifile.size = mf_size;
	file->ifile.get_ptr = mf_get_ptr;
	return 0;
}

rt_file_t* rt_mem_file_new(char* url, int mode){
	rt_mem_file_t* file = rt_new(sizeof(rt_mem_file_t));
	if(!file) return cnull;
	memset(file, 0, sizeof(rt_mem_file_t));
	if(rt_mem_file_init(file, url, mode) < 0){
		rt_delete(file);
		file = cnull;
	}
	return (rt_file_t*)file;
}
