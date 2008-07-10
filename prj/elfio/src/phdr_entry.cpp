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

#include "elfio/elfio.h"
#include "elftypes.h"
#include "phdr_entry.h"

typedef class _elf_phdr_entry_t : public elf_section_t{
	protected:
		Elf32_Phdr phdr;

		void type_str(FILE* fd, int p_type){
			char* str = (char*)"unknown";
			fprintf(fd, "p_type=");
			switch(p_type){
			case PT_NULL:		str = (char*)"PT_NULL";	break;
			case PT_LOAD:		str = (char*)"PT_LOAD";	break;
			case PT_DYNAMIC:	str = (char*)"PT_DYNAMIC";	break;
			case PT_INTERP:		str = (char*)"PT_INTERP";	break;
			case PT_NOTE:		str = (char*)"PT_NOTE";	break;
			case PT_SHLIB:		str = (char*)"PT_SHLIB";	break;
			case PT_PHDR:		str = (char*)"PT_PHDR";	break;
			case PT_LOPROC:		str = (char*)"PT_LOPROC";	break;
			case PT_HIPROC:		str = (char*)"PT_HIPROC";	break;
			}
			fprintf(fd, "%s\n", str);
		}

		void offset_str(FILE* fd, int p_offset){
			fprintf(fd, "p_offset=0x%x\n", p_offset);
		}

		void phdr_entry_vaddr_str(FILE* fd, int p_vaddr){
			fprintf(fd, "p_vaddr=0x%x\n", p_vaddr);
		}

		void vaddr_str(FILE* fd, int p_vaddr){
			fprintf(fd, "p_vaddr=0x%x\n", p_vaddr);
		}

		void paddr_str(FILE* fd, int p_paddr){
			fprintf(fd, "p_paddr=0x%x\n", p_paddr);
		}

		void filesz_str(FILE* fd, int p_filesz){
			fprintf(fd, "p_filesz=%d\n", p_filesz);
		}

		void memsz_str(FILE* fd, int p_memsz){
			fprintf(fd, "p_memsz=0x%x\n", p_memsz);
		}

		void flags_str(FILE* fd, int p_flags){
			fprintf(fd, "p_flags=");
			if(p_flags & PF_X)	fprintf(fd, "PF_X | ");
			if(p_flags & PF_W)	fprintf(fd, "PF_W | ");
			if(p_flags & PF_R)	fprintf(fd, "PF_R");
			fprintf(fd, "\n");
		}

		void align_str(FILE* fd, int p_align){
			fprintf(fd, "p_align=%d\n", p_align);
		}

	public:
		_elf_phdr_entry_t(FILE* fd, int phoff){
			fseek(fd, phoff, SEEK_SET);
			fread(&(phdr), sizeof(Elf32_Phdr), 1, fd);
		}

		virtual void dump(FILE* fd){
			fprintf(fd, "no date\n");
		}

		virtual void format(FILE* fd){
			type_str(fd, phdr.p_type);
			offset_str(fd, phdr.p_offset);
			vaddr_str(fd, phdr.p_vaddr);
			paddr_str(fd, phdr.p_paddr);
			filesz_str(fd, phdr.p_filesz);
			memsz_str(fd, phdr.p_memsz);
			flags_str(fd, phdr.p_flags);
			align_str(fd, phdr.p_align);
			fprintf(fd, "\n");
		}

		virtual elf_section_t* get_sub(const unsigned int idx){
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

}elf_phdr_entry_t;

elf_section_t* phdr_entry_new(FILE* fd, int phoff){
	return (elf_section_t*) new elf_phdr_entry_t(fd, phoff);

}
