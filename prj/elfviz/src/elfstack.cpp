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

#include "elfio/elfio.h"
#include "elfstack.h"

elf_stack::elf_stack():index(0){}

void elf_stack::push(elf_section_t* section){
	if(index == ELF_STACK_DEPTH) throw("stack overflow");
	stack[index++] = section;
}

void elf_stack::pop(void){
	if(index <= 0) throw("stack underflow");
	index--;
}

elf_section_t* elf_stack::root(void){
	if(index <= 0) throw("stack empty");
	return stack[0];
}

elf_section_t* elf_stack::first(void){
	if(index <= 0) return 0;
	iter = 0;
	return stack[iter++];
}

elf_section_t* elf_stack::next(void){
	if (iter >= index) return 0;
	return stack[iter++];
}

elf_stack::operator elf_section_t* (void){
	if(index <= 0) throw("stack empty");
	return stack[index - 1];
}

void elf_stack::empty(void){ index = 0; }
