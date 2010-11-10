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
#include <string.h>

#include "elfio/elfio.h"
#include "elftypes.h"
#include "armelftypes.h"
#include "phdr_entry.h"
#include "elf_defs.h"

static const section_attr_t phdr_entry_attr[] = {
	{"p_type",	ELF_TYPE_STR | ELF_TYPE_INT	},
	{"p_offset",	ELF_TYPE_INT | ELF_TYPE_HEX	},
	{"p_vaddr",	ELF_TYPE_INT | ELF_TYPE_HEX	},
	{"p_paddr",	ELF_TYPE_INT | ELF_TYPE_HEX	},
	{"p_filesz",	ELF_TYPE_INT			},
	{"p_memsz",	ELF_TYPE_INT			},
	{"p_flags",	ELF_TYPE_STR | ELF_TYPE_INT	},
	{"p_align",	ELF_TYPE_INT			}
};

class elfPhdrEntryAttribute : public elfAttribute {
	public:
		elfPhdrEntryAttribute(Elf32_Phdr& phdr):hdr(phdr), num(sizeof(phdr_entry_attr) / sizeof(section_attr_t)){ }

		const unsigned int number(void){
			return num;
		}

		const unsigned int type(int idx){
			return phdr_entry_attr[idx].type;
		}

		const char* name(int idx){
			return phdr_entry_attr[idx].name;
		}

		const char* stringValue(int idx){
			char* ret = 0;
			switch(idx){
				case 0:	ret = type_str();	break;
				case 6:	ret = flags_str();	break;
				default:	throw "invalid argument";	break;
			}
			return ret;
		}

		const int numericValue(int idx){
			unsigned int ret = 0;
			switch(idx){
				case 0:	ret = hdr.p_type;	break;
				case 1:	ret = hdr.p_offset;	break;
				case 2:	ret = hdr.p_vaddr;	break;
				case 3:	ret = hdr.p_paddr;	break;
				case 4:	ret = hdr.p_filesz;	break;
				case 5:	ret = hdr.p_memsz;	break;
				case 6:	ret = hdr.p_flags;	break;
				case 7:	ret = hdr.p_align;	break;
				default:	throw "invalid argument";	break;
			}
			return ret;
		}

	private:
		Elf32_Phdr& hdr;
		const unsigned int num;
		char* type_str(void){
			char* str = (char*)"unknown";
			switch(hdr.p_type){
			case PT_NULL:		str = (char*)"PT_NULL";	break;
			case PT_LOAD:		str = (char*)"PT_LOAD";	break;
			case PT_DYNAMIC:	str = (char*)"PT_DYNAMIC";	break;
			case PT_INTERP:		str = (char*)"PT_INTERP";	break;
			case PT_NOTE:		str = (char*)"PT_NOTE";	break;
			case PT_SHLIB:		str = (char*)"PT_SHLIB";	break;
			case PT_PHDR:		str = (char*)"PT_PHDR";	break;
//			case PT_LOPROC:		str = (char*)"PT_LOPROC";	break;
			case PT_HIPROC:		str = (char*)"PT_HIPROC";	break;
			case PT_ARM_ARCHEXT:	str = (char*)"PT_ARM_ARCHEXT";	break;
			case PT_ARM_EXIDX:	str = (char*)"PT_ARM_EXIDX";	break;
			}
			return str;
		}

		char* flags_str(void){
			static char str[128] = "\0";
			str[0] = 0;
			if(hdr.p_flags & PF_X){	if(str[0]) strcat(str, " | PF_X");	else strcpy(str, "PF_X");}
			if(hdr.p_flags & PF_W){	if(str[0]) strcat(str, " | PF_W");	else strcpy(str, "PF_W");}
			if(hdr.p_flags & PF_R){	if(str[0]) strcat(str, " | PF_R");	else strcpy(str, "PF_R");}
			return str;
		}

};

class elfPhdrEntry : public elfSection{
	protected:
		Elf32_Phdr phdr;
		char phename[8];
		elfPhdrEntryAttribute attr;

	public:
		elfPhdrEntry(FILE* fd, int phoff, char* pname):attr(phdr){
			fseek(fd, phoff, SEEK_SET);
			fread(&(phdr), sizeof(Elf32_Phdr), 1, fd);
			strcpy(phename, pname);
		}

		elfAttribute* attribute(void){
			return &attr;
		}

		const unsigned int childs(void){
			return 0;
		}

		elfSection* childAt(const int idx){
			return 0;
		}

		elfSection* childAt(const char* stridx){
			return 0;
		}

		elfBuffer* body(void){
			return 0;
		}

		const char* category(void){
			return "entry";
		}

		const char* name(void){
			return phename;
		}

};

elfSection* phdr_entry_new(FILE* fd, int phoff, char* name){
	return (elfSection*) new elfPhdrEntry(fd, phoff, name);

}
