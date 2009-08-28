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
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <ctype.h>

#include "elfio/elfio.h"
#include "elftypes.h"
#include "shdr_entry.h"
#include "shdr_symtab_entry.h"
#include "shdr_rel_entry.h"
#include "elf_defs.h"

static const section_attr_t shdr_entry_attr[] = {
	{"sh_name",		ELF_TYPE_STR | ELF_TYPE_INT	},
	{"sh_type",		ELF_TYPE_STR | ELF_TYPE_INT	},
	{"sh_flags",		ELF_TYPE_STR | ELF_TYPE_INT	},
	{"sh_addr",		ELF_TYPE_INT | ELF_TYPE_HEX	},
	{"sh_offset",		ELF_TYPE_INT | ELF_TYPE_HEX	},
	{"sh_size",		ELF_TYPE_INT			},
	{"sh_link",		ELF_TYPE_INT			},
	{"sh_info",		ELF_TYPE_INT			},
	{"sh_addralign",	ELF_TYPE_INT			},
	{"sh_entsize",		ELF_TYPE_INT			}
};

class shdr_entry_attr_t : public elf_attr_t {
	public:
		shdr_entry_attr_t(Elf32_Shdr& shdr, const char* ename):hdr(shdr), name(0), num(sizeof(shdr_entry_attr) / sizeof(section_attr_t)){
			if(ename) name = strdup(ename);
		}

		virtual const unsigned int get_num(void){
			return num;
		}

		virtual const unsigned int get_type(int idx){
			return shdr_entry_attr[idx].type;
		}

		virtual const char* get_name(int idx){
			return shdr_entry_attr[idx].name;
		}

		virtual const char* get_str(int idx){
			char* ret = 0;
			switch(idx){
				case 0:	ret = name_str();	break;
				case 1:	ret = type_str();	break;
				case 2:	ret = flags_str();	break;
				default:	throw "invalid argument";	break;
			}
			return ret;
		}

		virtual const int get_int(int idx){
			unsigned int ret = 0;
			switch(idx){
				case 0:	ret = hdr.sh_name;	break;
				case 1:	ret = hdr.sh_type;	break;
				case 2:	ret = hdr.sh_flags;	break;
				case 3:	ret = hdr.sh_addr;	break;
				case 4:	ret = hdr.sh_offset;	break;
				case 5:	ret = hdr.sh_size;	break;
				case 6:	ret = hdr.sh_link;	break;
				case 7:	ret = hdr.sh_info;	break;
				case 8:	ret = hdr.sh_addralign;	break;
				case 9:	ret = hdr.sh_entsize;	break;
				default:	throw "invalid argument";	break;
			}
			return ret;
		}

	private:
		Elf32_Shdr& hdr;
		char* name;
		const unsigned int num;

		char* name_str(void){
			return (name) ? name : (char*)"null";
		}

		char* type_str(void){
			char* str = (char*)"unknown";
			switch(hdr.sh_type){
			case SHT_NULL:		str = (char*)"SHT_NULL";	break;
			case SHT_PROGBITS:	str = (char*)"SHT_PROGBITS";	break;
			case SHT_SYMTAB:	str = (char*)"SHT_SYMTAB";	break;
			case SHT_STRTAB:	str = (char*)"SHT_STRTAB";	break;
			case SHT_RELA:		str = (char*)"SHT_RELA";	break;
			case SHT_HASH:		str = (char*)"SHT_HASH";	break;
			case SHT_DYNAMIC:	str = (char*)"SHT_DYNAMIC";	break;
			case SHT_NOTE:		str = (char*)"SHT_NOTE";	break;
			case SHT_NOBITS:	str = (char*)"SHT_NOBITS";	break;
			case SHT_REL:		str = (char*)"SHT_REL";		break;
			case SHT_SHLIB:		str = (char*)"SHT_SHLIB";	break;
			case SHT_DYNSYM:	str = (char*)"SHT_DYNSYM";	break;
			case SHT_LOPROC:	str = (char*)"SHT_LOPROC";	break;
			case SHT_HIPROC:	str = (char*)"SHT_HIPROC";	break;
			case SHT_LOUSER:	str = (char*)"SHT_LOUSER";	break;
			case SHT_HIUSER:	str = (char*)"SHT_HIUSER";	break;
			}
			return str;
		}

		char* flags_str(void){
			static char str[128];
			str[0] = 0;
			if(hdr.sh_flags & SHF_WRITE){		if(str[0]) strcat(str, " | SHF_WRITE");		else strcpy(str, "SHF_WRITE");}
			if(hdr.sh_flags & SHF_ALLOC){		if(str[0]) strcat(str, " | SHF_ALLOC");		else strcpy(str, "SHF_ALLOC");}
			if(hdr.sh_flags & SHF_EXECINSTR){	if(str[0]) strcat(str, " | SHF_EXECINSTR");	else strcpy(str, "SHF_EXECINSTR");}
			if(hdr.sh_flags & SHF_MASKPROC){	if(str[0]) strcat(str, " | SHF_MASKPROC");	else strcpy(str, "SHF_MASKPROC");}
			return str;
		}
};


class elf_shdr_entry_t : public elf_section_t{
	protected:
		Elf32_Shdr shdr;
		char* se_name;
		shdr_entry_attr_t attr;
		elf_buffer_t body;

	public:
		elf_shdr_entry_t(Elf32_Shdr eshdr, unsigned char* ebuf, char* ename):se_name(0), attr(shdr, ename),body(ebuf, eshdr.sh_size, eshdr.sh_offset){
			memcpy(&shdr, &eshdr, sizeof(Elf32_Shdr));
			if(ename) se_name = strdup(ename);
		}

		~elf_shdr_entry_t(){
			if(se_name) delete se_name;
		}

		virtual elf_attr_t* get_attr(void){
			return &attr;
		}

		virtual const unsigned int get_child_num(void){
			return 0;
		}

		virtual elf_section_t* get_child(const int idx){
			return 0;
		}

		virtual elf_section_t* get_child(const char* stridx){
			return 0;
		}

		virtual elf_buffer_t* get_body(void){
			return (body.buffer) ? &body : 0;
		}

		virtual const char* category(void){
			return "entry";
		}

		virtual const char* name(void){
			return se_name;
		}
};

class elf_shdr_symtab_t : public elf_shdr_entry_t{
	protected:
		elf_section_t** entry;
		int n_entry;
	public:
		elf_shdr_symtab_t(FILE* fd, Elf32_Shdr eshdr, unsigned char* ebuf, char* ename, char* strtab, elf_section_t*(*entry_new)(FILE*, unsigned int, char*))
							: elf_shdr_entry_t(eshdr, 0, ename){
			n_entry = eshdr.sh_size / eshdr.sh_entsize;
			entry = new elf_section_t* [n_entry];
			for(int i = 0 ; i < n_entry ; i++){
				entry[i] = entry_new(fd, eshdr.sh_offset + sizeof(Elf32_Sym) * i, strtab);
			}
		}

		~elf_shdr_symtab_t(){
			if(n_entry){
				for(int i = 0 ; i < n_entry ; i++) delete entry[i];
				delete entry;
			}
		}

		virtual const unsigned int get_child_num(void){
			return n_entry;
		}

		virtual elf_section_t* get_child(const int idx){
			return entry[idx];
		}


		virtual elf_section_t* get_child(const char* stridx){
			int i = 0, num = shdr.sh_size / shdr.sh_entsize;
			for(i = 0 ; i < num ; i++) if(!strcmp(entry[i]->name(), stridx)) return entry[i];
			return 0;
		}
};

elf_section_t* shdr_entry_new(FILE* fd, int e_shoff, char* shstrtab, char* strtab){
	Elf32_Shdr shdr;
	unsigned char* buf = 0;
	elf_section_t* section = 0;

	fseek(fd, e_shoff, SEEK_SET);
	fread(&shdr, sizeof(Elf32_Shdr), 1, fd);

	fseek(fd, shdr.sh_offset, SEEK_SET);
	buf = (unsigned char*)new unsigned char[shdr.sh_size];
	fread(buf, shdr.sh_size, 1, fd);

	switch(shdr.sh_type){
		case SHT_SYMTAB:
		case SHT_DYNSYM:
			section = (!strtab) ? 0 : new elf_shdr_symtab_t(fd, shdr, buf, (!shstrtab) ? 0 : shstrtab + shdr.sh_name, strtab, shdr_symtab_entry_new);
			break;
		case SHT_REL:
			section = (!strtab) ? 0 : new elf_shdr_symtab_t(fd, shdr, buf, (!shstrtab) ? 0 : shstrtab + shdr.sh_name, strtab, shdr_rel_entry_new);
			break;
		case SHT_RELA:
			section = (!strtab) ? 0 : new elf_shdr_symtab_t(fd, shdr, buf, (!shstrtab) ? 0 : shstrtab + shdr.sh_name, strtab, shdr_rela_entry_new);
			break;
		default:
			section = new elf_shdr_entry_t(shdr, buf, (!shstrtab) ? 0 : shstrtab + shdr.sh_name);
			break;
	}
	return section;
}
