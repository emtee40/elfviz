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
#include <stdlib.h>

#include "elfio/elfio.h"
#include "elftypes.h"
#include "shdr_symtab_entry.h"
#include "elf_defs.h"

static const section_attr_t shdr_symtab_attr[] = {
	{"st_name",		ELF_TYPE_STR | ELF_TYPE_INT	},
	{"st_value",		ELF_TYPE_INT | ELF_TYPE_HEX	},
	{"st_size",		ELF_TYPE_INT			},
	{"st_info",		ELF_TYPE_INT			},
	{"st_other",		ELF_TYPE_INT			},
	{"st_shndx",		ELF_TYPE_INT			},
	{"st_bind",		ELF_TYPE_STR			},
	{"st_type",		ELF_TYPE_STR			}
};

class shdr_symtab_attr_t : public elf_attr_t {
	public:
		shdr_symtab_attr_t(Elf32_Sym& shdr, char* ename):name(0), hdr(shdr), num(sizeof(shdr_symtab_attr) / sizeof(section_attr_t)){
			if(ename) name = strdup(ename);
		}

		~shdr_symtab_attr_t(){free(name);}

		virtual const unsigned int get_num(void){
			return num;
		}

		virtual const unsigned int get_type(int idx){
			return shdr_symtab_attr[idx].type;
		}

		virtual const char* get_name(int idx){
			return shdr_symtab_attr[idx].name;
		}

		virtual const char* get_str(int idx){
			char* ret = 0;
			switch(idx){
				case 0:	ret = name_str();	break;
				case 6:	ret = bind_str();	break;
				case 7:	ret = type_str();	break;
				default:	throw "invalid argument";	break;
			}
			return ret;
		}

		virtual const int get_int(int idx){
			unsigned int ret = 0;
			switch(idx){
				case 0:	ret = hdr.st_name;	break;
				case 1:	ret = hdr.st_value;	break;
				case 2:	ret = hdr.st_size;	break;
				case 3:	ret = hdr.st_info;	break;
				case 4:	ret = hdr.st_other;	break;
				case 5:	ret = hdr.st_shndx;	break;
				default:	throw "invalid argument";	break;
			}
			return ret;
		}

	private:
		Elf32_Sym& hdr;
		char* name;
		const unsigned int num;

		char* name_str(void){
			return (name) ? name : (char*)"null";
		}

		char* bind_str(void){
			char* str = 0;
			unsigned int sti = hdr.st_info;
			sti = ELF32_ST_BIND(sti);
			switch(sti){
				case STB_LOCAL:		str = (char*)"STB_LOCAL";	break;
				case STB_GLOBAL:	str = (char*)"STB_GLOBAL";	break;
				case STB_WEAK:		str = (char*)"STB_WEAK";	break;
				case STB_LOPROC:	str = (char*)"STB_LOPROC";	break;
				case STB_HIPROC:	str = (char*)"STB_HIPROC";	break;
			}
			return str;
		}

		char* type_str(void){
			char* str = 0;
			unsigned int sti = hdr.st_info;
			sti = ELF32_ST_TYPE(sti);
			switch(sti){
				case STT_NOTYPE:	str = (char*)"STT_NOTYPE";	break;
				case STT_OBJECT:	str = (char*)"STT_OBJECT";	break;
				case STT_FUNC:		str = (char*)"STT_FUNC";	break;
				case STT_SECTION:	str = (char*)"STT_SECTION";	break;
				case STT_FILE:		str = (char*)"STT_FILE";	break;
				case STT_LOPROC:	str = (char*)"STT_LOPROC";	break;
				case STT_HIPROC:	str = (char*)"STT_HIPROC";	break;
			}
			return str;
		}

};
	
class elf_shdr_symtab_entry_t : public elf_section_t{
	protected:
		Elf32_Sym symtab;
		char* ss_name;
		shdr_symtab_attr_t attr;

	public:
		elf_shdr_symtab_entry_t(Elf32_Sym esymtab, char* ename):attr(symtab, ename){
			memcpy(&symtab, &esymtab, sizeof(Elf32_Sym));
			ss_name = strdup(ename);
		}

		~elf_shdr_symtab_entry_t(){
			delete ss_name;
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
			return 0;
		}

		virtual const char* category(void){
			return "symbol";
		}

		virtual const char* name(void){
			return ss_name;
		}
};

elf_section_t* shdr_symtab_entry_new(FILE* fd, unsigned int sh_offset, char* strtab){
	Elf32_Sym symtab;
	fseek(fd, sh_offset, SEEK_SET);
	fread(&symtab, sizeof(Elf32_Sym), 1, fd);
	return (elf_section_t*) new elf_shdr_symtab_entry_t(symtab, symtab.st_name + strtab);
}
