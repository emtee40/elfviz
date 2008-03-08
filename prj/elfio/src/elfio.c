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
#include "shdr.h"

typedef struct _elfio_ctrl_t{
	elfio_t ielf;
	char* file;
	rt_file_t* fd;
	Elf32_Ehdr ehdr;
	elf_section_t* phdr;
	elf_section_t* shdr;
}elfio_ctrl_t;

const char* elfio_file_name(elfio_t* elfio){
	elfio_ctrl_t* elf_ctrl = (elfio_ctrl_t*)elfio;
	return elf_ctrl->file;
}

void elfio_type_str(rt_file_t* fd, int e_type){
	char* str = "unknown";
	fd->print(fd, "type=");
	switch(e_type){
	case ET_NONE:	str =  "ET_NONE";	break;
	case ET_REL:	str =  "ET_REL";	break;
	case ET_EXEC:	str =  "ET_EXEC";	break;
	case ET_DYN:	str =  "ET_DYN";	break;
	case ET_CORE:	str =  "ET_CORE";	break;
	case ET_LOPROC:	str =  "ET_LOPROC";	break;
	case ET_HIPROC:	str =  "ET_HIPROC";	break;
	}
	fd->print(fd, "%s\n", str);
}

void elfio_machine_str(rt_file_t* fd, int e_machine){
	char* str = "unknown";
	fd->print(fd, "machine=");
	switch(e_machine){
	case EM_NONE:			str =  "EM_NONE";			break;
	case EM_M32:			str =  "EM_M32";			break;
	case EM_SPARC:			str =  "EM_SPARC";			break;
	case EM_386:			str =  "EM_386";			break;
	case EM_68K:			str =  "EM_68K";			break;
	case EM_88K:			str =  "EM_88K";			break;
	case EM_860:			str =  "EM_860";			break;
	case EM_MIPS:			str =  "EM_MIPS";			break;
	case EM_MIPS_RS4_BE:	str =  "EM_MIPS_RS4_BE";	break;
	case EM_ARM:			str =  "EM_ARM";			break;
	}
	fd->print(fd, "%s\n", str);
}

void elfio_version_str(rt_file_t* fd, int e_version){
	char* str = "unknown";
	fd->print(fd, "version=");
	switch(e_version){
	case EV_NONE:		str =  "EV_NONE";		break;
	case EV_CURRENT:	str =  "EV_CURRENT";	break;
	}
	fd->print(fd, "%s\n", str);
}

void elfio_ident_str(rt_file_t* fd, char* e_ident){
	char* str = "unknown";
	fd->print(fd, "class=");
	switch(e_ident[EI_CLASS]){
	case ELFCLASSNONE:	str = "ELFCLASSNONE";	break;
	case ELFCLASS32:	str = "ELFCLASS32";		break;
	case ELFCLASS64:	str = "ELFCLASS64";		break;
	}
	fd->print(fd, "%s\n", str);
	str = "unknown";
	fd->print(fd, "data=");
	switch(e_ident[EI_DATA]){
	case ELFDATANONE:	str = "ELFDATANONE";	break;
	case ELFDATA2LSB:	str = "ELFDATA2LSB";	break;
	case ELFDATA2MSB:	str = "ELFDATA2MSB";	break;
	}
	fd->print(fd, "%s\n", str);
}

void elfio_entry_str(rt_file_t* fd, int e_entry){
	fd->print(fd, "entry=0x%x\n", e_entry);
}

void elfio_phoff_str(rt_file_t* fd, int e_phoff){
	fd->print(fd, "phoff=0x%x\n", e_phoff);
}

void elfio_shoff_str(rt_file_t* fd, int e_shoff){
	fd->print(fd, "shoff=0x%x\n", e_shoff);
}

void elfio_flags_str(rt_file_t* fd, int e_flags){
	fd->print(fd, "flags=");
	if(e_flags & EF_ARM_HASENTRY)			fd->print(fd, "EF_ARM_HASENTRY | ");
	if(e_flags & EF_ARM_SYMSARESORTED)		fd->print(fd, "EF_ARM_SYMSARESORTED | ");
	if(e_flags & EF_ARM_DYNSYMSUSESEGIDX)	fd->print(fd, "EF_ARM_DYNSYMSUSESEGIDX | ");
	if(e_flags & EF_ARM_MAPSYMSFIRST)		fd->print(fd, "EF_ARM_MAPSYMSFIRST | ");
	if(e_flags & EF_ARM_EABIMASK)			fd->print(fd, "EF_ARM_EABIMASK");
	fd->print(fd, "\n");
}

void elfio_ehsize_str(rt_file_t* fd, int e_ehsize){
	fd->print(fd, "ehsize=%d\n", e_ehsize);
}

void elfio_phentsize_str(rt_file_t* fd, int e_phentsize){
	fd->print(fd, "phentsize=%d\n", e_phentsize);
}

void elfio_phnum_str(rt_file_t* fd, int e_phnum){
	fd->print(fd, "phnum=%d\n", e_phnum);
}

void elfio_shentsize_str(rt_file_t* fd, int e_shentsize){
	fd->print(fd, "shentsize=%d\n", e_shentsize);
}

void elfio_shnum_str(rt_file_t* fd, int e_shnum){
	fd->print(fd, "shnum=%d\n", e_shnum);
}

void elfio_shstridx_str(rt_file_t* fd, int e_shstridx){
	fd->print(fd, "shstridx=%d\n", e_shstridx);
}

void elfio_format(rt_file_t* rtout, elfio_t* elfio){
	elfio_ctrl_t* elf_ctrl = (elfio_ctrl_t*)elfio;
	rtout->print(rtout, "ELF header\n");
	elfio_ident_str(rtout, elf_ctrl->ehdr.e_ident);
	elfio_type_str(rtout, elf_ctrl->ehdr.e_type);
	elfio_machine_str(rtout, elf_ctrl->ehdr.e_machine);
	elfio_version_str(rtout, elf_ctrl->ehdr.e_version);
	elfio_entry_str(rtout, elf_ctrl->ehdr.e_entry);
	elfio_phoff_str(rtout, elf_ctrl->ehdr.e_phoff);
	elfio_shoff_str(rtout, elf_ctrl->ehdr.e_shoff);
	elfio_flags_str(rtout, elf_ctrl->ehdr.e_flags);
	elfio_ehsize_str(rtout, elf_ctrl->ehdr.e_ehsize);
	elfio_phentsize_str(rtout, elf_ctrl->ehdr.e_phentsize);
	elfio_phnum_str(rtout, elf_ctrl->ehdr.e_phnum);
	elfio_shentsize_str(rtout, elf_ctrl->ehdr.e_shentsize);
	elfio_shnum_str(rtout, elf_ctrl->ehdr.e_shnum);
	elfio_shstridx_str(rtout, elf_ctrl->ehdr.e_shstrndx);
}

elf_section_t* elfio_phdr(elfio_t* elfio){
	elfio_ctrl_t* elf_ctrl = (elfio_ctrl_t*)elfio;
	if(!elf_ctrl->phdr) elf_ctrl->phdr = phdr_new(elf_ctrl->ehdr, elf_ctrl->fd);
	return elf_ctrl->phdr;
}

elf_section_t* elfio_shdr(elfio_t* elfio){
	elfio_ctrl_t* elf_ctrl = (elfio_ctrl_t*)elfio;
	if(!elf_ctrl->shdr) elf_ctrl->shdr = shdr_new(elf_ctrl->ehdr, elf_ctrl->fd);
	return elf_ctrl->shdr;
}

elfio_t* elfio_new(char* file){
	elfio_ctrl_t* elfio = rt_new(sizeof(elfio_ctrl_t));
	elfio->ielf.file_name = elfio_file_name;
	elfio->ielf.format = elfio_format;
	elfio->ielf.get_phdr = elfio_phdr;
	elfio->ielf.get_shdr = elfio_shdr;
	elfio->fd = rt_file_new(file, RT_FILE_OPEN_RDONLY);
	if(!elfio->fd) {
		rt_delete(elfio);
		return cnull;
	} else {
		elfio->fd->read(elfio->fd, &elfio->ehdr, sizeof(elfio->ehdr));
	}
	elfio->file = rt_strdup(file);
	return (elfio_t*)elfio;
}

void elfio_delete(elfio_t* elfio){
	elfio_ctrl_t* elf_ctrl = (elfio_ctrl_t*)elfio;
	rt_file_delete(elf_ctrl->fd);
	rt_delete(elf_ctrl->file);
	if(elf_ctrl->phdr) phdr_delete(elf_ctrl->phdr);
	if(elf_ctrl->shdr) shdr_delete(elf_ctrl->shdr);
	rt_delete(elf_ctrl);
}

static unsigned long elf_hash(const unsigned char *name) {
	unsigned long h, g;
	for (h = 0; *name != 0; ++name)	{
		h = (h << 4) + *name;
		g = h & 0xf0000000;
		if (g != 0) h ^= g >> 24;
		h &= ~g;
	}
	return h;
}
