#include "elfio/elfio.h"

elf_buffer_t::elf_buffer_t(const unsigned char* buf, const unsigned sz, const unsigned int off):
	buffer(buf),
	size(sz),
	offset(off)
{}

elf_buffer_t::~elf_buffer_t(){
	if(buffer) delete (void*) buffer;
}
