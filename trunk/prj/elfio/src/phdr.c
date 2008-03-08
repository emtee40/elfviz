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
#include "phdr.h"
#include "phdr_entry.h"

typedef struct _elf_phdr_t{
	elf_section_t isection;
	elf_section_t* entry;
	int n_entry;
	int n_entsize;
}elf_phdr_t;

void phdr_format(rt_file_t* fd, elf_section_t* section){
	fd->print(fd, "no header\n");
}
void phdr_dump(rt_file_t* fd, elf_section_t* section){
	elf_phdr_t* phdr = (elf_phdr_t*)section;
	elf_section_t* entry = cnull;
	int i;
	for(i = 0 ; i < phdr->n_entry ; i++){
		entry = section->get_sub(section, i);
		if(entry) entry->format(fd, entry);
	}
}

elf_section_t* phdr_sub(elf_section_t* section, const int idx){
	elf_phdr_t* phdr = (elf_phdr_t*)section;
	return (idx >= phdr->n_entry) ? cnull : (elf_section_t*)((cbyte*)(phdr->entry) + idx * phdr->n_entsize);
}

elf_section_t* phdr_new(Elf32_Ehdr ehdr, rt_file_t* fd){
	elf_phdr_t* phdr = rt_new(sizeof(elf_phdr_t));
	phdr->n_entry = ehdr.e_phnum;
	phdr->entry = phdr_entry_new(fd, ehdr.e_phoff, phdr->n_entry);
	phdr->n_entsize = phdr_entry_size();
	phdr->isection.format = phdr_format;
	phdr->isection.dump = phdr_dump;
	phdr->isection.get_sub = phdr_sub;
	return (elf_section_t*)phdr;
}

void phdr_delete(elf_section_t* section){
	elf_phdr_t* phdr = (elf_phdr_t*)section;
	if(phdr->entry) phdr_entry_delete(phdr->entry);
	rt_delete(phdr);
}
