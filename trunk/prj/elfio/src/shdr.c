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
#include "shdr.h"
#include "shdr_entry.h"

typedef struct _elf_shdr_t{
	elf_section_t isection;
	elf_section_t* entry;
	int n_entry;
	int n_entsize;
}elf_shdr_t;

void shdr_format(rt_file_t* fd, elf_section_t* section){
	fd->print(fd, "no header\n");
}

void shdr_dump(rt_file_t* fd, elf_section_t* section){
	elf_shdr_t* shdr = (elf_shdr_t*)section;
	elf_section_t* entry = cnull;
	int i;
	for(i = 0 ; i < shdr->n_entry ; i++){
		entry = section->get_sub(section, i);
		fd->print(fd, "shdr[%d]\n", i);
		if(entry) entry->format(fd, entry);
	}
}

elf_section_t* shdr_sub(elf_section_t* section, const int idx){
	elf_shdr_t* shdr = (elf_shdr_t*)section;
	return (idx >= shdr->n_entry) ? cnull : (elf_section_t*)((cbyte*)(shdr->entry) + idx * shdr->n_entsize);
}

elf_section_t* shdr_find_sub(elf_section_t* section, const char* stridx){
	elf_shdr_t* shdr = (elf_shdr_t*)section;
	elf_section_t* entry = cnull;
	int i;
	for(i = 0 ; i < shdr->n_entry ; i++){
		entry = section->get_sub(section, i);
		if(!rt_strcmp(entry->get_name(entry), stridx)) return entry;
	}
	return cnull;
}

const char* shdr_name(elf_section_t* section){
	return cnull;
}

elf_section_t* shdr_new(Elf32_Ehdr ehdr, rt_file_t* fd){
	elf_shdr_t* shdr = rt_new(sizeof(elf_shdr_t));
	shdr->n_entry = ehdr.e_shnum;
	shdr->entry = shdr_entry_new(fd, ehdr.e_shoff, shdr->n_entry, ehdr.e_shstrndx);
	shdr->n_entsize = shdr_entry_size();
	shdr->isection.format = shdr_format;
	shdr->isection.dump = shdr_dump;
	shdr->isection.get_sub = shdr_sub;
	shdr->isection.find_sub = shdr_find_sub;
	shdr->isection.get_name = shdr_name;
	return (elf_section_t*)shdr;
}

void shdr_delete(elf_section_t* section){
	elf_shdr_t* shdr = (elf_shdr_t*)section;
	if(shdr->entry) shdr_entry_delete(shdr->entry);
	rt_delete(shdr);
}
