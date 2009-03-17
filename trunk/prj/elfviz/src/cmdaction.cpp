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
//#include <stdlib.h>	//FIXME:it makes error. stdlib crashes with g++ when compiled with -O2 option
#include <string.h>
#include <ctype.h>

#include "elfio/elfio.h"
#include "cmdparam.h"
#include "elfstack.h"
#include "cmdaction.h"

class cmdaction_dummy : public cmdaction{
	public:
		virtual void act(cmdparam& param, elf_stack& elfstack){
			printf("unknown command");
		}
};

class cmdaction_help : public cmdaction{
	public:
		virtual void act(cmdparam& param, elf_stack& elfstack){
			printf("open {file_name}\n\topen elf file\n");
			printf("file\treturns file name of opened elf file\n");
			printf("close\tclose elf file\n");
			printf("ls [node_name] [-a]\n\tlists sub nodes by name\n");
			printf("\tif node_name presents, lists node_name only\n");
			printf("\t\t(wildcard is not allowed yet)\n");
			printf("\tif -a option presents, shows headers also\n");
			printf("inf node_name\tshow header\n");
			printf("cat node_name\tshow body\n");
			printf("cd node_name\tchange current node to sub node\n");
			printf("\tif sub node doesn't have child nodes, cd fails\n");
			printf("help\tthis help screen\n");
		}
};

class cmdaction_open : public cmdaction{
	public:
		virtual void act(cmdparam& param, elf_stack& elfstack){
			char* argv = param.argv(0);
			elf_section_t* elfio = elfio_new(argv);
			if(elfio) {
				printf("%s is opened\n", argv);
				elfstack.push(elfio);
			} else {
				printf(" %s is not opened\n", argv);
			}
		}
};

class cmdaction_close : public cmdaction{
	public:
		virtual void act(cmdparam& param, elf_stack& elfstack){
			elf_section_t* elfio = elfstack.root();
			if(elfio) {
				printf("%s is closed\n", elfio->name());
				delete elfio;
				elfstack.empty();
			} else {
				printf("no file is opened\n");
			}
		}
};

class cmdaction_file : public cmdaction{
	public:
		virtual void act(cmdparam& param, elf_stack& elfstack){
			elf_section_t* elfio = elfstack.root();
			printf("%s\n", (elfio) ? elfio->name() : "(null)");
		}
};

class cmdaction_ls : public cmdaction{
	private:
		void format_child(elf_section_t* e){
			elf_section_t* child = 0;
			printf(".\t..\t");
			for(int i = 0 ; i < e->get_child_num() ; i++){
				 child = e->get_child(i);
				if(child) printf("%s\t", child->name());
			}
		}

	public:
		virtual void act(cmdparam& param, elf_stack& elfstack){
			elf_section_t* elfio = elfstack;
			if(param == 0) {
				format_child(elfio);
			} else {
				char* argv = param.argv(0);
				if(!strcmp(argv, ".")){
					format_child(elfio);
				} else {
					format_child(elfio->get_child(argv));
				}
			}
		}
};

class cmdaction_inf : public cmdaction{
	void format_header(elf_section_t* e){
		elf_attr_t* attr = e->get_attr();
		if(!attr) return;
		for(int i = 0 ; i < attr->get_num() ; i++){
			int type = attr->get_type(i);
			if(type & ELF_TYPE_STR)	printf("%s=%s\n", attr->get_name(i), attr->get_str(i));
			else			printf("%s=%d\n", attr->get_name(i), attr->get_int(i));
		}
	}

	public:
		virtual void act(cmdparam& param, elf_stack& elfstack){
			elf_section_t* elfio = elfstack;
			elf_section_t* current = 0;
			char* argv = param.argv(0);
			if(!strcmp(argv, ".")) current = elfio;
			else current = elfio->get_child(argv);
			if(current) format_header(current);
		}
};

class cmdaction_cat : public cmdaction{
	#define SHDR_COLUMN_SIZE 16
	void format_body(elf_section_t* e){
		elf_buffer_t* buf = e->get_body();
		if(!buf) return;
		for(unsigned int i = 0 ; i < buf->size ; i += SHDR_COLUMN_SIZE){
			int j = 0;
			int mx = buf->size - i;
			if(mx > SHDR_COLUMN_SIZE) mx = SHDR_COLUMN_SIZE;
			printf("%08x: ", (unsigned int)(buf->offset + i));
			for(j = 0 ; j < mx ; j++) {
				printf("%02x ", buf->buffer[i + j]);
				if(j == 7)	printf(" ");
			}
			if(mx < SHDR_COLUMN_SIZE) {
				for(j = 0 ; j < SHDR_COLUMN_SIZE - mx ; j++){
					printf("   ");
				}
				if(mx < 7) printf(" ");
			}
			printf("  ");
			for(j = 0 ; j < mx ; j++) printf("%c", (isalnum(buf->buffer[i + j])) ? buf->buffer[i + j] : '.');
			printf("\n");
		}
	}

	public:
		virtual void act(cmdparam& param, elf_stack& elfstack){
			elf_section_t* elfio = elfstack;
			elf_section_t* current = 0;
			char* argv = param.argv(0);
			if(!strcmp(argv, ".")) current = elfio;
			else current = elfio->get_child(argv);
			if(current) format_body(current);
		}
};

class cmdaction_cd : public cmdaction{
	public:
		virtual void act(cmdparam& param, elf_stack& elfstack){
			elf_section_t* elfio = elfstack;
			elf_section_t* current = 0;
			char* argv = param.argv(0);
			if(!strcmp(argv, ".")) {
				return;
			} else if(!strcmp(argv, "..")) {
				elfstack.pop();
			} else {
				current = elfio->get_child(argv);
			}
			if(current) elfstack.push(current);
		}
};

static cmdaction_dummy dummy_act;
static cmdaction_help help_act;
static cmdaction_open open_act;
static cmdaction_close close_act;
static cmdaction_file file_act;
static cmdaction_ls ls_act;
static cmdaction_inf inf_act;
static cmdaction_cat cat_act;
static cmdaction_cd cd_act;

cmdaction* get_action(char* command){
	cmdaction* action_act = &dummy_act;
	if(command){
		if(!strcmp(command, "help")) action_act = (cmdaction*)&help_act;
		else if(!strcmp(command, "open")) action_act = (cmdaction*)&open_act;
		else if(!strcmp(command, "close")) action_act = (cmdaction*)&close_act;
		else if(!strcmp(command, "file")) action_act = (cmdaction*)&file_act;
		else if(!strcmp(command, "ls")) action_act = (cmdaction*)&ls_act;
		else if(!strcmp(command, "inf")) action_act = (cmdaction*)&inf_act;
		else if(!strcmp(command, "cat")) action_act = (cmdaction*)&cat_act;
		else if(!strcmp(command, "cd")) action_act = (cmdaction*)&cd_act;
	}
	return action_act;
}
