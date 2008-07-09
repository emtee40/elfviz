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

#include "elfio/elfio.h"
#include "elftypes.h"
#include "shdr_symtab_entry.h"

typedef struct _elf_shdr_symtab_entry_t : public elf_section_t{
	protected:
		Elf32_Sym symtab;
		char* ss_name;

	public:
		_elf_shdr_symtab_entry_t(Elf32_Sym esymtab, char* ename){
			memcpy(&symtab, &esymtab, sizeof(Elf32_Sym));
			ss_name = strdup(ename);
		}

		~_elf_shdr_symtab_entry_t(){
			delete ss_name;
		}

		virtual void format(FILE* fd){
			unsigned int sti = 0;
			char* str = 0;
			if(ss_name) fprintf(fd, "st_name_str=%s\n", ss_name);
			fprintf(fd, "st_name=%d\n", symtab.st_name);
			fprintf(fd, "st_vale=%d\n", symtab.st_value);
			fprintf(fd, "st_size=%d\n", symtab.st_size);
			fprintf(fd, "st_info=0x%x\n", symtab.st_info);
			sti = symtab.st_info;
			sti = ELF32_ST_BIND(sti);
			fprintf(fd, "st_bind=");
			switch(sti){
				case STB_LOCAL:		str = "STB_LOCAL";	break;
				case STB_GLOBAL:	str = "STB_GLOBAL";	break;
				case STB_WEAK:		str = "STB_WEAK";	break;
				case STB_LOPROC:	str = "STB_LOPROC";	break;
				case STB_HIPROC:	str = "STB_HIPROC";	break;
			}
			fprintf(fd, "%s\n", str);
			sti = symtab.st_info;
			sti = ELF32_ST_TYPE(sti);
			fprintf(fd, "st_type=");
			switch(sti){
				case STT_NOTYPE:	str = "STT_NOTYPE";		break;
				case STT_OBJECT:	str = "STT_OBJECT";		break;
				case STT_FUNC:		str = "STT_FUNC";		break;
				case STT_SECTION:	str = "STT_SECTION";	break;
				case STT_FILE:		str = "STT_FILE";		break;
				case STT_LOPROC:	str = "STT_LOPROC";		break;
				case STT_HIPROC:	str = "STT_HIPROC";		break;
			}
			fprintf(fd, "%s\n", str);
			fprintf(fd, "st_other=0x%x\n", symtab.st_other);
			fprintf(fd, "st_shndx=%d\n", symtab.st_shndx);
			fprintf(fd, "\n");
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
			return ss_name;
		}
}elf_shdr_symtab_entry_t;

elf_section_t* shdr_symtab_entry_new(FILE* fd, unsigned int sh_offset, char* strtab){
	Elf32_Sym symtab;
	fseek(fd, sh_offset, SEEK_SET);
	fread(&symtab, sizeof(Elf32_Sym), 1, fd);
	return (elf_section_t*) new elf_shdr_symtab_entry_t(symtab, symtab.st_name + strtab);
}
