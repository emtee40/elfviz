#include "elfio/elfio.h"

elf_buffer_t::elf_buffer_t(const unsigned char* buf, const unsigned sz, const unsigned int off):
	size(sz),
	offset(off),
	buffer(buf)
{}

elf_buffer_t::~elf_buffer_t(){
	if(buffer) delete buffer;
}
