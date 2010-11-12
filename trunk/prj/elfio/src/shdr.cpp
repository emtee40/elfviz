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

class elf_shdr_t : public elfSection{
	protected:
		elfSection** entry;
		unsigned int n_entry;
		elfBuffer* shstr;
		shdr_entry_factory factory;
	public:
		elf_shdr_t(int shnum, FILE* fd, int shoff, int shstrndx):shstr(0){
			entry = new elfSection* [shnum];
			n_entry = shnum;
			elfSection* shdr_shstr = factory.entry_new(fd, shoff + sizeof(Elf32_Shdr) * shstrndx, 0, 0);
			elfBuffer* shstr = shdr_shstr->body();
			elfBuffer* strtab = 0;
			unsigned int i = 0;
			//todo : order of names in shstrtab is not coinsident with that of sections

			entry = new elfSection* [n_entry];
			memset(entry, 0, sizeof(elfSection*) * n_entry);
			bool found = false;
			int foundIdx = 0;
			for(i = 0 ; i < n_entry ; i++){
				entry[i] = factory.entry_new(fd,
							shoff + sizeof(Elf32_Shdr) * i,
							(char*)((shstr) ? shstr->buffer : 0),
							(char*)((strtab) ? strtab->buffer : 0));
				if(!strtab && entry[i] && !strcmp(entry[i]->name(), ".strtab")) {
					strtab = entry[i]->body();
					foundIdx = i;
					found = true;
					break;
				}
			}
			if(!found){
				for(i = 0 ; i < n_entry ; i++){
					if(entry[i]) delete entry[i];
					entry[i] = factory.entry_new(fd,
								shoff + sizeof(Elf32_Shdr) * i,
								(char*)((shstr) ? shstr->buffer : 0),
								(char*)((strtab) ? strtab->buffer : 0));
					if(!strtab && entry[i] && !strcmp(entry[i]->name(), ".dynstr")) {
						strtab = entry[i]->body();
						foundIdx = i;
						found = true;
						break;
					}
				}
			}
			for(i = 0 ; i < n_entry ; i++){
				if(entry[i]) {
					if(i != foundIdx)
						delete entry[i];
					else
						continue;
				}
				entry[i] = factory.entry_new(fd,
							shoff + sizeof(Elf32_Shdr) * i,
							(char*)((shstr) ? shstr->buffer : 0),
							(char*)((strtab) ? strtab->buffer : 0));
				if(!strtab && entry[i] && !strcmp(entry[i]->name(), ".strtab")) {
					strtab = entry[i]->body();
					i = -1;
					continue;
				}
					
			}
			delete shdr_shstr;
		}

		~elf_shdr_t(){
			if(n_entry){
				for(unsigned int i = 0 ; i < n_entry ; i++) delete entry[i];
				delete entry;
				if(shstr)delete shstr;
			}
		}

		elfAttribute* attribute(void){return 0;}

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
			return "shdr";
		}

		const char* name(void){
			return "shdr";
		}
};

elfSection* shdr_new(Elf32_Ehdr ehdr, FILE* fd){
	return (elfSection*) new elf_shdr_t(ehdr.e_shnum, fd, ehdr.e_shoff, ehdr.e_shstrndx);
}
