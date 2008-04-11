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

#ifndef __ELF_SHDR_REL_ENTRY_H__
#define __ELF_SHDR_REL_ENTRY_H__
elf_section_t* shdr_rel_entry_new(rt_file_t fd, unsigned int sh_offset, char* strtab);
elf_section_t* shdr_rela_entry_new(rt_file_t fd, unsigned int sh_offset, char* strtab);
#endif //__ELF_SHDR_REL_ENTRY_H__
