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
#include "shdr_symtab_entry.h"

typedef struct _elf_shdr_symtab_entry_t : public elf_section_t{
	protected:
		Elf32_Sym symtab;
		char* ss_name;

	public:
		_elf_shdr_symtab_entry_t(Elf32_Sym esymtab, char* ename){
			rt_memcpy(&symtab, &esymtab, sizeof(Elf32_Sym));
			ss_name = rt_strdup(ename);
		}

		~_elf_shdr_symtab_entry_t(){
			rt_delete(ss_name);
		}

		virtual void format(rt_file_t fd){
			unsigned int sti = 0;
			char* str = cnull;
			if(ss_name) rt_fprint(fd, "st_name_str=%s\n", ss_name);
			rt_fprint(fd, "st_name=%d\n", symtab.st_name);
			rt_fprint(fd, "st_vale=%d\n", symtab.st_value);
			rt_fprint(fd, "st_size=%d\n", symtab.st_size);
			rt_fprint(fd, "st_info=0x%x\n", symtab.st_info);
			sti = symtab.st_info;
			sti = ELF32_ST_BIND(sti);
			rt_fprint(fd, "st_bind=");
			switch(sti){
				case STB_LOCAL:		str = "STB_LOCAL";	break;
				case STB_GLOBAL:	str = "STB_GLOBAL";	break;
				case STB_WEAK:		str = "STB_WEAK";	break;
				case STB_LOPROC:	str = "STB_LOPROC";	break;
				case STB_HIPROC:	str = "STB_HIPROC";	break;
			}
			rt_fprint(fd, "%s\n", str);
			sti = symtab.st_info;
			sti = ELF32_ST_TYPE(sti);
			rt_fprint(fd, "st_type=");
			switch(sti){
				case STT_NOTYPE:	str = "STT_NOTYPE";		break;
				case STT_OBJECT:	str = "STT_OBJECT";		break;
				case STT_FUNC:		str = "STT_FUNC";		break;
				case STT_SECTION:	str = "STT_SECTION";	break;
				case STT_FILE:		str = "STT_FILE";		break;
				case STT_LOPROC:	str = "STT_LOPROC";		break;
				case STT_HIPROC:	str = "STT_HIPROC";		break;
			}
			rt_fprint(fd, "%s\n", str);
			rt_fprint(fd, "st_other=0x%x\n", symtab.st_other);
			rt_fprint(fd, "st_shndx=%d\n", symtab.st_shndx);
			rt_fprint(fd, "\n");
		}

		virtual void dump(rt_file_t fd){
			rt_fprint(fd, "no data\n");
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
			return ss_name;
		}
}elf_shdr_symtab_entry_t;

elf_section_t* shdr_symtab_entry_new(rt_file_t fd, unsigned int sh_offset, char* strtab){
	Elf32_Sym symtab;
	rt_fseek(fd, sh_offset, RT_FILE_SEEK_SET);
	rt_fread(fd, &symtab, sizeof(Elf32_Sym));
	return (elf_section_t*) new elf_shdr_symtab_entry_t(symtab, symtab.st_name + strtab);
}
