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
#include <string.h>

#include "elfio/elfio.h"
#include "elftypes.h"
#include "phdr.h"
#include "phdr_entry.h"

class elf_phdr_t : public elf_section_t{
	protected:
		//todo:phdr_entry를 section container로 정의하여 iterator를 달아 순회하도록 할것
		elf_section_t** entry;
		unsigned int n_entry;

	public:
		elf_phdr_t(int phnum, FILE* fd, int phoff){
			char name[8];
			entry = new elf_section_t* [phnum];
			n_entry = phnum;
			for(unsigned int i = 0 ; i < n_entry ; i++){
				sprintf(name, "%d", i);
				entry[i] = phdr_entry_new(fd, phoff + sizeof(Elf32_Phdr) * i, name);
			}
		}

		~elf_phdr_t(){
			if(n_entry){
				for(unsigned int i = 0 ; i < n_entry ; i++) delete entry[i];
				delete entry;
			}
		}

		virtual elf_attr_t* get_attr(void) { return 0;}

		virtual const unsigned int get_child_num(void){
			return n_entry;
		}

		virtual elf_section_t* get_child(const int idx){
			return entry[idx];
		}

		virtual elf_section_t* get_child(const char* stridx){
			for(unsigned int i = 0 ; i < n_entry ; i++){
				if(!strcmp(entry[i]->name(), stridx)) return entry[i];
			}
			return 0;
		}
		virtual elf_buffer_t* get_body(void){
			return 0;
		}

		virtual const char* category(void){
			return "phdr";
		}

		virtual const char* name(void){
			return "phdr";
		}
};

elf_section_t* phdr_new(Elf32_Ehdr ehdr, FILE* fd){
	return (elf_section_t*)new elf_phdr_t(ehdr.e_phnum, fd, ehdr.e_phoff);
}
