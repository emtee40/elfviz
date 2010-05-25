#include "elfio/elfio.h"

elfBuffer::elfBuffer(const unsigned char* buf, const unsigned sz, const unsigned int off):
	size(sz),
	offset(off),
	buffer(buf)
{}

elfBuffer::~elfBuffer(){
	if(buffer) delete (void*)buffer;
}
