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

#ifndef __ELF_TYPES_H__
#define __ELF_TYPES_H__

typedef unsigned long	Elf32_Addr;
typedef unsigned short	Elf32_Half;
typedef unsigned long	Elf32_Off;
typedef signed long	Elf32_Sword;
typedef unsigned long	Elf32_Word;

/** @brief This member identifies the object file type*/
enum E_type{
	ET_NONE = 0,		/**<No file type*/
	ET_REL,			/**<Re-locatable file*/
	ET_EXEC,		/**<Executable file*/
	ET_DYN,			/**<Shared object file*/
	ET_CORE,		/**<Core file*/
	ET_LOPROC = 0xff00,	/**<Processor-specific*/
	ET_HIPROC = 0xffff	/**<Processor-specific*/
};

/** @brief This member¨s value specifies the required architecture for an individual file.*/
enum E_machine{
	EM_NONE = 0,		/**<No machine*/
	EM_M32,			/**<AT&T WE 32100*/
	EM_SPARC,		/**<SPARC*/
	EM_386,			/**<Intel Architecture*/
	EM_68K,			/**<Motorola 68000*/
	EM_88K,			/**<Motorola 88000*/
	EM_860 = 7,		/**<Intel 80860*/
	EM_MIPS,		/**<MIPS RS3000 Big-Endian*/
	EM_MIPS_RS4_BE = 10,	/**<MIPS RS4000 Big-Endian*/
	EM_ARM = 40		/**<ARM/Thumb Architecture*/
};

/** @brief This member identifies the object file version.*/
enum E_version{
	EV_NONE = 0,	/**<Invalid version*/
	EV_CURRENT = 1	/**<Current version*/
};

enum E_ident_idx{
	EI_MAG0 = 0,	/**<File identification*/
	EI_MAG1,	/**<File identification*/
	EI_MAG2,	/**<File identification*/
	EI_MAG3,	/**<File identification*/
	EI_CLASS,	/**<File class*/
	EI_DATA,	/**<Data encoding*/
	EI_VERSION,	/**<File version*/
	EI_PAD,		/**<Start of padding bytes*/
	EI_NIDENT = 16	/**<Size of e_ident[]*/
};

/** @brief The next byte, e_ident[EI_CLASS], identifies the file¨s class, or capacity.*/
enum EI_CLASS{
	ELFCLASSNONE = 0,	/**<Invalid class*/
	ELFCLASS32,		/**<32-bit objects*/
	ELFCLASS64,		/**<64-bit objects*/
};

/** 
 * @brief Byte e_ident[EI_DATA]specifies the data encoding of all data1 in the object file. 
 * The following encodings are currently defined.
 */
enum EI_DATA{
	ELFDATANONE = 0,	/**<Invalid data encoding*/
	ELFDATA2LSB,		/**<See Data encodings ELFDATA2LSB, below*/
	ELFDATA2MSB,		/**<See Data encodings ELFDATA2MSB, below*/
};
/**
 * @brief arm specific e_flags
 */
/** @brief e_entry contains a program-loader entry point
 * (see section 4.1.1, Entry points, below).
 */
#define EF_ARM_HASENTRY (0x02)

/** @brief Each subsection of the symbol table is sorted by symbol value
 * (see section 4.4.8, Symbol table order, below)
 */
#define EF_ARM_SYMSARESORTED (0x04)

/** @brief Symbols in dynamic symbol tables that are defined in sections included in program segment n have st_shndx = n + 1.
 * (see section 4.4.9, Dynamic symbol table entries, below).
 */
#define EF_ARM_DYNSYMSUSESEGIDX (0x8)

/** @brief Mapping symbols precede other local symbols in the symbol table
 * (see section 4.4.8, Symbol table order, below).
 */
#define EF_ARM_MAPSYMSFIRST (0x10)

/** @brief (current version is 0x02000000) This masks an 8-bit version number,
 * the version of the ARM EABI to which this ELF file conforms.
 * This EABI is version 2. A value of 0 denotes unknown conformance.
 */
#define EF_ARM_EABIMASK (0xFF000000)

#define SHN_UNDEF	0
#define SHN_LORESERVE	0xff00
#define SHN_LOPROC	0xff00
#define SHN_HIPROC	0xff1f
#define SHN_ABS		0xfff1
#define SHN_COMMON	0xfff2
#define SHN_HIRESERVE	0xffff

/** @brief ELF Header*/
typedef struct {
	unsigned char e_ident[EI_NIDENT];	
	Elf32_Half e_type;
	Elf32_Half e_machine;
	Elf32_Word e_version;
	Elf32_Addr e_entry;
	Elf32_Off e_phoff;
	Elf32_Off e_shoff;
	Elf32_Word e_flags;
	Elf32_Half e_ehsize;
	Elf32_Half e_phentsize;
	Elf32_Half e_phnum;
	Elf32_Half e_shentsize;
	Elf32_Half e_shnum;
	Elf32_Half e_shstrndx;
} Elf32_Ehdr;

/**
 * @brief This value marks a section header that does not have an associatedsection.
 * Other members of the section header have undefined values
 */
#define SHT_NULL	0 

/** @biref The section holds information defined by the program, whose format and meaning are determined solely by the program.*/
#define SHT_PROGBITS	1

/** @biref The section holds a symbol table.*/
#define SHT_SYMTAB	2

/** @brief The section holds a string table.*/
#define SHT_STRTAB	3

/**
 * @brief
 * The section holds relocation entries with explicit addends, such as type Elf32_Rela for the 32-bit class of object files.
 * An object file may have multiple relocation sections. See Relocation below for details.
 */
#define SHT_RELA	4

/** @brief The section holds a symbol hash table.*/
#define SHT_HASH	5

/** @brief The section holds information for dynamic linking.*/
#define SHT_DYNAMIC	6

/** @brief This section holds information that marks the file in some way.*/
#define SHT_NOTE	7

/**
 * @biref
 * A section of this type occupies no space in the file but otherwise resembles SHT_PROGBITS.
 * Although this section contains no bytes, the sh_offset member contains the conceptual file offset.
 */
#define SHT_NOBITS	8

/**
 * @breif
 * The section holds relocation entries without explicit addends, such as type Elf32_Rel for the 32-bit class of object files.
 * An object file may have multiple relocation sections. See Relocation below for details.
 */
#define SHT_REL		9

/** @brief This section type is reserved but has unspecified semantics.*/
#define SHT_SHLIB	10

/** @brief The section holds a symbol table.*/
#define SHT_DYNSYM	11

/** @brief Values in this inclusive range are reserved for processor-specific semantics.*/
#define SHT_LOPROC	0x70000000
#define SHT_HIPROC	0x7fffffff

/**
 * @brief
 * Values in this inclusive range are reserved for application programs.
 * Types between SHT_LOUSER and SHT_HIUSER may be used by an application,
 * without conflicting with current or future system-defined section types.
 */
#define SHT_LOUSER	0x80000000
#define SHT_HIUSER	0xffffffff

/** @biref The section contains data that should be writable during process execution*/
#define SHF_WRITE	0x1

/**
 * @breif
 * The section occupies memory during process execution.
 * Some control sections do not reside in the memory image of an object file;
 * this attribute is off for those sections
 */
#define SHF_ALLOC	0x2

/** @brief The section contains executable machine instructions.*/
#define SHF_EXECINSTR	0x4

/** @biref Bits in this mask are reserved for processor-specific semantics.*/
#define SHF_MASKPROC	0xf0000000

typedef struct {
	Elf32_Word sh_name;
	Elf32_Word sh_type;
	Elf32_Word sh_flags;
	Elf32_Addr sh_addr;
	Elf32_Off sh_offset;
	Elf32_Word sh_size;
	Elf32_Word sh_link;
	Elf32_Word sh_info;
	Elf32_Word sh_addralign;
	Elf32_Word sh_entsize;
} Elf32_Shdr;

#define ELF32_ST_BIND(i)	((i)>>4)
#define ELF32_ST_TYPE(i)	((i)&0xf)
#define ELF32_ST_INFO(b,t)	(((b)<<4)+((t)&0xf))

/** @brief Symbol Types, ELF32_ST_TYPE*/
#define STB_LOCAL	0
#define STB_GLOBAL	1
#define STB_WEAK	2
#define STB_LOPROC	13
#define STB_HIPROC	15

/** @brief Symbol Types, ELF32_ST_TYPE*/
/** @brief The symbol's type is not specified.*/
#define STT_NOTYPE	0

/** @brief The symbol is associated with a data object, such as a variable, an array, and so on.*/
#define STT_OBJECT	1

/** @brief The symbol is associated with a function or other executable code.*/
#define STT_FUNC	2

/**
 * @brief
 * The symbol is associated with a section.
 * Symbol table entries of this type exist primarily for relocation and normally have STB_LOCAL binding.
 */
#define STT_SECTION	3

/**
 * @brief
 * A file symbol has STB_LOCAL binding, its section index is SHN_A BS,
 * and it precedes the other STB_LOCAL symbols for the file, if it is present.
 */
#define STT_FILE	4

/**
 * @brief Values in this inclusive range are reserved for processor-specific semantics.
 * If a symbol's value refers to a specific location within a section, its section index member, st_shndx, holds an index into the section header table.
 * As the section moves during relocation, the symbol's value changes as well,
 * and references to the symbol continue to point to the same location in the program.
 * Some special section index values give other semantics.
 */
#define STT_LOPROC	13
#define STT_HIPROC	15

typedef struct {
	Elf32_Word st_name;
	Elf32_Addr st_value;
	Elf32_Word st_size;
	unsigned char st_info;
	unsigned char st_other;
	Elf32_Half st_shndx;
} Elf32_Sym;

enum R_TYPE{
	R_ARM_NONE = 0, ///<Any No relocation.  Encodes dependencies between sections.
	R_ARM_PC24, ///<ARM B/BL S ? P + A
	R_ARM_ABS32, ///<32-bit word S + A
	R_ARM_REL32, ///<32-bit word S ? P + A
	R_ARM_PC13, ///<ARM LDR r, [pc,・] S ? P + A
	R_ARM_ABS16, ///<16-bit half-word S + A
	R_ARM_ABS12, ///<ARM LDR/STR S + A
	R_ARM_THM_ABS5, ///<Thumb LDR/STR S + A
	R_ARM_ABS8, ///<8-bit byte S + A
	R_ARM_SBREL32, ///<32-bit word S ? B + A
	R_ARM_THM_PC22, ///<Thumb BL pair S ? P+ A
	R_ARM_THM_PC8, ///<Thumb LDR r, [pc,・] S ? P + A
	R_ARM_AMP_VCALL9, ///<AMP VCALL Obsolete?SA-1500 only.
	R_ARM_SWI24, ///<ARM SWI S + A
	R_ARM_THM_SWI8, ///<Thumb SWI S + A
	R_ARM_XPC25, ///<ARM BLX S ? P+ A
	R_ARM_THM_XPC22, ///<Thumb BLX pair S ? P+ A
	///<17-19 Reserved to ARM LINUX
	R_ARM_COPY = 20,///<32 bit word Copy symbol at dynamic link time.
	R_ARM_GLOB_DAT,///< 32 bit word Create GOT entry.
	R_ARM_JUMP_SLOT,///< 32 bit word Create PLT entry.
	R_ARM_RELATIVE,///< 32 bit word Adjust by program base.
	R_ARM_GOTOFF,///< 32 bit word Offset relative to start of GOT.
	R_ARM_GOTPC,///< 32 bit word Insert address of GOT.
	R_ARM_GOT32,///< 32 bit word Entry in GOT.
	R_ARM_PLT32,///< ARM BL Entry in PLT.
	///<28-31 Reserved to ARM LINUX
	R_ARM_ALU_PCREL_7_0 = 32, ///<ARM ADD/SUB (S ? P + A) & 0x000000FF
	R_ARM_ALU_PCREL_15_8,///< ARM ADD/SUB (S ? P + A) & 0x0000FF00
	R_ARM_ALU_PCREL_23_15,///< ARM ADD/SUB (S ? P + A) & 0x00FF0000
	R_ARM_LDR_SBREL_11_0,///< ARM LDR/STR (S ? B + A) & 0x00000FFF
	R_ARM_ALU_SBREL_19_12,///< ARM ADD/SUB (S ? B + A) & 0x000FF000
	R_ARM_ALU_SBREL_27_20,///< ARM ADD/SUB (S ? B + A) & 0x0FF00000
	///<38-95 Reserved to ARM
	///<96-99 Reserved to ARM g++
	R_ARM_GNU_VTENTRY = 100, ///<32 bit word Record C++ vtable entry.
	R_ARM_GNU_VTINHERIT,///<32 bit word Record C++ member usage.
	R_ARM_THM_PC11,///< Thumb B S ? P + A
	R_ARM_THM_PC9,///< Thumb B<cond> S ? P + A
	///<104-111 Reserved to ARM g++
	///<112-127 To be defined To be defined Reserved for private experiments?These values will never clash with relocations defined by ARM, but will always clash with other private experiments.
	///<128-248 Reserved to ARM
	R_ARM_RXPC25 = 249, ///<ARM BLX (ツS ? ツP) + A For calls between program segments.
	R_ARM_RSBREL32,///< Word (ツS ? ツSB) + A For an offset from SB, the static base.
	R_ARM_THM_RPC22,///< Thumb BL/BLX pair (ツS ? ツP) + A For calls between program segments.
	R_ARM_RREL32,///< Word (ツS ? ツP) + A For on offset between two segments.
	R_ARM_RABS32,///< Word ツS + A For the address of a location in the target segment.
	R_ARM_RPC24,///< ARM B/BL (ツS ? ツP) + A For calls between program segments.
	R_ARM_RBASE///< None None?Identifies the segment being relocated by the following relocation directives.
};

#define ELF32_R_SYM(i) ((i)>>8)
#define ELF32_R_TYPE(i) ((unsigned char)(i))
#define ELF32_R_INFO(s,t) (((s)<<8)+(unsigned char)(t))

typedef struct {
	Elf32_Addr r_offset;
	Elf32_Word r_info;
} Elf32_Rel;

typedef struct {
	Elf32_Addr r_offset;
	Elf32_Word r_info;
	Elf32_Sword r_addend;
} Elf32_Rela;

/**
 * @brief
 * The array element is unused; other members' values are undefined.
 * This type lets the program header table have ignored entries.
 */
#define PT_NULL		0

/**
 * @brief
 * The array element specifies a loadable segment,
 * described by p_filesz and p_memsz (for additional explanation, see PT_LOAD below).
 */
#define PT_LOAD		1

/** @brief The array element specifies dynamic linking information. See subsection 4.7.*/
#define PT_DYNAMIC	2

/** @brief The array element specifies the location and size of a null-terminated path name to invoke as an interpreter.*/
#define PT_INTERP	3

/** @brief The array element specifies the location and size of auxiliary information.*/
#define PT_NOTE		4

/** @brief This segment type is reserved but has unspecified semantics.*/
#define PT_SHLIB	5

/**
 * @brief
 * The array element, if present,
 * specifies the location and size of the program header table itself
 * (for additional explanation, see PT_ PHDR below).
 */
#define PT_PHDR		6

/** @brief Values in this inclusive range are reserved for processor-specific semantics.*/
#define PT_LOPROC	0x70000000
#define PT_HIPROC	0x7fffffff

/** @brief The segment may be executed.*/
#define PF_X		1

/** @brief The segment may be written to.*/
#define PF_W		2

/** @brief The segment may be read.*/
#define PF_R		4

/** @brief Reserved for processor-specific purposes (see 4.6, Program headers).*/
#define PF_MASKPROC	0xf0000000
/** @brief The segment contains the location addressed by the static base.*/
#define PF_ARM_SB	0x10000000
/** @brief The segment is position-independent.*/
#define PF_ARM_PI	0x20000000
/** @brief The segment contains the entry point.*/
#define PF_ARM_ENTRY	0x80000000

typedef struct {
	Elf32_Word p_type;
	Elf32_Off p_offset;
	Elf32_Addr p_vaddr;
	Elf32_Addr p_paddr;
	Elf32_Word p_filesz;
	Elf32_Word p_memsz;
	Elf32_Word p_flags;
	Elf32_Word p_align;
} Elf32_Phdr;

/** @brief Dynamic section tags*/
enum E_DT{
	DT_NULL,		/**<Ignored. This entry marks the end of the dynamic array. mandatory*/
	DT_NEEDED,		/**<Index in the string table of the name of a needed library. multiple*/
	DT_PLTRELSZ,		/**<These entries are unused by versions 1-2 of the ARM EABI. unused*/
	DT_PLTGOT,
	DT_HASH, 		/**<The offset of the hash table section in the dynamic segment. mandatory*/
	DT_STRTAB, 		/**<The offset of the string table section in the dynamic segment. mandatory*/
	DT_SYMTAB, 		/**<The offset of the symbol table section in the dynamic segment. mandatory*/
	DT_RELA,		/**<The offset in the dynamic segment of an SHT_RELA relocation section,
				  Its byte size, and the byte size of an ARM RELA-type relocation entry. optional*/
	DT_RELASZ,
	DT_RELAENT,
	DT_STRSZ, 		/**<The byte size of the string table section. mandatory*/
	DT_SYMENT, 		/**<The byte size of an ARM symbol table entry?16. mandatory*/
	DT_INIT,		/**<These entries are unused by versions 1-2 of the ARM EABI. unused*/
	DT_FINI,
	DT_SONAME,		/**<The Index in the string table of the name of this shared object. mandatory*/
	DT_RPATH,		/**<Unused by the ARM EABI. unused*/
	DT_SYMBOLIC,
	DT_REL,			/**<The offset in the dynamic segment of an SHT_REL relocation section,
				  Its byte size, and the byte size of an ARM REL-type relocation entry optional*/
	DT_RELSZ,
	DT_RELENT,
	DT_PLTREL,		/**<These entries are unused by versions 1-2 of the ARM EABI. unused*/
	DT_DEBUG,
	DT_TEXTREL,
	DT_JMPREL,
	DT_BIND_NOW,
	DT_LOPROC = 0x70000000,	/**<Values in this range are reserved to the ARM EABI. unused*/
	DT_HIPROC = 0x7fffffff 
};

struct Elf32_HashTable {
	Elf32_Word nBuckets;
	Elf32_Word nChains;
	Elf32_Word* bucket;
	Elf32_Word* chain;
};

#endif //__ELF_TYPES_H__
