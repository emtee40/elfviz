#ifndef __RT_FILE_INTERFACE_H__
#define __RT_FILE_INTERFACE_H__

typedef struct __rt_file_t{
	int (*is_eof)(struct __rt_file_t* file);
	char* (*get_line)(struct __rt_file_t* file, char* buf, int size);
	int (*flush)(struct __rt_file_t* file);
	void (*vprint)(struct __rt_file_t* file, char* format, rt_list list); 
	char (*get_char)(struct __rt_file_t* file);
	void (*unget_char)(struct __rt_file_t* file);
	int (*write)(struct __rt_file_t* file, void* buf, int size);
	int (*read)(struct __rt_file_t* file, void* buf, int size);
	void (*final)(struct __rt_file_t* file);
	cbool (*seek)(struct __rt_file_t* file, int offset, int origin);
	int (*size)(struct __rt_file_t* file);
	void* (*get_ptr)(struct __rt_file_t* file);
}_rt_file_t;

#endif //__RT_FILE_INTERFACE_H__
