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

typedef class _elf_section_t{
	public:
		virtual void format(rt_file_t rtout) = 0;
		virtual void dump(rt_file_t rtout) = 0;
		virtual class _elf_section_t* get_sub(const int idx) = 0;
		virtual class _elf_section_t* find_sub(const char* stridx) = 0;
		virtual const char* name(void) = 0;
		virtual const cbyte* data(void) = 0;
}elf_section_t;

typedef class _elfio_t{
	public:
		virtual const char* file_name(void) = 0;
		virtual void format(rt_file_t rtout) = 0;
		virtual elf_section_t* get_shdr(void) = 0;
		virtual elf_section_t* get_phdr(void) = 0;
}elfio_t;

elfio_t* elfio_new(char* file);

#endif //__ELF_IO_H__
