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
#include "phdr.h"
#include "shdr.h"
#include "elf_defs.h"

static const section_attr_t elf_attr[] = {
	{"class",	ELF_TYPE_INT | ELF_TYPE_STR	},
	{"data",	ELF_TYPE_INT | ELF_TYPE_STR	},
	{"machine",	ELF_TYPE_INT | ELF_TYPE_STR	},
	{"version",	ELF_TYPE_INT | ELF_TYPE_STR	},
	{"type",	ELF_TYPE_INT | ELF_TYPE_STR	},
	{"phoff",	ELF_TYPE_INT | ELF_TYPE_HEX	},
	{"shoff",	ELF_TYPE_INT | ELF_TYPE_HEX	},
	{"flags",	ELF_TYPE_INT | ELF_TYPE_STR	},
	{"ehsize",	ELF_TYPE_INT			},
	{"phentsize",	ELF_TYPE_INT			},
	{"phnum",	ELF_TYPE_INT			},
	{"shentsize",	ELF_TYPE_INT			},
	{"shnum",	ELF_TYPE_INT			},
	{"shstrndx",	ELF_TYPE_INT			}
};

class elfio_attr_t : public elf_attr_t{
	public:
		elfio_attr_t(Elf32_Ehdr& ehdr):hdr(ehdr), num(sizeof(elf_attr)/sizeof(section_attr_t)){}
		virtual const char* get_str(int idx){
			char* ret = 0;
			switch(idx){
				case 0:	ret = ident_class_str();	break;
				case 1:	ret = ident_data_str();		break;
				case 2:	ret = machine_str();		break;
				case 3:	ret = version_str();		break;
				case 4:	ret = type_str();		break;
				case 7:	ret = flags_str();		break;
				default:	throw "invalid argument";	break;
			}
			return ret;
		}

		virtual const int get_int(int idx){
			int ret = 0;
			switch(idx){
				case 0:		ret = hdr.e_ident[EI_CLASS];	break;
				case 1:		ret = hdr.e_ident[EI_DATA];	break;
				case 2:		ret = hdr.e_machine;		break;
				case 3:		ret = hdr.e_version;		break;
				case 4:		ret = hdr.e_entry;		break;
				case 5:		ret = hdr.e_phoff;		break;
				case 6:		ret = hdr.e_shoff;		break;
				case 7:		ret = hdr.e_flags;		break;
				case 8:		ret = hdr.e_ehsize;		break;
				case 9:		ret = hdr.e_phentsize;		break;
				case 10:	ret = hdr.e_phnum;		break;
				case 11:	ret = hdr.e_shentsize;		break;
				case 12:	ret = hdr.e_shnum;		break;
				case 13:	ret = hdr.e_shstrndx;		break;
				default:	throw "invalid argument";	break;
			}
			return ret;
		}
		virtual const unsigned int get_type(int idx){
			return elf_attr[idx].type;
		}

		virtual const char* get_name(int idx){
			return elf_attr[idx].name;
		}

		virtual const unsigned int get_num(void){
			return num;
		}

	private:
		Elf32_Ehdr& hdr;
		const unsigned int num;

		char* ident_class_str(void){
			char* str = (char*)"unknown";
			switch(hdr.e_ident[EI_CLASS]){
				case ELFCLASSNONE:	str = (char*)"ELFCLASSNONE";	break;
				case ELFCLASS32:	str = (char*)"ELFCLASS32";	break;
				case ELFCLASS64:	str = (char*)"ELFCLASS64";	break;
			}
			return str;
		}

		char* ident_data_str(void){
			char* str = (char*)"unknown";
			switch(hdr.e_ident[EI_DATA]){
				case ELFDATANONE:	str = (char*)"ELFDATANONE";	break;
				case ELFDATA2LSB:	str = (char*)"ELFDATA2LSB";	break;
				case ELFDATA2MSB:	str = (char*)"ELFDATA2MSB";	break;
			}
			return str;
		}

		char* type_str(void){
			char* str = (char*)"unknown";
			switch(hdr.e_type){
				case ET_NONE:	str = (char*)"ET_NONE";		break;
				case ET_REL:	str = (char*)"ET_REL";		break;
				case ET_EXEC:	str = (char*)"ET_EXEC";		break;
				case ET_DYN:	str = (char*)"ET_DYN";		break;
				case ET_CORE:	str = (char*)"ET_CORE";		break;
				case ET_LOPROC:	str = (char*)"ET_LOPROC";	break;
				case ET_HIPROC:	str = (char*)"ET_HIPROC";	break;
			}
			return str;
		}

		char* machine_str(void){
			char* str = (char*)"unknown";
			switch(hdr.e_machine){
				case EM_NONE:			str = (char*)"EM_NONE";			break;
				case EM_M32:			str = (char*)"EM_M32";			break;
				case EM_SPARC:			str = (char*)"EM_SPARC";		break;
				case EM_386:			str = (char*)"EM_386";			break;
				case EM_68K:			str = (char*)"EM_68K";			break;
				case EM_88K:			str = (char*)"EM_88K";			break;
				case EM_860:			str = (char*)"EM_860";			break;
				case EM_MIPS:			str = (char*)"EM_MIPS";			break;
				case EM_MIPS_RS4_BE:		str = (char*)"EM_MIPS_RS4_BE";		break;
				case EM_ARM:			str = (char*)"EM_ARM";			break;
			}
			return str;
		}

		char* version_str(void){
			char* str = (char*)"unknown";
			switch(hdr.e_version){
				case EV_NONE:		str = (char*)"EV_NONE";		break;
				case EV_CURRENT:	str = (char*)"EV_CURRENT";	break;
			}
			return str;
		}

		char* flags_str(void){
			static char str[512] = "\0";
			str[0] = 0;
			if(hdr.e_flags & EF_ARM_HASENTRY){		if(str[0]) strcat(str, " | EF_ARM_HASENTRY");		else strcpy(str, "EF_ARM_HASENTRY"); }
			if(hdr.e_flags & EF_ARM_SYMSARESORTED){		if(str[0]) strcat(str, " | EF_ARM_SYMSARESORTED");	else strcpy(str, "EF_ARM_SYMSARESORTED"); }
			if(hdr.e_flags & EF_ARM_DYNSYMSUSESEGIDX){	if(str[0]) strcat(str, " | EF_ARM_DYNSYMSUSESEGIDX");	else strcpy(str, "EF_ARM_DYNSYMSUSESEGIDX"); }
			if(hdr.e_flags & EF_ARM_MAPSYMSFIRST){		if(str[0]) strcat(str, " | EF_ARM_MAPSYMSFIRST");	else strcpy(str, "EF_ARM_MAPSYMSFIRST"); }
			if(hdr.e_flags & EF_ARM_EABIMASK){		if(str[0]) strcat(str, " | EF_ARM_EABIMASK");		else strcpy(str, "EF_ARM_EABIMASK"); }
			return str;
		}
};

class elfio_ctrl_t : public elf_section_t{
	protected:
		char file[512];
		FILE* fd;
		Elf32_Ehdr ehdr;
		elf_section_t* mphdr;
		elf_section_t* mshdr;
		elfio_attr_t elfio_attr;

		bool is_valid_elf(Elf32_Ehdr ehdr){
			char elf_mag[4] = {0x7f, 'E', 'L', 'F'};
			return (!strncmp(elf_mag, (char*)ehdr.e_ident, 4)) ? true : false;
		}

	public:
		elfio_ctrl_t(char* efile) : elfio_attr(ehdr){
			mphdr = mshdr = 0;
			file[0] = 0;
			fd = fopen(efile, "rb");
			if(!fd) throw"invalid file";
			fread(&ehdr, sizeof(ehdr), 1, fd);
			strcpy(file, efile);
			if(!is_valid_elf(ehdr)) throw "this is not ELF";
		}

		~elfio_ctrl_t(){
			if(fd) fclose(fd);
			if(mphdr) delete mphdr;
			if(mshdr) delete mshdr;
		}

		virtual elf_attr_t* get_attr(void){
			return &elfio_attr;
		}

		virtual elf_buffer_t* get_body(void){
			return 0;
		}

		virtual const char* category(void){
			return "elfml";
		}

		virtual const char* name(void){
			return file;
		}

		virtual const unsigned int get_child_num(void){
			return 2;
		}

		virtual elf_section_t* get_child(const int idx){
			switch(idx){
				case 0:
					if(!mphdr) mphdr = phdr_new(ehdr, fd);
					return mphdr;
					break;
				case 1:
					if(!mshdr) mshdr = shdr_new(ehdr, fd);
					return mshdr;
					break;
			}
			return 0;
		}

		virtual elf_section_t* get_child(const char* stridx){
			int idx = -1;
			if(!strcmp(stridx, "phdr"))	idx = 0;
			else if(!strcmp(stridx, "shdr")) idx = 1;
			return get_child(idx);
		}
};

elf_section_t* elfio_new(char* file){
	elfio_ctrl_t* elfio = new elfio_ctrl_t(file);
	if(!elfio->name()) {
		delete elfio;
		elfio = 0;
	}
	return elfio;
}

/*
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
*/
