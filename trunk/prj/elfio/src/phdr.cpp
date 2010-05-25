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

class elfPHDR : public elfSection{
	protected:
		//todo:phdr_entry를 section container로 정의하여 iterator를 달아 순회하도록 할것
		elfSection** entry;
		unsigned int n_entry;

	public:
		elfPHDR(int phnum, FILE* fd, int phoff){
			char name[8];
			entry = new elfSection* [phnum];
			n_entry = phnum;
			for(unsigned int i = 0 ; i < n_entry ; i++){
				sprintf(name, "%d", i);
				entry[i] = phdr_entry_new(fd, phoff + sizeof(Elf32_Phdr) * i, name);
			}
		}

		~elfPHDR(){
			if(n_entry){
				for(unsigned int i = 0 ; i < n_entry ; i++) delete entry[i];
				delete entry;
			}
		}

		elfAttribute* attribute(void) { return 0;}

		const unsigned int childs(void){
			return n_entry;
		}

		elfSection* childAt(const int idx){
			return entry[idx];
		}

		elfSection* childAt(const char* stridx){
			for(unsigned int i = 0 ; i < n_entry ; i++){
				if(!strcmp(entry[i]->name(), stridx)) return entry[i];
			}
			return 0;
		}
		elfBuffer* body(void){
			return 0;
		}

		const char* category(void){
			return "phdr";
		}

		const char* name(void){
			return "phdr";
		}
};

elfSection* phdr_new(Elf32_Ehdr ehdr, FILE* fd){
	return (elfSection*)new elfPHDR(ehdr.e_phnum, fd, ehdr.e_phoff);
}
