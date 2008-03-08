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
#include "shdr_entry.h"

typedef struct _elf_shdr_entry_t{
	elf_section_t isection;
	Elf32_Shdr shdr;
	rt_file_t* fd;
	char* name;
	cbyte* buf;
	elf_section_t* entry;
}elf_shdr_entry_t;

void shdr_entry_name_str(rt_file_t* fd, int sh_name){
	fd->print(fd, "sh_name=%d\n", sh_name);
}

void shdr_entry_type_str(rt_file_t* fd, int sh_type){
	char* str = "unknown";
	fd->print(fd, "sh_type=");
	switch(sh_type){
	case SHT_NULL:		str = "SHT_NULL";		break;
	case SHT_PROGBITS:	str = "SHT_PROGBITS";	break;
	case SHT_SYMTAB:	str = "SHT_SYMTAB";		break;
	case SHT_STRTAB:	str = "SHT_STRTAB";		break;
	case SHT_RELA:		str = "SHT_RELA";		break;
	case SHT_HASH:		str = "SHT_HASH";		break;
	case SHT_DYNAMIC:	str = "SHT_DYNAMIC";	break;
	case SHT_NOTE:		str = "SHT_NOTE";		break;
	case SHT_NOBITS:	str = "SHT_NOBITS";		break;
	case SHT_REL:		str = "SHT_REL";		break;
	case SHT_SHLIB:		str = "SHT_SHLIB";		break;
	case SHT_DYNSYM:	str = "SHT_DYNSYM";		break;
	case SHT_LOPROC:	str = "SHT_LOPROC";		break;
	case SHT_HIPROC:	str = "SHT_HIPROC";		break;
	case SHT_LOUSER:	str = "SHT_LOUSER";		break;
	case SHT_HIUSER:	str = "SHT_HIUSER";		break;
	}
	fd->print(fd, "%s\n", str);
}

void shdr_entry_offset_str(rt_file_t* fd, int sh_offset){
	fd->print(fd, "sh_offset=0x%x\n", sh_offset);
}

void shdr_entry_addr_str(rt_file_t* fd, int sh_addr){
	fd->print(fd, "sh_addr=0x%x\n", sh_addr);
}

void shdr_entry_size_str(rt_file_t* fd, int sh_size){
	fd->print(fd, "sh_size=%d\n", sh_size);
}

void shdr_entry_entsize_str(rt_file_t* fd, int sh_entsize){
	fd->print(fd, "sh_entsize=%d\n", sh_entsize);
}

void shdr_entry_link_str(rt_file_t* fd, int sh_link){
	fd->print(fd, "sh_link=%d\n", sh_link);
}

void shdr_entry_info_str(rt_file_t* fd, int sh_info){
	fd->print(fd, "sh_info=0x%x\n", sh_info);
}

void shdr_entry_flags_str(rt_file_t* fd, int sh_flags){
	fd->print(fd, "sh_flags=");
	if(sh_flags & SHF_WRITE)		fd->print(fd, "SHF_WRITE | ");
	if(sh_flags & SHF_ALLOC)		fd->print(fd, "SHF_ALLOC | ");
	if(sh_flags & SHF_EXECINSTR)	fd->print(fd, "SHF_EXEINSTR | ");
	if(sh_flags & SHF_MASKPROC)		fd->print(fd, "SHF_MASKPROC");
	fd->print(fd, "\n");
}

void shdr_entry_align_str(rt_file_t* fd, int sh_addralign){
	fd->print(fd, "sh_addralign=%d\n", sh_addralign);
}

void shdr_entry_format(rt_file_t* fd, elf_section_t* section){
	elf_shdr_entry_t* shdr = (elf_shdr_entry_t*)section;
	if(shdr->name) fd->print(fd, "sh_name_str=%s\n", shdr->name);
	shdr_entry_name_str(fd, shdr->shdr.sh_name);
	shdr_entry_type_str(fd, shdr->shdr.sh_type);
	shdr_entry_flags_str(fd, shdr->shdr.sh_flags);
	shdr_entry_addr_str(fd, shdr->shdr.sh_addr);
	shdr_entry_offset_str(fd, shdr->shdr.sh_offset);
	shdr_entry_size_str(fd, shdr->shdr.sh_size);
	shdr_entry_link_str(fd, shdr->shdr.sh_link);
	shdr_entry_info_str(fd, shdr->shdr.sh_info);
	shdr_entry_align_str(fd, shdr->shdr.sh_addralign);
	shdr_entry_entsize_str(fd, shdr->shdr.sh_entsize);
	fd->print(fd, "\n");
}

#define SHDR_COLUMN_SIZE 16
void shdr_entry_dump(rt_file_t* fd, elf_section_t* section){
	elf_shdr_entry_t* shdr = (elf_shdr_entry_t*)section;
	if(!shdr->buf) {
		shdr->buf = (cbyte*)shdr->isection.data(section);
	}
	{
		unsigned int i = 0;
		for(i = 0 ; i < shdr->shdr.sh_size ; i += SHDR_COLUMN_SIZE){
			int j = 0;
			int mx = shdr->shdr.sh_size - i;
			if(mx > SHDR_COLUMN_SIZE) mx = SHDR_COLUMN_SIZE;
			for(j = 0 ; j < mx ; j++) {
				fd->print(fd, "%02x ", shdr->buf[i + j]);
				if(j == 7)	fd->print(fd, " ");
			}
			if(mx < SHDR_COLUMN_SIZE) {
				for(j = 0 ; j < SHDR_COLUMN_SIZE - mx ; j++){
					fd->print(fd, "   ");
				}
				if(mx < 7) fd->print(fd, " ");
			}
			fd->print(fd, "  ");
			for(j = 0 ; j < mx ; j++) fd->print(fd, "%c", shdr->buf[i + j]);
			fd->print(fd, "\n");
		}
	}
}

void shdr_entry_symtab_dump(rt_file_t* fd, elf_section_t* section){
	elf_shdr_entry_t* shdr = (elf_shdr_entry_t*)section;
	Elf32_Sym* sym = (Elf32_Sym*)rt_new(shdr->shdr.sh_size);
	shdr->fd->seek(shdr->fd, shdr->shdr.sh_offset, RT_FILE_SEEK_SET);
	shdr->fd->read(shdr->fd, sym, shdr->shdr.sh_size);

	{
		char* str = cnull;
		unsigned int sti = 0;
		unsigned int i = 0, num = shdr->shdr.sh_size / shdr->shdr.sh_entsize;
		for(i = 0 ; i < num ; i++){
			fd->print(fd, "sym[%d].st_name=%d\n", i, sym[i].st_name);
			fd->print(fd, "sym[%d].st_vale=%d\n", i, sym[i].st_value);
			fd->print(fd, "sym[%d].st_size=%d\n", i, sym[i].st_size);
			fd->print(fd, "sym[%d].st_info=0x%x\n", i, sym[i].st_info);
			sti = sym[i].st_info;
			sti = ELF32_ST_BIND(sti);
			fd->print(fd, "sym[%d].st_bind=", i);
			switch(sti){
				case STB_LOCAL:		str = "STB_LOCAL";	break;
				case STB_GLOBAL:	str = "STB_GLOBAL";	break;
				case STB_WEAK:		str = "STB_WEAK";	break;
				case STB_LOPROC:	str = "STB_LOPROC";	break;
				case STB_HIPROC:	str = "STB_HIPROC";	break;
			}
			fd->print(fd, "%s\n", str);
			sti = sym[i].st_info;
			sti = ELF32_ST_TYPE(sti);
			fd->print(fd, "sym[%d].st_type=", i);
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
			fd->print(fd, "sym[%d].st_other=0x%x\n", i, sym[i].st_other);
			fd->print(fd, "sym[%d].st_shndx=%d\n", i, sym[i].st_shndx);
			fd->print(fd, "\n");
		}
	}
}

void shdr_entry_text_dump(rt_file_t* fd, elf_section_t* section){
	elf_shdr_entry_t* shdr = (elf_shdr_entry_t*)section;
	cbyte* data = rt_new(shdr->shdr.sh_size);
	shdr->fd->seek(shdr->fd, shdr->shdr.sh_offset, RT_FILE_SEEK_SET);
	shdr->fd->read(shdr->fd, data, shdr->shdr.sh_size);
	
	fd->print(fd, "%s", data);

	rt_delete(data);
}

elf_section_t* shdr_entry_sub(elf_section_t* section, const int idx){
	elf_shdr_entry_t* shdr = (elf_shdr_entry_t*)section;
	return cnull;
}

cbyte* shdr_entry_data(elf_section_t* section){
	elf_shdr_entry_t* shdr = (elf_shdr_entry_t*)section;
	if(!shdr->buf) {
		shdr->buf = rt_new(shdr->shdr.sh_size);
		shdr->fd->seek(shdr->fd, shdr->shdr.sh_offset, RT_FILE_SEEK_SET);
		shdr->fd->read(shdr->fd, shdr->buf, shdr->shdr.sh_size);
	}
	return shdr->buf;
}

const char* shdr_entry_name(elf_section_t* section){
	elf_shdr_entry_t* shdr = (elf_shdr_entry_t*)section;
	return shdr->name;
}

elf_section_t* shdr_entry_new(rt_file_t* fd, int e_shoff, int n_entry, int e_shstrndx){
	elf_shdr_entry_t* shdr_entry = rt_new(sizeof(elf_shdr_entry_t) * n_entry);
	fd->seek(fd, e_shoff, RT_FILE_SEEK_SET);
	{
		char* strtab = cnull;
		int i;
		for(i = 0 ; i < n_entry ; i++) {
			shdr_entry[i].fd = fd;
			fd->read(fd, &(shdr_entry[i].shdr), sizeof(Elf32_Shdr));
			shdr_entry[i].isection.format = shdr_entry_format;
			switch(shdr_entry[i].shdr.sh_type){
				case SHT_SYMTAB:
				case SHT_DYNSYM:
					shdr_entry[i].isection.dump = shdr_entry_symtab_dump;
					break;
				default:
					shdr_entry[i].isection.dump = shdr_entry_dump;
					break;
			}
			shdr_entry[i].isection.get_sub = shdr_entry_sub;
			shdr_entry[i].isection.data = shdr_entry_data;
			shdr_entry[i].isection.get_name = shdr_entry_name;
		}
		strtab = (char*)shdr_entry[e_shstrndx].isection.data((elf_section_t*)&shdr_entry[e_shstrndx]);
		for(i = 0 ; i < n_entry ; i++){
			shdr_entry[i].name = shdr_entry[i].shdr.sh_name + strtab;
		}
	}
	return (elf_section_t*) shdr_entry;
}

void shdr_entry_delete(elf_section_t* section){
	elf_shdr_entry_t* shdr_entry = (elf_shdr_entry_t*)section;
	if(shdr_entry->buf) rt_delete(shdr_entry->buf);
	rt_delete(shdr_entry);
}

int shdr_entry_size(void){
	return sizeof(elf_shdr_entry_t);
}
