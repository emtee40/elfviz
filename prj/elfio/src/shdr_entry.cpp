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
#include "shdr_entry.h"
#include "shdr_symtab_entry.h"
#include "shdr_rel_entry.h"

typedef class _elf_shdr_entry_t : public elf_section_t{
	protected:
		Elf32_Shdr shdr;
		unsigned char* buf;
		char* se_name;

		void name_str(FILE* fd, int sh_name){
			fprintf(fd, "sh_name=%d\n", sh_name);
		}

		void type_str(FILE* fd, int sh_type){
			char* str = "unknown";
			fprintf(fd, "sh_type=");
			switch(sh_type){
			case SHT_NULL:		str = "SHT_NULL";		break;
			case SHT_PROGBITS:	str = "SHT_PROGBITS";	break;
			case SHT_SYMTAB:	str = "SHT_SYMTAB";		break;
			case SHT_STRTAB:	str = "SHT_STRTAB";		break;
			case SHT_RELA:		str = "SHT_RELA";		break;
			case SHT_HASH:		str = "SHT_HASH";		break;
			case SHT_DYNAMIC:	str = "SHT_DYNAMIC";	break;
			case SHT_NOTE:		str = "SHT_NOTE";		break;
			case SHT_NOBITS:	str = "SHT_NOBITS";		break;
			case SHT_REL:		str = "SHT_REL";		break;
			case SHT_SHLIB:		str = "SHT_SHLIB";		break;
			case SHT_DYNSYM:	str = "SHT_DYNSYM";		break;
			case SHT_LOPROC:	str = "SHT_LOPROC";		break;
			case SHT_HIPROC:	str = "SHT_HIPROC";		break;
			case SHT_LOUSER:	str = "SHT_LOUSER";		break;
			case SHT_HIUSER:	str = "SHT_HIUSER";		break;
			}
			fprintf(fd, "%s\n", str);
		}

		void offset_str(FILE* fd, int sh_offset){
			fprintf(fd, "sh_offset=0x%x\n", sh_offset);
		}

		void addr_str(FILE* fd, int sh_addr){
			fprintf(fd, "sh_addr=0x%x\n", sh_addr);
		}

		void size_str(FILE* fd, int sh_size){
			fprintf(fd, "sh_size=%d\n", sh_size);
		}

		void entsize_str(FILE* fd, int sh_entsize){
			fprintf(fd, "sh_entsize=%d\n", sh_entsize);
		}

		void link_str(FILE* fd, int sh_link){
			fprintf(fd, "sh_link=%d\n", sh_link);
		}

		void info_str(FILE* fd, int sh_info){
			fprintf(fd, "sh_info=0x%x\n", sh_info);
		}

		void flags_str(FILE* fd, int sh_flags){
			fprintf(fd, "sh_flags=");
			if(sh_flags & SHF_WRITE)		fprintf(fd, "SHF_WRITE | ");
			if(sh_flags & SHF_ALLOC)		fprintf(fd, "SHF_ALLOC | ");
			if(sh_flags & SHF_EXECINSTR)	fprintf(fd, "SHF_EXEINSTR | ");
			if(sh_flags & SHF_MASKPROC)		fprintf(fd, "SHF_MASKPROC");
			fprintf(fd, "\n");
		}

		void align_str(FILE* fd, int sh_addralign){
			fprintf(fd, "sh_addralign=%d\n", sh_addralign);
		}

	public:
		_elf_shdr_entry_t(){}
		_elf_shdr_entry_t(Elf32_Shdr eshdr, unsigned char* ebuf, char* ename){
			memcpy(&shdr, &eshdr, sizeof(Elf32_Shdr));
			buf = ebuf;
			se_name = strdup(ename);
		}

		~_elf_shdr_entry_t(){
			delete buf;
			delete se_name;
		}

		virtual void format(FILE* fd){
			if(se_name) fprintf(fd, "sh_name_str=%s\n", se_name);
			name_str(fd, shdr.sh_name);
			type_str(fd, shdr.sh_type);
			flags_str(fd, shdr.sh_flags);
			addr_str(fd, shdr.sh_addr);
			offset_str(fd, shdr.sh_offset);
			size_str(fd, shdr.sh_size);
			link_str(fd, shdr.sh_link);
			info_str(fd, shdr.sh_info);
			align_str(fd, shdr.sh_addralign);
			entsize_str(fd, shdr.sh_entsize);
			fprintf(fd, "\n");
		}

		#define SHDR_COLUMN_SIZE 16
		virtual void dump(FILE* fd){
			for(unsigned int i = 0 ; i < shdr.sh_size ; i += SHDR_COLUMN_SIZE){
				int j = 0;
				int mx = shdr.sh_size - i;
				if(mx > SHDR_COLUMN_SIZE) mx = SHDR_COLUMN_SIZE;
				fprintf(fd, "%08x: ", shdr.sh_offset + i);
				for(j = 0 ; j < mx ; j++) {
					fprintf(fd, "%02x ", buf[i + j]);
					if(j == 7)	fprintf(fd, " ");
				}
				if(mx < SHDR_COLUMN_SIZE) {
					for(j = 0 ; j < SHDR_COLUMN_SIZE - mx ; j++){
						fprintf(fd, "   ");
					}
					if(mx < 7) fprintf(fd, " ");
				}
				fprintf(fd, "  ");
				for(j = 0 ; j < mx ; j++) fprintf(fd, "%c", buf[i + j]);
				fprintf(fd, "\n");
			}
		}

		virtual elf_section_t* get_sub(const int idx){
			return 0;
		}

		virtual elf_section_t* find_sub(const char* stridx){
			return 0;
		}

		virtual const unsigned char* data(void){
			return buf;
		}

		virtual const char* name(void){
			return se_name;
		}
}elf_shdr_entry_t;

typedef class _elf_shdr_symtab_t : public elf_shdr_entry_t{
	protected:
		elf_section_t** entry;
		int n_entry;
	public:
		_elf_shdr_symtab_t(FILE* fd, Elf32_Shdr eshdr, unsigned char* ebuf, char* ename, char* strtab, elf_section_t*(*entry_new)(FILE*, unsigned int, char*))
							: elf_shdr_entry_t(eshdr, ebuf, ename){
			n_entry = eshdr.sh_size / eshdr.sh_entsize;
			entry = new elf_section_t* [n_entry];
			for(int i = 0 ; i < n_entry ; i++){
				entry[i] = entry_new(fd, eshdr.sh_offset + sizeof(Elf32_Sym) * i, strtab);
			}
		}

		~_elf_shdr_symtab_t(){
			if(n_entry){
				for(int i = 0 ; i < n_entry ; i++) delete entry[i];
				delete entry;
			}
		}

		virtual void dump(FILE* fd){
			int i = 0, num = shdr.sh_size / shdr.sh_entsize;

			for(i = 0 ; i < num ; i++){
				fprintf(fd, "symtab[%d]\n", i);
				entry[i]->format(fd);
			}
		}

		virtual elf_section_t* get_sub(const int idx){
			return (idx >= (shdr.sh_size / shdr.sh_entsize)) ? 0 : entry[idx];
		}

}elf_shdr_symtab_t;

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
