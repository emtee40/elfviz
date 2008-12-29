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

class elf_phdr_entry_t : public elf_section_t{
	protected:
		Elf32_Phdr phdr;

		void type_str(int p_type){
			char* str = (char*)"unknown";
			printf("p_type=");
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
			printf("%s\n", str);
		}

		void offset_str(int p_offset){
			printf("p_offset=0x%x\n", p_offset);
		}

		void phdr_entry_vaddr_str(int p_vaddr){
			printf("p_vaddr=0x%x\n", p_vaddr);
		}

		void vaddr_str(int p_vaddr){
			printf("p_vaddr=0x%x\n", p_vaddr);
		}

		void paddr_str(int p_paddr){
			printf("p_paddr=0x%x\n", p_paddr);
		}

		void filesz_str(int p_filesz){
			printf("p_filesz=%d\n", p_filesz);
		}

		void memsz_str(int p_memsz){
			printf("p_memsz=0x%x\n", p_memsz);
		}

		void flags_str(int p_flags){
			printf("p_flags=");
			if(p_flags & PF_X)	printf("PF_X | ");
			if(p_flags & PF_W)	printf("PF_W | ");
			if(p_flags & PF_R)	printf("PF_R");
			printf("\n");
		}

		void align_str(int p_align){
			printf("p_align=%d\n", p_align);
		}

	public:
		elf_phdr_entry_t(FILE* fd, int phoff){
			fseek(fd, phoff, SEEK_SET);
			fread(&(phdr), sizeof(Elf32_Phdr), 1, fd);
		}

		virtual void format_body(void){
			printf("no date\n");
		}

		virtual void format_child(void){
			printf(".\t..\n");
		}

		virtual void format_header(void){
			type_str(phdr.p_type);
			offset_str(phdr.p_offset);
			vaddr_str(phdr.p_vaddr);
			paddr_str(phdr.p_paddr);
			filesz_str(phdr.p_filesz);
			memsz_str(phdr.p_memsz);
			flags_str(phdr.p_flags);
			align_str(phdr.p_align);
			printf("\n");
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

elf_section_t* phdr_entry_new(FILE* fd, int phoff){
	return (elf_section_t*) new elf_phdr_entry_t(fd, phoff);

}
