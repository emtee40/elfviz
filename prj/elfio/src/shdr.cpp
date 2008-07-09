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
#include "shdr.h"
#include "shdr_entry.h"

typedef class _elf_shdr_t : public elf_section_t{
	protected:
		elf_section_t** entry;
		int n_entry;
		char* shstr;

	public:
		_elf_shdr_t(int shnum, FILE* fd, int shoff, int shstrndx){
			entry = new elf_section_t* [shnum];
			n_entry = shnum;
			elf_section_t* shdr_shstr = shdr_entry_new(fd, shoff + sizeof(Elf32_Shdr) * shstrndx, 0, 0);
			char* shstr = (char*)shdr_shstr->data();
			char* strtab = 0;
			int strtab_idx = 0;
			int i = 0;
			//todo : order of names in shstrtab is not coinsident with that of sections

			entry = new elf_section_t* [n_entry];
			memset(entry, 0, sizeof(elf_section_t*) * n_entry);
			for(i = 0 ; i < n_entry ; i++){
				if(!entry[i]) entry[i] = shdr_entry_new(fd, shoff + sizeof(Elf32_Shdr) * i, shstr, strtab);
				if(!strtab && entry[i] && !strcmp(entry[i]->name(), ".strtab")) {
					strtab = (char*)entry[i]->data();
					i = -1;
					continue;
				}
					
			}
			delete shdr_shstr;
		}

		~_elf_shdr_t(){
			if(n_entry){
				for(int i = 0 ; i < n_entry ; i++) delete entry[i];
				delete entry;
				delete shstr;
			}
		}

		virtual void format(FILE* fd){
			fprintf(fd, "no header\n");
		}

		virtual void dump(FILE* fd){
			for(int i = 0 ; i < n_entry ; i++)	entry[i]->format(fd);
		}

		virtual elf_section_t* get_sub(const int idx){
			return (idx >= n_entry) ? 0 : entry[idx];
		}

		virtual elf_section_t* find_sub(const char* stridx){
			int i = 0;
			for(i = 0 ; i < n_entry ; i++){
				if(!strcmp(entry[i]->name(), stridx)) return entry[i];
			}
			return 0;
		}

		virtual const unsigned char* data(void){
			return 0;
		}

		virtual const char* name(void){
			return 0;
		}
}elf_shdr_t;

elf_section_t* shdr_new(Elf32_Ehdr ehdr, FILE* fd){
	return (elf_section_t*) new elf_shdr_t(ehdr.e_shnum, fd, ehdr.e_shoff, ehdr.e_shstrndx);
}
