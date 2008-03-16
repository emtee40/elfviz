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
#include "shdr_symtab_entry.h"

typedef struct _elf_shdr_symtab_entry_t{
	elf_section_t isection;
	Elf32_Sym symtab;
	rt_file_t* fd;
	char* name;
	unsigned int t_entry;
}elf_shdr_symtab_entry_t;

void shdr_entry_symtab_format(rt_file_t* fd, elf_section_t* section){
	unsigned int sti = 0;
	char* str = cnull;
	elf_shdr_symtab_entry_t* sse = (elf_shdr_symtab_entry_t*)section;
	if(sse->name) fd->print(fd, "st_name_str=%s\n", sse->name);
	fd->print(fd, "st_name=%d\n", sse->symtab.st_name);
	fd->print(fd, "st_vale=%d\n", sse->symtab.st_value);
	fd->print(fd, "st_size=%d\n", sse->symtab.st_size);
	fd->print(fd, "st_info=0x%x\n", sse->symtab.st_info);
	sti = sse->symtab.st_info;
	sti = ELF32_ST_BIND(sti);
	fd->print(fd, "st_bind=");
	switch(sti){
		case STB_LOCAL:		str = "STB_LOCAL";	break;
		case STB_GLOBAL:	str = "STB_GLOBAL";	break;
		case STB_WEAK:		str = "STB_WEAK";	break;
		case STB_LOPROC:	str = "STB_LOPROC";	break;
		case STB_HIPROC:	str = "STB_HIPROC";	break;
	}
	fd->print(fd, "%s\n", str);
	sti = sse->symtab.st_info;
	sti = ELF32_ST_TYPE(sti);
	fd->print(fd, "st_type=");
	switch(sti){
		case STT_NOTYPE:	str = "STT_NOTYPE";		break;
		case STT_OBJECT:	str = "STT_OBJECT";		break;
		case STT_FUNC:		str = "STT_FUNC";		break;
		case STT_SECTION:	str = "STT_SECTION";	break;
		case STT_FILE:		str = "STT_FILE";		break;
		case STT_LOPROC:	str = "STT_LOPROC";		break;
		case STT_HIPROC:	str = "STT_HIPROC";		break;
	}
	fd->print(fd, "%s\n", str);
	fd->print(fd, "st_other=0x%x\n", sse->symtab.st_other);
	fd->print(fd, "st_shndx=%d\n", sse->symtab.st_shndx);
	fd->print(fd, "\n");
}

void shdr_entry_symtab_dump(rt_file_t* fd, elf_section_t* section){
	fd->print(fd, "no data\n");
}

cbyte* shdr_entry_symtab_data(elf_section_t* section){
	return cnull;
}

elf_section_t* shdr_symtab_entry_new(rt_file_t* fd, unsigned int sh_offset, unsigned int sh_size, unsigned int sh_entsize, cbyte* strtab){
	int num = sh_size / sh_entsize;
	elf_shdr_symtab_entry_t* sse = rt_new(sizeof(elf_shdr_symtab_entry_t) * num);
	fd->seek(fd, sh_offset, RT_FILE_SEEK_SET);
	{
		int i;
		for(i = 0 ; i < num ; i++){
			sse[i].fd = fd;
			fd->read(fd, &sse[i].symtab, sizeof(Elf32_Sym));
			sse[i].name = sse[i].symtab.st_name + strtab;
			sse[i].isection.data = shdr_entry_symtab_data;
			sse[i].isection.dump = shdr_entry_symtab_dump;
			sse[i].isection.format = shdr_entry_symtab_format;
		}
	}
	return (elf_section_t*) sse;
}

void shdr_symtab_entry_delete(elf_section_t* section){
	rt_delete(section);
}

int shdr_symtab_entry_size(void){
	return sizeof(elf_shdr_symtab_entry_t);
}