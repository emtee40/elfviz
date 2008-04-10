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

#ifndef __UCRT_STD_H__
#define __UCRT_STD_H__

#ifdef __cplusplus
extern "C"{
#endif

#define RT_IS_DIGIT(a)	((((a) >= '0') && ((a) <= '9')) ? ctrue : cfalse)
#define RT_IS_LOW_ALPHA(a)	((((a) >= 'a') && ((a) <= 'z')) ? ctrue : cfalse)
#define RT_IS_UP_ALPHA(a)	((((a) >= 'A') && ((a) <= 'Z')) ? ctrue : cfalse)
#define RT_IS_ALPHA(a)	((RT_IS_LOW_ALPHA(a) || RT_IS_UP_ALPHA(a)) ? ctrue : cfalse)
#define RT_IS_ALNUM(a)	((RT_IS_DIGIT(a) || RT_IS_LOW_ALPHA(a) || RT_IS_UP_ALPHA(a)) ? ctrue : cfalse)

#define RT_ABS(a) (((a) < 0) ? -a : a)

RT_API void* rt_new(const int size);
RT_API int rt_size(void* mem);
RT_API void* rt_resize(void* mem, const int size);
RT_API void* rt_dup(void* src);
RT_API void* rt_ndup(void* src, const int size);
RT_API void rt_delete(void* mem);

RT_API int rt_strlen(const char* s);
RT_API int rt_strncmp(const char* dst, const char* src, const int len);
RT_API int rt_strcmp(const char* dst, const char* src);
RT_API char* rt_strcpy(char* dst, const char* src);
RT_API char* rt_strncpy(char* dst, const char* src, const int len);
RT_API char* rt_strchr(const char* s, const char c);
RT_API char* rt_strrchr(const char* s, const char c);
RT_API int rt_strcspn(const char* s, const char* c);
RT_API char* rt_strcat(char* s, const char* a);
RT_API char* rt_strdup(char* s);
RT_API char * rt_strtok(char *s1, const char *s2, char **lasts);
RT_API void* rt_memcpy(void* dst, const void* src, const int size);
RT_API void rt_memset(void* mem, const int i, const int size);
RT_API int rt_atoi(const char* s);
RT_API void rt_exit(int err);

#ifdef __cplusplus
}
#endif

#endif //__UCRT_STD_H__

