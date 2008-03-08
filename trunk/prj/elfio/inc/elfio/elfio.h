/* -------------------------------------------------------------------------
 *
 * elfviz, tool for visualization of elf file
 * ------------------------------------------
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

#ifndef __ELF_IO_H__
#define __ELF_IO_H__

typedef struct _elf_section_t{
	void (*format)(rt_file_t* rtout, struct _elf_section_t* section);
	void (*dump)(rt_file_t* rtout, struct _elf_section_t* section);
	struct _elf_section_t* (*get_sub)(struct _elf_section_t* section, const int idx);
	struct _elf_section_t* (*find_sub)(struct _elf_section_t* section, const char* stridx);
	const char* (*get_name)(struct _elf_section_t* section);
	const cbyte* (*data)(struct _elf_section_t* section);
}elf_section_t;

typedef struct _elfio_t{
	const char* (*file_name)(struct _elfio_t* elfio);
	void (*format)(rt_file_t* rtout, struct _elfio_t* elfio);
	elf_section_t* (*get_shdr)(struct _elfio_t* elfio);
	elf_section_t* (*get_phdr)(struct _elfio_t* elfio);
}elfio_t;

elfio_t* elfio_new(char* file);
void elfio_delete(elfio_t* elfio);
#endif //__ELF_IO_H__
