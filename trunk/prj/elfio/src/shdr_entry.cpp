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
#include <ctype.h>

#include "elfio/elfio.h"
#include "elftypes.h"
#include "shdr_entry.h"
#include "shdr_symtab_entry.h"
#include "shdr_rel_entry.h"

class elf_shdr_entry_t : public elf_section_t{
	protected:
		Elf32_Shdr shdr;
		unsigned char* buf;
		char* se_name;

		void name_str(int sh_name){
			printf("sh_name=%d\n", sh_name);
		}

		void type_str(int sh_type){
			char* str = (char*)"unknown";
			printf("sh_type=");
			switch(sh_type){
			case SHT_NULL:		str = (char*)"SHT_NULL";		break;
			case SHT_PROGBITS:	str = (char*)"SHT_PROGBITS";	break;
			case SHT_SYMTAB:	str = (char*)"SHT_SYMTAB";		break;
			case SHT_STRTAB:	str = (char*)"SHT_STRTAB";		break;
			case SHT_RELA:		str = (char*)"SHT_RELA";		break;
			case SHT_HASH:		str = (char*)"SHT_HASH";		break;
			case SHT_DYNAMIC:	str = (char*)"SHT_DYNAMIC";	break;
			case SHT_NOTE:		str = (char*)"SHT_NOTE";		break;
			case SHT_NOBITS:	str = (char*)"SHT_NOBITS";		break;
			case SHT_REL:		str = (char*)"SHT_REL";		break;
			case SHT_SHLIB:		str = (char*)"SHT_SHLIB";		break;
			case SHT_DYNSYM:	str = (char*)"SHT_DYNSYM";		break;
			case SHT_LOPROC:	str = (char*)"SHT_LOPROC";		break;
			case SHT_HIPROC:	str = (char*)"SHT_HIPROC";		break;
			case SHT_LOUSER:	str = (char*)"SHT_LOUSER";		break;
			case SHT_HIUSER:	str = (char*)"SHT_HIUSER";		break;
			}
			printf("%s\n", str);
		}

		void offset_str(int sh_offset){
			printf("sh_offset=0x%x\n", sh_offset);
		}

		void addr_str(int sh_addr){
			printf("sh_addr=0x%x\n", sh_addr);
		}

		void size_str(int sh_size){
			printf("sh_size=%d\n", sh_size);
		}

		void entsize_str(int sh_entsize){
			printf("sh_entsize=%d\n", sh_entsize);
		}

		void link_str(int sh_link){
			printf("sh_link=%d\n", sh_link);
		}

		void info_str(int sh_info){
			printf("sh_info=0x%x\n", sh_info);
		}

		void flags_str(int sh_flags){
			printf("sh_flags=");
			if(sh_flags & SHF_WRITE)		printf("SHF_WRITE | ");
			if(sh_flags & SHF_ALLOC)		printf("SHF_ALLOC | ");
			if(sh_flags & SHF_EXECINSTR)	printf("SHF_EXEINSTR | ");
			if(sh_flags & SHF_MASKPROC)		printf("SHF_MASKPROC");
			printf("\n");
		}

		void align_str(int sh_addralign){
			printf("sh_addralign=%d\n", sh_addralign);
		}

	public:
		elf_shdr_entry_t(){}
		elf_shdr_entry_t(Elf32_Shdr eshdr, unsigned char* ebuf, char* ename){
			memcpy(&shdr, &eshdr, sizeof(Elf32_Shdr));
			buf = ebuf;
			if(ename) se_name = strdup(ename);
		}

		~elf_shdr_entry_t(){
			if(buf) delete buf;
			if(se_name) delete se_name;
		}

		virtual void format_header(void){
			if(se_name) printf("sh_name_str=%s\n", se_name);
			name_str(shdr.sh_name);
			type_str(shdr.sh_type);
			flags_str(shdr.sh_flags);
			addr_str(shdr.sh_addr);
			offset_str(shdr.sh_offset);
			size_str(shdr.sh_size);
			link_str(shdr.sh_link);
			info_str(shdr.sh_info);
			align_str(shdr.sh_addralign);
			entsize_str(shdr.sh_entsize);
			printf("\n");
		}

		#define SHDR_COLUMN_SIZE 16
		virtual void format_body(void){
			for(unsigned int i = 0 ; i < shdr.sh_size ; i += SHDR_COLUMN_SIZE){
				int j = 0;
				int mx = shdr.sh_size - i;
				if(mx > SHDR_COLUMN_SIZE) mx = SHDR_COLUMN_SIZE;
				printf("%08x: ", (unsigned int)(shdr.sh_offset + i));
				for(j = 0 ; j < mx ; j++) {
					printf("%02x ", buf[i + j]);
					if(j == 7)	printf(" ");
				}
				if(mx < SHDR_COLUMN_SIZE) {
					for(j = 0 ; j < SHDR_COLUMN_SIZE - mx ; j++){
						printf("   ");
					}
					if(mx < 7) printf(" ");
				}
				printf("  ");
				for(j = 0 ; j < mx ; j++) printf("%c", (isalnum(buf[i + j])) ? buf[i + j] : '.');
				printf("\n");
			}
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
			return buf;
		}

		virtual const char* name(void){
			return se_name;
		}
};

class elf_shdr_symtab_t : public elf_shdr_entry_t{
	protected:
		elf_section_t** entry;
		int n_entry;
	public:
		elf_shdr_symtab_t(FILE* fd, Elf32_Shdr eshdr, unsigned char* ebuf, char* ename, char* strtab, elf_section_t*(*entry_new)(FILE*, unsigned int, char*))
							: elf_shdr_entry_t(eshdr, ebuf, ename){
			n_entry = eshdr.sh_size / eshdr.sh_entsize;
			entry = new elf_section_t* [n_entry];
			for(int i = 0 ; i < n_entry ; i++){
				entry[i] = entry_new(fd, eshdr.sh_offset + sizeof(Elf32_Sym) * i, strtab);
			}
		}

		~elf_shdr_symtab_t(){
			if(n_entry){
				for(int i = 0 ; i < n_entry ; i++) delete entry[i];
				delete entry;
			}
		}

		virtual void format_header(void){
			printf("no header\n");
		}

		virtual void format_body(void){
			printf("no body\n");
		}

		virtual void format_child(void){
			int i = 0, num = shdr.sh_size / shdr.sh_entsize;

			printf(".\t..\t");
			for(i = 0 ; i < num ; i++) printf("%s\t", entry[i]->name());
			printf("\n");
		}

		virtual elf_section_t* get_child(const unsigned int idx){
			return (idx >= (shdr.sh_size / shdr.sh_entsize)) ? 0 : entry[idx];
		}

		virtual elf_section_t* get_child(const char* stridx){
			int i = 0, num = shdr.sh_size / shdr.sh_entsize;
			for(i = 0 ; i < num ; i++) if(!strcmp(entry[i]->name(), stridx)) return entry[i];
			return 0;
		}
};

elf_section_t* shdr_entry_new(FILE* fd, int e_shoff, char* shstrtab, char* strtab){
	Elf32_Shdr shdr;
	unsigned char* buf = 0;
	elf_section_t* section = 0;

	fseek(fd, e_shoff, SEEK_SET);
	fread(&shdr, sizeof(Elf32_Shdr), 1, fd);

	fseek(fd, shdr.sh_offset, SEEK_SET);
	buf = (unsigned char*)new unsigned char[shdr.sh_size];
	fread(buf, shdr.sh_size, 1, fd);

	switch(shdr.sh_type){
		case SHT_SYMTAB:
		case SHT_DYNSYM:
			section = (!strtab) ? 0 : new elf_shdr_symtab_t(fd, shdr, buf, (!shstrtab) ? 0 : shstrtab + shdr.sh_name, strtab, shdr_symtab_entry_new);
			break;
		case SHT_REL:
			section = (!strtab) ? 0 : new elf_shdr_symtab_t(fd, shdr, buf, (!shstrtab) ? 0 : shstrtab + shdr.sh_name, strtab, shdr_rel_entry_new);
		case SHT_RELA:
			section = (!strtab) ? 0 : new elf_shdr_symtab_t(fd, shdr, buf, (!shstrtab) ? 0 : shstrtab + shdr.sh_name, strtab, shdr_rela_entry_new);
		default:
			section = new elf_shdr_entry_t(shdr, buf, (!shstrtab) ? 0 : shstrtab + shdr.sh_name);
			break;
	}
	return section;
}
