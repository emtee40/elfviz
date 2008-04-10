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

#include "ucrt/ucrt.h"
#include "elfio/elfio.h"
#include "elftypes.h"
#include "phdr.h"
#include "shdr.h"

typedef class _elfio_ctrl_t : public elfio_t{
	protected:
		char* file;
		rt_file_t fd;
		Elf32_Ehdr ehdr;
		elf_section_t* mphdr;
		elf_section_t* mshdr;

		void type_str(rt_file_t fd, int e_type){
			char* str = "unknown";
			rt_fprint(fd, "type=");
			switch(e_type){
			case ET_NONE:	str =  "ET_NONE";	break;
			case ET_REL:	str =  "ET_REL";	break;
			case ET_EXEC:	str =  "ET_EXEC";	break;
			case ET_DYN:	str =  "ET_DYN";	break;
			case ET_CORE:	str =  "ET_CORE";	break;
			case ET_LOPROC:	str =  "ET_LOPROC";	break;
			case ET_HIPROC:	str =  "ET_HIPROC";	break;
			}
			rt_fprint(fd, "%s\n", str);
		}

		void machine_str(rt_file_t fd, int e_machine){
			char* str = "unknown";
			rt_fprint(fd, "machine=");
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
			rt_fprint(fd, "%s\n", str);
		}

		void version_str(rt_file_t fd, int e_version){
			char* str = "unknown";
			rt_fprint(fd, "version=");
			switch(e_version){
			case EV_NONE:		str =  "EV_NONE";		break;
			case EV_CURRENT:	str =  "EV_CURRENT";	break;
			}
			rt_fprint(fd, "%s\n", str);
		}

		void ident_str(rt_file_t fd, char* e_ident){
			char* str = "unknown";
			rt_fprint(fd, "class=");
			switch(e_ident[EI_CLASS]){
			case ELFCLASSNONE:	str = "ELFCLASSNONE";	break;
			case ELFCLASS32:	str = "ELFCLASS32";		break;
			case ELFCLASS64:	str = "ELFCLASS64";		break;
			}
			rt_fprint(fd, "%s\n", str);
			str = "unknown";
			rt_fprint(fd, "data=");
			switch(e_ident[EI_DATA]){
			case ELFDATANONE:	str = "ELFDATANONE";	break;
			case ELFDATA2LSB:	str = "ELFDATA2LSB";	break;
			case ELFDATA2MSB:	str = "ELFDATA2MSB";	break;
			}
			rt_fprint(fd, "%s\n", str);
		}

		void entry_str(rt_file_t fd, int e_entry){
			rt_fprint(fd, "entry=0x%x\n", e_entry);
		}

		void phoff_str(rt_file_t fd, int e_phoff){
			rt_fprint(fd, "phoff=0x%x\n", e_phoff);
		}

		void shoff_str(rt_file_t fd, int e_shoff){
			rt_fprint(fd, "shoff=0x%x\n", e_shoff);
		}

		void flags_str(rt_file_t fd, int e_flags){
			rt_fprint(fd, "flags=");
			if(e_flags & EF_ARM_HASENTRY)			rt_fprint(fd, "EF_ARM_HASENTRY | ");
			if(e_flags & EF_ARM_SYMSARESORTED)		rt_fprint(fd, "EF_ARM_SYMSARESORTED | ");
			if(e_flags & EF_ARM_DYNSYMSUSESEGIDX)	rt_fprint(fd, "EF_ARM_DYNSYMSUSESEGIDX | ");
			if(e_flags & EF_ARM_MAPSYMSFIRST)		rt_fprint(fd, "EF_ARM_MAPSYMSFIRST | ");
			if(e_flags & EF_ARM_EABIMASK)			rt_fprint(fd, "EF_ARM_EABIMASK");
			rt_fprint(fd, "\n");
		}

		void ehsize_str(rt_file_t fd, int e_ehsize){
			rt_fprint(fd, "ehsize=%d\n", e_ehsize);
		}

		void phentsize_str(rt_file_t fd, int e_phentsize){
			rt_fprint(fd, "phentsize=%d\n", e_phentsize);
		}

		void phnum_str(rt_file_t fd, int e_phnum){
			rt_fprint(fd, "phnum=%d\n", e_phnum);
		}

		void shentsize_str(rt_file_t fd, int e_shentsize){
			rt_fprint(fd, "shentsize=%d\n", e_shentsize);
		}

		void shnum_str(rt_file_t fd, int e_shnum){
			rt_fprint(fd, "shnum=%d\n", e_shnum);
		}

		void shstridx_str(rt_file_t fd, int e_shstridx){
			rt_fprint(fd, "shstridx=%d\n", e_shstridx);
		}

		cbool is_valid_elf(Elf32_Ehdr ehdr){
			char elf_mag[4] = {0x7f, 'E', 'L', 'F'};
			return (!rt_strncmp(elf_mag, (char*)ehdr.e_ident, 4)) ? ctrue : cfalse;
		}

	public:
		_elfio_ctrl_t(char* efile){
			mphdr = mshdr = cnull;
			fd = rt_file_new(efile, RT_FILE_OPEN_RDONLY);
			if(!fd) return;
			rt_fread(fd, &ehdr, sizeof(ehdr));
			file = rt_strdup(efile);
		}

		~_elfio_ctrl_t(){
			rt_file_delete(fd);
			rt_delete(file);
			if(mphdr) delete mphdr;
			if(mshdr) delete mshdr;
		}

		virtual void format(rt_file_t rtout){
			if(!is_valid_elf(ehdr)) {
				rt_fprint(rtout, "not valid elf");
				return;
			}
			rt_fprint(rtout, "ELF header\n");
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
			if(!is_valid_elf(ehdr)) return cnull;
			if(!mphdr) mphdr = phdr_new(ehdr, fd);
			return mphdr;
		}

		virtual elf_section_t* get_shdr(void){
			if(!is_valid_elf(ehdr)) return cnull;
			if(!mshdr) mshdr = shdr_new(ehdr, fd);
			return mshdr;
		}
} elfio_ctrl_t;

elfio_t* elfio_new(char* file){
	elfio_ctrl_t* elfio = new elfio_ctrl_t(file);
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
