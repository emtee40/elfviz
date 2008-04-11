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
#include "shdr_entry.h"
#include "shdr_symtab_entry.h"
#include "shdr_rel_entry.h"

typedef class _elf_shdr_entry_t : public elf_section_t{
	protected:
		Elf32_Shdr shdr;
		cbyte* buf;
		char* se_name;

		void name_str(rt_file_t fd, int sh_name){
			rt_fprint(fd, "sh_name=%d\n", sh_name);
		}

		void type_str(rt_file_t fd, int sh_type){
			char* str = "unknown";
			rt_fprint(fd, "sh_type=");
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
			rt_fprint(fd, "%s\n", str);
		}

		void offset_str(rt_file_t fd, int sh_offset){
			rt_fprint(fd, "sh_offset=0x%x\n", sh_offset);
		}

		void addr_str(rt_file_t fd, int sh_addr){
			rt_fprint(fd, "sh_addr=0x%x\n", sh_addr);
		}

		void size_str(rt_file_t fd, int sh_size){
			rt_fprint(fd, "sh_size=%d\n", sh_size);
		}

		void entsize_str(rt_file_t fd, int sh_entsize){
			rt_fprint(fd, "sh_entsize=%d\n", sh_entsize);
		}

		void link_str(rt_file_t fd, int sh_link){
			rt_fprint(fd, "sh_link=%d\n", sh_link);
		}

		void info_str(rt_file_t fd, int sh_info){
			rt_fprint(fd, "sh_info=0x%x\n", sh_info);
		}

		void flags_str(rt_file_t fd, int sh_flags){
			rt_fprint(fd, "sh_flags=");
			if(sh_flags & SHF_WRITE)		rt_fprint(fd, "SHF_WRITE | ");
			if(sh_flags & SHF_ALLOC)		rt_fprint(fd, "SHF_ALLOC | ");
			if(sh_flags & SHF_EXECINSTR)	rt_fprint(fd, "SHF_EXEINSTR | ");
			if(sh_flags & SHF_MASKPROC)		rt_fprint(fd, "SHF_MASKPROC");
			rt_fprint(fd, "\n");
		}

		void align_str(rt_file_t fd, int sh_addralign){
			rt_fprint(fd, "sh_addralign=%d\n", sh_addralign);
		}

	public:
		_elf_shdr_entry_t(){}
		_elf_shdr_entry_t(Elf32_Shdr eshdr, cbyte* ebuf, char* ename){
			rt_memcpy(&shdr, &eshdr, sizeof(Elf32_Shdr));
			buf = ebuf;
			se_name = rt_strdup(ename);
		}

		~_elf_shdr_entry_t(){
			rt_delete(buf);
			rt_delete(se_name);
		}

		virtual void format(rt_file_t fd){
			if(se_name) rt_fprint(fd, "sh_name_str=%s\n", se_name);
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
			rt_fprint(fd, "\n");
		}

		#define SHDR_COLUMN_SIZE 16
		virtual void dump(rt_file_t fd){
			for(unsigned int i = 0 ; i < shdr.sh_size ; i += SHDR_COLUMN_SIZE){
				int j = 0;
				int mx = shdr.sh_size - i;
				if(mx > SHDR_COLUMN_SIZE) mx = SHDR_COLUMN_SIZE;
				rt_fprint(fd, "%08x: ", shdr.sh_offset + i);
				for(j = 0 ; j < mx ; j++) {
					rt_fprint(fd, "%02x ", buf[i + j]);
					if(j == 7)	rt_fprint(fd, " ");
				}
				if(mx < SHDR_COLUMN_SIZE) {
					for(j = 0 ; j < SHDR_COLUMN_SIZE - mx ; j++){
						rt_fprint(fd, "   ");
					}
					if(mx < 7) rt_fprint(fd, " ");
				}
				rt_fprint(fd, "  ");
				for(j = 0 ; j < mx ; j++) rt_fprint(fd, "%c", buf[i + j]);
				rt_fprint(fd, "\n");
			}
		}

		virtual elf_section_t* get_sub(const int idx){
			return cnull;
		}

		virtual elf_section_t* find_sub(const char* stridx){
			return cnull;
		}

		virtual const cbyte* data(void){
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
		_elf_shdr_symtab_t(rt_file_t fd, Elf32_Shdr eshdr, cbyte* ebuf, char* ename, char* strtab, elf_section_t*(*entry_new)(rt_file_t, unsigned int, char*))
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

		virtual void dump(rt_file_t fd){
			int i = 0, num = shdr.sh_size / shdr.sh_entsize;

			for(i = 0 ; i < num ; i++){
				rt_fprint(fd, "symtab[%d]\n", i);
				entry[i]->format(fd);
			}
		}

		virtual elf_section_t* get_sub(const int idx){
			return (idx >= (shdr.sh_size / shdr.sh_entsize)) ? cnull : entry[idx];
		}

}elf_shdr_symtab_t;

elf_section_t* shdr_entry_new(rt_file_t fd, int e_shoff, char* shstrtab, char* strtab){
	Elf32_Shdr shdr;
	cbyte* buf = cnull;
	elf_section_t* section = cnull;

	rt_fseek(fd, e_shoff, RT_FILE_SEEK_SET);
	rt_fread(fd, &shdr, sizeof(Elf32_Shdr));

	rt_fseek(fd, shdr.sh_offset, RT_FILE_SEEK_SET);
	buf = (cbyte*)rt_new(shdr.sh_size);
	rt_fread(fd, buf, shdr.sh_size);

	switch(shdr.sh_type){
		case SHT_SYMTAB:
		case SHT_DYNSYM:
			section = (!strtab) ? cnull : new elf_shdr_symtab_t(fd, shdr, buf, (!shstrtab) ? cnull : shstrtab + shdr.sh_name, strtab, shdr_symtab_entry_new);
			break;
		case SHT_REL:
			section = (!strtab) ? cnull : new elf_shdr_symtab_t(fd, shdr, buf, (!shstrtab) ? cnull : shstrtab + shdr.sh_name, strtab, shdr_rel_entry_new);
		case SHT_RELA:
			section = (!strtab) ? cnull : new elf_shdr_symtab_t(fd, shdr, buf, (!shstrtab) ? cnull : shstrtab + shdr.sh_name, strtab, shdr_rela_entry_new);
		default:
			section = new elf_shdr_entry_t(shdr, buf, (!shstrtab) ? cnull : shstrtab + shdr.sh_name);
			break;
	}
	return section;
}
