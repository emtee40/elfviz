#ifndef __ARM_ELF_TYPES_H__
#define __ARM_ELF_TYPES_H__

/**
 * @brief The ELF file contains BE-8 code, suitable for execution on an ARM Architecture v6 processor.
 * This flag must only be set on an executable file.
 */
#define EF_ARM_BE8	0x00800000

/**
 * @brief This field shall be zero unless the file uses objects that have flags which have OS-specific meanings
 * (for example, it makes use of a section index in the range SHN_LOOS through SHN_HIOS).
 */
enum EI_OSABI {
	ELFOSABI_ARM_AEABI = 64 /**<The object contains symbol versioning extensions*/
};
/**
 * @brief The defined processor-specific section types are listed in Table 4-4, Processor specific section types.
 * All other processor-specific values are reserved to future revisions of this specification
 */
#define SHT_ARM_EXIDX 0x70000001 /**<Exception Index table*/
#define SHT_ARM_PREEMPTMAP 0x70000002 /**<BPABI DLL dynamic linking pre-emption map*/
#define SHT_ARM_ATTRIBUTES 0x70000003 /**<Object file compatibility attributes*/
#define SHT_ARM_DEBUGOVERLAY 0x70000004 /**<See DBGOVL for details*/
#define SHT_ARM_OVERLAYSECTION 0x70000005 /**<See DBGOVL for details*/

/** Program Header */
/** @brief The Program Header provides a number of fields that assist in interpretation of the file.
 * Most of these are specified in the base standard. The following fields have ARM-specific meanings.
 */
#define PT_ARM_ARCHEXT 0x70000000 /**<Platform architecture compatibility information*/
#define PT_ARM_EXIDX 0x70000001 /**< Exception unwind tables */
#define PT_ARM_UNWIND 0x70000001 /**< Exception unwind tables */

#endif //__ARM_ELF_TYPES_H__
