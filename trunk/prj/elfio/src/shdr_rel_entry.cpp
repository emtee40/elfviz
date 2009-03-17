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

#include "elfio/elfio.h"
#include "elftypes.h"
#include "shdr_rel_entry.h"
#include "elf_defs.h"

section_attr_t shdr_rel_attr[] = {
	{"r_offset",	ELF_TYPE_INT | ELF_TYPE_HEX	},
	{"r_symbol",	ELF_TYPE_INT | ELF_TYPE_HEX	},
	{"r_type",	ELF_TYPE_INT | ELF_TYPE_STR	},
	{"r_info",	ELF_TYPE_INT			}
};

class shdr_rel_attr_abstract_t{
	public:
		shdr_rel_attr_abstract_t(const unsigned int i):num(i){ }

	protected:
		const unsigned int num;
		unsigned int ri;
		unsigned int ris;
		unsigned int rit;
		unsigned int rii;

		void invalidate(unsigned int info){
			ri = info;
			ris = ELF32_R_SYM(ri);
			rit = ELF32_R_TYPE(ri);
			rii = ELF32_R_INFO(ris, rit);
		}

		char* get_type_info(void){
			char* str = 0;
			switch(rit){
				case R_ARM_NONE:		str = (char*)"R_ARM_NONE";		break;
				case R_ARM_PC24:		str = (char*)"R_ARM_PC24";		break;
				case R_ARM_ABS32:		str = (char*)"R_ARM_ABS32";		break;
				case R_ARM_REL32:		str = (char*)"R_ARM_REL32";		break;
				case R_ARM_PC13:		str = (char*)"R_ARM_PC13";		break;
				case R_ARM_ABS16:		str = (char*)"R_ARM_ABS16";		break;
				case R_ARM_ABS12:		str = (char*)"R_ARM_ABS12";		break;
				case R_ARM_THM_ABS5:		str = (char*)"R_ARM_THM_ABS5";		break;
				case R_ARM_ABS8:		str = (char*)"R_ARM_ABS8";		break;
				case R_ARM_SBREL32:		str = (char*)"R_ARM_SBREL32";		break;
				case R_ARM_THM_PC22:		str = (char*)"R_ARM_THM_PC22";		break;
				case R_ARM_THM_PC8:		str = (char*)"R_ARM_THM_PC8";		break;
				case R_ARM_AMP_VCALL9:		str = (char*)"R_ARM_AMP_VCALL9";	break;
				case R_ARM_SWI24:		str = (char*)"R_ARM_SWI24";		break;
				case R_ARM_THM_SWI8:		str = (char*)"R_ARM_THM_SWI8";		break;
				case R_ARM_XPC25:		str = (char*)"R_ARM_XPC25";		break;
				case R_ARM_THM_XPC22:		str = (char*)"R_ARM_THM_XPC22";	break;
				case R_ARM_COPY:		str = (char*)"R_ARM_COPY";		break;
				case R_ARM_GLOB_DAT:		str = (char*)"R_ARM_GLOB_DAT";		break;
				case R_ARM_JUMP_SLOT:		str = (char*)"R_ARM_JUMP_SLOT";	break;
				case R_ARM_RELATIVE:		str = (char*)"R_ARM_RELATIVE";		break;
				case R_ARM_GOTOFF:		str = (char*)"R_ARM_GOTOFF";		break;
				case R_ARM_GOTPC:		str = (char*)"R_ARM_GOTPC";		break;
				case R_ARM_GOT32:		str = (char*)"R_ARM_GOT32";		break;
				case R_ARM_PLT32:		str = (char*)"R_ARM_PLT32";		break;
				case R_ARM_ALU_PCREL_7_0:	str = (char*)"R_ARM_ALU_PCREL_7_0";	break;
				case R_ARM_ALU_PCREL_15_8:	str = (char*)"R_ARM_ALU_PCREL_15_8";	break;
				case R_ARM_ALU_PCREL_23_15:	str = (char*)"R_ARM_ALU_PCREL_23_15";	break;
				case R_ARM_LDR_SBREL_11_0:	str = (char*)"R_ARM_LDR_SBREL_11_0";	break;
				case R_ARM_ALU_SBREL_19_12:	str = (char*)"R_ARM_ALU_SBREL_19_12";	break;
				case R_ARM_ALU_SBREL_27_20:	str = (char*)"R_ARM_ALU_SBREL_27_20";	break;
				case R_ARM_GNU_VTENTRY:		str = (char*)"R_ARM_GNU_VTENTRY";	break;
				case R_ARM_GNU_VTINHERIT:	str = (char*)"R_ARM_GNU_VTINHERIT";	break;
				case R_ARM_THM_PC11:		str = (char*)"R_ARM_THM_PC11";		break;
				case R_ARM_THM_PC9:		str = (char*)"R_ARM_THM_PC9";		break;
				case R_ARM_RXPC25:		str = (char*)"R_ARM_RXPC25";		break;
				case R_ARM_RSBREL32:		str = (char*)"R_ARM_RSBREL32";		break;
				case R_ARM_THM_RPC22:		str = (char*)"R_ARM_THM_RPC22";		break;
				case R_ARM_RREL32:		str = (char*)"R_ARM_RREL32";		break;
				case R_ARM_RABS32:		str = (char*)"R_ARM_RABS32";		break;
				case R_ARM_RPC24:		str = (char*)"R_ARM_RPC24";		break;
				case R_ARM_RBASE:		str = (char*)"R_ARM_RBASE";		break;
			}
			return str;
		}
};

class shdr_rel_attr_t:public shdr_rel_attr_abstract_t, public elf_attr_t{
	public:
		shdr_rel_attr_t(Elf32_Rel& shdr):hdr(shdr), shdr_rel_attr_abstract_t(sizeof(shdr_rel_attr) / sizeof(section_attr_t)){ }

		virtual const unsigned int get_num(void){
			return num;
		}

		virtual const unsigned int get_type(int idx){
			return shdr_rel_attr[idx].type;
		}

		virtual const char* get_name(int idx){
			return shdr_rel_attr[idx].name;
		}

		virtual const char* get_str(int idx){
			char* ret = 0;
			switch(idx){
				case 2:	ret = get_type_info();	break;
				default:	throw "invalid argument";	break;
			}
			return ret;
		}

		virtual const int get_int(int idx){
			unsigned int ret = 0;
			invalidate(hdr.r_info);
			switch(idx){
				case 0:	ret = hdr.r_offset;	break;
				case 1:	ret = ris;		break;
				case 2:	ret = rit;		break;
				case 3:	ret = rii;		break;
				default:	throw "invalid argument";	break;
			}
			return ret;
		}

	private:
		Elf32_Rel& hdr;
};

class elf_shdr_rel_entry_t : public elf_section_t{
	protected:
		Elf32_Rel reltab;
		shdr_rel_attr_t attr;

	public:
		elf_shdr_rel_entry_t(Elf32_Rel ereltab):attr(reltab){
			memcpy(&reltab, &ereltab, sizeof(Elf32_Rel));
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
			return "entry";
		}

		virtual const char* name(void){
			return "null";
		}
};

elf_section_t* shdr_rel_entry_new(FILE* fd, unsigned int sh_offset, char* strtab){
	Elf32_Rel reltab;
	fseek(fd, sh_offset, SEEK_SET);
	fread(&reltab, sizeof(Elf32_Rel), 1, fd);
	return (elf_section_t*) new elf_shdr_rel_entry_t(reltab);
}

/*========================================================================================================*/
section_attr_t shdr_rela_attr[] = {
	{"r_offset",	ELF_TYPE_INT			},
	{"r_symbol",	ELF_TYPE_INT			},
	{"r_type",	ELF_TYPE_INT | ELF_TYPE_STR	},
	{"r_info",	ELF_TYPE_INT			},
	{"r_addend",	ELF_TYPE_INT			}
};

class shdr_rela_attr_t:public shdr_rel_attr_abstract_t, public elf_attr_t{
	public:
		shdr_rela_attr_t(Elf32_Rela& shdr):hdr(shdr), shdr_rel_attr_abstract_t(sizeof(shdr_rela_attr) / sizeof(section_attr_t)){ }

		virtual const unsigned int get_num(void){
			return num;
		}

		virtual const unsigned int get_type(int idx){
			return shdr_rela_attr[idx].type;
		}

		virtual const char* get_name(int idx){
			return shdr_rela_attr[idx].name;
		}

		virtual const char* get_str(int idx){
			char* ret = 0;
			switch(idx){
				case 2:	ret = get_type_info();	break;
				default:	throw "invalid argument";	break;
			}
			return ret;
		}

		virtual const int get_int(int idx){
			unsigned int ret = 0;
			invalidate(hdr.r_info);
			switch(idx){
				case 0:	ret = hdr.r_offset;	break;
				case 1:	ret = ris;		break;
				case 2:	ret = rit;		break;
				case 3:	ret = rii;		break;
				case 4:	ret = hdr.r_addend;	break;
				default:	throw "invalid argument";	break;
			}
			return ret;
		}

	private:
		Elf32_Rela& hdr;
};

class elf_shdr_rela_entry_t : public elf_section_t{
	private:
		Elf32_Rela relatab;
		shdr_rela_attr_t attra;

	public:
		elf_shdr_rela_entry_t(Elf32_Rela erelatab):attra(relatab){
			memcpy(&relatab, &erelatab, sizeof(Elf32_Rela));
		}

		virtual elf_attr_t* get_attr(void){
			return &attra;
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
			return "entry";
		}

		virtual const char* name(void){
			return "null";
		}
};

elf_section_t* shdr_rela_entry_new(FILE* fd, unsigned int sh_offset, char* strtab){
	Elf32_Rela relatab;
	fseek(fd, sh_offset, SEEK_SET);
	fread(&relatab, sizeof(Elf32_Rela), 1, fd);
	return (elf_section_t*) new elf_shdr_rela_entry_t(relatab);
}
