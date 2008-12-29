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

#ifndef __ELF_STACK_H__
#define __ELF_STACK_H__

#define ELF_STACK_DEPTH	16
class elf_stack{
	protected:
		unsigned int index;
		unsigned int iter;
		elf_section_t* stack[ELF_STACK_DEPTH];
	public:
		elf_stack();
		void push(elf_section_t* section);
		void pop(void);
		void empty(void);
		elf_section_t* root(void);
		operator elf_section_t* (void);
		elf_section_t* first(void);
		elf_section_t* next(void);
};
#endif //__ELF_STACK_H__
