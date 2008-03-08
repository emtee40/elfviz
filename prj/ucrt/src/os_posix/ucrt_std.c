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
#include <stdlib.h>

#define RT_GET_SIZE(mem, size)	{			\
	cbyte* r = RT_GET_REAL_POINTER(mem);	\
	memcpy(&size, r, sizeof(int));			\
}
#define RT_GET_REAL_POINTER(mem) ((void*)((cbyte*)mem - sizeof(int)))
#define RT_GET_RT_POINTER(mem)	((void*)((cbyte*)mem + sizeof(int)))

cvoid* rt_new(const int size){
	cbyte* mem = cnull;
	if(size <= 0) return cnull;
	mem = malloc(size + sizeof(size));
	if(mem) {
		memset(mem, 0, size + sizeof(size));
		memcpy(mem, &size, sizeof(size));
	}
	return (mem) ? RT_GET_RT_POINTER(mem) : cnull;
}

int rt_size(cvoid* mem){
	int size = 0;
	if(mem) RT_GET_SIZE(mem, size);
	return size;
}

cvoid* rt_resize(cvoid* mem, const int size){
	int min = 0, mem_size = 0;
	cbyte* new_mem = rt_new(size);
	if(!new_mem) return cnull;
	if(mem) RT_GET_SIZE(mem, mem_size);
	min = (mem_size > size) ? size : mem_size;
	if(min > 0 && mem) memcpy(new_mem, mem, min);
	if(mem) rt_delete(mem);
	return new_mem;
}

cvoid* rt_dup(cvoid* mem){
	int size = 0;
	void* dst = cnull;
	RT_GET_SIZE(mem, size);
	dst = rt_new(size);
	memcpy(dst, mem, size);
	return dst;
}

cvoid* rt_ndup(void* mem, const int size){
	void* dst = rt_new(size);
	memcpy(dst, mem, size);
	return dst;
}

void rt_delete(cvoid* mem){
	if(mem) free(RT_GET_REAL_POINTER(mem));
}

int rt_strlen(const char* s){
	return strlen(s);
}

int rt_strncmp(const char* dst, const char* src, const int len){
	return strncmp(dst, src, len);
}

int rt_strcmp(const char* dst, const char* src){
	return strcmp(dst, src);
}

char* rt_strcpy(char* dst, const char* src){
	return strcpy(dst, src);
}

char* rt_strncpy(char* dst, const char* src, const int len){
	return strncpy(dst, src, len);
}

char* rt_strchr(const char* s, const char c){
	return strchr(s, c);
}

char* rt_strrchr(const char* s, const char c){
	return strrchr(s, c);
}

int rt_strcspn(const char* s, const char* c){
	return strcspn(s, c);
}

char* rt_strcat(char* s, const char* a){
	return strcat(s, a);
}

char* rt_strdup(char* s){
	char* ret = rt_new(strlen(s) + 1);
	if(ret) strcpy(ret, s);
	return ret;
}

int rt_atoi(const char* s){
	return atoi(s);
}

void rt_exit(int err){
	exit(err);
}

char * rt_strtok(char *s1, const char *s2, char **lasts){
	char *ret;

	if (s1 == NULL)	s1 = *lasts;
	while(*s1 && strchr(s2, *s1))	++s1;
	if(*s1 == '\0')	return NULL;
	ret = s1;
	while(*s1 && !strchr(s2, *s1))	++s1;
	if(*s1)	*s1++ = '\0';
	*lasts = s1;
	return ret;
}

void* rt_memcpy(void* dst, const void* src, const int size){
	return memcpy(dst, src, size);
}

