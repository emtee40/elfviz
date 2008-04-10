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
#include "phdr_entry.h"

typedef class _elf_phdr_entry_t : public elf_section_t{
	protected:
		Elf32_Phdr phdr;

		void type_str(rt_file_t fd, int p_type){
			char* str = "unknown";
			rt_fprint(fd, "p_type=");
			switch(p_type){
			case PT_NULL:		str = "PT_NULL";	break;
			case PT_LOAD:		str = "PT_LOAD";	break;
			case PT_DYNAMIC:	str = "PT_DYNAMIC";	break;
			case PT_INTERP:		str = "PT_INTERP";	break;
			case PT_NOTE:		str = "PT_NOTE";	break;
			case PT_SHLIB:		str = "PT_SHLIB";	break;
			case PT_PHDR:		str = "PT_PHDR";	break;
			case PT_LOPROC:		str = "PT_LOPROC";	break;
			case PT_HIPROC:		str = "PT_HIPROC";	break;
			}
			rt_fprint(fd, "%s\n", str);
		}

		void offset_str(rt_file_t fd, int p_offset){
			rt_fprint(fd, "p_offset=0x%x\n", p_offset);
		}

		void phdr_entry_vaddr_str(rt_file_t fd, int p_vaddr){
			rt_fprint(fd, "p_vaddr=0x%x\n", p_vaddr);
		}

		void vaddr_str(rt_file_t fd, int p_vaddr){
			rt_fprint(fd, "p_vaddr=0x%x\n", p_vaddr);
		}

		void paddr_str(rt_file_t fd, int p_paddr){
			rt_fprint(fd, "p_paddr=0x%x\n", p_paddr);
		}

		void filesz_str(rt_file_t fd, int p_filesz){
			rt_fprint(fd, "p_filesz=%d\n", p_filesz);
		}

		void memsz_str(rt_file_t fd, int p_memsz){
			rt_fprint(fd, "p_memsz=0x%x\n", p_memsz);
		}

		void flags_str(rt_file_t fd, int p_flags){
			rt_fprint(fd, "p_flags=");
			if(p_flags & PF_X)	rt_fprint(fd, "PF_X | ");
			if(p_flags & PF_W)	rt_fprint(fd, "PF_W | ");
			if(p_flags & PF_R)	rt_fprint(fd, "PF_R");
			rt_fprint(fd, "\n");
		}

		void align_str(rt_file_t fd, int p_align){
			rt_fprint(fd, "p_align=%d\n", p_align);
		}

	public:
		_elf_phdr_entry_t(rt_file_t fd, int phoff){
			rt_fseek(fd, phoff, RT_FILE_SEEK_SET);
			rt_fread(fd, &(phdr), sizeof(Elf32_Phdr));
		}

		virtual void dump(rt_file_t fd){
			rt_fprint(fd, "no date\n");
		}

		virtual void format(rt_file_t fd){
			type_str(fd, phdr.p_type);
			offset_str(fd, phdr.p_offset);
			vaddr_str(fd, phdr.p_vaddr);
			paddr_str(fd, phdr.p_paddr);
			filesz_str(fd, phdr.p_filesz);
			memsz_str(fd, phdr.p_memsz);
			flags_str(fd, phdr.p_flags);
			align_str(fd, phdr.p_align);
			rt_fprint(fd, "\n");
		}

		virtual elf_section_t* get_sub(const int idx){
			return cnull;
		}

		virtual elf_section_t* find_sub(const char* stridx){
			return cnull;
		}

		virtual const cbyte* data(void){
			return cnull;
		}

		virtual const char* name(void){
			return cnull;
		}

}elf_phdr_entry_t;

elf_section_t* phdr_entry_new(rt_file_t fd, int phoff){
	return (elf_section_t*) new elf_phdr_entry_t(fd, phoff);

}
