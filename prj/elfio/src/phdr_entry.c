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

#include "ucrt/ucrt.h"
#include "elfio/elfio.h"
#include "elftypes.h"
#include "phdr_entry.h"

typedef struct _elf_phdr_entry_t{
	elf_section_t isection;
	Elf32_Phdr phdr;
}elf_phdr_entry_t;

void phdr_entry_type_str(rt_file_t* fd, int p_type){
	char* str = "unknown";
	fd->print(fd, "p_type=");
	switch(p_type){
	case PT_NULL:		str = "PT_NULL";	break;
	case PT_LOAD:		str = "PT_LOAD";	break;
	case PT_DYNAMIC:	str = "PT_DYNAMIC";	break;
	case PT_INTERP:		str = "PT_INTERP";	break;
	case PT_NOTE:		str = "PT_NOTE";	break;
	case PT_SHLIB:		str = "PT_SHLIB";	break;
	case PT_PHDR:		str = "PT_PHDR";	break;
	case PT_LOPROC:		str = "PT_LOPROC";	break;
	case PT_HIPROC:		str = "PT_HIPROC";	break;
	}
	fd->print(fd, "%s\n", str);
}

void phdr_entry_offset_str(rt_file_t* fd, int p_offset){
	fd->print(fd, "p_offset=0x%x\n", p_offset);
}

void phdr_entry_vaddr_str(rt_file_t* fd, int p_vaddr){
	fd->print(fd, "p_vaddr=0x%x\n", p_vaddr);
}

void phdr_entry_paddr_str(rt_file_t* fd, int p_paddr){
	fd->print(fd, "p_paddr=0x%x\n", p_paddr);
}

void phdr_entry_filesz_str(rt_file_t* fd, int p_filesz){
	fd->print(fd, "p_filesz=%d\n", p_filesz);
}

void phdr_entry_memsz_str(rt_file_t* fd, int p_memsz){
	fd->print(fd, "p_memsz=0x%x\n", p_memsz);
}

void phdr_entry_flags_str(rt_file_t* fd, int p_flags){
	fd->print(fd, "p_flags=");
	if(p_flags & PF_X)	fd->print(fd, "PF_X | ");
	if(p_flags & PF_W)	fd->print(fd, "PF_W | ");
	if(p_flags & PF_R)	fd->print(fd, "PF_R");
	fd->print(fd, "\n");
}

void phdr_entry_align_str(rt_file_t* fd, int p_align){
	fd->print(fd, "p_align=%d\n", p_align);
}

void phdr_entry_dump(rt_file_t* fd, elf_section_t* section){
	fd->print(fd, "no date\n");
}

void phdr_entry_format(rt_file_t* fd, elf_section_t* section){
	elf_phdr_entry_t* phdr = (elf_phdr_entry_t*)section;
	phdr_entry_type_str(fd, phdr->phdr.p_type);
	phdr_entry_offset_str(fd, phdr->phdr.p_offset);
	phdr_entry_vaddr_str(fd, phdr->phdr.p_vaddr);
	phdr_entry_paddr_str(fd, phdr->phdr.p_paddr);
	phdr_entry_filesz_str(fd, phdr->phdr.p_filesz);
	phdr_entry_memsz_str(fd, phdr->phdr.p_memsz);
	phdr_entry_flags_str(fd, phdr->phdr.p_flags);
	phdr_entry_align_str(fd, phdr->phdr.p_align);
	fd->print(fd, "\n");
}

elf_section_t* phdr_entry_sub(elf_section_t* section, const int idx){
	elf_phdr_entry_t* phdr = (elf_phdr_entry_t*)section;
	return cnull;
}

elf_section_t* phdr_entry_new(rt_file_t* fd, int e_phoff, int n_entry){
	elf_phdr_entry_t* phdr_entry = rt_new(sizeof(elf_phdr_entry_t) * n_entry);
	fd->seek(fd, e_phoff, RT_FILE_SEEK_SET);
	{
		int i;
		for(i = 0 ; i < n_entry ; i++) {
			fd->read(fd, &(phdr_entry[i].phdr), sizeof(Elf32_Phdr));
			phdr_entry[i].isection.format = phdr_entry_format;
			phdr_entry[i].isection.get_sub = phdr_entry_sub;
		}
	}
	return (elf_section_t*) phdr_entry;
}

void phdr_entry_delete(elf_section_t* section){
	elf_phdr_entry_t* phdr_entry = (elf_phdr_entry_t*)section;
	rt_delete(phdr_entry);
}

int phdr_entry_size(void){
	return sizeof(elf_phdr_entry_t);
}
