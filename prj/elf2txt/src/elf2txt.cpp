/* -------------------------------------------------------------------------
 *
 * elf2txt, ELF to Text converter
 * ------------------------------------------
 *
 * Copyright (C) 2009 Song-Hwan Kim
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

#include <string.h>
#include "ucrt/ucrt.h"
#include "elfio/elfio.h"
#include "argument.h"

typedef enum etTitleType{
	TITLE_NAME,
	TITLE_CATEGORY
}etTitleType;

template <class T> void run(elf_section_t* elfio, T& writer, etState& state, etTitleType title){
	int i = 0;
	switch(title){
		case TITLE_CATEGORY:
			writer.element_open(elfio->category());
			break;
		case TITLE_NAME:
			writer.element_open(elfio->name());
			break;
	}
	if(state.get_flag() & ET_SHOW_ATTR){
		elf_attr_t* attr = elfio->get_attr();
		if(attr){
			for(i = 0 ; i < attr->get_num() ; i++){
				int type = attr->get_type(i);
				if(type & ELF_TYPE_STR)	{
					bool bval = type & ELF_TYPE_INT;
					int ival = (bval) ? attr->get_int(i) : 0;
					writer.attr(attr->get_name(i), attr->get_str(i), bval, ival);
				} else if(type & ELF_TYPE_HEX)	{
					writer.attr(attr->get_name(i), attr->get_int(i), 16);
				} else {
					writer.attr(attr->get_name(i), attr->get_int(i));
				}
			}
		}
	}
	bool has_body = false;
	if(state.get_flag() & ET_SHOW_BODY){
		elf_buffer_t* buf = elfio->get_body();
		if(buf){
			writer.element_close(true);
			has_body = true;
			writer.dump(buf->buffer, buf->size, buf->offset);
		}
	}
	int num = elfio->get_child_num();
	if(num){
		if(!has_body) writer.element_close(true);
		for(i = 0 ; i < num ; i++) run(elfio->get_child(i), writer, state, title);
	} else {
		if(!has_body) writer.element_close(false);
	}
	if(num || has_body) writer.wrap_up();
}

int main( int argc, char * argv[] ) {
	etState state;

	if (argc < 2) {
		amHelp m((char*)"");
		m.doit();
		return 0;
	} else if(argc == 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))){
		amHelp m((char*)"");
		m.doit();
		return 0;
	} else {
		rtArgument parse;

		amHelp help_short((char*)"-h");		parse.add_method(&help_short);
		amHelp help_long((char*)"--help");	parse.add_method(&help_long);
		amElf srcm((char*)"", state);		parse.add_method(&srcm);

		amTxt code_short((char*)"-o", state);		parse.add_method(&code_short);
		amTxt code_long((char*)"--output", state);	parse.add_method(&code_long);

		amFormat format_short((char*)"-f", state);	parse.add_method(&format_short);
		amFormat format_long((char*)"--format", state);	parse.add_method(&format_long);

		amFlagAttr flag_attr_short((char*)"-oa", state);		parse.add_method(&flag_attr_short);
		amFlagAttr flag_attr_long((char*)"--omit-attributes", state);	parse.add_method(&flag_attr_long);

		amFlagBody flag_body_short((char*)"-od", state);	parse.add_method(&flag_body_short);
		amFlagBody flag_body_long((char*)"--omit-data", state);	parse.add_method(&flag_body_long);

		parse.parse(argc, argv);
	}

	elf_section_t* elfio = elfio_new(state.get_elf_file());
	rtFile file(state.get_txt_file(), "w");
	try{
		switch(state.get_format()){
			case ET_OUT_FORMAT_XML:
				{
					rtXmlWriter writer(file);
					run<rtXmlWriter>(elfio, writer, state, TITLE_CATEGORY);
				}
				break;
			case ET_OUT_FORMAT_TXT:
				{
					rtTxtWriter writer(file);
					run<rtTxtWriter>(elfio, writer, state, TITLE_NAME);
				}
				break;
		}
	} catch (char* e){
		fprintf(stderr, "%s\n", e);
	}
	delete elfio;
	return 0;
}

