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

typedef class _elfio_ctrl_t : public elfio_t{
	protected:
		char* file;
		FILE* fd;
		Elf32_Ehdr ehdr;
		elf_section_t* mphdr;
		elf_section_t* mshdr;

		void type_str(FILE* fd, int e_type){
			char* str = "unknown";
			fprintf(fd, "type=");
			switch(e_type){
			case ET_NONE:	str =  "ET_NONE";	break;
			case ET_REL:	str =  "ET_REL";	break;
			case ET_EXEC:	str =  "ET_EXEC";	break;
			case ET_DYN:	str =  "ET_DYN";	break;
			case ET_CORE:	str =  "ET_CORE";	break;
			case ET_LOPROC:	str =  "ET_LOPROC";	break;
			case ET_HIPROC:	str =  "ET_HIPROC";	break;
			}
			fprintf(fd, "%s\n", str);
		}

		void machine_str(FILE* fd, int e_machine){
			char* str = "unknown";
			fprintf(fd, "machine=");
			switch(e_machine){
			case EM_NONE:			str =  "EM_NONE";			break;
			case EM_M32:			str =  "EM_M32";			break;
			case EM_SPARC:			str =  "EM_SPARC";			break;
			case EM_386:			str =  "EM_386";			break;
			case EM_68K:			str =  "EM_68K";			break;
			case EM_88K:			str =  "EM_88K";			break;
			case EM_860:			str =  "EM_860";			break;
			case EM_MIPS:			str =  "EM_MIPS";			break;
			case EM_MIPS_RS4_BE:	str =  "EM_MIPS_RS4_BE";	break;
			case EM_ARM:			str =  "EM_ARM";			break;
			}
			fprintf(fd, "%s\n", str);
		}

		void version_str(FILE* fd, int e_version){
			char* str = "unknown";
			fprintf(fd, "version=");
			switch(e_version){
			case EV_NONE:		str =  "EV_NONE";		break;
			case EV_CURRENT:	str =  "EV_CURRENT";	break;
			}
			fprintf(fd, "%s\n", str);
		}

		void ident_str(FILE* fd, char* e_ident){
			char* str = "unknown";
			fprintf(fd, "class=");
			switch(e_ident[EI_CLASS]){
			case ELFCLASSNONE:	str = "ELFCLASSNONE";	break;
			case ELFCLASS32:	str = "ELFCLASS32";		break;
			case ELFCLASS64:	str = "ELFCLASS64";		break;
			}
			fprintf(fd, "%s\n", str);
			str = "unknown";
			fprintf(fd, "data=");
			switch(e_ident[EI_DATA]){
			case ELFDATANONE:	str = "ELFDATANONE";	break;
			case ELFDATA2LSB:	str = "ELFDATA2LSB";	break;
			case ELFDATA2MSB:	str = "ELFDATA2MSB";	break;
			}
			fprintf(fd, "%s\n", str);
		}

		void entry_str(FILE* fd, int e_entry){
			fprintf(fd, "entry=0x%x\n", e_entry);
		}

		void phoff_str(FILE* fd, int e_phoff){
			fprintf(fd, "phoff=0x%x\n", e_phoff);
		}

		void shoff_str(FILE* fd, int e_shoff){
			fprintf(fd, "shoff=0x%x\n", e_shoff);
		}

		void flags_str(FILE* fd, int e_flags){
			fprintf(fd, "flags=");
			if(e_flags & EF_ARM_HASENTRY)			fprintf(fd, "EF_ARM_HASENTRY | ");
			if(e_flags & EF_ARM_SYMSARESORTED)		fprintf(fd, "EF_ARM_SYMSARESORTED | ");
			if(e_flags & EF_ARM_DYNSYMSUSESEGIDX)	fprintf(fd, "EF_ARM_DYNSYMSUSESEGIDX | ");
			if(e_flags & EF_ARM_MAPSYMSFIRST)		fprintf(fd, "EF_ARM_MAPSYMSFIRST | ");
			if(e_flags & EF_ARM_EABIMASK)			fprintf(fd, "EF_ARM_EABIMASK");
			fprintf(fd, "\n");
		}

		void ehsize_str(FILE* fd, int e_ehsize){
			fprintf(fd, "ehsize=%d\n", e_ehsize);
		}

		void phentsize_str(FILE* fd, int e_phentsize){
			fprintf(fd, "phentsize=%d\n", e_phentsize);
		}

		void phnum_str(FILE* fd, int e_phnum){
			fprintf(fd, "phnum=%d\n", e_phnum);
		}

		void shentsize_str(FILE* fd, int e_shentsize){
			fprintf(fd, "shentsize=%d\n", e_shentsize);
		}

		void shnum_str(FILE* fd, int e_shnum){
			fprintf(fd, "shnum=%d\n", e_shnum);
		}

		void shstridx_str(FILE* fd, int e_shstridx){
			fprintf(fd, "shstridx=%d\n", e_shstridx);
		}

		bool is_valid_elf(Elf32_Ehdr ehdr){
			char elf_mag[4] = {0x7f, 'E', 'L', 'F'};
			return (!strncmp(elf_mag, (char*)ehdr.e_ident, 4)) ? true : false;
		}

	public:
		_elfio_ctrl_t(char* efile){
			mphdr = mshdr = 0;
			file = 0;
			fd = fopen(efile, "rb");
			if(!fd) return;
			fread(&ehdr, sizeof(ehdr), 1, fd);
			file = strdup(efile);
		}

		~_elfio_ctrl_t(){
			if(fd) fclose(fd);
			if(file) delete file;
			if(mphdr) delete mphdr;
			if(mshdr) delete mshdr;
		}

		virtual void format(FILE* rtout){
			if(!is_valid_elf(ehdr)) {
				fprintf(rtout, "not valid elf");
				return;
			}
			fprintf(rtout, "ELF header\n");
			ident_str(rtout, (char*)ehdr.e_ident);
			type_str(rtout, ehdr.e_type);
			machine_str(rtout, ehdr.e_machine);
			version_str(rtout, ehdr.e_version);
			entry_str(rtout, ehdr.e_entry);
			phoff_str(rtout, ehdr.e_phoff);
			shoff_str(rtout, ehdr.e_shoff);
			flags_str(rtout, ehdr.e_flags);
			ehsize_str(rtout, ehdr.e_ehsize);
			phentsize_str(rtout, ehdr.e_phentsize);
			phnum_str(rtout, ehdr.e_phnum);
			shentsize_str(rtout, ehdr.e_shentsize);
			shnum_str(rtout, ehdr.e_shnum);
			shstridx_str(rtout, ehdr.e_shstrndx);
		}

		virtual const char* file_name(void){
			return file;
		}

		virtual elf_section_t* get_phdr(void){
			if(!is_valid_elf(ehdr)) return 0;
			if(!mphdr) mphdr = phdr_new(ehdr, fd);
			return mphdr;
		}

		virtual elf_section_t* get_shdr(void){
			if(!is_valid_elf(ehdr)) return 0;
			if(!mshdr) mshdr = shdr_new(ehdr, fd);
			return mshdr;
		}
} elfio_ctrl_t;

elfio_t* elfio_new(char* file){
	elfio_ctrl_t* elfio = new elfio_ctrl_t(file);
	if(!elfio->file_name()) {
		delete elfio;
		elfio = 0;
	}
	return (elfio_t*)elfio;
}

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
