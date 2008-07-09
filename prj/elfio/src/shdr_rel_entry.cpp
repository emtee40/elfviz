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

typedef struct _elf_shdr_rel_entry_t : public elf_section_t{
	protected:
		Elf32_Rel reltab;

		void format_offset(FILE* fd, int offset){
			fprintf(fd, "r_offset=%d\n", offset);
		}
		void format_info(FILE* fd, int info){
			unsigned int ri = 0, ris = 0, rit = 0, rii = 0;
			char* str = 0;
			ri = info;
			ris = ELF32_R_SYM(ri);
			rit = ELF32_R_TYPE(ri);
			rii = ELF32_R_INFO(ris, rit);
			fprintf(fd, "R_SYM=%d\n", ris);
			fprintf(fd, "R_TYPE=", rit);
			switch(ris){
				case R_ARM_NONE:		str = "R_ARM_NONE";		break;
				case R_ARM_PC24:		str = "R_ARM_PC24";		break;
				case R_ARM_ABS32:		str = "R_ARM_ABS32";		break;
				case R_ARM_REL32:		str = "R_ARM_REL32";		break;
				case R_ARM_PC13:		str = "R_ARM_PC13";		break;
				case R_ARM_ABS16:		str = "R_ARM_ABS16";		break;
				case R_ARM_ABS12:		str = "R_ARM_ABS12";		break;
				case R_ARM_THM_ABS5:		str = "R_ARM_THM_ABS5";		break;
				case R_ARM_ABS8:		str = "R_ARM_ABS8";		break;
				case R_ARM_SBREL32:		str = "R_ARM_SBREL32";		break;
				case R_ARM_THM_PC22:		str = "R_ARM_THM_PC22";		break;
				case R_ARM_THM_PC8:		str = "R_ARM_THM_PC8";		break;
				case R_ARM_AMP_VCALL9:		str = "R_ARM_AMP_VCALL9";	break;
				case R_ARM_SWI24:		str = "R_ARM_SWI24";		break;
				case R_ARM_THM_SWI8:		str = "R_ARM_THM_SWI8";		break;
				case R_ARM_XPC25:		str = "R_ARM_XPC25";		break;
				case R_ARM_THM_XPC22:		str = "R_ARM_THM_XPC22";	break;
				case R_ARM_COPY:		str = "R_ARM_COPY";		break;
				case R_ARM_GLOB_DAT:		str = "R_ARM_GLOB_DAT";		break;
				case R_ARM_JUMP_SLOT:		str = "R_ARM_JUMP_SLOT";	break;
				case R_ARM_RELATIVE:		str = "R_ARM_RELATIVE";		break;
				case R_ARM_GOTOFF:		str = "R_ARM_GOTOFF";		break;
				case R_ARM_GOTPC:		str = "R_ARM_GOTPC";		break;
				case R_ARM_GOT32:		str = "R_ARM_GOT32";		break;
				case R_ARM_PLT32:		str = "R_ARM_PLT32";		break;
				case R_ARM_ALU_PCREL_7_0:	str = "R_ARM_ALU_PCREL_7_0";	break;
				case R_ARM_ALU_PCREL_15_8:	str = "R_ARM_ALU_PCREL_15_8";	break;
				case R_ARM_ALU_PCREL_23_15:	str = "R_ARM_ALU_PCREL_23_15";	break;
				case R_ARM_LDR_SBREL_11_0:	str = "R_ARM_LDR_SBREL_11_0";	break;
				case R_ARM_ALU_SBREL_19_12:	str = "R_ARM_ALU_SBREL_19_12";	break;
				case R_ARM_ALU_SBREL_27_20:	str = "R_ARM_ALU_SBREL_27_20";	break;
				case R_ARM_GNU_VTENTRY:		str = "R_ARM_GNU_VTENTRY";	break;
				case R_ARM_GNU_VTINHERIT:	str = "R_ARM_GNU_VTINHERIT";	break;
				case R_ARM_THM_PC11:		str = "R_ARM_THM_PC11";		break;
				case R_ARM_THM_PC9:		str = "R_ARM_THM_PC9";		break;
				case R_ARM_RXPC25:		str = "R_ARM_RXPC25";		break;
				case R_ARM_RSBREL32:		str = "R_ARM_RSBREL32";		break;
				case R_ARM_THM_RPC22:		str = "R_ARM_THM_RPC22";	break;
				case R_ARM_RREL32:		str = "R_ARM_RREL32";		break;
				case R_ARM_RABS32:		str = "R_ARM_RABS32";		break;
				case R_ARM_RPC24:		str = "R_ARM_RPC24";		break;
				case R_ARM_RBASE:		str = "R_ARM_RBASE";		break;
			}
			fprintf(fd, "%s\n", str);
			fprintf(fd, "R_INFO=%d\n", rii);
			fprintf(fd, "\n");
		}
	public:
		_elf_shdr_rel_entry_t(){}

		_elf_shdr_rel_entry_t(Elf32_Rel ereltab){
			memcpy(&reltab, &ereltab, sizeof(Elf32_Rel));
		}

		virtual void format(FILE* fd){
			format_offset(fd, reltab.r_offset);
			format_info(fd, reltab.r_info);
		}

		virtual void dump(FILE* fd){
			fprintf(fd, "no data\n");
		}

		virtual elf_section_t* get_sub(const int idx){
			return 0;
		}

		virtual elf_section_t* find_sub(const char* stridx){
			return 0;
		}

		virtual const unsigned char* data(void){
			return 0;
		}

		virtual const char* name(void){
			return 0;
		}
}elf_shdr_rel_entry_t;

typedef struct _elf_shdr_rela_entry_t : public elf_shdr_rel_entry_t{
	protected:
		Elf32_Rela relatab;
		void format_addend(FILE* fd, int addend){
			fprintf(fd, "r_addend=%d\n", addend);
		}
	public:
		_elf_shdr_rela_entry_t(Elf32_Rela erelatab){
			memcpy(&relatab, &erelatab, sizeof(Elf32_Rela));
		}

		virtual void format(FILE* fd){
			format_offset(fd, relatab.r_offset);
			format_info(fd, relatab.r_info);
			format_addend(fd, relatab.r_addend);
		}
}elf_shdr_rela_entry_t;

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
