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

#define ELF_TYPE_INT	0x1
#define ELF_TYPE_HEX	0x2
#define ELF_TYPE_STR	0x4

class elfAttribute{
	public:
		virtual const unsigned int number(void) = 0;
		virtual const unsigned int type(int idx = -1) = 0;
		virtual const char* name(int idx = -1) = 0;
		virtual const char* stringValue(int idx = -1) = 0;
		virtual const int numericValue(int idx = -1) = 0;
		virtual ~elfAttribute(){};
};

class elfBuffer{
	private:
	public:
		const unsigned int size;
		const unsigned int offset;
		const unsigned char* buffer;

		elfBuffer(const unsigned char* buf, const unsigned sz, const unsigned int off);
		~elfBuffer();
};

class elfSection{
	public:
		virtual elfAttribute* attribute(void) = 0;
		virtual const unsigned int childs(void) = 0;
		virtual elfSection* childAt(const int idx) = 0;
		virtual elfSection* childAt(const char* stridx) = 0;
		virtual const char* name(void) = 0;
		virtual const char* category(void) = 0;
		virtual elfBuffer* body(void) = 0;
		virtual ~elfSection(){};
};

ELFIO_API elfSection* elfio_new(char* file);

#endif //__ELF_IO_H__
