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

class elf_shdr_symtab_entry_t : public elf_section_t{
	protected:
		Elf32_Sym symtab;
		char* ss_name;

	public:
		elf_shdr_symtab_entry_t(Elf32_Sym esymtab, char* ename){
			memcpy(&symtab, &esymtab, sizeof(Elf32_Sym));
			ss_name = strdup(ename);
		}

		~elf_shdr_symtab_entry_t(){
			delete ss_name;
		}

		virtual void format_header(void){
			unsigned int sti = 0;
			char* str = 0;
			if(ss_name) printf("st_name_str=%s\n", ss_name);
			printf("st_name=%d\n", (int)symtab.st_name);
			printf("st_vale=%d\n", (int)symtab.st_value);
			printf("st_size=%d\n", (int)symtab.st_size);
			printf("st_info=0x%x\n", symtab.st_info);
			sti = symtab.st_info;
			sti = ELF32_ST_BIND(sti);
			printf("st_bind=");
			switch(sti){
				case STB_LOCAL:		str = (char*)"STB_LOCAL";	break;
				case STB_GLOBAL:	str = (char*)"STB_GLOBAL";	break;
				case STB_WEAK:		str = (char*)"STB_WEAK";	break;
				case STB_LOPROC:	str = (char*)"STB_LOPROC";	break;
				case STB_HIPROC:	str = (char*)"STB_HIPROC";	break;
			}
			printf("%s\n", str);
			sti = symtab.st_info;
			sti = ELF32_ST_TYPE(sti);
			printf("st_type=");
			switch(sti){
				case STT_NOTYPE:	str = (char*)"STT_NOTYPE";	break;
				case STT_OBJECT:	str = (char*)"STT_OBJECT";	break;
				case STT_FUNC:		str = (char*)"STT_FUNC";	break;
				case STT_SECTION:	str = (char*)"STT_SECTION";	break;
				case STT_FILE:		str = (char*)"STT_FILE";	break;
				case STT_LOPROC:	str = (char*)"STT_LOPROC";	break;
				case STT_HIPROC:	str = (char*)"STT_HIPROC";	break;
			}
			printf("%s\n", str);
			printf("st_other=0x%x\n", symtab.st_other);
			printf("st_shndx=%d\n", symtab.st_shndx);
			printf("\n");
		}

		virtual void format_body(void){
			printf("no data\n");
		}

		virtual void format_child(void){
			printf("no children\n");
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
			return ss_name;
		}
};

elf_section_t* shdr_symtab_entry_new(FILE* fd, unsigned int sh_offset, char* strtab){
	Elf32_Sym symtab;
	fseek(fd, sh_offset, SEEK_SET);
	fread(&symtab, sizeof(Elf32_Sym), 1, fd);
	return (elf_section_t*) new elf_shdr_symtab_entry_t(symtab, symtab.st_name + strtab);
}
