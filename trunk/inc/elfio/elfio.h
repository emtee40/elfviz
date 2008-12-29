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

#ifdef WIN32
#ifdef LIBELFIO_EXPORTS
#define ELFIO_API __declspec(dllexport)
#else //LIBELFIO_EXPORTS
#define ELFIO_API __declspec(dllimport)
#endif //LIBELFIO_EXPORTS
#else //WIN32
#define ELFIO_API
#endif //WIN32
class elf_section_t{
	public:
		virtual void format_header(void) = 0;
		virtual void format_body(void) = 0;
		virtual void format_child(void) = 0;
		virtual elf_section_t* get_child(const unsigned int idx) = 0;
		virtual elf_section_t* get_child(const char* stridx) = 0;
		virtual const char* name(void) = 0;
		virtual const unsigned char* get_body(void) = 0;
};

ELFIO_API elf_section_t* elfio_new(char* file);

#endif //__ELF_IO_H__
