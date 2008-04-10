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

#ifndef __UCRT_FILE_H__
#define __UCRT_FILE_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef char *rt_list;

#define rt_intsizeof(n)    ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))

#define rt_start(ap, v)  (ap = (rt_list) &v + rt_intsizeof(v))
#define rt_arg(ap, t)    (*(t *)((ap += rt_intsizeof(t)) - rt_intsizeof(t)))
#define rt_end(ap)       (ap = (rt_list) 0)

#define rt_eof -1

enum rt_file_type{
	RT_FILE_TYPE_GENERAL = 0,
	RT_FILE_TYPE_STDIN,
	RT_FILE_TYPE_STDOUT,
	RT_FILE_TYPE_STDERR,
	RT_FILE_TYPE_MEMORY,
	RT_FILE_TYPE_MAX
};
enum rt_file_mode{
	RT_FILE_OPEN_RDONLY = 0,
	RT_FILE_OPEN_WRTRUNC,
	RT_FILE_OPEN_APPEND,
	RT_FILE_OPEN_MAX
};
enum rt_file_seek_mode{
	RT_FILE_SEEK_CUR = 0,
	RT_FILE_SEEK_SET,
	RT_FILE_SEEK_END
};

typedef unsigned int rt_file_t;

RT_API rt_file_t rt_stdin_new(void);
RT_API rt_file_t rt_stdout_new(void);
RT_API rt_file_t rt_stderr_new(void);
RT_API rt_file_t rt_file_new(char* url, int mode);
RT_API rt_file_t rt_mem_file_new(char* url, int mode);
RT_API void rt_file_delete(rt_file_t file);

RT_API int rt_fis_eof(rt_file_t file);
RT_API char* rt_fget_line(rt_file_t file, char* buf, int size);
RT_API int rt_fflush(rt_file_t file);
RT_API void rt_fprint(rt_file_t file, char* format, ...); 
RT_API char rt_fget_char(rt_file_t file);
RT_API void rt_funget_char(rt_file_t file);
RT_API int rt_fwrite(rt_file_t file, void* buf, int size);
RT_API int rt_fread(rt_file_t file, void* buf, int size);
RT_API void rt_ffinal(rt_file_t file);
RT_API cbool rt_fseek(rt_file_t file, int offset, int origin);
RT_API int rt_fsize(rt_file_t file);
RT_API void* rt_fget_ptr(rt_file_t file);
#ifdef __cplusplus
}
#endif

#endif //__UCRT_FILE_H__
