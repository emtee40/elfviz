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
#include "shdr.h"

class elfio_ctrl_t : public elf_section_t{
	protected:
		char file[512];
		FILE* fd;
		Elf32_Ehdr ehdr;
		elf_section_t* mphdr;
		elf_section_t* mshdr;

		void type_str(int e_type){
			char* str = (char*)"unknown";
			printf("type=");
			switch(e_type){
			case ET_NONE:	str = (char*)"ET_NONE";	break;
			case ET_REL:	str = (char*)"ET_REL";	break;
			case ET_EXEC:	str = (char*)"ET_EXEC";	break;
			case ET_DYN:	str = (char*)"ET_DYN";	break;
			case ET_CORE:	str = (char*)"ET_CORE";	break;
			case ET_LOPROC:	str = (char*)"ET_LOPROC";	break;
			case ET_HIPROC:	str = (char*)"ET_HIPROC";	break;
			}
			printf("%s\n", str);
		}

		void machine_str(int e_machine){
			char* str = (char*)"unknown";
			printf("machine=");
			switch(e_machine){
			case EM_NONE:			str = (char*)"EM_NONE";			break;
			case EM_M32:			str = (char*)"EM_M32";			break;
			case EM_SPARC:			str = (char*)"EM_SPARC";			break;
			case EM_386:			str = (char*)"EM_386";			break;
			case EM_68K:			str = (char*)"EM_68K";			break;
			case EM_88K:			str = (char*)"EM_88K";			break;
			case EM_860:			str = (char*)"EM_860";			break;
			case EM_MIPS:			str = (char*)"EM_MIPS";			break;
			case EM_MIPS_RS4_BE:		str = (char*)"EM_MIPS_RS4_BE";	break;
			case EM_ARM:			str = (char*)"EM_ARM";			break;
			}
			printf("%s\n", str);
		}

		void version_str(int e_version){
			char* str = (char*)"unknown";
			printf("version=");
			switch(e_version){
			case EV_NONE:		str = (char*)"EV_NONE";		break;
			case EV_CURRENT:	str = (char*)"EV_CURRENT";	break;
			}
			printf("%s\n", str);
		}

		void ident_str(char* e_ident){
			char* str = (char*)"unknown";
			printf("class=");
			switch(e_ident[EI_CLASS]){
			case ELFCLASSNONE:	str = (char*)"ELFCLASSNONE";	break;
			case ELFCLASS32:	str = (char*)"ELFCLASS32";		break;
			case ELFCLASS64:	str = (char*)"ELFCLASS64";		break;
			}
			printf("%s\n", str);
			str = (char*)"unknown";
			printf("data=");
			switch(e_ident[EI_DATA]){
			case ELFDATANONE:	str = (char*)"ELFDATANONE";	break;
			case ELFDATA2LSB:	str = (char*)"ELFDATA2LSB";	break;
			case ELFDATA2MSB:	str = (char*)"ELFDATA2MSB";	break;
			}
			printf("%s\n", str);
		}

		void entry_str(int e_entry){
			printf("entry=0x%x\n", e_entry);
		}

		void phoff_str(int e_phoff){
			printf("phoff=0x%x\n", e_phoff);
		}

		void shoff_str(int e_shoff){
			printf("shoff=0x%x\n", e_shoff);
		}

		void flags_str(int e_flags){
			printf("flags=");
			if(e_flags & EF_ARM_HASENTRY)			printf("EF_ARM_HASENTRY | ");
			if(e_flags & EF_ARM_SYMSARESORTED)		printf("EF_ARM_SYMSARESORTED | ");
			if(e_flags & EF_ARM_DYNSYMSUSESEGIDX)	printf("EF_ARM_DYNSYMSUSESEGIDX | ");
			if(e_flags & EF_ARM_MAPSYMSFIRST)		printf("EF_ARM_MAPSYMSFIRST | ");
			if(e_flags & EF_ARM_EABIMASK)			printf("EF_ARM_EABIMASK");
			printf("\n");
		}

		void ehsize_str(int e_ehsize){
			printf("ehsize=%d\n", e_ehsize);
		}

		void phentsize_str(int e_phentsize){
			printf("phentsize=%d\n", e_phentsize);
		}

		void phnum_str(int e_phnum){
			printf("phnum=%d\n", e_phnum);
		}

		void shentsize_str(int e_shentsize){
			printf("shentsize=%d\n", e_shentsize);
		}

		void shnum_str(int e_shnum){
			printf("shnum=%d\n", e_shnum);
		}

		void shstridx_str(int e_shstridx){
			printf("shstridx=%d\n", e_shstridx);
		}

		bool is_valid_elf(Elf32_Ehdr ehdr){
			char elf_mag[4] = {0x7f, 'E', 'L', 'F'};
			return (!strncmp(elf_mag, (char*)ehdr.e_ident, 4)) ? true : false;
		}

	public:
		elfio_ctrl_t(char* efile){
			mphdr = mshdr = 0;
			file[0] = 0;
			fd = fopen(efile, "rb");
			if(!fd) return;
			fread(&ehdr, sizeof(ehdr), 1, fd);
			strcpy(file, efile);
		}

		~elfio_ctrl_t(){
			if(fd) fclose(fd);
			if(mphdr) delete mphdr;
			if(mshdr) delete mshdr;
		}

		virtual void format_header(void){
			if(!is_valid_elf(ehdr)) {
				printf("not valid elf");
				return;
			}
			printf("ELF header\n");
			ident_str((char*)ehdr.e_ident);
			type_str(ehdr.e_type);
			machine_str(ehdr.e_machine);
			version_str(ehdr.e_version);
			entry_str(ehdr.e_entry);
			phoff_str(ehdr.e_phoff);
			shoff_str(ehdr.e_shoff);
			flags_str(ehdr.e_flags);
			ehsize_str(ehdr.e_ehsize);
			phentsize_str(ehdr.e_phentsize);
			phnum_str(ehdr.e_phnum);
			shentsize_str(ehdr.e_shentsize);
			shnum_str(ehdr.e_shnum);
			shstridx_str(ehdr.e_shstrndx);
		}

		virtual void format_body(void){
			printf("no body\n");
		}

		virtual void format_child(void){
			if(!mphdr) mphdr = phdr_new(ehdr, fd);
			if(!mshdr) mshdr = shdr_new(ehdr, fd);
			printf(".\t");
			if(mphdr) printf("%s\t", mphdr->name());
			if(mshdr) printf("%s\n", mshdr->name());
		}

		virtual const unsigned char* get_body(void){
			return 0;
		}

		virtual const char* name(void){
			return file;
		}

		virtual elf_section_t* get_child(const unsigned int idx){
			if(!is_valid_elf(ehdr)) return 0;
			switch(idx){
				case 0:
					if(!mphdr) mphdr = phdr_new(ehdr, fd);
					return mphdr;
					break;
				case 1:
					if(!mshdr) mshdr = shdr_new(ehdr, fd);
					return mshdr;
					break;
			}
			return 0;
		}

		virtual elf_section_t* get_child(const char* stridx){
			unsigned int idx = -1;
			if(!strcmp(stridx, "phdr")) idx = 0;
			if(!strcmp(stridx, "shdr")) idx = 1;
			return get_child(idx);
		}
};

elf_section_t* elfio_new(char* file){
	elfio_ctrl_t* elfio = new elfio_ctrl_t(file);
	if(!elfio->name()) {
		delete elfio;
		elfio = 0;
	}
	return elfio;
}

/*
static unsigned long elf_hash(const unsigned char *name) {
	unsigned long h, g;
	for (h = 0; *name != 0; ++name)	{
		h = (h << 4) + *name;
		g = h & 0xf0000000;
		if (g != 0) h ^= g >> 24;
		h &= ~g;
	}
	return h;
}
*/
