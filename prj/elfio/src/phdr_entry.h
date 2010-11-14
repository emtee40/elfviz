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

#ifndef __ELF_PHDR_ENTRY_H__
#define __ELF_PHDR_ENTRY_H__
class phdrEntryFactory{
	private:
		Elf32_Phdr* tags;
		int n_entry;
	public:
		phdrEntryFactory(FILE* fd, int phnum, int phoff);
		~phdrEntryFactory();
		elfSection* entry_new(int index);
		int entries(void);
};
#endif //__ELF_PHDR_ENTRY_H__
