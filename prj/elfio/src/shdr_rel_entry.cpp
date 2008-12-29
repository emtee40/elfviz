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

class elf_shdr_rel_entry_t : public elf_section_t{
	protected:
		Elf32_Rel reltab;

		void format_header_offset(int offset){
			printf("r_offset=%d\n", offset);
		}
		void format_header_info(int info){
			unsigned int ri = 0, ris = 0, rit = 0, rii = 0;
			char* str = 0;
			ri = info;
			ris = ELF32_R_SYM(ri);
			rit = ELF32_R_TYPE(ri);
			rii = ELF32_R_INFO(ris, rit);
			printf("R_SYM=%d\n", ris);
			printf("R_TYPE=");
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
				case R_ARM_THM_RPC22:		str = (char*)"R_ARM_THM_RPC22";	break;
				case R_ARM_RREL32:		str = (char*)"R_ARM_RREL32";		break;
				case R_ARM_RABS32:		str = (char*)"R_ARM_RABS32";		break;
				case R_ARM_RPC24:		str = (char*)"R_ARM_RPC24";		break;
				case R_ARM_RBASE:		str = (char*)"R_ARM_RBASE";		break;
			}
			printf("%s\n", str);
			printf("R_INFO=%d\n", rii);
			printf("\n");
		}
	public:
		elf_shdr_rel_entry_t(){}

		elf_shdr_rel_entry_t(Elf32_Rel ereltab){
			memcpy(&reltab, &ereltab, sizeof(Elf32_Rel));
		}

		virtual void format_header(void){
			format_header_offset(reltab.r_offset);
			format_header_info(reltab.r_info);
		}

		virtual void format_body(void){
			printf("no data\n");
		}

		virtual void format_child(void){
			printf(".\t..\n");
		}

		virtual elf_section_t* get_child(const unsigned int idx){
			return 0;
		}

		virtual elf_section_t* get_child(const char* stridx){
			return 0;
		}

		virtual const unsigned char* get_body(void){
			return 0;
		}

		virtual const char* name(void){
			return 0;
		}
};

class elf_shdr_rela_entry_t : public elf_shdr_rel_entry_t{
	protected:
		Elf32_Rela relatab;
		void format_header_addend(int addend){
			printf("r_addend=%d\n", addend);
		}
	public:
		elf_shdr_rela_entry_t(Elf32_Rela erelatab){
			memcpy(&relatab, &erelatab, sizeof(Elf32_Rela));
		}

		virtual void format_header(void){
			format_header_offset(relatab.r_offset);
			format_header_info(relatab.r_info);
			format_header_addend(relatab.r_addend);
		}
};

elf_section_t* shdr_rel_entry_new(FILE* fd, unsigned int sh_offset, char* strtab){
	Elf32_Rel reltab;
	fseek(fd, sh_offset, SEEK_SET);
	fread(&reltab, sizeof(Elf32_Rel), 1, fd);
	return (elf_section_t*) new elf_shdr_rel_entry_t(reltab);
}

elf_section_t* shdr_rela_entry_new(FILE* fd, unsigned int sh_offset, char* strtab){
	Elf32_Rela relatab;
	fseek(fd, sh_offset, SEEK_SET);
	fread(&relatab, sizeof(Elf32_Rela), 1, fd);
	return (elf_section_t*) new elf_shdr_rela_entry_t(relatab);
}
